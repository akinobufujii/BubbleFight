#ifndef __AKI_SYNCLO_H__
#define __AKI_SYNCLO_H__

//////////////////////////////
// 同期処理用クラス
// シングルトン
//////////////////////////////
class CSSyncro : public boost::noncopyable
{
public:	// 外部公開
	
	//////////////////////////////
	// 静的メソッド
	//////////////////////////////
	// 生成
	inline static bool Create()
	{
		if( m_lpInst == NULL )
		{
			m_lpInst = NEW CSSyncro();

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
	inline static CSSyncro* GetInst()
	{
		return m_lpInst;
	}

	//////////////////////////////
	// メソッド
	//////////////////////////////
	
	// 同期開始
	inline void Lock()
	{
		EnterCriticalSection( &m_Section );
	}

	// 同期終了
	inline void UnLock()
	{
		LeaveCriticalSection( &m_Section );
	}

private:	// 非公開

	//////////////////////////////
	// フィールド
	//////////////////////////////	
	static CSSyncro*	m_lpInst;	// インスタンス
	CRITICAL_SECTION	m_Section;	// クリティカルセクション

	//////////////////////////////
	// メソッド
	//////////////////////////////

	// コンストラクタ
	CSSyncro();

	// デストラクタ
	~CSSyncro();
};

#endif // __AKI_SYNCLO_H__