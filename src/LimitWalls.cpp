//////////////////////////////
// ���E��
//////////////////////////////
#include "LimitWalls.h"

//////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////
const float		CLimitWalls::FADEIN_OUT_SPEED	= 0.01f;
const LPCSTR	CLimitWalls::TEXPATH_WALL		= "Wall.png";

//////////////////////////////
// ����
//////////////////////////////

// �R���X�g���N�^
CLimitWalls::CLimitWalls():
m_NowAlpha( 0 ),
m_IsAlphaPlus( true )
{
	// �p�x�����͌Œ�Ȃ̂œ���Ă���
	// �O�̂��ߐ^�����ɐݒ�
	// ������
	for( int i = 0; i < WALL_NUM; i++ )
	{
		m_Walls[ i ].Board.SetAlpha( 1.0f );
		m_Walls[ i ].Board.SetColor( 1.f, 1.f, 1.f );
		m_Walls[ i ].mMat.RotateLocalY( i * 90 );
	}
}

// �f�X�g���N�^
CLimitWalls::~CLimitWalls()
{
}

// ���E�͈͂�ݒ肷��(���_����)
void CLimitWalls::SetLimit( const float limit )
{
	// �w�蕪�������_���痣��(���[�J������)
	BOOST_FOREACH( WALLDATA& Wall, m_Walls )
	{
		Wall.mMat.MoveLocal( 0, 0, -limit );
	}
}

// �傫�����w��
void CLimitWalls::SetScale( const float scale )
{
	// �傫����ݒ肷��
	BOOST_FOREACH( WALLDATA& Wall, m_Walls )
	{
		Wall.Board.SetVertex( scale );
	}
}

void CLimitWalls::SetScale( const float width, const float height )
{
	// �傫����ݒ肷��
	BOOST_FOREACH( WALLDATA& Wall, m_Walls )
	{
		Wall.Board.SetVertex( width, height );
	}
}

// �X�V
void CLimitWalls::Update()
{
	// �e�N�X�`���̍X�V
	if( m_IsAlphaPlus )
	{
		if( (m_NowAlpha += FADEIN_OUT_SPEED) > 1 )
		{
			m_NowAlpha = 1.f;
			m_IsAlphaPlus = false;
		}	
	
	}else{

		if( (m_NowAlpha -= FADEIN_OUT_SPEED) < 0 )
		{
			m_NowAlpha = 0.f;
			m_IsAlphaPlus = true;
		}
	}

	BOOST_FOREACH( WALLDATA& Wall, m_Walls )
	{
		Wall.Board.SetAlpha( m_NowAlpha );
	}
}

// �`��
void CLimitWalls::Draw( const LPDIRECT3DDEVICE9 lp_device )
{
	// ���_�t�H�[�}�b�g�ݒ�
	lp_device->SetFVF( CBoard::FVF_VERTEX );

	// �e�N�X�`���ݒ�
	lp_device->SetTexture( 0, CSTexManager::GetInst()->GetData( TEXPATH_WALL )->GetTexture() );

	// ���ꂼ��̍s��ň�C�ɕ`��
	BOOST_FOREACH( WALLDATA& Wall, m_Walls )
	{
		lp_device->SetTransform( D3DTS_WORLD, &Wall.mMat );

		Wall.Board.Draw( lp_device );
	}
}