#ifndef __AKI_XFILEMANAGER_H__
#define __AKI_XFILEMANAGER_H__

/////////////////////////////////
// 不完全型宣言
/////////////////////////////////
class CXfile;

/////////////////////////////////
// Xfileマネージャー
// (シングルトン)
/////////////////////////////////
class CSXfileManager : public boost::noncopyable
{
public:	// 外部公開

	/////////////////////////////////
	// メソッド
	/////////////////////////////////

	// 作成
	inline static bool Create()
	{
		// 無いときだけ作成
		if( m_lpInst == NULL )
		{
			m_lpInst = NEW CSXfileManager();
			return true;
		}

		return false;
	}

	// インスタンス獲得
	inline static CSXfileManager* GetInst()
	{
		return m_lpInst;
	}

	// 破壊
	inline static void Destroy()
	{
		SAFE_DELETE( m_lpInst );
	}

	// 読み込み
	CXfile* Load( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR name, bool is_create_normals = false );

	// 読み込みリストから読み込み
	bool LoadFromTextList( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR list_pass );

	// 情報獲得
	CXfile*	GetData( LPCSTR name );

	// 解放
	void Release( LPCSTR name );

	// 解放リストから解放
	void ReleaseFromTextList( const LPCSTR list_pass );

	// 全て解放
	void AllRelease();

private:	// 非公開

	/////////////////////////////////
	// 型宣言
	/////////////////////////////////
	typedef std::pair< std::string, CXfile* >	PAIRTYPE;
	typedef std::map< std::string, CXfile* >	MAPTYPE;

	/////////////////////////////////
	// フィールド
	/////////////////////////////////	
	static CSXfileManager*	m_lpInst;	// インスタンス
	MAPTYPE					m_XFileMap;	// XFileマップ

	/////////////////////////////////
	// メソッド
	/////////////////////////////////

	// コンストラクタ
	CSXfileManager();

	// デストラクタ
	~CSXfileManager();
};

#endif	// __AKI_XFILEMANAGER_H__