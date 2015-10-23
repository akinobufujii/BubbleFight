#ifndef __AKI_EFFECT_H__
#define __AKI_EFFECT_H__

////////////////////////////////
// シェーダマネージャ
////////////////////////////////
class CEffect
{
public:	// 外部公開

	//////////////////////////////
	// メソッド
	//////////////////////////////

	// コンストラクタ
	CEffect();
	CEffect( LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, LPCSTR technique );

	// デストラクタ
	~CEffect();

	// 読み込み
	bool Load(LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, LPCSTR technique );

	// 描画開始
	const LPD3DXEFFECT Begin( LPCSTR technique = NULL );

	// 描画終了
	void End();

	// テクニックを設定する
	void SetTechnique( LPCSTR technique_name );

	// パス設定
	void BeginPass( int pass );
	void EndPass();

	// 値を設定する
	void SetValue( LPCSTR handle, LPCVOID lp_value, UINT byte );
	
	// テクスチャを設定
	void SetTexture( LPCSTR handle, LPDIRECT3DBASETEXTURE9 lp_tex );
	
	// ハンドルを作る
	bool CreateHandle( LPCSTR handle_name );

	// ハンドルを獲得
	D3DXHANDLE GetHandle( LPCSTR handle_name );

	// デバイス獲得
	inline const LPD3DXEFFECT GetDevice()
	{
		return m_lpEffect;
	}


private:	// 非公開
	//////////////////////////////
	// フィールド
	//////////////////////////////
	LPD3DXEFFECT					m_lpEffect;	// シェーダデバイス
	std::map< LPCSTR, D3DXHANDLE >	m_HandleMap;// ハンドル
	LPCSTR							m_TecKey;	// テクニックキー

	//////////////////////////////
	// メソッド
	//////////////////////////////

	// ハンドルチェック
	D3DXHANDLE CheckHandle( LPCSTR handle_name );

	//////////////////////////////
	// フレンドクラス指定
	//////////////////////////////
	friend class CSShaderManager;
};

#endif // __AKI_EFFECT_H__