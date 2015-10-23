#ifndef __AKI_MESHTEXMANAGER_H__
#define __AKI_MESHTEXMANAGER_H__

//////////////////////////
// ���b�V����p
//	�e�N�X�`���}�l�[�W���[
// (�V���O���g��)
//////////////////////////
class CSMeshTexManager : public boost::noncopyable
{
public:	// �O�����J

	//////////////////////////
	// ���\�b�h
	//////////////////////////

	// ����
	inline static bool Create()
	{
		if( m_lpInst == NULL )
		{
			m_lpInst = NEW CSMeshTexManager();

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
	inline static CSMeshTexManager* GetInst()
	{
		return m_lpInst;
	}

	// �ǂݍ���
	LPDIRECT3DTEXTURE9 Load( const LPDIRECT3DDEVICE9 lp_device, const LPSTR path );

private:	// ����J

	//////////////////////////
	// �^�錾
	//////////////////////////
	typedef std::pair< std::string, LPDIRECT3DTEXTURE9 >	PAIRTYPE;
	typedef std::map< std::string, LPDIRECT3DTEXTURE9 >		MAPTYPE;

	//////////////////////////
	// �t�B�[���h
	//////////////////////////
	static CSMeshTexManager*	m_lpInst;	// �C���X�^���X
	MAPTYPE						m_TexMap;	// �e�N�X�`���}�b�v

	//////////////////////////
	// ���\�b�h
	//////////////////////////

	// �R���X�g���N�^
	CSMeshTexManager(){}

	// �f�X�g���N�^
	~CSMeshTexManager();
};

#endif	// __AKI_MESHTEXMANAGER_H__