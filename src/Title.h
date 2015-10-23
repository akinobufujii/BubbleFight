#ifndef __AKI_TITLE_H__
#define __AKI_TITLE_H__

////////////////////////////////////
// タイトルシーケンス
////////////////////////////////////
namespace Sequence
{
	class CTitle : public IBase
	{
	public:	// 外部公開

		////////////////////////////////////
		// 定数
		////////////////////////////////////
		CONSTANT LPSTR	TEXTURE_LOAD_FILE_PASS;	// 読み込むテクスチャのファイルパス
		CONSTANT LPSTR	XFILE_LOAD_FILE_PASS;	// 読み込むXファイルのファイルパス
		CONSTANT LPSTR	BUBBLE_TEX;				// 泡のテクスチャ
		CONSTANT LPSTR  MENU_TEX;				// メニューのテクスチャ
		CONSTANT LPSTR  TITLELOGO_TEX;			// タイトルロゴテクスチャ
		CONSTANT short	MENU_WIDTH;				// メニュー1つの幅
		CONSTANT short	MENU_HEIGHT;			// メニュー1つの高さ
		CONSTANT short	MENU_X;					// メニューのX座標
		CONSTANT short	MENU_Y;					// メニューのY座標
		CONSTANT short	TITLELOGO_WIDTH;		// タイトルロゴの幅
		CONSTANT short	TITLELOGO_HEIGHT;		// タイトルロゴの高さ
		CONSTANT short	THANKSLOGO_WIDTH;		// 提供の幅
		CONSTANT short	THANKSLOGO_HEIGHT;		// 提供の高さ
		CONSTANT float	BUBBLE_X_POS;			// 泡の出るX座標(左右対称)
		CONSTANT float	BUBBLE_Y_POS;			// 泡の出るY座標(共通)
		CONSTANT float	BUBBLE_SPEED_LIMIT;		// 泡の最大スピード
		CONSTANT BYTE	BUBBLE_NUM;				// 泡の数
		CONSTANT USHORT	BUBBLE_ERASE_TIME;		// 泡がリセットされる時間
		
		////////////////////////////////////
		// 列挙定数
		////////////////////////////////////
		// メニューの種類
		enum MENU_TYPE
		{
			MENU_START,	// スタート
			MENU_END,	// 終了
			MENU_NUM	// 数
		};

		////////////////////////////////////
		// メソッド
		////////////////////////////////////

		// コンストラクタ
		CTitle();

		// デストラクタ
		~CTitle();

		//////////////////////////////////////////////////
		//	・関数名	FileLoad
		//	・説明		読み込みや初期化を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void FileLoad();

		//////////////////////////////////////////////////
		//	・関数名	RunProc
		//	・説明		タイトルメイン関数
		//	・引数		なし
		//	・戻り値	処理があるならtrue
		//				終了ならfalseを返す
		//////////////////////////////////////////////////
		bool RunProc();

		//////////////////////////////////////////////////
		//	・関数名	GetNext
		//	・説明		次のシーケンスを獲得
		//	・引数		なし
		//	・戻り値	ステージセレクトを返す
		//////////////////////////////////////////////////
		IBase* GetNext();

	private:	// 非公開

		//////////////////////////////////////////////////
		// フィールド
		//////////////////////////////////////////////////
		CSprite				m_Sprite;			// スプライト
		CProj				m_Proj;				// 投影クラス
		CCamera				m_Camera;			// カメラクラス
		CPointSpriteManager	m_BubblesLeft;		// 左側の泡
		CPointSpriteManager	m_BubblesRight;		// 右側の泡
		CPointSpriteManager m_SelectMenuBubble;	// 泡破裂用
		CTexture			m_SceneTex;			// シーンテクスチャ
		char				m_SelectMenu;		// 選んでいるメニュー
		bool				m_IsRun;			// 処理を走るか
		bool				m_IsSelect;			// 選んだか
		bool				m_IsTeachAlphaPlus;	// アルファ値を加算するかどうか
		short				m_Alpha;			// アルファ
		short				m_TeachAlpha;		// 指示のアルファ値

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
		//	・関数名	Load
		//	・説明		読み込みや生成を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Load();

		//////////////////////////////////////////////////
		//	・関数名	Release
		//	・説明		解放と破壊を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Release();
	};
}

#endif // __AKI_TITLE_H__