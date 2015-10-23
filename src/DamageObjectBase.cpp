/////////////////////////////////
// インクルード
/////////////////////////////////
#include "EnemyManager.h"
#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "GameMain.h"

#include "BubbleManager.h"

#include "DamageObjectBase.h"

/////////////////////////////////
// 実装
/////////////////////////////////

// コンストラクタ
CDamageObjectBase::CDamageObjectBase():
m_lpBubbles( NEW CBubbleManager() )
{
}

// デストラクタ
CDamageObjectBase::~CDamageObjectBase()
{
	SAFE_DELETE( m_lpBubbles );
}

// 派生先が必ず呼び出してほしい処理
void CDamageObjectBase::MustUpdate()
{
	// 判定
	if( m_mMat.GetPos().y > Sequence::CGameMain::SURFACESEA_HEIGHT )
	{
		m_mMat._42 = Sequence::CGameMain::SURFACESEA_HEIGHT;
	}

	// 泡追加
	m_lpBubbles->Add( &m_mMat, 1 );

	m_lpBubbles->Update();
}