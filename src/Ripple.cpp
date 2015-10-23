//////////////////////////////
// インクルード
//////////////////////////////
#include "Ripple.h"

//////////////////////////////
// 静的メンバ実体化
//////////////////////////////
const float		CRipple::FADEOUT_SPEED		= 0.05f;
const float		CRipple::NEXT_RIPPLE_LINE	= 0.3f;
const float		CRipple::DRAW_UPDOWN_POS	= 0.01f;

//////////////////////////////
// 実装
//////////////////////////////

// コンストラクタ
CRipple::CRipple():
m_NowAlpha( 1.f ),
m_Width( 10 ),
m_Height( 10 )
{
	m_Borad.SetColor( 1.f, 1.f, 1.f );
	m_Borad.SetAlpha( 1.f );
}

// デストラクタ
CRipple::~CRipple()
{
}

// 更新
void CRipple::Update()
{
	// だんだんアルファ値を下げていく
	if( (m_NowAlpha -= FADEOUT_SPEED) < 0 )
	{	
		m_NowAlpha = 0.f;
	}

	// 大きさは大きくしていく
	m_Width += m_ScaleRate;
	m_Height+= m_ScaleRate;
}

// 上側に描画
void CRipple::DrawUp(const LPDIRECT3DDEVICE9 lp_device)
{
	// 少し上側に設定
	D3DXVECTOR3 UpPos = m_Pos;
	UpPos.y += DRAW_UPDOWN_POS;

	// 出す位置を決定
	lp_device->SetTransform( D3DTS_WORLD, &(m_mRotXY * CMatrix( UpPos )) );

	// 描画直前に頂点設定をする
	m_Borad.SetAlpha( m_NowAlpha );
	m_Borad.SetVertex( m_Width, m_Height );

	// 描画
	m_Borad.Draw( lp_device );
}

// 下側に描画
void CRipple::DrawDown(const LPDIRECT3DDEVICE9 lp_device)
{
	// 少し上側に設定
	D3DXVECTOR3 DownPos = m_Pos;
	DownPos.y -= DRAW_UPDOWN_POS;

	// 出す位置を決定
	lp_device->SetTransform( D3DTS_WORLD, &(m_mRotXY * CMatrix( DownPos )) );

	// 描画直前に頂点設定をする
	m_Borad.SetAlpha( m_NowAlpha );
	m_Borad.SetVertex( m_Width, m_Height );

	// 描画
	m_Borad.Draw( lp_device );
}