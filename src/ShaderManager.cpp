////////////////////////////////
// メソッド
////////////////////////////////
#include "Effect.h"

#include "ShaderManager.h"

////////////////////////////////
// 静的メンバ実体化
////////////////////////////////
CSShaderManager* CSShaderManager::m_lpInst	= NULL;

////////////////////////////////
// 実装
////////////////////////////////

// コンストラクタ
CSShaderManager::CSShaderManager()
{
}

// デストラクタ
CSShaderManager::~CSShaderManager()
{
	AllRelease();
}

// 読み込み
CEffect* CSShaderManager::Load(LPDIRECT3DDEVICE9 lp_device, LPCSTR path)
{
	MAPTYPE::iterator	It = m_ShaderMap.find( path );

	// あればそれを返す
	if( It != m_ShaderMap.end() )
	{
		return It->second;
	}

	// なければ読み込む
	std::string Path( "FX/" );
	Path += path;

	CEffect* lpTemp = NEW CEffect( lp_device, Path.c_str(), "Tec" );

	m_ShaderMap.insert( PAIRTYPE( path, lpTemp ) );

	return lpTemp;
}

// ファイルから一括読み込み
void CSShaderManager::LoadFromFile(LPDIRECT3DDEVICE9 lp_device, LPCSTR filepath)
{
	std::ifstream	In( filepath );
	std::string		Path;

	if( In.is_open() )
	{
		while( In.eof() == false )
		{
			In >> Path;

			Load( lp_device, Path.c_str() );
		}
	}
}

// 解放
void CSShaderManager::Release( LPCSTR path )
{
	MAPTYPE::iterator	It = m_ShaderMap.find( path );

	if( It != m_ShaderMap.end() )
	{
		SAFE_DELETE( It->second );

		m_ShaderMap.erase( It );
	}
}

// 全て開放
void CSShaderManager::AllRelease()
{
	BOOST_FOREACH( PAIRTYPE It, m_ShaderMap )
	{
		SAFE_DELETE( It.second );
	}

	m_ShaderMap.clear();
}

// 情報獲得
CEffect* CSShaderManager::GetData(LPCSTR path)
{
	MAPTYPE::iterator	It = m_ShaderMap.find( path );

	if( It != m_ShaderMap.end() )
	{
		return It->second;
	
	}else{

		return NULL;
	}
}