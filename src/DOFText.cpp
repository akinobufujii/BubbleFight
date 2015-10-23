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

	// ���C�g�ݒ�
	D3DLIGHT9	Light;
	ZeroMemory( &Light, sizeof( Light ) );

	Light.Type	= D3DLIGHT_DIRECTIONAL;	// ���s����
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

	// �����_�����O�^�[�Q�b�g�쐬
	// �V�[���`��p
	m_SceneTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT );

	// �k���p
	m_MiniSizeTex.CreateRenderTarget( 
		D3D::GetDevice(), WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4 );

	// �ڂ����p
	m_BlurTex.CreateRenderTarget(
		D3D::GetDevice(), WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4 );

	// �[�x�v�Z�p
	m_DepthTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT );

	// �V�F�[�_�[�ǂݍ���
	// �ڂ����p
	m_BlurShader.Load( D3D::GetDevice(), "FX/Blur.fx", "Tec" );

	// �[�x�v�Z�p
	m_DepthShader.Load( D3D::GetDevice(), "FX/Depth.fx", "Tec" );

	// ��ʊE�[�x
	m_DOFShader.Load( D3D::GetDevice(), "FX/DOF.fx", "Tec" );

	CXfile::SetShaderDevice( &m_DepthShader, "g_World" );
}

CDOFText::~CDOFText(void)
{
}

// ����
bool CDOFText::RunProc()
{
	Draw();
	return true;
}

// �`��
void CDOFText::Draw()
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// �V�[���`��J�n
	D3D::Begin();

	// �J�����A���e�ݒ�
	m_Camera.SetCameraPos( &D3DXVECTOR3( 0, 0, -10 ) );
	lpDevice->SetTransform( D3DTS_VIEW, m_Camera.GetMatrix() );
	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );

	// �o�b�N�o�b�t�@�ۑ�
	lpDevice->GetRenderTarget( 0, &m_lpBackBuffer );
	lpDevice->GetDepthStencilSurface( &m_lpZBuffer );

	CXfile::SetDrawShader( false );

	// �p�X1
	// �e�N�X�`���ɕ`��
	// �����_�[�^�[�Q�b�g�ύX
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

	// �`��
	CMatrix mRot, mTrans;
	mRot.SetRotY( timeGetTime() / 2 % 360 );
	
	lpDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	
	// �e�X�g���b�V��
	for( int i = 0; i < 5; i++ )
	{
		mTrans.SetTrans( -2 + i * 2, 0, -3 + i * 3 );
		lpDevice->SetTransform( D3DTS_WORLD, &(mRot * mTrans) );
		m_Mesh.Draw( lpDevice );
	}

	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );
	m_Sphere.Draw( lpDevice );

	// �p�X2
	// �k��������
	// �����_�[�^�[�Q�b�g�ύX
	// �ڂ����p(�r���[�|�[�g���ς���)
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

	// �e�N�X�`���ɕ`�悵�����̂𗬂�
	m_BlurShader.SetTexture( "g_Texture", m_SceneTex.GetTexture() );

	// �𑜓x��n��
	D3DXVECTOR2 TexPos( 0.5f / Vp.Width, 0.5f / Vp.Height );
	m_BlurShader.SetValue( "g_Offset", &TexPos, sizeof(D3DXVECTOR2) );

	// �V�F�[�_�[�`��J�n
	m_BlurShader.Begin();
	m_BlurShader.BeginPass( 0 );

	m_Scene.SetVertex( 2 );
	m_Scene.Draw( lpDevice );

	m_BlurShader.EndPass();
	m_BlurShader.End();

	// �p�X3
	// �ڂ��������
	// �����_�[�^�[�Q�b�g�ύX
	lpDevice->SetRenderTarget( 0, m_BlurTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_BlurTex.GetZbuffer() );
	lpDevice->SetViewport( &Vp );

	lpDevice->Clear(
		0,NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0xff, 0xff, 0 ), 1.0f, 0);

	// �k���摜���V�F�[�_�[�ɗ���
	m_BlurShader.SetTexture( "g_Texture", m_MiniSizeTex.GetTexture() );
	
	// �V�F�[�_�[�`��J�n
	m_BlurShader.Begin();
	m_BlurShader.BeginPass( 1 );

	lpDevice->SetFVF( CBoard::FVF_VERTEX );
	m_Scene.SetVertex( 2 );
	m_Scene.Draw( lpDevice );

	m_BlurShader.EndPass();
	m_BlurShader.End();

	// �p�X4
	// �[�x�v�Z���s��
	// �����_�[�^�[�Q�b�g�ύX
	// (�r���[�|�[�g�����Ƃɖ߂�)
	Vp.Width = WINDOW_WIDTH;
	Vp.Height = WINDOW_HEIGHT;
	lpDevice->SetViewport( &Vp );
	lpDevice->SetRenderTarget( 0, m_DepthTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_DepthTex.GetZbuffer() );

	lpDevice->Clear(
		0,NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0, 0xff, 0xff ), 1.0f, 0);

	// �V�F�[�_�[�ɒl��n��
	lpDevice->SetTransform( D3DTS_VIEW, m_Camera.GetMatrix() );
	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );
	
	m_DepthShader.SetValue( "g_Proj",  m_Proj.GetMatrix(), sizeof(D3DXMATRIX) );
	m_DepthShader.SetValue( "g_View",  m_Camera.GetMatrix(), sizeof(D3DXMATRIX) );

	// �V�F�[�_�[�`��J�n
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

	// �p�X5
	// ��ʊE�[�x�K�p
	// �o�b�N�o�b�t�@�ɕ`��
	// �o�b�N�o�b�t�@�Ƀ����_�[�^�[�Q�b�g��߂�
	lpDevice->SetRenderTarget( 0, m_lpBackBuffer );
	lpDevice->SetDepthStencilSurface( m_lpZBuffer );
	Vp.Width = WINDOW_WIDTH;
	Vp.Height= WINDOW_HEIGHT;
	lpDevice->SetViewport( &Vp );

	lpDevice->Clear(
		0,NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0xff, 0, 0xff ), 1.0f, 0);

	// �V�F�[�_�[�ɒl��n��
	m_DOFShader.SetTexture( "g_SceneTex", m_SceneTex.GetTexture() );	// �V�[��
	m_DOFShader.SetTexture( "g_BlurTex", m_BlurTex.GetTexture() );		// �ڂ���
	m_DOFShader.SetTexture( "g_DepthTex", m_DepthTex.GetTexture() );	// �[�x
	
	lpDevice->SetTransform( D3DTS_VIEW, &CMatrix() );
	lpDevice->SetTransform( D3DTS_PROJECTION, &CMatrix() );
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );

	// �|���S���ɕ`��
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

	// �V�[���I��
	D3D::End();
}

// ���b�V���`��
void CDOFText::DrawMesh(LPDIRECT3DDEVICE9 lpDevice)
{	
}

// �V�[���`��
void CDOFText::DrawSceneByPolygon(LPDIRECT3DDEVICE9 lpDevice)
{
	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpDevice->SetFVF( CBoard::FVF_VERTEX );
	lpDevice->SetTexture( 0, m_BlurTex.GetTexture() );
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );

	m_Scene.SetVertex( 2 );
	m_Scene.Draw( lpDevice );
}