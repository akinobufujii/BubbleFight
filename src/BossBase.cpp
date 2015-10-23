//////////////////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////////////////
#include "BubbleManager.h"
#include "ShotManager.h"

#include "StoneManager.h"

#include "BossBase.h"

//////////////////////////////////////////////////
// ���O��ԏȗ�
//////////////////////////////////////////////////
using namespace Boss;

//////////////////////////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////////////////////////
const CBase::FLGTYPE	CBase::FLG_ALIVE	= 0x01;	
const CBase::FLGTYPE	CBase::FLG_BREAK	= 0x02;
const CBase::FLGTYPE	CBase::FLG_ERASE	= 0x04;
const CBase::FLGTYPE	CBase::FLG_STOP		= 0x08;
const CBase::FLGTYPE	CBase::FLG_ATTACK	= 0x10;
const CBase::FLGTYPE	CBase::FLG_STICK	= 0x20;
const CBase::FLGTYPE	CBase::FLG_DAMAGE	= 0x40;
const CBase::FLGTYPE	CBase::FLG_WAIT		= 0x80;

CMatrix			CBase::m_PlayerMat;
CStoneManager*	CBase::m_lpStones = NULL;

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �R���X�g���N�^
CBase::CBase():
m_Flg( FLG_ALIVE ),
m_NowMovement( m_MovementList.end() )
{
}

// �s���X�V
void CBase::UpdateMovement()
{
	static bool IsCall = false;	// �Ă΂ꂽ���ǂ���

	// ��ԍŏ��Ȃ�
	if( IsCall == false )
	{
		IsCall = true;

		// ������
		m_NowMovement = m_MovementList.begin();

		// �����_���ōs������
		for( int i = (rand() % m_MovementList.size()); i > 0; i-- )
		{
			m_NowMovement++;
		}
	
	}
	
	// ���s���Ă��̍s�����I����
	if( (this->*(*m_NowMovement))() == false )
	{
		// ������
		m_NowMovement = m_MovementList.begin();

		// �����_���ōs������
		for( int i = (rand() % m_MovementList.size()); i > 0; i-- )
		{
			m_NowMovement++;
		}
	}
}