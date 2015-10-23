#ifndef __AKI_DEBUGFONT_H__
#define __AKI_DEBUGFONT_H__

////////////////////////////////
// デバッグ用フォント
////////////////////////////////
class CDebugFont
{
public:	// 外部公開

	////////////////////////////////
	// メソッド
	////////////////////////////////

	// コンストラクタ
	CDebugFont( const LPDIRECT3DDEVICE9 lpdevice );

	// 文字描画
	void Draw( int x, int y, LPSTR str );

	// デストラクタ
	~CDebugFont();

private:	// 非公開

	////////////////////////////////
	// フィールド
	////////////////////////////////
	LPD3DXFONT	m_lpFont;	// フォントデバイス
};

#endif // __AKI_DEBUGFONT_H__