//////////////////////////////
// �C���N���[�h
//////////////////////////////
#include "TexManager.h"

//////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////
CSTexManager* CSTexManager::m_lpInst = NULL;

//////////////////////////////
// ����
//////////////////////////////

// �R���X�g���N�^
CSTexManager::CSTexManager()
{
}

// �f�X�g���N�^
CSTexManager::~CSTexManager()
{
	AllRelease();
}

// �ǂݍ���
bool CSTexManager::Load( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR name, D3DCOLOR color_key )
{
	// ���ɂ��Ȃ���΂͂���
	if( name == NULL )
	{
		return false;
	}

	// �ǂݍ��݊m�F
	MAPTYPE::iterator It = m_TexMap.find( name );

	if( It != m_TexMap.end() )
	{
		return true;
	}

	// �ǂݍ���
	CTexture* lpTemp = NEW CTexture();
	std::string Path( "Texture/" );

	Path += name;

	LPDIRECT3DTEXTURE9 lpTex = lpTemp->CreateFile( lpd3ddevice, Path.c_str(), color_key );

	// ���g������Γo�^
	if( lpTex )
	{
		m_TexMap.insert( PAIRTYPE( name, lpTemp ) );
	
	}else{

		SAFE_DELETE( lpTemp );

		return false;
	}
	
	return true;
}

// �ǂݍ��݃��X�g����ǂݍ���
bool CSTexManager::LoadFromTextList(const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR list_pass)
{
	char Pass[ 0xff ];
	std::fstream File;

	// �ǂݍ��݃��X�g�I�[�v��
	File.open( list_pass, std::ios::in );

	if( File.is_open() )
	{
		// �Ō�܂œǂݍ���
		while( File.eof() == false )
		{
			File >> Pass;

			Load( lpd3ddevice, Pass );
		}
	
	}else{

		return false;
	}

	// �t�@�C���N���[�Y
	File.close();

	return true;
}

// ���l��
CTexture* CSTexManager::GetData( const LPCSTR name )
{
	MAPTYPE::iterator It = m_TexMap.find( name );

	
	if( It == m_TexMap.end() )
	{
		return NULL;
	}

	return It->second;
}

// ���
void CSTexManager::Release( const LPCSTR name )
{
	MAPTYPE::iterator It = m_TexMap.find( name );

	if( It != m_TexMap.end() )
	{
		SAFE_DELETE( It->second );

		m_TexMap.erase( It );
	}
}	

// ������X�g������
void CSTexManager::ReleaseFromTextList( const LPCSTR list_pass )
{
	char Pass[ 0xff ];
	std::fstream File;

	// �ǂݍ��݃��X�g�I�[�v��
	File.open( list_pass, std::ios::in );

	if( File.is_open() )
	{
		// �Ō�܂œǂݍ���
		while( File.eof() == false )
		{
			File >> Pass;

			Release( Pass );
		}	
	}

	// �t�@�C���N���[�Y
	File.close();
}

// �S�ĉ��
void CSTexManager::AllRelease()
{
	BOOST_FOREACH( PAIRTYPE It, m_TexMap )
	{
		SAFE_DELETE( It.second );
	}

	m_TexMap.clear();
}