#ifndef __AKI_SOUNDMANAGER_H__
#define __AKI_SOUNDMANAGER_H__

///////////////////////////////////////
// サウンドマネージャ(シングルトン)
///////////////////////////////////////
class CSoundManager
{
public:

	///////////////////////////////////////
	// 定数
	///////////////////////////////////////
	CONSTANT LPSTR	UNION_PATH;	// 共用パス文字列

	///////////////////////////////////////
	// メソッド
	///////////////////////////////////////

	// 作成
	static bool Create();

	// 破壊
	static void Destroy();

	// インスタンス獲得
	static inline CSoundManager* GetInst()
	{
		return m_lpInst;
	}

	// 初期化
	bool Init( HWND hwnd );

	// 読み込み
	bool Load( LPCSTR path );

	// 読み込みリストから読み込み
	bool LoadFromTextList( LPCSTR list_path );

	// 解放
	void Release( LPCSTR path );

	// 全て開放
	void AllRelease();

	// 再生
	void Play( LPCSTR play_path, bool is_loop = false, bool is_synchronized = false, const GUID* lp_effect = NULL, DWORD set_pos = 0 );

	// 停止
	void Stop( LPCSTR play_path );

	// パンを設定する
	inline void SetPan( long pan )
	{
		m_lpSPrimary->SetPan( pan );
	}

	// ボリュームを設定する
	inline void SetVol( long vol )
	{
		m_lpSPrimary->SetVolume( vol );
	}

	// 元のパンに設定する
	inline void SetDefaultPan()
	{
		m_lpSPrimary->SetPan( m_Pan );
	}

	// 元のボリュームに設定する
	inline void SetDefaultVol()
	{
		m_lpSPrimary->SetVolume( m_Vol );
	}

	// 元のボリュームを返す
	inline long GetVol() const
	{
		return m_Vol;
	}

	// 元のパンを返す
	inline long GetPan() const
	{
		return m_Pan;
	}

private:	// 非公開

	///////////////////////////////////////
	// 型宣言
	///////////////////////////////////////
	typedef std::list<LPDIRECTSOUNDBUFFER8>				SOUNDBUFFERLIST;
	typedef std::pair< std::string, SOUNDBUFFERLIST* >	PAIRTYPE;
	typedef std::map< std::string, SOUNDBUFFERLIST* >	MAPTYPE;

	///////////////////////////////////////
	// フィールド
	///////////////////////////////////////
	static CSoundManager*	m_lpInst;		// インスタンス
	LPDIRECTSOUND8			m_lpDSound;		// DirectSoundオブジェクト
	LPDIRECTSOUNDBUFFER		m_lpSPrimary;	// プライマリ
	MAPTYPE					m_SecondMap;	// セカンダリバッファマップ
	long					m_Vol;			// ボリューム
	long					m_Pan;			// パン

	///////////////////////////////////////
	// メソッド
	///////////////////////////////////////

	// コンストラクタ
	CSoundManager();

	// デストラクタ
	~CSoundManager();

	// 読み込み
	bool LoadWave( LPDIRECTSOUNDBUFFER8 &pDSData, const LPCSTR fname );

	CSoundManager( CSoundManager& object );			// 封印
	CSoundManager& operator= ( CSoundManager& obj );// 封印
};

#endif // __AKI_SOUNDMANAGER_H__