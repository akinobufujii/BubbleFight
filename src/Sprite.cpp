/////////////////////////////////////
// �C���N���[�h
/////////////////////////////////////
#include "Sprite.h"

/////////////////////////////////////
// ����
/////////////////////////////////////

// �R���X�g���N�^
CSprite::CSprite():
m_lpSprite( NULL ),
m_Alpha( 0xff )
{
}

CSprite::CSprite( const LPDIRECT3DDEVICE9 lpdevice ):
m_lpSprite( NULL ),
m_Alpha( 0xff )
{
	Create( lpdevice );
}

// �f�o�C�X�쐬
bool CSprite::Create( const LPDIRECT3DDEVICE9 lpdevice )
{
	// ���O�̂���
	SAFE_RELEASE( m_lpSprite );

	// �쐬
	HRESULT hr = D3DXCreateSprite( lpdevice, &m_lpSprite );

	if( SUCCEEDED( hr ) )
	{
		m_lpSprite->OnResetDevice();
		return true;
	
	}else{

		return false;
	}
}

// �`��
void CSprite::Draw( const LPDIRECT3DTEXTURE9 lptex, float dest_x, float dest_y, float rot, int src_left, int src_top, int src_right, int src_bottom )
{
	// ��`�ݒ�
	RECT Src = { src_left + 1, src_top + 1, src_right - 1, src_bottom - 1 };

	// ���S����
	float HarfX = (src_right - src_left) * 0.5f;
	float HarfY	= (src_bottom - src_top) * 0.5f;
	
	D3DXVECTOR3 Vec( HarfX, HarfY, 0.f );

	D3DXMATRIX mWorld, mTrans, mRotZ;

	// ������
	// �ړ�
	D3DXMatrixTranslation( &mTrans, dest_x + HarfX, dest_y + HarfY, 0.f );

	// ��](�O�̂��ߗ]��Ōv�Z)
	D3DXMatrixRotationZ( &mRotZ, D3DXToRadian( rot ) );

	//D3DXMatrixScaling( &mWorld , 0.5f, 0.5f, 0.5f );

	// ����
	mWorld = mRotZ * mTrans;

	// �ݒ肷��
	m_lpSprite->SetTransform( &mWorld );

	// �`��
	m_lpSprite->Draw( lptex, &Src, &Vec, NULL, D3DCOLOR_ARGB( m_Alpha, 0xff, 0xff, 0xff ) );
}

// ���łɂ���f�o�C�X�ŕ`��
void CSprite::Draw(
		const LPD3DXSPRITE lpsprite,
		const LPDIRECT3DTEXTURE9 lptex,
		float dest_x,
		float dest_y,
		float rot,
		int src_left,
		int src_top,
		int src_right,
		int src_bottom,
		int alpha )
{
	// ��`�ݒ�
	RECT Src = { src_left, src_top, src_right, src_bottom };

	// ���S����
	float HarfX = (src_right - src_left) * 0.5f;
	float HarfY	= (src_bottom - src_top) * 0.5f;
	
	D3DXVECTOR3 Vec( HarfX, HarfY, 0.f );

	D3DXMATRIX mWorld, mTrans, mRotZ;

	// ������
	// �ړ�
	D3DXMatrixTranslation( &mTrans, dest_x + HarfX, dest_y + HarfY, 0.f );

	// ��](�O�̂��ߗ]��Ōv�Z)
	D3DXMatrixRotationZ( &mRotZ, D3DXToRadian( rot ) );

	// ����
	mWorld = mRotZ * mTrans;

	// �ݒ肷��
	lpsprite->SetTransform( &mWorld );

	// �`��
	lpsprite->Draw( lptex, &Src, &Vec, NULL, D3DCOLOR_ARGB( alpha, 0xff, 0xff, 0xff ) );
}

// �f�X�g���N�^
CSprite::~CSprite()
{
	SAFE_RELEASE( m_lpSprite );
}
