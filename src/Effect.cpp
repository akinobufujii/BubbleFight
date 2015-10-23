////////////////////////////////
// インクルード
////////////////////////////////
#include "StdAfx.h"
#include <map>
#include "Effect.h"

////////////////////////////////
// 実装
////////////////////////////////

// コンストラクタ
CEffect::CEffect():
m_lpEffect( NULL ),
m_TecKey( NULL )
{
	// なし
}

CEffect::CEffect(LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, LPCSTR technique)
{
	CEffect();

	Load( lpdevice, file_name, technique );
}

// 読み込み
bool CEffect::Load(LPDIRECT3DDEVICE9 lpdevice, LPCSTR file_name, LPCSTR technique )
{
	// ファイルから読み込み
	LPD3DXBUFFER	lpError = NULL;

	char Str[ 0xff ];

	sprintf_s( Str, sizeof( Str ), "%sエフェクトファイル読み込み開始", file_name );
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

	DebugMessageBox( "関数クリア" );

	//D3DERR_INVALIDCALL;
	//E_OUTOFMEMORY;

	// 成功時
	if( SUCCEEDED( hr ) )
	{
		//sprintf_s( Str, sizeof( Str ), "D3DERR_INVALIDCALL = %d%nE_OUTOFMEMORY = %d", D3DERR_INVALIDCALL, E_OUTOFMEMORY );

		DebugMessageBox( Str );
	
		// テクニックだけは登録しておく
		m_TecKey = technique;
		m_HandleMap.insert( std::pair< LPCSTR, D3DXHANDLE >( technique, m_lpEffect->GetTechniqueByName( technique ) ) );

		DebugMessageBox( "無事成功" );

	}else{	// 失敗時

		sprintf_s( Str, sizeof( Str ), "失敗\nhr = %d\nlpError = %d", hr, lpError );

		GetLastError();
		DebugMessageBox( Str );

		MessageBox( NULL, static_cast<LPCSTR>( lpError->GetBufferPointer() ), "ShaderError", MB_OK );

		exit( 0 );
	}

	DebugMessageBox( "エラー表示解放" );
	SAFE_RELEASE( lpError );
	DebugMessageBox( "解放完了\n以上なし" );
	return true;
}

// デストラクタ
CEffect::~CEffect()
{
	SAFE_RELEASE( m_lpEffect );
}

// 描画開始
const LPD3DXEFFECT CEffect::Begin( LPCSTR technique )
{
	// テクニックになにか指定があれば
	if( technique )
	{
		m_lpEffect->SetTechnique( CheckHandle( technique ) );
	
	}else{	// 指定がなければ

		m_lpEffect->SetTechnique( CheckHandle( m_TecKey ) );
	}

	m_lpEffect->Begin( NULL, 0 );

	return m_lpEffect;
}

// 描画終了
void CEffect::End()
{
	m_lpEffect->End();
}

// パス設定(読み込みの時に設定したテクニックを設定)
void CEffect::BeginPass( int pass )
{
	m_lpEffect->BeginPass( pass );
}

// 描画終了
void CEffect::EndPass()
{
	m_lpEffect->EndPass();
}

// ハンドルを作る
bool CEffect::CreateHandle( LPCSTR handle_name )
{
	m_HandleMap.insert( std::pair< LPCSTR, D3DXHANDLE >( handle_name, m_lpEffect->GetParameterByName( NULL, handle_name ) ) );

	return true;
}

// ハンドルを獲得
D3DXHANDLE CEffect::GetHandle( LPCSTR handle_name )
{
	return CheckHandle( handle_name );
}

// 値を設定する
void CEffect::SetValue( LPCSTR handle, LPCVOID lp_value, UINT byte )
{
	m_lpEffect->SetValue( CheckHandle( handle ), lp_value, byte );

	m_lpEffect->CommitChanges();
}

// ハンドルチェック
D3DXHANDLE CEffect::CheckHandle( LPCSTR handle_name )
{
	// ハンドル確認
	std::map< LPCSTR, D3DXHANDLE >::iterator	It = m_HandleMap.find( handle_name );

	// なければ作る
	if( It == m_HandleMap.end() )
	{
		CreateHandle( handle_name );

		It = m_HandleMap.find( handle_name );
	}

	return It->second;
}

// テクスチャ設定
void CEffect::SetTexture( LPCSTR handle, LPDIRECT3DBASETEXTURE9 lp_tex )
{
	m_lpEffect->SetTexture( CheckHandle( handle ), lp_tex );

	m_lpEffect->CommitChanges();
}