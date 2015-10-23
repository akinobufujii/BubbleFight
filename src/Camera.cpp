/////////////////////////////////
// インクルード
/////////////////////////////////
#include "Camera.h"

/////////////////////////////////
// 実装
/////////////////////////////////

// コンストラクタ
CCamera::CCamera()
{
	m_vUp.y = 1;
}

// 初期値あり
CCamera::CCamera( const D3DXVECTOR3* pos, const D3DXVECTOR3* look, const D3DXVECTOR3* up )
{
	SetCamera( pos, look, up );
}

// 視点位置を設定
const D3DXMATRIX* CCamera::SetCameraPos( const D3DXVECTOR3* pos )
{
	m_vPos	= *pos;
	
	D3DXMatrixLookAtLH( &m_mMat, &m_vPos, &m_vlook, &m_vUp );

	return &m_mMat;
}

// 注視点の設定
const D3DXMATRIX* CCamera::SetCameraLook( const D3DXVECTOR3* look )
{
	m_vlook	= *look;
	
	D3DXMatrixLookAtLH( &m_mMat, &m_vPos, &m_vlook, &m_vUp );

	return &m_mMat;
}

// 上方向の設定
const D3DXMATRIX* CCamera::SetCameraUp( const D3DXVECTOR3* up )
{
	m_vUp	= *up;

	D3DXMatrixLookAtLH( &m_mMat, &m_vPos, &m_vlook, &m_vUp );

	return &m_mMat;
}

// カメラを設定(すべての内容)
const D3DXMATRIX* CCamera::SetCamera( const D3DXVECTOR3* pos, const D3DXVECTOR3* look, const D3DXVECTOR3* up )
{
	m_vPos	= *pos;
	m_vlook	= *look;
	m_vUp	= *up;

	D3DXMatrixLookAtLH( &m_mMat, &m_vPos, &m_vlook, &m_vUp );

	return &m_mMat;
}

// ビルボードの為の行列獲得
void CCamera::GetBillboard( LPD3DXMATRIX out, const LPD3DXMATRIX mat ) const 
{
	D3DXVECTOR3 Pos(0, 0, 0), Up(0, 1, 0);	// 位置、上方向

	// 座標を割り出す
	D3DXVec3TransformCoord( &Pos, &Pos, mat );

	// 上方向を割り出す
	D3DXVec3TransformNormal( &Up, &Up, mat );

	// ビルボードの為の回転計算
	D3DXMatrixLookAtLH( out, &m_vPos, &Pos, &Up );
	//D3DXMatrixLookAtLH( out, &D3DXVECTOR3( 0, 0, 1 ), &D3DXVECTOR3( 0, 0, 0 ), &D3DXVECTOR3( 0,1, 0) );
	D3DXMatrixInverse( out, NULL, out );

	// ※回転行列だけにする
	out->_41 = 0.f;
	out->_42 = 0.f;
	out->_43 = 0.f;
}

// 今向いているベクトル
D3DXVECTOR3 CCamera::GetScreenVector() const	
{
	D3DXVECTOR3 Vec;
	Vec = m_vlook - m_vPos;
	D3DXVec3Normalize( &Vec, &Vec );

	return Vec;
}

// デストラクタ
CCamera::~CCamera(void)
{
}
