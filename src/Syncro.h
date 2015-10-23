#ifndef __AKI_SYNCLO_H__
#define __AKI_SYNCLO_H__

//////////////////////////////
// ���������p�N���X
// �V���O���g��
//////////////////////////////
class CSSyncro : public boost::noncopyable
{
public:	// �O�����J
	
	//////////////////////////////
	// �ÓI���\�b�h
	//////////////////////////////
	// ����
	inline static bool Create()
	{
		if( m_lpInst == NULL )
		{
			m_lpInst = NEW CSSyncro();

			return true;
		}

		return false;
	}

	// �j��
	inline static void Destroy()
	{
		SAFE_DELETE( m_lpInst );
	}

	// �C���X�^���X�l��
	inline static CSSyncro* GetInst()
	{
		return m_lpInst;
	}

	//////////////////////////////
	// ���\�b�h
	//////////////////////////////
	
	// �����J�n
	inline void Lock()
	{
		EnterCriticalSection( &m_Section );
	}

	// �����I��
	inline void UnLock()
	{
		LeaveCriticalSection( &m_Section );
	}

private:	// ����J

	//////////////////////////////
	// �t�B�[���h
	//////////////////////////////	
	static CSSyncro*	m_lpInst;	// �C���X�^���X
	CRITICAL_SECTION	m_Section;	// �N���e�B�J���Z�N�V����

	//////////////////////////////
	// ���\�b�h
	//////////////////////////////

	// �R���X�g���N�^
	CSSyncro();

	// �f�X�g���N�^
	~CSSyncro();
};

#endif // __AKI_SYNCLO_H__