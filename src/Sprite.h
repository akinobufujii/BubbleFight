#ifndef __AKI_SPRITE_H__
#define __AKI_SPRITE_H__

/////////////////////////////////////
// スプライトデバイスラッピングクラス
/////////////////////////////////////
class CSprite
{
public:	// 外部公開

	/////////////////////////////////////
	// メソッド
	/////////////////////////////////////

	// コンストラクタ
	CSprite();
	CSprite( const LPDIRECT3DDEVICE9 lpdevice );

	// デバイス作成
	bool Create( const LPDIRECT3DDEVICE9 lpdevice );

	// デバイス獲得
	inline const LPD3DXSPRITE GetDevice()
	{
		return m_lpSprite;
	}

	// 描画開始
	inline const LPD3DXSPRITE Begin( int flg = D3DXSPRITE_ALPHABLEND )
	{
		m_lpSprite->Begin( flg );
		return m_lpSprite;
	} 

	// 描画終了
	inline void End()
	{
		m_lpSprite->End();
	}

	// 描画
	void Draw(
		const LPDIRECT3DTEXTURE9 lptex,
		float dest_x,
		float dest_y,
		float rot,
		int src_left,
		int src_top, 
		int src_right,
		int src_bottom );

	// すでにあるデバイスで描画
	static void Draw(
		const LPD3DXSPRITE lpsprite,
		const LPDIRECT3DTEXTURE9 lptex,
		float dest_x,
		float dest_y,
		float rot,
		int src_left,
		int src_top,
		int src_right,
		int src_bottom,
		int alpha = 0xff );

	// 不透明度設定
	inline void SetAlpha( int alpha )
	{
		m_Alpha = alpha;
	}

	// デストラクタ
	~CSprite();

private:	// 非公開

	/////////////////////////////////////
	// フィールド
	/////////////////////////////////////
	LPD3DXSPRITE	m_lpSprite;	// スプライトデバイス
	int				m_Alpha;	// 不透明度
};

#endif // __AKI_SPRITE_H__