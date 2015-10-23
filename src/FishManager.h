#ifndef	__AKI_FISHMANAGER_H__
#define __AKI_FISHMANAGER_H__

//////////////////////////////////////////////////
// �s���S�^�錾
//////////////////////////////////////////////////
class CObjectFish;
class CDamageObjectBase;
class CStoneManager;

//////////////////////////////////////////////////
// ���}�l�[�W���[(���̊Ǘ�)
//////////////////////////////////////////////////
class CFishManager
{
public:	// �O�����J

	//////////////////////////////////////////////////
	// �^�錾
	//////////////////////////////////////////////////
	typedef std::list<CObjectFish*>			OBJECTFISHLIST;	// �P�Ȃ����̋����X�g�^
	typedef	std::list<CDamageObjectBase*>	DAMAGEFISHLIST;	// �����߂��鋛�V�X�g�^

	//////////////////////////////////////////////////
	// �萔
	//////////////////////////////////////////////////
	enum OBJECTKIND
	{
		KIND_FISH,
		KINDMAX
	};

	//////////////////////////////////////////////////
	// ���\�b�h
	//////////////////////////////////////////////////

	// �R���X�g���N�^
	CFishManager();

	// �f�X�g���N�^
	~CFishManager();

	//////////////////////////////////////////////////
	//	�E�֐���	Update
	//	�E����		�S�Ă̍X�V���s��
	//	�E����		
	//		- is_collision	�Փ˔�����s�����ǂ���
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Update( bool is_collision = true );

	//////////////////////////////////////////////////
	//	�E�֐���	DrawObjectFish
	//	�E����		�P�Ȃ�I�u�W�F�N�g�̕`����s��
	//	�E����		
	//		- lp_device	�f�o�C�X
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void DrawObjectFish( const LPDIRECT3DDEVICE9 lp_device );

	//////////////////////////////////////////////////
	//	�E�֐���	DrawDamageFish
	//	�E����		�P����I�u�W�F�N�g�̕`����s��
	//	�E����		
	//		- lp_device	�f�o�C�X
	//		- is_shader	�v���O���}�u���V�F�[�_�ŕ`�悷�邩�ǂ���
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void DrawDamageFish( const LPDIRECT3DDEVICE9 lp_device, bool is_shader );

	//////////////////////////////////////////////////
	//	�E�֐���	SetNumAndLimit
	//	�E����		�P�Ȃ�I�u�W�F�N�g�̎Y�ݏo������
	//				�͈͂�y����]�������߂�
	//				�ǉ������Ő��ݏo��
	//	�E����		
	//		- num		���ݏo����
	//		- pos_limit	�����͈�
	//		- rot		��]�p�x(-1��n���ƁA�����_���ɉ�]����)
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void SetNumAndLimit( const int num, const D3DXVECTOR3& pos_limit, const int rot = -1 );
	
	//////////////////////////////////////////////////
	//	�E�֐���	LoadDamageFish
	//	�E����		�t�@�C������A
	//				�P����I�u�W�F�N�g�̓ǂݍ��݂��s��
	//	�E����		
	//		- load_path	�ǂݍ��ރt�@�C���̃p�X
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void LoadDamageFish( LPCSTR load_path );

	//////////////////////////////////////////////////
	//	�E�֐���	AllRelease
	//	�E����		�S�ẴI�u�W�F�N�g�̍폜���s��
	//	�E����		�Ȃ�
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void AllRelease();

	//////////////////////////////////////////////////
	//	�E�֐���	SetRot
	//	�E����		�P�Ȃ�I�u�W�F�N�g�̉�]�p�x��ݒ肷��
	//	�E����		
	//		- rot	��]�p�x
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void SetRot( const int rot );

	//////////////////////////////////////////////////
	//	�E�֐���	SetLimit
	//	�E����		�P�Ȃ�I�u�W�F�N�g�̈ړ�������ݒ肷��
	//	�E����		
	//		- limit	�͈�
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetLimit( const D3DXVECTOR3& limit )
	{
		m_MoveLimit = limit;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetMatrix
	//	�E����		�P�Ȃ�I�u�W�F�N�g�ɉe������
	//				�s���ݒ肷��
	//	�E����		
	//		- rot	��]�p�x
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetMatrix( const D3DXMATRIX& mat )
	{
		m_Mat = mat;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	IsAllOut
	//	�E����		�P�Ȃ�I�u�W�F�N�g��
	//				�����䂩����������ǂ������ׂ�
	//	�E����		�Ȃ�
	//	�E�߂�l	�S�Ď����䂩������Ă����true
	//				�����łȂ����false��Ԃ�
	//////////////////////////////////////////////////
	bool IsAllOut();

	//////////////////////////////////////////////////
	//	�E�֐���	GetDamageList
	//	�E����		�P����I�u�W�F�N�g���X�g���l��
	//	�E����		�Ȃ�
	//	�E�߂�l	�P����I�u�W�F�N�g���X�g��Ԃ�
	//////////////////////////////////////////////////
	inline DAMAGEFISHLIST* GetDamageList()
	{
		return &m_DamageObjectList;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetStoneManager
	//	�E����		�Փ˔�����s����}�l�[�W���[��ݒ肷��
	//	�E����		�Ȃ�
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline static void SetStoneManager( CStoneManager* object )
	{
		m_lpStones = object;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetStoneManager
	//	�E����		�ݒ肵�Ă����}�l�[�W���[���l��
	//	�E����		�Ȃ�
	//	�E�߂�l	��}�l�[�W���[��Ԃ�
	//////////////////////////////////////////////////
	inline static CStoneManager* GetStoneManager()
	{
		return m_lpStones;
	}

private:	// ����J

	//////////////////////////////////////////////////
	// �t�B�[���h
	//////////////////////////////////////////////////
	OBJECTFISHLIST			m_FishList;			// �����X�g
	DAMAGEFISHLIST			m_DamageObjectList;	// �P���郊�X�g
	CVector					m_MoveLimit;		// �ړ�����
	CMatrix					m_Mat;				// �s��(�S�Ă̋��ɉe��)
	static CStoneManager*	m_lpStones;			// ��		
};

#endif	// __AKI_FISHMANAGER_H__