////////////////////////////////
// �C���N���[�h
////////////////////////////////
#include "WaveMapManager.h"

#include "SurfaceSea.h"

////////////////////////////////
// �ÓI�����o���̉�
////////////////////////////////
const CSurfaceSea::FLGTYPE	CSurfaceSea::FLG_HEIGHTPLUS	= 0x01;

////////////////////////////////
// ����
////////////////////////////////

// �R���X�g���N�^
CSurfaceSea::CSurfaceSea():
m_SeaSize( 10.f ),
m_SeaBoard( m_SeaSize, false ),
m_Height( 0 ),
m_Speed( 2000 ),
m_ScrollCnt( 0 ),
m_Flg( FLG_HEIGHTPLUS ),
m_Wave( 0.f ),
m_lpWaveMap( NEW CWaveMapManager() )
{
	m_SeaBoard.SetColor( 1.f, 1.f, 1.f );

	// �V�F�[�_�[�ǂݍ���
	DebugMessageBox( "�o���v�}�b�v�V�F�[�_�ǂݍ���" );
	m_lpBumpShader = CSShaderManager::GetInst()->Load( D3D::GetDevice(), "BumpMap.fx" );
	
	// �����_�[�^�[�Q�b�g�쐬
	m_lpWaveMap->Create( D3D::GetDevice(), 512 );
}

// �f�X�g���N�^
CSurfaceSea::~CSurfaceSea()
{
	SAFE_DELETE( m_lpWaveMap );
}

// �X�V
void CSurfaceSea::Update(const D3DXVECTOR3 &player_pos)
{
	m_Pos = player_pos;

	// �����͎w�肳�ꂽ�Ƃ���ɂ���
	m_Pos.y = m_Height;

	m_SeaBoard.SetAlpha( 1.f );

	// �g�𗧂Ă�
	if( m_Flg & FLG_HEIGHTPLUS )
	{
		if( (m_Wave -= 0.01f) < -1.f )
		{
			m_Flg &= ~FLG_HEIGHTPLUS;
		}

	}else{

		if( (m_Wave += 0.01f) > 0 )
		{
			m_Flg |= FLG_HEIGHTPLUS;
		}
	}

	//m_Wave = 0;

	// �e�N�X�`���X�N���[��
	// ���@�ɍ��킹�ăX�N���[�����邽�߁A���S���v�Z����
	const float	TEXPOS	= 1.f / m_Speed;
	const float UV		= 0.5f / 100.f;

	float PlayerX = player_pos.x * UV + (TEXPOS * (++m_ScrollCnt % m_Speed) );
	float PlayerZ = -player_pos.z * UV;

	m_SeaBoard.SetTexturePos(
		&D3DXVECTOR2( PlayerX		, PlayerZ ),
		&D3DXVECTOR2( 1.f + PlayerX , PlayerZ ),
		&D3DXVECTOR2( 1.f + PlayerX , 1.f + PlayerZ),
		&D3DXVECTOR2( PlayerX		, 1.f + PlayerZ) );
}

// �`��
void CSurfaceSea::Draw(const LPDIRECT3DDEVICE9 lp_device)
{
	lp_device->SetTransform( D3DTS_WORLD, &CMatrix( m_Pos ) );

	lp_device->SetTexture( 0, NULL );
	m_SeaBoard.Draw( lp_device );
}

// �V�F�[�_�[�ŕ`��
void CSurfaceSea::DrawShader( const LPDIRECT3DDEVICE9 lp_device )
{
	// �g�}�b�v�X�V
	//UpdateWaveMap( lp_device );

	// ���C�g�̕���������Ă���
	D3DLIGHT9 Light;
	lp_device->GetLight( 0, &Light );
	
	// �V�F�[�_�[�ɕ�����n��
	m_lpBumpShader->SetValue( "g_LightDir", &Light.Direction, sizeof(D3DXVECTOR3) );

	// �e�N�X�`����ݒ�
	m_lpBumpShader->SetTexture( "g_Texture",
		CSTexManager::GetInst()->GetData( "SurfaceSea.jpg" )->GetTexture() );
	m_lpBumpShader->SetTexture( "g_NormalTex", m_lpWaveMap->Update( lp_device ) );

	// ���W�ϊ��s��ݒ�
	CMatrix mView, mProj, mInv( m_Pos );

	mInv.GetInverse();

	// �Œ�p�C�v���C������s��擾
	lp_device->GetTransform( D3DTS_VIEW, &mView );
	lp_device->GetTransform( D3DTS_PROJECTION, &mProj );

	m_lpBumpShader->SetValue( "g_View", &mView, sizeof(D3DXMATRIX) );
	m_lpBumpShader->SetValue( "g_Proj", &mProj, sizeof(D3DXMATRIX) );
	m_lpBumpShader->SetValue( "g_World", &CMatrix( m_Pos ), sizeof(D3DXMATRIX) );
	m_lpBumpShader->SetValue( "g_WorldInv", &mInv, sizeof(D3DXMATRIX) );
	m_lpBumpShader->SetValue( "g_Height", &m_Wave, sizeof(float) );

	// �o���v�}�b�s���O�J�n
	m_lpBumpShader->Begin();
	m_lpBumpShader->BeginPass( 0 );

	m_SeaBoard.SetVertex( m_SeaSize, false );
	m_SeaBoard.Draw( lp_device );

	m_lpBumpShader->EndPass();
	m_lpBumpShader->End();
}