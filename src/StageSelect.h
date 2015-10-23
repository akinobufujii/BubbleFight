#ifndef	__AKI_STAGESELECT_H__
#define __AKI_STAGESELECT_H__

namespace Sequence
{
	//////////////////////////////////////////////////
	// ステージシーケンス
	//////////////////////////////////////////////////
	class CStageSelect : public IBase
	{
	public:	// 外部公開

		//////////////////////////////////////////////////
		// 定数
		//////////////////////////////////////////////////
		CONSTANT LPSTR	PATH_LOADTEXFILE;		// ファイルパス
		CONSTANT LPSTR	PATH_LOADXFILE;			// XFile読み込みリストパス
		CONSTANT LPSTR	PATHES_STAGEIMAGES[];	// ステージごとのイメージ画像パス
		CONSTANT LPSTR	PATHES_EXPLAINIMAGES[];	// ステージごとの説明画像パス
		CONSTANT short	FONT_WIDTH;				// フォント幅
		CONSTANT short	FONT_HEIGHT;			// フォント高さ
		CONSTANT short	STAGETONT_Y;			// ステージフォントのY座標
		CONSTANT short	SELECTVEC_X;			// 矢印のX座標
		CONSTANT short	SELECTVEC_TOP_Y;		// 矢印の上側Y座標
		CONSTANT short	SELECTVEC_BOTTOM_Y;		// 矢印の下側Y座標
		CONSTANT short	BUTTON_X;				// ボタンのX座標
		CONSTANT short	BUTTON_Y;				// ボタンのY座標
		CONSTANT float	SLIDE_IN_SPEED;			// スライドインの速さ
		CONSTANT float	FONT_ALPHARATE;			// アルファレート
		CONSTANT float	IMAGE_MOVEMAX;			// ステージイメージの動く大きさ
		CONSTANT float	IMAGE_MOVESPEED;		// ステージイメージの動く速さ
		CONSTANT float	EXPLAIN_WIDTH;			// 説明板の幅
		CONSTANT float	EXPLAIN_HEIGHT;			// 説明板の高さ
		CONSTANT float	EXPLAIN_DRAWHEIGHT;		// 説明板の描画する高さ
		CONSTANT BYTE	NUMBER_SIZE;			// 数字画像の大きさ
		CONSTANT BYTE	STAGE_MAX;				// ステージ最大数
		CONSTANT BYTE	FADEOUT_SPEED;			// フェードアウトの速さ
		
		enum
		{
			FONT_STAGESELECT,	// ステージセレクト
		};

		//////////////////////////////////////////////////
		// フラグ定数
		//////////////////////////////////////////////////
		CONSTANT BYTE	FLG_RUN;		// 処理を走っている
		CONSTANT BYTE	FLG_IMAGE_UP;	// ステージイメージの上下

		//////////////////////////////////////////////////
		// メソッド
		//////////////////////////////////////////////////

		// コンストラクタ
		CStageSelect();

		// デストラクタ
		~CStageSelect();

		//////////////////////////////////////////////////
		//	・関数名	FileLoad
		//	・説明		読み込みや初期化を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void FileLoad();

		//////////////////////////////////////////////////
		//	・関数名	RunProc
		//	・説明		ステージセレクトメイン関数
		//	・引数		なし
		//	・戻り値	処理があるならtrue
		//				終了ならfalseを返す
		//////////////////////////////////////////////////
		bool RunProc();

		//////////////////////////////////////////////////
		//	・関数名	GetNext
		//	・説明		次のシーケンスを獲得
		//	・引数		なし
		//	・戻り値	ゲームメインを返す
		//////////////////////////////////////////////////
		IBase* GetNext();

	private:	// 非公開

		//////////////////////////////////////////////////
		// 型宣言
		//////////////////////////////////////////////////
		typedef void (CStageSelect::*FPUPDATE)();			// 更新関数ポインタ型
		typedef	std::list<CPointSpriteManager*>	BUBBLELIST;	// 泡(ポイントスプライト)リスト型

		//////////////////////////////////////////////////
		// フィールド
		//////////////////////////////////////////////////
		BYTE				m_Flg;			// フラグ
		CCamera				m_Camera;		// カメラ
		CProj				m_Proj;			// 投影
		CSprite				m_Sprite;		// スプライト描画
		CBoard				m_BackBoard;	// 背景板ポリゴン
		CBoard				m_StageImage;	// ステージイメージ用板ポリゴン
		CBoard				m_ExplainBoard;	// 説明用板ポリゴン
		CTexture			m_SceneTex;		// シーン用レンダーターゲット
		float				m_FontY;		// フォントのY座標
		float				m_StageImageY;	// ステージイメージのYの移動量
		float				m_PosZ;			// カメラのZ座標
		float				m_LockY;		// 注視点Y

		FPUPDATE			m_fpUpdate;		// 更新関数ポインタ
		char				m_StageIndex;	// ステージポインタ
		short				m_SceneAlpha;	// シーンのアルファ値

		BUBBLELIST			m_BubbleList;	// 泡リスト

		//////////////////////////////////////////////////
		// メソッド
		//////////////////////////////////////////////////

		//////////////////////////////////////////////////
		//	・関数名	Update
		//	・説明		更新を行う
		//				内部で関数ポインタを呼んでいるのみ
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Update();

		//////////////////////////////////////////////////
		//	・関数名	UpdateStartAnime
		//	・説明		シーン切り替え時に最初に行う更新
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateStartAnime();

		//////////////////////////////////////////////////
		//	・関数名	UpdateSelect
		//	・説明		ステージセレクト更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateSelect();

		//////////////////////////////////////////////////
		//	・関数名	UpdateFadeOut
		//	・説明		フェードアウトする
		//				更新を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateFadeOut();

		//////////////////////////////////////////////////
		//	・関数名	Draw
		//	・説明		描画を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Draw();
	};
}

#endif	// __AKI_STAGESELECT_H__