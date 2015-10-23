//////////////////////////////////
// �C���N���[�h
//////////////////////////////////
#include "StageSelect.h"

#include "Title.h"

//////////////////////////////////
// ���O��ԏȗ�
//////////////////////////////////
using namespace Sequence;
using namespace Input;

//////////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////////
const LPSTR		CTitle::TEXTURE_LOAD_FILE_PASS	= "LoadData/Title/Tex.txt";
const LPSTR		CTitle::XFILE_LOAD_FILE_PASS	= "LoadData/Title/Xdata.txt";
const LPSTR		CTitle::BUBBLE_TEX				= "Bubble.png";
const LPSTR		CTitle::MENU_TEX				= "Menu.png";
const LPSTR		CTitle::TITLELOGO_TEX			= "TitleLogo.png";
const short		CTitle::MENU_WIDTH				= 294;
const short		CTitle::MENU_HEIGHT				= 50;
const short		CTitle::MENU_X					= WINDOW_WIDTH / 2 - MENU_WIDTH / 2;
const short		CTitle::MENU_Y					= WINDOW_HEIGHT / 2 + 50;
const short		CTitle::TITLELOGO_WIDTH			= 600;
const short		CTitle::TITLELOGO_HEIGHT		= 100;
const short		CTitle::THANKSLOGO_WIDTH		= 400;
const short		CTitle::THANKSLOGO_HEIGHT		= 50;
const float		CTitle::BUBBLE_X_POS			= 5.5f;
const float		CTitle::BUBBLE_Y_POS			= -5.8f;
const float		CTitle::BUBBLE_SPEED_LIMIT		= -1.0001f;
const BYTE		CTitle::BUBBLE_NUM				= 60;
const USHORT	CTitle::BUBBLE_ERASE_TIME		= 1850;

// ������
CTitle::CTitle():
m_IsRun( true ),
m_SelectMenu( MENU_NUM ),
m_IsSelect( false ),
m_Alpha( 0xff ),
m_TeachAlpha( 0 ),
m_IsTeachAlphaPlus( true )
{
}

// �f�X�g���N�^
CTitle::~CTitle()
{
}

// �ǂݍ���
void CTitle::FileLoad()
{
	// �e��ǂݍ���
	Load();

	// �X�v���C�g�쐬
	m_Sprite.Create( D3D::GetDevice() );

	// ���C�g�ݒ�
	D3DLIGHT9	Light;
	ZeroMemory( &Light, sizeof( Light ) );

	Light.Type	= D3DLIGHT_DIRECTIONAL;	// ���s����
	Light.Diffuse.r	= 1.0f;
	Light.Diffuse.g	= 1.0f;
	Light.Diffuse.b	= 1.0f;

	Light.Position.x= 0.0f;
	Light.Position.y= 1.0f;
	Light.Position.z= 0.0f;

	Light.Range	= 10.f;

	Light.Direction = D3DXVECTOR3( 1.0f, -1.0f, 1.0f );

	D3D::GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	D3D::GetDevice()->SetLight( 0, &Light );
	D3D::GetDevice()->LightEnable( 0, TRUE );

	// �V�[���p�����_�[�^�[�Q�b�g�쐬
	m_SceneTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT );

	// �A�̏�����
	// ����
	m_BubblesLeft.SetType( CPointSpriteManager::TYPE_SPOUT );
	m_BubblesLeft.SetNum( BUBBLE_NUM );
	m_BubblesLeft.SetEraseTime( BUBBLE_ERASE_TIME );
	m_BubblesLeft.SetMove( D3DXVECTOR3( BUBBLE_SPEED_LIMIT, BUBBLE_SPEED_LIMIT, BUBBLE_SPEED_LIMIT ) );
	m_BubblesLeft.SetPos( D3DXVECTOR3( -BUBBLE_X_POS, BUBBLE_Y_POS, 0 ) );
	m_BubblesLeft.SetScaleMax( 3.5f );
	m_BubblesLeft.SetTexture( CSTexManager::GetInst()->GetData( BUBBLE_TEX )->GetTexture() );

	// �E��
	m_BubblesRight.SetType( CPointSpriteManager::TYPE_SPOUT );
	m_BubblesRight.SetNum( BUBBLE_NUM );
	m_BubblesRight.SetEraseTime( BUBBLE_ERASE_TIME );
	m_BubblesRight.SetMove( D3DXVECTOR3( BUBBLE_SPEED_LIMIT, BUBBLE_SPEED_LIMIT, BUBBLE_SPEED_LIMIT ) );
	m_BubblesRight.SetPos( D3DXVECTOR3( BUBBLE_X_POS, BUBBLE_Y_POS, 0 ) );
	m_BubblesRight.SetScaleMax( 3.5f );
	m_BubblesRight.SetTexture( CSTexManager::GetInst()->GetData( BUBBLE_TEX )->GetTexture() );

	CXfile::SetDrawShader( false );
	CBoard::SetDrawShader( false );

	// BGM
	CSoundManager::GetInst()->Play( "Sound/TitleBGM.wav", true );
}

// �{��
bool CTitle::RunProc()
{
	// �X�V
	Update();

	// �`��
	Draw();

	return m_IsRun;
}

// �X�V
void CTitle::Update()
{
	if( m_IsSelect == false )
	{
		// �I������
		// �}�E�X�ʒu������Ă���
		POINT MousePos = GetMousePos();
		bool IsHit = false;

		for( int i = 0; i < MENU_NUM; i++ )
		{
			// �I������Ă���Ȃ�
			if( IsHitbyCross( MousePos.x, MousePos.y, 0, 0,
				MENU_X, MENU_Y + MENU_HEIGHT * (i + 1), MENU_WIDTH, MENU_HEIGHT ) )
			{
				IsHit = true;

				// �����I�΂ꂽ���̂łȂ����
				if( m_SelectMenu != i )
				{
					// ����炷
					CSoundManager::GetInst()->Play( "Sound/Select.wav" );
				}

				// �I�����ꂽ���̂��i�[
				m_SelectMenu = i;

				break;
			}
		}

		if( IsHit == false )
		{
			m_SelectMenu = MENU_NUM;
		}

		// ����
		if( GetAsyncKeyState( VK_LBUTTON ) && m_SelectMenu != MENU_NUM )
		{
			CSoundManager::GetInst()->Play( "Sound/Choos.wav" );

			// �I�΂ꂽ���j���[�̍��W���i�[����
			D3DXVECTOR3 SelectPos( MENU_X, MENU_Y + MENU_HEIGHT * (m_SelectMenu + 1), 0);
			D3DVIEWPORT9	Vp;

			D3D::GetDevice()->GetViewport( &Vp );

			D3DXVec3Unproject( &SelectPos, &SelectPos, &Vp, m_Proj.GetMatrix(), m_Camera.GetMatrix(), &CMatrix( 0, -3, -9 ) );

			SelectPos.y -= m_SelectMenu;

			// �|�C���g�X�v���C�g�쐬
			m_SelectMenuBubble.SetNum( 100 );
			m_SelectMenuBubble.SetMove( D3DXVECTOR3( 0.5f, -0.5f, 0.1f ) );
			m_SelectMenuBubble.SetEraseTime( 1500 );
			m_SelectMenuBubble.SetTexture( CSTexManager::GetInst()->GetData( "Spray.png" )->GetTexture() );
			m_SelectMenuBubble.SetPos( SelectPos );

			m_IsSelect = true;
		}

	}else{

		// �t�F�[�h�A�E�g
		if( (m_Alpha -= 5) <= 0 )
		{
			m_IsRun = false;
		}
	}

	// �w���̃A���t�@�l�X�V
	if( m_IsTeachAlphaPlus )
	{
		if( (m_TeachAlpha += 5) > 0xff )
		{
			m_TeachAlpha = 0xff;
			m_IsTeachAlphaPlus = false;
		}
	
	}else{

		if( (m_TeachAlpha -= 5) < 0x80 )
		{
			m_TeachAlpha = 0x80;
			m_IsTeachAlphaPlus = true;
		}
	}

	// �A�X�V
	m_BubblesLeft.Update();
	m_BubblesRight.Update();
	m_SelectMenuBubble.Update();
}

// �`��
void CTitle::Draw()
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// �`��J�n
	D3D::Begin();
	
	// �����_�[�^�[�Q�b�g�ύX
	lpDevice->SetRenderTarget( 0, m_SceneTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_SceneTex.GetZbuffer() );

	// ��ʃN���A
	D3D::Clear( D3DCOLOR_XRGB( 0x00, 0x00, 0x00 ) );

	// ���e
	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );

	// �J����
	m_Camera.SetCamera(
		&D3DXVECTOR3( 0, -5, -10 ),
		&D3DXVECTOR3( 0, 0, 0 ),
		&D3DXVECTOR3( 0, 1, 0 ) );

	lpDevice->SetTransform( D3DTS_VIEW, m_Camera.GetMatrix() );

	// �X�e�[�W
	lpDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	lpDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );

	CSXfileManager::GetInst()->GetData( "stage.x" )->Draw( lpDevice );

	// �A
	lpDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	m_BubblesLeft.Draw( lpDevice );
	m_BubblesRight.Draw( lpDevice );
	m_SelectMenuBubble.Draw( lpDevice );

	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// �����_�[�^�[�Q�b�g��߂�
	lpDevice->SetRenderTarget( 0, D3D::GetBackBufferSurcface() );
	lpDevice->SetDepthStencilSurface( D3D::GetZBufer() );
	D3D::Clear( D3DCOLOR_XRGB( 0x00, 0x00, 0x00 ) );

	// �X�v���C�g�`��J�n
	m_Sprite.Begin();

	// �A���t�@�l�ݒ�
	m_Sprite.SetAlpha( m_Alpha );

	// �w�i
	m_Sprite.Draw( 
		m_SceneTex,
		0, 0, 0, 0, 0,
		WINDOW_WIDTH, WINDOW_HEIGHT );

	// �^�C�g�����S
	m_Sprite.Draw( 
		CSTexManager::GetInst()->GetData( TITLELOGO_TEX )->GetTexture(),
		WINDOW_WIDTH / 2 - TITLELOGO_WIDTH / 2,
		TITLELOGO_HEIGHT, 0, 0, 0,
		TITLELOGO_WIDTH, TITLELOGO_HEIGHT );

	// �}�E�X�ʒu������Ă���
	POINT MousePos = GetMousePos();

	// ���j���[
	for( int i = 0; i < MENU_NUM; i++ )
	{
		// �I������Ă��郁�j���[�Ȃ�F��ς���
		if( m_SelectMenu == i )
		{
			m_Sprite.Draw( 
				CSTexManager::GetInst()->GetData( MENU_TEX )->GetTexture(),
				MENU_X, MENU_Y + MENU_HEIGHT * (i + 1), 0,
				MENU_WIDTH, MENU_HEIGHT * i,
				MENU_WIDTH * 2, MENU_HEIGHT * (i + 1) );

		}else{

			m_Sprite.Draw( 
				CSTexManager::GetInst()->GetData( MENU_TEX )->GetTexture(),
				MENU_X, MENU_Y + MENU_HEIGHT * (i + 1), 0,
				0, MENU_HEIGHT * i,
				MENU_WIDTH, MENU_HEIGHT * (i + 1) );
		}
	}

	// ��
	m_Sprite.Draw( 
		CSTexManager::GetInst()->GetData( "Thanks.png" )->GetTexture(),
		0, WINDOW_HEIGHT - THANKSLOGO_HEIGHT, 0,
		0, 0,
		THANKSLOGO_WIDTH, THANKSLOGO_HEIGHT );

	// �I������Ă���ΑO�̃A���t�@�l���g�p����
	if( m_IsSelect == false )
	{
		//m_Sprite.SetAlpha( m_TeachAlpha );
	}
	
	// �w��
	/*m_Sprite.Draw( 
		CSTexManager::GetInst()->GetData( "ClearFont.png" )->GetTexture(),
		WINDOW_WIDTH / 2 - 200, 
		WINDOW_HEIGHT / 2 + MENU_HEIGHT * 3, 0,
		0, 190,
		400, 285 );*/

	// �J�[�\��
	POINT CursorPos = GetMousePos();

	m_Sprite.Draw( 
		CSTexManager::GetInst()->GetData( "Corsor.png" )->GetTexture(),
		CursorPos.x - CURSOR_SIZE / 2, 
		CursorPos.y - CURSOR_SIZE / 2, CURSOR_ROT,
		0, 0, CURSOR_SIZE, CURSOR_SIZE );

	// �X�v���C�g�`��I��
	m_Sprite.End();

	// �`��I��
	D3D::End();
}

// �ǂݍ���
void CTitle::Load()
{
	// �e�N�X�`���ǂݍ���
	CSTexManager::GetInst()->LoadFromTextList( D3D::GetDevice(), TEXTURE_LOAD_FILE_PASS );
	
	// X�t�@�C���ǂݍ���
	CSXfileManager::GetInst()->LoadFromTextList( D3D::GetDevice(), XFILE_LOAD_FILE_PASS );

	// �T�E���h�ǂݍ���
	CSoundManager::GetInst()->LoadFromTextList( "LoadData/Title/Sound.txt" );
}

// ���
void CTitle::Release()
{
	CSoundManager::GetInst()->AllRelease();

	// �e�N�X�`�����
	std::fstream File( TEXTURE_LOAD_FILE_PASS, std::ios::in );
	char Pass[ 256 ];

	if( File.is_open() )
	{
		// �ǂݍ���
		while( File.eof() == false )
		{
			File >> Pass;

			CSTexManager::GetInst()->Release( Pass );
		}
	}

	File.close();

	File.clear();

	// X�t�@�C���ǂݍ���
	File.open( XFILE_LOAD_FILE_PASS, std::ios::in );

	if( File.is_open() )
	{
		// �ǂݍ���
		while( File.eof() == false )
		{
			File >> Pass;
		
			CSXfileManager::GetInst()->Release( Pass );
		}
	}

	File.close();
}

// ���̃V�[�P���X
IBase* CTitle::GetNext()
{
	Release();

	// ���j���[����
	switch( m_SelectMenu )
	{
	case MENU_START:
		return NEW CStageSelect();
		break;

	case MENU_END:
		return NULL;
		break;
	}

	return NULL;
}