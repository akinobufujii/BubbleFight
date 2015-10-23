/////////////////////////////////
// �C���N���[�h
/////////////////////////////////
#include "EnemyManager.h"
#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "GameMain.h"

#include "BubbleManager.h"

#include "DamageObjectBase.h"

/////////////////////////////////
// ����
/////////////////////////////////

// �R���X�g���N�^
CDamageObjectBase::CDamageObjectBase():
m_lpBubbles( NEW CBubbleManager() )
{
}

// �f�X�g���N�^
CDamageObjectBase::~CDamageObjectBase()
{
	SAFE_DELETE( m_lpBubbles );
}

// �h���悪�K���Ăяo���Ăق�������
void CDamageObjectBase::MustUpdate()
{
	// ����
	if( m_mMat.GetPos().y > Sequence::CGameMain::SURFACESEA_HEIGHT )
	{
		m_mMat._42 = Sequence::CGameMain::SURFACESEA_HEIGHT;
	}

	// �A�ǉ�
	m_lpBubbles->Add( &m_mMat, 1 );

	m_lpBubbles->Update();
}