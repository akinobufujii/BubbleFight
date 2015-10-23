//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "BubbleManager.h"
#include "ShotManager.h"

#include "StoneManager.h"

#include "BossBase.h"

//////////////////////////////////////////////////
// 名前空間省略
//////////////////////////////////////////////////
using namespace Boss;

//////////////////////////////////////////////////
// 静的メンバ実体化
//////////////////////////////////////////////////
const CBase::FLGTYPE	CBase::FLG_ALIVE	= 0x01;	
const CBase::FLGTYPE	CBase::FLG_BREAK	= 0x02;
const CBase::FLGTYPE	CBase::FLG_ERASE	= 0x04;
const CBase::FLGTYPE	CBase::FLG_STOP		= 0x08;
const CBase::FLGTYPE	CBase::FLG_ATTACK	= 0x10;
const CBase::FLGTYPE	CBase::FLG_STICK	= 0x20;
const CBase::FLGTYPE	CBase::FLG_DAMAGE	= 0x40;
const CBase::FLGTYPE	CBase::FLG_WAIT		= 0x80;

CMatrix			CBase::m_PlayerMat;
CStoneManager*	CBase::m_lpStones = NULL;

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CBase::CBase():
m_Flg( FLG_ALIVE ),
m_NowMovement( m_MovementList.end() )
{
}

// 行動更新
void CBase::UpdateMovement()
{
	static bool IsCall = false;	// 呼ばれたかどうか

	// 一番最初なら
	if( IsCall == false )
	{
		IsCall = true;

		// 初期化
		m_NowMovement = m_MovementList.begin();

		// ランダムで行動決定
		for( int i = (rand() % m_MovementList.size()); i > 0; i-- )
		{
			m_NowMovement++;
		}
	
	}
	
	// 実行してその行動が終われば
	if( (this->*(*m_NowMovement))() == false )
	{
		// 初期化
		m_NowMovement = m_MovementList.begin();

		// ランダムで行動決定
		for( int i = (rand() % m_MovementList.size()); i > 0; i-- )
		{
			m_NowMovement++;
		}
	}
}