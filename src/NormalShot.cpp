//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "BubbleManager.h"

#include "ShotBase.h"
#include "ShotManager.h"

#include "BossBase.h"

#include "RippleManager.h"
#include "FinTail.h"
#include "StrongDolphin.h"

#include "LimitWalls.h"
#include "GameMain.h"

#include "NormalShot.h"

//////////////////////////////////////////////////
// 名前空間の省略
//////////////////////////////////////////////////
using namespace Shot;

//////////////////////////////////////////////////
// 静的メンバ実体化
//////////////////////////////////////////////////
const float		CNormal::POWER_MAX			= 5.f;
const float		CNormal::POWER_MIN			= 0.5f;
const float		CNormal::SPEED_MAX			= 1.5f;
const float		CNormal::BUBBLEWRAP_MAX		= 10.f;
const float		CNormal::POWER_SPEED		= 0.5f;
const BYTE		CNormal::POINT_NUM			= 20;
const LPSTR		CNormal::MESH_PASS			= "BigShot.x";
const LPSTR		CNormal::BUBBLE_MESH_PASS	= "Bubble.x";
const USHORT	CNormal::FINHIT_ERASE_TIME	= ERASE_TIME * 2;

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CNormal::CNormal():
m_CreateBubbleNum( 0 ),
m_Power( POWER_MIN ),
m_Speed( SPEED_MAX / POWER_MAX * (POWER_MAX + 1 - POWER_MIN) ),
m_lpMesh( CSXfileManager::GetInst()->GetData( MESH_PASS ) ),
m_lpBubbleMesh( CSXfileManager::GetInst()->GetData( BUBBLE_MESH_PASS ) ),
m_NowPower( 0 ),
m_StartTime( timeGetTime() )	// 呼ばれた時間を格納
{
	m_mMat.ScaleLocal( m_Power );

	m_Points.SetScaleMax( 2.f );
}

CNormal::CNormal( const D3DXMATRIX* mat ):
CBase( mat ), 
m_CreateBubbleNum( 0 ),
m_Power( POWER_MIN ),
m_Speed( SPEED_MAX / POWER_MAX * (POWER_MAX + 1 - POWER_MIN) ),
m_lpMesh( CSXfileManager::GetInst()->GetData( MESH_PASS ) ),
m_lpBubbleMesh( CSXfileManager::GetInst()->GetData( BUBBLE_MESH_PASS ) ),
m_NowPower( 0 ), 
m_StartTime( timeGetTime() )	// 呼ばれた時間を格納
{
	m_mMat.ScaleLocal( m_Power );

	m_Points.SetScaleMax( 2.f );
}

// 更新
void CNormal::Update()
{
	if( m_Flg & FLG_BREAK )
	{
		// 爆発更新
		UpdateBreak();
	
	}else if( m_Flg & FLG_ALIVE ){
		
		// 泡につつまれていなければ
		if( !(m_Flg & FLG_BUBBLE_WRAP) )
		{
			// 移動
			UpdateMove();
		
		}else if( m_Flg & FLG_FIN_HIT ){

			// 弾かれた移動
			UpdateHitByFin();
		}
	}

	// 泡も消えてたら削除
	if( !(m_Flg & FLG_ALIVE) && m_Points.IsErase() )
	{
		m_Flg |= FLG_ERASE;
	}

	// 泡
	m_Bubbles.Update();

	if( (m_NowPower += POWER_SPEED) > m_Power )
	{
		m_NowPower = m_Power;
	}

	// ポイントスプライト
	m_Points.Update();
}

// 描画
void CNormal::Draw( const LPDIRECT3DDEVICE9 lpdevice )
{
	if( IsCulling( &m_mMat ) )
	{
		if( m_Flg & FLG_BREAK ){

			CCamera Cam = *Sequence::CGameMain::GetCamera();

			CMatrix Rot;
			Cam.GetBillboard( &Rot, &m_mMat );

			CMatrix Trans( m_mMat.GetPos() );
		
		}else if( m_Flg & FLG_ALIVE ){

			// 弾描画
			lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );

			m_lpMesh->Draw( lpdevice );

			// 泡につつまれていればそれも描画
			if( m_Flg & FLG_BUBBLE_WRAP )
			{
				CMatrix mScale = m_mMat;

				// 大きくする
				mScale.ScaleLocal( m_NowPower );

				lpdevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
				lpdevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
				lpdevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
				lpdevice->SetTransform( D3DTS_WORLD, &mScale );

				m_lpBubbleMesh->Draw( lpdevice );

				lpdevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
			}
		} 
	}

	// 泡
	m_Bubbles.DrawNoBreak( lpdevice );

	lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpdevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	m_Points.Draw( lpdevice );

	lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}

// 移動更新
void CNormal::UpdateMove()
{	
	// 移動
	m_mMat.MoveLocal( 0, 0, SPEED_MAX );

	// 泡追加(約フレームに１回)
	if( m_StartTime + m_CreateBubbleNum * CREATE_BUBBLE_TIME < timeGetTime() )
	{
		m_CreateBubbleNum++;

		// 拡大成分を取り除く
		CMatrix mMat( m_mMat.GetPos(  ) );

		m_Bubbles.Add( &mMat, 1 );
	}

	// 一定時間で消滅
	if( m_StartTime + ERASE_TIME < timeGetTime() )
	{
		m_Board.SetVertex( 2 );

		float X = static_cast<float>(BREAK_GRAPH_SIZE) / BREAK_GRAPH_WIDTH;

		m_Board.SetTexturePos(
			&D3DXVECTOR2( m_AnimeCnt * X,		0 ),
			&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	0 ),
			&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	1.f ),
			&D3DXVECTOR2( m_AnimeCnt * X,		1.f ) );


		m_Flg &= ~FLG_ALIVE;

		m_Points.SetNum( POINT_NUM );
		m_Points.SetTexture( CSTexManager::GetInst()->GetData( SPRITE_TEX_PASS )->GetTexture() );
		m_Points.SetScaleMax( 2.f );
		m_Points.SetMove( D3DXVECTOR3( 1.f, 1.f, 1.f ) );
		m_Points.SetPos( m_mMat.GetPos(  ) );
		m_Points.SetEraseTime( 3000 );
	}
}

// フィンにあたってからの更新
void CNormal::UpdateHitByFin()
{
	// 包まれていてフィンに当たったらとばす
	m_mMat.MoveLocal( m_vDirection.x, m_vDirection.y, m_vDirection.z );

	// 一定時間で消滅
	if( (m_StartTime + FINHIT_ERASE_TIME < timeGetTime()) )
	{
		m_Board.SetVertex( 2 );

		float X = static_cast<float>(BREAK_GRAPH_SIZE) / BREAK_GRAPH_WIDTH;

		m_Board.SetTexturePos(
			&D3DXVECTOR2( m_AnimeCnt * X,		0 ),
			&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	0 ),
			&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	1.f ),
			&D3DXVECTOR2( m_AnimeCnt * X,		1.f ) );


		m_Flg &= ~FLG_ALIVE;

		m_Points.SetNum( POINT_NUM );
		m_Points.SetTexture( *CSTexManager::GetInst()->GetData( SPRITE_TEX_PASS ) );
		m_Points.SetScaleMax( 2.f );
		m_Points.SetMove( D3DXVECTOR3( 1.f, 1.f, 1.f ) );
		m_Points.SetPos( m_mMat.GetPos() );
		m_Points.SetEraseTime( 3000 );
	}
}

// 情報を送りつける
void CNormal::Dispatch(IDoubleDispatch *object)
{
	object->Action( *this );
}

// 敵に対する行動
void CNormal::Action( Enemy::CBase& object )
{
	m_Flg &= ~FLG_ALIVE;

	m_Points.SetNum( POINT_NUM );
	m_Points.SetTexture( CSTexManager::GetInst()->GetData( SPRITE_TEX_PASS )->GetTexture() );
	m_Points.SetScaleMax( 2.f );
	m_Points.SetMove( D3DXVECTOR3( 1.f, 1.f, 1.f ) );
	m_Points.SetPos( m_mMat.GetPos() );
	m_Points.SetEraseTime( 3000 );
	//m_Flg |= FLG_ERASE;
}

// ボスに対する行動
void CNormal::Action(Boss::CBase &object)
{
	// 何かしようと思ったが、貫通させるために何もしない
	if( (object.GetFlg() & (Boss::CBase::FLG_ALIVE) ) && 
		(m_Flg & FLG_BUBBLE_WRAP) )
	{
		//m_Flg &= ~FLG_ALIVE;
	}
}

// プレイヤーに対する行動
void CNormal::Action(Player::CDolphin &object)
{
	// 自分が破裂も泡につつまれてもなければ
	if( !(m_Flg & (FLG_BREAK | FLG_BUBBLE_WRAP)) )
	{
		// 破裂準備
		m_Points.SetNum( POINT_NUM );
		m_Points.SetTexture( *CSTexManager::GetInst()->GetData( "Sprite.jpg" ) );
		m_Points.SetScaleMax( 2.f );
		m_Points.SetMove( D3DXVECTOR3( 1.f, 1.f, 1.f ) );
		m_Points.SetPos( m_mMat.GetPos() );
		m_Points.SetEraseTime( 3000 );

		m_Flg &= ~FLG_ALIVE;
		m_Flg |= FLG_BREAK;
	}

	// 自分が泡につつまれていて、まだ攻撃されていなければ
	if( (m_Flg & FLG_BUBBLE_WRAP) && 
		(object.GetFlg() & Player::CDolphin::FLG_ATTACK_OK) &&
		!(m_Flg & FLG_FIN_HIT) )
	{
		// 拡大分を考慮してレイ判定
		CMatrix mScale = m_mMat;

		mScale.ScaleLocal( m_Power );

		float Result = IntersectByMesh( &object.GetMatrix(), &object.GetFinDir(), &mScale, *m_lpMesh->GetMeshData() );
		
		if( Result > 0 )
		{
			// フィンに当たった
			m_Flg |= FLG_FIN_HIT;

			// 飛んでいく方向を決定する
			CVector ScreenVec = Sequence::CGameMain::GetCamera()->GetScreenVector();

			// 逆行列を作る
			CMatrix mInv = m_mMat;
			mInv.GetInverse();
			
			ScreenVec.GetTransNormal( &mInv );
			m_vDirection = ScreenVec;

			// 時間を記憶する
			m_StartTime = timeGetTime();
		}
	}
}

// 弾に対する行動
void CNormal::Action(Shot::CBase &object)
{
	// 泡につつまれた
	m_Flg	|= FLG_BUBBLE_WRAP;

	// 包む泡の大きさ制限
	if( (m_Power += object.GetPower() * 2) > BUBBLEWRAP_MAX )
	{
		m_Power = BUBBLEWRAP_MAX;
	}
}

// デストラクタ
CNormal::~CNormal()
{
}