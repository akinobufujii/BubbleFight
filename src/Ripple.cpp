//////////////////////////////
// �C���N���[�h
//////////////////////////////
#include "Ripple.h"

//////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////
const float		CRipple::FADEOUT_SPEED		= 0.05f;
const float		CRipple::NEXT_RIPPLE_LINE	= 0.3f;
const float		CRipple::DRAW_UPDOWN_POS	= 0.01f;

//////////////////////////////
// ����
//////////////////////////////

// �R���X�g���N�^
CRipple::CRipple():
m_NowAlpha( 1.f ),
m_Width( 10 ),
m_Height( 10 )
{
	m_Borad.SetColor( 1.f, 1.f, 1.f );
	m_Borad.SetAlpha( 1.f );
}

// �f�X�g���N�^
CRipple::~CRipple()
{
}

// �X�V
void CRipple::Update()
{
	// ���񂾂�A���t�@�l�������Ă���
	if( (m_NowAlpha -= FADEOUT_SPEED) < 0 )
	{	
		m_NowAlpha = 0.f;
	}

	// �傫���͑傫�����Ă���
	m_Width += m_ScaleRate;
	m_Height+= m_ScaleRate;
}

// �㑤�ɕ`��
void CRipple::DrawUp(const LPDIRECT3DDEVICE9 lp_device)
{
	// �����㑤�ɐݒ�
	D3DXVECTOR3 UpPos = m_Pos;
	UpPos.y += DRAW_UPDOWN_POS;

	// �o���ʒu������
	lp_device->SetTransform( D3DTS_WORLD, &(m_mRotXY * CMatrix( UpPos )) );

	// �`�撼�O�ɒ��_�ݒ������
	m_Borad.SetAlpha( m_NowAlpha );
	m_Borad.SetVertex( m_Width, m_Height );

	// �`��
	m_Borad.Draw( lp_device );
}

// �����ɕ`��
void CRipple::DrawDown(const LPDIRECT3DDEVICE9 lp_device)
{
	// �����㑤�ɐݒ�
	D3DXVECTOR3 DownPos = m_Pos;
	DownPos.y -= DRAW_UPDOWN_POS;

	// �o���ʒu������
	lp_device->SetTransform( D3DTS_WORLD, &(m_mRotXY * CMatrix( DownPos )) );

	// �`�撼�O�ɒ��_�ݒ������
	m_Borad.SetAlpha( m_NowAlpha );
	m_Borad.SetVertex( m_Width, m_Height );

	// �`��
	m_Borad.Draw( lp_device );
}