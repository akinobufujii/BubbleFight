#ifndef __AKI_TEXTURE_H__
#define	__AKI_TEXTURE_H__

//////////////////////////////////////////////////
// �e�N�X�`���N���X
// 1�I�u�W�F�N�g��1��
//////////////////////////////////////////////////
class CTexture : public boost::noncopyable
{
public:

	//////////////////////////////////////////////////
	// ���\�b�h
	//////////////////////////////////////////////////

	// �R���X�g���N�^
	CTexture();
	
	// �f�X�g���N�^
	~CTexture();

	//////////////////////////////////////////////////
	//	�E�֐���	CreateFile
	//	�E����		�t�@�C������e�N�X�`�����쐬����
	//				�����2�ׂ̂���T�C�Y�ɒ�������
	//	�E����		
	//		- lpdevice	�f�o�C�X
	//		- file_name	�ǂݍ��މ摜�̃t�@�C����
	//		- color_key	�`�悵�Ȃ��F
	//	�E�߂�l	�ǂݍ��߂�΃e�N�X�`����
	//				�ǂݍ��߂Ȃ����NULL��Ԃ�
	//////////////////////////////////////////////////
	const LPDIRECT3DTEXTURE9 CreateFile( LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, D3DCOLOR color_key );

	//////////////////////////////////////////////////
	//	�E�֐���	CreateRenderTarget
	//	�E����		�t�@�C������e�N�X�`�����쐬����
	//				�����2�ׂ̂���T�C�Y�ɒ�������
	//	�E����		
	//		- lpdevice	�f�o�C�X
	//		- width		��
	//		- height	����
	//		- format	�t�H�[�}�b�g
	//	�E�߂�l	�ǂݍ��߂�΃e�N�X�`����
	//				�ǂݍ��߂Ȃ����NULL��Ԃ�
	//////////////////////////////////////////////////
	const LPDIRECT3DTEXTURE9 CreateRenderTarget( LPDIRECT3DDEVICE9 lpdevice, UINT width, UINT height, D3DFORMAT format = D3DFMT_A8B8G8R8 );

	//////////////////////////////////////////////////
	//	�E�֐���	GetTexture
	//	�E����		�����I�Ƀe�N�X�`�����l������
	//	�E����		�Ȃ�
	//	�E�߂�l	�e�N�X�`������Ԃ�
	//////////////////////////////////////////////////
	inline const LPDIRECT3DTEXTURE9 GetTexture() const
	{
		return m_lpTex;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	operator const LPDIRECT3DTEXTURE9()
	//	�E����		�ÖقɃe�N�X�`�����l������
	//	�E����		�Ȃ�
	//	�E�߂�l	�e�N�X�`������Ԃ�
	//////////////////////////////////////////////////
	inline operator const LPDIRECT3DTEXTURE9() const
	{
		return m_lpTex;
	}
	
	//////////////////////////////////////////////////
	//	�E�֐���	GetZbuffer
	//	�E����		�����_�[�^�[�Q�b�g��
	//				�[�x�o�b�t�@�����l��
	//	�E����		�Ȃ�
	//	�E�߂�l	�[�x�o�b�t�@����Ԃ�
	//////////////////////////////////////////////////
	inline const LPDIRECT3DSURFACE9 GetZbuffer() const
	{
		return m_lpZbuffer;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetSurface
	//	�E����		�����_�[�^�[�Q�b�g��
	//				�T�[�t�F�X�����l��
	//	�E����		�Ȃ�
	//	�E�߂�l	�T�[�t�F�X����Ԃ�
	//////////////////////////////////////////////////
	inline const LPDIRECT3DSURFACE9 GetSurface() const
	{
		return m_lpSurface;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetSurfaceDesc
	//	�E����		�����_�[�^�[�Q�b�g��
	//				�ڂ��������l��
	//	�E����		�Ȃ�
	//	�E�߂�l	�ڂ�������Ԃ�
	//////////////////////////////////////////////////
	inline const D3DSURFACE_DESC& GetSurfaceDesc() const
	{
		return m_Desc;
	}

private:	// ����J
		
	//////////////////////////////////////////////////
	// �t�B�[���h
	//////////////////////////////////////////////////
	LPDIRECT3DTEXTURE9		m_lpTex;		// �e�N�X�`��
	LPDIRECT3DSURFACE9		m_lpZbuffer;	// Z�o�b�t�@
	LPDIRECT3DSURFACE9		m_lpSurface;	// �T�[�t�F�X
	D3DSURFACE_DESC			m_Desc;			// �e�N�X�`�����
};

#endif // __AKI_TEXTURE_H__