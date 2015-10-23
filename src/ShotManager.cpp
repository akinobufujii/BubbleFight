//////////////////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

#include "ShotManager.h"

//////////////////////////////////////////////////
// ���O��ԏȗ�
//////////////////////////////////////////////////
using namespace Shot;

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �f�X�g���N�^
CManager::~CManager()
{
	BOOST_FOREACH( CBase* It, m_ShotList )
	{
		DeleteObjectByPool( It );
	}

	m_ShotList.clear();
}

// �X�V
void CManager::Update()
{
	// �r���폜������̂Œʏ탋�[�v
	SHOTLIST::iterator It = m_ShotList.begin();

	while( It != m_ShotList.end() )
	{
		// �X�V
		(*It)->Update();

		// �폜����
		if( (*It)->GetFlg() & CBase::FLG_ERASE )
		{
			DeleteObjectByPool( (*It) );
			
			It = m_ShotList.erase( It );
			continue;
		}

		++It;
	}
}

// �`��
void CManager::Draw(const LPDIRECT3DDEVICE9 lpdevice)
{
	BOOST_FOREACH( CBase* It, m_ShotList )
	{
		It->Draw( lpdevice );
	}
}

// �G�t�F�N�g�`��
void CManager::DrawEffect(const LPDIRECT3DDEVICE9 lpdevice)
{
	BOOST_FOREACH( CBase* It, m_ShotList )
	{
		It->DrawEffect( lpdevice );
	}
}

// �e�ǉ�
void CManager::Add(CManager::KIND kind, const D3DXMATRIX* mat, const float power)
{
	// ��ޕ���(�ϐ��錾�ɂ��G���[�h�~�̂��߁A�X�R�[�v�𖾊m�ɂ��Ă���)
	switch( kind )
	{
	case POWER:
		{
			CPower* lpPower = m_mpPower.construct();

			lpPower->SetMatrix( *mat );

			m_ShotList.push_back( lpPower );
		}
		
		break;

	case BIG:
		{
			CBig* lpBig = m_mpBig.construct();

			lpBig->SetMatrix( *mat );

			m_ShotList.push_back( lpBig );
		}
		
		break;

	case BUBBLERING:
		{
			CBubbleRing* lpBubbleRing = m_mpBubbleRing.construct();

			lpBubbleRing->SetPower( power );
			lpBubbleRing->SetMatrix( *mat );

			m_ShotList.push_back( lpBubbleRing );
		}
		break;

	case NORMAL:
		{
			CNormal* lpNormal = m_mpNormal.construct();

			lpNormal->SetMatrix( *mat );

			m_ShotList.push_back( lpNormal );
		}
		break;
	}
}

// �n���ꂽ�I�u�W�F�N�g�𔻕ʂ��ăv�[������폜
void CManager::DeleteObjectByPool( CBase* object )
{
	// RTTI�擾
	const type_info& NowType = typeid( *object );

	// 1��1���Ă����A�^�������΃_�E���L���X�g�ō폜
	if( NowType == typeid( CBubbleRing ) )
	{
		m_mpBubbleRing.destroy( dynamic_cast<CBubbleRing*>( object ) );
	
	}else if( NowType == typeid( CNormal ) ){
	
		m_mpNormal.destroy( dynamic_cast<CNormal*>( object ) );

	}else if( NowType == typeid( CBig ) ){

		m_mpBig.destroy( dynamic_cast<CBig*>( object ) );
	
	}else if( NowType == typeid( CPower ) ){

		m_mpPower.destroy( dynamic_cast<CPower*>( object ) );
	}
}