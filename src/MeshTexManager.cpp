//////////////////////////
// �C���N���[�h
//////////////////////////
#include "MeshTexManager.h"

//////////////////////////
// �ÓI�����o���̉�
//////////////////////////
CSMeshTexManager*	CSMeshTexManager::m_lpInst	= NULL;

//////////////////////////
// ����
//////////////////////////

// �f�X�g���N�^
CSMeshTexManager::~CSMeshTexManager()
{
	MAPTYPE::iterator It = m_TexMap.begin();

	BOOST_FOREACH( PAIRTYPE It, m_TexMap )
	{
		SAFE_RELEASE( It.second );
	}

	m_TexMap.clear();
}

// �ǂݍ���
LPDIRECT3DTEXTURE9 CSMeshTexManager::Load(const LPDIRECT3DDEVICE9 lp_device, const LPSTR path)
{
	// �p�X���Ȃ���Ή������Ȃ�
	if( path == NULL )
	{
		return NULL;
	}

	MAPTYPE::iterator It = m_TexMap.find( path );

	// ���łɂ����
	if( It != m_TexMap.end() )
	{
		return It->second;
	}

	// �Ȃ���Γǂݍ���
	LPDIRECT3DTEXTURE9 lpTemp = NULL;

	D3DXCreateTextureFromFile( lp_device, path, &lpTemp );

	m_TexMap.insert( PAIRTYPE( path, lpTemp ) );

	return lpTemp;
}