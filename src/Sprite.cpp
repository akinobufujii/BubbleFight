/////////////////////////////////////
// インクルード
/////////////////////////////////////
#include "Sprite.h"

/////////////////////////////////////
// 実装
/////////////////////////////////////

// コンストラクタ
CSprite::CSprite():
m_lpSprite( NULL ),
m_Alpha( 0xff )
{
}

CSprite::CSprite( const LPDIRECT3DDEVICE9 lpdevice ):
m_lpSprite( NULL ),
m_Alpha( 0xff )
{
	Create( lpdevice );
}

// デバイス作成
bool CSprite::Create( const LPDIRECT3DDEVICE9 lpdevice )
{
	// ※念のため
	SAFE_RELEASE( m_lpSprite );

	// 作成
	HRESULT hr = D3DXCreateSprite( lpdevice, &m_lpSprite );

	if( SUCCEEDED( hr ) )
	{
		m_lpSprite->OnResetDevice();
		return true;
	
	}else{

		return false;
	}
}

// 描画
void CSprite::Draw( const LPDIRECT3DTEXTURE9 lptex, float dest_x, float dest_y, float rot, int src_left, int src_top, int src_right, int src_bottom )
{
	// 矩形設定
	RECT Src = { src_left + 1, src_top + 1, src_right - 1, src_bottom - 1 };

	// 中心決定
	float HarfX = (src_right - src_left) * 0.5f;
	float HarfY	= (src_bottom - src_top) * 0.5f;
	
	D3DXVECTOR3 Vec( HarfX, HarfY, 0.f );

	D3DXMATRIX mWorld, mTrans, mRotZ;

	// 動かす
	// 移動
	D3DXMatrixTranslation( &mTrans, dest_x + HarfX, dest_y + HarfY, 0.f );

	// 回転(念のため余りで計算)
	D3DXMatrixRotationZ( &mRotZ, D3DXToRadian( rot ) );

	//D3DXMatrixScaling( &mWorld , 0.5f, 0.5f, 0.5f );

	// 合成
	mWorld = mRotZ * mTrans;

	// 設定する
	m_lpSprite->SetTransform( &mWorld );

	// 描画
	m_lpSprite->Draw( lptex, &Src, &Vec, NULL, D3DCOLOR_ARGB( m_Alpha, 0xff, 0xff, 0xff ) );
}

// すでにあるデバイスで描画
void CSprite::Draw(
		const LPD3DXSPRITE lpsprite,
		const LPDIRECT3DTEXTURE9 lptex,
		float dest_x,
		float dest_y,
		float rot,
		int src_left,
		int src_top,
		int src_right,
		int src_bottom,
		int alpha )
{
	// 矩形設定
	RECT Src = { src_left, src_top, src_right, src_bottom };

	// 中心決定
	float HarfX = (src_right - src_left) * 0.5f;
	float HarfY	= (src_bottom - src_top) * 0.5f;
	
	D3DXVECTOR3 Vec( HarfX, HarfY, 0.f );

	D3DXMATRIX mWorld, mTrans, mRotZ;

	// 動かす
	// 移動
	D3DXMatrixTranslation( &mTrans, dest_x + HarfX, dest_y + HarfY, 0.f );

	// 回転(念のため余りで計算)
	D3DXMatrixRotationZ( &mRotZ, D3DXToRadian( rot ) );

	// 合成
	mWorld = mRotZ * mTrans;

	// 設定する
	lpsprite->SetTransform( &mWorld );

	// 描画
	lpsprite->Draw( lptex, &Src, &Vec, NULL, D3DCOLOR_ARGB( alpha, 0xff, 0xff, 0xff ) );
}

// デストラクタ
CSprite::~CSprite()
{
	SAFE_RELEASE( m_lpSprite );
}
