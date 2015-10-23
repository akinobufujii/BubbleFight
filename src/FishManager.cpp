///////////////////////////////
// インクルード
///////////////////////////////
#include "ObjectFish.h"

#include "DamageObjectBase.h"
#include "DamageFish.h"

#include "FishManager.h"

///////////////////////////////
// 静的メンバ実体化
///////////////////////////////
CStoneManager* CFishManager::m_lpStones = NULL;

///////////////////////////////
// 実装
///////////////////////////////

// コンストラクタ
CFishManager::CFishManager()
{
}

// デストラクタ
CFishManager::~CFishManager()
{
	AllRelease();
}

// 更新
void CFishManager::Update( bool is_collision )
{
	// オブジェクト
	BOOST_FOREACH( CObjectFish* Fish, m_FishList )
	{
		Fish->Update( is_collision );
	}

	// ダメージを受けているオブジェクト
	BOOST_FOREACH( CDamageObjectBase* Fish, m_DamageObjectList )
	{
		Fish->Update();
	}
}

// 単なるオブジェクトの魚描画
void CFishManager::DrawObjectFish(const LPDIRECT3DDEVICE9 lp_device)
{
	// オブジェクト
	lp_device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	
	BOOST_FOREACH( CObjectFish* Fish, m_FishList )
	{
		Fish->Draw( lp_device, m_Mat );
	}

	lp_device->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
}

// ダメージを受けている魚の描画
void CFishManager::DrawDamageFish( const LPDIRECT3DDEVICE9 lp_device, bool is_shader )
{
	// ダメージを受けているオブジェクト
	BOOST_FOREACH( CDamageObjectBase* Fish, m_DamageObjectList )
	{
		Fish->Draw( lp_device, is_shader );
	}
}

// 魚数と範囲を設定
void CFishManager::SetNumAndLimit(const int num, const D3DXVECTOR3 &pos_limit, const int rot)
{
	D3DXVECTOR3 Pos;

	// 指定された分だけ回す
	for( int i = 0; i < num; i++ )
	{
		CObjectFish* lpTemp = NEW CObjectFish();

		Pos.x = (rand() % static_cast<int>(pos_limit.x) + 1) * GetPosiOrNega();
		Pos.y = (rand() % static_cast<int>(pos_limit.y) + 1) * GetPosiOrNega();
		Pos.z = (rand() % static_cast<int>(pos_limit.z) + 1) * GetPosiOrNega();
		
		lpTemp->SetPos( Pos );

		if( rot == -1 )
		{
			lpTemp->SetRotY( rand() % 360 );
		
		}else{

			lpTemp->SetRotY( rot );
		}
		
		m_FishList.push_back( lpTemp );
	}
}

// ダメージを受ける魚を読み込む
void CFishManager::LoadDamageFish(LPCSTR load_path)
{
	std::ifstream	File( load_path );

	if( File.is_open() )
	{
		CDamageObjectBase*	lpTemp = NULL;	// テンポラリ
		D3DXVECTOR3			LoadPos;		// 位置
		int					Kind;			// 種類
		float				Scale;			// 拡大率
		float				Speed;			// 速さ
		std::string			Path;			// メッシュパス

		while( File.eof() == false )
		{
			File >> Kind;

			// エラーチェック
			if( Kind < 0 || Kind >= KINDMAX )
			{
				continue;
			}

			// 生み出し分岐
			switch( Kind )
			{
			case KIND_FISH:
				lpTemp = NEW CDamageFish();
				break;
			}

			// 位置設定
			File >> LoadPos.x;
			File >> LoadPos.y;
			File >> LoadPos.z;

			lpTemp->SetPos( LoadPos );

			// 拡大率設定
			File >> Scale;

			lpTemp->SetScale( Scale );

			// 速さ設定
			File >> Speed;

			lpTemp->SetSpeed( Speed );

			// メッシュ設定
			File >> Path;

			lpTemp->SetMeshPath( D3D::GetDevice(), Path.c_str() );

			m_DamageObjectList.push_back( lpTemp );

			// ※念のため
			lpTemp = NULL;
		}
	}
}

// 回転設定
void CFishManager::SetRot( const int rot )
{
	BOOST_FOREACH( CObjectFish* Fish, m_FishList )
	{
		Fish->SetRotY( rot );
	}
}

// 画面から出たかどうか
bool CFishManager::IsAllOut()
{
	BOOST_FOREACH( CObjectFish* Fish, m_FishList )
	{
		if( IsCulling( &Fish->GetMatrix() ) )
		{
			return false;
		}	
	}

	return true;
}

// 空にする
void CFishManager::AllRelease()
{
	BOOST_FOREACH( CObjectFish* Fish, m_FishList )
	{
		SAFE_DELETE( Fish );
	}

	m_FishList.clear();

	BOOST_FOREACH( CDamageObjectBase* Fish, m_DamageObjectList )
	{
		SAFE_DELETE( Fish );
	}

	m_DamageObjectList.clear();
}