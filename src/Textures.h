#ifndef __AKI_TEXTURE_H__
#define	__AKI_TEXTURE_H__

//////////////////////////////////////////////////
// テクスチャクラス
// 1オブジェクト＝1つ
//////////////////////////////////////////////////
class CTexture : public boost::noncopyable
{
public:

	//////////////////////////////////////////////////
	// メソッド
	//////////////////////////////////////////////////

	// コンストラクタ
	CTexture();
	
	// デストラクタ
	~CTexture();

	//////////////////////////////////////////////////
	//	・関数名	CreateFile
	//	・説明		ファイルからテクスチャを作成する
	//				勝手に2のべき乗サイズに調整する
	//	・引数		
	//		- lpdevice	デバイス
	//		- file_name	読み込む画像のファイル名
	//		- color_key	描画しない色
	//	・戻り値	読み込めればテクスチャを
	//				読み込めなければNULLを返す
	//////////////////////////////////////////////////
	const LPDIRECT3DTEXTURE9 CreateFile( LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, D3DCOLOR color_key );

	//////////////////////////////////////////////////
	//	・関数名	CreateRenderTarget
	//	・説明		ファイルからテクスチャを作成する
	//				勝手に2のべき乗サイズに調整する
	//	・引数		
	//		- lpdevice	デバイス
	//		- width		幅
	//		- height	高さ
	//		- format	フォーマット
	//	・戻り値	読み込めればテクスチャを
	//				読み込めなければNULLを返す
	//////////////////////////////////////////////////
	const LPDIRECT3DTEXTURE9 CreateRenderTarget( LPDIRECT3DDEVICE9 lpdevice, UINT width, UINT height, D3DFORMAT format = D3DFMT_A8B8G8R8 );

	//////////////////////////////////////////////////
	//	・関数名	GetTexture
	//	・説明		明示的にテクスチャを獲得する
	//	・引数		なし
	//	・戻り値	テクスチャ情報を返す
	//////////////////////////////////////////////////
	inline const LPDIRECT3DTEXTURE9 GetTexture() const
	{
		return m_lpTex;
	}

	//////////////////////////////////////////////////
	//	・関数名	operator const LPDIRECT3DTEXTURE9()
	//	・説明		暗黙にテクスチャを獲得する
	//	・引数		なし
	//	・戻り値	テクスチャ情報を返す
	//////////////////////////////////////////////////
	inline operator const LPDIRECT3DTEXTURE9() const
	{
		return m_lpTex;
	}
	
	//////////////////////////////////////////////////
	//	・関数名	GetZbuffer
	//	・説明		レンダーターゲットの
	//				深度バッファ情報を獲得
	//	・引数		なし
	//	・戻り値	深度バッファ情報を返す
	//////////////////////////////////////////////////
	inline const LPDIRECT3DSURFACE9 GetZbuffer() const
	{
		return m_lpZbuffer;
	}

	//////////////////////////////////////////////////
	//	・関数名	GetSurface
	//	・説明		レンダーターゲットの
	//				サーフェス情報を獲得
	//	・引数		なし
	//	・戻り値	サーフェス情報を返す
	//////////////////////////////////////////////////
	inline const LPDIRECT3DSURFACE9 GetSurface() const
	{
		return m_lpSurface;
	}

	//////////////////////////////////////////////////
	//	・関数名	GetSurfaceDesc
	//	・説明		レンダーターゲットの
	//				詳しい情報を獲得
	//	・引数		なし
	//	・戻り値	詳しい情報を返す
	//////////////////////////////////////////////////
	inline const D3DSURFACE_DESC& GetSurfaceDesc() const
	{
		return m_Desc;
	}

private:	// 非公開
		
	//////////////////////////////////////////////////
	// フィールド
	//////////////////////////////////////////////////
	LPDIRECT3DTEXTURE9		m_lpTex;		// テクスチャ
	LPDIRECT3DSURFACE9		m_lpZbuffer;	// Zバッファ
	LPDIRECT3DSURFACE9		m_lpSurface;	// サーフェス
	D3DSURFACE_DESC			m_Desc;			// テクスチャ情報
};

#endif // __AKI_TEXTURE_H__