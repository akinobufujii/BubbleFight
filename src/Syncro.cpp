//////////////////////////////
// �C���N���[�h
//////////////////////////////
#include "Syncro.h"

//////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////
CSSyncro* CSSyncro::m_lpInst	= NULL;

//////////////////////////////
// ����
//////////////////////////////

// �R���X�g���N�^
CSSyncro::CSSyncro()
{
	// ������
	InitializeCriticalSection( &m_Section );
}


// �f�X�g���N�^
CSSyncro::~CSSyncro()
{
	// ���
	DeleteCriticalSection( &m_Section );
}