/////////////////////////////
// インクルード
/////////////////////////////
#include "BubbleManager.h"

#include "ShotManager.h"

#include "ShotBase.h"

#include "RippleManager.h"
#include "EnemyBase.h"
#include "ShotEnemy.h"

/////////////////////////////
// 名前空間省略
/////////////////////////////
using namespace Enemy;

/////////////////////////////
// 静的メンバ実体化
/////////////////////////////
const ULONG	CShoot::SCORE			= 150;
const LPSTR	CShoot::ENEMY_MESH_PASS	= "ShotEnemy.x";

/////////////////////////////
// 実装
/////////////////////////////

// コンストラクタ
CShoot::CShoot( float x, float y, float z  ):
SHOT_INTERVAL( rand() % 1501 + 2000 ),	// ※ちょっとバラつかせる
m_ShotSaveTime( timeGetTime() ),
m_lpMesh( CSXfileManager::GetInst()->GetData( ENEMY_MESH_PASS ) )
{
	m_mMat.SetTrans( x, y, z );

	// 波紋の設定
	m_Ripples.SetCreateHeight( m_lpMesh->GetRadius() );
	m_Ripples.SetSize( m_lpMesh->GetRadius(), m_lpMesh->GetRadius() );
}

// デストラクタ
CShoot::~CShoot()
{
}

// 更新
void CShoot::Update()
{
	// 破壊されてたら
	if( m_Flg & BREAK )
	{
		// 爆発更新
		if( m_Particle.IsErase() )
		{
			m_Flg |= ERASE;	// 消す
		}

		// パーティクル更新
		m_Particle.Update();

		// 泡更新
		m_Bubbles.Update();

	}else{

		// 一定以上の泡になったら
		if( m_Flg & BUBBLE_WRAP )
		{
			// 浮力
			UpdateFloatPower();

		}else{

			// 画面内に入っていいれば攻撃
			if( IsCulling( &m_mMat ) )
			{
				// 自機の方に向く
				UpdateRot();

				// 撃つ
				UpdateShot();
			}

			// 吹っ飛びの処理
			UpdateHitMove();
		}

		// 泡拡大
		UpdateBubbleScale();

		// 事後処理
		UpdateMust();
	}
}

// 描画
void CShoot::Draw(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	if( !(m_Flg & BREAK) )
	{
		if( IsCulling( &m_mMat ) )
		{
			lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );

			m_lpMesh->Draw( lpdevice );

			// 泡
			DrawBubble( lpdevice );
		}

		m_Shots.Draw( lpdevice );
	}
}

// エフェクト描画
void CShoot::DrawEffect(const LPDIRECT3DDEVICE9 lpdevice)
{
	// 爆発を描画
	if( m_Flg & BREAK )
	{
		DrawBreakEnemy( lpdevice );
	}

	m_Shots.DrawEffect( lpdevice );
}

// 弾更新
void CShoot::UpdateShot()
{
	// 一定時間超えたら
	if( timeGetTime() - m_ShotSaveTime > SHOT_INTERVAL )
	{
		m_Shots.Add( Shot::CManager::NORMAL, &m_mMat );

		m_ShotSaveTime = timeGetTime();
	}

	m_Shots.Update();
}