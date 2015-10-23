#ifndef __AKI_GAMEMAIN_H__
#define __AKI_GAMEMAIN_H__

//////////////////////////////////////////////////
// �s���S�^�錾
//////////////////////////////////////////////////
class CSurfaceSea;
class CUI;
class CPlanktonManager;
class CStoneManager;
class CFishManager;
class CSunLineManager;
namespace Enemy
{
	class CManager;
}

namespace Sequence
{
	//////////////////////////////////////////////////
	// �Q�[���{��
	//////////////////////////////////////////////////
	class CGameMain : public IBase
	{	
	public:
		//////////////////////////////////////////////////
		// �^
		//////////////////////////////////////////////////
		typedef BYTE	FLGTYPE;

		//////////////////////////////////////////////////
		// �萔
		//////////////////////////////////////////////////
		CONSTANT LPSTR	PATH_STONEDATA;			// ��̃t�@�C���p�X
		CONSTANT LPSTR	SKY_MESH_PASS;			// �󃁃b�V���p�X
		CONSTANT LPSTR	PATH_ENEMYDATA;			// �G�f�[�^�ǂݍ��݃t�@�C���p�X
		CONSTANT LPSTR	PATH_DAMAGEFISHDATA;	// �_���[�W���󂯂鋛�ǂݍ��݃p�X
		CONSTANT BYTE	BUBBLES_NUM;			// �A�������o���Ă����̐�
		CONSTANT float	SURFACE_ALPHA_RATE;		// �C�̓��߃��[�g
		CONSTANT float	SURFACESEA_HEIGHT;		// �C�ʂ̍���
		CONSTANT float	BOTTOMSEA_HEIGHT;		// �C��̍���
		CONSTANT float	SEA_MOVELIMIT;			// �C�̒��̐���
		CONSTANT float	STAGE_SCALE;			// �X�e�[�W�g��W��
		CONSTANT float	SEA_COLOR_ADD;			// �C�̐F�̉��Z��(�v���C���[�̈ʒu�ɂ���ĕς��)
		CONSTANT float	FOG_DEPTH_ADD;			// �t�H�O�̔Z��
		CONSTANT float	SCROLL_SPEED;			// �e�N�X�`���̃X�N���[���̑���
		CONSTANT float	JUMP_CAMERA_HEIGHT;		// �v���C���[���W�����v���Ă��鎞�̍���
		CONSTANT float	FOCUS_RATE;				// �t�H�[�J�X���[�g
		CONSTANT short	DEFAUL_GAMESCENE_ALPHA;	// �Q�[���V�[���̃A���t�@�l

		//////////////////////////////////////////////////
		// �t���O�萔
		//////////////////////////////////////////////////
		CONSTANT FLGTYPE	FLG_RESULT;		// ���ʂ��o�����ǂ���
		CONSTANT FLGTYPE	FLG_RUN;		// �����𑖂邩�ǂ���
		CONSTANT FLGTYPE	FLG_BOSSBATTLE;	// �{�X�o�g�����ǂ���
		CONSTANT FLGTYPE	FLG_BOSSENTER;	// �{�X�o��

		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////

		// �R���X�g���N�^
		CGameMain( BYTE stage );

		// �f�X�g���N�^
		~CGameMain();

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
		//	�E�߂�l	�N���A�V�[�P���X�̃|�C���^���A
		//				�Q�[���I�[�o�[�V�[�P���X�̃|�C���^��
		//				�Ԃ�
		//////////////////////////////////////////////////
		IBase* GetNext();
		
		//////////////////////////////////////////////////
		//	�E�֐���	GetCamera
		//	�E����		���̃N���X�̎g���Ă���J������Ԃ�
		//	�E����		�Ȃ�	
		//	�E�߂�l	�J�����N���X�̃|�C���^
		//////////////////////////////////////////////////
		static inline const CCamera* GetCamera()
		{
			return &m_Camera;
		}

	private:

		//////////////////////////////////////////////////
		// �t�B�[���h
		//////////////////////////////////////////////////
		Player::CDolphin*	m_lpPlayer;			// �v���C���[
		Boss::CBase*		m_lpBoss;			// �{�X�v���C���[
		Enemy::CManager*	m_lpEnemys;			// �G�}�l�[�W��
		CPlanktonManager*	m_lpPlanktons;		// �v�����N�g��
		CUI*				m_lpUI;				// �C���^�[�t�F�C�X
		CStoneManager*		m_lpStones;			// ��
		CLimitWalls			m_Walls;			// ��
		CFishManager*		m_lpFishes;			// ��
		CSurfaceSea*		m_lpSurfaceSea;		// �C��
		CXfile*				m_lpSky;			// ��̃��b�V��
		CVector				m_vSavePos;			// �O�̃J�����ʒu�ۑ��p
		boost::timer		m_Timer;			// �^�C�}�[
		CSprite*			m_lpSprite;			// �X�v���C�g
		CSunLineManager*	m_lpSunLines;		// ���z���}�l�[�W���[
	
		CBoard				m_BottomSea;		// �C��p�|���S��
		CBoard				m_SceneBorad;		// �V�[���`��p�|���S��

		CTexture			m_SceneTex;			// �I�t�X�N���[���p�e�N�X�`��
		CTexture			m_MiniSizeTex;		// �k���p�����_�[�e�N�X�`��
		CTexture			m_BlurTex;			// �ڂ����p�����_�[�e�N�X�`��
		CTexture			m_DepthTex;			// �[�x�ۑ��p�����_�[�e�N�X�`��
		CTexture			m_ResultTex;		// �ŏI�I�ɕ`�悷��e�N�X�`��

		CEffect				m_BlurShader;		// �ڂ����V�F�[�_�[
		CEffect				m_DepthShader;		// �[�x�v�Z�V�F�[�_�[
		CEffect				m_DOFShader;		// ��ʊE�[�x�V�F�[�_�[
		CEffect				m_BombBlurShader;	// �����u���[�V�F�[�_�|

		LPDIRECT3DSURFACE9	m_lpBackBuffer;		// �o�b�N�o�b�t�@�ۑ��p
		LPDIRECT3DSURFACE9	m_lpZBuffer;		// Z�o�b�t�@�ۑ��p
		D3DVIEWPORT9		m_VP;				// �r���[�|�[�g�ۑ�
		short				m_DamageAlphaCnt;	// �_���[�W�̃A���t�@�l
		short				m_GameSceneAlpha;	// �Q�[����ʂ̃A���t�@�l
		BYTE				m_StageIndex;		// �X�e�[�W
		BYTE				m_EnemyNum;			// �ŏ��ɂ���G��

		FLGTYPE				m_Flg;				// �t���O
		DWORD				m_SaveEnterBossTime;// �{�X���掞�ԋL�^�p

		std::string			m_BGMPath;			// BGM�̃t�@�C���p�X

		CProj				m_Proj;				// ���e
		static CCamera		m_Camera;			// �J����(�O���[�o�����̈�)
		
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
		//	�E�֐���	Collision
		//	�E����		�Փ˔���A�������s��
		//	�E����		�Ȃ�	
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Collision();

		//////////////////////////////////////////////////
		//	�E�֐���	Draw
		//	�E����		�`����s��
		//	�E����		�Ȃ�	
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Draw();

		//////////////////////////////////////////////////
		//	�E�֐���	DrawMesh
		//	�E����		���b�V���`����s��
		//				true��n���ƃv���O���}�u���V�F�[�_��
		//				�`����s��
		//	�E����		
		//		- lp_effect	�v���O���}�u���V�F�[�_��
		//					�`����s�����ǂ���
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawMesh( bool is_effect );

		//////////////////////////////////////////////////
		//	�E�֐���	DrawPolygon
		//	�E����		���b�V���ȊO��3D�I�u�W�F�N�g�`����s��
		//				true��n���ƃv���O���}�u���V�F�[�_��
		//				�`����s��
		//	�E����		
		//		- lp_effect	�v���O���}�u���V�F�[�_��
		//					�`����s�����ǂ���
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawPolygon( bool is_effect );

		//////////////////////////////////////////////////
		//	�E�֐���	DrawEffect
		//	�E����		�e�G�t�F�N�g�`����s��
		//				Z�o�b�t�@�֘A��؂��ĕ`����s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawEffect();

		//////////////////////////////////////////////////
		//	�E�֐���	DrawSprite
		//	�E����		�X�v���C�g�`����s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawSprite();

		//////////////////////////////////////////////////
		//	�E�֐���	Rendering
		//	�E����		DrawMesh��DrawPolygon��DrawEffect��
		//				���Ԃŕ`����s��
		//				true��n���ƃv���O���}�u���V�F�[�_��
		//				�`����s��
		//	�E����		
		//		- lp_effect	�v���O���}�u���V�F�[�_��
		//					�`����s�����ǂ���
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Rendering( bool is_effect );

		//////////////////////////////////////////////////
		//	�E�֐���	Load
		//	�E����		�ǂݍ��݂␶�����s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Load();

		//////////////////////////////////////////////////
		//	�E�֐���	LoadStage
		//	�E����		�X�e�[�W���Ƃ̃f�[�^�ǂݍ��݂��s��
		//	�E����		
		//		- stage	�X�e�[�W�̃C���f�b�N�X�ԍ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void LoadStage( BYTE stage );

		//////////////////////////////////////////////////
		//	�E�֐���	Allocs
		//	�E����		�����o�ϐ��̃������m�ۂ��s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Allocs();

		//////////////////////////////////////////////////
		//	�E�֐���	Release
		//	�E����		����Ɣj����s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Release();
	};
}

#endif // __AKI_GAMEMAIN_H__	