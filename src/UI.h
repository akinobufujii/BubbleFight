#ifndef __AKI_UI_H__
#define __AKI_UI_H__

//////////////////////////////////////////////////
// �s���S�^�錾
//////////////////////////////////////////////////
namespace Enemy
{
	class CManager;
}

//////////////////////////////////////////////////
// ���[�U�[�C���^�[�t�F�C�X
//////////////////////////////////////////////////
class CUI
{
public:	// �O�����J

	//////////////////////////////////////////////////
	// �萔
	//////////////////////////////////////////////////
	CONSTANT int	FONT_WIDTH;			// �t�H���g�摜�̕�
	CONSTANT int	FONT_HEIGHT;		// �t�H���g�摜�̍���
	CONSTANT int	NUMBER_WIDTH;		// �ԍ��摜�̕�
	CONSTANT int	NUMBER_HEIGHT;		// �ԍ��摜�̍���
	CONSTANT int	LIFEGAGE_WIDTH;		// ���C�t�Q�[�W��
	CONSTANT int	LIFEGAGE_HEIGHT;	// ���C�t�Q�[�W����
	CONSTANT int	POWERGAGE_WIDTH;	// �p���[�Q�[�W��
	CONSTANT int	POWERGAGE_HEIGHT;	// �p���[�Q�[�W����
	CONSTANT int	ENEMYPOSTEX_SIZE;	// ���[�_�[�̓G�摜�T�C�Y
	CONSTANT int	RADAR_SIZE;			// ���[�_�[�摜�T�C�Y
	CONSTANT int	BLESSGAGE_WIDTH;	// ���Q�[�W��
	CONSTANT int	BLESSGAGE_HEIGHT;	// ���Q�[�W����
	CONSTANT float	SEARCH_RADIUS;		// ���[�_�[�̊��m���a

	//////////////////////////////////////////////////
	// ���\�b�h
	//////////////////////////////////////////////////

	// �R���X�g���N�^
	CUI();

	// �f�X�g���N�^
	~CUI();

	//////////////////////////////////////////////////
	//	�E�֐���	Update
	//	�E����		�X�V���s��
	//	�E����		
	//		- score			���݂̃X�R�A
	//		- hp			�v���C���[��HP
	//		- power			�v���C���[�̂��߂Ă��鋭��
	//		- bless			�v���C���[�̑��c��
	//		- now_enemys	���̓G�̐�
	//		- all_enemys	�G�̑���
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Update(
		ULONG	score,
		USHORT	hp,
		float	power,
		USHORT	bless,
		BYTE	now_enemys,
		BYTE	all_enemys);

	//////////////////////////////////////////////////
	//	�E�֐���	Draw
	//	�E����		�`����s��
	//	�E����		
	//		- lpsprite	�X�v���C�g�f�o�C�X
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Draw( const LPD3DXSPRITE lpsprite );

	//////////////////////////////////////////////////
	//	�E�֐���	DrawRadar
	//	�E����		���[�_�[�̕`����s��
	//	�E����		
	//		- lpsprite		�X�v���C�g�f�o�C�X
	//		- lp_player_mat	�v���C���[�̍s��
	//		- lp_enemys		�G�̏�񂪓��������X�g
	//		- lp_boss		�{�X�̈ʒu(NULL�����Ε\������Ȃ�)
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void DrawRadar(
		const LPD3DXSPRITE lpsprite, 
		const D3DXMATRIX* lp_player_mat,
		Enemy::CManager* lp_enemys,
		const Boss::CBase* lp_boss );

	//////////////////////////////////////////////////
	//	�E�֐���	SetAlpha
	//	�E����		�A���t�@�l�ݒ���s��
	//	�E����		
	//		- alpha	�A���t�@�l(0�`255)
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetAlpha( int alpha )
	{
		m_Alpha = alpha;
	}

private:	// ����J

	//////////////////////////////////////////////////
	// �t�B�[���h
	//////////////////////////////////////////////////
	CSTexManager*	m_lpTexManager;	// �e�N�X�`���}�l�[�W��
	ULONG			m_Score;		// �X�R�A
	USHORT			m_HP;			// HP
	USHORT			m_Bless;		// ��
	float			m_Power;		// �p���[
	int				m_Alpha;		// �A���t�@�l
	BYTE			m_NowEnemys;	// ���̓G��
	BYTE			m_AllEnemys;	// �G����
};

#endif // __AKI_UI_H__