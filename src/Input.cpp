#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )

static LPDIRECTINPUT8	g_lpInput = NULL;	// �C���^�[�t�F�C�X

using namespace Input;

// �C���^�[�t�F�C�X�l��
bool Input::CreateInterFace( HINSTANCE hInst )
{
	HRESULT	hr = DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&g_lpInput, NULL);
	if( ErrorMessage( hr, "�C���v�b�g�C���^�[�t�F�C�X�쐬���s" ) )
	{
		return false;
	}

	return true;
}

// �C���^�[�t�F�C�X�j��
void Input::DestroyInterFase()
{
	SAFE_RELEASE( g_lpInput );
}

// �L�[�{�[�h�p�O���[�o���ϐ�
static LPDIRECTINPUTDEVICE8	g_lpKeyboard = NULL;	// �L�[�{�[�h�f�o�C�X

CONSTANT DWORD BUFFERSIZE	= 8;	// �o�b�t�@�T�C�Y
CONSTANT DWORD KEYBOARDSIZE	= 256;	// �L�[�{�[�h�T�C�Y

static BYTE g_KeyboardState[ KEYBOARDSIZE ];	// �L�[�{�[�h�̏��

// ������
bool Keyboard::Init( HWND hWnd )
{
	// �C���^�[�t�F�C�X�������
	if( g_lpInput )
	{
		// �L�[�{�[�h�f�o�C�X�쐬
		HRESULT hr = g_lpInput->CreateDevice( GUID_SysKeyboard, &g_lpKeyboard, NULL );
		if( ErrorMessage( hr, "�L�[�{�[�h�f�o�C�X�쐬���s" ) )
		{
			return false;
		}

		// �f�[�^�`���ݒ�
		hr = g_lpKeyboard->SetDataFormat( &c_dfDIKeyboard );
		if( ErrorMessage( hr, "�L�[�{�[�h�f�[�^�`���ݒ莸�s" ) )
		{
			return false;
		}

		// �������x���ݒ�
		g_lpKeyboard->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
		
		// �ڍאݒ�
		DIPROPDWORD diprop;
		diprop.diph.dwSize		= sizeof( DIPROPDWORD );
		diprop.diph.dwHeaderSize= sizeof( DIPROPHEADER );
		diprop.diph.dwObj		= 0;
		diprop.diph.dwHow		= DIPH_DEVICE;
		diprop.dwData			= BUFFERSIZE;	// �o�b�t�@�T�C�Y

		hr = g_lpKeyboard->SetProperty( DIPROP_BUFFERSIZE, &diprop.diph );
		if( ErrorMessage( hr, "�L�[�{�[�h���[�h�ڍאݒ莸�s" ) )
		{
			return false;
		}

		// ���͊J�n
		hr = g_lpKeyboard->Acquire();

		// ��ԏ�����
		for( int i = 0; i < KEYBOARDSIZE; i++ )
		{
			g_KeyboardState[ i ] = FREE;
		}

	}else{
		return false;
	}

	return true;
}

// �L�[�{�[�h���͏�ԍX�V
void Keyboard::UpdateState()
{
	BYTE KeyState[ KEYBOARDSIZE ];	// �L�[�{�[�h�̍��̓��͏��

	// �L�[�{�[�h��Ԋl��
	HRESULT hr = g_lpKeyboard->GetDeviceState( KEYBOARDSIZE, KeyState );

	// ��������Ώ�Ԃ��X�V
	if( SUCCEEDED( hr ) )
	{
		for( int i = 0; i < KEYBOARDSIZE; i++ )
		{
			// �{�^����Ԕ���
			if( KeyState[ i ] & 0x80 )// ������Ă��鎞
			{
				if( g_KeyboardState[ i ] == FREE || g_KeyboardState[ i ] == PULL )// �O�̃t���[���ŉ�����Ă��Ȃ����
				{
					g_KeyboardState[ i ] = PUSH;	// ������

				}else{
					g_KeyboardState[ i ] = HOLD;	// ������Ă���
				}
			}else{	// ������Ă��Ȃ��Ƃ�
				if( g_KeyboardState[ i ] == PUSH || g_KeyboardState[ i ] == HOLD )
				{
					g_KeyboardState[ i ] = PULL;	// �����ꂽ

				}else{
					g_KeyboardState[ i ] = FREE;	// ������Ă���
				}
			}
		}		
	
	}
}

// ���͐���
void Keyboard::SetAcquire( bool flg )
{
	// �^�Ȃ���͊J�n
	if( flg )
	{
		g_lpKeyboard->Acquire();
	
	}else{	// �U�Ȃ���͐��䂵�Ȃ�
		g_lpKeyboard->Unacquire();
	}
}

// ���͏�Ԏ擾
bool Keyboard::IsState( BYTE Key, BYTE State )
{
	if( (g_KeyboardState[ Key ] & State) )
	{
		return true;
	
	}else{

		return false;
	}
}

// ���
void Keyboard::Release()
{
	SAFE_RELEASE( g_lpKeyboard );
}



// �}�E�X�p�O���[�o���ϐ�
