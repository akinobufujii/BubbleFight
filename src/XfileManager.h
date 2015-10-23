#ifndef __AKI_XFILEMANAGER_H__
#define __AKI_XFILEMANAGER_H__

/////////////////////////////////
// �s���S�^�錾
/////////////////////////////////
class CXfile;

/////////////////////////////////
// Xfile�}�l�[�W���[
// (�V���O���g��)
/////////////////////////////////
class CSXfileManager : public boost::noncopyable
{
public:	// �O�����J

	/////////////////////////////////
	// ���\�b�h
	/////////////////////////////////

	// �쐬
	inline static bool Create()
	{
		// �����Ƃ������쐬
		if( m_lpInst == NULL )
		{
			m_lpInst = NEW CSXfileManager();
			return true;
		}

		return false;
	}

	// �C���X�^���X�l��
	inline static CSXfileManager* GetInst()
	{
		return m_lpInst;
	}

	// �j��
	inline static void Destroy()
	{
		SAFE_DELETE( m_lpInst );
	}

	// �ǂݍ���
	CXfile* Load( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR name, bool is_create_normals = false );

	// �ǂݍ��݃��X�g����ǂݍ���
	bool LoadFromTextList( const LPDIRECT3DDEVICE9 lpd3ddevice, LPCSTR list_pass );

	// ���l��
	CXfile*	GetData( LPCSTR name );

	// ���
	void Release( LPCSTR name );

	// ������X�g������
	void ReleaseFromTextList( const LPCSTR list_pass );

	// �S�ĉ��
	void AllRelease();

private:	// ����J

	/////////////////////////////////
	// �^�錾
	/////////////////////////////////
	typedef std::pair< std::string, CXfile* >	PAIRTYPE;
	typedef std::map< std::string, CXfile* >	MAPTYPE;

	/////////////////////////////////
	// �t�B�[���h
	/////////////////////////////////	
	static CSXfileManager*	m_lpInst;	// �C���X�^���X
	MAPTYPE					m_XFileMap;	// XFile�}�b�v

	/////////////////////////////////
	// ���\�b�h
	/////////////////////////////////

	// �R���X�g���N�^
	CSXfileManager();

	// �f�X�g���N�^
	~CSXfileManager();
};

#endif	// __AKI_XFILEMANAGER_H__