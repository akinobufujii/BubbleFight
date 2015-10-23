//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "FishManager.h"
#include "LimitWalls.h"
#include "GameMain.h"

#include "SunLine.h"

//////////////////////////////////////////////////
// 静的メンバ実体化
//////////////////////////////////////////////////
const float	CSunLine::SUNLINE_WIDTH		= 10;
const float	CSunLine::SUNLINE_HALFWIDTH	= SUNLINE_WIDTH / 2.f;
const float	CSunLine::BOTTOM_POS		= 30;

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CSunLine::CSunLine():
m_IsAddAlpha( true ),
m_NowBoradAlpha( 0 ),
m_ALPHASPEED( rand() % 3 + 1 )
{
	m_Borad.SetVertex( 
		D3DXVECTOR3( 0.f, Sequence::CGameMain::SURFACESEA_HEIGHT, 0.f ),
		D3DXVECTOR3( SUNLINE_HALFWIDTH, Sequence::CGameMain::SURFACESEA_HEIGHT, 0.f ),
		D3DXVECTOR3( SUNLINE_WIDTH, 0, 0.f ),
		D3DXVECTOR3( SUNLINE_HALFWIDTH, 0, 0.f ) );

	m_Borad.SetVertexColors(
		D3DCOLOR_ARGB( m_NowBoradAlpha + 0x80, 0xff, 0xff, 0xff ),
		D3DCOLOR_ARGB( m_NowBoradAlpha + 0x80, 0xff, 0xff, 0xff ),
		D3DCOLOR_ARGB( m_NowBoradAlpha, 0xff, 0xff, 0xff ),
		D3DCOLOR_ARGB( m_NowBoradAlpha, 0xff, 0xff, 0xff ) );
}

// デストラクタ
CSunLine::~CSunLine()
{
}

// 更新
void CSunLine::Update()
{
	static float Anime = 0;

	//if( (Anime += 0.01f) > 1.f )
	{
		Anime = 0;
	}

	m_Borad.SetTexturePos(
		&D3DXVECTOR2( Anime, 0.f ),
		&D3DXVECTOR2( 1.f + Anime, 0.f ),
		&D3DXVECTOR2( 1.f + Anime, 1.f ),
		&D3DXVECTOR2( Anime, 1.f ) );

	// アルファ値更新
	if( m_IsAddAlpha )
	{
		if( (m_NowBoradAlpha += m_ALPHASPEED) > 0x7f )
		{
			m_NowBoradAlpha = 0x7f;
			m_IsAddAlpha = false;
		}
	
	}else{

		if( (m_NowBoradAlpha -= m_ALPHASPEED) < 0 )
		{
			m_NowBoradAlpha = 0;
			m_IsAddAlpha = true;
		}
	}

	m_Borad.SetVertexColors(
		D3DCOLOR_ARGB( m_NowBoradAlpha + 0x80, 0xff, 0xff, 0xff ),
		D3DCOLOR_ARGB( m_NowBoradAlpha + 0x80, 0xff, 0xff, 0xff ),
		D3DCOLOR_ARGB( 0, 0xff, 0xff, 0xff ),
		D3DCOLOR_ARGB( 0, 0xff, 0xff, 0xff ) );
}

// 描画
void CSunLine::Draw(const LPDIRECT3DDEVICE9 lp_device) const
{
	// レンダーステートやテクスチャなどは
	// マネージャーに任せる
	m_Borad.Draw( lp_device );
	CMatrix mScale, mRotZ, mRotY;
	mScale.SetScale( 2.f, 12.f, 2.f );
	mRotZ.SetRotZ( 15 );
	mRotY.SetRotY( timeGetTime() / 50 % 360 );
	lp_device->SetTransform( D3DTS_WORLD, &(mScale * mRotY * mRotZ * CMatrix( m_Pos )) );
	CSXfileManager::GetInst()->GetData( "SpotLight.x" )->Draw( lp_device );
}

// 位置決定
void CSunLine::SetPos(const D3DXVECTOR3 &pos)
{
	m_Pos = pos;

	m_Borad.SetVertex( 
		pos,
		D3DXVECTOR3( pos.x + SUNLINE_HALFWIDTH, pos.y, pos.z ),
		D3DXVECTOR3( BOTTOM_POS + pos.x + SUNLINE_WIDTH,	 pos.y - 100, pos.z ),
		D3DXVECTOR3( BOTTOM_POS + pos.x + SUNLINE_HALFWIDTH, pos.y - 100, pos.z ) );
}