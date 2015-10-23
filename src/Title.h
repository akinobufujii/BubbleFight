#ifndef __AKI_TITLE_H__
#define __AKI_TITLE_H__

////////////////////////////////////
// �^�C�g���V�[�P���X
////////////////////////////////////
namespace Sequence
{
	class CTitle : public IBase
	{
	public:	// �O�����J

		////////////////////////////////////
		// �萔
		////////////////////////////////////
		CONSTANT LPSTR	TEXTURE_LOAD_FILE_PASS;	// �ǂݍ��ރe�N�X�`���̃t�@�C���p�X
		CONSTANT LPSTR	XFILE_LOAD_FILE_PASS;	// �ǂݍ���X�t�@�C���̃t�@�C���p�X
		CONSTANT LPSTR	BUBBLE_TEX;				// �A�̃e�N�X�`��
		CONSTANT LPSTR  MENU_TEX;				// ���j���[�̃e�N�X�`��
		CONSTANT LPSTR  TITLELOGO_TEX;			// �^�C�g�����S�e�N�X�`��
		CONSTANT short	MENU_WIDTH;				// ���j���[1�̕�
		CONSTANT short	MENU_HEIGHT;			// ���j���[1�̍���
		CONSTANT short	MENU_X;					// ���j���[��X���W
		CONSTANT short	MENU_Y;					// ���j���[��Y���W
		CONSTANT short	TITLELOGO_WIDTH;		// �^�C�g�����S�̕�
		CONSTANT short	TITLELOGO_HEIGHT;		// �^�C�g�����S�̍���
		CONSTANT short	THANKSLOGO_WIDTH;		// �񋟂̕�
		CONSTANT short	THANKSLOGO_HEIGHT;		// �񋟂̍���
		CONSTANT float	BUBBLE_X_POS;			// �A�̏o��X���W(���E�Ώ�)
		CONSTANT float	BUBBLE_Y_POS;			// �A�̏o��Y���W(����)
		CONSTANT float	BUBBLE_SPEED_LIMIT;		// �A�̍ő�X�s�[�h
		CONSTANT BYTE	BUBBLE_NUM;				// �A�̐�
		CONSTANT USHORT	BUBBLE_ERASE_TIME;		// �A�����Z�b�g����鎞��
		
		////////////////////////////////////
		// �񋓒萔
		////////////////////////////////////
		// ���j���[�̎��
		enum MENU_TYPE
		{
			MENU_START,	// �X�^�[�g
			MENU_END,	// �I��
			MENU_NUM	// ��
		};

		////////////////////////////////////
		// ���\�b�h
		////////////////////////////////////

		// �R���X�g���N�^
		CTitle();

		// �f�X�g���N�^
		~CTitle();

		//////////////////////////////////////////////////
		//	�E�֐���	FileLoad
		//	�E����		�ǂݍ��݂⏉�������s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void FileLoad();

		//////////////////////////////////////////////////
		//	�E�֐���	RunProc
		//	�E����		�^�C�g�����C���֐�
		//	�E����		�Ȃ�
		//	�E�߂�l	����������Ȃ�true
		//				�I���Ȃ�false��Ԃ�
		//////////////////////////////////////////////////
		bool RunProc();

		//////////////////////////////////////////////////
		//	�E�֐���	GetNext
		//	�E����		���̃V�[�P���X���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�X�e�[�W�Z���N�g��Ԃ�
		//////////////////////////////////////////////////
		IBase* GetNext();

	private:	// ����J

		//////////////////////////////////////////////////
		// �t�B�[���h
		//////////////////////////////////////////////////
		CSprite				m_Sprite;			// �X�v���C�g
		CProj				m_Proj;				// ���e�N���X
		CCamera				m_Camera;			// �J�����N���X
		CPointSpriteManager	m_BubblesLeft;		// �����̖A
		CPointSpriteManager	m_BubblesRight;		// �E���̖A
		CPointSpriteManager m_SelectMenuBubble;	// �A�j���p
		CTexture			m_SceneTex;			// �V�[���e�N�X�`��
		char				m_SelectMenu;		// �I��ł��郁�j���[
		bool				m_IsRun;			// �����𑖂邩
		bool				m_IsSelect;			// �I�񂾂�
		bool				m_IsTeachAlphaPlus;	// �A���t�@�l�����Z���邩�ǂ���
		short				m_Alpha;			// �A���t�@
		short				m_TeachAlpha;		// �w���̃A���t�@�l

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
		//	�E�֐���	Load
		//	�E����		�ǂݍ��݂␶�����s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Load();

		//////////////////////////////////////////////////
		//	�E�֐���	Release
		//	�E����		����Ɣj����s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Release();
	};
}

#endif // __AKI_TITLE_H__