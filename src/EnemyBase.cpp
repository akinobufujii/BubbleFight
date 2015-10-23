//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "BubbleManager.h"

#include "Fintail.h"
#include "Screw.h"

#include "RippleManager.h"
#include "ShotManager.h"
#include "ShotBase.h"
#include "StrongDolphin.h"

#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "GameMain.h"

#include "DamageObjectBase.h"

#include "WaterSpray.h"

#include "PointSpriteManager.h"
#include "EnemyBase.h"

//////////////////////////////////////////////////
// 名前空間の省略
//////////////////////////////////////////////////
using namespace Enemy;

//////////////////////////////////////////////////
// 静的メンバ実体化
//////////////////////////////////////////////////
Player::CDolphin*	CBase::m_lpPlayer	= NULL;
CStoneManager*		CBase::m_lpStone	= NULL;
CFishManager*		CBase::m_lpFishes	= NULL;

const float CBase::PARTICLE_POWER		= 0.5f;
const float CBase::FLOAT_POWER_LINE		= 0.2f;
const float CBase::FLOAT_SPEED			= 0.01f;
const float CBase::SCALE_UP_SPEED		= 0.01f;
const float CBase::DEFAULT_FLAOT_POWER	= 0.065f;
const float CBase::DEFAULT_HITPOWER_OFF	= 0.2f;
const float	CBase::DEFAULT_ROTSPEED		= 10.f;
const BYTE	CBase::ERASE_LENGTH			= 25;
const BYTE	CBase::BUBBLE_SCALE_RATE	= 30;

const BYTE	CBase::ERASE		= 0x01;
const BYTE	CBase::BREAK		= 0x02;
const BYTE	CBase::BUBBLE_HIT	= 0x04;
const BYTE	CBase::SEARCH		= 0x08;
const BYTE	CBase::ATTACK		= 0x10;
const BYTE	CBase::BUBBLE_WRAP	= 0x20;
const BYTE	CBase::JUMP			= 0x40;

const int	CBase::BURST_WIDTH				= 256;
const int	CBase::BURST_HEIGHT				= 256;
const int	CBase::BURST_ANIME_SIZE			= 32;
const int	CBase::BURST_ANIME_WIDTH_NUM	= 8;
const int	CBase::BURST_ANIME_HEIGHT_NUM	= 8;
const int	CBase::BURST_ANIME_NUM			= CBase::BURST_ANIME_WIDTH_NUM * (CBase::BURST_ANIME_HEIGHT_NUM - 1);

const LPSTR	CBase::BUBBLE_MESH_PASS	= "Bubble.x";

////////////////////////////////
// 実装
////////////////////////////////

// コンストラクタ
CBase::CBase():
m_Flg( 0 ),
m_FloatPower( 0.f ),
m_AnimeCnt( 0 ),
m_NowFloatSpeed( 0.f ),
m_NowScale( 0.f ),
m_HitPower( 0.f ),
m_lpTargetFish( NULL ),
m_JumpSpeed( 0.f ),
m_lpSpray( NEW CWaterSpray() )
{
	// 波紋の設定
	m_Ripples.SetDefaultTexture();
}

// デストラクタ
CBase::~CBase()
{
	SAFE_DELETE( m_lpSpray );
}

// プレイヤーに向かって回転する
void CBase::UpdateRot( float rotmax )
{
	// 回転軸と角度を求める
	D3DXVECTOR3 RotAxis;
	float Ang = GetAxisAndRot( &RotAxis, m_lpPlayer->GetPos(), m_mMat );

	// 回転角度最大値計算
	if( Ang > rotmax )
	{
		Ang = rotmax;
	}

	// 指定軸で回転
	m_mMat.RotateLocalAxis( &RotAxis, Ang );
}

// 破壊された敵を描画
void CBase::DrawBreakEnemy( const LPDIRECT3DDEVICE9 lpdevice )
{
	CCamera Camera = *Sequence::CGameMain::GetCamera();

	CMatrix Mat, mTrans;

	lpdevice->SetFVF( CBoard::FVF_VERTEX );

	Camera.GetBillboard( &Mat, &m_mMat );

	mTrans.SetTrans( &m_mMat.GetPos(  ) );

	Mat = Mat * mTrans;
	lpdevice->SetTransform( D3DTS_WORLD, &Mat );

	// ライティングを切る
	lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpdevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	lpdevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	lpdevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	
	lpdevice->SetFVF( CBoard::FVF_VERTEX );

	// パーティクル描画
	m_Particle.Draw( lpdevice );

	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	lpdevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	// 泡描画
	m_Bubbles.DrawNoBreak( lpdevice );
	m_Bubbles.DrawBreak( lpdevice );
}

// プログラマブルシェーダで描画
void CBase::DrawShader(const LPDIRECT3DDEVICE9 lpdevice, CXfile* mesh)
{
	CEffect* lpSharder = CSShaderManager::GetInst()->GetData( "Phong.fx" );

	// 固定パイプラインの情報を獲得する
	CMatrix	mView, mProj;
	lpdevice->GetTransform( D3DTS_PROJECTION, &mProj );
	lpdevice->GetTransform( D3DTS_VIEW, &mView );

	// 座標変換
	lpSharder->SetValue( "g_WVP",
		&(m_mMat * mView * mProj),
		sizeof( D3DXMATRIX ) );

	// ライト位置計算
	lpSharder->SetValue( "g_LightDir",
		&GetLightPos( lpdevice, m_mMat ),
		sizeof( D3DXVECTOR4 ) );

	// カメラ位置計算
	lpSharder->SetValue( "g_EyePos",
		&GetViewPos( lpdevice, m_mMat ),
		sizeof( D3DXVECTOR4 ) );

	// 描画
	lpSharder->Begin();

	mesh->DrawShader( lpSharder, "g_Diffuse", "g_Tex" );

	lpSharder->End();
}

// 弾に対しての行動
void CBase::Action( Shot::CBase& object )
{
	//m_Flg |= BREAK;	// 消す

	// 跳ね返された弾なら
	if( object.GetFlg() & Shot::CBase::FLG_FIN_HIT )
	{
		m_Flg |= BREAK;	// 消す

		m_Particle.SetNum( 100 );
		m_Particle.SetPos( m_mMat.GetPos(  ) );
		m_Particle.SetScaleMax( 2.f );
		m_Particle.SetEraseTime( 1000 );
		m_Particle.SetMove( D3DXVECTOR3( PARTICLE_POWER, PARTICLE_POWER, PARTICLE_POWER ) );
		m_Particle.SetTexture( CSTexManager::GetInst()->GetData( "Sprite.jpg" )->GetTexture() );

	}else{

		m_FloatPower += object.GetPower() * DEFAULT_FLAOT_POWER;	// 浮力加算

		// 一定以上食らったら
		if( m_FloatPower >= FLOAT_POWER_LINE )
		{
			m_Flg |= BUBBLE_WRAP;
		}
	}
}

// 敵同士の行動
void CBase::Action(Enemy::CBase &object)
{
	// 吹っ飛び方向決定
	m_HitDir = m_mMat.GetPos() - object.GetPos();

	// 正規化
	m_HitDir.GetNormalVector();

	// 吹っ飛び力リセット(一定)
	m_HitPower = 1.f;

	if( m_FloatPower < FLOAT_POWER_LINE )
	{
		m_Flg &= ~BUBBLE_WRAP;
	}
}

// 座標を返す
D3DXVECTOR3 CBase::GetPos()
{
	return m_mMat.GetPos();
}

// 浮力更新
void CBase::UpdateFloatPower()
{
	// 浮力
	if( (m_NowFloatSpeed += FLOAT_SPEED) > m_FloatPower )
	{
		m_NowFloatSpeed = m_FloatPower;
	}

	CMatrix Power( 0.f, m_NowFloatSpeed, 0.f );

	m_mMat = m_mMat * Power;

	// 消すか判断
	D3DXVECTOR3 Pos( 0, 0, 0 ), EPos( 0, 0, 0 );
	D3DXVec3TransformCoord( &Pos, &Pos, &GetPlayerInfo()->GetMatrix() );
	D3DXVec3TransformCoord( &EPos, &EPos, &m_mMat );

	if( abs( Pos.y - EPos.y ) > ERASE_LENGTH			||
		EPos.y > Sequence::CGameMain::SURFACESEA_HEIGHT )
	{
		m_Particle.SetNum( 100 );
		m_Particle.SetPos( m_mMat.GetPos() );
		m_Particle.SetScaleMax( 2.f );
		m_Particle.SetEraseTime( 1000 );
		m_Particle.SetMove( D3DXVECTOR3( PARTICLE_POWER, PARTICLE_POWER, PARTICLE_POWER ) );

		CTexture* lpTex = CSTexManager::GetInst()->GetData( "Sprite.jpg" );

		m_Particle.SetTexture( *lpTex );
		
		m_Flg |= BREAK;
	}

	//// 海面に出たら消える
	//if( EPos.y > Sequence::CGameMain::SURFACESEA_HEIGHT )
	//{
	//	m_Flg |= BREAK;

	//	CTexture* lpTex = CSTexManager::GetInst()->GetData( "Sprite.jpg" );
	//	m_Particle.SetTexture( lpTex->GetTexture() );
	//}
}

// 泡(敵を包む泡)描画
void CBase::DrawBubble( const LPDIRECT3DDEVICE9 lpdevice )
{
	if( m_FloatPower > 0.f )
	{
		lpdevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
		lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );
	
		lpdevice->SetTransform( D3DTS_WORLD, &m_mBalloon );
		
		CSXfileManager::GetInst()->GetData( BUBBLE_MESH_PASS )->Draw( lpdevice );
	
		lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	}
}

// 泡の拡大更新
void CBase::UpdateBubbleScale()
{
	CMatrix mScale;

	// 徐々に拡大していく
	if( (m_NowScale += SCALE_UP_SPEED) > m_FloatPower )
	{
		m_NowScale = m_FloatPower;
	}

	mScale.SetScale( 1.f + m_NowScale * BUBBLE_SCALE_RATE );

	m_mBalloon = mScale * m_mMat;
}

// 泡を吸い取る(BubbleBreakerだけが使う)
void CBase::VacuumBubble( float rate )
{
	CMatrix mScale;

	if( (m_FloatPower -= rate) < 0 )
	{
		m_FloatPower = 0;
	}

	if( (m_FloatPower -= SCALE_UP_SPEED) < 0 )
	{
		m_FloatPower = 0;
	}

	mScale.SetScale( 1.f + m_NowScale * BUBBLE_SCALE_RATE );

	m_mBalloon = mScale * m_mMat;
}

// 吹っ飛ぶ更新
void CBase::UpdateHitMove()
{
	if( m_HitPower > 0 )
	{
		CVector Temp = m_HitDir * m_HitPower;

		UpdateCollisionMove( &Temp, 3  );
		//m_mMat.MoveLocal( Temp.x, Temp.y, Temp.z );

		m_HitPower -= DEFAULT_HITPOWER_OFF;
	}
}

// 継承先が必ず呼び出してほしい処理
void CBase::UpdateMust()
{
	// 海面以上なら
	//if( m_mMat._42 > Sequence::CGameMain::SURFACESEA_HEIGHT )
	//{
	//	m_mMat._42 = Sequence::CGameMain::SURFACESEA_HEIGHT;
	//
	//}else 

	if( m_mMat._42 < Sequence::CGameMain::BOTTOMSEA_HEIGHT ){
		
		// 海底以下なら
		m_mMat._42 = Sequence::CGameMain::BOTTOMSEA_HEIGHT;
	}

	// 上方向に修正する(姿勢を正す)
	// どれくらい傾いているか計算する
	CVector Vec( 0, 1, 0 );

	// 逆行列作成
	CMatrix mInv = m_mMat;
	mInv.GetInverse();

	// 方向を求める
	Vec.GetTransNormal( &mInv );

	// 傾きのみのベクトルにする
	Vec.z = 0;
	Vec.GetNormalVector();

	// 角度抽出
	float Degree = Vec.GetDotDegree( &CVector( 0, 1, 0 ) );

	// 回転
	if( abs( Degree ) > 1 )
	{
		if( Vec.x < 0 )
		{
			m_mMat.RotateLocalZ( 1 );

		}else{

			m_mMat.RotateLocalZ( -1 );
		}
	}

	// 波紋の更新
	m_Ripples.Update( m_mMat );

	// 水しぶき
	m_lpSpray->Update();
}

// 衝突判定行いながら移動
bool CBase::UpdateCollisionMove( const D3DXVECTOR3* move, const float limit )
{
	// 長さと方向を抽出
	float Length = D3DXVec3Length( move );

	CVector Direction;
	D3DXVec3Normalize( &Direction, move );

	Direction.GetTransNormal( &m_mMat );

	bool IsMove = true;

	CMatrix mScale;

	DWORD PolyNo;

	// 岩との衝突判定
	BOOST_FOREACH( CStoneManager::STONEDATA* Stone, *m_lpStone->GetList() )
	{
		mScale.SetScale( Stone->Scale );

		// 判定
		float Distance = IntersectByMesh( 
			&m_mMat,
			&Direction,
			&( mScale * CMatrix( Stone->Pos ) ),
			*Stone->lpMesh->GetMeshData(),
			&PolyNo);

		// 応答
		if( Distance > 0 && Distance - Length < 2 )
		{
			// 今の前方方向を調べる
			CVector	NowFront( 0, 0, 1 );

			NowFront.GetTransNormal( &m_mMat );

			// 滑りベクトル計算
			D3DXVECTOR3 SlipVec = GetSlipVec( NowFront, Stone->lpMesh->GetNormal( PolyNo ) );

			// 滑りベクトルでもう一度判定
			DWORD SlipPoly;

			float SlipDistance = IntersectByMesh( 
				&m_mMat,
				&SlipVec,
				&( mScale * CMatrix( Stone->Pos ) ),
				*Stone->lpMesh->GetMeshData(),
				&SlipPoly );

			D3DXVECTOR3 TempVec;

			if( SlipDistance > 0 && SlipDistance - Length < 2 )
			{
				// 滑りベクトルで当たったなら止める
				TempVec = D3DXVECTOR3( 0, 0, 0 );
		
			}else{
				
				// そうでなければ最初のベクトルを使う
				TempVec = SlipVec;
			}
			
			// 移動
			m_mMat._41 += TempVec.x;
			m_mMat._42 += TempVec.y;
			m_mMat._43 += TempVec.z;

			IsMove = false;
			break;
		}
	}

	// 動けるなら動く
	if( IsMove )
	{
		// 包まれている泡の分、減速する
		float Temp = (FLOAT_POWER_LINE - m_FloatPower) / FLOAT_POWER_LINE;
		m_mMat.MoveLocal( &(*move * max( Temp, 0.5f )) );
	}

	// 動いたか動いてないかを返す
	return IsMove;
}

// 追いかける目標を決定する
void CBase::SearchTarget()
{
	CFishManager::DAMAGEFISHLIST* lpList = m_lpFishes->GetDamageList();
	CVector Pos = m_mMat.GetPos();

	BOOST_FOREACH( CDamageObjectBase* Fish, *lpList )
	{
		// 目標が入っていないなら
		if( m_lpTargetFish == NULL )
		{
			// とりあえず目標に
			m_lpTargetFish = Fish;

		}else{

			// 差を求める
			CVector	Old = Pos - m_lpTargetFish->GetPos();
			CVector	New = Pos - Fish->GetPos();

			// 長さが短ければ新たな目標に
			if( New.GetLength() < Old.GetLength() )
			{
				m_lpTargetFish = Fish;
			}
		}
	}
}

// ジャンプ更新
void CBase::UpdateJump( float speed )
{
	// 進む
	m_mMat.MoveLocal( 0, 0, speed + (m_JumpSpeed += 0.01f) );

	// 絶対軸のY方向に落とす
	CMatrix mTrans( 0, -0.01f, 0 );
	m_mMat *= mTrans;

	// 上方向に修正する(姿勢を正す)
	// どれくらい傾いているか計算する
	CVector Vec( 0, 1, 0 );

	// 逆行列作成
	CMatrix mInv = m_mMat;
	mInv.GetInverse();

	// 方向を求める
	Vec.GetTransNormal( &mInv );

	// 傾きのみのベクトルにする
	Vec.z = 0;
	Vec.GetNormalVector();

	// 角度抽出
	float Degree = Vec.GetDotDegree( &CVector( 0, 1, 0 ) );

	// 回転
	if( abs( Degree ) > 1 )
	{
		if( Vec.x < 0 )
		{
			m_mMat.RotateLocalZ( 5 );

		}else{

			m_mMat.RotateLocalZ( -5 );
		}
	}

	// X軸を曲げる
	m_mMat.RotateLocalX( 2 );

	// 水面より下になればフラグをたたむ
	if( m_mMat.GetPos().y < Sequence::CGameMain::SURFACESEA_HEIGHT )
	{
		m_JumpSpeed = 0.f;

		// 水しぶき追加
		m_lpSpray->Create( m_mMat.GetPos() );

		m_Flg &= ~JUMP;
	}
}

// ジャンプ判断
void CBase::JudgeJump()
{
	if( Sequence::CGameMain::SURFACESEA_HEIGHT < m_mMat.GetPos().y )
	{
		m_lpSpray->Create( m_mMat.GetPos() );
		m_Flg |= JUMP;
	}
}