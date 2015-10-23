#ifndef __AKI_NORMALENEMY_H__
#define __AKI_NORMALENEMY_H__

namespace Enemy
{
	////////////////////////////////
	// ���ʂ̓G�N���X
	////////////////////////////////
	class CNormal : public CBase
	{
	public:	// �O�����J

		////////////////////////////////
		// �萔
		////////////////////////////////
		CONSTANT float	SPEED;				// ����
		CONSTANT float	ROT_SPEED;			// ��]���x	
		CONSTANT ULONG	SCORE;				// �X�R�A
		CONSTANT LPSTR	ENEMY_MESH_PASS;	// �G�̃��b�V���p�X
		CONSTANT DWORD	AWAY_TIME;			// �U�����Ă���܂��U���Ɉڂ�܂ł̎���
		CONSTANT DWORD	CHANGE_ATTACK_TIME;	// �U���Ɉڂ鎞��

		////////////////////////////////
		// ���\�b�h
		////////////////////////////////

		// �R���X�g���N�^
		CNormal( float x, float y, float z );

		// �f�X�g���N�^
		~CNormal();

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
		void Draw( const LPDIRECT3DDEVICE9 lpdevice, bool is_shader);

		//////////////////////////////////////////////////
		//	�E�֐���	DrawEffect
		//	�E����		�G�t�F�N�g�̕`����s��
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

		////////////////////////////////
		// �t�B�[���h
		////////////////////////////////
		BYTE		m_AnimeCnt;			// �A�j���[�V�����J�E���^
		CFinTail	m_Tail;				// ������
		CXfile*		m_lpMesh;			// �����̃��b�V��
		bool		m_IsAway;			// �����Ă��邩
		bool		m_IsAttack;			// �U�����悤�Ƃ��Ă��邩
		DWORD		m_StartAwayTime;	// �����n�߂����ԕۑ��p
		DWORD		m_ViewSaveTime;		// ��ʂɉf���Ă鎞�ԕۑ��p

		////////////////////////////////
		// ���\�b�h
		////////////////////////////////

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateAway
		//	�E����		������X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateAway();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateDrive
		//	�E����		�ǂ�������X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateDrive();
	};
}

#endif // __AKI_NORMALENEMY_H__