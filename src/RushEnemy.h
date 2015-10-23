#ifndef __AKI_RUSHENEMY_H__
#define __AKI_RUSHENEMY_H__

namespace Enemy
{
	/////////////////////////////////
	// �ːi����G
	// ��Ƀ_���[�W�I�u�W�F�N�g��_��
	/////////////////////////////////
	class CRush : public CBase
	{
	public:	// �O�����J

		/////////////////////////////////
		// �萔
		/////////////////////////////////
		CONSTANT ULONG	SCORE;				// ���̓G�̃X�R�A
		CONSTANT float	SPEED;				// ����
		CONSTANT float	TAIL_ROT_SPEED;		// �K���̑���
		CONSTANT float	TAIL_ROT_MAX;		// �K���̍ő��]�p�x
		CONSTANT float	RETURN_TAIL_SPEED;	// �K���̖߂鑬��
		CONSTANT USHORT	CHANGE_MOVE_TIME;	// �s����؂�ւ���
		CONSTANT LPSTR	ENEMY_MESH_PASS;	// �G�̃��b�V���p�X

		/////////////////////////////////
		// ���\�b�h
		/////////////////////////////////
		
		// �R���X�g���N�^
		CRush( float x, float y, float z );
		
		// �f�X�g���N�^
		~CRush();

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
		//		- is_shader	�v���O���}�u���V�F�[�_�`�悷�邩�ǂ���
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Draw( const LPDIRECT3DDEVICE9 lpdevice, bool is_shader );

		//////////////////////////////////////////////////
		//	�E�֐���	DrawEffect
		//	�E����		�`����s��
		//	�E����		
		//		- lpdevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawEffect( const LPDIRECT3DDEVICE9 lpdevice );

		//////////////////////////////////////////////////
		//	�E�֐���	GetScore
		//	�E����		���̓G�̃X�R�A���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���̓G�̃X�R�A��Ԃ�
		//////////////////////////////////////////////////
		inline ULONG GetScore() const
		{
			return SCORE;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetMesh
		//	�E����		���b�V�������l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���b�V������Ԃ�
		//////////////////////////////////////////////////
		inline const CXfile* GetMesh() const
		{
			return m_lpMesh;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	Dispatch
		//	�E����		�����̏��𑗂��Ă���
		//				�I�u�W�F�N�g�ɑ������
		//	�E����		
		//		- object	���̃N���X�̏��𑗂����
		//					�I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Dispatch( IDoubleDispatch* object )
		{
			object->Action( *this );
		}

		//////////////////////////////////////////////////
		//	�E�֐���	Action
		//	�E����		�v���C���[�ɑ΂��Ă̍s�����N����
		//				Dispatch����Ă΂��
		//	�E����		�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Action( Player::CDolphin& object );
	
		//////////////////////////////////////////////////
		//	�E�֐���	Action
		//	�E����		�P���I�u�W�F�N�g�ɑ΂��Ă̍s�����N����
		//				Dispatch����Ă΂��
		//	�E����		�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Action( CDamageObjectBase& object );
		
	private:	// ����J

		/////////////////////////////////
		// �t�B�[���h
		/////////////////////////////////
		DWORD				m_SaveTime;		// ���ԋL���p�ϐ�
		CFinTail			m_Tail;			// ������
		CXfile*				m_lpMesh;		// �����̃��b�V��
		float				m_NowTailRot;	// �K���̉�]�p�x
	};
}

#endif // __AKI_RUSHENEMY_H__