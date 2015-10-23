///////////////////////////////
// �C���N���[�h
///////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

#include "ShotBase.h"
#include "NormalShot.h"
#include "ShotManager.h"

#include "BossBase.h"
#include "EnemyManager.h"

#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "UI.h"
#include "BubbleManager.h"
#include "GameMain.h"

#include "Bubble.h"

///////////////////////////////
// �ÓI�����o���̉�
///////////////////////////////
const LPSTR		CBubble::BUBBLE_MESH_PASS	= "Bubble.x";
const USHORT	CBubble::ERASE_TIME			= 3500;
const BYTE		CBubble::BUBBLE_RAND		= 100;
const float		CBubble::SPEED				= 0.5f / 50;
const float		CBubble::SPEED_MAX			= SPEED * 500;
const float		CBubble::SCALE				= 0.3f;

const int		CBubble::BURST_WIDTH			= 256;
const int		CBubble::BURST_HEIGHT			= 256;
const int		CBubble::BURST_ANIME_SIZE		= 32;
const int		CBubble::BURST_ANIME_WIDTH_NUM	= 8;
const int		CBubble::BURST_ANIME_HEIGHT_NUM	= 8;
const int		CBubble::BURST_ANIME_NUM		= CBubble::BURST_ANIME_WIDTH_NUM * (CBubble::BURST_ANIME_HEIGHT_NUM - 1);

const BYTE		CBubble::ERASE		= 0x01;
const BYTE		CBubble::BREAK		= 0x02;
const BYTE		CBubble::ENEMY_HIT	= 0x04;

///////////////////////////////
// ����
///////////////////////////////

// �R���X�g���N�^
CBubble::CBubble():
m_Flg( 0 ),
m_StartTime( timeGetTime() ),
m_NowSpeed( 0.f ),
m_AnimeCnt( 0 ),
m_lpMesh( CSXfileManager::GetInst()->GetData( BUBBLE_MESH_PASS ) )
{
	// �K���ɂ΂炷
	m_mMat.RotateLocalX( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );
	m_mMat.RotateLocalY( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );
	m_mMat.RotateLocalZ( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );

	// �k������
	m_mMat.ScaleLocal( SCALE ); 

	m_Board.SetVertex( 1.0f );
}

// �f�X�g���N�^
CBubble::~CBubble()
{
}

// �X�V
void CBubble::Update()
{
	// �j�󂳂�Ă���
	if( m_Flg & BREAK )
	{
		// �����X�V
		if( ++m_AnimeCnt >= BURST_ANIME_NUM )
		{
			m_Flg |= ERASE;	// ����
		}

	}else{
		// �A�̕���
		m_NowSpeed += SPEED;

		// ���ۂɓ�����
		m_mMat.MoveLocal( 0, 0, ( m_NowSpeed < SPEED_MAX )? SPEED - m_NowSpeed : 0 );

		// ���͂̕����ɓ�����
		CMatrix Mat( 0, m_NowSpeed, 0 );

		m_mMat = m_mMat * Mat;

		// ��莞�Ԓ�����Δj��
		if( m_StartTime + ERASE_TIME < timeGetTime() )
		{
			m_Flg |= BREAK;

			//m_Points.SetTexture( CSTexManager::GetInst()->GetData( "Sprite2.png" )->GetTexture() );
		}

		// ���ʂɏo��Ώ�����
		if( m_mMat.GetPos().y > Sequence::CGameMain::SURFACESEA_HEIGHT )
		{
			m_Flg |= ERASE;	// ����
		}
	}
}

// �`��
void CBubble::Draw( const LPDIRECT3DDEVICE9 lpdevice )
{
	// �J�����l��
	CCamera Camera = *Sequence::CGameMain::GetCamera();

	// ��ʓ��Ȃ�
	if( IsCulling( &m_mMat ) )
	{
		// �j�󂳂�Ă���
		if( m_Flg & BREAK )
		{
			// �e�N�X�`�����W��ݒ肷��
			m_Board.SetTexturePos(
				&D3DXVECTOR2(
				(m_AnimeCnt % BURST_ANIME_WIDTH_NUM * (float)BURST_ANIME_SIZE) / BURST_WIDTH,
				(m_AnimeCnt / BURST_ANIME_HEIGHT_NUM* (float)BURST_ANIME_SIZE) / BURST_HEIGHT ),

				&D3DXVECTOR2( 
				((m_AnimeCnt % BURST_ANIME_WIDTH_NUM + 1)	* (float)BURST_ANIME_SIZE) / BURST_WIDTH,
				(m_AnimeCnt	 / BURST_ANIME_HEIGHT_NUM		* (float)BURST_ANIME_SIZE) / BURST_HEIGHT ),

				&D3DXVECTOR2(
				((m_AnimeCnt % BURST_ANIME_WIDTH_NUM + 1)	* (float)BURST_ANIME_SIZE) / BURST_WIDTH,
				((m_AnimeCnt / BURST_ANIME_HEIGHT_NUM+ 1)	* (float)BURST_ANIME_SIZE)	/ BURST_HEIGHT ),

				&D3DXVECTOR2( 
				(m_AnimeCnt	 % BURST_ANIME_WIDTH_NUM		* (float)BURST_ANIME_SIZE)	/ BURST_WIDTH,
				((m_AnimeCnt / BURST_ANIME_HEIGHT_NUM + 1)  * (float)BURST_ANIME_SIZE)	/ BURST_HEIGHT )
				);

			// ���C�e�B���O��؂�
			lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );

			CMatrix Mat, mTrans;
			CVector	Pos = m_mMat.GetPos();

			lpdevice->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
			
			Camera.GetBillboard( &Mat, &m_mMat );

			mTrans.SetTrans( Pos.x, Pos.y, Pos.z );

			Mat = Mat * mTrans;
			lpdevice->SetTransform( D3DTS_WORLD, &Mat );

			CTexture* lpTex = CSTexManager::GetInst()->GetData( "Smoke.png" );

			if( lpTex )
			{
				lpdevice->SetTexture( 0, lpTex->GetTexture() );
			}
			lpdevice->SetRenderState( D3DRS_ZENABLE, FALSE );
			lpdevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
			lpdevice->SetFVF( CBoard::FVF_VERTEX );
			
			m_Board.Draw( lpdevice );

			lpdevice->SetRenderState( D3DRS_ZENABLE, TRUE );
			lpdevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
			lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );

		}else{
		
			lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
			m_lpMesh->Draw( lpdevice );
		}
	}
}

// �s���ݒ肷��
void CBubble::SetMatrix( const D3DXMATRIX& mat )
{
	m_mMat = mat;

	// �K���ɂ΂炷
	m_mMat.RotateLocalX( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );
	m_mMat.RotateLocalY( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );
	m_mMat.RotateLocalZ( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );
	m_mMat.ScaleLocal( SCALE );
}

// �G�ɑ΂���s��
void CBubble::Action( Enemy::CBase& object )
{
	m_Flg |= ENEMY_HIT | ERASE;
}