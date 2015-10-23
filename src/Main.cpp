/////////////////////////////////////
// インクルード
/////////////////////////////////////
#include "../resource.h"
#include "Loading.h"
#include "SequenceManager.h"

/////////////////////////////////////
// 関数プロトタイプ宣言
/////////////////////////////////////

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

// ウィンドウクラス設計
HWND SetupWindowClass( HINSTANCE hInst );

/////////////////////////////////////
// エントリーポイント
/////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// メモリリークチェック
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	CoInitialize( NULL );

	// ウィンドウクラス設計
	HWND hWnd = SetupWindowClass( hInstance );

	if( hWnd == NULL )
	{
		return -1;
	}

	// フルスクリーンかどうかを聞く
	BOOL IsWindow = TRUE;
	
	if( MessageBox( hWnd, "フルスクリーンで起動しますか？", "起動方法確認", MB_YESNO ) == IDYES )
	{
		IsWindow = FALSE;
	}

	// DirectX初期化
	if( D3D::Init( hWnd, IsWindow ) == false )
	{
		return -1;
	}

	// インプット初期化
	Input::CreateInterFace( hInstance );
	Input::Keyboard::Init( hWnd );
	//Input::Mouse::Init( hWnd );

	// サウンド初期化
	CSoundManager::Create();
	CSoundManager::GetInst()->Init( hWnd );

	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	// シングルトン作成
	CSXfileManager::Create();
	CSTexManager::Create();
	CSSyncro::Create();
	CSMeshTexManager::Create();
	CSShaderManager::Create();

	//　ハードウェアチェック
	/*D3DCAPS9 Caps;
	D3D::GetDevice()->GetDeviceCaps( &Caps );

	char Str[ 0xff ];

	sprintf_s( Str, sizeof( Str ), "VertexShaderUse = %s\nPixelShaderUse = %s",
		( Caps.VertexShaderVersion > D3DVS_VERSION( 2, 0 ) )? "OK" : "NO",
		( Caps.PixelShaderVersion > D3DPS_VERSION( 2, 0 )  )? "OK" : "NO");*/

	//MessageBox( NULL, Str, "Info", MB_OK );

	// シーケンス作成
	Sequence::CManager*	GameMain = NEW Sequence::CManager();

	// ゲームループ
	MSG msg;

	timeBeginPeriod(1);

	ShowCursor( FALSE );

	while( true )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			GetMessage( &msg, NULL, 0, 0 );
			TranslateMessage( &msg );
			DispatchMessage( &msg );

			// もし終了なら
			if( msg.message == WM_QUIT )
			{
				break;
			}
		
		}else{

			if( GameMain->GameLoop() == false )
			{
				break;
			}
		}
	}

	ShowCursor( TRUE );

	// シーケンス破壊
	SAFE_DELETE( GameMain );

	// シングルトン破壊
	CSShaderManager::Destroy();
	CSXfileManager::Destroy();
	CSTexManager::Destroy();
	CSSyncro::Destroy();
	CSMeshTexManager::Destroy();

	// サウンド解放
	CSoundManager::Destroy();

	// インプット解放
	Input::DestroyInterFase();
	Input::Keyboard::Release();

	// DirectX解放
	D3D::Release();

	CoUninitialize();

	// ウィンドウクラス解除
	UnregisterClass( CLASSNAME, hInstance );

	return 0;
}

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// メッセージ処理
	switch( msg )
	{
	case WM_DESTROY:	// ウィンドウが破棄されたとき
		PostQuitMessage( 0 );
		break;

	case WM_KEYDOWN:	// キーボード押されたとき
		switch( wParam )
		{
		case VK_ESCAPE:
			PostQuitMessage( 0 );
			break;
		}
		
	default:	// 指定されたメッセージ以外の時
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}

	return 0;
}

// ウィンドウクラス設計
HWND SetupWindowClass( HINSTANCE hInst )
{
	WNDCLASSEX wc;
	wc.cbSize             = sizeof( WNDCLASSEX );			// 構造体に必要なバイト数
	wc.style              = 0;								// ウィンドウスタイル
	wc.lpfnWndProc        = WndProc;						// ウィンドウプロシージャ
	wc.cbClsExtra         = 0;								// この構造体に割り当てる余分なメモリ
	wc.cbWndExtra         = 0;								// ウィンドウインスタンスに割り当てる余分なメモリ
	wc.hInstance          = hInst;							// インスタンスハンドル
	wc.hIcon              = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON1 ) );							// アイコン
	wc.hCursor            = LoadCursor( hInst, IDC_ARROW );	// カーソル
	wc.hbrBackground      = ( HBRUSH )( COLOR_WINDOW + 1 );	// 背景色
	wc.lpszMenuName       = NULL;							// メニューハンドル
	wc.lpszClassName      = CLASSNAME;						// クラス名
	wc.hIconSm            = NULL;							// スモールアイコン

	// 設計クラス登録
	if( RegisterClassEx( &wc ) == NULL )
	{
		return NULL;
	}

	// デスクトップサイズ取得
	RECT Size;
	GetWindowRect( GetDesktopWindow(), &Size );

	// ウィンドウ作成
	HWND hWnd = CreateWindow(
		CLASSNAME,							// クラス名
		CLASSNAME,							// タイトルバー文字列

		// ウィンドウスタイル(サイズ変更無効)
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		//WS_OVERLAPPEDWINDOW,
		//WS_POPUP,
		Size.right  / 2 - WINDOW_WIDTH  / 2,		// x座標
		Size.bottom / 2 - WINDOW_HEIGHT / 2,		// y座標

		// クライアント領域を考えて幅高さ指定
		WINDOW_WIDTH,//  + GetSystemMetrics( SM_CXFRAME ) * 2,
		WINDOW_HEIGHT,// + GetSystemMetrics( SM_CYFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION ),
		NULL,								// 親ハンドル
		NULL,								// メニューハンドル
		hInst,								// インスタンスハンドル
		NULL								// 特殊データ
		);

	return hWnd;
}