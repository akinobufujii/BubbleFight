//////////////////////////////////
// インクルード
//////////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

//////////////////////////////////
// 実装
//////////////////////////////////

// 更新
void CBubbleManager::Update()
{
	// ※途中削除があるので通常ループ
	BUBBLELIST::iterator	It = m_BubbleList.begin();

	while( It != m_BubbleList.end() )
	{
		(*It)->Update();

		// 消すなら消す
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

// 描画
void CBubbleManager::DrawNoBreak(const LPDIRECT3DDEVICE9 &lpdevice)
{
	lpdevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	lpdevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );

	BOOST_FOREACH( CBubble* It, m_BubbleList )
	{
		// 判定
		if( ~(It->GetFlg() & CBubble::BREAK) )
		{
			It->Draw( lpdevice );
		}
	}

	lpdevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );
}

// 描画
void CBubbleManager::DrawBreak(const LPDIRECT3DDEVICE9 &lpdevice)
{
	BOOST_FOREACH( CBubble* It, m_BubbleList )
	{
		// 判定
		if( It->GetFlg() & CBubble::BREAK )
		{
			It->Draw( lpdevice );
		}
	}
}

// 追加
void CBubbleManager::Add(const D3DXMATRIX *mat, int num)
{
	CBubble* lpTemp = m_mpBubble.construct();
	
	lpTemp->SetMatrix( *mat );

	m_BubbleList.push_back( lpTemp );
}

// 破壊するように通知する
void CBubbleManager::BreakAll()
{
	BOOST_FOREACH( CBubble* It, m_BubbleList )
	{
		It->BreakBubble();
	}
}

// デストラクタ
CBubbleManager::~CBubbleManager()
{
	BOOST_FOREACH( CBubble* It, m_BubbleList )
	{
		m_mpBubble.destroy( It );
	}

	m_BubbleList.clear();
}
