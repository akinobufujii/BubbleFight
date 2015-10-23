#ifndef __AKI_PLAYERBASE_H__
#define __AKI_PLAYERBASE_H__

//////////////////////////////////////////////////
// �s���S�^�錾
//////////////////////////////////////////////////
class CStoneManager;
class CSandSpray;

namespace Player
{
	//////////////////////////////////////////////////
	// �v���C���[(�C���J)
	//////////////////////////////////////////////////
	class CDolphin : virtual public IDoubleDispatch
	{
	public:	// �O�����J

		//////////////////////////////////////////////////
		// �^�錾
		//////////////////////////////////////////////////
		typedef USHORT	FLGTYPE;

		//////////////////////////////////////////////////
		// �萔
		//////////////////////////////////////////////////
		CONSTANT float		SPEED;						// �ړ����x
		CONSTANT float		SPEED_MAX;					// �ő呬�x
		CONSTANT float		EVASION_MOVE_SPEED;			// ����ړ����x
		CONSTANT float		POWERCHARGE_SPEED;			// �p���[�𒙂߂鑬�x
		CONSTANT float		HITPOWER_PLUS;				// �G�Ƃ��ɓ����������̋���
		CONSTANT float		RECOVERY_SPEED_LINE;		// ���ʂɔ�яo��Ƃ��̑�������
		CONSTANT float		POINTER_ANIME_RATE;			// �A�j��������
		CONSTANT float		CHARGE_ANIME_RATE;			// ������
		CONSTANT float		CHARGEMAX_SCALE;			// �ő嗭�ߎ��̑傫��
		CONSTANT float		CHARGESCALE_RATE;			// ���ߒ��̑傫���Ȃ���Z�l
		CONSTANT float		FLOAT_SPEED;				// ��������
		CONSTANT short		LEARROT_MAX;				// ����]�ō��p�x	
		CONSTANT USHORT		BLESS_LOST_TIME;			// ����1�����܂ł̎���
		CONSTANT USHORT		HP_MAX;						// ���C�t�����l
		CONSTANT USHORT		EVASION_EFFECTERACE_TIME;	// ����G�t�F�N�g�̏����鎞��
		CONSTANT BYTE		MOVE_BUBBLE_MAX;			// ���������̖A���Y�ő吔
		CONSTANT BYTE		ROT_MAX;					// ��]�ő�p�x
		CONSTANT BYTE		LEARROT_SPEED;				// ����]����
		CONSTANT BYTE		POWER_MAX;					// �p���[�ő�l
		CONSTANT BYTE		BLESS_MAX;					// ���̃Q�[�W�ő�l
		CONSTANT BYTE		AFTER_IMAGE_MAX;			// �c���ۑ���
		CONSTANT BYTE		SHOTLOST_BLESS_RATE;		// �e�ŏ���鑧�̃��[�g
		CONSTANT BYTE		EVASION_ROT_SPEED;			// �����]���x	
		CONSTANT BYTE		OVERRUN_ROT;				// �͈͒��������̉�]
		CONSTANT BYTE		RETURN_POS;					// �߂�ʒu
		CONSTANT BYTE		POINTER_ANIME_NUM;			// �|�C���^�[�̃A�j����
		CONSTANT BYTE		CHARGE_ANIME_NUM;			// �`���[�W�A�j����
		CONSTANT BYTE		REFORM_SPEED;				// �p���𐳂�����
		CONSTANT BYTE		DIE_ROT_SPEED;				// ���񂾎��ɓ]���鑬��
		CONSTANT BYTE		RECOVERYBLESS_SPEED;		// ���̉񕜑��x

		CONSTANT float	ATTACK_SPEED;		// �U���̑���
		CONSTANT USHORT	ATTACK_ROT_BEGIN;	// �U���p�x�̎n�܂�
		CONSTANT USHORT	ATTACK_ROT_END;		// �U���p�x�̏I���

		/////////////////////////////////
		// �t���O�萔
		/////////////////////////////////
		CONSTANT FLGTYPE	FLG_RIGHT_EVASION;	// �E�ɉ��
		CONSTANT FLGTYPE	FLG_OVER;			// �o�b�N
		CONSTANT FLGTYPE	FLG_LEFT_EVASION;	// ���ɉ��
		CONSTANT FLGTYPE	FLG_SHOT;			// ����
		CONSTANT FLGTYPE	FLG_EVASION;		// ���
		CONSTANT FLGTYPE	FLG_ATTACK;			// �U����
		CONSTANT FLGTYPE	FLG_ATTACK_OK;		// �U��������s����
		CONSTANT FLGTYPE	FLG_DAMAGE;			// �_���[�W��H�����
		CONSTANT FLGTYPE	FLG_HIT_STONE;		// �΂ɓ�������
		CONSTANT FLGTYPE	FLG_JUMP;			// ���񕜒�(���ʉ��o)
		CONSTANT FLGTYPE	FLG_CHARGE;			// ���ߒ�
		CONSTANT FLGTYPE	FLG_CHARGEMAX;		// �ő嗭��
		CONSTANT FLGTYPE	FLG_DRAW_CHARGE;	// �`���[�W�A�j���[�V�����`��
		CONSTANT FLGTYPE	FLG_MAX_EMISSIVE;	// ���ȏؖ����点�邩�ǂ���
		CONSTANT FLGTYPE	FLG_DIE;			// ����

		/////////////////////////////////
		// ���\�b�h
		/////////////////////////////////

		// �R���X�g���N�^
		CDolphin();

		// �f�X�g���N�^
		~CDolphin();

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
		void Draw( const LPDIRECT3DDEVICE9 lpDevice );

		//////////////////////////////////////////////////
		//	�E�֐���	DrawEffect
		//	�E����		�G�t�F�N�g�̕`����s��
		//	�E����		
		//		- lpDevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawEffect( const LPDIRECT3DDEVICE9 lpdevice );

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

		//////////////////////////////////////////////////
		//	�E�֐���	Action
		//	�E����		�e�ɑ΂���s��(Dispatch����̂݌Ă΂��)
		//	�E����		
		//		- object	�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Action( Shot::CBase& object );

		//////////////////////////////////////////////////
		//	�E�֐���	Action
		//	�E����		�{�X�ɑ΂���s��(Dispatch����̂݌Ă΂��)
		//	�E����		
		//		- object	�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Action( Boss::CBase& object );

		//////////////////////////////////////////////////
		//	�E�֐���	GetMatrix
		//	�E����		�s����l������
		//	�E����		�Ȃ�
		//	�E�߂�l	�s���Ԃ�
		//////////////////////////////////////////////////
		inline D3DXMATRIX GetMatrix() const
		{
			return m_mMat;
		}
		
		//////////////////////////////////////////////////
		//	�E�֐���	GetAfterMatrix
		//	�E����		�c���̍Ō�̍s����l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�c���̍Ō�̍s���Ԃ�
		//////////////////////////////////////////////////
		inline const D3DXMATRIX GetAfterMatrix() const
		{
			return m_lpmAfter[ AFTER_IMAGE_MAX - 1 ];
		}
		
		//////////////////////////////////////////////////
		//	�E�֐���	GetPos
		//	�E����		�ʒu���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�ʒu��Ԃ�
		//////////////////////////////////////////////////
		inline D3DXVECTOR3 GetPos() const
		{
			return m_mMat.GetPos();
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetFlg
		//	�E����		�t���O(���)���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�t���O(���)��Ԃ�
		//////////////////////////////////////////////////
		inline FLGTYPE GetFlg() const
		{
			return m_Flg;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetShots
		//	�E����		�e�}�l�[�W���[���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�e�}�l�[�W���[��Ԃ�
		//////////////////////////////////////////////////
		inline Shot::CManager* GetShots()
		{
			return &m_Shots;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetBubbles
		//	�E����		�A�}�l�[�W���[���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�A�}�l�[�W���[��Ԃ�
		//////////////////////////////////////////////////
		inline CBubbleManager* GetBubbles()
		{
			return &m_Bubbles;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetLife
		//	�E����		���C�t���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���C�t��Ԃ�
		//////////////////////////////////////////////////
		inline USHORT GetLife() const
		{
			return m_HP;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetPower
		//	�E����		���߂Ă���͂̋������l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���߂Ă���͂̋�����Ԃ�
		//////////////////////////////////////////////////
		inline float GetPower() const
		{
			return m_Power;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetFinDir
		//	�E����		���݂̌����Ă�����Ђ�̕�����
		//				�v�Z���ĕԂ�
		//	�E����		�Ȃ�
		//	�E�߂�l	���Ђ�̕�����Ԃ�
		//////////////////////////////////////////////////
		D3DXVECTOR3 GetFinDir() const;

		//////////////////////////////////////////////////
		//	�E�֐���	GetBless
		//	�E����		���ݑ��̎c�ʂ��l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���ݑ��̎c�ʂ�Ԃ�
		//////////////////////////////////////////////////
		inline USHORT GetBless() const
		{
			return m_Bless;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetSaveMat
		//	�E����		�ۑ��s����l��(�͈̓I�[�o�[�̎��Ɏg�p)
		//	�E����		�Ȃ�
		//	�E�߂�l	�ۑ��s���Ԃ�
		//////////////////////////////////////////////////
		inline D3DXMATRIX GetSaveMat() const 
		{
			return m_mSave;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	RecoveryBless
		//	�E����		���Q�[�W���񕜂���
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		inline void RecoveryBless()
		{
			if( (m_Bless += RECOVERYBLESS_SPEED) > BLESS_MAX )
			{
				m_Bless = BLESS_MAX;
			}
		}
		
		//////////////////////////////////////////////////
		//	�E�֐���	GetNowSpeed
		//	�E����		���݂̈ړ��̑������l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���݂̈ړ��̑�����Ԃ�
		//////////////////////////////////////////////////
		inline float GetNowSpeed() const
		{
			return m_NowSpeed;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	SetStoneManager
		//	�E����		�ړ����ɏՓ˔�����s��
		//				��}�l�[�W���[��ݒ肷��
		//	�E����		
		//		- lp_stones	��}�l�[�W���[�̃|�C���^
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		inline void SetStoneManager( CStoneManager* lp_stones )
		{
			m_lpStones = lp_stones;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	DrawRipples
		//	�E����		�g��̕`����s��
		//	�E����		
		//		- lp_device	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawRipples( const LPDIRECT3DDEVICE9 lp_device );
	
		//////////////////////////////////////////////////
		//	�E�֐���	SetMatrix
		//	�E����		�s���ݒ肷��(�N���A�V�[�P���X�̂�)
		//	�E����		
		//		- mat	�s��
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		inline void SetMatrix( const D3DXMATRIX& mat )
		{
			m_mMat = mat;
		}

	private:	// ����J	

		//friend Sequence::CClear;

		//////////////////////////////////////////////////
		// �t�B�[���h
		//////////////////////////////////////////////////
		CMatrix				m_mMat;				// �s��(���[�J�����W�O��)
		CMatrix				m_mSave;			// �ۑ��s��
		CMatrix*			m_lpmAfter;			// �c���ۑ�(���I�z��)
		CVector				m_MoveVec;			// ������ԕ���
		CVector				m_EvasionVec;		// �������x�N�g��
		CVector				m_AttackMoveDir;	// �U�����ɐi�ޕ���
		Shot::CManager		m_Shots;			// �e�}�l�[�W���[
		CBubbleManager		m_Bubbles;			// �A�}�l�[�W���[
		CFinTail			m_Fin;				// ���Ђ�
		CXfile*				m_lpMesh;			// ���b�V��
		FLGTYPE				m_Flg;				// �t���O(��ԂȂ�)
		DWORD				m_SaveBlessTime;	// ��������������
		POINT				m_SaveMousePos;		// �}�E�X�̍��W
		CStoneManager*		m_lpStones;			// ��̃|�C���^
		CRippleManager		m_Ripples;			// �g��
		CPointSpriteManager	m_Spray;			// ���Ԃ��p�|�C���g�X�v���C�g
		CPointSpriteManager	m_Evasion;			// ����G�t�F�N�g
		//CSandSpray*			m_lpSandSplay;		// �����Ԃ�
		CBoard				m_Pointer;			// ���ˈʒu�𖾊m�ɂ���
		CBoard				m_ChargeEffect;		// �`���[�W�G�t�F�N�g
		BYTE				m_PointerAnime;		// �|�C���^�[�̃A�j���J�E���^
		DWORD				m_PointerSaveTime;	// �|�C���^�[�̎��ԕۑ��p
		CGraphicAnime		m_ChargeMaxAnime;	// �`���[�WMax�A�j���[�V����
		CGraphicAnime		m_ChargeAnime;		// �`���[�W�A�j���[�V����
		DWORD				m_EmissiveSaveTime;	// �G�~�b�V�u���ԕۑ�
		short				m_HP;				// ���C�t
		short				m_RotCnt;			// ��]�J�E���^
		short				m_Bless;			// ��
		float				m_Power;			// �p���[(�e�ɉe��)
		float				m_NowSpeed;			// ���̑���
		float				m_NowRotSpeed;		// ����]���Ă��鑬��
		float				m_HitPower;			// ������т̋���
		float				m_NowPlayerScale;	// �v���C���[�g�嗦	
		float				m_NowFloatPower;	// ������
		
		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////
		
		//////////////////////////////////////////////////
		//	�E�֐���	UpdateMove
		//	�E����		�����̍X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateMove();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateKey
		//	�E����		���͍X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateKey();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdatePowerAndShot
		//	�E����		�e�Ƃ��̋����X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdatePowerAndShot();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateBless
		//	�E����		���̍X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateBless();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateAttack
		//	�E����		�t�B���̍U���X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateAttack();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateEvasion
		//	�E����		����X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateEvasion();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateAction
		//	�E����		�s���̍X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateAction();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateOverRun
		//	�E����		�͈̓I�[�o�[�̍X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateOverRun();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateSurfaceSea
		//	�E����		�C�ʂɏo�����̍X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateSurfaceSea();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateCollisionMove
		//	�E����		�Փ˔�����s���Ȃ���
		//				�ړ��X�V���s��
		//	�E����		
		//		- lp_move	�ړ������
		//		- is_move	�������ǂ���
		//	�E�߂�l	�Փ˂����Ȃ�true
		//				�����łȂ��Ȃ�false��Ԃ�
		//////////////////////////////////////////////////
		bool UpdateCollisionMove( const D3DXVECTOR3* lp_move, bool is_move = true );

		//////////////////////////////////////////////////
		//	�E�֐���	ReformPose
		//	�E����		�C�ʂɑ����Ĕw��������悤��
		//				���݂̎p���𐳂��Ă���
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void ReformPose();

		//////////////////////////////////////////////////
		//	�E�֐���	ReformPose
		//	�E����		�������g�̕`����s��
		//	�E����		
		//		- lpdevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawPlayer( const LPDIRECT3DDEVICE9 lpdevice );

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateDie
		//	�E����		�s�k�����Ƃ��̍X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateDie();
	};
}

#endif // __AKI_PLAYERBASE_H__