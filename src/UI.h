#ifndef __AKI_UI_H__
#define __AKI_UI_H__

//////////////////////////////////////////////////
// 不完全型宣言
//////////////////////////////////////////////////
namespace Enemy
{
	class CManager;
}

//////////////////////////////////////////////////
// ユーザーインターフェイス
//////////////////////////////////////////////////
class CUI
{
public:	// 外部公開

	//////////////////////////////////////////////////
	// 定数
	//////////////////////////////////////////////////
	CONSTANT int	FONT_WIDTH;			// フォント画像の幅
	CONSTANT int	FONT_HEIGHT;		// フォント画像の高さ
	CONSTANT int	NUMBER_WIDTH;		// 番号画像の幅
	CONSTANT int	NUMBER_HEIGHT;		// 番号画像の高さ
	CONSTANT int	LIFEGAGE_WIDTH;		// ライフゲージ幅
	CONSTANT int	LIFEGAGE_HEIGHT;	// ライフゲージ高さ
	CONSTANT int	POWERGAGE_WIDTH;	// パワーゲージ幅
	CONSTANT int	POWERGAGE_HEIGHT;	// パワーゲージ高さ
	CONSTANT int	ENEMYPOSTEX_SIZE;	// レーダーの敵画像サイズ
	CONSTANT int	RADAR_SIZE;			// レーダー画像サイズ
	CONSTANT int	BLESSGAGE_WIDTH;	// 息ゲージ幅
	CONSTANT int	BLESSGAGE_HEIGHT;	// 息ゲージ高さ
	CONSTANT float	SEARCH_RADIUS;		// レーダーの感知半径

	//////////////////////////////////////////////////
	// メソッド
	//////////////////////////////////////////////////

	// コンストラクタ
	CUI();

	// デストラクタ
	~CUI();

	//////////////////////////////////////////////////
	//	・関数名	Update
	//	・説明		更新を行う
	//	・引数		
	//		- score			現在のスコア
	//		- hp			プレイヤーのHP
	//		- power			プレイヤーのためている強さ
	//		- bless			プレイヤーの息残量
	//		- now_enemys	今の敵の数
	//		- all_enemys	敵の総数
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void Update(
		ULONG	score,
		USHORT	hp,
		float	power,
		USHORT	bless,
		BYTE	now_enemys,
		BYTE	all_enemys);

	//////////////////////////////////////////////////
	//	・関数名	Draw
	//	・説明		描画を行う
	//	・引数		
	//		- lpsprite	スプライトデバイス
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void Draw( const LPD3DXSPRITE lpsprite );

	//////////////////////////////////////////////////
	//	・関数名	DrawRadar
	//	・説明		レーダーの描画を行う
	//	・引数		
	//		- lpsprite		スプライトデバイス
	//		- lp_player_mat	プレイヤーの行列
	//		- lp_enemys		敵の情報が入ったリスト
	//		- lp_boss		ボスの位置(NULL入れれば表示されない)
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void DrawRadar(
		const LPD3DXSPRITE lpsprite, 
		const D3DXMATRIX* lp_player_mat,
		Enemy::CManager* lp_enemys,
		const Boss::CBase* lp_boss );

	//////////////////////////////////////////////////
	//	・関数名	SetAlpha
	//	・説明		アルファ値設定を行う
	//	・引数		
	//		- alpha	アルファ値(0～255)
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void SetAlpha( int alpha )
	{
		m_Alpha = alpha;
	}

private:	// 非公開

	//////////////////////////////////////////////////
	// フィールド
	//////////////////////////////////////////////////
	CSTexManager*	m_lpTexManager;	// テクスチャマネージャ
	ULONG			m_Score;		// スコア
	USHORT			m_HP;			// HP
	USHORT			m_Bless;		// 息
	float			m_Power;		// パワー
	int				m_Alpha;		// アルファ値
	BYTE			m_NowEnemys;	// 今の敵数
	BYTE			m_AllEnemys;	// 敵総数
};

#endif // __AKI_UI_H__