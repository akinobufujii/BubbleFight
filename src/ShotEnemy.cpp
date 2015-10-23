/////////////////////////////
// �C���N���[�h
/////////////////////////////
#include "BubbleManager.h"

#include "ShotManager.h"

#include "ShotBase.h"

#include "RippleManager.h"
#include "EnemyBase.h"
#include "ShotEnemy.h"

/////////////////////////////
// ���O��ԏȗ�
/////////////////////////////
using namespace Enemy;

/////////////////////////////
// �ÓI�����o���̉�
/////////////////////////////
const ULONG	CShoot::SCORE			= 150;
const LPSTR	CShoot::ENEMY_MESH_PASS	= "ShotEnemy.x";

/////////////////////////////
// ����
/////////////////////////////

// �R���X�g���N�^
CShoot::CShoot( float x, float y, float z  ):
SHOT_INTERVAL( rand() % 1501 + 2000 ),	// ��������ƃo��������
m_ShotSaveTime( timeGetTime() ),
m_lpMesh( CSXfileManager::GetInst()->GetData( ENEMY_MESH_PASS ) )
{
	m_mMat.SetTrans( x, y, z );

	// �g��̐ݒ�
	m_Ripples.SetCreateHeight( m_lpMesh->GetRadius() );
	m_Ripples.SetSize( m_lpMesh->GetRadius(), m_lpMesh->GetRadius() );
}

// �f�X�g���N�^
CShoot::~CShoot()
{
}

// �X�V
void CShoot::Update()
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

			// ��ʓ��ɓ����Ă�����΍U��
			if( IsCulling( &m_mMat ) )
			{
				// ���@�̕��Ɍ���
				UpdateRot();

				// ����
				UpdateShot();
			}

			// ������т̏���
			UpdateHitMove();
		}

		// �A�g��
		UpdateBubbleScale();

		// ���㏈��
		UpdateMust();
	}
}

// �`��
void CShoot::Draw(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	if( !(m_Flg & BREAK) )
	{
		if( IsCulling( &m_mMat ) )
		{
			lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );

			m_lpMesh->Draw( lpdevice );

			// �A
			DrawBubble( lpdevice );
		}

		m_Shots.Draw( lpdevice );
	}
}

// �G�t�F�N�g�`��
void CShoot::DrawEffect(const LPDIRECT3DDEVICE9 lpdevice)
{
	// ������`��
	if( m_Flg & BREAK )
	{
		DrawBreakEnemy( lpdevice );
	}

	m_Shots.DrawEffect( lpdevice );
}

// �e�X�V
void CShoot::UpdateShot()
{
	// ��莞�Ԓ�������
	if( timeGetTime() - m_ShotSaveTime > SHOT_INTERVAL )
	{
		m_Shots.Add( Shot::CManager::NORMAL, &m_mMat );

		m_ShotSaveTime = timeGetTime();
	}

	m_Shots.Update();
}