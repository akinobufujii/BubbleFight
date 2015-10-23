#ifndef __AKI_VECTOR_H__
#define __AKI_VECTOR_H__

///////////////////////////////
// �x�N�g�����b�s���O�N���X
///////////////////////////////
class CVector : public D3DXVECTOR3
{
public:	// �O�����J

	///////////////////////////////
	// ���\�b�h
	///////////////////////////////

	// �R���X�g���N�^
	CVector();
	CVector( float x, float y, float z );
	CVector( const D3DXVECTOR3& v );
	CVector( const D3DXMATRIX&	mat );

	// �f�X�g���N�^(���ɖ���)
	~CVector(){}

	// ���ς����W�A���ŕԂ�
	float GetDotRadian( const D3DXVECTOR3* v);

	// ���ς��p�x�ŕԂ�
	float GetDotDegree( const D3DXVECTOR3* v );

	// �O�ς�Ԃ�
	CVector& GetCross( const D3DXVECTOR3* v );

	// ������Ԃ�
	float GetLength();

	// ������Ԃ�
	float GetDistance( const D3DXVECTOR3* v );

	// ���K��
	CVector& GetNormalVector();

	// ���W�ݒ�
	void Set( float x, float y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// ���Z�q
	CVector& operator= ( const D3DXVECTOR3& v );
	CVector operator- ( const D3DXVECTOR3& v ) const ;
	CVector operator+ ( const D3DXVECTOR3& v ) const ;

	// �ʒu�x�N�g��
	CVector& GetTransCoord( const D3DXMATRIX* m );

	// �����x�N�g��
	CVector& GetTransNormal( const D3DXMATRIX* m );
};

#endif // __AKI_VECTOR_H__