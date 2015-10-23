//////////////////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////////////////
#include "BubbleManager.h"

#include "ShotBase.h"
#include "BigShot.h"

//////////////////////////////////////////////////
// ���O��ԏȗ�
//////////////////////////////////////////////////
using namespace Shot;

//////////////////////////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////////////////////////
const float	CBig::SPEED			= 1.f;
const DWORD	CBig::ERASE_TIME	= 10000;
const LPSTR	CBig::SHOT_MESH_PASS= "BigShot.x";

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �R���X�g���N�^
CBig::CBig():
m_StartTime( timeGetTime() ),
m_lpMesh( CSXfileManager::GetInst()->GetData( SHOT_MESH_PASS ) )
{
}

CBig::CBig( const D3DXMATRIX* mat ):
CBase( mat ),
m_StartTime( timeGetTime() ),
m_lpMesh( CSXfileManager::GetInst()->GetData( SHOT_MESH_PASS ) )
{
}

// �X�V
void CBig::Update()
{
	if( m_Flg & FLG_ALIVE )
	{
		m_mMat.MoveLocal( 0.f, 0.f, SPEED );

		// �莞���߂���Ώ�����
		if( timeGetTime() - m_StartTime > ERASE_TIME )
		{
			m_Flg |= FLG_ERASE;
		}
	}
}

// �`��
void CBig::Draw(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( IsCulling( &m_mMat ) )
	{
		lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
		m_lpMesh->Draw( lpdevice );
	}
}

// �f�X�g���N�^
CBig::~CBig(void)
{
}
