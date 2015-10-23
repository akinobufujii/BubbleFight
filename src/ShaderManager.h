#ifndef __AKI_SHADERMANAGER_H__
#define __AKI_SHADERMANAGER_H__

////////////////////////////////
// �V�F�[�_�[�}�l�[�W���[
// (�V���O���g��)
////////////////////////////////
class CSShaderManager : public boost::noncopyable
{
public:	// �O�����J

	////////////////////////////////
	// ���\�b�h
	////////////////////////////////

	// ����
	inline static bool Create()
	{
		if( m_lpInst == NULL )
		{
			m_lpInst = NEW CSShaderManager();
			
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
	inline static CSShaderManager* GetInst() 
	{
		return m_lpInst;
	}

	// �ǂݍ���
	CEffect* Load( LPDIRECT3DDEVICE9 lp_device, LPCSTR path );

	// �t�@�C������ǂݍ���
	void LoadFromFile( LPDIRECT3DDEVICE9 lp_device, LPCSTR filepath );

	// �V�F�[�_�[�l��
	CEffect* GetData( LPCSTR path );

	// ���
	void Release( LPCSTR path );

	// �S�ĊJ��
	void AllRelease();

private:	// ����J
	////////////////////////////////
	// �^�錾
	////////////////////////////////
	typedef std::pair< std::string, CEffect* >	PAIRTYPE;
	typedef std::map< std::string, CEffect* >	MAPTYPE;

	////////////////////////////////
	// �t�B�[���h
	////////////////////////////////
	static CSShaderManager*	m_lpInst;	// �C���X�^���X

	MAPTYPE					m_ShaderMap;	// �V�F�[�_�}�b�v

	////////////////////////////////
	// ���\�b�h
	////////////////////////////////

	// �R���X�g���N�^
	CSShaderManager();

	// �f�X�g���N�^
	~CSShaderManager(void);
};

#endif	// __AKI_SHADERMANAGER_H__