#ifndef __AKI_DOUBLEDISPATCH_H__
#define __AKI_DOUBLEDISPATCH_H__

/////////////////////////////////////
// 不完全型宣言
/////////////////////////////////////
namespace Player
{
	class CDolphin;
}

namespace Enemy
{
	class CBase;	
}

namespace Shot
{
	class CBase;
}

namespace Boss
{
	class CBase;
}

class CBubble;
class CDamageObjectBase;

/////////////////////////////////////
// 当たり判定用インターフェース
// 当たり判定ある物全てに継承させる
// Dispatchは必ず定義
// Actionは必要に応じてオーバーライドする
/////////////////////////////////////
interface IDoubleDispatch
{
public:
	// 送りつける
	virtual void Dispatch( IDoubleDispatch* object ) = 0;
	
	/////////////////////////////////////
	// 以下行動関数
	// 必要に応じてオーバーライドする
	/////////////////////////////////////
	// 自機に対して
	virtual void Action( Player::CDolphin& object ){}

	// 敵に対して
	virtual void Action( Enemy::CBase& object ){}
	
	// 弾に対して
	virtual void Action( Shot::CBase& object ){}

	// ボスに対して
	virtual void Action( Boss::CBase& object ){}

	// 泡に対して
	virtual void Action( CBubble& object ){}

	// いじめられる魚に対して
	virtual void Action( CDamageObjectBase& object ){}
};

#endif // __AKI_DOUBLEDISPATCH_H__