//////////////////////////
// インクルード
//////////////////////////
#include "MeshTexManager.h"

//////////////////////////
// 静的メンバ実体化
//////////////////////////
CSMeshTexManager*	CSMeshTexManager::m_lpInst	= NULL;

//////////////////////////
// 実装
//////////////////////////

// デストラクタ
CSMeshTexManager::~CSMeshTexManager()
{
	MAPTYPE::iterator It = m_TexMap.begin();

	BOOST_FOREACH( PAIRTYPE It, m_TexMap )
	{
		SAFE_RELEASE( It.second );
	}

	m_TexMap.clear();
}

// 読み込み
LPDIRECT3DTEXTURE9 CSMeshTexManager::Load(const LPDIRECT3DDEVICE9 lp_device, const LPSTR path)
{
	// パスがなければ何もしない
	if( path == NULL )
	{
		return NULL;
	}

	MAPTYPE::iterator It = m_TexMap.find( path );

	// すでにあれば
	if( It != m_TexMap.end() )
	{
		return It->second;
	}

	// なければ読み込み
	LPDIRECT3DTEXTURE9 lpTemp = NULL;

	D3DXCreateTextureFromFile( lp_device, path, &lpTemp );

	m_TexMap.insert( PAIRTYPE( path, lpTemp ) );

	return lpTemp;
}