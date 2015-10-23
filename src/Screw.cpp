// インクルード
#include "Screw.h"

// コンストラクタ
CScrew::CScrew()
{
	m_lpMesh = NEW CXfile( D3D::GetDevice(), "Fin.x" );
}

// 更新
void CScrew::Update( const D3DXMATRIX& mat, float speed )
{
	static float Cnt = 0;

	CMatrix RotZ, Trans;

	Trans.MoveLocal( 0, 0, -1 );

	if( (Cnt += speed * 100) > 360 )
	{
		Cnt = 0;
	}

	RotZ.RotateLocalZ( Cnt );

	m_mMat = RotZ * Trans * mat;
}

// 描画
void CScrew::Draw(const LPDIRECT3DDEVICE9 &lpdevice)
{
	lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
	m_lpMesh->Draw( lpdevice );
}

CScrew::~CScrew()
{
	SAFE_DELETE( m_lpMesh );
}
