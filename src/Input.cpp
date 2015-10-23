#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )

static LPDIRECTINPUT8	g_lpInput = NULL;	// インターフェイス

using namespace Input;

// インターフェイス獲得
bool Input::CreateInterFace( HINSTANCE hInst )
{
	HRESULT	hr = DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&g_lpInput, NULL);
	if( ErrorMessage( hr, "インプットインターフェイス作成失敗" ) )
	{
		return false;
	}

	return true;
}

// インターフェイス破壊
void Input::DestroyInterFase()
{
	SAFE_RELEASE( g_lpInput );
}

// キーボード用グローバル変数
static LPDIRECTINPUTDEVICE8	g_lpKeyboard = NULL;	// キーボードデバイス

CONSTANT DWORD BUFFERSIZE	= 8;	// バッファサイズ
CONSTANT DWORD KEYBOARDSIZE	= 256;	// キーボードサイズ

static BYTE g_KeyboardState[ KEYBOARDSIZE ];	// キーボードの状態

// 初期化
bool Keyboard::Init( HWND hWnd )
{
	// インターフェイスがあれば
	if( g_lpInput )
	{
		// キーボードデバイス作成
		HRESULT hr = g_lpInput->CreateDevice( GUID_SysKeyboard, &g_lpKeyboard, NULL );
		if( ErrorMessage( hr, "キーボードデバイス作成失敗" ) )
		{
			return false;
		}

		// データ形式設定
		hr = g_lpKeyboard->SetDataFormat( &c_dfDIKeyboard );
		if( ErrorMessage( hr, "キーボードデータ形式設定失敗" ) )
		{
			return false;
		}

		// 強調レベル設定
		g_lpKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
		
		// 詳細設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize		= sizeof( DIPROPDWORD );
		diprop.diph.dwHeaderSize= sizeof( DIPROPHEADER );
		diprop.diph.dwObj		= 0;
		diprop.diph.dwHow		= DIPH_DEVICE;
		diprop.dwData			= BUFFERSIZE;	// バッファサイズ

		hr = g_lpKeyboard->SetProperty( DIPROP_BUFFERSIZE, &diprop.diph );
		if( ErrorMessage( hr, "キーボードモード詳細設定失敗" ) )
		{
			return false;
		}

		// 入力開始
		hr = g_lpKeyboard->Acquire();

		// 状態初期化
		for( int i = 0; i < KEYBOARDSIZE; i++ )
		{
			g_KeyboardState[ i ] = FREE;
		}

	}else{
		return false;
	}

	return true;
}

// キーボード入力状態更新
void Keyboard::UpdateState()
{
	BYTE KeyState[ KEYBOARDSIZE ];	// キーボードの今の入力状態

	// キーボード状態獲得
	HRESULT hr = g_lpKeyboard->GetDeviceState( KEYBOARDSIZE, KeyState );

	// 成功すれば状態を更新
	if( SUCCEEDED( hr ) )
	{
		for( int i = 0; i < KEYBOARDSIZE; i++ )
		{
			// ボタン状態判定
			if( KeyState[ i ] & 0x80 )// 押されている時
			{
				if( g_KeyboardState[ i ] == FREE || g_KeyboardState[ i ] == PULL )// 前のフレームで押されていなければ
				{
					g_KeyboardState[ i ] = PUSH;	// 押した

				}else{
					g_KeyboardState[ i ] = HOLD;	// 押されている
				}
			}else{	// 押されていないとき
				if( g_KeyboardState[ i ] == PUSH || g_KeyboardState[ i ] == HOLD )
				{
					g_KeyboardState[ i ] = PULL;	// 離された

				}else{
					g_KeyboardState[ i ] = FREE;	// 離されている
				}
			}
		}		
	
	}
}

// 入力制御
void Keyboard::SetAcquire( bool flg )
{
	// 真なら入力開始
	if( flg )
	{
		g_lpKeyboard->Acquire();
	
	}else{	// 偽なら入力制御しない
		g_lpKeyboard->Unacquire();
	}
}

// 入力状態取得
bool Keyboard::IsState( BYTE Key, BYTE State )
{
	if( (g_KeyboardState[ Key ] & State) )
	{
		return true;
	
	}else{

		return false;
	}
}

// 解放
void Keyboard::Release()
{
	SAFE_RELEASE( g_lpKeyboard );
}



// マウス用グローバル変数
