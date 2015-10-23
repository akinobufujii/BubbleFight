////////////////////////////////
// �C���N���[�h
////////////////////////////////
#include "StdAfx.h"
#include <map>
#include "Effect.h"

////////////////////////////////
// ����
////////////////////////////////

// �R���X�g���N�^
CEffect::CEffect():
m_lpEffect( NULL ),
m_TecKey( NULL )
{
	// �Ȃ�
}

CEffect::CEffect(LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, LPCSTR technique)
{
	CEffect();

	Load( lpdevice, file_name, technique );
}

// �ǂݍ���
bool CEffect::Load(LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, LPCSTR technique )
{
	// �t�@�C������ǂݍ���
	LPD3DXBUFFER	lpError = NULL;

	char Str[ 0xff ];

	sprintf_s( Str, sizeof( Str ), "%s�G�t�F�N�g�t�@�C���ǂݍ��݊J�n", file_name );
	DebugMessageBox( Str );

	HRESULT hr = D3DXCreateEffectFromFile(
		lpdevice,
		file_name,
		NULL,
		NULL,
		NULL,
		NULL,
		&m_lpEffect,
		&lpError );

	DebugMessageBox( "�֐��N���A" );

	//D3DERR_INVALIDCALL;
	//E_OUTOFMEMORY;

	// ������
	if( SUCCEEDED( hr ) )
	{
		//sprintf_s( Str, sizeof( Str ), "D3DERR_INVALIDCALL = %d%nE_OUTOFMEMORY = %d", D3DERR_INVALIDCALL, E_OUTOFMEMORY );

		DebugMessageBox( Str );
	
		// �e�N�j�b�N�����͓o�^���Ă���
		m_TecKey = technique;
		m_HandleMap.insert( std::pair< LPCSTR, D3DXHANDLE >( technique, m_lpEffect->GetTechniqueByName( technique ) ) );

		DebugMessageBox( "��������" );

	}else{	// ���s��

		sprintf_s( Str, sizeof( Str ), "���s\nhr = %d\nlpError = %d", hr, lpError );

		GetLastError();
		DebugMessageBox( Str );

		MessageBox( NULL, static_cast<LPCSTR>( lpError->GetBufferPointer() ), "ShaderError", MB_OK );

		exit( 0 );
	}

	DebugMessageBox( "�G���[�\�����" );
	SAFE_RELEASE( lpError );
	DebugMessageBox( "�������\n�ȏ�Ȃ�" );
	return true;
}

// �f�X�g���N�^
CEffect::~CEffect()
{
	SAFE_RELEASE( m_lpEffect );
}

// �`��J�n
const LPD3DXEFFECT CEffect::Begin( LPCSTR technique )
{
	// �e�N�j�b�N�ɂȂɂ��w�肪�����
	if( technique )
	{
		m_lpEffect->SetTechnique( CheckHandle( technique ) );
	
	}else{	// �w�肪�Ȃ����

		m_lpEffect->SetTechnique( CheckHandle( m_TecKey ) );
	}

	m_lpEffect->Begin( NULL, 0 );

	return m_lpEffect;
}

// �`��I��
void CEffect::End()
{
	m_lpEffect->End();
}

// �p�X�ݒ�(�ǂݍ��݂̎��ɐݒ肵���e�N�j�b�N��ݒ�)
void CEffect::BeginPass( int pass )
{
	m_lpEffect->BeginPass( pass );
}

// �`��I��
void CEffect::EndPass()
{
	m_lpEffect->EndPass();
}

// �n���h�������
bool CEffect::CreateHandle( LPCSTR handle_name )
{
	m_HandleMap.insert( std::pair< LPCSTR, D3DXHANDLE >( handle_name, m_lpEffect->GetParameterByName( NULL, handle_name ) ) );

	return true;
}

// �n���h�����l��
D3DXHANDLE CEffect::GetHandle( LPCSTR handle_name )
{
	return CheckHandle( handle_name );
}

// �l��ݒ肷��
void CEffect::SetValue( LPCSTR handle, LPCVOID lp_value, UINT byte )
{
	m_lpEffect->SetValue( CheckHandle( handle ), lp_value, byte );

	m_lpEffect->CommitChanges();
}

// �n���h���`�F�b�N
D3DXHANDLE CEffect::CheckHandle( LPCSTR handle_name )
{
	// �n���h���m�F
	std::map< LPCSTR, D3DXHANDLE >::iterator	It = m_HandleMap.find( handle_name );

	// �Ȃ���΍��
	if( It == m_HandleMap.end() )
	{
		CreateHandle( handle_name );

		It = m_HandleMap.find( handle_name );
	}

	return It->second;
}

// �e�N�X�`���ݒ�
void CEffect::SetTexture( LPCSTR handle, LPDIRECT3DBASETEXTURE9 lp_tex )
{
	m_lpEffect->SetTexture( CheckHandle( handle ), lp_tex );

	m_lpEffect->CommitChanges();
}