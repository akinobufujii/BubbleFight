/////////////////////////////////
// インクルード
/////////////////////////////////
#include "SandSpray.h"

/////////////////////////////////
// 実装
/////////////////////////////////

// コンストラクタ
CSandSpray::CSandSpray()
{
}

// デストラクタ
CSandSpray::~CSandSpray()
{
	BOOST_FOREACH( CPointSpriteManager* It, m_SandList )
	{
		SAFE_DELETE( It );
	}

	m_SandList.clear();
}

// 更新
void CSandSpray::Update()
{
	// ※途中削除があるので通常ループ
	LISTTYPE::iterator	It = m_SandList.begin();

	while( It != m_SandList.end() )
	{
		// 更新
		(*It)->Update();

		// 用なしなら
		if( (*It)->IsErase() )
		{
			// リストから削除
			SAFE_DELETE( *It );
			It = m_SandList.erase( It );
			continue;
		}

		++It;
	}
}

// 描画
void CSandSpray::Draw(LPDIRECT3DDEVICE9 lp_device)
{
	BOOST_FOREACH( CPointSpriteManager* It, m_SandList )
	{
		It->Draw( lp_device );
	}
}

// 追加
void CSandSpray::Add(CPointSpriteManager *lp_sand)
{
	// 単純に追加
	m_SandList.push_back( lp_sand );
}

// こちらで設定して追加
void CSandSpray::AddDefault(const D3DXVECTOR3& pos)
{
	// マジックナンバー満載だが
	CPointSpriteManager* lpTemp = NEW CPointSpriteManager();

	// 設定
	lpTemp->SetType( CPointSpriteManager::TYPE_GRAVITYPOP );
	lpTemp->SetNum( 5 );
	lpTemp->SetEraseTime( 100 );
	lpTemp->SetTexture( *CSTexManager::GetInst()->GetData( "Spray.png" ) );
	lpTemp->SetPos( pos );
	lpTemp->SetMove( D3DXVECTOR3( 0.1f, 0.1f, 0.1f ) );
	lpTemp->SetScaleMax( 50 );

	// 追加
	m_SandList.push_back( lpTemp );
}