// インクルード
#include "XfileManager.h"

// 静的メンバ実体化
CSXfileManager* CSXfileManager::m_lpInst = NULL;

// コンストラクタ
CSXfileManager::CSXfileManager()
{
}

// デストラクタ
CSXfileManager::~CSXfileManager()
{
	AllRelease();
}

// 読み込み
CXfile* CSXfileManager::Load( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR name, bool is_create_normals )
{
	if( name == NULL )
	{
		return NULL;
	}

	// すでに読み込まれているか調べる
	MAPTYPE::iterator It = m_XFileMap.find( name );

	if( It != m_XFileMap.end() )
	{
		return It->second;
	}

	// 読み込む
	std::string Pass( "XFile/" );

	Pass += name;

	CXfile* lpTemp = NEW CXfile( lpd3ddevice, Pass.c_str() );
	
	// 法線情報を作る
	if( is_create_normals )
	{
		lpTemp->CreateNormals();
	}

	m_XFileMap.insert( PAIRTYPE( name,  lpTemp ) );

	return lpTemp;
}

// 読み込みリストから読み込み
bool CSXfileManager::LoadFromTextList( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR list_pass )
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
CXfile*	CSXfileManager::GetData( LPCSTR name )
{
	MAPTYPE::iterator It = m_XFileMap.find( name );

	if( It == m_XFileMap.end() )
	{
		return NULL;
	}

	return It->second;
}

// 解放
void CSXfileManager::Release( LPCSTR name )
{
	MAPTYPE::iterator It = m_XFileMap.find( name );

	if( It != m_XFileMap.end() )
	{
		SAFE_DELETE( It->second );

		m_XFileMap.erase( It );
	}
}

// 解放リストから解放
void  CSXfileManager::ReleaseFromTextList( LPCSTR list_pass )
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
void CSXfileManager::AllRelease()
{
	BOOST_FOREACH( PAIRTYPE It, m_XFileMap )
	{
		SAFE_DELETE( It.second );
	}

	m_XFileMap.clear();
}