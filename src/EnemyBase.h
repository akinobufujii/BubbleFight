#ifndef __AKI_ENEMYBASE_H__
#define __AKI_ENEMYBASE_H__

////////////////////////////////
// �s���S�^�錾
////////////////////////////////
class Player::CDolphin;
class CStoneManager;
class CFishManager;
class CDamageObjectBase;
class CWaterSpray;

namespace Enemy
{
	////////////////////////////////
	// �G���N���X
	////////////////////////////////
	class CBase : virtual public IDoubleDispatch
	{
	public:	// �O�����J

		////////////////////////////////
		// �萔
		////////////////////////////////
		CONSTANT float	PARTICLE_POWER;			// �p�[�e�B�N���ړ��̋���
		CONSTANT float	FLOAT_POWER_LINE;		// �s����ς��镂�̗͂�
		CONSTANT float	FLOAT_SPEED;			// ��������
		CONSTANT float	SCALE_UP_SPEED;			// �g��̑���
		CONSTANT float	DEFAULT_FLAOT_POWER;	// �W���̕���
		CONSTANT float	DEFAULT_HITPOWER_OFF;	// �����
		CONSTANT float	DEFAULT_ROTSPEED;		// ��]���鑬��
		CONSTANT BYTE	ERASE_LENGTH;			// �����鋗��
		CONSTANT BYTE	BUBBLE_SCALE_RATE;		// �A�̊g�嗦
		CONSTANT LPSTR	BUBBLE_MESH_PASS;		// �G���ޖA�̃��b�V���p�X

		////////////////////////////////
		// �t���O�萔
		////////////////////////////////
		CONSTANT BYTE ERASE;		// ����
		CONSTANT BYTE BREAK;		// ���Ă���
		CONSTANT BYTE BUBBLE_HIT;	// �A�ɓ������Ă���
		CONSTANT BYTE SEARCH;		// �T���Ă���
		CONSTANT BYTE ATTACK;		// �U����
		CONSTANT BYTE BUBBLE_WRAP;	// �A�ɂ܂ꂫ���Ă���
		CONSTANT BYTE JUMP;			// �W�����v��
	
		////////////////////////////////
		// ���\�b�h
		////////////////////////////////
		
		// �R���X�g���N�^
		CBase();

		// ���z�f�X�g���N�^
		virtual ~CBase();		

		//////////////////////////////////////////////////
		//	�E�֐���	Update
		//	�E����		�X�V���s��(�h����Œ�`)
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		virtual void Update() = 0;

		//////////////////////////////////////////////////
		//	�E�֐���	Draw
		//	�E����		�`����s��(�h����Œ�`)
		//	�E����		
		//		- lp_device	�f�o�C�X
		//		- is_shader	�v���O���}�u���V�F�[�_�ŕ`�悷�邩�ǂ���
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		virtual void Draw( const LPDIRECT3DDEVICE9 lp_device, bool is_shader ) = 0;

		//////////////////////////////////////////////////
		//	�E�֐���	DrawEffect
		//	�E����		�G�t�F�N�g�`����s��(�h����ł���Β�`)
		//	�E����		
		//		- lp_device	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		virtual void DrawEffect( const LPDIRECT3DDEVICE9 lp_device ){}

		//////////////////////////////////////////////////
		//	�E�֐���	GetScore
		//	�E����		�_�����l��(�h����Ō��߂�)
		//	�E����		�Ȃ�
		//	�E�߂�l	�_����Ԃ�
		//////////////////////////////////////////////////
		virtual ULONG GetScore() const = 0;

		//////////////////////////////////////////////////
		//	�E�֐���	GetMesh
		//	�E����		���b�V�������l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���b�V������Ԃ�
		//////////////////////////////////////////////////
		virtual const CXfile* GetMesh() const = 0;

		//////////////////////////////////////////////////
		//	�E�֐���	GetShotManager
		//	�E����		�e�}�l�[�W���[���l��
		//				�����ǂ����͔h����Ō��߂�
		//	�E����		�Ȃ�
		//	�E�߂�l	�e�}�l�[�W���[��Ԃ�
		//////////////////////////////////////////////////
		virtual Shot::CManager* GetShotManager()
		{
			return NULL;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	DrawRipples
		//	�E����		�g���`�悷��
		//	�E����		
		//		- lp_device	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		inline void DrawRipples( const LPDIRECT3DDEVICE9 lp_device )
		{
			m_Ripples.Draw( lp_device );
		}

		//////////////////////////////////////////////////
		//	�E�֐���	SetPlayerInfo
		//	�E����		�l�X�Ȃ��ƂɎg�����߂�
		//				�v���C���[��ݒ肷��
		//	�E����		
		//		- info	�v���C���[�̃|�C���^
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		inline static void SetPlayerInfo( Player::CDolphin* info )
		{
			m_lpPlayer = info;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	SetStoneManager
		//	�E����		�Փ˔�����s�����߂�
		//				��}�l�[�W���[����ݒ肷��
		//	�E����		
		//		- lp_stones	��}�l�[�W���[�̃|�C���^
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		inline static void SetStoneManager( CStoneManager* lp_stones )
		{
			m_lpStone = lp_stones;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	SetFishManager
		//	�E����		�P�����߂�
		//				���}�l�[�W���[����ݒ肷��
		//	�E����		
		//		- lp_fishes	���}�l�[�W���[�̃|�C���^
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		inline static void SetFishManager( CFishManager* lp_fishes )
		{
			m_lpFishes = lp_fishes;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetFlg
		//	�E����		�t���O(���)���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�t���O(���)��Ԃ�
		//////////////////////////////////////////////////
		inline BYTE GetFlg() const
		{
			return m_Flg;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetMatrix
		//	�E����		�s����l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�s��̃|�C���^��Ԃ�
		//////////////////////////////////////////////////
		inline D3DXMATRIX* GetMatrix()
		{
			return &m_mMat;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetPos
		//	�E����		�ʒu���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�ʒu��Ԃ�
		//////////////////////////////////////////////////
		D3DXVECTOR3 GetPos();

		//////////////////////////////////////////////////
		//	�E�֐���	VacuumBubble
		//	�E����		BubbleBreaker�������g��
		//				�A���z�����
		//	�E����		
		//		- rate	�ǂꂾ���z����邩�̒l
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void VacuumBubble( float rate );

		//////////////////////////////////////////////////
		//	�E�֐���	GetBubbleScale
		//	�E����		�A�̑傫�����l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�A�̑傫����Ԃ�
		//////////////////////////////////////////////////
		inline float GetBubbleScale() const 
		{
			return m_FloatPower;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	Action
		//	�E����		�e�ɑ΂��Ă̍s�����N����
		//				Dispatch����Ă΂��
		//	�E����		�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		virtual void Action( Shot::CBase& object );

		//////////////////////////////////////////////////
		//	�E�֐���	Action
		//	�E����		�G�ɑ΂��Ă̍s�����N����
		//				Dispatch����Ă΂��
		//	�E����		�s�����N�����Ώۂ̃I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		virtual void Action( Enemy::CBase& object );

	protected:	// ���ʃ����o
		
		//////////////////////////////////////////////////
		// �萔
		//////////////////////////////////////////////////
		CONSTANT int	BURST_WIDTH;			// �����摜�A��
		CONSTANT int	BURST_HEIGHT;			// �����摜�A����
		CONSTANT int	BURST_ANIME_SIZE;		// �����摜�A�A�j���P�̑傫��
		CONSTANT int	BURST_ANIME_WIDTH_NUM;	// �����摜�A���A�j����
		CONSTANT int	BURST_ANIME_HEIGHT_NUM;	// �����摜�A�����A�j����
		CONSTANT int	BURST_ANIME_NUM;		// �����摜�A�A�j������

		//////////////////////////////////////////////////
		// �t�B�[���h
		//////////////////////////////////////////////////
		CMatrix					m_mMat;			// �s��(���[�J���O��)
		CMatrix					m_mBalloon;		// ���D�s��
		BYTE					m_Flg;			// �t���O
		BYTE					m_AnimeCnt;		// �A�j���J�E���^
		float					m_FloatPower;	// ����
		CBubbleManager			m_Bubbles;		// �A�}�l�[�W��
		CPointSpriteManager		m_Particle;		// �p�[�e�B�N���p
		CWaterSpray*			m_lpSpray;		// �����Ԃ�
		CVector					m_HitDir;		// �������Ĕ��ł�������
		float					m_HitPower;		// ������т̗�
		CRippleManager			m_Ripples;		// �g��
		CDamageObjectBase*		m_lpTargetFish;	// �^�[�Q�b�g�i�[�p

		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////
		
		//////////////////////////////////////////////////
		//	�E�֐���	UpdateRot
		//	�E����		�v���C���[�̕����ɉ�]����
		//	�E����		
		//		- rotmax	��]�ő�p�x
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateRot( float rotmax = DEFAULT_ROTSPEED );

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateFloatPower
		//	�E����		���͍X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateFloatPower();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateBubbleScale
		//	�E����		��܂��A�̑傫���X�V
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateBubbleScale();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateHitMove
		//	�E����		������эX�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateHitMove();

		//////////////////////////////////////////////////
		//	�E�֐���	DrawBreakEnemy
		//	�E����		���񂾓G��`�悷��
		//	�E����		
		//		- lpdevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawBreakEnemy( const LPDIRECT3DDEVICE9 lpdevice );
		
		//////////////////////////////////////////////////
		//	�E�֐���	DrawBubble
		//	�E����		�A��`�悷��
		//	�E����		
		//		- lpdevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawBubble( const LPDIRECT3DDEVICE9 lpdevice );

		//////////////////////////////////////////////////
		//	�E�֐���	DrawShader
		//	�E����		�v���O���}�u���V�F�[�_�ŕ`�悷��
		//	�E����		
		//		- lpdevice	�f�o�C�X
		//		- mesh		���b�V��
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawShader( const LPDIRECT3DDEVICE9 lpdevice, CXfile* mesh );

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateMust
		//	�E����		�h����ŕK���Ăяo���ė~��������
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateMust();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateCollisionMove
		//	�E����		�Փ˔�����s���Ȃ���ړ��X�V���s��
		//	�E����		
		//		- move	�ړ���
		//		- limit	���
		//	�E�߂�l	�Փ˂����true
		//				�����łȂ����false��Ԃ�
		//////////////////////////////////////////////////
		bool UpdateCollisionMove( const D3DXVECTOR3* move, const float limit );

		//////////////////////////////////////////////////
		//	�E�֐���	SearchTarget
		//	�E����		�P���ڕW�����߂�
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void SearchTarget();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateJump
		//	�E����		�W�����v�X�V���s��
		//				���ʂ�艺���΃t���O��������
		//	�E����		
		//		- speed	�i�ޑ���
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateJump( float speed );

		//////////////////////////////////////////////////
		//	�E�֐���	JudgeJump
		//	�E����		�W�����v�������s�����ǂ���
		//				�W�����v�t���O�𗧂Ă���
		//				�����Ԃ������������肷��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void JudgeJump();
		
		//////////////////////////////////////////////////
		//	�E�֐���	GetPlayerInfo
		//	�E����		�h���悪�v���C���[�����l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�v���C���[�̃|�C���^��Ԃ�
		//////////////////////////////////////////////////
		inline Player::CDolphin* GetPlayerInfo() const
		{
			return m_lpPlayer;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetStoneManager
		//	�E����		�h���悪��}�l�[�W���[�����l��
		//	�E����		�Ȃ�
		//	�E�߂�l	��}�l�[�W���[�̃|�C���^��Ԃ�
		//////////////////////////////////////////////////
		inline CStoneManager* GetStoneManager()
		{
			return m_lpStone;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetFishManager
		//	�E����		�h���悪���}�l�[�W���[�����l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���}�l�[�W���[�̃|�C���^��Ԃ�
		//////////////////////////////////////////////////
		inline CFishManager* GetFishManager()
		{
			return m_lpFishes;
		}

	private:	// ����J

		//////////////////////////////////////////////////
		// �t�B�[���h
		//////////////////////////////////////////////////
		static Player::CDolphin*	m_lpPlayer;		// �v���C���[���
		static CStoneManager*		m_lpStone;		// ��̏��
		static CFishManager*		m_lpFishes;		// �����
		float						m_NowFloatSpeed;// ���͂̉����x
		float						m_NowScale;		// ���̑傫��
		float						m_JumpSpeed;	// �W�����v���̑���
	};
}

#endif	// __AKI_ENEMYBASE_H__