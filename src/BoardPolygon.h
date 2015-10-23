#ifndef __BOARDPOLYGON_H__
#define __BOARDPOLYGON_H__

#include "using.h"

// 板ポリクラス
class CBoardPolygon
{
private:
	static const int VERTEX_FVF;	// 頂点フォーマット

	// 内部構造体定義
	struct Vertex
	{
		D3DXVECTOR3	Pos;	// 座標
		D3DCOLOR	Color;	// 色
		D3DXVECTOR2	Tex;	// テクスチャ座標
	};

	LPDIRECT3DTEXTURE9		m_lpTex;// テクスチャ
	LPDIRECT3DVERTEXBUFFER9	m_lpVB;	// 頂点バッファ

public:
	// コンストラクタ
	CBoardPolygon( const LPDIRECT3DDEVICE9 lpDevice );
	CBoardPolygon( const LPDIRECT3DDEVICE9 lpDevice, float w, float h );

	// 板ポリゴンの幅高さを設定
	void SetWH( float w, float h );
	
	// テクスチャ設定
	inline void SetTexture(const LPDIRECT3DTEXTURE9 tex ){ m_lpTex = tex; }

	// 描画
	void Draw( const LPDIRECT3DDEVICE9 lpDevice );

	// デストラクタ
	~CBoardPolygon();
};

#endif // __BOARDPOLYGON_H__