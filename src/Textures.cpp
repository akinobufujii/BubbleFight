#include "Textures.h"

// �R���X�g���N�^
CTexture::CTexture():
m_lpTex( NULL ),
m_lpSurface( NULL ),
m_lpZbuffer( NULL )
{
	ZeroMemory( &m_Desc, sizeof( m_Desc ) );
}

// �t�@�C������e�N�X�`���쐬
const LPDIRECT3DTEXTURE9 CTexture::CreateFile( LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, D3DCOLOR color_key )
{
	// �J�����Ă���
	if( m_lpTex )
	{
		this->~CTexture();
	}

	// �e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFileEx(
		lpdevice,
		file_name,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		color_key,
		NULL,
		NULL,
		&m_lpTex);

	if( m_lpTex )
	{
		m_lpTex->GetLevelDesc( 0, &m_Desc );
	
	}else{

		return NULL;
	}
	
	return m_lpTex;
}

// �����_�����O�^�[�Q�b�g�p�e�N�X�`���쐬
const LPDIRECT3DTEXTURE9 CTexture::CreateRenderTarget( LPDIRECT3DDEVICE9 lpdevice, UINT width, UINT height, D3DFORMAT format )
{
	// �J�����Ă���
	this->~CTexture();

	// �w�肳�ꂽ�T�C�Y�Ńe�N�X�`���쐬
	D3DXCreateTexture(
		lpdevice,
		width,
		height,
		1,
		D3DUSAGE_RENDERTARGET,
		format,
		D3DPOOL_DEFAULT,
		&m_lpTex );

	// �[�x�o�b�t�@�쐬
	lpdevice->CreateDepthStencilSurface( 
		width,
		height,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_lpZbuffer,
		NULL );

	if( m_lpTex )
	{
		// �e�N�X�`�����烌���_�����O�^�[�Q�b�g�p�T�[�t�F�X�쐬
		m_lpTex->GetSurfaceLevel( 0, &m_lpSurface );

		m_lpTex->GetLevelDesc( 0, &m_Desc );
	
	}else{

		return NULL;
	}
	
	return m_lpTex;
}

// �f�X�g���N�^
CTexture::~CTexture()
{
	SAFE_RELEASE( m_lpSurface );
	SAFE_RELEASE( m_lpZbuffer );
	SAFE_RELEASE( m_lpTex );
}