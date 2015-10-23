/////////////////////////////////////
// �C���N���[�h
/////////////////////////////////////
#include "../resource.h"
#include "Loading.h"
#include "SequenceManager.h"

/////////////////////////////////////
// �֐��v���g�^�C�v�錾
/////////////////////////////////////

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

// �E�B���h�E�N���X�݌v
HWND SetupWindowClass( HINSTANCE hInst );

/////////////////////////////////////
// �G���g���[�|�C���g
/////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	// ���������[�N�`�F�b�N
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	CoInitialize( NULL );

	// �E�B���h�E�N���X�݌v
	HWND hWnd = SetupWindowClass( hInstance );

	if( hWnd == NULL )
	{
		return -1;
	}

	// �t���X�N���[�����ǂ����𕷂�
	BOOL IsWindow = TRUE;
	
	if( MessageBox( hWnd, "�t���X�N���[���ŋN�����܂����H", "�N�����@�m�F", MB_YESNO ) == IDYES )
	{
		IsWindow = FALSE;
	}

	// DirectX������
	if( D3D::Init( hWnd, IsWindow ) == false )
	{
		return -1;
	}

	// �C���v�b�g������
	Input::CreateInterFace( hInstance );
	Input::Keyboard::Init( hWnd );
	//Input::Mouse::Init( hWnd );

	// �T�E���h������
	CSoundManager::Create();
	CSoundManager::GetInst()->Init( hWnd );

	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	// �V���O���g���쐬
	CSXfileManager::Create();
	CSTexManager::Create();
	CSSyncro::Create();
	CSMeshTexManager::Create();
	CSShaderManager::Create();

	//�@�n�[�h�E�F�A�`�F�b�N
	/*D3DCAPS9 Caps;
	D3D::GetDevice()->GetDeviceCaps( &Caps );

	char Str[ 0xff ];

	sprintf_s( Str, sizeof( Str ), "VertexShaderUse = %s\nPixelShaderUse = %s",
		( Caps.VertexShaderVersion > D3DVS_VERSION( 2, 0 ) )? "OK" : "NO",
		( Caps.PixelShaderVersion > D3DPS_VERSION( 2, 0 )  )? "OK" : "NO");*/

	//MessageBox( NULL, Str, "Info", MB_OK );

	// �V�[�P���X�쐬
	Sequence::CManager*	GameMain = NEW Sequence::CManager();

	// �Q�[�����[�v
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

			// �����I���Ȃ�
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

	// �V�[�P���X�j��
	SAFE_DELETE( GameMain );

	// �V���O���g���j��
	CSShaderManager::Destroy();
	CSXfileManager::Destroy();
	CSTexManager::Destroy();
	CSSyncro::Destroy();
	CSMeshTexManager::Destroy();

	// �T�E���h���
	CSoundManager::Destroy();

	// �C���v�b�g���
	Input::DestroyInterFase();
	Input::Keyboard::Release();

	// DirectX���
	D3D::Release();

	CoUninitialize();

	// �E�B���h�E�N���X����
	UnregisterClass( CLASSNAME, hInstance );

	return 0;
}

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// ���b�Z�[�W����
	switch( msg )
	{
	case WM_DESTROY:	// �E�B���h�E���j�����ꂽ�Ƃ�
		PostQuitMessage( 0 );
		break;

	case WM_KEYDOWN:	// �L�[�{�[�h�����ꂽ�Ƃ�
		switch( wParam )
		{
		case VK_ESCAPE:
			PostQuitMessage( 0 );
			break;
		}
		
	default:	// �w�肳�ꂽ���b�Z�[�W�ȊO�̎�
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}

	return 0;
}

// �E�B���h�E�N���X�݌v
HWND SetupWindowClass( HINSTANCE hInst )
{
	WNDCLASSEX wc;
	wc.cbSize             = sizeof( WNDCLASSEX );			// �\���̂ɕK�v�ȃo�C�g��
	wc.style              = 0;								// �E�B���h�E�X�^�C��
	wc.lpfnWndProc        = WndProc;						// �E�B���h�E�v���V�[�W��
	wc.cbClsExtra         = 0;								// ���̍\���̂Ɋ��蓖�Ă�]���ȃ�����
	wc.cbWndExtra         = 0;								// �E�B���h�E�C���X�^���X�Ɋ��蓖�Ă�]���ȃ�����
	wc.hInstance          = hInst;							// �C���X�^���X�n���h��
	wc.hIcon              = LoadIcon( hInst, MAKEINTRESOURCE( IDI_ICON1 ) );							// �A�C�R��
	wc.hCursor            = LoadCursor( hInst, IDC_ARROW );	// �J�[�\��
	wc.hbrBackground      = ( HBRUSH )( COLOR_WINDOW + 1 );	// �w�i�F
	wc.lpszMenuName       = NULL;							// ���j���[�n���h��
	wc.lpszClassName      = CLASSNAME;						// �N���X��
	wc.hIconSm            = NULL;							// �X���[���A�C�R��

	// �݌v�N���X�o�^
	if( RegisterClassEx( &wc ) == NULL )
	{
		return NULL;
	}

	// �f�X�N�g�b�v�T�C�Y�擾
	RECT Size;
	GetWindowRect( GetDesktopWindow(), &Size );

	// �E�B���h�E�쐬
	HWND hWnd = CreateWindow(
		CLASSNAME,							// �N���X��
		CLASSNAME,							// �^�C�g���o�[������

		// �E�B���h�E�X�^�C��(�T�C�Y�ύX����)
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		//WS_OVERLAPPEDWINDOW,
		//WS_POPUP,
		Size.right  / 2 - WINDOW_WIDTH  / 2,		// x���W
		Size.bottom / 2 - WINDOW_HEIGHT / 2,		// y���W

		// �N���C�A���g�̈���l���ĕ������w��
		WINDOW_WIDTH,//  + GetSystemMetrics( SM_CXFRAME ) * 2,
		WINDOW_HEIGHT,// + GetSystemMetrics( SM_CYFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION ),
		NULL,								// �e�n���h��
		NULL,								// ���j���[�n���h��
		hInst,								// �C���X�^���X�n���h��
		NULL								// ����f�[�^
		);

	return hWnd;
}