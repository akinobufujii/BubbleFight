
#include "Using.h"
#include "DirectGraphics.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

static LPDIRECT3D9				s_lpD3D;		// �I�u�W�F�N�g
static LPDIRECT3DDEVICE9		s_lpD3DDevice;	// �f�o�C�X
static D3DPRESENT_PARAMETERS	s_D3DPP;		// �p�����[�^
static LPDIRECT3DSURFACE9		s_lpBackBuffer;	// �o�b�N�o�b�t�@�̃T�[�t�F�X
static LPDIRECT3DSURFACE9		s_lpZBuffer;	// Z�o�b�t�@�̃T�[�t�F�X

// ������
bool D3D::Init( HWND hWnd, BOOL iswindow )
{
	// DirectX������
	s_lpD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( s_lpD3D == NULL )
	{
		return false;
	}

	// �p�����[�^�ݒ�
	s_D3DPP.BackBufferWidth				= WINDOW_WIDTH;							// �o�b�N�o�b�t�@��
	s_D3DPP.BackBufferHeight			= WINDOW_HEIGHT;						// �o�b�N�o�b�t�@����
	s_D3DPP.BackBufferFormat			= D3DFMT_A8R8G8B8;						// �o�b�N�o�b�t�@�t�H�[�}�b�g
	s_D3DPP.BackBufferCount				= 1;									// �o�b�N�o�b�t�@��
	s_D3DPP.MultiSampleType				= D3DMULTISAMPLE_NONE;					// �}���`�T���v�����O�^�C�v
	s_D3DPP.MultiSampleQuality			= 0;									// �i�����x��
	s_D3DPP.SwapEffect					= D3DSWAPEFFECT_FLIP;					// �X���b�v�G�t�F�N�g
	s_D3DPP.hDeviceWindow				= hWnd;									// �E�B���h�E�n���h��
	s_D3DPP.Windowed					= iswindow;								// �E�B���h�E���[�h
	s_D3DPP.EnableAutoDepthStencil		= TRUE;									// �T�[�t�F�X�t�H�[�}�b�g
	s_D3DPP.AutoDepthStencilFormat		= D3DFMT_D24S8;							// �X�e���V���o�b�t�@�t�H�[�}�b�g
	s_D3DPP.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;	// ��ʃt���b�v���̌��ʃI�v�V����
	s_D3DPP.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;				// �t���X�N���[�����̃��t���b�V�����[�g
	s_D3DPP.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;			// �t���b�v�̃^�C�~���O�ݒ�

	// �f�o�C�X�쐬
	// �n�[�h�E�F�A HAL
	HRESULT hr = s_lpD3D->CreateDevice(
		D3DADAPTER_DEFAULT,					// �A�_�v�^
		D3DDEVTYPE_HAL,						// �f�o�C�X�^�C�v
		hWnd,								// �E�B���h�E�n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,// �f�o�C�X�����I�v�V����
		&s_D3DPP,							// �ݒ肵���p�����[�^
		&s_lpD3DDevice );					// �i�[����f�o�C�X

	// ���s����� �\�t�g�E�F�A�@HAL
	if( FAILED( hr ) )
	{
		hr = s_lpD3D->CreateDevice(
			D3DADAPTER_DEFAULT,					// �A�_�v�^
			D3DDEVTYPE_HAL,						// �f�o�C�X�^�C�v
			hWnd,								// �E�B���h�E�n���h��
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,// �f�o�C�X�����I�v�V����
			&s_D3DPP,							// �ݒ肵���p�����[�^
			&s_lpD3DDevice );					// �i�[����f�o�C�X

		// ����ł����s����΁@�\�t�g�E�F�A�@REF
		if( FAILED( hr ) )
		{
			hr = s_lpD3D->CreateDevice(
				D3DADAPTER_DEFAULT,					// �A�_�v�^
				D3DDEVTYPE_REF,						// �f�o�C�X�^�C�v
				hWnd,								// �E�B���h�E�n���h��
				D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,// �f�o�C�X�����I�v�V����
				&s_D3DPP,							// �ݒ肵���p�����[�^
				&s_lpD3DDevice );					// �i�[����f�o�C�X

			// �����Ŏ��s����΂�������
			if( ErrorMessage( hr, "�f�o�C�X�쐬���s") )
			{
				return false;
			}
		}
	}

	// �����_�[�^�[�Q�b�g�i�[
	s_lpD3DDevice->GetRenderTarget( 0, &s_lpBackBuffer );
	s_lpD3DDevice->GetDepthStencilSurface( &s_lpZBuffer );

	// �����_�[�X�e�[�g�ݒ�
	s_lpD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	s_lpD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_ALPHAREF, TRUE );
	s_lpD3DDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

	// �A���t�@�l�̊|���Z�w��
	s_lpD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_DIFFUSE );
	s_lpD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE );
	s_lpD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TEXTURE );

	return true;
}

// �`��J�n
bool D3D::Begin()
{
	if( FAILED( s_lpD3DDevice->BeginScene() ) )
	{
		return false;
	}

	return false;
}

// �`��I��
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

// ��ʃN���A
void D3D::Clear( D3DCOLOR color, DWORD target )
{
	s_lpD3DDevice->Clear( 0, NULL, target, color, 1.f, 0 );
}

// ���
void D3D::Release()
{
	SAFE_RELEASE( s_lpZBuffer );
	SAFE_RELEASE( s_lpBackBuffer );
	SAFE_RELEASE( s_lpD3DDevice );
	SAFE_RELEASE( s_lpD3D );
}

// �f�o�C�X�l��
const LPDIRECT3DDEVICE9 D3D::GetDevice()
{
	return s_lpD3DDevice;
}

// �o�b�N�o�b�t�@�̃T�[�t�F�X�l��
const LPDIRECT3DSURFACE9 D3D::GetBackBufferSurcface()
{
	return s_lpBackBuffer;
}

const LPDIRECT3DSURFACE9 D3D::GetZBufer()
{
	return s_lpZBuffer;
}