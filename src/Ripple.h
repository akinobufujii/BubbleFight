#ifndef __AKI_REPPLE_H__
#define __AKI_REPPLE_H__

//////////////////////////////
// �g��N���X
//////////////////////////////
class CRipple
{
public:	// �O�����J

	//////////////////////////////
	// �萔
	//////////////////////////////
	CONSTANT float	FADEOUT_SPEED;		// �t�F�[�h�A�E�g�̑���
	CONSTANT float	NEXT_RIPPLE_LINE;	// ���̔g������{�[�_�[���C��
	CONSTANT float	DRAW_UPDOWN_POS;	// �㉺�̔����Ȍ덷

	//////////////////////////////
	// ���\�b�h
	//////////////////////////////

	// �R���X�g���N�^
	CRipple();

	// �f�X�g���N�^
	~CRipple();

	//////////////////////////////////////////////////
	//	�E�֐���	Update
	//	�E����		�X�V���s��
	//	�E����		�Ȃ�
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Update();

	//////////////////////////////////////////////////
	//	�E�֐���	DrawUp
	//	�E����		�C�ʂ̏㑤�̕`����s��
	//	�E����		
	//		- lp_device	�f�o�C�X
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void DrawUp( const LPDIRECT3DDEVICE9 lp_device );

	//////////////////////////////////////////////////
	//	�E�֐���	DrawDown
	//	�E����		�C�ʂ̉����̕`����s��
	//	�E����		
	//		- lp_device	�f�o�C�X
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void DrawDown( const LPDIRECT3DDEVICE9 lp_device );

	//////////////////////////////////////////////////
	//	�E�֐���	SetPos
	//	�E����		�o��������ʒu��ݒ肷��
	//	�E����		
	//		- pos	�ʒu
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetPos( const D3DXVECTOR3& pos )
	{
		m_Pos = pos;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetRotY
	//	�E����		�g���Y����]������p�x
	//	�E����		
	//		- degree	�p�x
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetRotY( const float degree )
	{
		CMatrix mXRot;
		mXRot.SetRotX( 90 );

		m_mRotXY.SetRotY( degree );

		m_mRotXY = mXRot * m_mRotXY;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetSize
	//	�E����		�g��̕�������ݒ肷��
	//	�E����		
	//		- width		��
	//		- height	����
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetSize( const float width, const float height )
	{
		m_Width = width;
		m_Height= height;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetScaleRate
	//	�E����		�g��̍L�����Ă����傫����ݒ肷��
	//	�E����		
	//		- rate	�傫��
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetScaleRate( const float rate )
	{
		m_ScaleRate = rate;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	IsNext
	//	�E����		���̔g�������ė~�������ǂ�����
	//				���ׂ�
	//	�E����		
	//		- rate	�傫��
	//	�E�߂�l	���𐶂ݏo���ė~�����Ȃ�true
	//				�����łȂ����false��Ԃ�
	//////////////////////////////////////////////////
	inline bool IsNext() const
	{
		return (NEXT_RIPPLE_LINE > m_NowAlpha)? true : false;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	IsErase
	//	�E����		���̔g��������ė~�������ǂ�����
	//				���ׂ�
	//	�E����		
	//		- rate	�傫��
	//	�E�߂�l	�����ׂ��Ȃ�true
	//				�����łȂ����false��Ԃ�
	//////////////////////////////////////////////////
	inline bool IsErase() const 
	{
		return (m_NowAlpha <= 0)? true : false;
	}

private:	// ����J

	//////////////////////////////
	// �t�B�[���h
	//////////////////////////////
	CBoard	m_Borad;	// �|���S��
	float	m_NowAlpha;	// ���̃A���t�@�l(0.0�`1.0)
	float	m_Width;	// ��
	float	m_Height;	// ����
	float	m_ScaleRate;// �g��̔䗦
	CVector	m_Pos;		// �ʒu
	CMatrix	m_mRotXY;	// X���AY���̉�]���������s��
};

#endif	// __AKI_REPPLE_H__