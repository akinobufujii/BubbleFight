#include "Textures.h"

// コンストラクタ
CTexture::CTexture():
m_lpTex( NULL ),
m_lpSurface( NULL ),
m_lpZbuffer( NULL )
{
	ZeroMemory( &m_Desc, sizeof( m_Desc ) );
}

// ファイルからテクスチャ作成
const LPDIRECT3DTEXTURE9 CTexture::CreateFile( LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, D3DCOLOR color_key )
{
	// 開放しておく
	if( m_lpTex )
	{
		this->~CTexture();
	}

	// テクスチャ読み込み
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

// レンダリングターゲット用テクスチャ作成
const LPDIRECT3DTEXTURE9 CTexture::CreateRenderTarget( LPDIRECT3DDEVICE9 lpdevice, UINT width, UINT height, D3DFORMAT format )
{
	// 開放しておく
	this->~CTexture();

	// 指定されたサイズでテクスチャ作成
	D3DXCreateTexture(
		lpdevice,
		width,
		height,
		1,
		D3DUSAGE_RENDERTARGET,
		format,
		D3DPOOL_DEFAULT,
		&m_lpTex );

	// 深度バッファ作成
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
		// テクスチャからレンダリングターゲット用サーフェス作成
		m_lpTex->GetSurfaceLevel( 0, &m_lpSurface );

		m_lpTex->GetLevelDesc( 0, &m_Desc );
	
	}else{

		return NULL;
	}
	
	return m_lpTex;
}

// デストラクタ
CTexture::~CTexture()
{
	SAFE_RELEASE( m_lpSurface );
	SAFE_RELEASE( m_lpZbuffer );
	SAFE_RELEASE( m_lpTex );
}