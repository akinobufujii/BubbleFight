//////////////////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////////////////
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

#include "PowerShot.h"

//////////////////////////////////////////////////
// ���O��ԏȗ�
//////////////////////////////////////////////////
using namespace Shot;

//////////////////////////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////////////////////////
const float CPower::SPEED			= 2.5f;
const LPSTR	CPower::SHOT_MESH_PASS	= "Spike.x";

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �R���X�g���N�^
CPower::CPower():
m_StartTime( timeGetTime() ),
m_lpMesh( CSXfileManager::GetInst()->GetData( SHOT_MESH_PASS ) )
{
}

CPower::CPower(const D3DXMATRIX *mat):
CBase( mat ),
m_StartTime( timeGetTime() ),
m_lpMesh( CSXfileManager::GetInst()->GetData( SHOT_MESH_PASS ) )
{
}

// �X�V
void CPower::Update()
{
	// �����Ă�����
	if( m_Flg & FLG_ALIVE )
	{
		// �ړ�
		m_mMat.MoveLocal( 0, 0, SPEED );

		// �A�ǉ�
		m_Bubbles.Add( &m_mMat, 1 );

		// ��莞�ԉ߂����������
		if( timeGetTime() - m_StartTime > 3000 )
		{
			m_Flg &= ~FLG_ALIVE;
		}
	}

	m_Bubbles.Update();

	// ����ł��ĖA���Ȃ����
	if( ~(m_Flg & FLG_ALIVE) && m_Bubbles.IsEmpty() )
	{
		m_Flg |= FLG_ERASE;	// ����
	}
}

// �`��
void CPower::Draw(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( IsCulling( &m_mMat ) )
	{
		lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );

		m_lpMesh->Draw( lpdevice );
	}
}

// �G�ɑ΂���
void CPower::Action(Enemy::CBase &object)
{
	//if( ~(object.GetFlg() & Enemy::CBase::BREAK) )
	{
		m_Flg &= ~FLG_ALIVE;
	}
}

// �f�X�g���N�^
CPower::~CPower()
{
}