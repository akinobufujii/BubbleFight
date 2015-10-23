#ifndef __AKI_BUBBLERING_H__
#define __AKI_BUBBLERING_H__

//////////////////////////////////////////////////
// �o�u�������O
// (��Ɏ��@���g���e)
//////////////////////////////////////////////////
namespace Shot
{
	class CBubbleRing : public CBase
	{
	public:
		//////////////////////////////////////////////////
		// �萔
		//////////////////////////////////////////////////
		CONSTANT float	SPEED_MAX;			// �������
		CONSTANT float	POWER_MAX;			// �������
		CONSTANT float	POWER_MIN;			// ��������
		CONSTANT float	BIG_SPEED;			// �傫���Ȃ�X�s�[�h
		CONSTANT BYTE	POINT_NUM;			// �|�C���g�X�v���C�g��
		CONSTANT USHORT	BUBBLE_ERASE_TIME;	// �����鎞��
		CONSTANT LPSTR	MESH_PASS;			// ���b�V���p�X

		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////

		// �R���X�g���N�^
		CBubbleRing();
		CBubbleRing( const D3DXMATRIX* mat, const float power );

		// �f�X�g���N�^
		~CBubbleRing();

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
		//		- lpDevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Draw( const LPDIRECT3DDEVICE9 lpdevice );

		//////////////////////////////////////////////////
		//	�E�֐���	DrawEffect
		//	�E����		�G�t�F�N�g�`����s��
		//	�E����		
		//		- lpDevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawEffect( const LPDIRECT3DDEVICE9 lpdevice );

		//////////////////////////////////////////////////
		//	�E�֐���	GetPos
		//	�E����		���݂̈ʒu�����l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���݂̈ʒu��Ԃ�Ԃ�
		//////////////////////////////////////////////////
		inline D3DXVECTOR3 GetPos() const
		{
			return m_mMat.GetPos() + m_lpMesh->GetCenter();
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetRadius
		//	�E����		���b�V���̔��a�{�����𑫂������̂��l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���b�V���̔��a�{�����𑫂������̂�Ԃ�
		//////////////////////////////////////////////////
		inline float GetRadius() const
		{
			return m_lpMesh->GetRadius() + m_Power;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetPower
		//	�E����		���̃I�u�W�F�N�g�̋������l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���̃I�u�W�F�N�g�̋�����Ԃ�
		//////////////////////////////////////////////////
		inline float GetPower() const 
		{
			return m_RingMax;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	SetPower
		//	�E����		���̃I�u�W�F�N�g�̋�����ݒ肷��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		inline void SetPower( const float power )
		{
			m_RingMax = (power < POWER_MIN)? POWER_MIN : power;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	Dispatch
		//	�E����		�����Ă����I�u�W�F�N�g�Ɏ����𑗂����
		//	�E����		
		//		- object	�s�����N�����ė~�����I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Dispatch( IDoubleDispatch* object);

		//////////////////////////////////////////////////
		//	�E�֐���	Action
		//	�E����		�G�ɑ΂���s��(Dispatch����̂݌Ă΂��)
		//	�E����		
		//		- object	�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Action( Enemy::CBase& object );

		//////////////////////////////////////////////////
		//	�E�֐���	Action
		//	�E����		�v���C���[�ɑ΂���s��(Dispatch����̂݌Ă΂��)
		//	�E����		
		//		- object	�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Action( Player::CDolphin& object );

		//////////////////////////////////////////////////
		//	�E�֐���	Action
		//	�E����		�{�X�ɑ΂���s��(Dispatch����̂݌Ă΂��)
		//	�E����		
		//		- object	�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Action( Boss::CBase& object );

		//////////////////////////////////////////////////
		//	�E�֐���	Action
		//	�E����		�e�ɑ΂���s��(Dispatch����̂݌Ă΂��)
		//	�E����		
		//		- object	�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Action( Shot::CBase& object );

	private:	// ����J

		//////////////////////////////////////////////////
		// �t�B�[���h
		//////////////////////////////////////////////////
		DWORD				m_StartTime;		// ���ꂽ����
		BYTE				m_CreateBubbleNum;	// �A������
		CPointSpriteManager	m_Points;			// �|�C���g�X�v���C�g
		float				m_Power;			// ��(���a�ɂ���p)
		float				m_Speed;			// ����
		float				m_RingMax;			// �ő�X�P�[��
		CXfile*				m_lpMesh;			// ���b�V��
	};
}

#endif // __AKI_BUBBLERING_H__