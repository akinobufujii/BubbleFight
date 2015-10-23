#ifndef __AKI_BUBBLE_H__
#define __AKI_BUBBLE_H__

///////////////////////////////
// 泡クラス
///////////////////////////////
class CBubble : virtual public IDoubleDispatch
{
public:	// 外部公開

	///////////////////////////////
	// 定数
	///////////////////////////////
	CONSTANT LPSTR	BUBBLE_MESH_PASS;	// 泡のメッシュパス
	CONSTANT USHORT	ERASE_TIME;			// 消える時間
	CONSTANT BYTE	BUBBLE_RAND;		// ぶれる係数
	CONSTANT float	SPEED;				// 速さ
	CONSTANT float	SPEED_MAX;			// 最大速度
	CONSTANT float	SCALE;				// 大きさ

	CONSTANT int	BURST_WIDTH;			// 爆発画像、幅
	CONSTANT int	BURST_HEIGHT;			// 爆発画像、高さ
	CONSTANT int	BURST_ANIME_SIZE;		// 爆発画像、アニメ１つの大きさ
	CONSTANT int	BURST_ANIME_WIDTH_NUM;	// 爆発画像、幅アニメ数
	CONSTANT int	BURST_ANIME_HEIGHT_NUM;	// 爆発画像、高さアニメ数
	CONSTANT int	BURST_ANIME_NUM;		// 爆発画像、アニメ総数

	///////////////////////////////
	// フラグ定数
	///////////////////////////////
	CONSTANT BYTE	ERASE;		// 消える
	CONSTANT BYTE	BREAK;		// 破裂
	CONSTANT BYTE	ENEMY_HIT;	// 敵に当たっている

	///////////////////////////////
	// メソッド
	///////////////////////////////

	// コンストラクタ
	CBubble();

	// デストラクタ
	~CBubble();

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
	//	・引数		
	//		- lpdevice	デバイス
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void Draw( const LPDIRECT3DDEVICE9 lpdevice );

	//////////////////////////////////////////////////
	//	・関数名	SetMatrix
	//	・説明		渡された行列に対して
	//				少しばらして自分の行列にする
	//	・引数		
	//		- mat	出す位置の行列
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void SetMatrix( const D3DXMATRIX& mat );

	//////////////////////////////////////////////////
	//	・関数名	BreakBubble
	//	・説明		この泡を破壊する
	//	・引数		なし
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void BreakBubble()
	{
		m_Flg |= BREAK;
	}

	//////////////////////////////////////////////////
	//	・関数名	GetFlg
	//	・説明		状態(フラグ)を獲得する
	//	・引数		なし
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline BYTE GetFlg() const
	{
		return m_Flg;
	}

	//////////////////////////////////////////////////
	//	・関数名	GetMatrix
	//	・説明		行列を返す
	//	・引数		なし
	//	・戻り値	行列をポインタで返す
	//////////////////////////////////////////////////
	inline const D3DXMATRIX* GetMatrix() const
	{
		return &m_mMat;
	}

	//////////////////////////////////////////////////
	//	・関数名	GetPos
	//	・説明		位置を獲得する
	//	・引数		なし
	//	・戻り値	位置を返す
	//////////////////////////////////////////////////
	inline D3DXVECTOR3 GetPos() const 
	{
		return m_mMat.GetPos();
	}

	//////////////////////////////////////////////////
	//	・関数名	GetSpeed
	//	・説明		現在の速さを獲得する
	//	・引数		なし
	//	・戻り値	速さを返す
	//////////////////////////////////////////////////
	inline float GetSpeed() const
	{
		return m_NowSpeed;
	}


	//////////////////////////////////////////////////
	//	・関数名	Dispatch
	//	・説明		送られてきたオブジェクトに自分を送りつける
	//	・引数		
	//		- object	行動を起こして欲しいオブジェクト
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void Dispatch( IDoubleDispatch* object )
	{
		object->Action( *this );
	}

	//////////////////////////////////////////////////
	//	・関数名	Action
	//	・説明		敵に対する行動(Dispatchからのみ呼ばれる)
	//	・引数		
	//		- object	行動を起こす対象のオブジェクト
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void Action( Enemy::CBase& object );

private:	// 非公開

	///////////////////////////////
	// フィールド
	///////////////////////////////
	CMatrix				m_mMat;		// 行列
	BYTE				m_Flg;		// フラグ
	float				m_NowSpeed;	// 今の速さ
	DWORD				m_StartTime;// 作られた時間
	BYTE				m_AnimeCnt;	// アニメカウンタ
	CBoard				m_Board;	// 板ポリゴン
	CPointSpriteManager	m_Points;	// ポイントスプライト
	CXfile*				m_lpMesh;	// メッシュ(共有)
};

#endif // __AKI_BUBBLE_H__