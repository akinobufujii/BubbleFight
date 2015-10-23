/////////////////////////////////
// インクルード
/////////////////////////////////
#include "RippleManager.h"
#include "BubbleManager.h"
#include "ShotManager.h"
#include "EnemyBase.h"
#include "FinTail.h"

#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "EnemyManager.h"
#include "GameMain.h"

#include "DamageObjectBase.h"
#include "FishManager.h"

#include "RushEnemy.h"

/////////////////////////////////
// 名前空間省略
/////////////////////////////////
using namespace Enemy;

/////////////////////////////////
// 静的メンバ実体化
/////////////////////////////////
const ULONG		CRush::SCORE			= 200;
const float		CRush::SPEED			= 1.f;
const float		CRush::TAIL_ROT_MAX		= 80.f;
const float		CRush::TAIL_ROT_SPEED	= CRush::TAIL_ROT_MAX / CRush::CHANGE_MOVE_TIME;
const float		CRush::RETURN_TAIL_SPEED= 5.f;
const USHORT	CRush::CHANGE_MOVE_TIME	= 5000;
const LPSTR		CRush::ENEMY_MESH_PASS	= "Enemy.x";

/////////////////////////////////
// 実装
/////////////////////////////////

// コンストラクタ
CRush::CRush( float x, float y, float z ):
m_lpMesh( CSXfileManager::GetInst()->GetData( ENEMY_MESH_PASS ) ),
m_NowTailRot( 0 ),
m_SaveTime( timeGetTime() )
{
	m_Flg |= SEARCH;
	m_mMat.SetTrans( x, y, z );

	// 波紋の設定
	m_Ripples.SetCreateHeight( m_lpMesh->GetRadius() );
	m_Ripples.SetSize(
		m_lpMesh->GetRadius(),
		m_lpMesh->GetRadius() );

	m_Tail.SetFarPos( &D3DXVECTOR3( 0, 0, 0 ) );
}

// デストラクタ
CRush::~CRush()
{
}

// 更新
void CRush::Update()
{
	// 破壊されているなら
	if( m_Flg & BREAK )
	{
		// 爆発更新
		if( m_Particle.IsErase() )
		{
			m_Flg |= ERASE;	// 消す
		}

		// パーティクル更新
		m_Particle.Update();

	}else{

		// 目標に回す
		// 何か見つけていたら
		if( m_lpTargetFish )
		{
			D3DXVECTOR3 Axis;
			float Rot = GetAxisAndRot( &Axis, m_lpTargetFish->GetPos(), m_mMat );

			if( Rot > CBase::DEFAULT_ROTSPEED )
			{
				Rot = CBase::DEFAULT_ROTSPEED;
			}

			m_mMat.RotateLocalAxis( &Axis, Rot );

		}else{

			UpdateRot();
		}

		// 検索中なら
		if( m_Flg & SEARCH )
		{
			// 目標が定まっていないなら
			if( m_lpTargetFish == NULL )
			{
				// 検索する
				SearchTarget();
			}
		
			// 尻尾の角度をだんだんと曲げていく
			m_NowTailRot = -TAIL_ROT_SPEED * (timeGetTime() - m_SaveTime);

			// 少しづつ後退
			UpdateCollisionMove( &D3DXVECTOR3( 0, 0, -0.1f ), 3 );

		}else{

			// 浮力が強くなければ突進
			if( !(m_Flg & BUBBLE_WRAP) )
			{
				// 何かに当たって動けないなら
				if( UpdateCollisionMove( &D3DXVECTOR3( 0, 0, SPEED ), 3 ) == false )
				{
					// 行動を切り替える
					m_Flg |= SEARCH;

					m_SaveTime = timeGetTime();
				}
				
				m_Bubbles.Add( &m_mMat, 1 );

				D3DXVECTOR3 Pos = m_mMat.GetPos(  );
			}

			// 尻尾の角度戻す
			if( m_NowTailRot > 0 )
			{
				m_NowTailRot = 0;
			}
		}

		// 時間によって行動を切り替える
		if( m_SaveTime + CHANGE_MOVE_TIME < timeGetTime() )
		{
			m_Flg ^= SEARCH;

			m_NowTailRot = 0;

			m_SaveTime = timeGetTime();
		}

		// 一定以上の泡になったら
		if( m_Flg & BUBBLE_WRAP )
		{
			// 浮力
			UpdateFloatPower();
		}

		// 泡拡大
		UpdateBubbleScale();

		// 当たられた時の処理
		UpdateHitMove();

		// 必ず呼び出す処理
		UpdateMust();
	}

	// 泡更新
	m_Bubbles.Update();

	if( m_mMat.GetPos().y > Sequence::CGameMain::SURFACESEA_HEIGHT )
	{
		m_mMat._42 = Sequence::CGameMain::SURFACESEA_HEIGHT;
	}

	// しっぽ更新
	CMatrix mRotX = m_mMat;

	mRotX.RotateLocalX( m_NowTailRot );
	m_Tail.Update( mRotX, (m_Flg & SEARCH)? 0 : SPEED );
}

// 描画
void CRush::Draw(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	if( IsCulling( &m_mMat ) )
	{
		if( !(m_Flg & BREAK) )
		{
			// 本体
			// シェーダ描画するならフォンシェーディング
			if( is_shader )
			{
				DrawShader( lpdevice, m_lpMesh );
				
			}else{

				// そうでなければ普通に描画
				lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
				
				m_lpMesh->Draw( lpdevice );
			}

			// 自分を包む泡
			DrawBubble( lpdevice );
		
			// しっぽ
			if( is_shader )
			{
				m_Tail.DrawShader( lpdevice );

			}else{

				m_Tail.Draw( lpdevice );
			}
		}
	}

	// 泡
	m_Bubbles.DrawNoBreak( lpdevice );
}

// エフェクト描画
void CRush::DrawEffect(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( m_Flg & BREAK )
	{
		DrawBreakEnemy( lpdevice );
	}

	m_Bubbles.DrawBreak( lpdevice );
}

// プレイヤーに対しての行動
void CRush::Action(Player::CDolphin &object)
{
	m_Flg |= SEARCH;
	m_SaveTime = timeGetTime();
}

// 襲うオブジェクトに対しての行動
void CRush::Action(CDamageObjectBase &object)
{
	m_Flg |= SEARCH;
	m_SaveTime = timeGetTime();
	m_lpTargetFish = NULL;
}