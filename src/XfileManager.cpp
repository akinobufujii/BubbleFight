// �C���N���[�h
#include "XfileManager.h"

// �ÓI�����o���̉�
CSXfileManager* CSXfileManager::m_lpInst = NULL;

// �R���X�g���N�^
CSXfileManager::CSXfileManager()
{
}

// �f�X�g���N�^
CSXfileManager::~CSXfileManager()
{
	AllRelease();
}

// �ǂݍ���
CXfile* CSXfileManager::Load( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR name, bool is_create_normals )
{
	if( name == NULL )
	{
		return NULL;
	}

	// ���łɓǂݍ��܂�Ă��邩���ׂ�
	MAPTYPE::iterator It = m_XFileMap.find( name );

	if( It != m_XFileMap.end() )
	{
		return It->second;
	}

	// �ǂݍ���
	std::string Pass( "XFile/" );

	Pass += name;

	CXfile* lpTemp = NEW CXfile( lpd3ddevice, Pass.c_str() );
	
	// �@���������
	if( is_create_normals )
	{
		lpTemp->CreateNormals();
	}

	m_XFileMap.insert( PAIRTYPE( name,  lpTemp ) );

	return lpTemp;
}

// �ǂݍ��݃��X�g����ǂݍ���
bool CSXfileManager::LoadFromTextList( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR list_pass )
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
CXfile*	CSXfileManager::GetData( LPCSTR name )
{
	MAPTYPE::iterator It = m_XFileMap.find( name );

	if( It == m_XFileMap.end() )
	{
		return NULL;
	}

	return It->second;
}

// ���
void CSXfileManager::Release( LPCSTR name )
{
	MAPTYPE::iterator It = m_XFileMap.find( name );

	if( It != m_XFileMap.end() )
	{
		SAFE_DELETE( It->second );

		m_XFileMap.erase( It );
	}
}

// ������X�g������
void  CSXfileManager::ReleaseFromTextList( LPCSTR list_pass )
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
void CSXfileManager::AllRelease()
{
	BOOST_FOREACH( PAIRTYPE It, m_XFileMap )
	{
		SAFE_DELETE( It.second );
	}

	m_XFileMap.clear();
}