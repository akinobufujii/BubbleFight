#ifndef __AKI_LINE_H__
#define __AKI_LINE_H__

///////////////////////////////////////////
// ����`�悷�邽�߂����̃N���X
// (���܂���e���Ȃ��̂őS�Ă����Ŏ���)
///////////////////////////////////////////
class CLine
{
public:	// �O�����J
	
	///////////////////////////////////////////
	// ���\�b�h
	///////////////////////////////////////////

	// �R���X�g���N�^
	CLine( const D3DXVECTOR3* pos1 = NULL, const D3DXVECTOR3* pos2 = NULL )
	{
		// ���ɐݒ�
		SetColor( D3DCOLOR_XRGB( 0xff, 0xff, 0xff ) );
		
		// �ʒu�͒��g������ΐݒ�
		if( pos1 )
		{
			m_Line[ 0 ].Pos = *pos1;
		
		}else{

			m_Line[ 0 ].Pos = D3DXVECTOR3( 0, 0, 0 );
		}

		if( pos2 )
		{
			m_Line[ 1 ].Pos = *pos2;
		
		}else{

			m_Line[ 1 ].Pos = D3DXVECTOR3( 0, 0, 0 );
		}
	}

	// ���W�ݒ�
	inline void SetPos( const D3DXVECTOR3* pos1, const D3DXVECTOR3* pos2 )
	{
		m_Line[ 0 ].Pos = *pos1;
		m_Line[ 1 ].Pos = *pos2;
	}

	inline void SetPos( const D3DXVECTOR3* pos, const D3DXVECTOR3* dir, float length )
	{
		m_Line[ 0 ].Pos = *pos;
		m_Line[ 1 ].Pos = (*pos) + (*dir) * length;
	}

	// �F�ݒ�
	inline void SetColor( D3DCOLOR color )
	{
		m_Line[ 0 ].Color = m_Line[ 1 ].Color = color;
	}
	
	// �F�ݒ�
	inline void SetColor( D3DCOLOR color1, D3DCOLOR color2 )
	{
		m_Line[ 0 ].Color = color1;
		m_Line[ 1 ].Color = color2;
	}

	// �`��
	inline void Draw( const LPDIRECT3DDEVICE9 lpdevice ) const
	{
		// ���_�t�H�[�}�b�g
		lpdevice->SetFVF( D3DFVF_DIFFUSE | D3DFVF_XYZ );

		// �P�ʍs����͂߂Ă���
		CMatrix mMat;

		lpdevice->SetTransform( D3DTS_WORLD, &mMat );

		// �`��
		HRESULT hr = lpdevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, m_Line, sizeof( LINE ) );
	}
	
	// �f�X�g���N�^
	~CLine(){}

private:	// ����J

	///////////////////////////////////////////
	// ���̍\����
	///////////////////////////////////////////
	typedef struct
	{
		D3DXVECTOR3	Pos;	// �ʒu
		D3DCOLOR	Color;	// �F
	
	}LINE; 

	///////////////////////////////////////////
	// �t�B�[���h
	///////////////////////////////////////////
	LINE	m_Line[ 2 ];	// ��
};

#endif // __AKI_LINE_H__