#ifndef __AKI_TEXMANAGER_H__
#define __AKI_TEXMANAGER_H__

////////////////////////////////////
// �e�N�X�`���}�l�[�W���[
// (�V���O���g��)
////////////////////////////////////
class CSTexManager : public boost::noncopyable
{
public:	// �O�����J

	////////////////////////////////////
	// ���\�b�h
	////////////////////////////////////

	// ����
	static bool Create()
	{
		if( m_lpInst == NULL )
		{
			m_lpInst = NEW CSTexManager();
			return true;
		}

		return false;
	}

	// �C���X�^���X�l��
	static CSTexManager* GetInst()
	{
		return m_lpInst;
	}

	// �j��
	static void Destroy()
	{
		SAFE_DELETE( m_lpInst );
	}

	// �ǂݍ���
	bool Load( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR name, D3DCOLOR color_key = NULL );

	// �ǂݍ��݃��X�g����ǂݍ���
	bool LoadFromTextList( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR list_pass );

	// ���l��
	CTexture* GetData( const LPCSTR name );

	// ���
	void Release( const LPCSTR name );

	// ������X�g������
	void ReleaseFromTextList( const LPCSTR list_pass );

	// �S�ĉ��
	void AllRelease();

private:// ����J

	////////////////////////////////////
	// �^�錾
	////////////////////////////////////
	typedef std::pair< std::string, CTexture* >	PAIRTYPE;
	typedef std::map< std::string, CTexture* >	MAPTYPE;

	////////////////////////////////////
	// �t�B�[���h
	////////////////////////////////////
	static CSTexManager*	m_lpInst;	// �C���X�^���X
	MAPTYPE					m_TexMap;	// �e�N�X�`���}�b�v

	////////////////////////////////////
	// ���\�b�h
	////////////////////////////////////
	
	// �R���X�g���N�^
	CSTexManager();

	// �f�X�g���N�^
	~CSTexManager();
};

#endif // __AKI_TEXMANAGER_H__