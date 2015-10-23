//////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////
#include "Using.h"
#include "Board.h"

//////////////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////////////
const int CBoard::FVF_VERTEX = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

bool			CBoard::m_IsShaderDraw	= false;
CEffect*		CBoard::m_lpEffect		= NULL;
D3DXHANDLE		CBoard::m_lphWorld		= NULL;

//////////////////////////////////////
// ����
//////////////////////////////////////

// �R���X�g���N�^
CBoard::CBoard():
m_lpTex( NULL ),
m_Color( 1.f, 1.f, 1.f, 1.f )
{
	m_Vertex[ 0 ].Pos	= D3DXVECTOR3( -10.f,  10.f, 0.f );
	m_Vertex[ 1 ].Pos	= D3DXVECTOR3(  10.f,  10.f, 0.f );
	m_Vertex[ 2 ].Pos	= D3DXVECTOR3(  10.f, -10.f, 0.f );
	m_Vertex[ 3 ].Pos	= D3DXVECTOR3( -10.f, -10.f, 0.f );

	SetColor( m_Color );
	
	m_Vertex[ 0 ].Tex	= D3DXVECTOR2( 0.0f, 0.0f );
	m_Vertex[ 1 ].Tex	= D3DXVECTOR2( 1.0f, 0.0f );
	m_Vertex[ 2 ].Tex	= D3DXVECTOR2( 1.0f, 1.0f );
	m_Vertex[ 3 ].Tex	= D3DXVECTOR2( 0.0f, 1.0f );
}

CBoard::CBoard( float size, bool is_wall):
m_lpTex( NULL ),
m_Color( 1.f, 1.f, 1.f, 1.f )
{
	m_Vertex[ 0 ].Pos	= D3DXVECTOR3( -10.f,  10.f, 0.f );
	m_Vertex[ 1 ].Pos	= D3DXVECTOR3(  10.f,  10.f, 0.f );
	m_Vertex[ 2 ].Pos	= D3DXVECTOR3(  10.f, -10.f, 0.f );
	m_Vertex[ 3 ].Pos	= D3DXVECTOR3( -10.f, -10.f, 0.f );

	SetColor( m_Color );
	
	m_Vertex[ 0 ].Tex	= D3DXVECTOR2( 0.0f, 0.0f );
	m_Vertex[ 1 ].Tex	= D3DXVECTOR2( 1.0f, 0.0f );
	m_Vertex[ 2 ].Tex	= D3DXVECTOR2( 1.0f, 1.0f );
	m_Vertex[ 3 ].Tex	= D3DXVECTOR2( 0.0f, 1.0f );

	SetVertex( size, is_wall );
}

CBoard::CBoard(float w, float h):
m_lpTex( NULL ),
m_Color( 1.f, 1.f, 1.f, 1.f )
{
	m_Vertex[ 0 ].Pos	= D3DXVECTOR3( -10.f,  10.f, 0.f );
	m_Vertex[ 1 ].Pos	= D3DXVECTOR3(  10.f,  10.f, 0.f );
	m_Vertex[ 2 ].Pos	= D3DXVECTOR3(  10.f, -10.f, 0.f );
	m_Vertex[ 3 ].Pos	= D3DXVECTOR3( -10.f, -10.f, 0.f );

	SetColor( m_Color );
	
	m_Vertex[ 0 ].Tex	= D3DXVECTOR2( 0.0f, 0.0f );
	m_Vertex[ 1 ].Tex	= D3DXVECTOR2( 1.0f, 0.0f );
	m_Vertex[ 2 ].Tex	= D3DXVECTOR2( 1.0f, 1.0f );
	m_Vertex[ 3 ].Tex	= D3DXVECTOR2( 0.0f, 1.0f );

	SetVertex( w, h );
}

CBoard::CBoard(float left, float top, float right, float bottom):
m_lpTex( NULL ),
m_Color( 1.f, 1.f, 1.f, 1.f )
{
	CBoard();

	SetVertex( left, top, right, bottom );
}

CBoard::CBoard(const D3DXVECTOR3 &v1, 
			   const D3DXVECTOR3 &v2,
			   const D3DXVECTOR3 &v3,
			   const D3DXVECTOR3 &v4)
{
	CBoard();

	SetVertex( v1, v2, v3, v4 );
}

// �f�X�g���N�^
CBoard::~CBoard()
{
	// ���ɖ���
}

// ���_�ݒ�
// �����`
void CBoard::SetVertex(float size, bool is_wall)
{
	float Half = size * 0.5f;

	// �c�^
	if( is_wall )
	{
		m_Vertex[ 0 ].Pos	= D3DXVECTOR3( -Half,  Half, 0.f );
		m_Vertex[ 1 ].Pos	= D3DXVECTOR3(  Half,  Half, 0.f );
		m_Vertex[ 2 ].Pos	= D3DXVECTOR3(  Half, -Half, 0.f );
		m_Vertex[ 3 ].Pos	= D3DXVECTOR3( -Half, -Half, 0.f );
	
	// �n�ʌ^
	}else{

		m_Vertex[ 0 ].Pos	= D3DXVECTOR3( -Half, 0.f,  Half );
		m_Vertex[ 1 ].Pos	= D3DXVECTOR3(  Half, 0.f,  Half );
		m_Vertex[ 2 ].Pos	= D3DXVECTOR3(  Half, 0.f, -Half );
		m_Vertex[ 3 ].Pos	= D3DXVECTOR3( -Half, 0.f, -Half );
	}
}

// �����`
void CBoard::SetVertex(float w, float h)
{
	float Width = w * 0.5f, Height = h * 0.5f;

	m_Vertex[ 0 ].Pos	= D3DXVECTOR3( -Width,  Height, 0.f );
	m_Vertex[ 1 ].Pos	= D3DXVECTOR3(  Width,  Height, 0.f );
	m_Vertex[ 2 ].Pos	= D3DXVECTOR3(  Width, -Height, 0.f );
	m_Vertex[ 3 ].Pos	= D3DXVECTOR3( -Width, -Height, 0.f );
}

// ��`
void CBoard::SetVertex(float left, float top, float right, float bottom)
{
	m_Vertex[ 0 ].Pos	= D3DXVECTOR3( -left,   top,   0.f );
	m_Vertex[ 1 ].Pos	= D3DXVECTOR3(  left,   top,   0.f );
	m_Vertex[ 2 ].Pos	= D3DXVECTOR3(  right, -bottom, 0.f );
	m_Vertex[ 3 ].Pos	= D3DXVECTOR3( -right, -bottom, 0.f );
}

//�@���S�w��
void CBoard::SetVertex(const D3DXVECTOR3 &v1, 
					   const D3DXVECTOR3 &v2,
					   const D3DXVECTOR3 &v3,
					   const D3DXVECTOR3 &v4)
{
	m_Vertex[ 0 ].Pos = v1;
	m_Vertex[ 1 ].Pos = v2;
	m_Vertex[ 2 ].Pos = v3;
	m_Vertex[ 3 ].Pos = v4;
}

// �F�ݒ�֘A
void CBoard::SetColor(const D3DXCOLOR &color)
{
	m_Color = color;

	m_Vertex[ 0 ].Color	= 
	m_Vertex[ 1 ].Color	= 
	m_Vertex[ 2 ].Color	= 
	m_Vertex[ 3 ].Color	= m_Color;
}

void CBoard::SetColor(float r, float g, float b)
{
	m_Color.r = r;
	m_Color.g = g;
	m_Color.b = b;

	SetColor( m_Color );
}

// ���_�P�ʂŐF�w��
void CBoard::SetVertexColors(const D3DCOLOR v1,
							 const D3DCOLOR v2,
							 const D3DCOLOR v3,
							 const D3DCOLOR v4 )
{
	m_Vertex[ 0 ].Color	= v1;
	m_Vertex[ 1 ].Color	= v2;
	m_Vertex[ 2 ].Color	= v3;
	m_Vertex[ 3 ].Color	= v4;
}

// �P�F�ݒ�
void CBoard::SetRed(float r)
{
	m_Color.r = r;

	SetColor( m_Color );
}

void CBoard::SetGreen(float g)
{
	m_Color.g = g;

	SetColor( m_Color );
}

void CBoard::SetBlue(float b)
{
	m_Color.b = b;

	SetColor( m_Color );
}

void CBoard::SetAlpha(float a)
{
	m_Color.a = a;

	SetColor( m_Color );
}

// �e�N�X�`�����W�ݒ�
void CBoard::SetTexturePos(const D3DXVECTOR2* pos1, const D3DXVECTOR2* pos2, const D3DXVECTOR2* pos3, const D3DXVECTOR2* pos4)
{
	m_Vertex[ 0 ].Tex = *pos1;
	m_Vertex[ 1 ].Tex = *pos2;
	m_Vertex[ 2 ].Tex = *pos3;
	m_Vertex[ 3 ].Tex = *pos4;
}

// �`��
bool CBoard::Draw(const LPDIRECT3DDEVICE9 lpDevice) const 
{
	lpDevice->SetFVF( FVF_VERTEX );

	if( m_IsShaderDraw && m_lpEffect && m_lphWorld )
	{
		D3DXMATRIX mWorld;

		// �f�o�C�X����s��l��
		D3D::GetDevice()->GetTransform( D3DTS_WORLD, &mWorld );

		m_lpEffect->SetValue( m_lphWorld, &mWorld, sizeof(D3DXMATRIX) );
	}

	lpDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, &m_Vertex, sizeof( Vertex ) );
	
	return true;
}