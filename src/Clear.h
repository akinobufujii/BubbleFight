#ifndef __AKI_CLEAR_H__
#define __AKI_CLEAR_H__

//////////////////////////////////
// クリアシーケンス
//////////////////////////////////
namespace Sequence
{
	class CClear : public IBase
	{
	public:	// 外部公開

		//////////////////////////////////////////////////
		// 定数
		//////////////////////////////////////////////////
		
		// フォント種類の列挙体
		enum FONTKIND	
		{
			FONTKIND_SCENE,
			FONTKIND_CLEAR,
			FONTKIND_ZKEY,
			FONTKIND_SCORE_AND_TIME,
			FONTKIND_TOTALSCORE
		};

		CONSTANT LPSTR	PASS_BACK_GROUND_MESH;	// 背景のメッシュ
		CONSTANT LPSTR	TEX_BUBBLE;				// 泡のテクスチャ
		CONSTANT LPSTR	TEXPASS_FONT;			// フォントのパス
		CONSTANT short	BUBBLE_ADD_TIME;		// 泡の追加時間
		CONSTANT BYTE	BUBBLE_NUM;				// 一度に作る泡数
		CONSTANT BYTE	BUBBLE_ADD_POS_LIMIT;	// 泡の創りだす範囲
		CONSTANT int	FONT_WIDTH;				// フォント幅
		CONSTANT int	FONT_HEIGHT;			// フォント高さ
		CONSTANT BYTE	FADEIN_OUT_SPEED;		// フェードの速度
		CONSTANT BYTE	FADEOUT_MIN;			// フェードアウトの最小値
		CONSTANT BYTE	NUMBER_SIZE;			// 数字の大きさ
		CONSTANT BYTE	GRAPHNUM_COLON;			// 時間の気切りであるコロンのナンバー
		CONSTANT int	DEST_SCOREFONT_X;		// スコアフォントの描画先X座標
		CONSTANT int	DEST_SCOREFONT_Y;		// スコアフォントの描画先Y座標
		CONSTANT int	DEST_NUMBER_X;			// 数字を描画するX座標
		CONSTANT int	DEST_SOCRE_Y;			// スコア描画Y座標
		CONSTANT int	DEST_CLEARTIME_Y;		// クリア時間描画Y座標
		CONSTANT int	DEST_TOTALSCORE_Y;		// トータルスコアフォントY座標
		CONSTANT int	DEST_TOTALSCORENUM_X;	// トータルスコアの数字X座標
		CONSTANT int	DEST_TOTALSCORENUM_Y;	// トータルスコアの数字Y座標
		
		//////////////////////////////////////////////////
		// メソッド
		//////////////////////////////////////////////////

		// コンストラクタ
		CClear( ULONG score, double clear_time );

		// デストラクタ
		~CClear();

		//////////////////////////////////////////////////
		//	・関数名	FileLoad
		//	・説明		読み込みを行う
		//	・引数		なし	
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void FileLoad();

		//////////////////////////////////////////////////
		//	・関数名	RunProc
		//	・説明		このシーケンスのメイン関数
		//	・引数		なし	
		//	・戻り値	まだ更新を行うならtrue
		//				破棄するならばfalseを返す
		//////////////////////////////////////////////////
		bool RunProc();

		//////////////////////////////////////////////////
		//	・関数名	GetNext
		//	・説明		次のシーケンスを獲得する
		//	・引数		なし	
		//	・戻り値	タイトルシーケンスのポインタを返す
		//////////////////////////////////////////////////
		IBase* GetNext();

	private:	// 非公開

		//////////////////////////////////////////////////
		// 型宣言
		//////////////////////////////////////////////////
		typedef void (CClear::*FPDRAWFUNC)();				// 描画関数ポインタ型
		typedef std::list<FPDRAWFUNC>			DRAWLIST;	// 描画関数ポインタリスト型
		typedef std::list<CPointSpriteManager*>	BUBBLELIST;	// 泡(ポイントスプライト)リスト型

		//////////////////////////////////////////////////
		// フィールド
		//////////////////////////////////////////////////
		bool				m_IsRun;			// 更新するかどうか
		CCamera				m_Camera;			// カメラ
		CProj				m_Proj;				// 投影
		DWORD				m_BubbleSaveTime;	// 泡の時間保存用
		CSprite				m_Sprite;			// スプライトデバイス
		int					m_Alpha;			// フォント用アルファ値
		bool				m_IsFadeIn;			// フェードイン・アウトフラグ(真でイン)
		CFishManager		m_Fishes;			// 魚たち
		ULONG				m_Score;			// スコア
		DWORD				m_FishMoveTime;		// 時間保存用
		int					m_ClearSecond;		// クリア時間(秒)
		Player::CDolphin*	m_lpDolphin;		// イルカ
		CMatrix				m_mDolphin;			// イルカの行列
		CVector				m_Target;			// イルカの目的地

		BUBBLELIST			m_PointBubbleList;	// 泡リスト
		DRAWLIST			m_DrawFpList;		// 描画関数ポインタリスト

		//////////////////////////////////////////////////
		// メソッド
		//////////////////////////////////////////////////
		
		//////////////////////////////////////////////////
		//	・関数名	Update
		//	・説明		更新を行う
		//	・引数		なし	
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Update();

		//////////////////////////////////////////////////
		//	・関数名	Draw
		//	・説明		描画を行う
		//	・引数		なし	
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Draw();

		//////////////////////////////////////////////////
		//	・関数名	DrawNumber
		//	・説明		数字の描画を行う
		//	・引数		
		//		- num	描画する数字
		//		- x		描画先頭のX座標
		//		- y		描画先頭のY座標
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void DrawNumber( int num, int x, int y );

		//////////////////////////////////////////////////
		//	・関数名	Load
		//	・説明		読み込みを行う
		//	・引数		なし	
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Load();

		//////////////////////////////////////////////////
		//	・関数名	Release
		//	・説明		読み込みの解放を行う
		//	・引数		なし	
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Release();
	};
}

#endif // __AKI_CLEAR_H__