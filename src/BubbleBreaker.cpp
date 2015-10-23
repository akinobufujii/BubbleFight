/////////////////////////////////////
// インクルード
/////////////////////////////////////
#include "RippleManager.h"
#include "BubbleManager.h"
#include "ShotManager.h"
#include "BubbleManager.h"
#include "EnemyBase.h"

#include "FishManager.h"
#include "RippleManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "GameMain.h"

#include "WaterSpray.h"

#include "RippleManager.h"
#include "FinTail.h"
#include "StrongDolphin.h"

#include "BubbleBreaker.h"

/////////////////////////////////////
// 名前空間省略
/////////////////////////////////////
using namespace Enemy;

/////////////////////////////////////
// 静的メンバ初期化
/////////////////////////////////////
const float		CBubbleBreaker::SPEED				= 0.5f;
const ULONG		CBubbleBreaker::SCORE				= 250;
const LPSTR		CBubbleBreaker::MESHPASS_ENEMY		= "VacuumBubble.x";
const DWORD		CBubbleBreaker::MOVE_CHANGETIME		= 5000;
const float		CBubbleBreaker::DIRCHANGE_ANIMETIME	= 0.01f;
const BYTE		CBubbleBreaker::ROT_SPEED			= 5;
const USHORT	CBubbleBreaker::AWAY_TIME			= 10000;
const USHORT	CBubbleBreaker::CHANGE_ATTACK_TIME	= 3500;

/////////////////////////////////////
// 実装
/////////////////////////////////////

// コンストラクタ
CBubbleBreaker::CBubbleBreaker( float x, float y, float z ):
m_IsDirChange( false ),
m_NowAnimetionCnt( 0.f ),
m_NowSpeed( SPEED ),
m_lpTargetEnemy( NULL ),
m_IsAway( false ),
m_IsAttack( false ),
m_SaveTime( timeGetTime() ),
m_SaveDriveTime( timeGetTime() ) 
{
	// 初期位置設定
	m_mMat.SetTrans( x, y, z );

	// メッシュ獲得
	m_lpMesh = CSXfileManager::GetInst()->GetData( MESHPASS_ENEMY );

	// 尻尾位置設定
	m_Tail.SetFarPos( &D3DXVECTOR3( 0, 0, -1.2f ) );

	m_Tail.SetRotMax( 10 );

	m_Ripples.SetSize(
		m_lpMesh->GetRadius(),
		m_lpMesh->GetRadius() + 1.2f );

	m_Tail.SetFarPos( &CVector() );
}

// デストラクタ
CBubbleBreaker::~CBubbleBreaker()
{
}

// 更新
void CBubbleBreaker::Update()
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

			if( m_Flg & JUMP )
			{
				// ジャンプ更新
				UpdateJump( SPEED );
			
			}else{

				if( m_IsAway )
				{
					// 逃げる
					UpdateAway();

				}else{

					// 移動
					UpdateMove();
				}
			}
		}

		// 泡拡大
		UpdateBubbleScale();

		// 当たった時の更新
		UpdateHitMove();

		// 事後処理
		UpdateMust();

		// 尻尾
		m_Tail.Update( m_mMat, m_NowSpeed );
	}
}

// 移動更新
void CBubbleBreaker::UpdateMove()
{
	if( m_lpTargetEnemy == NULL )
	{
		// プレイヤーに向かって攻撃を仕掛ける
		// 攻撃予備動作中
		if( m_IsAttack == false )
		{
			D3DXVECTOR3 Pos = m_mMat.GetPos();
			CVector		TargetPos;

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
				m_SaveDriveTime = timeGetTime();

			}else{

				// 一定時間画面内なら
				if( timeGetTime() - m_SaveDriveTime > CHANGE_ATTACK_TIME )
				{
					m_IsAttack = true;
				}
			}

			// 倍速で移動
			UpdateCollisionMove( &D3DXVECTOR3( 0.f, 0.f, SPEED * 2 ), m_lpMesh->GetRadius() );

		}else{

			// 攻撃中ならプレイヤーに向かう
			UpdateRot();

			// 画面内から出たら
			if( IsCulling( &m_mMat ) == false )
			{	
				m_SaveDriveTime = timeGetTime();
				m_IsAttack = false;
			}

			UpdateCollisionMove( &D3DXVECTOR3( 0.f, 0.f, SPEED ), m_lpMesh->GetRadius() );
		}

	}else{

		// もう手遅れなら負わない
		if( m_lpTargetEnemy->GetFlg() & (BREAK | ERASE) )
		{
			m_lpTargetEnemy = NULL;
			return;
		}

		// 追尾処理
		CVector RotAxis;
		float Degree = GetAxisAndRot( &RotAxis, m_lpTargetEnemy->GetPos(), m_mMat );

		// 回転限度
		if( Degree > CBase::DEFAULT_ROTSPEED )
		{
			Degree = CBase::DEFAULT_ROTSPEED;
		}

		m_mMat.RotateLocalAxis( &RotAxis, Degree );

		UpdateCollisionMove( &D3DXVECTOR3( 0, 0, SPEED ), m_lpMesh->GetRadius() );
	}

	// ジャンプするかどうか
	JudgeJump();
}

// 逃げる更新
void CBubbleBreaker::UpdateAway()
{
	// 今の位置から弾かれた方向に進む
	CVector TargetPos = m_mMat.GetPos() + m_HitDir * 10.f;

	// 追尾処理
	CVector RotAxis;
	float Degree = GetAxisAndRot( &RotAxis, TargetPos, m_mMat );

	// 回転限度
	if( Degree > CBase::DEFAULT_ROTSPEED )
	{
		Degree = CBase::DEFAULT_ROTSPEED;
	}

	// 角度補正して回転
	if( Degree > ROT_SPEED )
	{
		Degree = ROT_SPEED;
	}

	// 回す
	m_mMat.RotateLocalAxis( &RotAxis, Degree );

	// 倍の速度で移動
	UpdateCollisionMove( &D3DXVECTOR3( 0, 0, SPEED * 2 ), m_lpMesh->GetRadius() );
	
	// 一定時間経てばまた追尾する
	if( timeGetTime() - m_AwayStartTime > AWAY_TIME )
	{
		m_IsAway = false;
	}

	// ジャンプするかどうか
	JudgeJump();
}

// 描画
void CBubbleBreaker::Draw(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	if( !(m_Flg & BREAK) )
	{
		if( IsCulling( &m_mMat ) )
		{
			// レンダリング設定
			lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
			lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );
			lpdevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

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
}

// エフェクト描画
void CBubbleBreaker::DrawEffect( const LPDIRECT3DDEVICE9 lpdevice )
{
	if( m_Flg & BREAK )
	{
		DrawBreakEnemy( lpdevice );
	}

	m_lpSpray->Draw( lpdevice );
}

// 目的地を設定する
void CBubbleBreaker::SetDirEnemy( Enemy::CBase* enemy )
{
	// もし自分自身か手遅れなら何もしない
	if( this == enemy || (enemy->GetFlg() & (BREAK | ERASE)) )
	{
		return;
	}

	// 何も目的地になっていなければ
	if( m_lpTargetEnemy == NULL )
	{
		// 無条件で目標
		m_lpTargetEnemy = enemy;
	
	}else{

		// 距離を計算する
		D3DXVECTOR3	Pos	= m_mMat.GetPos();
		CVector		vOld= m_lpTargetEnemy->GetPos() - Pos;
		CVector		vNew= enemy->GetPos() - Pos;

		// 新しい情報の方が近ければそちらを設定する
		if( vNew.GetLength() < vOld.GetLength() )
		{
			m_lpTargetEnemy = enemy;
		}
	}
}

// 敵に対する行動
void CBubbleBreaker::Action(Enemy::CBase &object)
{
	// 少しでも包まれていれば
	if( object.GetBubbleScale() > 0.f )
	{
		// 吸引？
		object.VacuumBubble( 0.01f );
		
	}else{

		// そうでなければ標準の動き
		CBase::Action( object );

		// ターゲットも設定しない
		m_lpTargetEnemy = NULL;
	}
}

// プレイヤーに対する行動
void CBubbleBreaker::Action(Player::CDolphin &object)
{
	// 当てれば逃げる
	// 方向を決める
	m_HitDir =  m_mMat.GetPos() - object.GetPos();

	m_HitDir.GetNormalVector();

	// 吹っ飛ぶ力を決める
	m_HitPower = 1.f;

	// 逃げる
	m_IsAway = true;
	m_IsAttack = false;

	// 時間保存
	m_AwayStartTime = timeGetTime();
}