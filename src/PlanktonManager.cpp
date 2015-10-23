//////////////////////////////
// インクルード
//////////////////////////////
#include "Plankton.h"
#include "PlanktonManager.h"

//////////////////////////////
// 静的メンバ実体化
//////////////////////////////
const LPSTR	CPlanktonManager::TEX_PASS	= "Plankton.png";

//////////////////////////////
// 実装
//////////////////////////////

// コンストラクタ
CPlanktonManager::CPlanktonManager()
{
}

// 更新
void CPlanktonManager::Update()
{
	//PLANKTONLIST::iterator It = m_PlanktonList.begin();

	//// 更新
	//while( It != m_PlanktonList.end() )
	//{
	//	(*It)->Update( player_pos );

	//	++It;
	//}
}

// 描画
void CPlanktonManager::Draw(const LPDIRECT3DDEVICE9 lpdevice, const CCamera *camera)
{
	lpdevice->SetFVF( CBoard::FVF_VERTEX );
	lpdevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	// テクスチャ設定
	lpdevice->SetTexture( 0, CSTexManager::GetInst()->GetData( TEX_PASS )->GetTexture() );

	// 描画
	BOOST_FOREACH( CPlankton* It, m_PlanktonList )
	{
		It->Draw( lpdevice, camera );
	}
}

// プランクトンを作る
void CPlanktonManager::SetNum(const UINT num)
{
	// 指定された数が多い場合
	if( num > m_PlanktonList.size() )
	{
		// 追加する
		for( int i = num - m_PlanktonList.size(); i > 0; i-- )
		{
			m_PlanktonList.push_back( NEW CPlankton() );
		}
	
	}else if( num < m_PlanktonList.size() ){

		// 少ない場合は削除する
		PLANKTONLIST::iterator It = m_PlanktonList.begin();

		for( int i = m_PlanktonList.size() - num; i > 0 || It != m_PlanktonList.end(); i-- )
		{
			SAFE_DELETE( (*It) );

			It = m_PlanktonList.erase( It );
		}
	}
}

// デストラクタ
CPlanktonManager::~CPlanktonManager()
{
	// 解放
	BOOST_FOREACH( CPlankton* It, m_PlanktonList )
	{
		SAFE_DELETE( It );
	}
}
