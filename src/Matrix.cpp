///////////////////////////
// インクルード
///////////////////////////
#include "Matrix.h"

///////////////////////////
// 実装
///////////////////////////

// コンストラクタ
CMatrix::CMatrix()
{
	D3DXMatrixIdentity( this );
}

// コピーコンストラクタ
CMatrix::CMatrix( const D3DXMATRIX& m )
{
	*this = m;
}

// 初期値代入
CMatrix::CMatrix( float x, float y, float z )
{
	D3DXMatrixTranslation( this, x, y, z );
}

CMatrix::CMatrix( const D3DXVECTOR3& vec )
{
	D3DXMatrixTranslation( this, vec.x, vec.y, vec.z );
}

// 移動
void CMatrix::SetTrans( float x, float y, float z )
{
	D3DXMatrixTranslation( this, x, y, z );
}

void CMatrix::SetTrans( const D3DXVECTOR3* vec )
{
	D3DXMatrixTranslation( this, vec->x, vec->y, vec->z );
}

// 移動(ローカル座標)
void CMatrix::MoveLocal( float x, float y, float z )
{
	D3DXMATRIX mTemp;

	D3DXMatrixTranslation( &mTemp, x, y, z );

	*this = mTemp * (*this);
}

void CMatrix::MoveLocal( const D3DXVECTOR3* vec )
{
	MoveLocal( vec->x, vec->y, vec->z );
}

// 回転
void CMatrix::SetRotX( float x )
{
	D3DXMatrixRotationX( this, D3DXToRadian( x ) );
}

void CMatrix::SetRotY( float y )
{
	D3DXMatrixRotationY( this, D3DXToRadian( y ) );
}

void CMatrix::SetRotZ( float z )
{
	D3DXMatrixRotationZ( this, D3DXToRadian( z ) );
}

void CMatrix::SetRotAxis( const D3DXVECTOR3* vec, float rot )
{
	D3DXMatrixRotationAxis( this, vec, D3DXToRadian( rot ) );
}

// 回転(ローカル座標)
void CMatrix::RotateLocalX( float x )
{
	D3DXMATRIX mTemp;

	D3DXMatrixRotationX( &mTemp, D3DXToRadian( x ) );

	*this = mTemp * (*this);
}

void CMatrix::RotateLocalY( float y )
{
	D3DXMATRIX mTemp;

	D3DXMatrixRotationY( &mTemp, D3DXToRadian( y ) );

	*this = mTemp * (*this);
}

void CMatrix::RotateLocalZ( float z )
{
	D3DXMATRIX mTemp;

	D3DXMatrixRotationZ( &mTemp, D3DXToRadian( z ) );

	*this = mTemp * (*this);
}

void CMatrix::RotateLocalAxis( const D3DXVECTOR3* vec, float rot )
{
	D3DXMATRIX mTemp;

	D3DXMatrixRotationAxis( &mTemp, vec, D3DXToRadian( rot ) );

	*this = mTemp * (*this);
}

// 拡大
void CMatrix::SetScale( float x, float y, float z )
{
	D3DXMatrixScaling( this, x, y, z );
}

void CMatrix::SetScale( float scale )
{
	D3DXMatrixScaling( this, scale, scale, scale );
}

// 拡大(ローカル座標)
void CMatrix::ScaleLocal( float scale )
{
	D3DXMATRIX mTemp;

	D3DXMatrixScaling( &mTemp, scale, scale, scale );

	*this = mTemp * (*this);
}

void CMatrix::ScaleLocalX( float x )
{
	D3DXMATRIX mTemp;

	D3DXMatrixScaling( &mTemp, x, 0.f, 0.f );

	*this = mTemp * (*this);
}

void CMatrix::ScaleLocalY( float y )
{
	D3DXMATRIX mTemp;

	D3DXMatrixScaling( &mTemp, 0.f, y, 0.f );

	*this = mTemp * (*this);
}

void CMatrix::ScaleLocalZ( float z )
{
	D3DXMATRIX mTemp;

	D3DXMatrixScaling( &mTemp, 0.f, 0.f, z );

	*this = mTemp * (*this);
}

// 逆行列
CMatrix& CMatrix::GetInverse()
{
	D3DXMatrixInverse( this, NULL, this );
	
	return *this;
}

// 座標獲得
D3DXVECTOR3	CMatrix::GetPos() const 
{
	return D3DXVECTOR3( this->_41, this->_42, this->_43 );
}

// 代入
D3DXMATRIX& CMatrix::operator= ( const D3DXMATRIX& m )
{
	if( this == &m )
	{
		return *this;
	}

	D3DXMATRIX::operator =( m );

	return *this;
}

// X軸回転獲得
float CMatrix::GetRotX() const
{
	CVector Vec( 0, 0, 1 );

	// 自らの逆行列を作る
	D3DXMATRIX mInv;
	D3DXMatrixInverse( &mInv, NULL, this );
	
	Vec.GetTransNormal( &mInv );
	
	Vec.x = 0;
	Vec.GetNormalVector();

	float Rot = Vec.GetDotDegree( &D3DXVECTOR3( 0, 0, 1 ) );

	if( Rot < 1 )
	{
		return 0;
	}

	// 方向を見て正負を決定する
	return (Vec.y < 0)? Rot : -Rot;
}

// Y軸回転獲得
float CMatrix::GetRotY() const
{
	CVector Vec( 0, 0, 1 );

	// 自らの逆行列を作る
	D3DXMATRIX mInv;
	D3DXMatrixInverse( &mInv, NULL, this );
	
	Vec.GetTransNormal( &mInv );

	Vec.y = 0;
	Vec.GetNormalVector();

	float Rot = Vec.GetDotDegree( &D3DXVECTOR3( 0, 0, 1 )  );

	if( Rot < 1 )
	{
		return 0;
	}

	// 方向を見て正負を決定する
	return (Vec.x > 0)? Rot : -Rot;
}

// Z軸回転獲得
float CMatrix::GetRotZ() const
{
	CVector Vec( 0, 1, 0 );

	// 自らの逆行列を作る
	D3DXMATRIX mInv;
	D3DXMatrixInverse( &mInv, NULL, this );

	Vec.GetTransNormal( &mInv );
	
	Vec.z = 0;
	Vec.GetNormalVector();

	float Rot = Vec.GetDotDegree( &D3DXVECTOR3( 0, 1, 0 ) );

	if( Rot < 1 )
	{
		return 0;
	}

	// 方向を見て正負を決定する
	return (Vec.x > 0)? Rot : -Rot;
}