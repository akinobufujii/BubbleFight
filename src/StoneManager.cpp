//////////////////////////////
// インクルード
//////////////////////////////
#include "StoneManager.h"

#include "FishManager.h"
#include "LimitWalls.h"
#include "UI.h"
#include "EnemyManager.h"
#include "PlanktonManager.h"
#include "GameMain.h"

//////////////////////////////
// 静的メンバ実体化
//////////////////////////////
const UINT	CStoneManager::RENDERTARGET_SIZE	= 650;		// 何故かこれより下げると表示されなくなる
const DWORD	CStoneManager::CLEAR_TARGET_TIME	= 60000;

//////////////////////////////
// 実装
//////////////////////////////

// コンストラクタ
CStoneManager::CStoneManager()
{
	// シェーダ読み込み
	m_lpCausticShader = CSShaderManager::GetInst()->Load( 
		D3D::GetDevice(), "Caustic.fx" );
	m_lpWaveShader = CSShaderManager::GetInst()->Load( 
		D3D::GetDevice(), "Wave.fx" );

	// レンダーターゲット作成
	m_WaveTex.CreateRenderTarget( D3D::GetDevice(),
		RENDERTARGET_SIZE, RENDERTARGET_SIZE );

	m_BumpTex.CreateRenderTarget( D3D::GetDevice(),
		RENDERTARGET_SIZE, RENDERTARGET_SIZE );

	m_CausticTex.CreateRenderTarget( D3D::GetDevice(),
		RENDERTARGET_SIZE, RENDERTARGET_SIZE );

	m_SaveClearTime = timeGetTime();
}

// デストラクタ
CStoneManager::~CStoneManager()
{
	// 解放
	Release();
}

// ファイルから読み込み
bool CStoneManager::LoadFromData(const LPDIRECT3DDEVICE9 lp_device, LPCSTR path)
{
	// 指定されたファイルを開く
	std::ifstream In( path );

	if( In.is_open() )
	{
		STONEDATA*	lpTemp = NULL;	// テンポラリ
		std::string	Str;

		// 最後まで読み込み
		while( In.eof() == false )
		{
			lpTemp = NEW STONEDATA();

			// 位置情報取得
			In >> lpTemp->Pos.x;
			In >> lpTemp->Pos.z;
			lpTemp->Pos.y = Sequence::CGameMain::BOTTOMSEA_HEIGHT;

			// 拡大率取得
			In >> lpTemp->Scale;

			// メッシュパス取得
			In >> Str;

			// 読み込み
			CSXfileManager::GetInst()->Load( lp_device, Str.c_str(), true );

			// メッシュ獲得
			lpTemp->lpMesh = CSXfileManager::GetInst()->GetData( Str.c_str() );

			// リストに登録
			m_StoneList.push_back( lpTemp );
		}

	}else{

		// ファイルオープン失敗
		return false;
	}

	return true;
}

// 描画
void CStoneManager::Draw(const LPDIRECT3DDEVICE9 lp_device)
{
	lp_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	lp_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	lp_device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lp_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	lp_device->SetRenderState( D3DRS_LIGHTING, FALSE );

	BOOST_FOREACH( STONEDATA* It, m_StoneList )
	{
		CMatrix mMat( It->Pos );

		mMat.ScaleLocal( It->Scale );

		lp_device->SetTransform( D3DTS_WORLD, &mMat );

		It->lpMesh->Draw( lp_device );
	}
}

// シェーダー描画
void CStoneManager::DrawShader( const LPDIRECT3DDEVICE9 lp_device )
{
	// コースティックマップ作成
	CreateCausticMap( lp_device );

	// 行列とライト方向獲得
	D3DXMATRIX	mView, mProj, mInv;

	lp_device->GetTransform( D3DTS_PROJECTION, &mProj );
	lp_device->GetTransform( D3DTS_VIEW, &mView );

	// シェーダーに流す
	m_lpCausticShader->SetValue( "g_View", &mView, sizeof(D3DXMATRIX) );
	m_lpCausticShader->SetValue( "g_Proj", &mProj, sizeof(D3DXMATRIX) );
	m_lpCausticShader->SetTexture( "g_CausticTex", m_CausticTex.GetTexture() );

	m_lpCausticShader->Begin();
	//m_lpCausticShader->BeginPass( 1 );

	BOOST_FOREACH( STONEDATA* It, m_StoneList )
	{
		CMatrix mMat( It->Pos );

		mMat.ScaleLocal( It->Scale );

		m_lpCausticShader->SetValue( "g_LightDir", &GetLightPos( lp_device, mMat ), sizeof(D3DVECTOR) );
		m_lpCausticShader->SetValue( "g_World", &mMat, sizeof(D3DXMATRIX) );

		It->lpMesh->DrawShader( m_lpCausticShader, NULL, "g_Texture", 1, 1 );
	}

	//m_lpCausticShader->EndPass();
	m_lpCausticShader->End();
}

// 解放
void CStoneManager::Release()
{
	BOOST_FOREACH( STONEDATA* It, m_StoneList )
	{
		SAFE_DELETE( It );
	}
	
	m_StoneList.clear();
}

// コースティックマップ作成
void CStoneManager::CreateCausticMap(const LPDIRECT3DDEVICE9 lp_device)
{
	LPDIRECT3DSURFACE9 lpBackBuffer, lpZBuffer;	// サーフェス保存用
	D3DVIEWPORT9 SaveVp, Vp = { 0, 0, RENDERTARGET_SIZE, RENDERTARGET_SIZE, 0, 1.f };

	// バックバッファを保存する
	lp_device->GetRenderTarget( 0, &lpBackBuffer );
	lp_device->GetDepthStencilSurface( &lpZBuffer );
	lp_device->GetViewport( &SaveVp );

	m_lpWaveShader->Begin();

	// 波マップ作成
	// レンダーターゲットを変える
	// ※テクスチャとレンダーターゲットが一緒なので
	//   処理的にまずい
	m_lpWaveShader->BeginPass( 0 );

	lp_device->SetViewport( &Vp );
	lp_device->SetRenderTarget( 0, m_WaveTex.GetSurface() );
	lp_device->SetDepthStencilSurface( m_WaveTex.GetZbuffer() );
	m_lpWaveShader->SetTexture( "g_Texture", m_WaveTex.GetTexture() );

	// もし一定時間たったなら
	if( timeGetTime() - m_SaveClearTime > CLEAR_TARGET_TIME )
	{
		// レンダーターゲット初期化
		lp_device->Clear(
			0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB( 0, 0, 0 ), 1.f, 0  );

		m_SaveClearTime = timeGetTime();
	}
	
	m_lpWaveShader->SetValue( 
		"g_AddWavePos",
		&D3DXVECTOR2( rand() % 100 * 0.01f, rand() % 100 * 0.01f ),
		sizeof( D3DXVECTOR2 ) );

	m_Borad.SetVertex( 2 );
	m_Borad.Draw( lp_device );

	m_lpWaveShader->EndPass();

	// 法線マップ作成
	// レンダーターゲットを変える
	lp_device->SetRenderTarget( 0, m_BumpTex.GetSurface() );
	lp_device->SetDepthStencilSurface( m_BumpTex.GetZbuffer() );
	m_lpWaveShader->SetTexture( "g_Texture", m_WaveTex.GetTexture() );

	m_lpWaveShader->BeginPass( 1 );

	m_Borad.SetVertex( 2 );
	m_Borad.Draw( lp_device );

	m_lpWaveShader->EndPass();
	m_lpWaveShader->End();

	// コースティックマップ作成
	m_lpCausticShader->Begin();
	m_lpCausticShader->BeginPass( 0 );

	// レンダーターゲットを変える
	lp_device->SetRenderTarget( 0, m_CausticTex.GetSurface() );
	lp_device->SetDepthStencilSurface( NULL );
	m_lpCausticShader->SetTexture( "g_NormalTex", m_BumpTex.GetTexture() );

	m_Borad.SetVertex( 2 );
	m_Borad.Draw( lp_device );

	m_lpCausticShader->EndPass();
	m_lpCausticShader->End();

	// レンダーターゲットを戻す
	lp_device->SetRenderTarget( 0, lpBackBuffer );
	lp_device->SetDepthStencilSurface( lpZBuffer );
	lp_device->SetViewport( &SaveVp );

	SAFE_RELEASE( lpBackBuffer );
	SAFE_RELEASE( lpZBuffer );
}