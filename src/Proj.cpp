/////////////////////////////////
// �C���N���[�h
/////////////////////////////////
#include "StdAfx.h"
#include "Proj.h"

/////////////////////////////////
// ����
/////////////////////////////////

// �R���X�g���N�^
CProj::CProj( float fovy, float aspect, float zn, float zf )
{
	SetProj( fovy, aspect, zn, zf );
}

// �j�A�N���b�v�ݒ�
const D3DXMATRIX* CProj::SetNear( float zn )
{
	return SetProj( m_Fovy, m_Aspect, zn, m_Far );
}

// �t�@�[�N���b�v�ݒ�
const D3DXMATRIX* CProj::SetFar( float zf )
{
	return SetProj( m_Fovy, m_Aspect, m_Near, zf );
}

// ��p�ݒ�
const D3DXMATRIX* CProj::SetFovy( float fovy )
{
	return SetProj( fovy, m_Aspect, m_Near, m_Far );
}

// �A�X�y�N�g���ݒ�
const D3DXMATRIX* CProj::SetAspect( float aspect )
{
	return SetProj( m_Fovy, aspect, m_Near, m_Far );
}

// �S���ݒ�
const D3DXMATRIX* CProj::SetProj( float fovy, float aspect, float zn, float zf )
{
	m_Fovy	= fovy;
	m_Aspect= aspect;
	m_Near	= zn;
	m_Far	= zf;

	D3DXMatrixPerspectiveFovLH( &m_mMat, m_Fovy, m_Aspect, m_Near, m_Far );

	return &m_mMat;
}

// �f�X�g���N�^
CProj::~CProj()
{
}
