//////////////////////////////
// インクルード
//////////////////////////////
#include "Title.h"

#include "GameOver.h"

//////////////////////////////
// 名前空間省略
//////////////////////////////
using namespace Sequence;

//////////////////////////////
// 静的メンバ初期化
//////////////////////////////
const LPSTR	CGameOver::BACKGROUND_TEX_PASS	= "GameOver.png";
const BYTE	CGameOver::FADEIN_OUT_SPEED		= 5;
const BYTE	CGameOver::FADEOUT_MIN			= 80;

//////////////////////////////
// 実装
//////////////////////////////

// コンストラクタ
CGameOver::CGameOver():
m_IsRun( true ),
m_Sprite( D3D::GetDevice() ),
m_IsAlphaPlus( true ),
m_Alpha( 0 )
{
}

// 読み込み
void CGameOver::FileLoad()
{
	Load();

	m_lpBackTex = CSTexManager::GetInst()->GetData( BACKGROUND_TEX_PASS );

	// BGMを流す(ループなし)
	CSoundManager::GetInst()->Play( "Sound/GameOverBGM.wav" );
}

// 実行関数
bool CGameOver::RunProc()
{
	// 更新
	Update();

	// 描画
	Draw();

	return m_IsRun;
}

// 更新
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

// 描画
void CGameOver::Draw()
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// 描画開始
	D3D::Begin();

	// 画面クリア
	lpDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0x00, 0x00, 0xff ), 1.f, 0 );

	// スプライト描画開始
	LPD3DXSPRITE lpSprite =  m_Sprite.Begin();

	// 背景
	D3DSURFACE_DESC Desc = m_lpBackTex->GetSurfaceDesc();

	RECT Rc = { 0, 0, 1024, 1024 };

	lpSprite->SetTransform( &CMatrix() );

	lpSprite->Draw( m_lpBackTex->GetTexture(), &Rc, NULL, NULL, 0xffffffff );

	// 次へ
	m_Sprite.SetAlpha( m_Alpha );

	m_Sprite.Draw(
		CSTexManager::GetInst()->GetData( "ClearFont.png" )->GetTexture(),
		WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT - 95 * 2, 0, 
		0, 95 * 2, 400, 95 * 3 );

	m_Sprite.SetAlpha( 0xff );

	// スプライト描画終了
	m_Sprite.End();

	// 描画終了
	D3D::End();
}

// 読み込み
void CGameOver::Load()
{
	CSTexManager::GetInst()->LoadFromTextList( D3D::GetDevice(), "LoadData/GameOver/Tex.txt" );

	CSoundManager::GetInst()->LoadFromTextList( "LoadData/GameOver/Sound.txt" );
}

// 解放
void CGameOver::Release()
{
	CSTexManager::GetInst()->ReleaseFromTextList( "LoadData/GameOver/Tex.txt" );

	CSoundManager::GetInst()->AllRelease();
}

// 次のシーケンス
IBase* CGameOver::GetNext()
{
	return NEW CTitle();
}

// デストラクタ
CGameOver::~CGameOver()
{
	//Release();

	CSoundManager::GetInst()->Stop( "Sound/GameOverBGM.wav" );
}