////////////////////////////
// インクルード
////////////////////////////
#include "WaterSpray.h"

////////////////////////////
// 実装
////////////////////////////

// コンストラクタ
CWaterSpray::CWaterSpray()
{
}

// デストラクタ
CWaterSpray::~CWaterSpray()
{
	BOOST_FOREACH( CPointSpriteManager* It, m_SprayList )
	{
		SAFE_DELETE( It );
	}

	m_SprayList.clear();
}

// 更新
void CWaterSpray::Update()
{
	// 一気に更新
	BOOST_FOREACH( CPointSpriteManager* It, m_SprayList )
	{
		It->Update();
	}
}

// 描画
void CWaterSpray::Draw(const LPDIRECT3DDEVICE9 lp_device)
{
	// 一気に描画
	BOOST_FOREACH( CPointSpriteManager* It, m_SprayList )
	{
		It->Draw( lp_device );
	}
}

// 生成
void CWaterSpray::Create(const D3DXVECTOR3 &pos)
{
	// もし使えるなら使いまわす
	bool IsCreate = true;

	BOOST_FOREACH( CPointSpriteManager* It, m_SprayList )
	{
		if( It->IsErase() )
		{
			It->SetPos( pos );
			It->Reset();
			IsCreate = false;
			break;
		}
	}

	// いっぱいなら追加する
	if( IsCreate )
	{
		CPointSpriteManager* lpTemp = NEW CPointSpriteManager();

		lpTemp->SetNum( 100 );
		lpTemp->SetTexture( CSTexManager::GetInst()->GetData( "Spray.png" )->GetTexture() );
		lpTemp->SetEraseTime( 1000 );
		lpTemp->SetMove( D3DXVECTOR3( 1, 1, 1 ) );
		lpTemp->SetScaleMax( 10.f );
		lpTemp->SetPos( pos );

		// 追加
		m_SprayList.push_back( lpTemp );
	}
}