////////////////////////////////
// インクルード
////////////////////////////////
#include "EnemyManager.h"
#include "UI.h"

#include "Bubble.h"
#include "BubbleManager.h"

#include "ShotBase.h"
#include "NormalShot.h"
#include "ShotManager.h"

#include "FishManager.h"
#include "RippleManager.h"
#include "LimitWalls.h"
#include "GameMain.h"

#include "PointSpriteManager.h"
#include "EnemyBase.h"

#include "RippleManager.h"
#include "Fintail.h"
#include "Screw.h"
#include "StrongDolphin.h"

#include "DamageObjectBase.h"

#include "WaterSpray.h"

#include "FinTail.h"

#include "NormalEnemy.h"

////////////////////////////////
// 名前空間の省略
////////////////////////////////
using namespace Enemy;

////////////////////////////////
// 静的メンバ実体化
////////////////////////////////
const float CNormal::SPEED				= 0.45f;
const float	CNormal::ROT_SPEED			= 8.f;
const ULONG	CNormal::SCORE				= 100;
const LPSTR	CNormal::ENEMY_MESH_PASS	= "Chara.x";
const DWORD	CNormal::AWAY_TIME			= 10000;
const DWORD CNormal::CHANGE_ATTACK_TIME	= 3500;

////////////////////////////////
// 実装
////////////////////////////////

// コンストラクタ
CNormal::CNormal( float x, float y, float z ):
m_AnimeCnt( 0 ),
m_lpMesh( CSXfileManager::GetInst()->GetData( ENEMY_MESH_PASS ) ),
m_IsAway( false ),
m_StartAwayTime( timeGetTime() ),
m_IsAttack( false ),
m_ViewSaveTime( timeGetTime() )
{
	m_mMat.SetTrans( x, y, z );

	// 波紋設定
	m_Ripples.SetCreateHeight( m_lpMesh->GetRadius() );
	m_Ripples.SetSize(
		m_lpMesh->GetRadius(),
		m_lpMesh->GetRadius() );

	m_Tail.SetFarPos( &CVector() );
}

// デストラクタ
CNormal::~CNormal()
{
}

// 更新
void CNormal::Update()
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

			// 水面で跳ねているか
			if( m_Flg & JUMP )
			{
				// 跳ねている
				UpdateJump( SPEED );

			}else{

				if( m_IsAway )
				{
					// 逃げている
					UpdateAway();

				}else{

					// 追尾している
					UpdateDrive();
				}
			}
		}

		// 泡拡大
		UpdateBubbleScale();

		// 吹っ飛びの処理
		UpdateHitMove();

		// 事後処理
		UpdateMust();

		m_Tail.Update( m_mMat, SPEED );
	}
}

// 描画
void CNormal::Draw(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	if( !(m_Flg & BREAK) )
	{
		// 画面内判定
		if( IsCulling( &m_mMat ) )
		{
			lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );

			lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );

			// 本体
			// シェーダ描画するならフォンシェーディング
			if( is_shader )
			{
				DrawShader( lpdevice, m_lpMesh );
				
			}else{

				// そうでなければ普通に描画
				m_lpMesh->Draw( lpdevice );
			}
		}

		// しっぽ
		if( is_shader )
		{
			m_Tail.DrawShader( lpdevice );

		}else{

			m_Tail.Draw( lpdevice );
		}

		// 自分を包む泡
		DrawBubble( lpdevice );
	}

	// 泡
	m_Bubbles.DrawNoBreak( lpdevice );
}

// エフェクト描画
void CNormal::DrawEffect( const LPDIRECT3DDEVICE9 lpdevice )
{
	if( m_Flg & BREAK )
	{
		DrawBreakEnemy( lpdevice );
	}

	m_Bubbles.DrawBreak( lpdevice );

	m_lpSpray->Draw( lpdevice );
}

// 逃げる更新
void CNormal::UpdateAway()
{
	// 漂う
	// 今の位置から弾かれた方向に進む
	CVector TargetPos = m_mMat.GetPos() + m_HitDir * 10.f;

	// 追尾処理
	CVector RotAxis;
	float Degree = GetAxisAndRot( &RotAxis, TargetPos, m_mMat );

	// 回転限度
	if( Degree > ROT_SPEED )
	{
		Degree = ROT_SPEED;
	}

	// 回す
	m_mMat.RotateLocalAxis( &RotAxis, Degree );

	// 倍の速度で移動
	UpdateCollisionMove(
		&D3DXVECTOR3( 0.f, 0.f, SPEED * 2 ), 
		m_lpMesh->GetRadius() );

	// もしステージ時から出たならば
	D3DXVECTOR3 Pos = m_mMat.GetPos();
	if( Sequence::CGameMain::SEA_MOVELIMIT - 10 < abs( Pos.x ) ||
		Sequence::CGameMain::SEA_MOVELIMIT - 10 < abs( Pos.z ) )
	{
		// 原点に向かって逃げる
		D3DXVec3Normalize( &m_HitDir, &(Pos - D3DXVECTOR3( 0, 0, 0 ) ) );
	}
	
	// 一定時間経てばまた追尾する
	if( timeGetTime() - m_StartAwayTime > AWAY_TIME )
	{
		m_IsAway = false;

		// 追い回す目標を決める
		SearchTarget();
	}

	// ジャンプするかどうか
	JudgeJump();
}

// 追う更新
void CNormal::UpdateDrive()
{
	// 目標が決まっているなら
	if( m_lpTargetFish )
	{
		// 追尾処理
		CVector RotAxis;
		float Degree = GetAxisAndRot( &RotAxis, m_lpTargetFish->GetPos(), m_mMat );

		// 回転限度
		if( Degree > CBase::DEFAULT_ROTSPEED )
		{
			Degree = CBase::DEFAULT_ROTSPEED;
		}

		// 回転
		m_mMat.RotateLocalAxis( &RotAxis, Degree );

		UpdateCollisionMove( 
			&D3DXVECTOR3( 0.f, 0.f, SPEED ),
			m_lpMesh->GetRadius() );
	
	}else if( m_IsAttack == false ){	// まだ攻撃に移っていないなら

		D3DXVECTOR3 Pos = m_mMat.GetPos();
		CVector		TargetPos;

		// もしステージ時から出たならば
		if( Sequence::CGameMain::SEA_MOVELIMIT - 50 < abs( Pos.x ) ||
			Sequence::CGameMain::SEA_MOVELIMIT - 50 < abs( Pos.z ) )
		{
			// 原点に向かって逃げる
			TargetPos = D3DXVECTOR3( 0, 0, 0 );

		}else{
			
			CMatrix mPlayer = GetPlayerInfo()->GetMatrix();

			// プレイヤーの今の方向を抽出する
			CVector Dir( 0, 0, 1 );
			Dir.GetTransNormal( &mPlayer );

			// 適当に前方に足しこみ、目的地に設定
			Dir *= 30.f;
			Dir.y += 5;
			TargetPos = mPlayer.GetPos() + Dir;
		}

		// 追尾処理
		CVector RotAxis;
		float Degree = GetAxisAndRot( &RotAxis, TargetPos, m_mMat );

		// 回転限度
		if( Degree > CBase::DEFAULT_ROTSPEED )
		{
			Degree = CBase::DEFAULT_ROTSPEED;
		}

		// 回転
		m_mMat.RotateLocalAxis( &RotAxis, Degree );

		// 画面内かどうかを判断する
		if( IsCulling( &m_mMat ) == false )
		{	
			m_ViewSaveTime = timeGetTime();

		}else{

			// 一定時間画面内なら
			if( timeGetTime() - m_ViewSaveTime > CHANGE_ATTACK_TIME )
			{
				m_IsAttack = true;
			}
		}

		// 倍速で移動
		UpdateCollisionMove(
			&D3DXVECTOR3( 0.f, 0.f, SPEED * 2 ),
			m_lpMesh->GetRadius() );

	}else{

		// 攻撃中ならプレイヤーに向かう
		UpdateRot();

		// 画面内から出たら
		if( IsCulling( &m_mMat ) == false )
		{	
			m_ViewSaveTime = timeGetTime();
			m_IsAttack = false;
		}

		UpdateCollisionMove(
			&D3DXVECTOR3( 0.f, 0.f, SPEED ), 
			m_lpMesh->GetRadius() );
	}

	// 水面に出てたらジャンプへ
	JudgeJump();
}

// プレイヤーに対する行動
void CNormal::Action(Player::CDolphin &object)
{
	// 方向を決定する
	m_HitDir = m_mMat.GetPos() - object.GetPos();
	
	// 正規化
	m_HitDir.GetNormalVector();

	// 吹っ飛び力をリセット
	m_HitPower = 1.f;

	// ヒット＆アウェイのアウェイをする
	m_IsAway = true;
	m_IsAttack = false;

	// 時間保存
	m_StartAwayTime = timeGetTime();
}

// 襲うオブジェクトに対しての行動
void CNormal::Action(CDamageObjectBase &object)
{
	// 逃げる
	m_IsAway = true;
	m_IsAttack = false;
	m_lpTargetFish = NULL;
}