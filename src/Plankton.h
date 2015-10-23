#ifndef __AKI_PLANKTON_H__
#define __AKI_PLANKTON_H__

////////////////////////////////
// �v�����N�g��
// (�Y���Ă��邽���̃I�u�W�F�N�g)
////////////////////////////////
class CPlankton
{
public:	// �O�����J

	////////////////////////////////
	// �萔
	////////////////////////////////
	CONSTANT	float	SIZE;	// �傫��

	////////////////////////////////
	// ���\�b�h
	////////////////////////////////
	
	// �R���X�g���N�^
	CPlankton();

	// �f�X�g���N�^
	~CPlankton();

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
	//		- lp_device	�f�o�C�X
	//		- camera	�r���{�[�h����Ɏg�p����J����
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Draw( const LPDIRECT3DDEVICE9 lpdevice, const CCamera* camera );

private:	// ����J

	////////////////////////////////
	// �t�B�[���h
	////////////////////////////////
	CVector				m_vPos;		// �ʒu
	static CBoard		m_Board;	// �|���S��
};

#endif // __AKI_PLANKTON_H__