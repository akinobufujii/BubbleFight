//////////////////////////////
// �C���N���[�h
//////////////////////////////
#include "StoneManager.h"

#include "FishManager.h"
#include "LimitWalls.h"
#include "UI.h"
#include "EnemyManager.h"
#include "PlanktonManager.h"
#include "GameMain.h"

//////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////
const UINT	CStoneManager::RENDERTARGET_SIZE	= 650;		// ���̂������艺����ƕ\������Ȃ��Ȃ�
const DWORD	CStoneManager::CLEAR_TARGET_TIME	= 60000;

//////////////////////////////
// ����
//////////////////////////////

// �R���X�g���N�^
CStoneManager::CStoneManager()
{
	// �V�F�[�_�ǂݍ���
	m_lpCausticShader = CSShaderManager::GetInst()->Load( 
		D3D::GetDevice(), "Caustic.fx" );
	m_lpWaveShader = CSShaderManager::GetInst()->Load( 
		D3D::GetDevice(), "Wave.fx" );

	// �����_�[�^�[�Q�b�g�쐬
	m_WaveTex.CreateRenderTarget( D3D::GetDevice(),
		RENDERTARGET_SIZE, RENDERTARGET_SIZE );

	m_BumpTex.CreateRenderTarget( D3D::GetDevice(),
		RENDERTARGET_SIZE, RENDERTARGET_SIZE );

	m_CausticTex.CreateRenderTarget( D3D::GetDevice(),
		RENDERTARGET_SIZE, RENDERTARGET_SIZE );

	m_SaveClearTime = timeGetTime();
}

// �f�X�g���N�^
CStoneManager::~CStoneManager()
{
	// ���
	Release();
}

// �t�@�C������ǂݍ���
bool CStoneManager::LoadFromData(const LPDIRECT3DDEVICE9 lp_device, LPCSTR path)
{
	// �w�肳�ꂽ�t�@�C�����J��
	std::ifstream In( path );

	if( In.is_open() )
	{
		STONEDATA*	lpTemp = NULL;	// �e���|����
		std::string	Str;

		// �Ō�܂œǂݍ���
		while( In.eof() == false )
		{
			lpTemp = NEW STONEDATA();

			// �ʒu���擾
			In >> lpTemp->Pos.x;
			In >> lpTemp->Pos.z;
			lpTemp->Pos.y = Sequence::CGameMain::BOTTOMSEA_HEIGHT;

			// �g�嗦�擾
			In >> lpTemp->Scale;

			// ���b�V���p�X�擾
			In >> Str;

			// �ǂݍ���
			CSXfileManager::GetInst()->Load( lp_device, Str.c_str(), true );

			// ���b�V���l��
			lpTemp->lpMesh = CSXfileManager::GetInst()->GetData( Str.c_str() );

			// ���X�g�ɓo�^
			m_StoneList.push_back( lpTemp );
		}

	}else{

		// �t�@�C���I�[�v�����s
		return false;
	}

	return true;
}

// �`��
void CStoneManager::Draw(const LPDIRECT3DDEVICE9 lp_device)
{
	lp_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	lp_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	lp_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lp_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	lp_device->SetRenderState( D3DRS_LIGHTING, FALSE );

	BOOST_FOREACH( STONEDATA* It, m_StoneList )
	{
		CMatrix mMat( It->Pos );

		mMat.ScaleLocal( It->Scale );

		lp_device->SetTransform( D3DTS_WORLD, &mMat );

		It->lpMesh->Draw( lp_device );
	}
}

// �V�F�[�_�[�`��
void CStoneManager::DrawShader( const LPDIRECT3DDEVICE9 lp_device )
{
	// �R�[�X�e�B�b�N�}�b�v�쐬
	CreateCausticMap( lp_device );

	// �s��ƃ��C�g�����l��
	D3DXMATRIX	mView, mProj, mInv;

	lp_device->GetTransform( D3DTS_PROJECTION, &mProj );
	lp_device->GetTransform( D3DTS_VIEW, &mView );

	// �V�F�[�_�[�ɗ���
	m_lpCausticShader->SetValue( "g_View", &mView, sizeof(D3DXMATRIX) );
	m_lpCausticShader->SetValue( "g_Proj", &mProj, sizeof(D3DXMATRIX) );
	m_lpCausticShader->SetTexture( "g_CausticTex", m_CausticTex.GetTexture() );

	m_lpCausticShader->Begin();
	//m_lpCausticShader->BeginPass( 1 );

	BOOST_FOREACH( STONEDATA* It, m_StoneList )
	{
		CMatrix mMat( It->Pos );

		mMat.ScaleLocal( It->Scale );

		m_lpCausticShader->SetValue( "g_LightDir", &GetLightPos( lp_device, mMat ), sizeof(D3DVECTOR) );
		m_lpCausticShader->SetValue( "g_World", &mMat, sizeof(D3DXMATRIX) );

		It->lpMesh->DrawShader( m_lpCausticShader, NULL, "g_Texture", 1, 1 );
	}

	//m_lpCausticShader->EndPass();
	m_lpCausticShader->End();
}

// ���
void CStoneManager::Release()
{
	BOOST_FOREACH( STONEDATA* It, m_StoneList )
	{
		SAFE_DELETE( It );
	}
	
	m_StoneList.clear();
}

// �R�[�X�e�B�b�N�}�b�v�쐬
void CStoneManager::CreateCausticMap(const LPDIRECT3DDEVICE9 lp_device)
{
	LPDIRECT3DSURFACE9 lpBackBuffer, lpZBuffer;	// �T�[�t�F�X�ۑ��p
	D3DVIEWPORT9 SaveVp, Vp = { 0, 0, RENDERTARGET_SIZE, RENDERTARGET_SIZE, 0, 1.f };

	// �o�b�N�o�b�t�@��ۑ�����
	lp_device->GetRenderTarget( 0, &lpBackBuffer );
	lp_device->GetDepthStencilSurface( &lpZBuffer );
	lp_device->GetViewport( &SaveVp );

	m_lpWaveShader->Begin();

	// �g�}�b�v�쐬
	// �����_�[�^�[�Q�b�g��ς���
	// ���e�N�X�`���ƃ����_�[�^�[�Q�b�g���ꏏ�Ȃ̂�
	//   �����I�ɂ܂���
	m_lpWaveShader->BeginPass( 0 );

	lp_device->SetViewport( &Vp );
	lp_device->SetRenderTarget( 0, m_WaveTex.GetSurface() );
	lp_device->SetDepthStencilSurface( m_WaveTex.GetZbuffer() );
	m_lpWaveShader->SetTexture( "g_Texture", m_WaveTex.GetTexture() );

	// ������莞�Ԃ������Ȃ�
	if( timeGetTime() - m_SaveClearTime > CLEAR_TARGET_TIME )
	{
		// �����_�[�^�[�Q�b�g������
		lp_device->Clear(
			0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB( 0, 0, 0 ), 1.f, 0  );

		m_SaveClearTime = timeGetTime();
	}
	
	m_lpWaveShader->SetValue( 
		"g_AddWavePos",
		&D3DXVECTOR2( rand() % 100 * 0.01f, rand() % 100 * 0.01f ),
		sizeof( D3DXVECTOR2 ) );

	m_Borad.SetVertex( 2 );
	m_Borad.Draw( lp_device );

	m_lpWaveShader->EndPass();

	// �@���}�b�v�쐬
	// �����_�[�^�[�Q�b�g��ς���
	lp_device->SetRenderTarget( 0, m_BumpTex.GetSurface() );
	lp_device->SetDepthStencilSurface( m_BumpTex.GetZbuffer() );
	m_lpWaveShader->SetTexture( "g_Texture", m_WaveTex.GetTexture() );

	m_lpWaveShader->BeginPass( 1 );

	m_Borad.SetVertex( 2 );
	m_Borad.Draw( lp_device );

	m_lpWaveShader->EndPass();
	m_lpWaveShader->End();

	// �R�[�X�e�B�b�N�}�b�v�쐬
	m_lpCausticShader->Begin();
	m_lpCausticShader->BeginPass( 0 );

	// �����_�[�^�[�Q�b�g��ς���
	lp_device->SetRenderTarget( 0, m_CausticTex.GetSurface() );
	lp_device->SetDepthStencilSurface( NULL );
	m_lpCausticShader->SetTexture( "g_NormalTex", m_BumpTex.GetTexture() );

	m_Borad.SetVertex( 2 );
	m_Borad.Draw( lp_device );

	m_lpCausticShader->EndPass();
	m_lpCausticShader->End();

	// �����_�[�^�[�Q�b�g��߂�
	lp_device->SetRenderTarget( 0, lpBackBuffer );
	lp_device->SetDepthStencilSurface( lpZBuffer );
	lp_device->SetViewport( &SaveVp );

	SAFE_RELEASE( lpBackBuffer );
	SAFE_RELEASE( lpZBuffer );
}