
#include "Using.h"
#include "DirectGraphics.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

static LPDIRECT3D9				s_lpD3D;		// オブジェクト
static LPDIRECT3DDEVICE9		s_lpD3DDevice;	// デバイス
static D3DPRESENT_PARAMETERS	s_D3DPP;		// パラメータ
static LPDIRECT3DSURFACE9		s_lpBackBuffer;	// バックバッファのサーフェス
static LPDIRECT3DSURFACE9		s_lpZBuffer;	// Zバッファのサーフェス

// 初期化
bool D3D::Init( HWND hWnd, BOOL iswindow )
{
	// DirectX初期化
	s_lpD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( s_lpD3D == NULL )
	{
		return false;
	}

	// パラメータ設定
	s_D3DPP.BackBufferWidth				= WINDOW_WIDTH;							// バックバッファ幅
	s_D3DPP.BackBufferHeight			= WINDOW_HEIGHT;						// バックバッファ高さ
	s_D3DPP.BackBufferFormat			= D3DFMT_A8R8G8B8;						// バックバッファフォーマット
	s_D3DPP.BackBufferCount				= 1;									// バックバッファ数
	s_D3DPP.MultiSampleType				= D3DMULTISAMPLE_NONE;					// マルチサンプリングタイプ
	s_D3DPP.MultiSampleQuality			= 0;									// 品質レベル
	s_D3DPP.SwapEffect					= D3DSWAPEFFECT_FLIP;					// スワップエフェクト
	s_D3DPP.hDeviceWindow				= hWnd;									// ウィンドウハンドル
	s_D3DPP.Windowed					= iswindow;								// ウィンドウモード
	s_D3DPP.EnableAutoDepthStencil		= TRUE;									// サーフェスフォーマット
	s_D3DPP.AutoDepthStencilFormat		= D3DFMT_D24S8;							// ステンシルバッファフォーマット
	s_D3DPP.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;	// 画面フリップ時の効果オプション
	s_D3DPP.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;				// フルスクリーン時のリフレッシュレート
	s_D3DPP.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;			// フリップのタイミング設定

	// デバイス作成
	// ハードウェア HAL
	HRESULT hr = s_lpD3D->CreateDevice(
		D3DADAPTER_DEFAULT,					// アダプタ
		D3DDEVTYPE_HAL,						// デバイスタイプ
		hWnd,								// ウィンドウハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,// デバイス生成オプション
		&s_D3DPP,							// 設定したパラメータ
		&s_lpD3DDevice );					// 格納するデバイス

	// 失敗すれば ソフトウェア　HAL
	if( FAILED( hr ) )
	{
		hr = s_lpD3D->CreateDevice(
			D3DADAPTER_DEFAULT,					// アダプタ
			D3DDEVTYPE_HAL,						// デバイスタイプ
			hWnd,								// ウィンドウハンドル
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,// デバイス生成オプション
			&s_D3DPP,							// 設定したパラメータ
			&s_lpD3DDevice );					// 格納するデバイス

		// それでも失敗すれば　ソフトウェア　REF
		if( FAILED( hr ) )
		{
			hr = s_lpD3D->CreateDevice(
				D3DADAPTER_DEFAULT,					// アダプタ
				D3DDEVTYPE_REF,						// デバイスタイプ
				hWnd,								// ウィンドウハンドル
				D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,// デバイス生成オプション
				&s_D3DPP,							// 設定したパラメータ
				&s_lpD3DDevice );					// 格納するデバイス

			// ここで失敗すればもう無理
			if( ErrorMessage( hr, "デバイス作成失敗") )
			{
				return false;
			}
		}
	}

	// レンダーターゲット格納
	s_lpD3DDevice->GetRenderTarget( 0, &s_lpBackBuffer );
	s_lpD3DDevice->GetDepthStencilSurface( &s_lpZBuffer );

	// レンダーステート設定
	s_lpD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	s_lpD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_ALPHAREF, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

	// アルファ値の掛け算指定
	s_lpD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE );
	s_lpD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE );
	s_lpD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE );

	return true;
}

// 描画開始
bool D3D::Begin()
{
	if( FAILED( s_lpD3DDevice->BeginScene() ) )
	{
		return false;
	}

	return false;
}

// 描画終了
bool D3D::End()
{
	s_lpD3DDevice->EndScene();

	if( FAILED( s_lpD3DDevice->Present( NULL, NULL, NULL, NULL ) ) )
	{
		if( FAILED( s_lpD3DDevice->Reset( &s_D3DPP ) ) )
		{
			return false;
		}
	}

	return true;
}

// 画面クリア
void D3D::Clear( D3DCOLOR color, DWORD target )
{
	s_lpD3DDevice->Clear( 0, NULL, target, color, 1.f, 0 );
}

// 解放
void D3D::Release()
{
	SAFE_RELEASE( s_lpZBuffer );
	SAFE_RELEASE( s_lpBackBuffer );
	SAFE_RELEASE( s_lpD3DDevice );
	SAFE_RELEASE( s_lpD3D );
}

// デバイス獲得
const LPDIRECT3DDEVICE9 D3D::GetDevice()
{
	return s_lpD3DDevice;
}

// バックバッファのサーフェス獲得
const LPDIRECT3DSURFACE9 D3D::GetBackBufferSurcface()
{
	return s_lpBackBuffer;
}

const LPDIRECT3DSURFACE9 D3D::GetZBufer()
{
	return s_lpZBuffer;
}