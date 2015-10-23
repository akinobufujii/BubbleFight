/////////////////////////////////////
// �C���N���[�h
/////////////////////////////////////
#include "RippleManager.h"
#include "BubbleManager.h"
#include "ShotManager.h"
#include "BubbleManager.h"
#include "EnemyBase.h"

#include "FishManager.h"
#include "RippleManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "GameMain.h"

#include "WaterSpray.h"

#include "RippleManager.h"
#include "FinTail.h"
#include "StrongDolphin.h"

#include "BubbleBreaker.h"

/////////////////////////////////////
// ���O��ԏȗ�
/////////////////////////////////////
using namespace Enemy;

/////////////////////////////////////
// �ÓI�����o������
/////////////////////////////////////
const float		CBubbleBreaker::SPEED				= 0.5f;
const ULONG		CBubbleBreaker::SCORE				= 250;
const LPSTR		CBubbleBreaker::MESHPASS_ENEMY		= "VacuumBubble.x";
const DWORD		CBubbleBreaker::MOVE_CHANGETIME		= 5000;
const float		CBubbleBreaker::DIRCHANGE_ANIMETIME	= 0.01f;
const BYTE		CBubbleBreaker::ROT_SPEED			= 5;
const USHORT	CBubbleBreaker::AWAY_TIME			= 10000;
const USHORT	CBubbleBreaker::CHANGE_ATTACK_TIME	= 3500;

/////////////////////////////////////
// ����
/////////////////////////////////////

// �R���X�g���N�^
CBubbleBreaker::CBubbleBreaker( float x, float y, float z ):
m_IsDirChange( false ),
m_NowAnimetionCnt( 0.f ),
m_NowSpeed( SPEED ),
m_lpTargetEnemy( NULL ),
m_IsAway( false ),
m_IsAttack( false ),
m_SaveTime( timeGetTime() ),
m_SaveDriveTime( timeGetTime() ) 
{
	// �����ʒu�ݒ�
	m_mMat.SetTrans( x, y, z );

	// ���b�V���l��
	m_lpMesh = CSXfileManager::GetInst()->GetData( MESHPASS_ENEMY );

	// �K���ʒu�ݒ�
	m_Tail.SetFarPos( &D3DXVECTOR3( 0, 0, -1.2f ) );

	m_Tail.SetRotMax( 10 );

	m_Ripples.SetSize(
		m_lpMesh->GetRadius(),
		m_lpMesh->GetRadius() + 1.2f );

	m_Tail.SetFarPos( &CVector() );
}

// �f�X�g���N�^
CBubbleBreaker::~CBubbleBreaker()
{
}

// �X�V
void CBubbleBreaker::Update()
{
	// �j�󂳂�Ă���
	if( m_Flg & BREAK )
	{
		// �����X�V
		if( m_Particle.IsErase() )
		{
			m_Flg |= ERASE;	// ����
		}

		// �p�[�e�B�N���X�V
		m_Particle.Update();

		// �A�X�V
		m_Bubbles.Update();

	}else{

		// ���ȏ�̖A�ɂȂ�����
		if( m_Flg & BUBBLE_WRAP )
		{
			// ����
			UpdateFloatPower();

		}else{

			if( m_Flg & JUMP )
			{
				// �W�����v�X�V
				UpdateJump( SPEED );
			
			}else{

				if( m_IsAway )
				{
					// ������
					UpdateAway();

				}else{

					// �ړ�
					UpdateMove();
				}
			}
		}

		// �A�g��
		UpdateBubbleScale();

		// �����������̍X�V
		UpdateHitMove();

		// ���㏈��
		UpdateMust();

		// �K��
		m_Tail.Update( m_mMat, m_NowSpeed );
	}
}

// �ړ��X�V
void CBubbleBreaker::UpdateMove()
{
	if( m_lpTargetEnemy == NULL )
	{
		// �v���C���[�Ɍ������čU�����d�|����
		// �U���\�����쒆
		if( m_IsAttack == false )
		{
			D3DXVECTOR3 Pos = m_mMat.GetPos();
			CVector		TargetPos;

			if( Sequence::CGameMain::SEA_MOVELIMIT - 50 < abs( Pos.x ) ||
				Sequence::CGameMain::SEA_MOVELIMIT - 50 < abs( Pos.z ) )
			{
				// ���_�Ɍ������ē�����
				TargetPos = D3DXVECTOR3( 0, 0, 0 );

			}else{

				CMatrix mPlayer = GetPlayerInfo()->GetMatrix();

				// �v���C���[�̍��̕����𒊏o����
				CVector Dir( 0, 0, 1 );
				Dir.GetTransNormal( &mPlayer );

				// �K���ɑO���ɑ������݁A�ړI�n�ɐݒ�
				Dir *= 30.f;
				Dir.y += 5;
				TargetPos = mPlayer.GetPos() + Dir;
			}

			// �ǔ�����
			CVector RotAxis;
			float Degree = GetAxisAndRot( &RotAxis, TargetPos, m_mMat );

			// ��]���x
			if( Degree > CBase::DEFAULT_ROTSPEED )
			{
				Degree = CBase::DEFAULT_ROTSPEED;
			}

			// ��]
			m_mMat.RotateLocalAxis( &RotAxis, Degree );

			// ��ʓ����ǂ����𔻒f����
			if( IsCulling( &m_mMat ) == false )
			{	
				m_SaveDriveTime = timeGetTime();

			}else{

				// ��莞�ԉ�ʓ��Ȃ�
				if( timeGetTime() - m_SaveDriveTime > CHANGE_ATTACK_TIME )
				{
					m_IsAttack = true;
				}
			}

			// �{���ňړ�
			UpdateCollisionMove( &D3DXVECTOR3( 0.f, 0.f, SPEED * 2 ), m_lpMesh->GetRadius() );

		}else{

			// �U�����Ȃ�v���C���[�Ɍ�����
			UpdateRot();

			// ��ʓ�����o����
			if( IsCulling( &m_mMat ) == false )
			{	
				m_SaveDriveTime = timeGetTime();
				m_IsAttack = false;
			}

			UpdateCollisionMove( &D3DXVECTOR3( 0.f, 0.f, SPEED ), m_lpMesh->GetRadius() );
		}

	}else{

		// ������x��Ȃ畉��Ȃ�
		if( m_lpTargetEnemy->GetFlg() & (BREAK | ERASE) )
		{
			m_lpTargetEnemy = NULL;
			return;
		}

		// �ǔ�����
		CVector RotAxis;
		float Degree = GetAxisAndRot( &RotAxis, m_lpTargetEnemy->GetPos(), m_mMat );

		// ��]���x
		if( Degree > CBase::DEFAULT_ROTSPEED )
		{
			Degree = CBase::DEFAULT_ROTSPEED;
		}

		m_mMat.RotateLocalAxis( &RotAxis, Degree );

		UpdateCollisionMove( &D3DXVECTOR3( 0, 0, SPEED ), m_lpMesh->GetRadius() );
	}

	// �W�����v���邩�ǂ���
	JudgeJump();
}

// ������X�V
void CBubbleBreaker::UpdateAway()
{
	// ���̈ʒu����e���ꂽ�����ɐi��
	CVector TargetPos = m_mMat.GetPos() + m_HitDir * 10.f;

	// �ǔ�����
	CVector RotAxis;
	float Degree = GetAxisAndRot( &RotAxis, TargetPos, m_mMat );

	// ��]���x
	if( Degree > CBase::DEFAULT_ROTSPEED )
	{
		Degree = CBase::DEFAULT_ROTSPEED;
	}

	// �p�x�␳���ĉ�]
	if( Degree > ROT_SPEED )
	{
		Degree = ROT_SPEED;
	}

	// ��
	m_mMat.RotateLocalAxis( &RotAxis, Degree );

	// �{�̑��x�ňړ�
	UpdateCollisionMove( &D3DXVECTOR3( 0, 0, SPEED * 2 ), m_lpMesh->GetRadius() );
	
	// ��莞�Ԍo�Ă΂܂��ǔ�����
	if( timeGetTime() - m_AwayStartTime > AWAY_TIME )
	{
		m_IsAway = false;
	}

	// �W�����v���邩�ǂ���
	JudgeJump();
}

// �`��
void CBubbleBreaker::Draw(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	if( !(m_Flg & BREAK) )
	{
		if( IsCulling( &m_mMat ) )
		{
			// �����_�����O�ݒ�
			lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
			lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );
			lpdevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

			// �{��
			// �V�F�[�_�`�悷��Ȃ�t�H���V�F�[�f�B���O
			if( is_shader )
			{
				DrawShader( lpdevice, m_lpMesh );
				
			}else{

				// �����łȂ���Ε��ʂɕ`��
				m_lpMesh->Draw( lpdevice );
			}
		}
	
		// ������
		if( is_shader )
		{
			m_Tail.DrawShader( lpdevice );

		}else{

			m_Tail.Draw( lpdevice );
		}

		// �������ޖA
		DrawBubble( lpdevice );
	}
}

// �G�t�F�N�g�`��
void CBubbleBreaker::DrawEffect( const LPDIRECT3DDEVICE9 lpdevice )
{
	if( m_Flg & BREAK )
	{
		DrawBreakEnemy( lpdevice );
	}

	m_lpSpray->Draw( lpdevice );
}

// �ړI�n��ݒ肷��
void CBubbleBreaker::SetDirEnemy( Enemy::CBase* enemy )
{
	// �����������g����x��Ȃ牽�����Ȃ�
	if( this == enemy || (enemy->GetFlg() & (BREAK | ERASE)) )
	{
		return;
	}

	// �����ړI�n�ɂȂ��Ă��Ȃ����
	if( m_lpTargetEnemy == NULL )
	{
		// �������ŖڕW
		m_lpTargetEnemy = enemy;
	
	}else{

		// �������v�Z����
		D3DXVECTOR3	Pos	= m_mMat.GetPos();
		CVector		vOld= m_lpTargetEnemy->GetPos() - Pos;
		CVector		vNew= enemy->GetPos() - Pos;

		// �V�������̕����߂���΂������ݒ肷��
		if( vNew.GetLength() < vOld.GetLength() )
		{
			m_lpTargetEnemy = enemy;
		}
	}
}

// �G�ɑ΂���s��
void CBubbleBreaker::Action(Enemy::CBase &object)
{
	// �����ł���܂�Ă����
	if( object.GetBubbleScale() > 0.f )
	{
		// �z���H
		object.VacuumBubble( 0.01f );
		
	}else{

		// �����łȂ���ΕW���̓���
		CBase::Action( object );

		// �^�[�Q�b�g���ݒ肵�Ȃ�
		m_lpTargetEnemy = NULL;
	}
}

// �v���C���[�ɑ΂���s��
void CBubbleBreaker::Action(Player::CDolphin &object)
{
	// ���Ă�Γ�����
	// ���������߂�
	m_HitDir =  m_mMat.GetPos() - object.GetPos();

	m_HitDir.GetNormalVector();

	// ������ԗ͂����߂�
	m_HitPower = 1.f;

	// ������
	m_IsAway = true;
	m_IsAttack = false;

	// ���ԕۑ�
	m_AwayStartTime = timeGetTime();
}