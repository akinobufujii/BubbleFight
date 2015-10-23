#ifndef __AKI_GRAPHICANIME_H__
#define __AKI_GRAPHICANIME_H__

//////////////////////////////////////////////////
// 画像アニメーション管理クラス
//////////////////////////////////////////////////
class CGraphicAnime
{
public:	// 外部公開

	//////////////////////////////////////////////////
	// フラグ定数
	//////////////////////////////////////////////////
	CONSTANT BYTE	FLG_MOVE_WIDTH;	// 横にアニメーション
	CONSTANT BYTE	FLG_LOOP_ANIME;	// ループアニメーション
	CONSTANT BYTE	FLG_BACK_LOOP;	// 折り返しループ
	CONSTANT BYTE	FLG_NOW_BACK;	// バックアニメーション中

	//////////////////////////////////////////////////
	// メソッド
	//////////////////////////////////////////////////

	// コンストラクタ
	CGraphicAnime();

	// デストラクタ
	~CGraphicAnime();

	//////////////////////////////////////////////////
	//	・関数名	Update
	//	・説明		更新を行う
	//	・引数		なし
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void Update();

	// アニメーション設定
	void SetAnimeSize( 
		const float width,
		const float height,
		const float anime_width,
		const float anime_height );

	// アニメーション数設定
	void SetAnimeNum(
		const BYTE num,
		const BYTE width_num,
		const BYTE height_num );

	// アニメーション方法設定
	void SetAnimetionType( const bool is_move_width );
	
	// ループ設定
	void SetLoop( const bool is_loop = true, const bool is_back = false );

	// 時間設定
	inline void SetTime( const DWORD time )
	{
		m_NextAnimeTime = time;
	}

	// テクスチャアニメーションの位置獲得
	inline const D3DXVECTOR2* GetTexPos() const 
	{
		return m_TexPos;
	}

	// 1周したかどうか
	bool IsOneRound();

	// リセット
	void Reset();

private:	// 非公開

	//////////////////////////////////////////////////
	// フィールド
	//////////////////////////////////////////////////
	DWORD		m_SaveTime;		// 時間保存用
	DWORD		m_NextAnimeTime;// 次のアニメーションに行く
	BYTE		m_AnimeCnt;		// アニメカウンタ
	BYTE		m_WidthNum;		// 幅数
	BYTE		m_HeightNum;	// 高さ数
	BYTE		m_AnimeNum;		// アニメーション数
	float		m_WidthRate;	// 幅レート
	float		m_HeightRate;	// 高さレート
	BYTE		m_Flg;			// フラグ
	D3DXVECTOR2	m_TexPos[ 4 ];	// テクスチャ位置

	//CBoard	m_Board;		// 板ポリゴン
};

#endif	// __AKI_GRAPHICANIME_H__