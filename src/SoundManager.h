#ifndef __AKI_SOUNDMANAGER_H__
#define __AKI_SOUNDMANAGER_H__

///////////////////////////////////////
// �T�E���h�}�l�[�W��(�V���O���g��)
///////////////////////////////////////
class CSoundManager
{
public:

	///////////////////////////////////////
	// �萔
	///////////////////////////////////////
	CONSTANT LPSTR	UNION_PATH;	// ���p�p�X������

	///////////////////////////////////////
	// ���\�b�h
	///////////////////////////////////////

	// �쐬
	static bool Create();

	// �j��
	static void Destroy();

	// �C���X�^���X�l��
	static inline CSoundManager* GetInst()
	{
		return m_lpInst;
	}

	// ������
	bool Init( HWND hwnd );

	// �ǂݍ���
	bool Load( LPCSTR path );

	// �ǂݍ��݃��X�g����ǂݍ���
	bool LoadFromTextList( LPCSTR list_path );

	// ���
	void Release( LPCSTR path );

	// �S�ĊJ��
	void AllRelease();

	// �Đ�
	void Play( LPCSTR play_path, bool is_loop = false, bool is_synchronized = false, const GUID* lp_effect = NULL, DWORD set_pos = 0 );

	// ��~
	void Stop( LPCSTR play_path );

	// �p����ݒ肷��
	inline void SetPan( long pan )
	{
		m_lpSPrimary->SetPan( pan );
	}

	// �{�����[����ݒ肷��
	inline void SetVol( long vol )
	{
		m_lpSPrimary->SetVolume( vol );
	}

	// ���̃p���ɐݒ肷��
	inline void SetDefaultPan()
	{
		m_lpSPrimary->SetPan( m_Pan );
	}

	// ���̃{�����[���ɐݒ肷��
	inline void SetDefaultVol()
	{
		m_lpSPrimary->SetVolume( m_Vol );
	}

	// ���̃{�����[����Ԃ�
	inline long GetVol() const
	{
		return m_Vol;
	}

	// ���̃p����Ԃ�
	inline long GetPan() const
	{
		return m_Pan;
	}

private:	// ����J

	///////////////////////////////////////
	// �^�錾
	///////////////////////////////////////
	typedef std::list<LPDIRECTSOUNDBUFFER8>				SOUNDBUFFERLIST;
	typedef std::pair< std::string, SOUNDBUFFERLIST* >	PAIRTYPE;
	typedef std::map< std::string, SOUNDBUFFERLIST* >	MAPTYPE;

	///////////////////////////////////////
	// �t�B�[���h
	///////////////////////////////////////
	static CSoundManager*	m_lpInst;		// �C���X�^���X
	LPDIRECTSOUND8			m_lpDSound;		// DirectSound�I�u�W�F�N�g
	LPDIRECTSOUNDBUFFER		m_lpSPrimary;	// �v���C�}��
	MAPTYPE					m_SecondMap;	// �Z�J���_���o�b�t�@�}�b�v
	long					m_Vol;			// �{�����[��
	long					m_Pan;			// �p��

	///////////////////////////////////////
	// ���\�b�h
	///////////////////////////////////////

	// �R���X�g���N�^
	CSoundManager();

	// �f�X�g���N�^
	~CSoundManager();

	// �ǂݍ���
	bool LoadWave( LPDIRECTSOUNDBUFFER8 &pDSData, const LPCSTR fname );

	CSoundManager( CSoundManager& object );			// ����
	CSoundManager& operator= ( CSoundManager& obj );// ����
};

#endif // __AKI_SOUNDMANAGER_H__