//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "BubbleManager.h"

#include "ShotBase.h"
#include "NormalShot.h"
#include "ShotManager.h"

#include "BossBase.h"

#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "EnemyManager.h"
#include "UI.h"
#include "GameMain.h"

#include "PowerShot.h"

//////////////////////////////////////////////////
// 名前空間省略
//////////////////////////////////////////////////
using namespace Shot;

//////////////////////////////////////////////////
// 静的メンバ実体化
//////////////////////////////////////////////////
const float CPower::SPEED			= 2.5f;
const LPSTR	CPower::SHOT_MESH_PASS	= "Spike.x";

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CPower::CPower():
m_StartTime( timeGetTime() ),
m_lpMesh( CSXfileManager::GetInst()->GetData( SHOT_MESH_PASS ) )
{
}

CPower::CPower(const D3DXMATRIX *mat):
CBase( mat ),
m_StartTime( timeGetTime() ),
m_lpMesh( CSXfileManager::GetInst()->GetData( SHOT_MESH_PASS ) )
{
}

// 更新
void CPower::Update()
{
	// 生きていたら
	if( m_Flg & FLG_ALIVE )
	{
		// 移動
		m_mMat.MoveLocal( 0, 0, SPEED );

		// 泡追加
		m_Bubbles.Add( &m_mMat, 1 );

		// 一定時間過ぎたら消える
		if( timeGetTime() - m_StartTime > 3000 )
		{
			m_Flg &= ~FLG_ALIVE;
		}
	}

	m_Bubbles.Update();

	// 死んでいて泡もなければ
	if( ~(m_Flg & FLG_ALIVE) && m_Bubbles.IsEmpty() )
	{
		m_Flg |= FLG_ERASE;	// 消す
	}
}

// 描画
void CPower::Draw(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( IsCulling( &m_mMat ) )
	{
		lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );

		m_lpMesh->Draw( lpdevice );
	}
}

// 敵に対して
void CPower::Action(Enemy::CBase &object)
{
	//if( ~(object.GetFlg() & Enemy::CBase::BREAK) )
	{
		m_Flg &= ~FLG_ALIVE;
	}
}

// デストラクタ
CPower::~CPower()
{
}