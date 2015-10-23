/////////////////////////////////
// �C���N���[�h
/////////////////////////////////
#include "Camera.h"

/////////////////////////////////
// ����
/////////////////////////////////

// �R���X�g���N�^
CCamera::CCamera()
{
	m_vUp.y = 1;
}

// �����l����
CCamera::CCamera( const D3DXVECTOR3* pos, const D3DXVECTOR3* look, const D3DXVECTOR3* up )
{
	SetCamera( pos, look, up );
}

// ���_�ʒu��ݒ�
const D3DXMATRIX* CCamera::SetCameraPos( const D3DXVECTOR3* pos )
{
	m_vPos	= *pos;
	
	D3DXMatrixLookAtLH( &m_mMat, &m_vPos, &m_vlook, &m_vUp );

	return &m_mMat;
}

// �����_�̐ݒ�
const D3DXMATRIX* CCamera::SetCameraLook( const D3DXVECTOR3* look )
{
	m_vlook	= *look;
	
	D3DXMatrixLookAtLH( &m_mMat, &m_vPos, &m_vlook, &m_vUp );

	return &m_mMat;
}

// ������̐ݒ�
const D3DXMATRIX* CCamera::SetCameraUp( const D3DXVECTOR3* up )
{
	m_vUp	= *up;

	D3DXMatrixLookAtLH( &m_mMat, &m_vPos, &m_vlook, &m_vUp );

	return &m_mMat;
}

// �J������ݒ�(���ׂĂ̓��e)
const D3DXMATRIX* CCamera::SetCamera( const D3DXVECTOR3* pos, const D3DXVECTOR3* look, const D3DXVECTOR3* up )
{
	m_vPos	= *pos;
	m_vlook	= *look;
	m_vUp	= *up;

	D3DXMatrixLookAtLH( &m_mMat, &m_vPos, &m_vlook, &m_vUp );

	return &m_mMat;
}

// �r���{�[�h�ׂ̈̍s��l��
void CCamera::GetBillboard( LPD3DXMATRIX out, const LPD3DXMATRIX mat ) const 
{
	D3DXVECTOR3 Pos(0, 0, 0), Up(0, 1, 0);	// �ʒu�A�����

	// ���W������o��
	D3DXVec3TransformCoord( &Pos, &Pos, mat );

	// �����������o��
	D3DXVec3TransformNormal( &Up, &Up, mat );

	// �r���{�[�h�ׂ̈̉�]�v�Z
	D3DXMatrixLookAtLH( out, &m_vPos, &Pos, &Up );
	//D3DXMatrixLookAtLH( out, &D3DXVECTOR3( 0, 0, 1 ), &D3DXVECTOR3( 0, 0, 0 ), &D3DXVECTOR3( 0,1, 0) );
	D3DXMatrixInverse( out, NULL, out );

	// ����]�s�񂾂��ɂ���
	out->_41 = 0.f;
	out->_42 = 0.f;
	out->_43 = 0.f;
}

// �������Ă���x�N�g��
D3DXVECTOR3 CCamera::GetScreenVector() const	
{
	D3DXVECTOR3 Vec;
	Vec = m_vlook - m_vPos;
	D3DXVec3Normalize( &Vec, &Vec );

	return Vec;
}

// �f�X�g���N�^
CCamera::~CCamera(void)
{
}
