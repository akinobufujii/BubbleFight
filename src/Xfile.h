#ifndef __AKI_XFILE_H__
#define __AKI_XFILE_H__

//////////////////////////////////////////////////
// X�t�@�C���ǂݍ��݃N���X
//////////////////////////////////////////////////
class CXfile
{
public:	// �O�����J

	//////////////////////////////////////////////////
	// �萔
	//////////////////////////////////////////////////
	CONSTANT DWORD				CUSTOM_FVF;	// ���_�t�H�[�}�b�g
	CONSTANT D3DVERTEXELEMENT9	MESH_ELEMENT[];	// ���_�t�H�[�}�b�g
		
	//////////////////////////////////////////////////
	// ���ʃ��\�b�h
	//////////////////////////////////////////////////

	//////////////////////////////////////////////////
	//	�E�֐���	SetShaderDevice
	//	�E����		�S�Ẵ��b�V���ɑ΂��Ďg�p����
	//				�V�F�[�_�f�o�C�X�ƃ��[���h�s��A
	//				�e�N�X�`���n���h����ݒ肷��
	//				SetDrawShader��true�ɂ���ƓK�������
	//	�E����		
	//		- lp_effect	�g�p����V�F�[�_�f�o�C�X
	//		- world		���[���h�s���n���n���h����
	//		- tex		�e�N�X�`����n���n���h����
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	static void SetShaderDevice( CEffect* lp_effect, D3DXHANDLE world, D3DXHANDLE tex = NULL )
	{
		m_lpEffect	= lp_effect;
		m_lphWorld	= world;
		m_lphTex	= tex;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetDrawShader
	//	�E����		�v���O���}�u���V�F�[�_�ŕ`�悷�邩��ݒ肷��
	//				SetShaderDevice���Ă�ł����K�v������
	//				true��n���Ƃ��̌�Draw���\�b�h��
	//				SetShaderDevice�Őݒ肳�ꂽ���ŕ`�悷��
	//	�E����		
	//		- is_draw	�V�F�[�_�ŕ`�悷�邩�ǂ���
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	static void SetDrawShader( const bool is_draw )
	{
		m_IsShaderDraw = is_draw;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetShaderParam
	//	�E����		���ݐݒ肳��Ă���
	//				�v���O���}�u���V�F�[�_�̏����l��
	//	�E����		
	//		- lpp_effect	�i�[����CEffect*�^�ϐ�
	//		- lp_world		���[���h�s����i�[����D3DXHANDLE*�^�ϐ�
	//		- lp_tex		�e�N�X�`���n���h�����i�[����D3DXHANDLE*�ϐ�
	//	�E�߂�l	�Ȃ�(�������߂�l)
	//////////////////////////////////////////////////
	static void GetShaderParam( CEffect** lpp_effect, D3DXHANDLE* lp_world, D3DXHANDLE* lp_tex )
	{
		*lpp_effect	= m_lpEffect;
		*lp_world	= m_lphWorld;
		*lp_tex		= m_lphTex;
	}

	//////////////////////////////////////////////////
	// ���\�b�h
	//////////////////////////////////////////////////
	
	// �R���X�g���N�^
	CXfile();
	CXfile( const LPDIRECT3DDEVICE9 lpD3DDevice, const LPCSTR name );

	// �f�X�g���N�^
	~CXfile();

	//////////////////////////////////////////////////
	//	�E�֐���	Load
	//	�E����		X�t�@�C���̓ǂݍ��݂��s��
	//	�E����		
	//		- lpdevice	�f�o�C�X
	//		- name		�t�@�C���p�X
	//	�E�߂�l	�ǂݍ��ݐ����Ȃ�true
	//				���s�Ȃ�false��Ԃ�
	//////////////////////////////////////////////////
	bool Load( const LPDIRECT3DDEVICE9 lpdevice, const LPCSTR name );

	//////////////////////////////////////////////////
	//	�E�֐���	Draw
	//	�E����		�ǂݍ���X�t�@�C���̕`����s��
	//				SetDrawShader��true�Őݒ肷���
	//				SetShaderDevice�Őݒ肳�ꂽ���e��
	//				�`����s��
	//	�E����		
	//		- lpdevice	�f�o�C�X
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Draw( const LPDIRECT3DDEVICE9 lpD3DDevice ) const;

	//////////////////////////////////////////////////
	//	�E�֐���	DrawShader
	//	�E����		�v���O���}�u���V�F�[�_�œǂݍ���
	//				X�t�@�C���̕`����s��
	//	�E����		
	//		- lp_shader			�v���O���}�u���V�F�[�_�f�o�C�X
	//		- diffuse_handle	�F��n���n���h��
	//							NULL�ł���Ύg�p����Ȃ�
	//		- tex_handle		�e�N�X�`����n���n���h��
	//							NULL�ł���Ύg�p����Ȃ�
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void DrawShader(
		CEffect*	lp_shader,
		D3DXHANDLE	diffuse_handle = NULL,
		D3DXHANDLE	tex_handle = NULL,
		DWORD		no_tex_pass = 0,
		DWORD		use_tex_pass = 1);

	//////////////////////////////////////////////////
	//	�E�֐���	CreateNormals
	//	�E����		�|���S���ɑ΂���@�������쐬����
	//	�E����		�Ȃ�
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void CreateNormals();

	//////////////////////////////////////////////////
	//	�E�֐���	SetRed
	//	�E����		�f�B�t���[�Y�̐Ԑ�����ݒ肷��
	//	�E����		
	//		- r		�ݒ肷��l(0.f�`1.f)
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetRed( const float r )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ].Diffuse.r = r;
		}
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetBlue
	//	�E����		�f�B�t���[�Y�̐Ԑ�����ݒ肷��
	//	�E����		
	//		- b		�ݒ肷��l(0.f�`1.f)
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetBlue( const float b )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ].Diffuse.b = b;
		}
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetGreen
	//	�E����		�f�B�t���[�Y�̐Ԑ�����ݒ肷��
	//	�E����		
	//		- g		�ݒ肷��l(0.f�`1.f)
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetGreen( const float g )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ].Diffuse.g = g;
		}
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetAlpha
	//	�E����		�f�B�t���[�Y�̐Ԑ�����ݒ肷��
	//	�E����		
	//		- a		�ݒ肷��l(0.f�`1.f)
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetAlpha( const float a )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ].Diffuse.a = a;
		}
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetEmissive
	//	�E����		���ȏƖ���ݒ肷��
	//	�E����		
	//		- emissive	�ݒ肷��l(0.f�`1.f)
	//					RGB�ꊇ�Őݒ肳���
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetEmissive( const float emissive )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ].Emissive.r = emissive;
			m_lpNowMatInfo[ i ].Emissive.g = emissive;
			m_lpNowMatInfo[ i ].Emissive.b = emissive;
		}
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetDefaultMaterial
	//	�E����		�}�e���A����ǂݍ��񂾎��_�̏��ɖ߂�
	//	�E����		�Ȃ�
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetDefaultMaterial()
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			m_lpNowMatInfo[ i ] = m_lpMatInfo[ i ];
		}
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetMeshData
	//	�E����		���b�V�������l��
	//	�E����		�Ȃ�
	//	�E�߂�l	���b�V�������|�C���^�ŕԂ�
	//////////////////////////////////////////////////
	inline const LPD3DXMESH* GetMeshData()	const 
	{
		return &m_lpMesh;
	} 

	//////////////////////////////////////////////////
	//	�E�֐���	GetNormal
	//	�E����		���b�V���ɑ΂���@������Ԃ�
	//				CreateNormals���Ă�ł����K�v������
	//	�E����		
	//		- num	�@�����̊i�[����Ă���C���f�b�N�X�ԍ�
	//	�E�߂�l	�@�����̊i�[����Ă���
	//				�C���f�b�N�X�ԍ��Ȃ�΁A�@����
	//				���݂��Ȃ��Ȃ�(0,0,0)�ŕԂ�
	//////////////////////////////////////////////////
	inline D3DXVECTOR3 GetNormal( DWORD num ) const 
	{
		if( m_lpNormals )
		{
			if( num >= 0 && num < m_lpMesh->GetNumFaces() )
			{
				return m_lpNormals[ num ];
			}
		}

		return D3DXVECTOR3( 0, 0, 0 );
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetCenter
	//	�E����		���b�V���̒��S�ʒu���l��
	//	�E����		�Ȃ�
	//	�E�߂�l	���b�V���̒��S�ʒu��Ԃ�
	//////////////////////////////////////////////////
	inline D3DXVECTOR3 GetCenter() const 
	{
		return m_CenterPos;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetRadius
	//	�E����		���b�V���̔��a���l��
	//	�E����		�Ȃ�
	//	�E�߂�l	���b�V���̔��a��Ԃ�
	//////////////////////////////////////////////////
	inline float GetRadius() const 
	{
		return m_Radius;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	GetMaterialNum
	//	�E����		�}�e���A�������l��
	//	�E����		�Ȃ�
	//	�E�߂�l	�}�e���A������Ԃ�
	//////////////////////////////////////////////////
	inline DWORD GetMaterialNum() const 
	{
		return m_MatNum;
	}

private:	// ����J

	

	//////////////////////////////////////////////////
	// �t�B�[���h
	//////////////////////////////////////////////////
	LPD3DXMESH						m_lpMesh;		// ���b�V��
	DWORD							m_MatNum;		// �}�e���A����
	LPMATERIALARRAY					m_lpMatInfo;	// �}�e���A�����
	LPMATERIALARRAY					m_lpNowMatInfo;	// �}�e���A�����݂̏��
	LPTEXTUREARRAY					m_lppTex;		// �e�N�X�`��
	LPDIRECT3DVERTEXDECLARATION9	m_lpDec;		// ���_�f�N�����[�V����(�g��Ȃ�)
	D3DXVECTOR3						m_CenterPos;	// ���b�V���̒��S
	LPVECTOR3ARRAY					m_lpNormals;	// �@�����
	float							m_Radius;		// ���a

	//////////////////////////////////////////////////
	// ���ʃt�B�[���h
	//////////////////////////////////////////////////
	static bool			m_IsShaderDraw;	// �V�F�[�_�[�ŕ`�悷�邩�ǂ���
	static CEffect*		m_lpEffect;		// �V�F�[�_�[
	static D3DXHANDLE	m_lphWorld;		// �s���n���n���h��
	static D3DXHANDLE	m_lphTex;		// �e�N�X�`����n���n���h��

	// ����֎~
	CXfile( const CXfile& obj );
	CXfile& operator =( const CXfile& obj );

};

#endif // __AKI_XFILE_H__