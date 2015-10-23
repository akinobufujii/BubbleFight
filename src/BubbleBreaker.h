#ifndef __AKI_BUBBLEBREAKER_H__
#define __AKI_BUBBLEBREAKER_H__

namespace Enemy
{
	/////////////////////////////////////
	// �A�ɕ�܂�Ă���G�̖A��
	// ��(�ƌ������͋z���Ƃ�)�G
	/////////////////////////////////////
	class CBubbleBreaker : public CBase
	{
	public:	// �O�����J

		/////////////////////////////////////
		// �萔
		/////////////////////////////////////
		CONSTANT float	SPEED;				// ����
		CONSTANT ULONG	SCORE;				// �X�R�A
		CONSTANT LPSTR	MESHPASS_ENEMY;		// ���b�V���p�X
		CONSTANT DWORD	MOVE_CHANGETIME;	// ������؂�ւ��鎞��(�~���b�P��)
		CONSTANT float	DIRCHANGE_ANIMETIME;// �����ؑւ̃A�j���[�V�������Z����
		CONSTANT BYTE	ROT_SPEED;			// ��]���x
		CONSTANT USHORT	AWAY_TIME;			// �����鎞��
		CONSTANT USHORT	CHANGE_ATTACK_TIME;	// �U���ɐ؂�ւ��鎞��	

		/////////////////////////////////////
		// ���\�b�h
		/////////////////////////////////////
		
		// �R���X�g���N�^
		CBubbleBreaker( float x, float y, float z );

		// �f�X�g���N�^
		~CBubbleBreaker();

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
		//	�E����		�G�ɑ΂��Ă̍s�����N����
		//				Dispatch����Ă΂��
		//	�E����		�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Action( Enemy::CBase& object );

		//////////////////////////////////////////////////
		//	�E�֐���	Action
		//	�E����		�v���C���[�ɑ΂��Ă̍s�����N����
		//				Dispatch����Ă΂��
		//	�E����		�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Action( Player::CDolphin& object );

		//////////////////////////////////////////////////
		//	�E�֐���	SetDirEnemy
		//	�E����		���̓G���ǔ�����G�����肷��
		//				�_�E���L���X�g�ŌĂԂ��Ƃ�O��Ƃ��Ă���
		//	�E����		
		//		- enemy	�ǔ������G�̃|�C���^
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void SetDirEnemy( Enemy::CBase* enemy );

	private:	// ����J

		/////////////////////////////////////
		// �t�B�[���h
		/////////////////////////////////////
		CFinTail		m_Tail;				// �K���i����j
		CXfile*			m_lpMesh;			// ���b�V��
		bool			m_IsDirChange;		// �����]�����邩
		float			m_NowAnimetionCnt;	// �A�j���[�V�����J�E���^(0.f�`1.f)
		DWORD			m_SaveTime;			// ���ԕۑ��p
		D3DXMATRIX		m_mSave;			// �ŏI�A�j���[�V�����ۑ��s��
		float			m_NowSpeed;			// ���̑���
		Enemy::CBase*	m_lpTargetEnemy;	// �ڕW�ɂ��Ă���G
		bool			m_IsAway;			// �����邩�ǂ���
		bool			m_IsAttack;			// �U�������ǂ���
		DWORD			m_AwayStartTime;	// �����n�߂����ԕۑ��p
		DWORD			m_SaveDriveTime;	// �ǂ��n�߂����ԋL�^

		/////////////////////////////////////
		// ���\�b�h
		/////////////////////////////////////

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateAway
		//	�E����		������X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateAway();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateMove
		//	�E����		�����܂��X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateMove();
	};
}

#endif	// __AKI_BUBBLEBREAKER_H__