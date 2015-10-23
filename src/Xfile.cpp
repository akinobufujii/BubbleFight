//////////////////////////////
// �C���N���[�h
//////////////////////////////
#include "Xfile.h"

//////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////
const DWORD				CXfile::CUSTOM_FVF	= D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
const D3DVERTEXELEMENT9	CXfile::MESH_ELEMENT[]	= 
{
	{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,		0},
	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0},
	{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,	0},
	D3DDECL_END()
};

bool			CXfile::m_IsShaderDraw	= false;
CEffect*		CXfile::m_lpEffect		= NULL;
D3DXHANDLE		CXfile::m_lphWorld		= NULL;
D3DXHANDLE		CXfile::m_lphTex		= NULL;

//////////////////////////////
// ����
//////////////////////////////

// �R���X�g���N�^
CXfile::CXfile():
m_lpMesh(NULL),
m_MatNum(0),
m_lpMatInfo(),
m_lppTex(),
m_CenterPos( 0, 0, 0 ),
m_Radius( 0.f ),
m_lpNormals(),
m_lpDec( NULL )
{
	// �������Ȃ�
}

// �R�s�[�R���X�g���N�^
CXfile::CXfile(const CXfile &object)
{
	//// ���̂܂܃R�s�[�ł�����̂��R�s�[����
	//m_MatNum	= object.m_MatNum;
	//m_CenterPos	= object.m_CenterPos;
	//m_Radius	= object.m_Radius;

	//// ���̂܂܃R�s�[�o���Ȃ�����
	//// ���I�z��m��
	//m_lpMatInfo	= LPMATERIALARRAY( NEW D3DMATERIAL9[ m_MatNum ] );
	//m_lppTex	= NEW LPDIRECT3DTEXTURE9[ m_MatNum ];

	//// �R�s�[
	//for( UINT i = 0; i < m_MatNum; i++ )
	//{
	//	m_lpMatInfo[ i ]	= object.m_lpMatInfo[ i ];
	//	//m_lppTex[ i ]		= object.m_lppTex[ i ]->
	//}
}

// �R���X�g���N�^(���[�h�t��)
CXfile::CXfile( const LPDIRECT3DDEVICE9 lpD3DDevice, const LPCSTR name):
m_lpMesh(NULL),
m_MatNum(0),
m_lpMatInfo(),
m_lppTex(),
m_CenterPos( 0, 0, 0 ),
m_Radius( 0.f ),
m_lpNormals(),
m_lpDec( NULL )
{
	// �����o�ϐ�������
	CXfile();

	Load( lpD3DDevice, name );
}

// �f�X�g���N�^
CXfile::~CXfile()
{
	// �����[�X
	if( m_lpMesh )
	{
		SAFE_RELEASE( m_lpDec );
		SAFE_RELEASE( m_lpMesh );
	}
}


// �ǂݍ���
bool CXfile::Load(const LPDIRECT3DDEVICE9 lpD3DDevice, const LPCSTR name)
{
	LPD3DXBUFFER lpBuffer;

	// �����ǂݍ��܂�Ă���
	if( m_lpMesh )
	{
		this->~CXfile();
	}

	// �t�@�C���ǂݍ���
	D3DXLoadMeshFromX(
		name,		// �t�@�C����
		D3DXMESH_MANAGED,	// �ǂݍ��݃I�v�V����
		lpD3DDevice,		// �f�o�C�X 
		NULL,				//  
		&lpBuffer,			// �o�b�t�@
		NULL,
		&m_MatNum,			// �}�e���A����
		&m_lpMesh );		// ���b�V��

	// �}�e���A�������z��m��
	m_lpMatInfo		= LPMATERIALARRAY( NEW D3DMATERIAL9[ m_MatNum ] );
	m_lpNowMatInfo	= LPMATERIALARRAY( NEW D3DMATERIAL9[ m_MatNum ] );
	m_lppTex		= LPTEXTUREARRAY( NEW LPDIRECT3DTEXTURE9[ m_MatNum ] );

	D3DXMATERIAL*	lpMat =
		reinterpret_cast<D3DXMATERIAL*>( lpBuffer->GetBufferPointer() );

	// �����i�[
	for( DWORD	i = 0;	i < m_MatNum; i++ )
	{
		m_lpMatInfo[ i ] = lpMat[ i ].MatD3D;
		m_lpMatInfo[ i ].Ambient= m_lpMatInfo[ i ].Diffuse;

		// �e�N�X�`��
		m_lppTex[ i ] = CSMeshTexManager::GetInst()->Load(
			lpD3DDevice, lpMat[ i ].pTextureFilename );
		
		// �R�s�[
		m_lpNowMatInfo[ i ] = m_lpMatInfo[ i ];
	}

	// �o�b�t�@���
	SAFE_RELEASE( lpBuffer );

	// �~�Փ˔���p�ɒ��S�Ɣ��a���Z�o
	LPDIRECT3DVERTEXBUFFER9 lpVB = NULL;
	LPVOID lpVertexs;

	// ���_�o�b�t�@���b�N
	m_lpMesh->GetVertexBuffer( &lpVB );
	lpVB->Lock( 0, 0, &lpVertexs, 0 );

	// ���S�Ɣ��a���Z�o
	D3DXComputeBoundingSphere(
		reinterpret_cast<D3DXVECTOR3*>(lpVertexs),	// ���_�̍ŏ��̈ʒu
		m_lpMesh->GetNumVertices(),					// ���_��
		D3DXGetFVFVertexSize( m_lpMesh->GetFVF() ),	// ���_�t�H�[�}�b�g��
		&m_CenterPos,								// ���S�ʒu�i�[
		&m_Radius );								// ���a�i�[

	// ���_�o�b�t�@�A�����b�N
	lpVB->Unlock();

	SAFE_RELEASE( lpVB );

	// �œK��
	LPD3DXMESH	lpTemp;	// ���b�V���̃e���|����

	// �}�X�^�[�f�[�^����N���[�������
	m_lpMesh->CloneMesh( m_lpMesh->GetOptions(), MESH_ELEMENT, lpD3DDevice, &lpTemp );

	// �ڐ��ǉ�����
	D3DXComputeTangent( lpTemp, 0, 0, D3DX_DEFAULT, TRUE, NULL );

	// ���̐�̓e���|�����̓��e���}�X�^�[�f�[�^�Ƃ��Ĉ���
	// �����b�V���������e��ύX����
	m_lpMesh->Release();
	m_lpMesh = lpTemp;

	return true;
}

// �`��
void CXfile::Draw(LPDIRECT3DDEVICE9 lpD3DDevice) const
{
	if( m_IsShaderDraw && m_lpEffect && m_lphWorld )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			D3DXMATRIX mWorld;

			// �f�o�C�X����s��l��
			D3D::GetDevice()->GetTransform( D3DTS_WORLD, &mWorld );

			m_lpEffect->SetValue( m_lphWorld, &mWorld, sizeof(D3DXMATRIX) );

			// �e�N�X�`���n���h��������ΐݒ�
			if( m_lphTex )
			{
				m_lpEffect->SetTexture( m_lphTex, m_lppTex[ i ] );
			}

			m_lpMesh->DrawSubset( i );							// �`��
		}

	}else{
	
		for( DWORD i = 0; i < m_MatNum; i++ )
		{
			lpD3DDevice->SetMaterial( &m_lpNowMatInfo[ i ] );	// �ގ��ݒ�
			lpD3DDevice->SetTexture( 0, m_lppTex[ i ]);			// �e�N�X�`������
			m_lpMesh->DrawSubset( i );							// �`��
		}
	}
}

// �V�F�[�_�[�`��
void CXfile::DrawShader(CEffect		*lp_shader,
						D3DXHANDLE	diffuse_handle,
						D3DXHANDLE	tex_handle,
						DWORD		no_tex_pass,
						DWORD		use_tex_pass)
{
	for( DWORD i = 0; i < m_MatNum; i++ )
	{
		if( diffuse_handle )
		{
			lp_shader->SetValue(
				diffuse_handle,
				&m_lpNowMatInfo[ i ].Diffuse,
				sizeof( m_lpNowMatInfo[ i ].Diffuse ) );
		}

		if( m_lppTex[ i ] )
		{
			if( tex_handle  )
			{
				lp_shader->SetTexture( tex_handle, m_lppTex[ i ] );
			}
		
			lp_shader->BeginPass( use_tex_pass );

		}else{
		
			lp_shader->BeginPass( no_tex_pass );
		}
		
		m_lpMesh->DrawSubset( i );

		lp_shader->EndPass();
	}
}

// �@���������
void CXfile::CreateNormals()
{
	// ������������΍��Ȃ�
	if( m_lpNormals )
	{
		return;
	}

	D3DXVECTOR3				VPos[ 3 ];								// ���_�i�[�p�e���|����
	LPWORD					lpIndex		= NULL;						// �C���f�b�N�X�i�[�p
	DWORD					FaceNum		= m_lpMesh->GetNumFaces();	// �|���S����
	LPDIRECT3DVERTEXBUFFER9	lpVBuffer	= NULL;						// ���_�o�b�t�@
	LPBYTE					lpBufferInfo= NULL;						// �o�b�t�@���
	float*					lpInfoArray	= NULL;						// ���(�z��̂悤�Ɏg�p)
	DWORD					Stride		= m_lpMesh->GetNumBytesPerVertex();	// ���_���o�C�g��

	// ���I�z��m��
	m_lpNormals = LPVECTOR3ARRAY( NEW D3DXVECTOR3[ FaceNum ] );

	// �@���v�Z�J�n
	for( DWORD i = 0; i < FaceNum; i++ )
	{
		// �C���f�b�N�X�o�b�t�@���b�N
		m_lpMesh->LockIndexBuffer( 0, reinterpret_cast<LPVOID*>( &lpIndex ) );

		// ���_�o�b�t�@���b�N
		m_lpMesh->GetVertexBuffer( &lpVBuffer );
		lpVBuffer->Lock( 0, 0, reinterpret_cast<LPVOID*>( &lpBufferInfo ), 0 );
		
		// �@���o�b�t�@���邩�炻����g���΂����Ǝv����
		// ���C�e�B���O�̏����֌W�ŐM�p�o���Ȃ����
		// �|���S���ɑ΂��Đ����Ȗ@������ɓ����
		// ���_�o�b�t�@������W���l������
		for( int j = 0; j < 3; j++ )
		{
			lpInfoArray = reinterpret_cast<float*>( &lpBufferInfo[ Stride * lpIndex[ i * 3 + j ] ] );
			
			VPos[ j ] = D3DXVECTOR3( lpInfoArray[ 0 ], lpInfoArray[ 1 ], lpInfoArray[ 2 ] );
		}
		
		// ���_�o�b�t�@�A�����b�N
		lpVBuffer->Unlock();
	
		// �C���f�b�N�X�o�b�t�@�A�����b�N
		m_lpMesh->UnlockIndexBuffer();
		
		// ���v�Z���@�͓������_�����2�����̃x�N�g��������ɓ����
		// �ǂ̒��_����ł��v�Z���Ă�OK
		// �O�ς����߂ē��e��ۑ�
		D3DXVec3Cross( 
			&m_lpNormals[ i ],
			&(VPos[ 1 ] - VPos[ 0 ]),
			&(VPos[ 2 ] - VPos[ 0 ]) );

		SAFE_RELEASE( lpVBuffer );
	}
}