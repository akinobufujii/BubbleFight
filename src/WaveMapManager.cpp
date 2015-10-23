//////////////////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////////////////
#include "WaveMapManager.h"

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �R���X�g���N�^
CWaveMapManager::CWaveMapManager():
m_lpWaveShader( NULL ),
m_Board( 2 ),
m_IsFirst( true )
{
}

// �f�X�g���N�^
CWaveMapManager::~CWaveMapManager()
{
	m_lpWaveShader = NULL;
}

// ���
bool CWaveMapManager::Create(LPDIRECT3DDEVICE9 lpdevice, UINT size)
{
	// �V�F�[�_�f�o�C�X�l��
	m_lpWaveShader = CSShaderManager::GetInst()->Load( lpdevice, "WaveMap.fx" );

	// �g�}�b�v
	for( int i = 0; i < 2; i++ )
	{
		m_WaveMap[ i ].CreateRenderTarget( lpdevice, size, size, D3DFMT_G16R16F );
	}

	// �@���}�b�v
	m_NormalMap.CreateRenderTarget( lpdevice, size, size );

	// �G���[�`�F�b�N
	if( !m_WaveMap[ 0 ].GetTexture()	|| !m_WaveMap[ 1 ].GetTexture() || 
		!m_NormalMap.GetTexture()		|| !m_lpWaveShader )
	{
		return false;
	}

	// �𑜓x���l�������l���V�F�[�_�ɓn��
	m_lpWaveShader->SetValue( "g_TexMove", 
		&D3DXVECTOR2( 1.f / size, 1.f / size ),
		sizeof( D3DXVECTOR2 ) );

	return true;
}

// �X�V
const LPDIRECT3DTEXTURE9 CWaveMapManager::Update(LPDIRECT3DDEVICE9 lpdevice)
{
	LPDIRECT3DSURFACE9	lpDepth, lpBack;
	D3DVIEWPORT9		SaveVp, MapVp;

	// ���̃����_�[�^�[�Q�b�g��ۑ�����
	lpdevice->GetViewport( &SaveVp );
	lpdevice->GetRenderTarget( 0, &lpBack );
	lpdevice->GetDepthStencilSurface( &lpDepth );

	// �}�b�v�p�Ƀr���[�|�[�g��ݒ肷��
	MapVp.X		= 0;
	MapVp.Y		= 0;
	MapVp.Width = m_NormalMap.GetSurfaceDesc().Width;
	MapVp.Height= m_NormalMap.GetSurfaceDesc().Height;
	MapVp.MinZ	= 0.f;
	MapVp.MaxZ	= 1.f;

	lpdevice->SetViewport( &MapVp );

	// �p�X1
	// �g�}�b�v�̍X�V
	UpdateWaveMap( lpdevice );

	// �p�X2
	// �@���}�b�v�����
	UpdateNormalMap( lpdevice );

	// �����_�[�^�[�Q�b�g��߂�
	lpdevice->SetViewport( &SaveVp );
	lpdevice->SetRenderTarget( 0, lpBack );
	lpdevice->SetDepthStencilSurface( lpDepth );

	// �Q�ƃJ�E���^�����炷
	lpBack->Release();
	lpDepth->Release();

	return m_NormalMap;
}

// �g�}�b�v�X�V
void CWaveMapManager::UpdateWaveMap(LPDIRECT3DDEVICE9 lpdevice)
{
	// �l������(�K����)
	// �o��������e�N�Z�����W
	m_lpWaveShader->SetValue( "g_AddWavePos",
		&D3DXVECTOR2( 
		static_cast<float>( rand() % 201 ) / 100,
		static_cast<float>( rand() % 201 ) / 100 ),
		sizeof( D3DXVECTOR2 ) );

	// ��������g�̍�����ݒ肷��
	float Temp = static_cast<float>( rand() % 201 + 100 ) / 100;
	m_lpWaveShader->SetValue( "g_AddWaveHeight", &Temp, sizeof( float ) );

	// �[�x�o�b�t�@�g��Ȃ�
	lpdevice->SetDepthStencilSurface( NULL );

	// �����_�[�^�[�Q�b�g�̎d�l��2��`����s��
	for( int i = 0; i < 2; i++ )
	{
		// �����_�[�^�[�Q�b�g�ύX
		lpdevice->SetRenderTarget( 0, m_WaveMap[ i ].GetSurface() );

		// �����ق�Ƃɍŏ��Ȃ�
		if( m_IsFirst )
		{
			// �K���ȊG��`�悷��
			m_lpWaveShader->SetTexture( "g_Texture", *CSTexManager::GetInst()->GetData( "SurfaceSea.jpg" ) );
			m_IsFirst = false;
		
		}else{

			// �e�N�X�`���𗬂�
			m_lpWaveShader->SetTexture( "g_Texture", m_WaveMap[ (i + 1) % 2 ] );
		}

		// �`��
		m_lpWaveShader->Begin();
		m_lpWaveShader->BeginPass( 0 );

		m_Board.Draw( lpdevice );

		m_lpWaveShader->EndPass();
		m_lpWaveShader->End();
	}
}

// �@���}�b�v�쐬
void CWaveMapManager::UpdateNormalMap(LPDIRECT3DDEVICE9 lpdevice)
{
	// �����_�[�^�[�Q�b�g�ݒ�
	lpdevice->SetRenderTarget( 0, m_NormalMap.GetSurface() );

	// �[�x�o�b�t�@�g��Ȃ�
	lpdevice->SetDepthStencilSurface( NULL );

	// �g�}�b�v���V�F�[�_�ɓn��
	m_lpWaveShader->SetTexture( "g_Texture", m_WaveMap[ 1 ] );

	// �`��
	m_lpWaveShader->Begin();
	m_lpWaveShader->BeginPass( 1 );

	m_Board.Draw( lpdevice );

	m_lpWaveShader->EndPass();
	m_lpWaveShader->End();
}