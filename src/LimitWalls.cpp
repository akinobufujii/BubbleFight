//////////////////////////////
// 境界壁
//////////////////////////////
#include "LimitWalls.h"

//////////////////////////////
// 静的メンバ実体化
//////////////////////////////
const float		CLimitWalls::FADEIN_OUT_SPEED	= 0.01f;
const LPCSTR	CLimitWalls::TEXPATH_WALL		= "Wall.png";

//////////////////////////////
// 実装
//////////////////////////////

// コンストラクタ
CLimitWalls::CLimitWalls():
m_NowAlpha( 0 ),
m_IsAlphaPlus( true )
{
	// 角度だけは固定なので入れておく
	// 念のため真っ白に設定
	// 半透明
	for( int i = 0; i < WALL_NUM; i++ )
	{
		m_Walls[ i ].Board.SetAlpha( 1.0f );
		m_Walls[ i ].Board.SetColor( 1.f, 1.f, 1.f );
		m_Walls[ i ].mMat.RotateLocalY( i * 90 );
	}
}

// デストラクタ
CLimitWalls::~CLimitWalls()
{
}

// 境界範囲を設定する(原点から)
void CLimitWalls::SetLimit( const float limit )
{
	// 指定分だけ原点から離す(ローカル方式)
	BOOST_FOREACH( WALLDATA& Wall, m_Walls )
	{
		Wall.mMat.MoveLocal( 0, 0, -limit );
	}
}

// 大きさを指定
void CLimitWalls::SetScale( const float scale )
{
	// 大きさを設定する
	BOOST_FOREACH( WALLDATA& Wall, m_Walls )
	{
		Wall.Board.SetVertex( scale );
	}
}

void CLimitWalls::SetScale( const float width, const float height )
{
	// 大きさを設定する
	BOOST_FOREACH( WALLDATA& Wall, m_Walls )
	{
		Wall.Board.SetVertex( width, height );
	}
}

// 更新
void CLimitWalls::Update()
{
	// テクスチャの更新
	if( m_IsAlphaPlus )
	{
		if( (m_NowAlpha += FADEIN_OUT_SPEED) > 1 )
		{
			m_NowAlpha = 1.f;
			m_IsAlphaPlus = false;
		}	
	
	}else{

		if( (m_NowAlpha -= FADEIN_OUT_SPEED) < 0 )
		{
			m_NowAlpha = 0.f;
			m_IsAlphaPlus = true;
		}
	}

	BOOST_FOREACH( WALLDATA& Wall, m_Walls )
	{
		Wall.Board.SetAlpha( m_NowAlpha );
	}
}

// 描画
void CLimitWalls::Draw( const LPDIRECT3DDEVICE9 lp_device )
{
	// 頂点フォーマット設定
	lp_device->SetFVF( CBoard::FVF_VERTEX );

	// テクスチャ設定
	lp_device->SetTexture( 0, CSTexManager::GetInst()->GetData( TEXPATH_WALL )->GetTexture() );

	// それぞれの行列で一気に描画
	BOOST_FOREACH( WALLDATA& Wall, m_Walls )
	{
		lp_device->SetTransform( D3DTS_WORLD, &Wall.mMat );

		Wall.Board.Draw( lp_device );
	}
}