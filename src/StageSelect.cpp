/////////////////////////////
// �C���N���[�h
/////////////////////////////
#include "LimitWalls.h"
#include "GameMain.h"

#include "StageSelect.h"

/////////////////////////////
// ���O��ԏȗ�
/////////////////////////////
using namespace Sequence;

/////////////////////////////
// �ÓI�����o���̉�
/////////////////////////////
const LPSTR	CStageSelect::PATH_LOADTEXFILE	= "LoadData/StageSelect/Tex.txt";
const LPSTR	CStageSelect::PATH_LOADXFILE	= "LoadData/StageSelect/Xfile.txt";
const short	CStageSelect::FONT_WIDTH		= 450;
const short	CStageSelect::FONT_HEIGHT		= 90;
const short CStageSelect::STAGETONT_Y		= WINDOW_HEIGHT / 2 - FONT_HEIGHT - 20;
const short CStageSelect::SELECTVEC_X		= FONT_WIDTH / 4;
const short CStageSelect::SELECTVEC_TOP_Y	= STAGETONT_Y - 20;
const short CStageSelect::SELECTVEC_BOTTOM_Y= STAGETONT_Y + FONT_HEIGHT;
const short CStageSelect::BUTTON_X			= 30;
const short CStageSelect::BUTTON_Y			= STAGETONT_Y + FONT_HEIGHT * 1.8f;
const float	CStageSelect::FONT_ALPHARATE	= 255.f / FONT_HEIGHT;
const float	CStageSelect::SLIDE_IN_SPEED	= 3.f;
const float	CStageSelect::IMAGE_MOVEMAX		= 0.4f;
const float	CStageSelect::IMAGE_MOVESPEED	= 0.01f;
const float	CStageSelect::EXPLAIN_WIDTH		= 14.f;
const float	CStageSelect::EXPLAIN_HEIGHT	= 2.5f;
const float	CStageSelect::EXPLAIN_DRAWHEIGHT= -4.f;
const BYTE	CStageSelect::NUMBER_SIZE		= 60;
const BYTE	CStageSelect::STAGE_MAX			= 3;
const BYTE	CStageSelect::FADEOUT_SPEED		= 5;

const BYTE	CStageSelect::FLG_RUN			= 0x01;
const BYTE	CStageSelect::FLG_IMAGE_UP		= 0x02;

const LPSTR	CStageSelect::PATHES_STAGEIMAGES[] =
{
	"Stage/Stage1.png",
	"Stage/Stage2.png",
	"Stage/Stage3.png"
};

const LPSTR CStageSelect::PATHES_EXPLAINIMAGES[] = 
{
	"Stage/Stage1Explain.png",
	"Stage/Stage2Explain.png",
	"Stage/Stage3Explain.png"
};

// �R���X�g���N�^
CStageSelect::CStageSelect():
m_Flg( FLG_RUN ),
m_BackBoard( 16.f, 12.f ),
m_StageImage( 7.f, 4.f ),
m_FontY( -FONT_HEIGHT ),
m_fpUpdate( &CStageSelect::UpdateStartAnime ),
m_StageIndex( 1 ),
m_StageImageY( 0 ),
m_SceneAlpha( 0xff ),
m_PosZ( -10.f ),
m_LockY( 0 )
{
}

// �f�X�g���N�^
CStageSelect::~CStageSelect()
{
	// �A���
	BOOST_FOREACH( CPointSpriteManager* lpBubble, m_BubbleList )
	{
		SAFE_DELETE( lpBubble );
	}

	m_BubbleList.clear();
}

//�@�ǂݍ���
void CStageSelect::FileLoad()
{
	m_Sprite.Create( D3D::GetDevice() );

	// �w�i�ݒ�
	m_BackBoard.SetTexturePos(
		&D3DXVECTOR2( 0.f, 0.f ),
		&D3DXVECTOR2( 1.f / 1024 * 800, 0.f ),
		&D3DXVECTOR2( 1.f / 1024 * 800, 1.f / 1024 * 600 ),
		&D3DXVECTOR2( 0.f, 1.f / 1024 * 600 ) );

	m_BackBoard.SetColor( 1.f, 1.f, 1.f );

	m_StageImage.SetTexturePos(
		&D3DXVECTOR2( 0.f, 0.f ),
		&D3DXVECTOR2( 1.f, 0.f ),
		&D3DXVECTOR2( 1.f, 1.f ),
		&D3DXVECTOR2( 0.f, 1.f ) );

	// �����ݒ�
	m_ExplainBoard.SetVertex( EXPLAIN_WIDTH, EXPLAIN_HEIGHT );

	m_ExplainBoard.SetColor( 1.f, 1.f, 1.f );

	m_ExplainBoard.SetTexturePos(
		&D3DXVECTOR2( 0.f, 0.f ),
		&D3DXVECTOR2( 1.f, 0.f ),
		&D3DXVECTOR2( 1.f, 1.f ),
		&D3DXVECTOR2( 0.f, 1.f ) );

	// �ǂݍ���
	CSTexManager::GetInst()->LoadFromTextList( D3D::GetDevice(), PATH_LOADTEXFILE );
	CSXfileManager::GetInst()->LoadFromTextList( D3D::GetDevice(), PATH_LOADXFILE );

	// �����_�[�^�[�Q�b�g�쐬
	m_SceneTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT );

	// BGM
	CSoundManager::GetInst()->Play( "Sound/StageSelect.wav", true );
}

// ���̃V�[�P���X
IBase* CStageSelect::GetNext()
{
	CSoundManager::GetInst()->Stop( "Sound/StageSelect.wav" );

	return NEW CGameMain( m_StageIndex );
}

// �����{��
bool CStageSelect::RunProc()
{
	// �X�V
	Update();

	// �`��
	Draw();

	return m_Flg & FLG_RUN;
}

// �X�V
void CStageSelect::Update()
{
	// �p�[�g�X�V
	if( m_fpUpdate )
	{
		(this->*m_fpUpdate)();
	}

	// �A�X�V
	BOOST_FOREACH( CPointSpriteManager* lpBubble, m_BubbleList )
	{
		lpBubble->Update();
	}
}

// �ŏ��̃A�j���[�V�����X�V
void CStageSelect::UpdateStartAnime()
{
	if( (m_FontY += SLIDE_IN_SPEED) > 0 )
	{
		m_FontY = 0;

		D3DVIEWPORT9 Vp;
		D3DXVECTOR3	Pos( WINDOW_WIDTH / 2, 0, 0 );
		D3D::GetDevice()->GetViewport( &Vp );
			
		D3DXVec3Unproject( &Pos, &Pos, 
			&Vp, m_Proj.GetMatrix(), m_Camera.GetMatrix(),
			&CMatrix( 0, -5, -10 ) );
		
		// �A�ݒ�
		CPointSpriteManager* lpTemp = NEW CPointSpriteManager();

		lpTemp->SetNum( 100 );
		lpTemp->SetMove( D3DXVECTOR3( 1, -1, 1 ) );
		lpTemp->SetEraseTime( 1200 );
		lpTemp->SetPos( Pos );
		lpTemp->SetScaleMax( 5.f );
		lpTemp->SetTexture( CSTexManager::GetInst()->GetData( "Spray.png" )->GetTexture() );

		m_BubbleList.push_back( lpTemp );

		// ���̃p�[�g��
		m_fpUpdate = &CStageSelect::UpdateSelect;
	}
}

// �I�𒆍X�V
void CStageSelect::UpdateSelect()
{
	// �}�E�X���W������Ă���
	POINT CursorPos = GetMousePos();

	bool IsUpDown = false;
	static int Cnt = 0;
	Cnt++;

	if( GetAsyncKeyState( VK_LBUTTON ) )
	{
		// ����
		if( IsHitbyCross( CursorPos.x, CursorPos.y, 0, 0, 
			BUTTON_X, BUTTON_Y, FONT_WIDTH / 2, FONT_HEIGHT ) )
		{
			// �V�[���̐؂�ւ�
			m_fpUpdate = &CStageSelect::UpdateFadeOut;

			CSoundManager::GetInst()->Play( "Sound/Choos.wav" );
		}

		// �I��
		// �C���f�b�N�X��������
		if( IsHitbyCross( CursorPos.x, CursorPos.y, 0, 0, 
			SELECTVEC_X, SELECTVEC_TOP_Y, NUMBER_SIZE, NUMBER_SIZE / 2 ) && Cnt > 10 )
		{
			Cnt = 0;

			// ����
			if( --m_StageIndex <= 0 )
			{
				m_StageIndex = STAGE_MAX;
			}

			CSoundManager::GetInst()->Play( "Sound/Select.wav" );

			IsUpDown = true;
		}

		// �C���f�b�N�X���グ��
		if( IsHitbyCross( CursorPos.x, CursorPos.y, 0, 0, 
			SELECTVEC_X, SELECTVEC_BOTTOM_Y, NUMBER_SIZE, NUMBER_SIZE / 2 ) && Cnt > 10  )
		{
			Cnt = 0;

			// ����
			if( ++m_StageIndex > STAGE_MAX )
			{
				m_StageIndex = 1;
			}

			CSoundManager::GetInst()->Play( "Sound/Select.wav" );

			IsUpDown = true;
		}
	}

	// ��
	//if( Input::Keyboard::IsState( DIK_UP, Input::PUSH  ) )
	//{
	//	// ����
	//	if( --m_StageIndex <= 0 )
	//	{
	//		m_StageIndex = STAGE_MAX;
	//	}

	//	CSoundManager::GetInst()->Play( "Sound/Select.wav" );

	//	IsUpDown = true;
	//}

	//// ��
	//if( Input::Keyboard::IsState( DIK_DOWN, Input::PUSH  ) )
	//{
	//	// ����
	//	if( ++m_StageIndex > STAGE_MAX )
	//	{
	//		m_StageIndex = 1;
	//	}

	//	CSoundManager::GetInst()->Play( "Sound/Select.wav" );

	//	IsUpDown = true;
	//}

	// �Z���N�g���㉺�����ꂩ�����Ȃ�
	if( IsUpDown )
	{
		// �o���ʒu�����肷��
		D3DVIEWPORT9 Vp;
		D3DXVECTOR3	Pos( FONT_WIDTH / 4, (WINDOW_HEIGHT / 2 - FONT_HEIGHT - 20), 0 );
		D3D::GetDevice()->GetViewport( &Vp );

		D3DXVec3Unproject( &Pos, &Pos, 
			&Vp, m_Proj.GetMatrix(), m_Camera.GetMatrix(),
			&CMatrix( 5, -1.5f, -10 ) );

		// �A��ǉ�����
		// ���Z�b�g�ł�����̂�����Ȃ烊�Z�b�g����
		bool IsReset = false;

		BOOST_FOREACH( CPointSpriteManager* lpBubble, m_BubbleList )
		{
			if( lpBubble->IsErase() )
			{
				lpBubble->SetNum( 50 );
				lpBubble->SetPos( Pos );
				lpBubble->Reset();

				IsReset = true;

				break;
			}
		}

		// ���Z�b�g�ł��Ȃ��Ȃ���
		if( IsReset == false )
		{
			CPointSpriteManager* lpTemp = NEW CPointSpriteManager();

			lpTemp->SetNum( 50 );
			lpTemp->SetMove( D3DXVECTOR3( 1, -1, 1 ) );
			lpTemp->SetEraseTime( 1200 );
			lpTemp->SetPos( Pos );
			lpTemp->SetScaleMax( 5.f );
			lpTemp->SetTexture( CSTexManager::GetInst()->GetData( "Spray.png" )->GetTexture() );

			m_BubbleList.push_back( lpTemp );
		}
	}

	// �X�e�[�W�C���[�W�𓮂���
	m_StageImageY += (m_Flg & FLG_IMAGE_UP)? -IMAGE_MOVESPEED : IMAGE_MOVESPEED;

	if( abs( m_StageImageY ) > IMAGE_MOVEMAX )
	{
		m_Flg ^= FLG_IMAGE_UP;
	}
}

// �t�F�[�h�A�E�g�̍X�V
void CStageSelect::UpdateFadeOut()
{
	if( (m_SceneAlpha -= FADEOUT_SPEED) < 0 )
	{
		m_SceneAlpha = 0;
		m_Flg &= ~FLG_RUN;
	}

	m_PosZ += 0.1f;
	m_LockY+= 0.1f;
}

// �`��
void CStageSelect::Draw()
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// �J�����ݒ�
	m_Camera.SetCameraPos( &D3DXVECTOR3( 0, 0, m_PosZ ) );
	m_Camera.SetCameraLook( &D3DXVECTOR3( 0, m_LockY, 0 ) );

	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );
	lpDevice->SetTransform( D3DTS_VIEW, m_Camera.GetMatrix() );

	// �J�����O�ݒ�
	SetCullingCamera( &m_Camera );
	SetCullingProj( &m_Proj );

	// �`��J�n
	D3D::Begin();

	// �S�Đ��`�␳
	lpDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	lpDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	// �����_�[�^�[�Q�b�g�ύX
	lpDevice->SetRenderTarget( 0, m_SceneTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_SceneTex.GetZbuffer() );

	// ��ʃN���A
	D3D::Clear( D3DCOLOR_XRGB( 0, 0, 0 ) );

	// �w�i
	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	// �������������ɂ��炷
	CMatrix mRotX;
	mRotX.SetRotX( 30 );
	lpDevice->SetTransform( D3DTS_WORLD, &( mRotX * CMatrix( 0, -1, 0 ) ) );

	CSXfileManager::GetInst()->GetData( "stage.x" )->Draw( lpDevice );

	// �X�e�[�W�C���[�W
	lpDevice->SetTexture( 0, 
		CSTexManager::GetInst()->GetData( PATHES_STAGEIMAGES[ m_StageIndex - 1 ] )->GetTexture() );

	lpDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix( 3, m_StageImageY, 0 ) );
	
	m_StageImage.SetAlpha( 1.f - (1.f / 0xff) * abs( FONT_ALPHARATE * m_FontY ) );
	m_StageImage.Draw( lpDevice );

	// ������
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix( 0, EXPLAIN_DRAWHEIGHT, 0 ) );
	lpDevice->SetTexture( 0, 
		CSTexManager::GetInst()->GetData( PATHES_EXPLAINIMAGES[ m_StageIndex - 1 ] )->GetTexture() );

	m_ExplainBoard.Draw( lpDevice );

	// �A
	lpDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	BOOST_FOREACH( CPointSpriteManager* lpBubble, m_BubbleList )
	{
		lpBubble->Draw( lpDevice );
	}

	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// �����_�[�^�[�Q�b�g�ύX
	lpDevice->SetRenderTarget( 0, D3D::GetBackBufferSurcface() );
	lpDevice->SetDepthStencilSurface( D3D::GetZBufer() );

	D3D::Clear( D3DCOLOR_XRGB( 0, 0, 0 ) );

	// �X�v���C�g�`��J�n
	m_Sprite.Begin();

	// �A���t�@�l�ݒ�
	if( m_fpUpdate == &CStageSelect::UpdateFadeOut )
	{
		m_Sprite.SetAlpha( m_SceneAlpha );

	}else{
		
		m_Sprite.SetAlpha( static_cast<int>( 0xff - abs( FONT_ALPHARATE * m_FontY ) ) );
	}

	// �V�[��
	m_Sprite.Draw( m_SceneTex,
		0, 0, 0,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );

	// Stage Select
	m_Sprite.Draw(
		CSTexManager::GetInst()->GetData( "StageSelectFont.png" )->GetTexture(),
		WINDOW_WIDTH / 2 - FONT_WIDTH / 2, m_FontY, 0,
		FONT_HEIGHT * FONT_STAGESELECT, 0,
		FONT_WIDTH, FONT_HEIGHT * (FONT_STAGESELECT + 1) );

	// Stage
	m_Sprite.Draw(
		CSTexManager::GetInst()->GetData( "StageSelectFont.png" )->GetTexture(),
		0, STAGETONT_Y, 0,
		FONT_HEIGHT * FONT_STAGESELECT, 0, FONT_WIDTH / 2,
		FONT_HEIGHT * (FONT_STAGESELECT + 1) );

	// �X�e�[�W�C���f�b�N�X�̖��
	int MoveY = abs( m_StageImageY * 30 );

	// ��
	m_Sprite.Draw(
			CSTexManager::GetInst()->GetData( "Number.png" )->GetTexture(),
			SELECTVEC_X, SELECTVEC_TOP_Y - MoveY, 0,
			720 - NUMBER_SIZE, 0, 720, NUMBER_SIZE / 2 );

	// ��
	m_Sprite.Draw(
			CSTexManager::GetInst()->GetData( "Number.png" )->GetTexture(),
			SELECTVEC_X, SELECTVEC_BOTTOM_Y + MoveY, 0,
			720 - NUMBER_SIZE, NUMBER_SIZE / 2, 720, NUMBER_SIZE );

	// �X�e�[�W�C���f�b�N�X
	char Str[ sizeof( int ) * 8 ];

	sprintf_s( Str, sizeof( Str ), "%d", m_StageIndex );

	for( int i = 0; Str[ i ] != '\0'; i++ )
	{
		BYTE Num = Str[ i ] - '0';

		m_Sprite.Draw(
			CSTexManager::GetInst()->GetData( "Number.png" )->GetTexture(),
			FONT_WIDTH / 2 + NUMBER_SIZE * i, STAGETONT_Y + 15, 0,
			NUMBER_SIZE * Num, 0, NUMBER_SIZE * (Num + 1), NUMBER_SIZE );
	}

	// ����
	// �I������Ă��
	POINT CursorPos = GetMousePos();

	// �J�[�\�����������Ă����
	if( IsHitbyCross( CursorPos.x, CursorPos.y, 0, 0, 
		30, BUTTON_Y, FONT_WIDTH / 2, FONT_HEIGHT ) )
	{
		m_Sprite.Draw(
			*CSTexManager::GetInst()->GetData( "StageSelectFont.png" ),
			BUTTON_X, BUTTON_Y, 0,
			FONT_WIDTH / 2, FONT_HEIGHT,
			FONT_WIDTH, FONT_HEIGHT * 2 );

	}else{
	
		m_Sprite.Draw(
			*CSTexManager::GetInst()->GetData( "StageSelectFont.png" ),
			BUTTON_X, BUTTON_Y, 0,
			0, FONT_HEIGHT,
			FONT_WIDTH / 2, FONT_HEIGHT * 2 );
	}

	// �J�[�\��
	m_Sprite.Draw(
			CSTexManager::GetInst()->GetData( "Corsor.png" )->GetTexture(),
			CursorPos.x - CURSOR_SIZE / 2, CursorPos.y - CURSOR_SIZE / 2,
			CURSOR_ROT,
			0, 0,
			CURSOR_SIZE, CURSOR_SIZE );

	// �X�v���C�g�`��I��
	m_Sprite.End();
	
	// �`��I��
	D3D::End();
}