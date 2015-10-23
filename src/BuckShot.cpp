//////////////////////////////
// �C���N���[�h
//////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

#include "ShotBase.h"
#include "BuckShot.h"

//////////////////////////////
// ���O��ԏȗ�
//////////////////////////////
using namespace Shot;

//////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////
const BYTE	CBuck::BUCKMAX_RAND		= 20;
const float	CBuck::SPEED			= 0.5f;
const short	CBuck::ERASE_TIME		= 5000;
const float	CBuck::BUBBLE_SPEED_MAX	= CBuck::SPEED * 10;
const LPSTR	CBuck::SHOT_MESH_PASS	= "Shot.x";

//////////////////////////////
// ����
//////////////////////////////

// �R���X�g���N�^
CBuck::CBuck( const D3DXMATRIX* mat ):
CBase( mat ),
m_StartTime( timeGetTime() ),
m_BubbleSpeed( 0.f )
{
	// �K���ɂ΂炷
	m_mMat.RotateLocalX( (rand() % BUCKMAX_RAND) * (rand() % 2 * 2 - 1) );
	m_mMat.RotateLocalY( (rand() % BUCKMAX_RAND) * (rand() % 2 * 2 - 1) );
	m_mMat.RotateLocalZ( (rand() % BUCKMAX_RAND) * (rand() % 2 * 2 - 1) );

	// �k������
	m_mMat.ScaleLocal( 0.3f); 
}

// �X�V
void CBuck::Update()
{
	// �A�̕���
	if( (m_BubbleSpeed += SPEED / 50) > BUBBLE_SPEED_MAX )
	{
		m_BubbleSpeed = BUBBLE_SPEED_MAX;	
	}

	// ���ۂɓ�����
	m_mMat.MoveLocal( 0, 0, ( m_BubbleSpeed < SPEED )? -SPEED + m_BubbleSpeed : 0 );

	// ���͂̕����ɓ�����
	CMatrix Mat( 0, m_BubbleSpeed, 0 );
	
	m_mMat = m_mMat * Mat;

	if( m_StartTime + ERASE_TIME < timeGetTime() )
	{
		m_Flg |= ERASE;
	}
}

// �`��
void CBuck::Draw( const LPDIRECT3DDEVICE9 lpdevice )
{
	lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
	CSXfileManager::GetInst()->GetData( SHOT_MESH_PASS )->Draw( lpdevice );
}

// ���𑗂����
void CBuck::Dispatch( IDoubleDispatch* object )
{
	object->Action( *this );
}

// �G�ɑ΂���s��
void CBuck::Action( Enemy::CBase& object )
{
	m_Flg |= ERASE;
}

// �f�X�g���N�^
CBuck::~CBuck()
{
}