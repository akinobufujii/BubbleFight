#ifndef __AKI_GAMEMAIN_H__
#define __AKI_GAMEMAIN_H__

//////////////////////////////////////////////////
// 不完全型宣言
//////////////////////////////////////////////////
class CSurfaceSea;
class CUI;
class CPlanktonManager;
class CStoneManager;
class CFishManager;
class CSunLineManager;
namespace Enemy
{
	class CManager;
}

namespace Sequence
{
	//////////////////////////////////////////////////
	// ゲーム本編
	//////////////////////////////////////////////////
	class CGameMain : public IBase
	{	
	public:
		//////////////////////////////////////////////////
		// 型
		//////////////////////////////////////////////////
		typedef BYTE	FLGTYPE;

		//////////////////////////////////////////////////
		// 定数
		//////////////////////////////////////////////////
		CONSTANT LPSTR	PATH_STONEDATA;			// 岩のファイルパス
		CONSTANT LPSTR	SKY_MESH_PASS;			// 空メッシュパス
		CONSTANT LPSTR	PATH_ENEMYDATA;			// 敵データ読み込みファイルパス
		CONSTANT LPSTR	PATH_DAMAGEFISHDATA;	// ダメージを受ける魚読み込みパス
		CONSTANT BYTE	BUBBLES_NUM;			// 泡が噴き出している一つの数
		CONSTANT float	SURFACE_ALPHA_RATE;		// 海の透過レート
		CONSTANT float	SURFACESEA_HEIGHT;		// 海面の高さ
		CONSTANT float	BOTTOMSEA_HEIGHT;		// 海底の高さ
		CONSTANT float	SEA_MOVELIMIT;			// 海の中の制限
		CONSTANT float	STAGE_SCALE;			// ステージ拡大係数
		CONSTANT float	SEA_COLOR_ADD;			// 海の色の加算数(プレイヤーの位置によって変わる)
		CONSTANT float	FOG_DEPTH_ADD;			// フォグの濃さ
		CONSTANT float	SCROLL_SPEED;			// テクスチャのスクロールの速さ
		CONSTANT float	JUMP_CAMERA_HEIGHT;		// プレイヤーがジャンプしている時の高さ
		CONSTANT float	FOCUS_RATE;				// フォーカスレート
		CONSTANT short	DEFAUL_GAMESCENE_ALPHA;	// ゲームシーンのアルファ値

		//////////////////////////////////////////////////
		// フラグ定数
		//////////////////////////////////////////////////
		CONSTANT FLGTYPE	FLG_RESULT;		// 結果が出たかどうか
		CONSTANT FLGTYPE	FLG_RUN;		// 処理を走るかどうか
		CONSTANT FLGTYPE	FLG_BOSSBATTLE;	// ボスバトルかどうか
		CONSTANT FLGTYPE	FLG_BOSSENTER;	// ボス登場

		//////////////////////////////////////////////////
		// メソッド
		//////////////////////////////////////////////////

		// コンストラクタ
		CGameMain( BYTE stage );

		// デストラクタ
		~CGameMain();

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
		//	・戻り値	クリアシーケンスのポインタか、
		//				ゲームオーバーシーケンスのポインタを
		//				返す
		//////////////////////////////////////////////////
		IBase* GetNext();
		
		//////////////////////////////////////////////////
		//	・関数名	GetCamera
		//	・説明		このクラスの使っているカメラを返す
		//	・引数		なし	
		//	・戻り値	カメラクラスのポインタ
		//////////////////////////////////////////////////
		static inline const CCamera* GetCamera()
		{
			return &m_Camera;
		}

	private:

		//////////////////////////////////////////////////
		// フィールド
		//////////////////////////////////////////////////
		Player::CDolphin*	m_lpPlayer;			// プレイヤー
		Boss::CBase*		m_lpBoss;			// ボスプレイヤー
		Enemy::CManager*	m_lpEnemys;			// 敵マネージャ
		CPlanktonManager*	m_lpPlanktons;		// プランクトン
		CUI*				m_lpUI;				// インターフェイス
		CStoneManager*		m_lpStones;			// 岩
		CLimitWalls			m_Walls;			// 壁
		CFishManager*		m_lpFishes;			// 魚
		CSurfaceSea*		m_lpSurfaceSea;		// 海面
		CXfile*				m_lpSky;			// 空のメッシュ
		CVector				m_vSavePos;			// 前のカメラ位置保存用
		boost::timer		m_Timer;			// タイマー
		CSprite*			m_lpSprite;			// スプライト
		CSunLineManager*	m_lpSunLines;		// 太陽線マネージャー
	
		CBoard				m_BottomSea;		// 海底用板ポリゴン
		CBoard				m_SceneBorad;		// シーン描画用ポリゴン

		CTexture			m_SceneTex;			// オフスクリーン用テクスチャ
		CTexture			m_MiniSizeTex;		// 縮小用レンダーテクスチャ
		CTexture			m_BlurTex;			// ぼかし用レンダーテクスチャ
		CTexture			m_DepthTex;			// 深度保存用レンダーテクスチャ
		CTexture			m_ResultTex;		// 最終的に描画するテクスチャ

		CEffect				m_BlurShader;		// ぼかしシェーダー
		CEffect				m_DepthShader;		// 深度計算シェーダー
		CEffect				m_DOFShader;		// 被写界深度シェーダー
		CEffect				m_BombBlurShader;	// 爆発ブラーシェーダ－

		LPDIRECT3DSURFACE9	m_lpBackBuffer;		// バックバッファ保存用
		LPDIRECT3DSURFACE9	m_lpZBuffer;		// Zバッファ保存用
		D3DVIEWPORT9		m_VP;				// ビューポート保存
		short				m_DamageAlphaCnt;	// ダメージのアルファ値
		short				m_GameSceneAlpha;	// ゲーム画面のアルファ値
		BYTE				m_StageIndex;		// ステージ
		BYTE				m_EnemyNum;			// 最初にいる敵数

		FLGTYPE				m_Flg;				// フラグ
		DWORD				m_SaveEnterBossTime;// ボス搭乗時間記録用

		std::string			m_BGMPath;			// BGMのファイルパス

		CProj				m_Proj;				// 投影
		static CCamera		m_Camera;			// カメラ(グローバル化の為)
		
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
		//	・関数名	Collision
		//	・説明		衝突判定、応答を行う
		//	・引数		なし	
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Collision();

		//////////////////////////////////////////////////
		//	・関数名	Draw
		//	・説明		描画を行う
		//	・引数		なし	
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Draw();

		//////////////////////////////////////////////////
		//	・関数名	DrawMesh
		//	・説明		メッシュ描画を行う
		//				trueを渡すとプログラマブルシェーダで
		//				描画を行う
		//	・引数		
		//		- lp_effect	プログラマブルシェーダで
		//					描画を行うかどうか
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void DrawMesh( bool is_effect );

		//////////////////////////////////////////////////
		//	・関数名	DrawPolygon
		//	・説明		メッシュ以外の3Dオブジェクト描画を行う
		//				trueを渡すとプログラマブルシェーダで
		//				描画を行う
		//	・引数		
		//		- lp_effect	プログラマブルシェーダで
		//					描画を行うかどうか
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void DrawPolygon( bool is_effect );

		//////////////////////////////////////////////////
		//	・関数名	DrawEffect
		//	・説明		各エフェクト描画を行う
		//				Zバッファ関連を切って描画を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void DrawEffect();

		//////////////////////////////////////////////////
		//	・関数名	DrawSprite
		//	・説明		スプライト描画を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void DrawSprite();

		//////////////////////////////////////////////////
		//	・関数名	Rendering
		//	・説明		DrawMesh→DrawPolygon→DrawEffectの
		//				順番で描画を行う
		//				trueを渡すとプログラマブルシェーダで
		//				描画を行う
		//	・引数		
		//		- lp_effect	プログラマブルシェーダで
		//					描画を行うかどうか
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Rendering( bool is_effect );

		//////////////////////////////////////////////////
		//	・関数名	Load
		//	・説明		読み込みや生成を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Load();

		//////////////////////////////////////////////////
		//	・関数名	LoadStage
		//	・説明		ステージごとのデータ読み込みを行う
		//	・引数		
		//		- stage	ステージのインデックス番号
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void LoadStage( BYTE stage );

		//////////////////////////////////////////////////
		//	・関数名	Allocs
		//	・説明		メンバ変数のメモリ確保を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Allocs();

		//////////////////////////////////////////////////
		//	・関数名	Release
		//	・説明		解放と破壊を行う
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Release();
	};
}

#endif // __AKI_GAMEMAIN_H__	