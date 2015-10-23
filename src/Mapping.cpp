#include "SequenceBase.h"
#include "Main.h"
#include "Mapping.h"

using namespace Sequence;

// コンストラクタ
CMapping::CMapping()
{
	// ティーポット作成
	LPD3DXMESH	lpMesh;
	D3DXCreateTeapot( D3D::GetDevice(), &lpMesh, NULL );

	// クローン作製（頂点情報いじくるため）
	lpMesh->CloneMeshFVF( lpMesh->GetOptions(), D3DFVF_XYZ | D3DFVF_NORMAL, D3D::GetDevice(), &m_lpTeapot);
	SAFE_RELEASE( lpMesh );

	// 法線を作る
	D3DXComputeNormals( m_lpTeapot, NULL );

	// テクスチャ読み込み
	D3DXCreateTextureFromFileEx(
		D3D::GetDevice(),
		"Ground.bmp",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		NULL,
		NULL,
		NULL,
		&m_lpBackTex);
	
	// いたポリ作成
	m_lpBoard = new CBoard();

	m_lpBoard->SetVertex( 100 );
}

// 本体
bool CMapping::RunProc()
{
	Update();

	Draw();

	return true;
}

// 更新
void CMapping::Update()
{

}

// 描画
void CMapping::Draw()
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	D3D::Begin();

	//D3D::SetProj();
	//D3D::SetCamera();

	lpDevice->SetTexture( 0, m_lpBackTex );
	lpDevice->SetFVF( CBoard::FVF_VERTEX );
	m_lpBoard->Draw( lpDevice );

	D3D::End();
}

// デストラクタ
CMapping::~CMapping()
{
	SAFE_RELEASE( m_lpTeapot );
	SAFE_RELEASE( m_lpBackTex );
	SAFE_DELETE( m_lpBoard );
}
