#include "StdAfx.h"

#include "SequenceBase.h"
#include "DOFText.h"

using namespace Sequence;

CDOFText::CDOFText(void)
{
	m_Scene.SetVertex( 10 );
	m_Scene.SetGreen( 5.f );
	m_Mesh.Load( D3D::GetDevice(), "XFile/Chara.x" );
	m_Sphere.Load( D3D::GetDevice(), "XFile/stage.x");

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

	// レンダリングターゲット作成
	// シーン描画用
	m_SceneTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT );

	// 縮小用
	m_MiniSizeTex.CreateRenderTarget( 
		D3D::GetDevice(), WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4 );

	// ぼかし用
	m_BlurTex.CreateRenderTarget(
		D3D::GetDevice(), WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4 );

	// 深度計算用
	m_DepthTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT );

	// シェーダー読み込み
	// ぼかし用
	m_BlurShader.Load( D3D::GetDevice(), "FX/Blur.fx", "Tec" );

	// 深度計算用
	m_DepthShader.Load( D3D::GetDevice(), "FX/Depth.fx", "Tec" );

	// 被写界深度
	m_DOFShader.Load( D3D::GetDevice(), "FX/DOF.fx", "Tec" );

	CXfile::SetShaderDevice( &m_DepthShader, "g_World" );
}

CDOFText::~CDOFText(void)
{
}

// 処理
bool CDOFText::RunProc()
{
	Draw();
	return true;
}

// 描画
void CDOFText::Draw()
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// シーン描画開始
	D3D::Begin();

	// カメラ、投影設定
	m_Camera.SetCameraPos( &D3DXVECTOR3( 0, 0, -10 ) );
	lpDevice->SetTransform( D3DTS_VIEW, m_Camera.GetMatrix() );
	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );

	// バックバッファ保存
	lpDevice->GetRenderTarget( 0, &m_lpBackBuffer );
	lpDevice->GetDepthStencilSurface( &m_lpZBuffer );

	CXfile::SetDrawShader( false );

	// パス1
	// テクスチャに描画
	// レンダーターゲット変更
	lpDevice->SetRenderTarget( 0, m_SceneTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_SceneTex.GetZbuffer() );

	lpDevice->Clear(
		0,NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0, 0xff, 0xff ), 1.0f, 0);

	lpDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_FOGCOLOR, D3DCOLOR_XRGB( 0, 0x00, 0x80 ) );
	lpDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_LINEAR );
	lpDevice->SetRenderState( D3DRS_FOGSTART, GetAddress( 0.f ) );
	lpDevice->SetRenderState( D3DRS_FOGEND,	GetAddress( 100.f ) );

	// 描画
	CMatrix mRot, mTrans;
	mRot.SetRotY( timeGetTime() / 2 % 360 );
	
	lpDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	
	// テストメッシュ
	for( int i = 0; i < 5; i++ )
	{
		mTrans.SetTrans( -2 + i * 2, 0, -3 + i * 3 );
		lpDevice->SetTransform( D3DTS_WORLD, &(mRot * mTrans) );
		m_Mesh.Draw( lpDevice );
	}

	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );
	m_Sphere.Draw( lpDevice );

	// パス2
	// 縮小させる
	// レンダーターゲット変更
	// ぼかし用(ビューポートも変える)
	D3DVIEWPORT9 Vp = { 0, 0, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, 0.f, 1.f };
	lpDevice->SetViewport( &Vp );
	lpDevice->SetRenderTarget( 0, m_MiniSizeTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_MiniSizeTex.GetZbuffer() );

	lpDevice->Clear(
		0,NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0xff, 0xff, 0 ), 1.0f, 0);

	lpDevice->SetTransform( D3DTS_VIEW, &CMatrix() );
	lpDevice->SetTransform( D3DTS_PROJECTION, &CMatrix() );

	// テクスチャに描画したものを流す
	m_BlurShader.SetTexture( "g_Texture", m_SceneTex.GetTexture() );

	// 解像度を渡す
	D3DXVECTOR2 TexPos( 0.5f / Vp.Width, 0.5f / Vp.Height );
	m_BlurShader.SetValue( "g_Offset", &TexPos, sizeof(D3DXVECTOR2) );

	// シェーダー描画開始
	m_BlurShader.Begin();
	m_BlurShader.BeginPass( 0 );

	m_Scene.SetVertex( 2 );
	m_Scene.Draw( lpDevice );

	m_BlurShader.EndPass();
	m_BlurShader.End();

	// パス3
	// ぼかし入れる
	// レンダーターゲット変更
	lpDevice->SetRenderTarget( 0, m_BlurTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_BlurTex.GetZbuffer() );
	lpDevice->SetViewport( &Vp );

	lpDevice->Clear(
		0,NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0xff, 0xff, 0 ), 1.0f, 0);

	// 縮小画像をシェーダーに流す
	m_BlurShader.SetTexture( "g_Texture", m_MiniSizeTex.GetTexture() );
	
	// シェーダー描画開始
	m_BlurShader.Begin();
	m_BlurShader.BeginPass( 1 );

	lpDevice->SetFVF( CBoard::FVF_VERTEX );
	m_Scene.SetVertex( 2 );
	m_Scene.Draw( lpDevice );

	m_BlurShader.EndPass();
	m_BlurShader.End();

	// パス4
	// 深度計算を行う
	// レンダーターゲット変更
	// (ビューポートももとに戻す)
	Vp.Width = WINDOW_WIDTH;
	Vp.Height = WINDOW_HEIGHT;
	lpDevice->SetViewport( &Vp );
	lpDevice->SetRenderTarget( 0, m_DepthTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_DepthTex.GetZbuffer() );

	lpDevice->Clear(
		0,NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0, 0xff, 0xff ), 1.0f, 0);

	// シェーダーに値を渡す
	lpDevice->SetTransform( D3DTS_VIEW, m_Camera.GetMatrix() );
	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );
	
	m_DepthShader.SetValue( "g_Proj",  m_Proj.GetMatrix(), sizeof(D3DXMATRIX) );
	m_DepthShader.SetValue( "g_View",  m_Camera.GetMatrix(), sizeof(D3DXMATRIX) );

	// シェーダー描画開始
	m_DepthShader.Begin();
	m_DepthShader.BeginPass( 0 );

	CXfile::SetDrawShader( true );
	//CXfile::SetShaderDevice( &m_DepthShader, "g_mWVP" );

	for( int i = 0; i < 5; i++ )
	{
		mTrans.SetTrans( -2 + i * 2, 0, -3 + i * 3 );
		lpDevice->SetTransform( D3DTS_WORLD, &(mRot * mTrans) );
		m_Mesh.Draw( lpDevice );
	}

	//m_DepthShader.SetValue( "g_World", &CMatrix(), sizeof(D3DXMATRIX) );
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );
	m_Sphere.Draw( lpDevice );

	m_DepthShader.EndPass();
	m_DepthShader.End();

	// パス5
	// 被写界深度適用
	// バックバッファに描画
	// バックバッファにレンダーターゲットを戻す
	lpDevice->SetRenderTarget( 0, m_lpBackBuffer );
	lpDevice->SetDepthStencilSurface( m_lpZBuffer );
	Vp.Width = WINDOW_WIDTH;
	Vp.Height= WINDOW_HEIGHT;
	lpDevice->SetViewport( &Vp );

	lpDevice->Clear(
		0,NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0xff, 0, 0xff ), 1.0f, 0);

	// シェーダーに値を渡す
	m_DOFShader.SetTexture( "g_SceneTex", m_SceneTex.GetTexture() );	// シーン
	m_DOFShader.SetTexture( "g_BlurTex", m_BlurTex.GetTexture() );		// ぼかし
	m_DOFShader.SetTexture( "g_DepthTex", m_DepthTex.GetTexture() );	// 深度
	
	lpDevice->SetTransform( D3DTS_VIEW, &CMatrix() );
	lpDevice->SetTransform( D3DTS_PROJECTION, &CMatrix() );
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );

	// 板ポリゴンに描画
	m_DOFShader.Begin();
	m_DOFShader.BeginPass( 0 );

	m_Scene.SetVertex( 2 );
	m_Scene.Draw( lpDevice );

	m_DOFShader.EndPass();
	m_DOFShader.End();

	/*Vp.Y		= 300.f;
	Vp.Height	= 600.f;
	lpDevice->SetViewport( &Vp );*/

	DrawSceneByPolygon(lpDevice);

	SAFE_RELEASE( m_lpBackBuffer );
	SAFE_RELEASE( m_lpZBuffer );

	// シーン終了
	D3D::End();
}

// メッシュ描画
void CDOFText::DrawMesh(LPDIRECT3DDEVICE9 lpDevice)
{	
}

// シーン描画
void CDOFText::DrawSceneByPolygon(LPDIRECT3DDEVICE9 lpDevice)
{
	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpDevice->SetFVF( CBoard::FVF_VERTEX );
	lpDevice->SetTexture( 0, m_BlurTex.GetTexture() );
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );

	m_Scene.SetVertex( 2 );
	m_Scene.Draw( lpDevice );
}