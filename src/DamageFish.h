#ifndef __AKI_DAMAGEFISH_H__
#define __AKI_DAMAGEFISH_H__

/////////////////////////////////
// いじめられる魚
// プレイヤーが助ける対象
/////////////////////////////////
class CDamageFish : virtual public CDamageObjectBase
{
public:	// 外部公開

	/////////////////////////////////
	// 定数
	/////////////////////////////////
	CONSTANT float	MOVESPEED;	// 移動の速さ
	CONSTANT float	MOVEHEIGHT;	// 動きまわる高さ(固定)
	CONSTANT float	TURNSPEED;	// 回る速度

	/////////////////////////////////
	// メソッド
	/////////////////////////////////

	// コンストラクタ
	CDamageFish();

	// デストラクタ
	~CDamageFish();

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
	void Draw( const LPDIRECT3DDEVICE9 lp_device, bool is_shader );

	//////////////////////////////////////////////////
	//	・関数名	Dispatch
	//	・説明		自分の情報を送られてきた
	//				オブジェクトに送りつける
	//	・引数		
	//		- object	このクラスの情報を送りつける
	//					オブジェクト
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void Dispatch( IDoubleDispatch* object )
	{
		object->Action( *this );
	}

	//////////////////////////////////////////////////
	//	・関数名	Action
	//	・説明		敵に対しての行動を起こす
	//				Dispatchから呼ばれる
	//	・引数		行動を起こす対象のオブジェクト
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void Action( Enemy::CBase& object );

private:	// 非公開
};

#endif // __AKI_DAMAGEFISH_H__