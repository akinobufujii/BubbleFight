//////////////////////////////
// �C���N���[�h
//////////////////////////////
#include "Title.h"

#include "GameOver.h"

//////////////////////////////
// ���O��ԏȗ�
//////////////////////////////
using namespace Sequence;

//////////////////////////////
// �ÓI�����o������
//////////////////////////////
const LPSTR	CGameOver::BACKGROUND_TEX_PASS	= "GameOver.png";
const BYTE	CGameOver::FADEIN_OUT_SPEED		= 5;
const BYTE	CGameOver::FADEOUT_MIN			= 80;

//////////////////////////////
// ����
//////////////////////////////

// �R���X�g���N�^
CGameOver::CGameOver():
m_IsRun( true ),
m_Sprite( D3D::GetDevice() ),
m_IsAlphaPlus( true ),
m_Alpha( 0 )
{
}

// �ǂݍ���
void CGameOver::FileLoad()
{
	Load();

	m_lpBackTex = CSTexManager::GetInst()->GetData( BACKGROUND_TEX_PASS );

	// BGM�𗬂�(���[�v�Ȃ�)
	CSoundManager::GetInst()->Play( "Sound/GameOverBGM.wav" );
}

// ���s�֐�
bool CGameOver::RunProc()
{
	// �X�V
	Update();

	// �`��
	Draw();

	return m_IsRun;
}

// �X�V
void CGameOver::Update()
{
	if( Input::Keyboard::IsState( DIK_Z, Input::PUSH ) )
	{
		m_IsRun = false;
	}

	if( m_IsAlphaPlus )
	{
		if( (m_Alpha += FADEIN_OUT_SPEED) > 0xff )
		{
			m_Alpha = 0xff;
			m_IsAlphaPlus = false;
		}

	}else{

		if( (m_Alpha -= FADEIN_OUT_SPEED) < FADEOUT_MIN )
		{
			m_Alpha = FADEOUT_MIN;
			m_IsAlphaPlus = true;
		}
	}
}

// �`��
void CGameOver::Draw()
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// �`��J�n
	D3D::Begin();

	// ��ʃN���A
	lpDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0x00, 0x00, 0xff ), 1.f, 0 );

	// �X�v���C�g�`��J�n
	LPD3DXSPRITE lpSprite =  m_Sprite.Begin();

	// �w�i
	D3DSURFACE_DESC Desc = m_lpBackTex->GetSurfaceDesc();

	RECT Rc = { 0, 0, 1024, 1024 };

	lpSprite->SetTransform( &CMatrix() );

	lpSprite->Draw( m_lpBackTex->GetTexture(), &Rc, NULL, NULL, 0xffffffff );

	// ����
	m_Sprite.SetAlpha( m_Alpha );

	m_Sprite.Draw(
		CSTexManager::GetInst()->GetData( "ClearFont.png" )->GetTexture(),
		WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT - 95 * 2, 0, 
		0, 95 * 2, 400, 95 * 3 );

	m_Sprite.SetAlpha( 0xff );

	// �X�v���C�g�`��I��
	m_Sprite.End();

	// �`��I��
	D3D::End();
}

// �ǂݍ���
void CGameOver::Load()
{
	CSTexManager::GetInst()->LoadFromTextList( D3D::GetDevice(), "LoadData/GameOver/Tex.txt" );

	CSoundManager::GetInst()->LoadFromTextList( "LoadData/GameOver/Sound.txt" );
}

// ���
void CGameOver::Release()
{
	CSTexManager::GetInst()->ReleaseFromTextList( "LoadData/GameOver/Tex.txt" );

	CSoundManager::GetInst()->AllRelease();
}

// ���̃V�[�P���X
IBase* CGameOver::GetNext()
{
	return NEW CTitle();
}

// �f�X�g���N�^
CGameOver::~CGameOver()
{
	//Release();

	CSoundManager::GetInst()->Stop( "Sound/GameOverBGM.wav" );
}