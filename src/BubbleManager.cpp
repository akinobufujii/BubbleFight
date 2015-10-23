//////////////////////////////////
// �C���N���[�h
//////////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

//////////////////////////////////
// ����
//////////////////////////////////

// �X�V
void CBubbleManager::Update()
{
	// ���r���폜������̂Œʏ탋�[�v
	BUBBLELIST::iterator	It = m_BubbleList.begin();

	while( It != m_BubbleList.end() )
	{
		(*It)->Update();

		// �����Ȃ����
		if( ((*It)->GetFlg() & CBubble::ERASE) ||
			IsCulling( (*It)->GetMatrix() ) == false )
		{
			m_mpBubble.destroy( (*It) );
			
			It = m_BubbleList.erase( It );
			
			continue;
		}

		++It;
	}
}

// �`��
void CBubbleManager::DrawNoBreak(const LPDIRECT3DDEVICE9 &lpdevice)
{
	lpdevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	lpdevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );

	BOOST_FOREACH( CBubble* It, m_BubbleList )
	{
		// ����
		if( ~(It->GetFlg() & CBubble::BREAK) )
		{
			It->Draw( lpdevice );
		}
	}

	lpdevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );
}

// �`��
void CBubbleManager::DrawBreak(const LPDIRECT3DDEVICE9 &lpdevice)
{
	BOOST_FOREACH( CBubble* It, m_BubbleList )
	{
		// ����
		if( It->GetFlg() & CBubble::BREAK )
		{
			It->Draw( lpdevice );
		}
	}
}

// �ǉ�
void CBubbleManager::Add(const D3DXMATRIX *mat, int num)
{
	CBubble* lpTemp = m_mpBubble.construct();
	
	lpTemp->SetMatrix( *mat );

	m_BubbleList.push_back( lpTemp );
}

// �j�󂷂�悤�ɒʒm����
void CBubbleManager::BreakAll()
{
	BOOST_FOREACH( CBubble* It, m_BubbleList )
	{
		It->BreakBubble();
	}
}

// �f�X�g���N�^
CBubbleManager::~CBubbleManager()
{
	BOOST_FOREACH( CBubble* It, m_BubbleList )
	{
		m_mpBubble.destroy( It );
	}

	m_BubbleList.clear();
}
