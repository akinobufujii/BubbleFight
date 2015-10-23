#ifndef __AKI_LINE_H__
#define __AKI_LINE_H__

///////////////////////////////////////////
// 線を描画するためだけのクラス
// (あまり内容がないので全てここで実装)
///////////////////////////////////////////
class CLine
{
public:	// 外部公開
	
	///////////////////////////////////////////
	// メソッド
	///////////////////////////////////////////

	// コンストラクタ
	CLine( const D3DXVECTOR3* pos1 = NULL, const D3DXVECTOR3* pos2 = NULL )
	{
		// 白に設定
		SetColor( D3DCOLOR_XRGB( 0xff, 0xff, 0xff ) );
		
		// 位置は中身があれば設定
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

	// 座標設定
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

	// 色設定
	inline void SetColor( D3DCOLOR color )
	{
		m_Line[ 0 ].Color = m_Line[ 1 ].Color = color;
	}
	
	// 色設定
	inline void SetColor( D3DCOLOR color1, D3DCOLOR color2 )
	{
		m_Line[ 0 ].Color = color1;
		m_Line[ 1 ].Color = color2;
	}

	// 描画
	inline void Draw( const LPDIRECT3DDEVICE9 lpdevice ) const
	{
		// 頂点フォーマット
		lpdevice->SetFVF( D3DFVF_DIFFUSE | D3DFVF_XYZ );

		// 単位行列をはめておく
		CMatrix mMat;

		lpdevice->SetTransform( D3DTS_WORLD, &mMat );

		// 描画
		HRESULT hr = lpdevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, m_Line, sizeof( LINE ) );
	}
	
	// デストラクタ
	~CLine(){}

private:	// 非公開

	///////////////////////////////////////////
	// 線の構造体
	///////////////////////////////////////////
	typedef struct
	{
		D3DXVECTOR3	Pos;	// 位置
		D3DCOLOR	Color;	// 色
	
	}LINE; 

	///////////////////////////////////////////
	// フィールド
	///////////////////////////////////////////
	LINE	m_Line[ 2 ];	// 線
};

#endif // __AKI_LINE_H__