#ifndef __AKI_BUBBLE_H__
#define __AKI_BUBBLE_H__

///////////////////////////////
// �A�N���X
///////////////////////////////
class CBubble : virtual public IDoubleDispatch
{
public:	// �O�����J

	///////////////////////////////
	// �萔
	///////////////////////////////
	CONSTANT LPSTR	BUBBLE_MESH_PASS;	// �A�̃��b�V���p�X
	CONSTANT USHORT	ERASE_TIME;			// �����鎞��
	CONSTANT BYTE	BUBBLE_RAND;		// �Ԃ��W��
	CONSTANT float	SPEED;				// ����
	CONSTANT float	SPEED_MAX;			// �ő呬�x
	CONSTANT float	SCALE;				// �傫��

	CONSTANT int	BURST_WIDTH;			// �����摜�A��
	CONSTANT int	BURST_HEIGHT;			// �����摜�A����
	CONSTANT int	BURST_ANIME_SIZE;		// �����摜�A�A�j���P�̑傫��
	CONSTANT int	BURST_ANIME_WIDTH_NUM;	// �����摜�A���A�j����
	CONSTANT int	BURST_ANIME_HEIGHT_NUM;	// �����摜�A�����A�j����
	CONSTANT int	BURST_ANIME_NUM;		// �����摜�A�A�j������

	///////////////////////////////
	// �t���O�萔
	///////////////////////////////
	CONSTANT BYTE	ERASE;		// ������
	CONSTANT BYTE	BREAK;		// �j��
	CONSTANT BYTE	ENEMY_HIT;	// �G�ɓ������Ă���

	///////////////////////////////
	// ���\�b�h
	///////////////////////////////

	// �R���X�g���N�^
	CBubble();

	// �f�X�g���N�^
	~CBubble();

	//////////////////////////////////////////////////
	//	�E�֐���	Update
	//	�E����		�X�V���s��
	//	�E����		�Ȃ�
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Update();

	//////////////////////////////////////////////////
	//	�E�֐���	Draw
	//	�E����		�`����s��
	//	�E����		
	//		- lpdevice	�f�o�C�X
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Draw( const LPDIRECT3DDEVICE9 lpdevice );

	//////////////////////////////////////////////////
	//	�E�֐���	SetMatrix
	//	�E����		�n���ꂽ�s��ɑ΂���
	//				�����΂炵�Ď����̍s��ɂ���
	//	�E����		
	//		- mat	�o���ʒu�̍s��
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void SetMatrix( const D3DXMATRIX& mat );

	//////////////////////////////////////////////////
	//	�E�֐���	BreakBubble
	//	�E����		���̖A��j�󂷂�
	//	�E����		�Ȃ�
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void BreakBubble()
	{
		m_Flg |= BREAK;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetFlg
	//	�E����		���(�t���O)���l������
	//	�E����		�Ȃ�
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline BYTE GetFlg() const
	{
		return m_Flg;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetMatrix
	//	�E����		�s���Ԃ�
	//	�E����		�Ȃ�
	//	�E�߂�l	�s����|�C���^�ŕԂ�
	//////////////////////////////////////////////////
	inline const D3DXMATRIX* GetMatrix() const
	{
		return &m_mMat;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetPos
	//	�E����		�ʒu���l������
	//	�E����		�Ȃ�
	//	�E�߂�l	�ʒu��Ԃ�
	//////////////////////////////////////////////////
	inline D3DXVECTOR3 GetPos() const 
	{
		return m_mMat.GetPos();
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetSpeed
	//	�E����		���݂̑������l������
	//	�E����		�Ȃ�
	//	�E�߂�l	������Ԃ�
	//////////////////////////////////////////////////
	inline float GetSpeed() const
	{
		return m_NowSpeed;
	}


	//////////////////////////////////////////////////
	//	�E�֐���	Dispatch
	//	�E����		�����Ă����I�u�W�F�N�g�Ɏ����𑗂����
	//	�E����		
	//		- object	�s�����N�����ė~�����I�u�W�F�N�g
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void Dispatch( IDoubleDispatch* object )
	{
		object->Action( *this );
	}

	//////////////////////////////////////////////////
	//	�E�֐���	Action
	//	�E����		�G�ɑ΂���s��(Dispatch����̂݌Ă΂��)
	//	�E����		
	//		- object	�s�����N�����Ώۂ̃I�u�W�F�N�g
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Action( Enemy::CBase& object );

private:	// ����J

	///////////////////////////////
	// �t�B�[���h
	///////////////////////////////
	CMatrix				m_mMat;		// �s��
	BYTE				m_Flg;		// �t���O
	float				m_NowSpeed;	// ���̑���
	DWORD				m_StartTime;// ���ꂽ����
	BYTE				m_AnimeCnt;	// �A�j���J�E���^
	CBoard				m_Board;	// �|���S��
	CPointSpriteManager	m_Points;	// �|�C���g�X�v���C�g
	CXfile*				m_lpMesh;	// ���b�V��(���L)
};

#endif // __AKI_BUBBLE_H__