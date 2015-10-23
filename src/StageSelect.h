#ifndef	__AKI_STAGESELECT_H__
#define __AKI_STAGESELECT_H__

namespace Sequence
{
	//////////////////////////////////////////////////
	// �X�e�[�W�V�[�P���X
	//////////////////////////////////////////////////
	class CStageSelect : public IBase
	{
	public:	// �O�����J

		//////////////////////////////////////////////////
		// �萔
		//////////////////////////////////////////////////
		CONSTANT LPSTR	PATH_LOADTEXFILE;		// �t�@�C���p�X
		CONSTANT LPSTR	PATH_LOADXFILE;			// XFile�ǂݍ��݃��X�g�p�X
		CONSTANT LPSTR	PATHES_STAGEIMAGES[];	// �X�e�[�W���Ƃ̃C���[�W�摜�p�X
		CONSTANT LPSTR	PATHES_EXPLAINIMAGES[];	// �X�e�[�W���Ƃ̐����摜�p�X
		CONSTANT short	FONT_WIDTH;				// �t�H���g��
		CONSTANT short	FONT_HEIGHT;			// �t�H���g����
		CONSTANT short	STAGETONT_Y;			// �X�e�[�W�t�H���g��Y���W
		CONSTANT short	SELECTVEC_X;			// ����X���W
		CONSTANT short	SELECTVEC_TOP_Y;		// ���̏㑤Y���W
		CONSTANT short	SELECTVEC_BOTTOM_Y;		// ���̉���Y���W
		CONSTANT short	BUTTON_X;				// �{�^����X���W
		CONSTANT short	BUTTON_Y;				// �{�^����Y���W
		CONSTANT float	SLIDE_IN_SPEED;			// �X���C�h�C���̑���
		CONSTANT float	FONT_ALPHARATE;			// �A���t�@���[�g
		CONSTANT float	IMAGE_MOVEMAX;			// �X�e�[�W�C���[�W�̓����傫��
		CONSTANT float	IMAGE_MOVESPEED;		// �X�e�[�W�C���[�W�̓�������
		CONSTANT float	EXPLAIN_WIDTH;			// �����̕�
		CONSTANT float	EXPLAIN_HEIGHT;			// �����̍���
		CONSTANT float	EXPLAIN_DRAWHEIGHT;		// �����̕`�悷�鍂��
		CONSTANT BYTE	NUMBER_SIZE;			// �����摜�̑傫��
		CONSTANT BYTE	STAGE_MAX;				// �X�e�[�W�ő吔
		CONSTANT BYTE	FADEOUT_SPEED;			// �t�F�[�h�A�E�g�̑���
		
		enum
		{
			FONT_STAGESELECT,	// �X�e�[�W�Z���N�g
		};

		//////////////////////////////////////////////////
		// �t���O�萔
		//////////////////////////////////////////////////
		CONSTANT BYTE	FLG_RUN;		// �����𑖂��Ă���
		CONSTANT BYTE	FLG_IMAGE_UP;	// �X�e�[�W�C���[�W�̏㉺

		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////

		// �R���X�g���N�^
		CStageSelect();

		// �f�X�g���N�^
		~CStageSelect();

		//////////////////////////////////////////////////
		//	�E�֐���	FileLoad
		//	�E����		�ǂݍ��݂⏉�������s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void FileLoad();

		//////////////////////////////////////////////////
		//	�E�֐���	RunProc
		//	�E����		�X�e�[�W�Z���N�g���C���֐�
		//	�E����		�Ȃ�
		//	�E�߂�l	����������Ȃ�true
		//				�I���Ȃ�false��Ԃ�
		//////////////////////////////////////////////////
		bool RunProc();

		//////////////////////////////////////////////////
		//	�E�֐���	GetNext
		//	�E����		���̃V�[�P���X���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Q�[�����C����Ԃ�
		//////////////////////////////////////////////////
		IBase* GetNext();

	private:	// ����J

		//////////////////////////////////////////////////
		// �^�錾
		//////////////////////////////////////////////////
		typedef void (CStageSelect::*FPUPDATE)();			// �X�V�֐��|�C���^�^
		typedef	std::list<CPointSpriteManager*>	BUBBLELIST;	// �A(�|�C���g�X�v���C�g)���X�g�^

		//////////////////////////////////////////////////
		// �t�B�[���h
		//////////////////////////////////////////////////
		BYTE				m_Flg;			// �t���O
		CCamera				m_Camera;		// �J����
		CProj				m_Proj;			// ���e
		CSprite				m_Sprite;		// �X�v���C�g�`��
		CBoard				m_BackBoard;	// �w�i�|���S��
		CBoard				m_StageImage;	// �X�e�[�W�C���[�W�p�|���S��
		CBoard				m_ExplainBoard;	// �����p�|���S��
		CTexture			m_SceneTex;		// �V�[���p�����_�[�^�[�Q�b�g
		float				m_FontY;		// �t�H���g��Y���W
		float				m_StageImageY;	// �X�e�[�W�C���[�W��Y�̈ړ���
		float				m_PosZ;			// �J������Z���W
		float				m_LockY;		// �����_Y

		FPUPDATE			m_fpUpdate;		// �X�V�֐��|�C���^
		char				m_StageIndex;	// �X�e�[�W�|�C���^
		short				m_SceneAlpha;	// �V�[���̃A���t�@�l

		BUBBLELIST			m_BubbleList;	// �A���X�g

		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////

		//////////////////////////////////////////////////
		//	�E�֐���	Update
		//	�E����		�X�V���s��
		//				�����Ŋ֐��|�C���^���Ă�ł���̂�
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Update();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateStartAnime
		//	�E����		�V�[���؂�ւ����ɍŏ��ɍs���X�V
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateStartAnime();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateSelect
		//	�E����		�X�e�[�W�Z���N�g�X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateSelect();

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateFadeOut
		//	�E����		�t�F�[�h�A�E�g����
		//				�X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateFadeOut();

		//////////////////////////////////////////////////
		//	�E�֐���	Draw
		//	�E����		�`����s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Draw();
	};
}

#endif	// __AKI_STAGESELECT_H__