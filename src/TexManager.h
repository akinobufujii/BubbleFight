#ifndef __AKI_TEXMANAGER_H__
#define __AKI_TEXMANAGER_H__

////////////////////////////////////
// テクスチャマネージャー
// (シングルトン)
////////////////////////////////////
class CSTexManager : public boost::noncopyable
{
public:	// 外部公開

	////////////////////////////////////
	// メソッド
	////////////////////////////////////

	// 生成
	static bool Create()
	{
		if( m_lpInst == NULL )
		{
			m_lpInst = NEW CSTexManager();
			return true;
		}

		return false;
	}

	// インスタンス獲得
	static CSTexManager* GetInst()
	{
		return m_lpInst;
	}

	// 破壊
	static void Destroy()
	{
		SAFE_DELETE( m_lpInst );
	}

	// 読み込み
	bool Load( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR name, D3DCOLOR color_key = NULL );

	// 読み込みリストから読み込み
	bool LoadFromTextList( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR list_pass );

	// 情報獲得
	CTexture* GetData( const LPCSTR name );

	// 解放
	void Release( const LPCSTR name );

	// 解放リストから解放
	void ReleaseFromTextList( const LPCSTR list_pass );

	// 全て解放
	void AllRelease();

private:// 非公開

	////////////////////////////////////
	// 型宣言
	////////////////////////////////////
	typedef std::pair< std::string, CTexture* >	PAIRTYPE;
	typedef std::map< std::string, CTexture* >	MAPTYPE;

	////////////////////////////////////
	// フィールド
	////////////////////////////////////
	static CSTexManager*	m_lpInst;	// インスタンス
	MAPTYPE					m_TexMap;	// テクスチャマップ

	////////////////////////////////////
	// メソッド
	////////////////////////////////////
	
	// コンストラクタ
	CSTexManager();

	// デストラクタ
	~CSTexManager();
};

#endif // __AKI_TEXMANAGER_H__