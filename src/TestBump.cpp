#include "StdAfx.h"
#include "TestBump.h"

using namespace Sequence;

CTestBump::CTestBump(void)
{
	// シェーダー読み込み
	m_BumpShader.Load( D3D::GetDevice(), "FX/PointSprite.fx", "Tec" );

	m_Tex.CreateFileA( D3D::GetDevice(), "GameOver.png", NULL );
	m_NormalTex.CreateFileA( D3D::GetDevice(), "SurfaceSeaBump.jpg", NULL );
	m_BumpBoard.SetVertex( 3 );
}

CTestBump::~CTestBump(void)
{
}

// 処理
bool CTestBump::RunProc()
{
	D3D::Begin();

	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	lpDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DXCOLOR( 1.f, 1.f, 1.f, 1.f ), 1, 0 );

	m_Camera.SetCameraPos( &D3DXVECTOR3( 0, 0, -5 ) );

	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );
	lpDevice->SetTransform( D3DTS_VIEW, m_Camera.GetMatrix() );

	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );

	lpDevice->SetTexture( 0, m_Tex.GetTexture() );

	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	CMatrix mWorld, mInv;

	static float	Height = 0;
	static bool		Flg = true;

	if( Flg )
	{
		if( (Height += 0.1f) > 5 )
		{
			Flg = false;
		}
	}else{
		if( (Height -= 0.1f) < 0 )
		{
			Flg = true;
		}
	}
	
	// シェーダー描画開始
	m_BumpShader.Begin();
	m_BumpShader.BeginPass( 0 );
	m_BumpBoard.SetAlpha( 0.5f );

	m_BumpShader.SetValue( "g_View", &CMatrix(), sizeof(D3DXMATRIX) );
	m_BumpShader.SetValue( "g_Proj", &CMatrix(), sizeof(D3DXMATRIX) );
	m_BumpShader.SetValue( "g_Height", &Height, sizeof(float) );

	mWorld.SetTrans( -1.5f, 0, 0 );
	mInv = mWorld;
	mInv.GetInverse();

	m_BumpShader.SetValue( "g_World", &CMatrix(), sizeof(D3DXMATRIX) );
	m_BumpShader.SetValue( "g_WorldInv", &CMatrix(), sizeof(D3DXMATRIX) );

	m_BumpShader.SetTexture( "g_Texture", m_Tex.GetTexture() );
	m_BumpShader.SetTexture( "g_NormalTex", m_NormalTex.GetTexture() );

	static int	Cnt = 0;
	static const float S = 1.f / 500.f;

	Cnt = ++Cnt % 500;

	float R = Cnt * S;

	m_BumpBoard.SetTexturePos(
		&D3DXVECTOR2( 0 + R, 0 ),
		&D3DXVECTOR2( 1 + R, 0 ),
		&D3DXVECTOR2( 1 + R, 1 ),
		&D3DXVECTOR2( 0 + R, 1 ) );

	D3DXVECTOR4 LightVec( 0, 1, -1, 1 );

	D3DXVec4Normalize( &LightVec, &LightVec );

	CMatrix mRot;
	//mRot.SetRotY( timeGetTime() / 10 % 360 );

	D3DXVec4Transform( &LightVec, &LightVec, &mRot );
	
	m_BumpShader.SetValue( "g_LightDir", &LightVec, sizeof(D3DXVECTOR4) );

	m_BumpBoard.Draw( lpDevice );

	// 反対に当てた光も描画
	LightVec *= -1;

	m_BumpShader.SetValue( "g_LightDir", &LightVec, sizeof(D3DXVECTOR4) );

	mWorld.SetTrans( 1.5f, 0, 0 );
	mInv = mWorld;
	mInv.GetInverse();

	m_BumpShader.SetValue( "g_World", &mWorld, sizeof(D3DXMATRIX) );
	m_BumpShader.SetValue( "g_WorldInv", &mInv, sizeof(D3DXMATRIX) );
	//m_BumpBoard.Draw( lpDevice );

	m_BumpShader.EndPass();
	m_BumpShader.End();

	D3D::End();

	return true;
}