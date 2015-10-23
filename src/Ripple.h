#ifndef __AKI_REPPLE_H__
#define __AKI_REPPLE_H__

//////////////////////////////
// 波紋クラス
//////////////////////////////
class CRipple
{
public:	// 外部公開

	//////////////////////////////
	// 定数
	//////////////////////////////
	CONSTANT float	FADEOUT_SPEED;		// フェードアウトの速さ
	CONSTANT float	NEXT_RIPPLE_LINE;	// 次の波紋を作るボーダーライン
	CONSTANT float	DRAW_UPDOWN_POS;	// 上下の微妙な誤差

	//////////////////////////////
	// メソッド
	//////////////////////////////

	// コンストラクタ
	CRipple();

	// デストラクタ
	~CRipple();

	//////////////////////////////////////////////////
	//	・関数名	Update
	//	・説明		更新を行う
	//	・引数		なし
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void Update();

	//////////////////////////////////////////////////
	//	・関数名	DrawUp
	//	・説明		海面の上側の描画を行う
	//	・引数		
	//		- lp_device	デバイス
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void DrawUp( const LPDIRECT3DDEVICE9 lp_device );

	//////////////////////////////////////////////////
	//	・関数名	DrawDown
	//	・説明		海面の下側の描画を行う
	//	・引数		
	//		- lp_device	デバイス
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void DrawDown( const LPDIRECT3DDEVICE9 lp_device );

	//////////////////////////////////////////////////
	//	・関数名	SetPos
	//	・説明		出現させる位置を設定する
	//	・引数		
	//		- pos	位置
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void SetPos( const D3DXVECTOR3& pos )
	{
		m_Pos = pos;
	}

	//////////////////////////////////////////////////
	//	・関数名	SetRotY
	//	・説明		波紋をY軸回転させる角度
	//	・引数		
	//		- degree	角度
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void SetRotY( const float degree )
	{
		CMatrix mXRot;
		mXRot.SetRotX( 90 );

		m_mRotXY.SetRotY( degree );

		m_mRotXY = mXRot * m_mRotXY;
	}

	//////////////////////////////////////////////////
	//	・関数名	SetSize
	//	・説明		波紋の幅高さを設定する
	//	・引数		
	//		- width		幅
	//		- height	高さ
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void SetSize( const float width, const float height )
	{
		m_Width = width;
		m_Height= height;
	}

	//////////////////////////////////////////////////
	//	・関数名	SetScaleRate
	//	・説明		波紋の広がっていく大きさを設定する
	//	・引数		
	//		- rate	大きさ
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void SetScaleRate( const float rate )
	{
		m_ScaleRate = rate;
	}

	//////////////////////////////////////////////////
	//	・関数名	IsNext
	//	・説明		次の波紋を作って欲しいかどうかを
	//				調べる
	//	・引数		
	//		- rate	大きさ
	//	・戻り値	次を生み出して欲しいならtrue
	//				そうでなければfalseを返す
	//////////////////////////////////////////////////
	inline bool IsNext() const
	{
		return (NEXT_RIPPLE_LINE > m_NowAlpha)? true : false;
	}

	//////////////////////////////////////////////////
	//	・関数名	IsErase
	//	・説明		この波紋を消して欲しいかどうかを
	//				調べる
	//	・引数		
	//		- rate	大きさ
	//	・戻り値	消すべきならtrue
	//				そうでなければfalseを返す
	//////////////////////////////////////////////////
	inline bool IsErase() const 
	{
		return (m_NowAlpha <= 0)? true : false;
	}

private:	// 非公開

	//////////////////////////////
	// フィールド
	//////////////////////////////
	CBoard	m_Borad;	// 板ポリゴン
	float	m_NowAlpha;	// 今のアルファ値(0.0～1.0)
	float	m_Width;	// 幅
	float	m_Height;	// 高さ
	float	m_ScaleRate;// 拡大の比率
	CVector	m_Pos;		// 位置
	CMatrix	m_mRotXY;	// X軸、Y軸の回転が入った行列
};

#endif	// __AKI_REPPLE_H__