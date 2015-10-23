#ifndef __AKI_MATRIX_H__
#define __AKI_MATRIX_H__

///////////////////////////
// �s�񃉃b�s���O�N���X
///////////////////////////
class CMatrix :	public D3DXMATRIX
{
public:	// �O�����J

	///////////////////////////
	// ���\�b�h
	///////////////////////////

	// �R���X�g���N�^
	CMatrix();
	CMatrix( const D3DXMATRIX& m );
	CMatrix( float x, float y, float z );
	CMatrix( const D3DXVECTOR3& vec ); 

	// �f�X�g���N�^
	~CMatrix(){}

	// �ړ�
	void SetTrans( float x, float y, float z );
	void SetTrans( const D3DXVECTOR3* vec );

	// �ړ�(���[�J�����W)
	void MoveLocal( float x, float y, float z );
	void MoveLocal( const D3DXVECTOR3* vec );

	// ��]
	void SetRotX( float x );
	void SetRotY( float y );
	void SetRotZ( float z );
	void SetRotAxis( const D3DXVECTOR3* vec, float rot );

	// ��](���[�J�����W)
	void RotateLocalX( float x );
	void RotateLocalY( float y );
	void RotateLocalZ( float z );
	void RotateLocalAxis( const D3DXVECTOR3* vec, float rot );

	// �g��
	void SetScale( float x, float y, float z );
	void SetScale( float scale );

	// �g��(���[�J�����W)
	void ScaleLocal( float scale );
	void ScaleLocalX( float x );
	void ScaleLocalY( float y );
	void ScaleLocalZ( float z );
	
	// �t�s��
	CMatrix& GetInverse();

	// ���W�l��
	D3DXVECTOR3	GetPos() const;

	// ���
	D3DXMATRIX& operator= ( const D3DXMATRIX& m );

	// ���ꂼ��̉�]�p�x���v�Z���ĕԂ�
	float GetRotX() const; 
	float GetRotY() const; 
	float GetRotZ() const; 
};

#endif // __AKI_MATRIX_H__