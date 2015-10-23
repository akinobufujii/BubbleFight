#ifndef __AKI_CAMERA_H__
#define __AKI_CAMERA_H__

//////////////////////
// �J�����N���X
//////////////////////
class CCamera
{
public:	// �O�����J

	//////////////////////
	// ���\�b�h
	//////////////////////

	// �R���X�g���N�^
	CCamera();
	CCamera( const D3DXVECTOR3* pos, const D3DXVECTOR3* look, const D3DXVECTOR3* up );

	// ���_�ʒu��ݒ�
	const D3DXMATRIX* SetCameraPos( const D3DXVECTOR3* pos );

	// �����_�̐ݒ�
	const D3DXMATRIX* SetCameraLook( const D3DXVECTOR3* look );

	// ������̐ݒ�
	const D3DXMATRIX* SetCameraUp( const D3DXVECTOR3* up );

	// �J������ݒ�(���ׂĂ̓��e)
	const D3DXMATRIX* SetCamera( const D3DXVECTOR3* pos, const D3DXVECTOR3* look, const D3DXVECTOR3* up );

	// �r���{�[�h�ׂ̈̍s��l��
	void GetBillboard( LPD3DXMATRIX out, const LPD3DXMATRIX pos ) const;

	// �J�����ʒu
	inline const D3DXVECTOR3* GetPos() const
	{
		return &m_vPos;
	} 

	// �����_
	inline const D3DXVECTOR3* GetLook() const 
	{
		return &m_vlook;
	}

	// �����
	inline const D3DXVECTOR3* GetUp() const
	{
		return &m_vUp;
	}

	// ���_�s��
	inline const D3DXMATRIX* GetMatrix() const
	{
		return &m_mMat;
	}

	// �������Ă���x�N�g��
	D3DXVECTOR3 GetScreenVector() const;

	// �f�X�g���N�^
	~CCamera();

private:	// ����J

	//////////////////////
	// �t�B�[���h
	//////////////////////
	CMatrix	m_mMat;	// �J�����s��
	CVector	m_vPos;	// �J�����ʒu
	CVector	m_vlook;// �J���������_
	CVector	m_vUp;	// �J���������
};

#endif // __AKI_CAMERA_H__