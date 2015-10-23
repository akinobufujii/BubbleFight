#ifndef __AKI_BOARD_H__
#define __AKI_BOARD_H__

/////////////////////////////////
// 板ポリゴンクラス
/////////////////////////////////
class CBoard
{
public:	// 外部公開

	/////////////////////////////////
	// 定数
	/////////////////////////////////
	CONSTANT int FVF_VERTEX;	// 頂点フォーマット

	//////////////////////////////
	// 共通メソッド
	//////////////////////////////

	// シェーダーデバイス設定(共通)
	static void SetShaderDevice( CEffect* lp_effect, D3DXHANDLE world )
	{
		m_lpEffect	= lp_effect;
		m_lphWorld	= world;
	}

	// シェーダーで描画するかどうか
	static void SetDrawShader( const bool is_draw )
	{
		m_IsShaderDraw = is_draw;
	}

	/////////////////////////////////
	// メソッド
	/////////////////////////////////

	// コンストラクタ
	CBoard();
	CBoard( float size, bool is_wall = true );
	CBoard( float w, float h );
	CBoard( float left, float top, float right, float bottom );
	CBoard(	const D3DXVECTOR3& v1, 
		const D3DXVECTOR3& v2,
		const D3DXVECTOR3& v3,
		const D3DXVECTOR3& v4 );


	// 頂点設定
	// 正方形
	void SetVertex( float size, bool is_wall = true );
	
	// 長方形	
	void SetVertex( float w, float h );

	// 矩形
	void SetVertex( float left, float top, float right, float bottom );

	// 完全指定
	void SetVertex(
		const D3DXVECTOR3& v1, 
		const D3DXVECTOR3& v2,
		const D3DXVECTOR3& v3,
		const D3DXVECTOR3& v4 );

	// 色設定
	void SetColor( const D3DXCOLOR& color );
	void SetColor( float r, float g, float b );

	// 細く色を決める
	void SetVertexColors(
		const D3DCOLOR v1,
		const D3DCOLOR v2,
		const D3DCOLOR v3,
		const D3DCOLOR v4 );
	
	// 単色設定
	void SetRed		( float r );
	void SetGreen	( float g );
	void SetBlue	( float b );
	void SetAlpha	( float a );

	// テクスチャ座標指定
	void SetTexturePos( const D3DXVECTOR2* pos1, const D3DXVECTOR2* pos2, const D3DXVECTOR2* pos3, const D3DXVECTOR2* pos4 ); 

	// 描画
	bool Draw( const LPDIRECT3DDEVICE9 lpDevice ) const;
	
	// デストラクタ
	~CBoard();

private:	// 非公開
	
	/////////////////////////////////
	// 構造体宣言
	/////////////////////////////////
	struct Vertex
	{
		D3DXVECTOR3	Pos;	// 座標
		D3DCOLOR	Color;	// 色
		D3DXVECTOR2	Tex;	// テクスチャ座標
	};

	/////////////////////////////////
	// 共通フィールド
	/////////////////////////////////
	static bool			m_IsShaderDraw;	// シェーダーで描画するかどうか
	static CEffect*		m_lpEffect;		// シェーダー
	static D3DXHANDLE	m_lphWorld;		// 行列を渡すハンドル

	/////////////////////////////////
	// フィールド
	/////////////////////////////////
	LPDIRECT3DTEXTURE9	m_lpTex;		// テクスチャ
	Vertex				m_Vertex[ 4 ];	// 頂点情報
	D3DXCOLOR			m_Color;		// 色情報
};

#endif // __AKI_BOARD_H__