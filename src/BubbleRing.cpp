//////////////////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

#include "ShotBase.h"
#include "NormalShot.h"
#include "ShotManager.h"

#include "BossBase.h"

#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "EnemyManager.h"
#include "UI.h"
#include "GameMain.h"

#include "BubbleRing.h"

//////////////////////////////////////////////////
// ���O��Ԃ̏ȗ�
//////////////////////////////////////////////////
using namespace Shot;

//////////////////////////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////////////////////////
const float		CBubbleRing::POWER_MAX			= 5.f;
const float		CBubbleRing::POWER_MIN			= 0.5f;
const float		CBubbleRing::SPEED_MAX			= 1.5f;
const float		CBubbleRing::BIG_SPEED			= 0.2f;
const BYTE		CBubbleRing::POINT_NUM			= 20;
const USHORT	CBubbleRing::BUBBLE_ERASE_TIME	= 1000;
const LPSTR		CBubbleRing::MESH_PASS			= "BubbleRing.x";

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �R���X�g���N�^
CBubbleRing::CBubbleRing(): 
m_CreateBubbleNum( 0 ),
m_Power( BIG_SPEED ),
m_Speed( 1.f ),
m_StartTime( timeGetTime() )
{
	// ���b�V���f�[�^�l��
	m_lpMesh = CSXfileManager::GetInst()->GetData( MESH_PASS );

	m_Points.SetScaleMax( 4.f );
}

CBubbleRing::CBubbleRing( const D3DXMATRIX* mat, const float power ):
CBase( mat ), 
m_CreateBubbleNum( 0 ),
m_Power( BIG_SPEED ),
m_Speed( 1.f ),
m_RingMax( (power < POWER_MIN)? POWER_MIN : power ),
m_StartTime( timeGetTime() )
{
	// ���b�V���f�[�^�l��
	m_lpMesh = CSXfileManager::GetInst()->GetData( MESH_PASS );

	m_Points.SetScaleMax( 4.f );
}

// �X�V
void CBubbleRing::Update()
{
	if( m_Flg & FLG_BREAK )
	{
		// �����X�V
		UpdateBreak();
	
	}else if( m_Flg & FLG_ALIVE ){

		// ���X�Ɋg��
		if( ( m_Power += BIG_SPEED ) > m_RingMax )
		{
			m_Power = m_RingMax;
		}

		m_mMat.MoveLocal( 0, 0, m_Speed );

		// �A�ǉ�(�t���[���ɂP��)
		if( m_StartTime + m_CreateBubbleNum * CREATE_BUBBLE_TIME < timeGetTime() )
		{
			m_CreateBubbleNum++;

			// �g�听������菜��
			CMatrix mMat( m_mMat.GetPos(  ) );

			m_Bubbles.Add( &mMat, 1 );
		}

		// ��莞�Ԃŏ���
		if( (m_StartTime + ERASE_TIME < timeGetTime())									|| 
			Sequence::CGameMain::SURFACESEA_HEIGHT < m_mMat.GetPos(  ).y	)
		{
			m_Board.SetVertex( 2 );

			float X = static_cast<float>(BREAK_GRAPH_SIZE) / BREAK_GRAPH_WIDTH;

			m_Board.SetTexturePos(
				&D3DXVECTOR2( m_AnimeCnt * X,		0 ),
				&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	0 ),
				&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	1.f ),
				&D3DXVECTOR2( m_AnimeCnt * X,		1.f ) );

			m_Flg &= ~FLG_ALIVE;

			m_Points.SetNum( POINT_NUM );
			m_Points.SetTexture( CSTexManager::GetInst()->GetData( SPRITE_TEX_PASS )->GetTexture() );
			m_Points.SetScaleMax( 4.f );
			m_Points.SetMove( D3DXVECTOR3( 1.f, -0.5f, 1.f ) );
			m_Points.SetPos( m_mMat.GetPos(  ) );
			m_Points.SetEraseTime( BUBBLE_ERASE_TIME );
		}
	}

	// �A�������Ă���폜
	if( !(m_Flg & FLG_ALIVE) && m_Points.IsErase() && m_Bubbles.IsEmpty() )
	{
		m_Flg |= FLG_ERASE;
	}

	// �A
	m_Bubbles.Update();

	// �|�C���g�X�v���C�g
	m_Points.Update();
}

// �`��
void CBubbleRing::Draw( const LPDIRECT3DDEVICE9 lpdevice )
{
	if( m_Flg & FLG_ALIVE && IsCulling( &m_mMat ) )
	{
		if( m_Flg & FLG_ALIVE )
		{
			// �g��s�������ĕ`��̎������K�p
			CMatrix mScale;
			mScale.SetScale( m_Power );

			lpdevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
			lpdevice->SetTransform( D3DTS_WORLD, &( mScale * m_mMat ) );

			lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

			m_lpMesh->Draw( lpdevice );

			lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			lpdevice->SetRenderState( D3DRS_NORMALIZENORMALS, FALSE );
		}
	}

	m_Bubbles.DrawNoBreak( lpdevice );
}

// �G�t�F�N�g�`��
void CBubbleRing::DrawEffect( const LPDIRECT3DDEVICE9 lpdevice )
{
	lpdevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// �A
	if( !(m_Flg & FLG_ALIVE) )
	{
		m_Points.Draw( lpdevice );
	}
	
	m_Bubbles.DrawBreak( lpdevice );

	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}

// ���𑗂����
void CBubbleRing::Dispatch(IDoubleDispatch *object)
{
	object->Action( *this );
}

// �G�ɑ΂���s��
void CBubbleRing::Action( Enemy::CBase& object )
{
	m_Flg &= ~FLG_ALIVE;

	m_Points.SetNum( POINT_NUM );
	m_Points.SetTexture( CSTexManager::GetInst()->GetData( SPRITE_TEX_PASS )->GetTexture() );
	m_Points.SetMove( D3DXVECTOR3( 1.f, -1.f, 1.f ) );
	m_Points.SetScaleMax( 4.f );
	m_Points.SetPos( m_mMat.GetPos() );
	m_Points.SetEraseTime( BUBBLE_ERASE_TIME );
}

// �{�X�ɑ΂���s��
void CBubbleRing::Action(Boss::CBase &object)
{
	if( (object.GetFlg() & Boss::CBase::FLG_ALIVE) && 
		(m_Flg & FLG_ALIVE) )
	{
		m_Flg &= ~FLG_ALIVE;

		m_Points.SetNum( POINT_NUM );
		m_Points.SetTexture( CSTexManager::GetInst()->GetData( SPRITE_TEX_PASS )->GetTexture() );
		m_Points.SetMove( D3DXVECTOR3( 1.f, -1.f, 1.f ) );
		m_Points.SetScaleMax( 4.f );
		m_Points.SetPos( m_mMat.GetPos() );
		m_Points.SetEraseTime( BUBBLE_ERASE_TIME );
	}
}

// �v���C���[�ɑ΂���s��
void CBubbleRing::Action(Player::CDolphin &object)
{
	if( !(m_Flg & FLG_BREAK) )
	{
		m_Board.SetVertex( 2 );

		float X = static_cast<float>(BREAK_GRAPH_SIZE) / BREAK_GRAPH_WIDTH;

		m_Board.SetTexturePos(
			&D3DXVECTOR2( m_AnimeCnt * X,		0 ),
			&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	0 ),
			&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	1.f ),
			&D3DXVECTOR2( m_AnimeCnt * X,		1.f ) );

		m_Flg |= FLG_BREAK;
	}
}

// �e�ɑ΂���s��
void CBubbleRing::Action(Shot::CBase &object)
{
	m_Flg &= ~FLG_ALIVE;
}

// �f�X�g���N�^
CBubbleRing::~CBubbleRing()
{
}