//////////////////////////////
// インクルード
//////////////////////////////
#include "Xfile.h"

//////////////////////////////
// 静的メンバ実体化
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
// 実装
//////////////////////////////

// コンストラクタ
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
	// 何もしない
}

// コピーコンストラクタ
CXfile::CXfile(const CXfile &object)
{
	//// そのままコピーできるものをコピーする
	//m_MatNum	= object.m_MatNum;
	//m_CenterPos	= object.m_CenterPos;
	//m_Radius	= object.m_Radius;

	//// そのままコピー出来ないもの
	//// 動的配列確保
	//m_lpMatInfo	= LPMATERIALARRAY( NEW D3DMATERIAL9[ m_MatNum ] );
	//m_lppTex	= NEW LPDIRECT3DTEXTURE9[ m_MatNum ];

	//// コピー
	//for( UINT i = 0; i < m_MatNum; i++ )
	//{
	//	m_lpMatInfo[ i ]	= object.m_lpMatInfo[ i ];
	//	//m_lppTex[ i ]		= object.m_lppTex[ i ]->
	//}
}

// コンストラクタ(ロード付き)
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
	// メンバ変数初期化
	CXfile();

	Load( lpD3DDevice, name );
}

// デストラクタ
CXfile::~CXfile()
{
	// リリース
	if( m_lpMesh )
	{
		SAFE_RELEASE( m_lpDec );
		SAFE_RELEASE( m_lpMesh );
	}
}


// 読み込み
bool CXfile::Load(const LPDIRECT3DDEVICE9 lpD3DDevice, const LPCSTR name)
{
	LPD3DXBUFFER lpBuffer;

	// 何か読み込まれてたら
	if( m_lpMesh )
	{
		this->~CXfile();
	}

	// ファイル読み込み
	D3DXLoadMeshFromX(
		name,		// ファイル名
		D3DXMESH_MANAGED,	// 読み込みオプション
		lpD3DDevice,		// デバイス 
		NULL,				//  
		&lpBuffer,			// バッファ
		NULL,
		&m_MatNum,			// マテリアル数
		&m_lpMesh );		// メッシュ

	// マテリアル数分配列確保
	m_lpMatInfo		= LPMATERIALARRAY( NEW D3DMATERIAL9[ m_MatNum ] );
	m_lpNowMatInfo	= LPMATERIALARRAY( NEW D3DMATERIAL9[ m_MatNum ] );
	m_lppTex		= LPTEXTUREARRAY( NEW LPDIRECT3DTEXTURE9[ m_MatNum ] );

	D3DXMATERIAL*	lpMat =
		reinterpret_cast<D3DXMATERIAL*>( lpBuffer->GetBufferPointer() );

	// 情報を格納
	for( DWORD	i = 0;	i < m_MatNum; i++ )
	{
		m_lpMatInfo[ i ] = lpMat[ i ].MatD3D;
		m_lpMatInfo[ i ].Ambient= m_lpMatInfo[ i ].Diffuse;

		// テクスチャ
		m_lppTex[ i ] = CSMeshTexManager::GetInst()->Load(
			lpD3DDevice, lpMat[ i ].pTextureFilename );
		
		// コピー
		m_lpNowMatInfo[ i ] = m_lpMatInfo[ i ];
	}

	// バッファ解放
	SAFE_RELEASE( lpBuffer );

	// 円衝突判定用に中心と半径を算出
	LPDIRECT3DVERTEXBUFFER9 lpVB = NULL;
	LPVOID lpVertexs;

	// 頂点バッファロック
	m_lpMesh->GetVertexBuffer( &lpVB );
	lpVB->Lock( 0, 0, &lpVertexs, 0 );

	// 中心と半径を算出
	D3DXComputeBoundingSphere(
		reinterpret_cast<D3DXVECTOR3*>(lpVertexs),	// 頂点の最初の位置
		m_lpMesh->GetNumVertices(),					// 頂点数
		D3DXGetFVFVertexSize( m_lpMesh->GetFVF() ),	// 頂点フォーマット数
		&m_CenterPos,								// 中心位置格納
		&m_Radius );								// 半径格納

	// 頂点バッファアンロック
	lpVB->Unlock();

	SAFE_RELEASE( lpVB );

	// 最適化
	LPD3DXMESH	lpTemp;	// メッシュのテンポラリ

	// マスターデータからクローンを作る
	m_lpMesh->CloneMesh( m_lpMesh->GetOptions(), MESH_ELEMENT, lpD3DDevice, &lpTemp );

	// 接戦を追加する
	D3DXComputeTangent( lpTemp, 0, 0, D3DX_DEFAULT, TRUE, NULL );

	// この先はテンポラリの内容をマスターデータとして扱う
	// ※メッシュだけ内容を変更する
	m_lpMesh->Release();
	m_lpMesh = lpTemp;

	return true;
}

// 描画
void CXfile::Draw(LPDIRECT3DDEVICE9 lpD3DDevice) const
{
	if( m_IsShaderDraw && m_lpEffect && m_lphWorld )
	{
		for( DWORD i = 0; i< m_MatNum; i++ )
		{
			D3DXMATRIX mWorld;

			// デバイスから行列獲得
			D3D::GetDevice()->GetTransform( D3DTS_WORLD, &mWorld );

			m_lpEffect->SetValue( m_lphWorld, &mWorld, sizeof(D3DXMATRIX) );

			// テクスチャハンドルがあれば設定
			if( m_lphTex )
			{
				m_lpEffect->SetTexture( m_lphTex, m_lppTex[ i ] );
			}

			m_lpMesh->DrawSubset( i );							// 描画
		}

	}else{
	
		for( DWORD i = 0; i < m_MatNum; i++ )
		{
			lpD3DDevice->SetMaterial( &m_lpNowMatInfo[ i ] );	// 材質設定
			lpD3DDevice->SetTexture( 0, m_lppTex[ i ]);			// テクスチャ張る
			m_lpMesh->DrawSubset( i );							// 描画
		}
	}
}

// シェーダー描画
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

// 法線情報を作る
void CXfile::CreateNormals()
{
	// もう何かあれば作らない
	if( m_lpNormals )
	{
		return;
	}

	D3DXVECTOR3				VPos[ 3 ];								// 頂点格納用テンポラリ
	LPWORD					lpIndex		= NULL;						// インデックス格納用
	DWORD					FaceNum		= m_lpMesh->GetNumFaces();	// ポリゴン数
	LPDIRECT3DVERTEXBUFFER9	lpVBuffer	= NULL;						// 頂点バッファ
	LPBYTE					lpBufferInfo= NULL;						// バッファ情報
	float*					lpInfoArray	= NULL;						// 情報(配列のように使用)
	DWORD					Stride		= m_lpMesh->GetNumBytesPerVertex();	// 頂点情報バイト数

	// 動的配列確保
	m_lpNormals = LPVECTOR3ARRAY( NEW D3DXVECTOR3[ FaceNum ] );

	// 法線計算開始
	for( DWORD i = 0; i < FaceNum; i++ )
	{
		// インデックスバッファロック
		m_lpMesh->LockIndexBuffer( 0, reinterpret_cast<LPVOID*>( &lpIndex ) );

		// 頂点バッファロック
		m_lpMesh->GetVertexBuffer( &lpVBuffer );
		lpVBuffer->Lock( 0, 0, reinterpret_cast<LPVOID*>( &lpBufferInfo ), 0 );
		
		// 法線バッファあるからそれを使えばいいと思うが
		// ライティングの処理関係で信用出来ない情報
		// ポリゴンに対して垂直な法線を手に入れる
		// 頂点バッファから座標を獲得する
		for( int j = 0; j < 3; j++ )
		{
			lpInfoArray = reinterpret_cast<float*>( &lpBufferInfo[ Stride * lpIndex[ i * 3 + j ] ] );
			
			VPos[ j ] = D3DXVECTOR3( lpInfoArray[ 0 ], lpInfoArray[ 1 ], lpInfoArray[ 2 ] );
		}
		
		// 頂点バッファアンロック
		lpVBuffer->Unlock();
	
		// インデックスバッファアンロック
		m_lpMesh->UnlockIndexBuffer();
		
		// ※計算方法は同じ頂点からの2方向のベクトルさえ手に入れば
		// どの頂点からでも計算してもOK
		// 外積を求めて内容を保存
		D3DXVec3Cross( 
			&m_lpNormals[ i ],
			&(VPos[ 1 ] - VPos[ 0 ]),
			&(VPos[ 2 ] - VPos[ 0 ]) );

		SAFE_RELEASE( lpVBuffer );
	}
}