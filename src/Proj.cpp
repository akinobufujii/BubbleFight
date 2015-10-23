/////////////////////////////////
// インクルード
/////////////////////////////////
#include "StdAfx.h"
#include "Proj.h"

/////////////////////////////////
// 実装
/////////////////////////////////

// コンストラクタ
CProj::CProj( float fovy, float aspect, float zn, float zf )
{
	SetProj( fovy, aspect, zn, zf );
}

// ニアクリップ設定
const D3DXMATRIX* CProj::SetNear( float zn )
{
	return SetProj( m_Fovy, m_Aspect, zn, m_Far );
}

// ファークリップ設定
const D3DXMATRIX* CProj::SetFar( float zf )
{
	return SetProj( m_Fovy, m_Aspect, m_Near, zf );
}

// 画角設定
const D3DXMATRIX* CProj::SetFovy( float fovy )
{
	return SetProj( fovy, m_Aspect, m_Near, m_Far );
}

// アスペクト比を設定
const D3DXMATRIX* CProj::SetAspect( float aspect )
{
	return SetProj( m_Fovy, aspect, m_Near, m_Far );
}

// 全部設定
const D3DXMATRIX* CProj::SetProj( float fovy, float aspect, float zn, float zf )
{
	m_Fovy	= fovy;
	m_Aspect= aspect;
	m_Near	= zn;
	m_Far	= zf;

	D3DXMatrixPerspectiveFovLH( &m_mMat, m_Fovy, m_Aspect, m_Near, m_Far );

	return &m_mMat;
}

// デストラクタ
CProj::~CProj()
{
}
