////////////////////////////////
// ���\�b�h
////////////////////////////////
#include "Effect.h"

#include "ShaderManager.h"

////////////////////////////////
// �ÓI�����o���̉�
////////////////////////////////
CSShaderManager* CSShaderManager::m_lpInst	= NULL;

////////////////////////////////
// ����
////////////////////////////////

// �R���X�g���N�^
CSShaderManager::CSShaderManager()
{
}

// �f�X�g���N�^
CSShaderManager::~CSShaderManager()
{
	AllRelease();
}

// �ǂݍ���
CEffect* CSShaderManager::Load(LPDIRECT3DDEVICE9 lp_device, LPCSTR path)
{
	MAPTYPE::iterator	It = m_ShaderMap.find( path );

	// ����΂����Ԃ�
	if( It != m_ShaderMap.end() )
	{
		return It->second;
	}

	// �Ȃ���Γǂݍ���
	std::string Path( "FX/" );
	Path += path;

	CEffect* lpTemp = NEW CEffect( lp_device, Path.c_str(), "Tec" );

	m_ShaderMap.insert( PAIRTYPE( path, lpTemp ) );

	return lpTemp;
}

// �t�@�C������ꊇ�ǂݍ���
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

// ���
void CSShaderManager::Release( LPCSTR path )
{
	MAPTYPE::iterator	It = m_ShaderMap.find( path );

	if( It != m_ShaderMap.end() )
	{
		SAFE_DELETE( It->second );

		m_ShaderMap.erase( It );
	}
}

// �S�ĊJ��
void CSShaderManager::AllRelease()
{
	BOOST_FOREACH( PAIRTYPE It, m_ShaderMap )
	{
		SAFE_DELETE( It.second );
	}

	m_ShaderMap.clear();
}

// ���l��
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