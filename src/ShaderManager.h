#ifndef __AKI_SHADERMANAGER_H__
#define __AKI_SHADERMANAGER_H__

////////////////////////////////
// シェーダーマネージャー
// (シングルトン)
////////////////////////////////
class CSShaderManager : public boost::noncopyable
{
public:	// 外部公開

	////////////////////////////////
	// メソッド
	////////////////////////////////

	// 生成
	inline static bool Create()
	{
		if( m_lpInst == NULL )
		{
			m_lpInst = NEW CSShaderManager();
			
			return true;
		}

		return false;
	}

	// 破壊
	inline static void Destroy()
	{
		SAFE_DELETE( m_lpInst );
	}

	// インスタンス獲得
	inline static CSShaderManager* GetInst() 
	{
		return m_lpInst;
	}

	// 読み込み
	CEffect* Load( LPDIRECT3DDEVICE9 lp_device, LPCSTR path );

	// ファイルから読み込み
	void LoadFromFile( LPDIRECT3DDEVICE9 lp_device, LPCSTR filepath );

	// シェーダー獲得
	CEffect* GetData( LPCSTR path );

	// 解放
	void Release( LPCSTR path );

	// 全て開放
	void AllRelease();

private:	// 非公開
	////////////////////////////////
	// 型宣言
	////////////////////////////////
	typedef std::pair< std::string, CEffect* >	PAIRTYPE;
	typedef std::map< std::string, CEffect* >	MAPTYPE;

	////////////////////////////////
	// フィールド
	////////////////////////////////
	static CSShaderManager*	m_lpInst;	// インスタンス

	MAPTYPE					m_ShaderMap;	// シェーダマップ

	////////////////////////////////
	// メソッド
	////////////////////////////////

	// コンストラクタ
	CSShaderManager();

	// デストラクタ
	~CSShaderManager(void);
};

#endif	// __AKI_SHADERMANAGER_H__