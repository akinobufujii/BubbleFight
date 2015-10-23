#ifndef __AKI_CLEAR_H__
#define __AKI_CLEAR_H__

//////////////////////////////////
// �N���A�V�[�P���X
//////////////////////////////////
namespace Sequence
{
	class CClear : public IBase
	{
	public:	// �O�����J

		//////////////////////////////////////////////////
		// �萔
		//////////////////////////////////////////////////
		
		// �t�H���g��ނ̗񋓑�
		enum FONTKIND	
		{
			FONTKIND_SCENE,
			FONTKIND_CLEAR,
			FONTKIND_ZKEY,
			FONTKIND_SCORE_AND_TIME,
			FONTKIND_TOTALSCORE
		};

		CONSTANT LPSTR	PASS_BACK_GROUND_MESH;	// �w�i�̃��b�V��
		CONSTANT LPSTR	TEX_BUBBLE;				// �A�̃e�N�X�`��
		CONSTANT LPSTR	TEXPASS_FONT;			// �t�H���g�̃p�X
		CONSTANT short	BUBBLE_ADD_TIME;		// �A�̒ǉ�����
		CONSTANT BYTE	BUBBLE_NUM;				// ��x�ɍ��A��
		CONSTANT BYTE	BUBBLE_ADD_POS_LIMIT;	// �A�̑n�肾���͈�
		CONSTANT int	FONT_WIDTH;				// �t�H���g��
		CONSTANT int	FONT_HEIGHT;			// �t�H���g����
		CONSTANT BYTE	FADEIN_OUT_SPEED;		// �t�F�[�h�̑��x
		CONSTANT BYTE	FADEOUT_MIN;			// �t�F�[�h�A�E�g�̍ŏ��l
		CONSTANT BYTE	NUMBER_SIZE;			// �����̑傫��
		CONSTANT BYTE	GRAPHNUM_COLON;			// ���Ԃ̋C�؂�ł���R�����̃i���o�[
		CONSTANT int	DEST_SCOREFONT_X;		// �X�R�A�t�H���g�̕`���X���W
		CONSTANT int	DEST_SCOREFONT_Y;		// �X�R�A�t�H���g�̕`���Y���W
		CONSTANT int	DEST_NUMBER_X;			// ������`�悷��X���W
		CONSTANT int	DEST_SOCRE_Y;			// �X�R�A�`��Y���W
		CONSTANT int	DEST_CLEARTIME_Y;		// �N���A���ԕ`��Y���W
		CONSTANT int	DEST_TOTALSCORE_Y;		// �g�[�^���X�R�A�t�H���gY���W
		CONSTANT int	DEST_TOTALSCORENUM_X;	// �g�[�^���X�R�A�̐���X���W
		CONSTANT int	DEST_TOTALSCORENUM_Y;	// �g�[�^���X�R�A�̐���Y���W
		
		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////

		// �R���X�g���N�^
		CClear( ULONG score, double clear_time );

		// �f�X�g���N�^
		~CClear();

		//////////////////////////////////////////////////
		//	�E�֐���	FileLoad
		//	�E����		�ǂݍ��݂��s��
		//	�E����		�Ȃ�	
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void FileLoad();

		//////////////////////////////////////////////////
		//	�E�֐���	RunProc
		//	�E����		���̃V�[�P���X�̃��C���֐�
		//	�E����		�Ȃ�	
		//	�E�߂�l	�܂��X�V���s���Ȃ�true
		//				�j������Ȃ��false��Ԃ�
		//////////////////////////////////////////////////
		bool RunProc();

		//////////////////////////////////////////////////
		//	�E�֐���	GetNext
		//	�E����		���̃V�[�P���X���l������
		//	�E����		�Ȃ�	
		//	�E�߂�l	�^�C�g���V�[�P���X�̃|�C���^��Ԃ�
		//////////////////////////////////////////////////
		IBase* GetNext();

	private:	// ����J

		//////////////////////////////////////////////////
		// �^�錾
		//////////////////////////////////////////////////
		typedef void (CClear::*FPDRAWFUNC)();				// �`��֐��|�C���^�^
		typedef std::list<FPDRAWFUNC>			DRAWLIST;	// �`��֐��|�C���^���X�g�^
		typedef std::list<CPointSpriteManager*>	BUBBLELIST;	// �A(�|�C���g�X�v���C�g)���X�g�^

		//////////////////////////////////////////////////
		// �t�B�[���h
		//////////////////////////////////////////////////
		bool				m_IsRun;			// �X�V���邩�ǂ���
		CCamera				m_Camera;			// �J����
		CProj				m_Proj;				// ���e
		DWORD				m_BubbleSaveTime;	// �A�̎��ԕۑ��p
		CSprite				m_Sprite;			// �X�v���C�g�f�o�C�X
		int					m_Alpha;			// �t�H���g�p�A���t�@�l
		bool				m_IsFadeIn;			// �t�F�[�h�C���E�A�E�g�t���O(�^�ŃC��)
		CFishManager		m_Fishes;			// ������
		ULONG				m_Score;			// �X�R�A
		DWORD				m_FishMoveTime;		// ���ԕۑ��p
		int					m_ClearSecond;		// �N���A����(�b)
		Player::CDolphin*	m_lpDolphin;		// �C���J
		CMatrix				m_mDolphin;			// �C���J�̍s��
		CVector				m_Target;			// �C���J�̖ړI�n

		BUBBLELIST			m_PointBubbleList;	// �A���X�g
		DRAWLIST			m_DrawFpList;		// �`��֐��|�C���^���X�g

		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////
		
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
		//	�E����		�Ȃ�	
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Draw();

		//////////////////////////////////////////////////
		//	�E�֐���	DrawNumber
		//	�E����		�����̕`����s��
		//	�E����		
		//		- num	�`�悷�鐔��
		//		- x		�`��擪��X���W
		//		- y		�`��擪��Y���W
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawNumber( int num, int x, int y );

		//////////////////////////////////////////////////
		//	�E�֐���	Load
		//	�E����		�ǂݍ��݂��s��
		//	�E����		�Ȃ�	
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Load();

		//////////////////////////////////////////////////
		//	�E�֐���	Release
		//	�E����		�ǂݍ��݂̉�����s��
		//	�E����		�Ȃ�	
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Release();
	};
}

#endif // __AKI_CLEAR_H__