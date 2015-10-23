///////////////////////////
// �C���N���[�h
///////////////////////////
#include "Vector.h"

///////////////////////////
// ����
///////////////////////////

// �R���X�g���N�^
CVector::CVector()
{
	this->x = this->y = this->z = 0.f;
}

// �����l���
CVector::CVector( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// �������Ή�
CVector::CVector( const D3DXVECTOR3& v )
{
	*this = v;
}

// �s�񂩂���W���
CVector::CVector(const D3DXMATRIX &mat)
{
	this->x	= mat._41;
	this->y	= mat._42;
	this->z	= mat._43;
}

// ���ς����W�A���ŕԂ�
float CVector::GetDotRadian( const D3DXVECTOR3* v )
{
	//GetNormalVector();
	return D3DXVec3Dot( this, v );
}

// ���ς��p�x�ŕԂ�
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

// �O�ς�Ԃ�
CVector& CVector::GetCross( const D3DXVECTOR3* v )
{
	D3DXVec3Cross( this, this, v );

	return *this;
}

// ������Ԃ�
float CVector::GetLength()
{
	return D3DXVec3Length( this );
}

// ������Ԃ�
float CVector::GetDistance( const D3DXVECTOR3* v )
{
	CVector Temp = *this - *v;

	return Temp.GetLength();
}

// ���K��
CVector& CVector::GetNormalVector()
{
	D3DXVec3Normalize( this, this );

	return *this;
}

// ���
CVector& CVector::operator= ( const D3DXVECTOR3& v )
{
	if( this == &v )
	{
		return *this;
	}

	D3DXVECTOR3::operator =( v );

	return *this;
}

// ���Z
CVector CVector::operator- ( const D3DXVECTOR3& v )const
{
	CVector Vec = D3DXVECTOR3::operator -( v );

	return Vec;
}

// ���Z
CVector CVector::operator+ ( const D3DXVECTOR3& v )const
{
	CVector Vec = D3DXVECTOR3::operator +( v );

	return Vec;
}

// �ʒu�x�N�g��
CVector& CVector::GetTransCoord( const D3DXMATRIX* m )
{
	D3DXVec3TransformCoord( this, this, m );

	return *this;
}

// �����x�N�g��
CVector& CVector::GetTransNormal( const D3DXMATRIX* m )
{
	D3DXVec3TransformNormal( this, this, m );

	return *this;
}