////////////////////////////////
// インクルード
////////////////////////////////
#include "DebugFont.h"

////////////////////////////////
// 実装
////////////////////////////////
CDebugFont::CDebugFont( const LPDIRECT3DDEVICE9 lpdevice )
{
	D3DXCreateFont( 
		lpdevice,
		14,
		6,
		FW_BOLD,
		NULL,                   //ミップマップモデルの数
        FALSE,                  //斜体にするかどうか
        SHIFTJIS_CHARSET,       //文字セット
        OUT_DEFAULT_PRECIS,
        PROOF_QUALITY,
        FIXED_PITCH | FF_MODERN,
        "tahoma",               //フォントの種類
        &m_lpFont);
}

// 文字列描画
void CDebugFont::Draw( int x, int y, LPSTR str )
{
	RECT rect = {x, y, 0, 0}; //表示領域

    //文字列サイズを計算
    m_lpFont->DrawText(
        NULL,
        str,
        -1,             //表示サイズ(-1で全部)
        &rect,          //表示範囲
        DT_CALCRECT,    //表示範囲に調整
        NULL);

    //そのサイズでレンダリング
    m_lpFont->DrawText(
        NULL,
        str,
        -1,							//表示サイズ(-1で全部)
        &rect,						//表示範囲
        DT_LEFT | DT_BOTTOM,		//左詰めで両端揃え
		D3DCOLOR_XRGB( 0, 0, 0 ) ); //色
}

// デストラクタ
CDebugFont::~CDebugFont()
{
	SAFE_RELEASE( m_lpFont );
}