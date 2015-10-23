//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "LimitWalls.h"
#include "GameMain.h"

#include "SunLine.h"

#include "SunLineManager.h"

//////////////////////////////////////////////////
// 名前空間省略
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CSunLineManager::CSunLineManager()
{
}

// デストラクタ
CSunLineManager::~CSunLineManager()
{
	// 中身解放
	BOOST_FOREACH( CSunLine* It, m_SunLineList )
	{
		SAFE_DELETE( It );
	}

	m_SunLineList.clear();
}

// 更新
void CSunLineManager::Update()
{
	// 一気に更新
	BOOST_FOREACH( CSunLine* It, m_SunLineList )
	{
		It->Update();
	}
}

// 描画
void CSunLineManager::Draw(LPDIRECT3DDEVICE9 lpdevice) const
{
	// レンダーステート設定
	// 加算合成で描画
	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// 座標変換は必要としない
	lpdevice->SetTransform( D3DTS_WORLD, &CMatrix() );

	// テクスチャを貼る
	lpdevice->SetTexture( 0, *CSTexManager::GetInst()->GetData( "SunLine.png" ) );

	// 一気に描画
	BOOST_FOREACH( CSunLine* It, m_SunLineList )
	{
		It->Draw( lpdevice );
	}

	// 加算合成解除
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}

// ランダム配置
void CSunLineManager::SetRandom(UINT num)
{
	// 動的確保していく
	CSunLine*	lpTemp = NULL;
	CVector		TempPos;
	UINT		Limit = static_cast<UINT>( Sequence::CGameMain::SEA_MOVELIMIT );

	srand( timeGetTime() );
	for( UINT i = 0; i < num; i++ )
	{
		lpTemp = NEW CSunLine();

		// 高さ固定で位置決め
		TempPos.Set(
			rand() % Limit * GetPosiOrNega(),
			Sequence::CGameMain::SURFACESEA_HEIGHT,
			rand() % Limit * GetPosiOrNega() );

		// 設定
		lpTemp->SetPos( TempPos );

		// 追加
		m_SunLineList.push_back( lpTemp );
	}
}