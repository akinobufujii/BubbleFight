/////////////////////////////////
// �C���N���[�h
/////////////////////////////////
#include "RippleManager.h"
#include "BubbleManager.h"
#include "ShotManager.h"
#include "EnemyBase.h"
#include "FinTail.h"

#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "EnemyManager.h"
#include "GameMain.h"

#include "DamageObjectBase.h"
#include "FishManager.h"

#include "RushEnemy.h"

/////////////////////////////////
// ���O��ԏȗ�
/////////////////////////////////
using namespace Enemy;

/////////////////////////////////
// �ÓI�����o���̉�
/////////////////////////////////
const ULONG		CRush::SCORE			= 200;
const float		CRush::SPEED			= 1.f;
const float		CRush::TAIL_ROT_MAX		= 80.f;
const float		CRush::TAIL_ROT_SPEED	= CRush::TAIL_ROT_MAX / CRush::CHANGE_MOVE_TIME;
const float		CRush::RETURN_TAIL_SPEED= 5.f;
const USHORT	CRush::CHANGE_MOVE_TIME	= 5000;
const LPSTR		CRush::ENEMY_MESH_PASS	= "Enemy.x";

/////////////////////////////////
// ����
/////////////////////////////////

// �R���X�g���N�^
CRush::CRush( float x, float y, float z ):
m_lpMesh( CSXfileManager::GetInst()->GetData( ENEMY_MESH_PASS ) ),
m_NowTailRot( 0 ),
m_SaveTime( timeGetTime() )
{
	m_Flg |= SEARCH;
	m_mMat.SetTrans( x, y, z );

	// �g��̐ݒ�
	m_Ripples.SetCreateHeight( m_lpMesh->GetRadius() );
	m_Ripples.SetSize(
		m_lpMesh->GetRadius(),
		m_lpMesh->GetRadius() );

	m_Tail.SetFarPos( &D3DXVECTOR3( 0, 0, 0 ) );
}

// �f�X�g���N�^
CRush::~CRush()
{
}

// �X�V
void CRush::Update()
{
	// �j�󂳂�Ă���Ȃ�
	if( m_Flg & BREAK )
	{
		// �����X�V
		if( m_Particle.IsErase() )
		{
			m_Flg |= ERASE;	// ����
		}

		// �p�[�e�B�N���X�V
		m_Particle.Update();

	}else{

		// �ڕW�ɉ�
		// ���������Ă�����
		if( m_lpTargetFish )
		{
			D3DXVECTOR3 Axis;
			float Rot = GetAxisAndRot( &Axis, m_lpTargetFish->GetPos(), m_mMat );

			if( Rot > CBase::DEFAULT_ROTSPEED )
			{
				Rot = CBase::DEFAULT_ROTSPEED;
			}

			m_mMat.RotateLocalAxis( &Axis, Rot );

		}else{

			UpdateRot();
		}

		// �������Ȃ�
		if( m_Flg & SEARCH )
		{
			// �ڕW����܂��Ă��Ȃ��Ȃ�
			if( m_lpTargetFish == NULL )
			{
				// ��������
				SearchTarget();
			}
		
			// �K���̊p�x�����񂾂�ƋȂ��Ă���
			m_NowTailRot = -TAIL_ROT_SPEED * (timeGetTime() - m_SaveTime);

			// �����Â��
			UpdateCollisionMove( &D3DXVECTOR3( 0, 0, -0.1f ), 3 );

		}else{

			// ���͂������Ȃ���Γːi
			if( !(m_Flg & BUBBLE_WRAP) )
			{
				// �����ɓ������ē����Ȃ��Ȃ�
				if( UpdateCollisionMove( &D3DXVECTOR3( 0, 0, SPEED ), 3 ) == false )
				{
					// �s����؂�ւ���
					m_Flg |= SEARCH;

					m_SaveTime = timeGetTime();
				}
				
				m_Bubbles.Add( &m_mMat, 1 );

				D3DXVECTOR3 Pos = m_mMat.GetPos(  );
			}

			// �K���̊p�x�߂�
			if( m_NowTailRot > 0 )
			{
				m_NowTailRot = 0;
			}
		}

		// ���Ԃɂ���čs����؂�ւ���
		if( m_SaveTime + CHANGE_MOVE_TIME < timeGetTime() )
		{
			m_Flg ^= SEARCH;

			m_NowTailRot = 0;

			m_SaveTime = timeGetTime();
		}

		// ���ȏ�̖A�ɂȂ�����
		if( m_Flg & BUBBLE_WRAP )
		{
			// ����
			UpdateFloatPower();
		}

		// �A�g��
		UpdateBubbleScale();

		// ������ꂽ���̏���
		UpdateHitMove();

		// �K���Ăяo������
		UpdateMust();
	}

	// �A�X�V
	m_Bubbles.Update();

	if( m_mMat.GetPos().y > Sequence::CGameMain::SURFACESEA_HEIGHT )
	{
		m_mMat._42 = Sequence::CGameMain::SURFACESEA_HEIGHT;
	}

	// �����ۍX�V
	CMatrix mRotX = m_mMat;

	mRotX.RotateLocalX( m_NowTailRot );
	m_Tail.Update( mRotX, (m_Flg & SEARCH)? 0 : SPEED );
}

// �`��
void CRush::Draw(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	if( IsCulling( &m_mMat ) )
	{
		if( !(m_Flg & BREAK) )
		{
			// �{��
			// �V�F�[�_�`�悷��Ȃ�t�H���V�F�[�f�B���O
			if( is_shader )
			{
				DrawShader( lpdevice, m_lpMesh );
				
			}else{

				// �����łȂ���Ε��ʂɕ`��
				lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
				
				m_lpMesh->Draw( lpdevice );
			}

			// �������ޖA
			DrawBubble( lpdevice );
		
			// ������
			if( is_shader )
			{
				m_Tail.DrawShader( lpdevice );

			}else{

				m_Tail.Draw( lpdevice );
			}
		}
	}

	// �A
	m_Bubbles.DrawNoBreak( lpdevice );
}

// �G�t�F�N�g�`��
void CRush::DrawEffect(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( m_Flg & BREAK )
	{
		DrawBreakEnemy( lpdevice );
	}

	m_Bubbles.DrawBreak( lpdevice );
}

// �v���C���[�ɑ΂��Ă̍s��
void CRush::Action(Player::CDolphin &object)
{
	m_Flg |= SEARCH;
	m_SaveTime = timeGetTime();
}

// �P���I�u�W�F�N�g�ɑ΂��Ă̍s��
void CRush::Action(CDamageObjectBase &object)
{
	m_Flg |= SEARCH;
	m_SaveTime = timeGetTime();
	m_lpTargetFish = NULL;
}