#ifndef __AKI_MESHTEXMANAGER_H__
#define __AKI_MESHTEXMANAGER_H__

//////////////////////////
// メッシュ専用
//	テクスチャマネージャー
// (シングルトン)
//////////////////////////
class CSMeshTexManager : public boost::noncopyable
{
public:	// 外部公開

	//////////////////////////
	// メソッド
	//////////////////////////

	// 生成
	inline static bool Create()
	{
		if( m_lpInst == NULL )
		{
			m_lpInst = NEW CSMeshTexManager();

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
	inline static CSMeshTexManager* GetInst()
	{
		return m_lpInst;
	}

	// 読み込み
	LPDIRECT3DTEXTURE9 Load( const LPDIRECT3DDEVICE9 lp_device, const LPSTR path );

private:	// 非公開

	//////////////////////////
	// 型宣言
	//////////////////////////
	typedef std::pair< std::string, LPDIRECT3DTEXTURE9 >	PAIRTYPE;
	typedef std::map< std::string, LPDIRECT3DTEXTURE9 >		MAPTYPE;

	//////////////////////////
	// フィールド
	//////////////////////////
	static CSMeshTexManager*	m_lpInst;	// インスタンス
	MAPTYPE						m_TexMap;	// テクスチャマップ

	//////////////////////////
	// メソッド
	//////////////////////////

	// コンストラクタ
	CSMeshTexManager(){}

	// デストラクタ
	~CSMeshTexManager();
};

#endif	// __AKI_MESHTEXMANAGER_H__