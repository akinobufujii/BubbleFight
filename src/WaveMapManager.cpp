//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "WaveMapManager.h"

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CWaveMapManager::CWaveMapManager():
m_lpWaveShader( NULL ),
m_Board( 2 ),
m_IsFirst( true )
{
}

// デストラクタ
CWaveMapManager::~CWaveMapManager()
{
	m_lpWaveShader = NULL;
}

// 作る
bool CWaveMapManager::Create(LPDIRECT3DDEVICE9 lpdevice, UINT size)
{
	// シェーダデバイス獲得
	m_lpWaveShader = CSShaderManager::GetInst()->Load( lpdevice, "WaveMap.fx" );

	// 波マップ
	for( int i = 0; i < 2; i++ )
	{
		m_WaveMap[ i ].CreateRenderTarget( lpdevice, size, size, D3DFMT_G16R16F );
	}

	// 法線マップ
	m_NormalMap.CreateRenderTarget( lpdevice, size, size );

	// エラーチェック
	if( !m_WaveMap[ 0 ].GetTexture()	|| !m_WaveMap[ 1 ].GetTexture() || 
		!m_NormalMap.GetTexture()		|| !m_lpWaveShader )
	{
		return false;
	}

	// 解像度を考慮した値をシェーダに渡す
	m_lpWaveShader->SetValue( "g_TexMove", 
		&D3DXVECTOR2( 1.f / size, 1.f / size ),
		sizeof( D3DXVECTOR2 ) );

	return true;
}

// 更新
const LPDIRECT3DTEXTURE9 CWaveMapManager::Update(LPDIRECT3DDEVICE9 lpdevice)
{
	LPDIRECT3DSURFACE9	lpDepth, lpBack;
	D3DVIEWPORT9		SaveVp, MapVp;

	// 元のレンダーターゲットを保存する
	lpdevice->GetViewport( &SaveVp );
	lpdevice->GetRenderTarget( 0, &lpBack );
	lpdevice->GetDepthStencilSurface( &lpDepth );

	// マップ用にビューポートを設定する
	MapVp.X		= 0;
	MapVp.Y		= 0;
	MapVp.Width = m_NormalMap.GetSurfaceDesc().Width;
	MapVp.Height= m_NormalMap.GetSurfaceDesc().Height;
	MapVp.MinZ	= 0.f;
	MapVp.MaxZ	= 1.f;

	lpdevice->SetViewport( &MapVp );

	// パス1
	// 波マップの更新
	UpdateWaveMap( lpdevice );

	// パス2
	// 法線マップを作る
	UpdateNormalMap( lpdevice );

	// レンダーターゲットを戻す
	lpdevice->SetViewport( &SaveVp );
	lpdevice->SetRenderTarget( 0, lpBack );
	lpdevice->SetDepthStencilSurface( lpDepth );

	// 参照カウンタを減らす
	lpBack->Release();
	lpDepth->Release();

	return m_NormalMap;
}

// 波マップ更新
void CWaveMapManager::UpdateWaveMap(LPDIRECT3DDEVICE9 lpdevice)
{
	// 値を入れる(適当に)
	// 出現させるテクセル座標
	m_lpWaveShader->SetValue( "g_AddWavePos",
		&D3DXVECTOR2( 
		static_cast<float>( rand() % 201 ) / 100,
		static_cast<float>( rand() % 201 ) / 100 ),
		sizeof( D3DXVECTOR2 ) );

	// 生成する波の高さを設定する
	float Temp = static_cast<float>( rand() % 201 + 100 ) / 100;
	m_lpWaveShader->SetValue( "g_AddWaveHeight", &Temp, sizeof( float ) );

	// 深度バッファ使わない
	lpdevice->SetDepthStencilSurface( NULL );

	// レンダーターゲットの仕様上2回描画を行う
	for( int i = 0; i < 2; i++ )
	{
		// レンダーターゲット変更
		lpdevice->SetRenderTarget( 0, m_WaveMap[ i ].GetSurface() );

		// もしほんとに最初なら
		if( m_IsFirst )
		{
			// 適当な絵を描画する
			m_lpWaveShader->SetTexture( "g_Texture", *CSTexManager::GetInst()->GetData( "SurfaceSea.jpg" ) );
			m_IsFirst = false;
		
		}else{

			// テクスチャを流す
			m_lpWaveShader->SetTexture( "g_Texture", m_WaveMap[ (i + 1) % 2 ] );
		}

		// 描画
		m_lpWaveShader->Begin();
		m_lpWaveShader->BeginPass( 0 );

		m_Board.Draw( lpdevice );

		m_lpWaveShader->EndPass();
		m_lpWaveShader->End();
	}
}

// 法線マップ作成
void CWaveMapManager::UpdateNormalMap(LPDIRECT3DDEVICE9 lpdevice)
{
	// レンダーターゲット設定
	lpdevice->SetRenderTarget( 0, m_NormalMap.GetSurface() );

	// 深度バッファ使わない
	lpdevice->SetDepthStencilSurface( NULL );

	// 波マップをシェーダに渡す
	m_lpWaveShader->SetTexture( "g_Texture", m_WaveMap[ 1 ] );

	// 描画
	m_lpWaveShader->Begin();
	m_lpWaveShader->BeginPass( 1 );

	m_Board.Draw( lpdevice );

	m_lpWaveShader->EndPass();
	m_lpWaveShader->End();
}