#ifndef __AKI_OBJECTFISH_H__
#define __AKI_OBJECTFISH_H__

//////////////////////////////
// �P�Ȃ鋛(���h���p)
//////////////////////////////
class CObjectFish
{
public:	// �O�����J

	//////////////////////////////
	// �^�錾
	//////////////////////////////
	typedef BYTE	FLGTYPE;	// �t���O�^

	//////////////////////////////
	// �萔
	//////////////////////////////
	CONSTANT float	MOVE_SPEED;		// �ړ����x
	CONSTANT float	SIZE;			// �傫��
	CONSTANT float	TEXPOS_WIDTH;	// �e�N�X�`���̕�
	CONSTANT float	TEXPOS_HEIGHT;	// �e�N�X�`���̍���
	CONSTANT BYTE	NODE_RANGE;		// �q�͈̔�
	CONSTANT BYTE	KIND_NUM;		// ��ލő吔

	enum
	{
		NODE_MAX = 10	// �q�̍ő吔
	};

	//////////////////////////////
	// �t���O�萔
	//////////////////////////////
	CONSTANT FLGTYPE	FLG_TURN;	// �U��Ԃ���

	//////////////////////////////
	// ���\�b�h
	//////////////////////////////

	// �R���X�g���N�^
	CObjectFish();

	// �f�X�g���N�^
	~CObjectFish();

	//////////////////////////////////////////////////
	//	�E�֐���	Update
	//	�E����		�X�V���s��
	//	�E����		
	//		- is_collision	�Փ˔�����s�����ǂ���
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Update( bool is_collision = true );

	//////////////////////////////////////////////////
	//	�E�֐���	Draw
	//	�E����		�`����s��
	//	�E����		
	//		- lp_device	�f�o�C�X
	//		- mat		�e��������s��
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Draw( const LPDIRECT3DDEVICE9 lp_device, const D3DXMATRIX& mat );

	//////////////////////////////////////////////////
	//	�E�֐���	SetPos
	//	�E����		�o���ʒu��ݒ肷��
	//	�E����		
	//		- pos	�o���ʒu
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void SetPos( const D3DXVECTOR3& pos );
	
	//////////////////////////////////////////////////
	//	�E�֐���	SetRotY
	//	�E����		Y����]��ݒ肷��
	//	�E����		
	//		- rot	��]��
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void SetRotY( const float rot );	

	//////////////////////////////////////////////////
	//	�E�֐���	GetMatrix
	//	�E����		�s����l��
	//	�E����		�Ȃ�
	//	�E�߂�l	�s���Ԃ�
	//////////////////////////////////////////////////
	inline D3DXMATRIX GetMatrix() const 
	{
		return m_mMat;
	}

private:	// ����J

	//////////////////////////////
	// �t�B�[���h
	//////////////////////////////
	CBoard	m_Board;	// �|���S��
	CMatrix	m_mMat;		// �s��
	FLGTYPE	m_Flg;		// �t���O

	CMatrix m_NodeMat[ NODE_MAX ];	
};

#endif	// __AKI_OBJECTFISH_H__