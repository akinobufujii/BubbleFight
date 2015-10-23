///////////////////////////
// インクルード
///////////////////////////
#include "Vector.h"

///////////////////////////
// 実装
///////////////////////////

// コンストラクタ
CVector::CVector()
{
	this->x = this->y = this->z = 0.f;
}

// 初期値代入
CVector::CVector( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// 初期化対応
CVector::CVector( const D3DXVECTOR3& v )
{
	*this = v;
}

// 行列から座標代入
CVector::CVector(const D3DXMATRIX &mat)
{
	this->x	= mat._41;
	this->y	= mat._42;
	this->z	= mat._43;
}

// 内積をラジアンで返す
float CVector::GetDotRadian( const D3DXVECTOR3* v )
{
	//GetNormalVector();
	return D3DXVec3Dot( this, v );
}

// 内積を角度で返す
float CVector::GetDotDegree( const D3DXVECTOR3* v )
{
	float Dot = GetDotRadian( v );

	if( Dot > 1.f )
	{
		Dot = 1.f;
	}

	if( Dot < -1.f )
	{
		Dot = -1.f;
	}

	return D3DXToDegree( acos( Dot ) );
}

// 外積を返す
CVector& CVector::GetCross( const D3DXVECTOR3* v )
{
	D3DXVec3Cross( this, this, v );

	return *this;
}

// 長さを返す
float CVector::GetLength()
{
	return D3DXVec3Length( this );
}

// 距離を返す
float CVector::GetDistance( const D3DXVECTOR3* v )
{
	CVector Temp = *this - *v;

	return Temp.GetLength();
}

// 正規化
CVector& CVector::GetNormalVector()
{
	D3DXVec3Normalize( this, this );

	return *this;
}

// 代入
CVector& CVector::operator= ( const D3DXVECTOR3& v )
{
	if( this == &v )
	{
		return *this;
	}

	D3DXVECTOR3::operator =( v );

	return *this;
}

// 減算
CVector CVector::operator- ( const D3DXVECTOR3& v )const
{
	CVector Vec = D3DXVECTOR3::operator -( v );

	return Vec;
}

// 加算
CVector CVector::operator+ ( const D3DXVECTOR3& v )const
{
	CVector Vec = D3DXVECTOR3::operator +( v );

	return Vec;
}

// 位置ベクトル
CVector& CVector::GetTransCoord( const D3DXMATRIX* m )
{
	D3DXVec3TransformCoord( this, this, m );

	return *this;
}

// 方向ベクトル
CVector& CVector::GetTransNormal( const D3DXMATRIX* m )
{
	D3DXVec3TransformNormal( this, this, m );

	return *this;
}