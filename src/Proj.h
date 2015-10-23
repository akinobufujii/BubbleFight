#ifndef __AKI_PROJ_H__
#define __AKI_PROJ_H__

/////////////////////////////////
// ���e�N���X
/////////////////////////////////
class CProj
{
public:	// �O�����J

	/////////////////////////////////
	// ���\�b�h
	/////////////////////////////////

	// �R���X�g���N�^
	CProj( float fovy = 45.f, float aspect = 4.f / 3.f, float zn = 1.f, float zf = 100.f );

	// �j�A�N���b�v�ݒ�
	const D3DXMATRIX* SetNear( float zn );

	// �t�@�[�N���b�v�ݒ�
	const D3DXMATRIX* SetFar( float zf );

	// ��p�ݒ�
	const D3DXMATRIX* SetFovy( float fovy );

	// �A�X�y�N�g���ݒ�
	const D3DXMATRIX* SetAspect( float aspect );

	// �S���ݒ�
	const D3DXMATRIX* SetProj( float fovy, float aspect, float zn, float zf );

	// �Q�b�^�[
	// ��p
	inline float GetFovy() const
	{
		return m_Fovy;
	} 

	// �j�A�N���b�v
	inline float GetNearClip() const
	{
		return m_Near;
	}

	// �t�@�[�N���b�v
	inline float GetFarClip() const 
	{
		return m_Far;
	}

	// �A�X�y�N�g��
	inline float GetAspect() const
	{
		return m_Aspect;
	}

	// ���e�s��
	inline const D3DXMATRIX* GetMatrix() const
	{
		return &m_mMat;
	}

	// �f�X�g���N�^
	~CProj();

private:	// ����J

	/////////////////////////////////
	// �t�B�[���h
	/////////////////////////////////
	D3DXMATRIX	m_mMat;		// ���e�s��

	float		m_Fovy;		// ��p
	float		m_Near;		// �j�A�N���b�v
	float		m_Far;		// �t�@�[�N���b�v
	float		m_Aspect;	// �A�X�y�N�g��
};

#endif // __AKI_PROJ_H__