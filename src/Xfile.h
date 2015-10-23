#ifndef __AKI_XFILE_H__
#define __AKI_XFILE_H__

//////////////////////////////////////////////////
// Xファイル読み込みクラス
//////////////////////////////////////////////////
class CXfile
{
public:	// 外部公開

	//////////////////////////////////////////////////
	// 定数
	//////////////////////////////////////////////////
	CONSTANT DWORD				CUSTOM_FVF;	// 頂点フォーマット
	CONSTANT D3DVERTEXELEMENT9	MESH_ELEMENT[];	// 頂点フォーマット
		
	//////////////////////////////////////////////////
	// 共通メソッド
	//////////////////////////////////////////////////

	//////////////////////////////////////////////////
	//	・関数名	SetShaderDevice
	//	・説明		全てのメッシュに対して使用する
	//				シェーダデバイスとワールド行列、
	//				テクスチャハンドルを設定する
	//				SetDrawShaderをtrueにすると適応される
	//	・引数		
	//		- lp_effect	使用するシェーダデバイス
	//		- world		ワールド行列を渡すハンドル名
	//		- tex		テクスチャを渡すハンドル名
	//	・戻り値	なし
	//////////////////////////////////////////////////
	static void SetShaderDevice( CEffect* lp_effect, D3DXHANDLE world, D3DXHANDLE tex = NULL )
	{
		m_lpEffect	= lp_effect;
		m_lphWorld	= world;
		m_lphTex	= tex;
	}

	//////////////////////////////////////////////////
	//	・関数名	SetDrawShader
	//	・説明		プログラマブルシェーダで描画するかを設定する
	//				SetShaderDeviceを呼んでおく必要がある
	//				trueを渡すとその後Drawメソッドは
	//				SetShaderDeviceで設定された情報で描画する
	//	・引数		
	//		- is_draw	シェーダで描画するかどうか
	//	・戻り値	なし
	//////////////////////////////////////////////////
	static void SetDrawShader( const bool is_draw )
	{
		m_IsShaderDraw = is_draw;
	}

	//////////////////////////////////////////////////
	//	・関数名	GetShaderParam
	//	・説明		現在設定されている
	//				プログラマブルシェーダの情報を獲得
	//	・引数		
	//		- lpp_effect	格納するCEffect*型変数
	//		- lp_world		ワールド行列を格納するD3DXHANDLE*型変数
	//		- lp_tex		テクスチャハンドルを格納するD3DXHANDLE*変数
	//	・戻り値	なし(引数が戻り値)
	//////////////////////////////////////////////////
	static void GetShaderParam( CEffect** lpp_effect, D3DXHANDLE* lp_world, D3DXHANDLE* lp_tex )
	{
		*lpp_effect	= m_lpEffect;
		*lp_world	= m_lphWorld;
		*lp_tex		= m_lphTex;
	}

	//////////////////////////////////////////////////
	// メソッド
	//////////////////////////////////////////////////
	
	// コンストラクタ
	CXfile();
	CXfile( const LPDIRECT3DDEVICE9 lpD3DDevice, const LPCSTR name );

	// デストラクタ
	~CXfile();

	//////////////////////////////////////////////////
	//	・関数名	Load
	//	・説明		Xファイルの読み込みを行う
	//	・引数		
	//		- lpdevice	デバイス
	//		- name		ファイルパス
	//	・戻り値	読み込み成功ならtrue
	//				失敗ならfalseを返す
	//////////////////////////////////////////////////
	bool Load( const LPDIRECT3DDEVICE9 lpdevice, const LPCSTR name );

	//////////////////////////////////////////////////
	//	・関数名	Draw
	//	・説明		読み込んだXファイルの描画を行う
	//				SetDrawShaderをtrueで設定すると
	//				SetShaderDeviceで設定された内容で
	//				描画を行う
	//	・引数		
	//		- lpdevice	デバイス
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void Draw( const LPDIRECT3DDEVICE9 lpD3DDevice ) const;

	//////////////////////////////////////////////////
	//	・関数名	DrawShader
	//	・説明		プログラマブルシェーダで読み込んだ
	//				Xファイルの描画を行う
	//	・引数		
	//		- lp_shader			プログラマブルシェーダデバイス
	//		- diffuse_handle	色を渡すハンドル
	//							NULLであれば使用されない
	//		- tex_handle		テクスチャを渡すハンドル
	//							NULLであれば使用されない
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void DrawShader(
		CEffect*	lp_shader,
		D3DXHANDLE	diffuse_handle = NULL,
		D3DXHANDLE	tex_handle = NULL,
		DWORD		no_tex_pass = 0,
		DWORD		use_tex_pass = 1);

	//////////////////////////////////////////////////
	//	・関数名	CreateNormals
	//	・説明		ポリゴンに対する法線情報を作成する
	//	・引数		なし
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void CreateNormals();

	//////////////////////////////////////////////////
	//	・関数名	SetRed
	//	・説明		ディフューズの赤成分を設定する
	//	・引数		
	//		- r		設定する値(0.f～1.f)
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void SetRed( const float r )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ].Diffuse.r = r;
		}
	}

	//////////////////////////////////////////////////
	//	・関数名	SetBlue
	//	・説明		ディフューズの赤成分を設定する
	//	・引数		
	//		- b		設定する値(0.f～1.f)
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void SetBlue( const float b )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ].Diffuse.b = b;
		}
	}

	//////////////////////////////////////////////////
	//	・関数名	SetGreen
	//	・説明		ディフューズの赤成分を設定する
	//	・引数		
	//		- g		設定する値(0.f～1.f)
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void SetGreen( const float g )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ].Diffuse.g = g;
		}
	}

	//////////////////////////////////////////////////
	//	・関数名	SetAlpha
	//	・説明		ディフューズの赤成分を設定する
	//	・引数		
	//		- a		設定する値(0.f～1.f)
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void SetAlpha( const float a )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ].Diffuse.a = a;
		}
	}

	//////////////////////////////////////////////////
	//	・関数名	SetEmissive
	//	・説明		自己照明を設定する
	//	・引数		
	//		- emissive	設定する値(0.f～1.f)
	//					RGB一括で設定される
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void SetEmissive( const float emissive )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ].Emissive.r = emissive;
			m_lpNowMatInfo[ i ].Emissive.g = emissive;
			m_lpNowMatInfo[ i ].Emissive.b = emissive;
		}
	}

	//////////////////////////////////////////////////
	//	・関数名	SetDefaultMaterial
	//	・説明		マテリアルを読み込んだ時点の情報に戻す
	//	・引数		なし
	//	・戻り値	なし
	//////////////////////////////////////////////////
	inline void SetDefaultMaterial()
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ] = m_lpMatInfo[ i ];
		}
	}

	//////////////////////////////////////////////////
	//	・関数名	GetMeshData
	//	・説明		メッシュ情報を獲得
	//	・引数		なし
	//	・戻り値	メッシュ情報をポインタで返す
	//////////////////////////////////////////////////
	inline const LPD3DXMESH* GetMeshData()	const 
	{
		return &m_lpMesh;
	} 

	//////////////////////////////////////////////////
	//	・関数名	GetNormal
	//	・説明		メッシュに対する法線情報を返す
	//				CreateNormalsを呼んでおく必要がある
	//	・引数		
	//		- num	法線情報の格納されているインデックス番号
	//	・戻り値	法線情報の格納されている
	//				インデックス番号ならば、法線が
	//				存在しないなら(0,0,0)で返す
	//////////////////////////////////////////////////
	inline D3DXVECTOR3 GetNormal( DWORD num ) const 
	{
		if( m_lpNormals )
		{
			if( num >= 0 && num < m_lpMesh->GetNumFaces() )
			{
				return m_lpNormals[ num ];
			}
		}

		return D3DXVECTOR3( 0, 0, 0 );
	}

	//////////////////////////////////////////////////
	//	・関数名	GetCenter
	//	・説明		メッシュの中心位置を獲得
	//	・引数		なし
	//	・戻り値	メッシュの中心位置を返す
	//////////////////////////////////////////////////
	inline D3DXVECTOR3 GetCenter() const 
	{
		return m_CenterPos;
	}

	//////////////////////////////////////////////////
	//	・関数名	GetRadius
	//	・説明		メッシュの半径を獲得
	//	・引数		なし
	//	・戻り値	メッシュの半径を返す
	//////////////////////////////////////////////////
	inline float GetRadius() const 
	{
		return m_Radius;
	}

	//////////////////////////////////////////////////
	//	・関数名	GetMaterialNum
	//	・説明		マテリアル数を獲得
	//	・引数		なし
	//	・戻り値	マテリアル数を返す
	//////////////////////////////////////////////////
	inline DWORD GetMaterialNum() const 
	{
		return m_MatNum;
	}

private:	// 非公開

	

	//////////////////////////////////////////////////
	// フィールド
	//////////////////////////////////////////////////
	LPD3DXMESH						m_lpMesh;		// メッシュ
	DWORD							m_MatNum;		// マテリアル数
	LPMATERIALARRAY					m_lpMatInfo;	// マテリアル情報
	LPMATERIALARRAY					m_lpNowMatInfo;	// マテリアル現在の情報
	LPTEXTUREARRAY					m_lppTex;		// テクスチャ
	LPDIRECT3DVERTEXDECLARATION9	m_lpDec;		// 頂点デクラレーション(使わない)
	D3DXVECTOR3						m_CenterPos;	// メッシュの中心
	LPVECTOR3ARRAY					m_lpNormals;	// 法線情報
	float							m_Radius;		// 半径

	//////////////////////////////////////////////////
	// 共通フィールド
	//////////////////////////////////////////////////
	static bool			m_IsShaderDraw;	// シェーダーで描画するかどうか
	static CEffect*		m_lpEffect;		// シェーダー
	static D3DXHANDLE	m_lphWorld;		// 行列を渡すハンドル
	static D3DXHANDLE	m_lphTex;		// テクスチャを渡すハンドル

	// 代入禁止
	CXfile( const CXfile& obj );
	CXfile& operator =( const CXfile& obj );

};

#endif // __AKI_XFILE_H__