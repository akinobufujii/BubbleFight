//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

#include "ShotManager.h"

//////////////////////////////////////////////////
// 名前空間省略
//////////////////////////////////////////////////
using namespace Shot;

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// デストラクタ
CManager::~CManager()
{
	BOOST_FOREACH( CBase* It, m_ShotList )
	{
		DeleteObjectByPool( It );
	}

	m_ShotList.clear();
}

// 更新
void CManager::Update()
{
	// 途中削除があるので通常ループ
	SHOTLIST::iterator It = m_ShotList.begin();

	while( It != m_ShotList.end() )
	{
		// 更新
		(*It)->Update();

		// 削除判定
		if( (*It)->GetFlg() & CBase::FLG_ERASE )
		{
			DeleteObjectByPool( (*It) );
			
			It = m_ShotList.erase( It );
			continue;
		}

		++It;
	}
}

// 描画
void CManager::Draw(const LPDIRECT3DDEVICE9 lpdevice)
{
	BOOST_FOREACH( CBase* It, m_ShotList )
	{
		It->Draw( lpdevice );
	}
}

// エフェクト描画
void CManager::DrawEffect(const LPDIRECT3DDEVICE9 lpdevice)
{
	BOOST_FOREACH( CBase* It, m_ShotList )
	{
		It->DrawEffect( lpdevice );
	}
}

// 弾追加
void CManager::Add(CManager::KIND kind, const D3DXMATRIX* mat, const float power)
{
	// 種類分岐(変数宣言によるエラー防止のため、スコープを明確にしている)
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

// 渡されたオブジェクトを判別してプールから削除
void CManager::DeleteObjectByPool( CBase* object )
{
	// RTTI取得
	const type_info& NowType = typeid( *object );

	// 1つ1つ見ていき、型が合えばダウンキャストで削除
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