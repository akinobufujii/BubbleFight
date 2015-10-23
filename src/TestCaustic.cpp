#include "StdAfx.h"
#include "TestCaustic.h"

using namespace Sequence;

CTestCaustic::CTestCaustic(void)
{
	m_CausticShader.Load( D3D::GetDevice(), "FX/Caustic.fx", "Tec" );
	m_WaveShader.Load( D3D::GetDevice(), "FX/Wave.fx", "Tec" );
	m_BlurShader.Load( D3D::GetDevice(), "FX/Blur.fx", "Tec" );

	//m_Mesh.Load( D3D::GetDevice(), "XFile/wall.x" );
	m_Stage.Load( D3D::GetDevice(), "XFile/stage.x" );

	m_BumpTex.CreateRenderTarget( D3D::GetDevice(), 512, 512  );
	m_CausticTex.CreateRenderTarget( D3D::GetDevice(), 512, 512 );
	m_WaveTex.CreateRenderTarget( D3D::GetDevice(), 512, 512 );
	m_BlurTex.CreateRenderTarget( D3D::GetDevice(), 512, 512 );

	// ライト設定
	D3DLIGHT9	Light;
	ZeroMemory( &Light, sizeof( Light ) );

	Light.Type	= D3DLIGHT_DIRECTIONAL;	// 平行光源
	Light.Diffuse.r	= 1.0f;
	Light.Diffuse.g	= 1.0f;
	Light.Diffuse.b	= 1.0f;

	Light.Position.x= 0.0f;
	Light.Position.y= 1.0f;
	Light.Position.z= 0.0f;

	Light.Range	= 10.f;

	Light.Direction = D3DXVECTOR3( 1.0f, -1.0f, 1.0f);

	D3D::GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	D3D::GetDevice()->SetLight( 0, &Light );
	D3D::GetDevice()->LightEnable( 0, TRUE );

	D3D::GetDevice()->GetRenderTarget( 0, &m_lpBackBuffer );
	D3D::GetDevice()->GetDepthStencilSurface( &m_lpZBuffer );
}

CTestCaustic::~CTestCaustic(void)
{
	SAFE_RELEASE( m_lpBackBuffer );
	SAFE_RELEASE( m_lpZBuffer );
}

// 処理
bool CTestCaustic::RunProc()
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	m_Camera.SetCameraPos( &D3DXVECTOR3( 0, 0, -15 ) );

	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );
	lpDevice->SetTransform( D3DTS_VIEW,	m_Camera.GetMatrix() );

	// 描画開始
	D3D::Begin();

	lpDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		0xff0000ff, 1.f, 0 );

	m_WaveShader.Begin();

	// 波の法線マップ更新
	m_WaveShader.BeginPass( 0 );

	D3DVIEWPORT9 SaveVp, Vp = { 0, 0, 512, 512, 0, 1.f };

	lpDevice->GetViewport( &SaveVp );
	lpDevice->SetViewport( &Vp );

	lpDevice->SetRenderTarget( 0, m_WaveTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_WaveTex.GetZbuffer() );
	m_WaveShader.SetTexture( "g_Texture", m_WaveTex.GetTexture() );

	D3DXVECTOR2	Pos( rand() % 100 * 0.01f, rand() % 100 * 0.01f );
	m_WaveShader.SetValue( "g_AddWavePos", &Pos, sizeof( D3DXVECTOR2 ) );

	m_Board.SetVertex( 2 );
	m_Board.Draw( lpDevice );

	m_WaveShader.EndPass();

	// 波の法線マップ作成
	m_WaveShader.BeginPass( 1 );

	lpDevice->SetRenderTarget( 0, m_BumpTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_BumpTex.GetZbuffer() );
	m_WaveShader.SetTexture( "g_Texture", m_WaveTex.GetTexture() );

	m_Board.SetVertex( 2 );
	m_Board.Draw( lpDevice );

	m_WaveShader.EndPass();

	m_WaveShader.End();

	// ぼかす
	m_BlurShader.Begin();
	m_BlurShader.BeginPass( 1 );

	lpDevice->SetRenderTarget( 0, m_BlurTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_BlurTex.GetZbuffer() );
	m_BlurShader.SetTexture( "g_Texture", m_BumpTex.GetTexture() );

	m_BlurShader.SetValue( "g_Offset", &D3DXVECTOR2( 512, 512 ), sizeof( D3DXVECTOR2 ) );

	m_Board.SetVertex( 2 );
	m_Board.Draw( lpDevice );

	m_BlurShader.EndPass();
	m_BlurShader.End();

	m_CausticShader.Begin();

	// コースティックマップ作成
	m_CausticShader.BeginPass( 0 );

	lpDevice->SetRenderTarget( 0, m_CausticTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_CausticTex.GetZbuffer() );
	m_CausticShader.SetTexture( "g_NormalTex", m_BumpTex.GetTexture() );

	m_Board.SetVertex( 2 );
	m_Board.Draw( lpDevice );

	m_CausticShader.EndPass();

	// コースティックマップ適応
	lpDevice->SetRenderTarget( 0, m_lpBackBuffer );
	lpDevice->SetDepthStencilSurface( m_lpZBuffer );
	lpDevice->SetViewport( &SaveVp );

	CMatrix mRotY, mTrans( 0, -10, 10 ), mScale;

	mRotY.SetRotY( -45 /*timeGetTime() / 100 % 360*/ );
	mScale.SetScale( 5 );

	m_CausticShader.SetValue( "g_World", &(mScale * mRotY * mTrans), sizeof( D3DXMATRIX ) );
	m_CausticShader.SetValue( "g_View", m_Camera.GetMatrix(), sizeof( D3DXMATRIX ) );
	m_CausticShader.SetValue( "g_Proj", m_Proj.GetMatrix(), sizeof( D3DXMATRIX ) );
	m_CausticShader.SetValue( "g_LightDir", &D3DXVECTOR3( 1.0f, -1.0f, 1.0f ), sizeof( D3DXVECTOR3) );

	m_CausticShader.SetTexture( "g_CausticTex", m_CausticTex.GetTexture() );
	m_Mesh.DrawShader( &m_CausticShader, NULL, "g_Texture", 1, 1 );

	m_CausticShader.End();

	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpDevice->SetTexture( 0, m_CausticTex.GetTexture() );
	m_Board.SetVertex( 6 );
	//m_Board.Draw( lpDevice );

	lpDevice->SetTransform( D3DTS_WORLD, &mScale );
	m_Stage.Draw( lpDevice );

	// 描画終了
	D3D::End();

	return true;
}