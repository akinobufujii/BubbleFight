

#include "BoardPolygon.h"

// 静的変数実体化
const int CBoardPolygon::VERTEX_FVF	= (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

// コンストラクタ
CBoardPolygon::CBoardPolygon( const LPDIRECT3DDEVICE9 lpDevice )
:m_lpTex(NULL)
{
	// 頂点バッファ作成
	lpDevice->CreateVertexBuffer(
		sizeof(Vertex) * 4,
		D3DUSAGE_WRITEONLY,
		VERTEX_FVF,
		D3DPOOL_MANAGED,
		&m_lpVB,
		NULL);
}

// コンストラクタ(引数付き)
CBoardPolygon::CBoardPolygon( const LPDIRECT3DDEVICE9 lpDevice, float w, float h )
:m_lpTex(NULL)
{
	// 頂点バッファ作成
	lpDevice->CreateVertexBuffer(
		sizeof(Vertex) * 4,
		D3DUSAGE_WRITEONLY,
		VERTEX_FVF,
		D3DPOOL_MANAGED,
		&m_lpVB,
		NULL);

	SetWH( w, h );
}

// 幅高さ設定
void CBoardPolygon::SetWH(float w, float h)
{
	Vertex*	lpV;

	// バッファロック
	HRESULT hr = m_lpVB->Lock( 0, 0, (LPVOID*)&lpV, 0 );

	float width = w / 2, height = h / 2;
	lpV[ 0 ].Pos = D3DXVECTOR3( -width, -height, 0 );
	lpV[ 1 ].Pos = D3DXVECTOR3(  width, -height, 0 );
	lpV[ 2 ].Pos = D3DXVECTOR3(  width,  height, 0 );
	lpV[ 3 ].Pos = D3DXVECTOR3( -width,  height, 0 );

	lpV[ 0 ].Color = lpV[ 1 ].Color = lpV[ 2 ].Color = lpV[ 3 ].Color
		= D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff );

	lpV[ 0 ].Tex = D3DXVECTOR2( 0.0f, 0.0f );
	lpV[ 1 ].Tex = D3DXVECTOR2( 1.0f, 0.0f );
	lpV[ 2 ].Tex = D3DXVECTOR2( 1.0f, 1.0f );
	lpV[ 3 ].Tex = D3DXVECTOR2( 0.0f, 1.0f );

	// 解除
	hr = m_lpVB->Unlock();
}

// 描画
void CBoardPolygon::Draw( const LPDIRECT3DDEVICE9 lpDevice )
{
	HRESULT hr = lpDevice->SetStreamSource( 0, m_lpVB, 0, sizeof(Vertex) );	// 頂点バッファセット
	hr = lpDevice->SetTexture( 0, m_lpTex );
	hr = lpDevice->SetFVF( VERTEX_FVF );
	hr = lpDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );
}

CBoardPolygon::~CBoardPolygon(void)
{
}
