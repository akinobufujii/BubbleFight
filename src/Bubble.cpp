///////////////////////////////
// インクルード
///////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

#include "ShotBase.h"
#include "NormalShot.h"
#include "ShotManager.h"

#include "BossBase.h"
#include "EnemyManager.h"

#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "UI.h"
#include "BubbleManager.h"
#include "GameMain.h"

#include "Bubble.h"

///////////////////////////////
// 静的メンバ実体化
///////////////////////////////
const LPSTR		CBubble::BUBBLE_MESH_PASS	= "Bubble.x";
const USHORT	CBubble::ERASE_TIME			= 3500;
const BYTE		CBubble::BUBBLE_RAND		= 100;
const float		CBubble::SPEED				= 0.5f / 50;
const float		CBubble::SPEED_MAX			= SPEED * 500;
const float		CBubble::SCALE				= 0.3f;

const int		CBubble::BURST_WIDTH			= 256;
const int		CBubble::BURST_HEIGHT			= 256;
const int		CBubble::BURST_ANIME_SIZE		= 32;
const int		CBubble::BURST_ANIME_WIDTH_NUM	= 8;
const int		CBubble::BURST_ANIME_HEIGHT_NUM	= 8;
const int		CBubble::BURST_ANIME_NUM		= CBubble::BURST_ANIME_WIDTH_NUM * (CBubble::BURST_ANIME_HEIGHT_NUM - 1);

const BYTE		CBubble::ERASE		= 0x01;
const BYTE		CBubble::BREAK		= 0x02;
const BYTE		CBubble::ENEMY_HIT	= 0x04;

///////////////////////////////
// 実装
///////////////////////////////

// コンストラクタ
CBubble::CBubble():
m_Flg( 0 ),
m_StartTime( timeGetTime() ),
m_NowSpeed( 0.f ),
m_AnimeCnt( 0 ),
m_lpMesh( CSXfileManager::GetInst()->GetData( BUBBLE_MESH_PASS ) )
{
	// 適当にばらす
	m_mMat.RotateLocalX( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );
	m_mMat.RotateLocalY( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );
	m_mMat.RotateLocalZ( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );

	// 縮小する
	m_mMat.ScaleLocal( SCALE ); 

	m_Board.SetVertex( 1.0f );
}

// デストラクタ
CBubble::~CBubble()
{
}

// 更新
void CBubble::Update()
{
	// 破壊されてたら
	if( m_Flg & BREAK )
	{
		// 爆発更新
		if( ++m_AnimeCnt >= BURST_ANIME_NUM )
		{
			m_Flg |= ERASE;	// 消す
		}

	}else{
		// 泡の浮力
		m_NowSpeed += SPEED;

		// 実際に動かす
		m_mMat.MoveLocal( 0, 0, ( m_NowSpeed < SPEED_MAX )? SPEED - m_NowSpeed : 0 );

		// 浮力の方向に動かす
		CMatrix Mat( 0, m_NowSpeed, 0 );

		m_mMat = m_mMat * Mat;

		// 一定時間超えれば破裂
		if( m_StartTime + ERASE_TIME < timeGetTime() )
		{
			m_Flg |= BREAK;

			//m_Points.SetTexture( CSTexManager::GetInst()->GetData( "Sprite2.png" )->GetTexture() );
		}

		// 水面に出れば消える
		if( m_mMat.GetPos().y > Sequence::CGameMain::SURFACESEA_HEIGHT )
		{
			m_Flg |= ERASE;	// 消す
		}
	}
}

// 描画
void CBubble::Draw( const LPDIRECT3DDEVICE9 lpdevice )
{
	// カメラ獲得
	CCamera Camera = *Sequence::CGameMain::GetCamera();

	// 画面内なら
	if( IsCulling( &m_mMat ) )
	{
		// 破壊されてたら
		if( m_Flg & BREAK )
		{
			// テクスチャ座標を設定する
			m_Board.SetTexturePos(
				&D3DXVECTOR2(
				(m_AnimeCnt % BURST_ANIME_WIDTH_NUM * (float)BURST_ANIME_SIZE) / BURST_WIDTH,
				(m_AnimeCnt / BURST_ANIME_HEIGHT_NUM* (float)BURST_ANIME_SIZE) / BURST_HEIGHT ),

				&D3DXVECTOR2( 
				((m_AnimeCnt % BURST_ANIME_WIDTH_NUM + 1)	* (float)BURST_ANIME_SIZE) / BURST_WIDTH,
				(m_AnimeCnt	 / BURST_ANIME_HEIGHT_NUM		* (float)BURST_ANIME_SIZE) / BURST_HEIGHT ),

				&D3DXVECTOR2(
				((m_AnimeCnt % BURST_ANIME_WIDTH_NUM + 1)	* (float)BURST_ANIME_SIZE) / BURST_WIDTH,
				((m_AnimeCnt / BURST_ANIME_HEIGHT_NUM+ 1)	* (float)BURST_ANIME_SIZE)	/ BURST_HEIGHT ),

				&D3DXVECTOR2( 
				(m_AnimeCnt	 % BURST_ANIME_WIDTH_NUM		* (float)BURST_ANIME_SIZE)	/ BURST_WIDTH,
				((m_AnimeCnt / BURST_ANIME_HEIGHT_NUM + 1)  * (float)BURST_ANIME_SIZE)	/ BURST_HEIGHT )
				);

			// ライティングを切る
			lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );

			CMatrix Mat, mTrans;
			CVector	Pos = m_mMat.GetPos();

			lpdevice->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 );
			
			Camera.GetBillboard( &Mat, &m_mMat );

			mTrans.SetTrans( Pos.x, Pos.y, Pos.z );

			Mat = Mat * mTrans;
			lpdevice->SetTransform( D3DTS_WORLD, &Mat );

			CTexture* lpTex = CSTexManager::GetInst()->GetData( "Smoke.png" );

			if( lpTex )
			{
				lpdevice->SetTexture( 0, lpTex->GetTexture() );
			}
			lpdevice->SetRenderState( D3DRS_ZENABLE, FALSE );
			lpdevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
			lpdevice->SetFVF( CBoard::FVF_VERTEX );
			
			m_Board.Draw( lpdevice );

			lpdevice->SetRenderState( D3DRS_ZENABLE, TRUE );
			lpdevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
			lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );

		}else{
		
			lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
			m_lpMesh->Draw( lpdevice );
		}
	}
}

// 行列を設定する
void CBubble::SetMatrix( const D3DXMATRIX& mat )
{
	m_mMat = mat;

	// 適当にばらす
	m_mMat.RotateLocalX( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );
	m_mMat.RotateLocalY( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );
	m_mMat.RotateLocalZ( static_cast<float>( (rand() % BUBBLE_RAND) * (rand() % 2 * 2 - 1) ) );
	m_mMat.ScaleLocal( SCALE );
}

// 敵に対する行動
void CBubble::Action( Enemy::CBase& object )
{
	m_Flg |= ENEMY_HIT | ERASE;
}