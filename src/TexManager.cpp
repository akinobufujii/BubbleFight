//////////////////////////////
// インクルード
//////////////////////////////
#include "TexManager.h"

//////////////////////////////
// 静的メンバ実体化
//////////////////////////////
CSTexManager* CSTexManager::m_lpInst = NULL;

//////////////////////////////
// 実装
//////////////////////////////

// コンストラクタ
CSTexManager::CSTexManager()
{
}

// デストラクタ
CSTexManager::~CSTexManager()
{
	AllRelease();
}

// 読み込み
bool CSTexManager::Load( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR name, D3DCOLOR color_key )
{
	// 何にもなければはじく
	if( name == NULL )
	{
		return false;
	}

	// 読み込み確認
	MAPTYPE::iterator It = m_TexMap.find( name );

	if( It != m_TexMap.end() )
	{
		return true;
	}

	// 読み込み
	CTexture* lpTemp = NEW CTexture();
	std::string Path( "Texture/" );

	Path += name;

	LPDIRECT3DTEXTURE9 lpTex = lpTemp->CreateFile( lpd3ddevice, Path.c_str(), color_key );

	// 中身があれば登録
	if( lpTex )
	{
		m_TexMap.insert( PAIRTYPE( name, lpTemp ) );
	
	}else{

		SAFE_DELETE( lpTemp );

		return false;
	}
	
	return true;
}

// 読み込みリストから読み込み
bool CSTexManager::LoadFromTextList(const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR list_pass)
{
	char Pass[ 0xff ];
	std::fstream File;

	// 読み込みリストオープン
	File.open( list_pass, std::ios::in );

	if( File.is_open() )
	{
		// 最後まで読み込み
		while( File.eof() == false )
		{
			File >> Pass;

			Load( lpd3ddevice, Pass );
		}
	
	}else{

		return false;
	}

	// ファイルクローズ
	File.close();

	return true;
}

// 情報獲得
CTexture* CSTexManager::GetData( const LPCSTR name )
{
	MAPTYPE::iterator It = m_TexMap.find( name );

	
	if( It == m_TexMap.end() )
	{
		return NULL;
	}

	return It->second;
}

// 解放
void CSTexManager::Release( const LPCSTR name )
{
	MAPTYPE::iterator It = m_TexMap.find( name );

	if( It != m_TexMap.end() )
	{
		SAFE_DELETE( It->second );

		m_TexMap.erase( It );
	}
}	

// 解放リストから解放
void CSTexManager::ReleaseFromTextList( const LPCSTR list_pass )
{
	char Pass[ 0xff ];
	std::fstream File;

	// 読み込みリストオープン
	File.open( list_pass, std::ios::in );

	if( File.is_open() )
	{
		// 最後まで読み込み
		while( File.eof() == false )
		{
			File >> Pass;

			Release( Pass );
		}	
	}

	// ファイルクローズ
	File.close();
}

// 全て解放
void CSTexManager::AllRelease()
{
	BOOST_FOREACH( PAIRTYPE It, m_TexMap )
	{
		SAFE_DELETE( It.second );
	}

	m_TexMap.clear();
}