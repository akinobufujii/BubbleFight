//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "Bubble.h"
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

#include "BubbleRing.h"

//////////////////////////////////////////////////
// 名前空間の省略
//////////////////////////////////////////////////
using namespace Shot;

//////////////////////////////////////////////////
// 静的メンバ実体化
//////////////////////////////////////////////////
const float		CBubbleRing::POWER_MAX			= 5.f;
const float		CBubbleRing::POWER_MIN			= 0.5f;
const float		CBubbleRing::SPEED_MAX			= 1.5f;
const float		CBubbleRing::BIG_SPEED			= 0.2f;
const BYTE		CBubbleRing::POINT_NUM			= 20;
const USHORT	CBubbleRing::BUBBLE_ERASE_TIME	= 1000;
const LPSTR		CBubbleRing::MESH_PASS			= "BubbleRing.x";

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CBubbleRing::CBubbleRing(): 
m_CreateBubbleNum( 0 ),
m_Power( BIG_SPEED ),
m_Speed( 1.f ),
m_StartTime( timeGetTime() )
{
	// メッシュデータ獲得
	m_lpMesh = CSXfileManager::GetInst()->GetData( MESH_PASS );

	m_Points.SetScaleMax( 4.f );
}

CBubbleRing::CBubbleRing( const D3DXMATRIX* mat, const float power ):
CBase( mat ), 
m_CreateBubbleNum( 0 ),
m_Power( BIG_SPEED ),
m_Speed( 1.f ),
m_RingMax( (power < POWER_MIN)? POWER_MIN : power ),
m_StartTime( timeGetTime() )
{
	// メッシュデータ獲得
	m_lpMesh = CSXfileManager::GetInst()->GetData( MESH_PASS );

	m_Points.SetScaleMax( 4.f );
}

// 更新
void CBubbleRing::Update()
{
	if( m_Flg & FLG_BREAK )
	{
		// 爆発更新
		UpdateBreak();
	
	}else if( m_Flg & FLG_ALIVE ){

		// 徐々に拡大
		if( ( m_Power += BIG_SPEED ) > m_RingMax )
		{
			m_Power = m_RingMax;
		}

		m_mMat.MoveLocal( 0, 0, m_Speed );

		// 泡追加(フレームに１回)
		if( m_StartTime + m_CreateBubbleNum * CREATE_BUBBLE_TIME < timeGetTime() )
		{
			m_CreateBubbleNum++;

			// 拡大成分を取り除く
			CMatrix mMat( m_mMat.GetPos(  ) );

			m_Bubbles.Add( &mMat, 1 );
		}

		// 一定時間で消滅
		if( (m_StartTime + ERASE_TIME < timeGetTime())									|| 
			Sequence::CGameMain::SURFACESEA_HEIGHT < m_mMat.GetPos(  ).y	)
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
			m_Points.SetScaleMax( 4.f );
			m_Points.SetMove( D3DXVECTOR3( 1.f, -0.5f, 1.f ) );
			m_Points.SetPos( m_mMat.GetPos(  ) );
			m_Points.SetEraseTime( BUBBLE_ERASE_TIME );
		}
	}

	// 泡も消えてたら削除
	if( !(m_Flg & FLG_ALIVE) && m_Points.IsErase() && m_Bubbles.IsEmpty() )
	{
		m_Flg |= FLG_ERASE;
	}

	// 泡
	m_Bubbles.Update();

	// ポイントスプライト
	m_Points.Update();
}

// 描画
void CBubbleRing::Draw( const LPDIRECT3DDEVICE9 lpdevice )
{
	if( m_Flg & FLG_ALIVE && IsCulling( &m_mMat ) )
	{
		if( m_Flg & FLG_ALIVE )
		{
			// 拡大行列を作って描画の時だけ適用
			CMatrix mScale;
			mScale.SetScale( m_Power );

			lpdevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
			lpdevice->SetTransform( D3DTS_WORLD, &( mScale * m_mMat ) );

			lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

			m_lpMesh->Draw( lpdevice );

			lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			lpdevice->SetRenderState( D3DRS_NORMALIZENORMALS, FALSE );
		}
	}

	m_Bubbles.DrawNoBreak( lpdevice );
}

// エフェクト描画
void CBubbleRing::DrawEffect( const LPDIRECT3DDEVICE9 lpdevice )
{
	lpdevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// 泡
	if( !(m_Flg & FLG_ALIVE) )
	{
		m_Points.Draw( lpdevice );
	}
	
	m_Bubbles.DrawBreak( lpdevice );

	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}

// 情報を送りつける
void CBubbleRing::Dispatch(IDoubleDispatch *object)
{
	object->Action( *this );
}

// 敵に対する行動
void CBubbleRing::Action( Enemy::CBase& object )
{
	m_Flg &= ~FLG_ALIVE;

	m_Points.SetNum( POINT_NUM );
	m_Points.SetTexture( CSTexManager::GetInst()->GetData( SPRITE_TEX_PASS )->GetTexture() );
	m_Points.SetMove( D3DXVECTOR3( 1.f, -1.f, 1.f ) );
	m_Points.SetScaleMax( 4.f );
	m_Points.SetPos( m_mMat.GetPos() );
	m_Points.SetEraseTime( BUBBLE_ERASE_TIME );
}

// ボスに対する行動
void CBubbleRing::Action(Boss::CBase &object)
{
	if( (object.GetFlg() & Boss::CBase::FLG_ALIVE) && 
		(m_Flg & FLG_ALIVE) )
	{
		m_Flg &= ~FLG_ALIVE;

		m_Points.SetNum( POINT_NUM );
		m_Points.SetTexture( CSTexManager::GetInst()->GetData( SPRITE_TEX_PASS )->GetTexture() );
		m_Points.SetMove( D3DXVECTOR3( 1.f, -1.f, 1.f ) );
		m_Points.SetScaleMax( 4.f );
		m_Points.SetPos( m_mMat.GetPos() );
		m_Points.SetEraseTime( BUBBLE_ERASE_TIME );
	}
}

// プレイヤーに対する行動
void CBubbleRing::Action(Player::CDolphin &object)
{
	if( !(m_Flg & FLG_BREAK) )
	{
		m_Board.SetVertex( 2 );

		float X = static_cast<float>(BREAK_GRAPH_SIZE) / BREAK_GRAPH_WIDTH;

		m_Board.SetTexturePos(
			&D3DXVECTOR2( m_AnimeCnt * X,		0 ),
			&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	0 ),
			&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	1.f ),
			&D3DXVECTOR2( m_AnimeCnt * X,		1.f ) );

		m_Flg |= FLG_BREAK;
	}
}

// 弾に対する行動
void CBubbleRing::Action(Shot::CBase &object)
{
	m_Flg &= ~FLG_ALIVE;
}

// デストラクタ
CBubbleRing::~CBubbleRing()
{
}