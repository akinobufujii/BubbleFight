////////////////////////////////
// �C���N���[�h
////////////////////////////////
#include "EnemyManager.h"
#include "UI.h"

#include "Bubble.h"
#include "BubbleManager.h"

#include "ShotBase.h"
#include "NormalShot.h"
#include "ShotManager.h"

#include "FishManager.h"
#include "RippleManager.h"
#include "LimitWalls.h"
#include "GameMain.h"

#include "PointSpriteManager.h"
#include "EnemyBase.h"

#include "RippleManager.h"
#include "Fintail.h"
#include "Screw.h"
#include "StrongDolphin.h"

#include "DamageObjectBase.h"

#include "WaterSpray.h"

#include "FinTail.h"

#include "NormalEnemy.h"

////////////////////////////////
// ���O��Ԃ̏ȗ�
////////////////////////////////
using namespace Enemy;

////////////////////////////////
// �ÓI�����o���̉�
////////////////////////////////
const float CNormal::SPEED				= 0.45f;
const float	CNormal::ROT_SPEED			= 8.f;
const ULONG	CNormal::SCORE				= 100;
const LPSTR	CNormal::ENEMY_MESH_PASS	= "Chara.x";
const DWORD	CNormal::AWAY_TIME			= 10000;
const DWORD CNormal::CHANGE_ATTACK_TIME	= 3500;

////////////////////////////////
// ����
////////////////////////////////

// �R���X�g���N�^
CNormal::CNormal( float x, float y, float z ):
m_AnimeCnt( 0 ),
m_lpMesh( CSXfileManager::GetInst()->GetData( ENEMY_MESH_PASS ) ),
m_IsAway( false ),
m_StartAwayTime( timeGetTime() ),
m_IsAttack( false ),
m_ViewSaveTime( timeGetTime() )
{
	m_mMat.SetTrans( x, y, z );

	// �g��ݒ�
	m_Ripples.SetCreateHeight( m_lpMesh->GetRadius() );
	m_Ripples.SetSize(
		m_lpMesh->GetRadius(),
		m_lpMesh->GetRadius() );

	m_Tail.SetFarPos( &CVector() );
}

// �f�X�g���N�^
CNormal::~CNormal()
{
}

// �X�V
void CNormal::Update()
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

			// ���ʂŒ��˂Ă��邩
			if( m_Flg & JUMP )
			{
				// ���˂Ă���
				UpdateJump( SPEED );

			}else{

				if( m_IsAway )
				{
					// �����Ă���
					UpdateAway();

				}else{

					// �ǔ����Ă���
					UpdateDrive();
				}
			}
		}

		// �A�g��
		UpdateBubbleScale();

		// ������т̏���
		UpdateHitMove();

		// ���㏈��
		UpdateMust();

		m_Tail.Update( m_mMat, SPEED );
	}
}

// �`��
void CNormal::Draw(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	if( !(m_Flg & BREAK) )
	{
		// ��ʓ�����
		if( IsCulling( &m_mMat ) )
		{
			lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );

			lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );

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

	// �A
	m_Bubbles.DrawNoBreak( lpdevice );
}

// �G�t�F�N�g�`��
void CNormal::DrawEffect( const LPDIRECT3DDEVICE9 lpdevice )
{
	if( m_Flg & BREAK )
	{
		DrawBreakEnemy( lpdevice );
	}

	m_Bubbles.DrawBreak( lpdevice );

	m_lpSpray->Draw( lpdevice );
}

// ������X�V
void CNormal::UpdateAway()
{
	// �Y��
	// ���̈ʒu����e���ꂽ�����ɐi��
	CVector TargetPos = m_mMat.GetPos() + m_HitDir * 10.f;

	// �ǔ�����
	CVector RotAxis;
	float Degree = GetAxisAndRot( &RotAxis, TargetPos, m_mMat );

	// ��]���x
	if( Degree > ROT_SPEED )
	{
		Degree = ROT_SPEED;
	}

	// ��
	m_mMat.RotateLocalAxis( &RotAxis, Degree );

	// �{�̑��x�ňړ�
	UpdateCollisionMove(
		&D3DXVECTOR3( 0.f, 0.f, SPEED * 2 ), 
		m_lpMesh->GetRadius() );

	// �����X�e�[�W������o���Ȃ��
	D3DXVECTOR3 Pos = m_mMat.GetPos();
	if( Sequence::CGameMain::SEA_MOVELIMIT - 10 < abs( Pos.x ) ||
		Sequence::CGameMain::SEA_MOVELIMIT - 10 < abs( Pos.z ) )
	{
		// ���_�Ɍ������ē�����
		D3DXVec3Normalize( &m_HitDir, &(Pos - D3DXVECTOR3( 0, 0, 0 ) ) );
	}
	
	// ��莞�Ԍo�Ă΂܂��ǔ�����
	if( timeGetTime() - m_StartAwayTime > AWAY_TIME )
	{
		m_IsAway = false;

		// �ǂ��񂷖ڕW�����߂�
		SearchTarget();
	}

	// �W�����v���邩�ǂ���
	JudgeJump();
}

// �ǂ��X�V
void CNormal::UpdateDrive()
{
	// �ڕW�����܂��Ă���Ȃ�
	if( m_lpTargetFish )
	{
		// �ǔ�����
		CVector RotAxis;
		float Degree = GetAxisAndRot( &RotAxis, m_lpTargetFish->GetPos(), m_mMat );

		// ��]���x
		if( Degree > CBase::DEFAULT_ROTSPEED )
		{
			Degree = CBase::DEFAULT_ROTSPEED;
		}

		// ��]
		m_mMat.RotateLocalAxis( &RotAxis, Degree );

		UpdateCollisionMove( 
			&D3DXVECTOR3( 0.f, 0.f, SPEED ),
			m_lpMesh->GetRadius() );
	
	}else if( m_IsAttack == false ){	// �܂��U���Ɉڂ��Ă��Ȃ��Ȃ�

		D3DXVECTOR3 Pos = m_mMat.GetPos();
		CVector		TargetPos;

		// �����X�e�[�W������o���Ȃ��
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
			m_ViewSaveTime = timeGetTime();

		}else{

			// ��莞�ԉ�ʓ��Ȃ�
			if( timeGetTime() - m_ViewSaveTime > CHANGE_ATTACK_TIME )
			{
				m_IsAttack = true;
			}
		}

		// �{���ňړ�
		UpdateCollisionMove(
			&D3DXVECTOR3( 0.f, 0.f, SPEED * 2 ),
			m_lpMesh->GetRadius() );

	}else{

		// �U�����Ȃ�v���C���[�Ɍ�����
		UpdateRot();

		// ��ʓ�����o����
		if( IsCulling( &m_mMat ) == false )
		{	
			m_ViewSaveTime = timeGetTime();
			m_IsAttack = false;
		}

		UpdateCollisionMove(
			&D3DXVECTOR3( 0.f, 0.f, SPEED ), 
			m_lpMesh->GetRadius() );
	}

	// ���ʂɏo�Ă���W�����v��
	JudgeJump();
}

// �v���C���[�ɑ΂���s��
void CNormal::Action(Player::CDolphin &object)
{
	// ���������肷��
	m_HitDir = m_mMat.GetPos() - object.GetPos();
	
	// ���K��
	m_HitDir.GetNormalVector();

	// ������ї͂����Z�b�g
	m_HitPower = 1.f;

	// �q�b�g���A�E�F�C�̃A�E�F�C������
	m_IsAway = true;
	m_IsAttack = false;

	// ���ԕۑ�
	m_StartAwayTime = timeGetTime();
}

// �P���I�u�W�F�N�g�ɑ΂��Ă̍s��
void CNormal::Action(CDamageObjectBase &object)
{
	// ������
	m_IsAway = true;
	m_IsAttack = false;
	m_lpTargetFish = NULL;
}