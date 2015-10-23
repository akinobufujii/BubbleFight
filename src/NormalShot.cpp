//////////////////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////////////////
#include "BubbleManager.h"

#include "ShotBase.h"
#include "ShotManager.h"

#include "BossBase.h"

#include "RippleManager.h"
#include "FinTail.h"
#include "StrongDolphin.h"

#include "LimitWalls.h"
#include "GameMain.h"

#include "NormalShot.h"

//////////////////////////////////////////////////
// ���O��Ԃ̏ȗ�
//////////////////////////////////////////////////
using namespace Shot;

//////////////////////////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////////////////////////
const float		CNormal::POWER_MAX			= 5.f;
const float		CNormal::POWER_MIN			= 0.5f;
const float		CNormal::SPEED_MAX			= 1.5f;
const float		CNormal::BUBBLEWRAP_MAX		= 10.f;
const float		CNormal::POWER_SPEED		= 0.5f;
const BYTE		CNormal::POINT_NUM			= 20;
const LPSTR		CNormal::MESH_PASS			= "BigShot.x";
const LPSTR		CNormal::BUBBLE_MESH_PASS	= "Bubble.x";
const USHORT	CNormal::FINHIT_ERASE_TIME	= ERASE_TIME * 2;

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �R���X�g���N�^
CNormal::CNormal():
m_CreateBubbleNum( 0 ),
m_Power( POWER_MIN ),
m_Speed( SPEED_MAX / POWER_MAX * (POWER_MAX + 1 - POWER_MIN) ),
m_lpMesh( CSXfileManager::GetInst()->GetData( MESH_PASS ) ),
m_lpBubbleMesh( CSXfileManager::GetInst()->GetData( BUBBLE_MESH_PASS ) ),
m_NowPower( 0 ),
m_StartTime( timeGetTime() )	// �Ă΂ꂽ���Ԃ��i�[
{
	m_mMat.ScaleLocal( m_Power );

	m_Points.SetScaleMax( 2.f );
}

CNormal::CNormal( const D3DXMATRIX* mat ):
CBase( mat ), 
m_CreateBubbleNum( 0 ),
m_Power( POWER_MIN ),
m_Speed( SPEED_MAX / POWER_MAX * (POWER_MAX + 1 - POWER_MIN) ),
m_lpMesh( CSXfileManager::GetInst()->GetData( MESH_PASS ) ),
m_lpBubbleMesh( CSXfileManager::GetInst()->GetData( BUBBLE_MESH_PASS ) ),
m_NowPower( 0 ), 
m_StartTime( timeGetTime() )	// �Ă΂ꂽ���Ԃ��i�[
{
	m_mMat.ScaleLocal( m_Power );

	m_Points.SetScaleMax( 2.f );
}

// �X�V
void CNormal::Update()
{
	if( m_Flg & FLG_BREAK )
	{
		// �����X�V
		UpdateBreak();
	
	}else if( m_Flg & FLG_ALIVE ){
		
		// �A�ɂ܂�Ă��Ȃ����
		if( !(m_Flg & FLG_BUBBLE_WRAP) )
		{
			// �ړ�
			UpdateMove();
		
		}else if( m_Flg & FLG_FIN_HIT ){

			// �e���ꂽ�ړ�
			UpdateHitByFin();
		}
	}

	// �A�������Ă���폜
	if( !(m_Flg & FLG_ALIVE) && m_Points.IsErase() )
	{
		m_Flg |= FLG_ERASE;
	}

	// �A
	m_Bubbles.Update();

	if( (m_NowPower += POWER_SPEED) > m_Power )
	{
		m_NowPower = m_Power;
	}

	// �|�C���g�X�v���C�g
	m_Points.Update();
}

// �`��
void CNormal::Draw( const LPDIRECT3DDEVICE9 lpdevice )
{
	if( IsCulling( &m_mMat ) )
	{
		if( m_Flg & FLG_BREAK ){

			CCamera Cam = *Sequence::CGameMain::GetCamera();

			CMatrix Rot;
			Cam.GetBillboard( &Rot, &m_mMat );

			CMatrix Trans( m_mMat.GetPos() );
		
		}else if( m_Flg & FLG_ALIVE ){

			// �e�`��
			lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );

			m_lpMesh->Draw( lpdevice );

			// �A�ɂ܂�Ă���΂�����`��
			if( m_Flg & FLG_BUBBLE_WRAP )
			{
				CMatrix mScale = m_mMat;

				// �傫������
				mScale.ScaleLocal( m_NowPower );

				lpdevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
				lpdevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
				lpdevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
				lpdevice->SetTransform( D3DTS_WORLD, &mScale );

				m_lpBubbleMesh->Draw( lpdevice );

				lpdevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
			}
		} 
	}

	// �A
	m_Bubbles.DrawNoBreak( lpdevice );

	lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpdevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	m_Points.Draw( lpdevice );

	lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}

// �ړ��X�V
void CNormal::UpdateMove()
{	
	// �ړ�
	m_mMat.MoveLocal( 0, 0, SPEED_MAX );

	// �A�ǉ�(��t���[���ɂP��)
	if( m_StartTime + m_CreateBubbleNum * CREATE_BUBBLE_TIME < timeGetTime() )
	{
		m_CreateBubbleNum++;

		// �g�听������菜��
		CMatrix mMat( m_mMat.GetPos(  ) );

		m_Bubbles.Add( &mMat, 1 );
	}

	// ��莞�Ԃŏ���
	if( m_StartTime + ERASE_TIME < timeGetTime() )
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
		m_Points.SetScaleMax( 2.f );
		m_Points.SetMove( D3DXVECTOR3( 1.f, 1.f, 1.f ) );
		m_Points.SetPos( m_mMat.GetPos(  ) );
		m_Points.SetEraseTime( 3000 );
	}
}

// �t�B���ɂ������Ă���̍X�V
void CNormal::UpdateHitByFin()
{
	// ��܂�Ă��ăt�B���ɓ���������Ƃ΂�
	m_mMat.MoveLocal( m_vDirection.x, m_vDirection.y, m_vDirection.z );

	// ��莞�Ԃŏ���
	if( (m_StartTime + FINHIT_ERASE_TIME < timeGetTime()) )
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
		m_Points.SetTexture( *CSTexManager::GetInst()->GetData( SPRITE_TEX_PASS ) );
		m_Points.SetScaleMax( 2.f );
		m_Points.SetMove( D3DXVECTOR3( 1.f, 1.f, 1.f ) );
		m_Points.SetPos( m_mMat.GetPos() );
		m_Points.SetEraseTime( 3000 );
	}
}

// ���𑗂����
void CNormal::Dispatch(IDoubleDispatch *object)
{
	object->Action( *this );
}

// �G�ɑ΂���s��
void CNormal::Action( Enemy::CBase& object )
{
	m_Flg &= ~FLG_ALIVE;

	m_Points.SetNum( POINT_NUM );
	m_Points.SetTexture( CSTexManager::GetInst()->GetData( SPRITE_TEX_PASS )->GetTexture() );
	m_Points.SetScaleMax( 2.f );
	m_Points.SetMove( D3DXVECTOR3( 1.f, 1.f, 1.f ) );
	m_Points.SetPos( m_mMat.GetPos() );
	m_Points.SetEraseTime( 3000 );
	//m_Flg |= FLG_ERASE;
}

// �{�X�ɑ΂���s��
void CNormal::Action(Boss::CBase &object)
{
	// �������悤�Ǝv�������A�ђʂ����邽�߂ɉ������Ȃ�
	if( (object.GetFlg() & (Boss::CBase::FLG_ALIVE) ) && 
		(m_Flg & FLG_BUBBLE_WRAP) )
	{
		//m_Flg &= ~FLG_ALIVE;
	}
}

// �v���C���[�ɑ΂���s��
void CNormal::Action(Player::CDolphin &object)
{
	// �������j����A�ɂ܂�Ă��Ȃ����
	if( !(m_Flg & (FLG_BREAK | FLG_BUBBLE_WRAP)) )
	{
		// �j�􏀔�
		m_Points.SetNum( POINT_NUM );
		m_Points.SetTexture( *CSTexManager::GetInst()->GetData( "Sprite.jpg" ) );
		m_Points.SetScaleMax( 2.f );
		m_Points.SetMove( D3DXVECTOR3( 1.f, 1.f, 1.f ) );
		m_Points.SetPos( m_mMat.GetPos() );
		m_Points.SetEraseTime( 3000 );

		m_Flg &= ~FLG_ALIVE;
		m_Flg |= FLG_BREAK;
	}

	// �������A�ɂ܂�Ă��āA�܂��U������Ă��Ȃ����
	if( (m_Flg & FLG_BUBBLE_WRAP) && 
		(object.GetFlg() & Player::CDolphin::FLG_ATTACK_OK) &&
		!(m_Flg & FLG_FIN_HIT) )
	{
		// �g�啪���l�����ă��C����
		CMatrix mScale = m_mMat;

		mScale.ScaleLocal( m_Power );

		float Result = IntersectByMesh( &object.GetMatrix(), &object.GetFinDir(), &mScale, *m_lpMesh->GetMeshData() );
		
		if( Result > 0 )
		{
			// �t�B���ɓ�������
			m_Flg |= FLG_FIN_HIT;

			// ���ł������������肷��
			CVector ScreenVec = Sequence::CGameMain::GetCamera()->GetScreenVector();

			// �t�s������
			CMatrix mInv = m_mMat;
			mInv.GetInverse();
			
			ScreenVec.GetTransNormal( &mInv );
			m_vDirection = ScreenVec;

			// ���Ԃ��L������
			m_StartTime = timeGetTime();
		}
	}
}

// �e�ɑ΂���s��
void CNormal::Action(Shot::CBase &object)
{
	// �A�ɂ܂ꂽ
	m_Flg	|= FLG_BUBBLE_WRAP;

	// ��ޖA�̑傫������
	if( (m_Power += object.GetPower() * 2) > BUBBLEWRAP_MAX )
	{
		m_Power = BUBBLEWRAP_MAX;
	}
}

// �f�X�g���N�^
CNormal::~CNormal()
{
}