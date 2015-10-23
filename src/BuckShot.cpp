//////////////////////////////
// インクルード
//////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

#include "ShotBase.h"
#include "BuckShot.h"

//////////////////////////////
// 名前空間省略
//////////////////////////////
using namespace Shot;

//////////////////////////////
// 静的メンバ実体化
//////////////////////////////
const BYTE	CBuck::BUCKMAX_RAND		= 20;
const float	CBuck::SPEED			= 0.5f;
const short	CBuck::ERASE_TIME		= 5000;
const float	CBuck::BUBBLE_SPEED_MAX	= CBuck::SPEED * 10;
const LPSTR	CBuck::SHOT_MESH_PASS	= "Shot.x";

//////////////////////////////
// 実装
//////////////////////////////

// コンストラクタ
CBuck::CBuck( const D3DXMATRIX* mat ):
CBase( mat ),
m_StartTime( timeGetTime() ),
m_BubbleSpeed( 0.f )
{
	// 適当にばらす
	m_mMat.RotateLocalX( (rand() % BUCKMAX_RAND) * (rand() % 2 * 2 - 1) );
	m_mMat.RotateLocalY( (rand() % BUCKMAX_RAND) * (rand() % 2 * 2 - 1) );
	m_mMat.RotateLocalZ( (rand() % BUCKMAX_RAND) * (rand() % 2 * 2 - 1) );

	// 縮小する
	m_mMat.ScaleLocal( 0.3f); 
}

// 更新
void CBuck::Update()
{
	// 泡の浮力
	if( (m_BubbleSpeed += SPEED / 50) > BUBBLE_SPEED_MAX )
	{
		m_BubbleSpeed = BUBBLE_SPEED_MAX;	
	}

	// 実際に動かす
	m_mMat.MoveLocal( 0, 0, ( m_BubbleSpeed < SPEED )? -SPEED + m_BubbleSpeed : 0 );

	// 浮力の方向に動かす
	CMatrix Mat( 0, m_BubbleSpeed, 0 );
	
	m_mMat = m_mMat * Mat;

	if( m_StartTime + ERASE_TIME < timeGetTime() )
	{
		m_Flg |= ERASE;
	}
}

// 描画
void CBuck::Draw( const LPDIRECT3DDEVICE9 lpdevice )
{
	lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
	CSXfileManager::GetInst()->GetData( SHOT_MESH_PASS )->Draw( lpdevice );
}

// 情報を送りつける
void CBuck::Dispatch( IDoubleDispatch* object )
{
	object->Action( *this );
}

// 敵に対する行動
void CBuck::Action( Enemy::CBase& object )
{
	m_Flg |= ERASE;
}

// デストラクタ
CBuck::~CBuck()
{
}