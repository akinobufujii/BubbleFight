////////////////////////////////////
// インクルード
////////////////////////////////////
#include "Loading.h"

////////////////////////////////////
// 静的メンバ実体化
////////////////////////////////////
const LPSTR CLoading::TEXPASS_LOADFONT	= "Loading.png";
const BYTE	CLoading::FONT_ANIMEMAX		= 5;
const short CLoading::FONT_WIDTH		= 420;
const short	CLoading::FONT_HEIGHT		= 300 / FONT_ANIMEMAX;
const BYTE	CLoading::ANIME_TIME		= 200;

////////////////////////////////////
// 実装
////////////////////////////////////

// コンストラクタ
CLoading::CLoading():
m_IsRun( true ),
m_AnimeCnt( 0 ),
m_Sprite( D3D::GetDevice() ),
m_lpFontTex( CSTexManager::GetInst()->GetData( TEXPASS_LOADFONT ) ),
m_SaveTime( timeGetTime() )
{
}

// メソッド実行関数
void CLoading::operator ()()
{
	// 終了通告があるまで無限ループ
	while( m_IsRun )
	{
		// 同期開始
		//CSSyncro::GetInst()->Lock();

		// 描画開始
		D3D::Begin();

		// 画面クリア
		D3D::GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.f, 0 );

		// スプライト描画開始
		m_Sprite.Begin();

		m_Sprite.Draw(
			m_lpFontTex->GetTexture(),
			WINDOW_WIDTH - FONT_WIDTH, WINDOW_HEIGHT - FONT_HEIGHT, 0, 
			0, FONT_HEIGHT * m_AnimeCnt,
			FONT_WIDTH, FONT_HEIGHT * (m_AnimeCnt + 1) );

		// スプライト描画終了
		m_Sprite.End();

		// 描画終了
		D3D::End();

		// 同期開始
		//CSSyncro::GetInst()->UnLock();

		// 待つ
		if( timeGetTime() < m_SaveTime )
		{

			Sleep( ANIME_TIME );

		}else{

			Sleep( ANIME_TIME - (timeGetTime() - m_SaveTime) );
		}
		
		m_SaveTime = timeGetTime();

		++m_AnimeCnt %= FONT_ANIMEMAX;
	}
}

// 終了通告
void CLoading::End()
{
	m_IsRun = false;
}

// デストラクタ
CLoading::~CLoading()
{
}
