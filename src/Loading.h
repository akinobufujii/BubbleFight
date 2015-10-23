#ifndef __AKI_LOADING_H__
#define __AKI_LOADING_H__

////////////////////////////////////
// ローディング画面描画クラス
// (マルチスレッドに送る用クラス)
////////////////////////////////////
class CLoading
{
public:	// 外部公開

	//////////////////////////////
	// 定数
	//////////////////////////////
	CONSTANT LPSTR	TEXPASS_LOADFONT;	// ローディングフォント
	CONSTANT BYTE	FONT_ANIMEMAX;		// 最大アニメ数
	CONSTANT short	FONT_WIDTH;			// フォント幅
	CONSTANT short	FONT_HEIGHT;		// フォント高さ
	CONSTANT BYTE	ANIME_TIME;			// アニメーションする時間

	//////////////////////////////
	// メソッド
	//////////////////////////////

	// コンストラクタ
	CLoading();

	// マルチスレッドで実行関数
	void operator()();

	// 終了通告
	void End();

	// デストラクタ
	~CLoading();

private:	// 非公開

	////////////////////////////////////
	// フィールド
	////////////////////////////////////
	bool		m_IsRun;	// 処理を走るかどうか
	CSprite		m_Sprite;	// スプライトデバイス
	BYTE		m_AnimeCnt;	// アニメカウンタ
	CTexture*	m_lpFontTex;// フォントテクスチャ
	DWORD		m_SaveTime;	// 時間保存用
};

#endif	// __AKI_LOADING_H__