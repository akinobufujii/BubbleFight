#ifndef __AKI_EFFECT_H__
#define __AKI_EFFECT_H__

////////////////////////////////
// �V�F�[�_�}�l�[�W��
////////////////////////////////
class CEffect
{
public:	// �O�����J

	//////////////////////////////
	// ���\�b�h
	//////////////////////////////

	// �R���X�g���N�^
	CEffect();
	CEffect( LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, LPCSTR technique );

	// �f�X�g���N�^
	~CEffect();

	// �ǂݍ���
	bool Load(LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, LPCSTR technique );

	// �`��J�n
	const LPD3DXEFFECT Begin( LPCSTR technique = NULL );

	// �`��I��
	void End();

	// �e�N�j�b�N��ݒ肷��
	void SetTechnique( LPCSTR technique_name );

	// �p�X�ݒ�
	void BeginPass( int pass );
	void EndPass();

	// �l��ݒ肷��
	void SetValue( LPCSTR handle, LPCVOID lp_value, UINT byte );
	
	// �e�N�X�`����ݒ�
	void SetTexture( LPCSTR handle, LPDIRECT3DBASETEXTURE9 lp_tex );
	
	// �n���h�������
	bool CreateHandle( LPCSTR handle_name );

	// �n���h�����l��
	D3DXHANDLE GetHandle( LPCSTR handle_name );

	// �f�o�C�X�l��
	inline const LPD3DXEFFECT GetDevice()
	{
		return m_lpEffect;
	}


private:	// ����J
	//////////////////////////////
	// �t�B�[���h
	//////////////////////////////
	LPD3DXEFFECT					m_lpEffect;	// �V�F�[�_�f�o�C�X
	std::map< LPCSTR, D3DXHANDLE >	m_HandleMap;// �n���h��
	LPCSTR							m_TecKey;	// �e�N�j�b�N�L�[

	//////////////////////////////
	// ���\�b�h
	//////////////////////////////

	// �n���h���`�F�b�N
	D3DXHANDLE CheckHandle( LPCSTR handle_name );

	//////////////////////////////
	// �t�����h�N���X�w��
	//////////////////////////////
	friend class CSShaderManager;
};

#endif // __AKI_EFFECT_H__