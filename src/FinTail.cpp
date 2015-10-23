/////////////////////////////////
// インクルード
/////////////////////////////////
#include "BubbleManager.h"

#include "FinTail.h"

/////////////////////////////////
// 静的メンバ実体化
/////////////////////////////////
const BYTE	CFinTail::FLG_CNT_PLUS	= 0x01;

const BYTE			CFinTail::MOVE_MAX			= 30;
const BYTE			CFinTail::SPEED_RATE		= 12;
const float			CFinTail::DEFAULT_MOVE_SPEED= 0.05f;
const D3DXVECTOR3	CFinTail::DEFAULT_FARPOS	= D3DXVECTOR3( 0, 0, -0.8f );
const LPSTR			CFinTail::MESHPASS_FIN		= "Fin.x";

/////////////////////////////////
// 実装
/////////////////////////////////

// コンストラクタ
CFinTail::CFinTail():
m_lpMesh( CSXfileManager::GetInst()->GetData( MESHPASS_FIN ) ),
m_MoveCnt( 0.f ),
m_Flg( 0 ),
m_MoveType( MOVE_SIDE ),
m_FarPos( DEFAULT_FARPOS ),
m_NowRotMax( MOVE_MAX ),
m_IsCreateBubble( true )
{
}

// デストラクタ
CFinTail::~CFinTail()
{
}

// 更新
void CFinTail::Update( const D3DXMATRIX& mat, float speed )
{
	CMatrix RotY, Trans;

	// 動いていれば
	if( speed > 0.2f )
	{
		if( m_IsCreateBubble == false )
		{
			m_IsCreateBubble = true;
		
		}else{

			// 回転
			if( m_MoveType == MOVE_SIDE )
			{
				// 泡の出す位置を決める
				CMatrix mTemp = m_mMat;

				mTemp.MoveLocal( 0, -m_lpMesh->GetRadius() / 2, -m_lpMesh->GetRadius() / 2 );

				m_Bubbles.Add( &mTemp, 1 );

				mTemp.MoveLocal( 0, m_lpMesh->GetRadius(), 0 );

				m_Bubbles.Add( &mTemp, 1 );

			}else if( m_MoveType == MOVE_TOP ){

				// 泡の出す位置を決める
				CMatrix mTemp = m_mMat;

				mTemp.MoveLocal( -m_lpMesh->GetRadius() / 2, 0, -m_lpMesh->GetRadius() / 2 );

				m_Bubbles.Add( &mTemp, 1 );

				mTemp.MoveLocal( m_lpMesh->GetRadius(), 0, 0 );

				m_Bubbles.Add( &mTemp, 1 );
			}

			m_IsCreateBubble = false;
		}
	}

	// 親より離れる
	Trans.SetTrans( &m_FarPos );

	speed += DEFAULT_MOVE_SPEED;

	// フラグで加算か減算か決める
	if( m_Flg & FLG_CNT_PLUS )
	{
		if( (m_MoveCnt += abs( speed ) * SPEED_RATE) > m_NowRotMax )
		{
			m_Flg ^= FLG_CNT_PLUS;
		}
	
	}else{

		if( (m_MoveCnt -= abs( speed ) * SPEED_RATE) < -m_NowRotMax )
		{
			m_Flg ^= FLG_CNT_PLUS;
		}
	}

	// 回転
	if( m_MoveType == MOVE_SIDE )
	{
		RotY.RotateLocalY( m_MoveCnt );

	}else if( m_MoveType == MOVE_TOP )
	{
		RotY.RotateLocalX( m_MoveCnt );
	}
	
	m_mMat = Trans * RotY * mat;

	m_Bubbles.Update();
}

// 描画
void CFinTail::Draw( const LPDIRECT3DDEVICE9& lpdevice )
{
	if( IsCulling( &m_mMat ) )
	{
		lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
	
		m_lpMesh->Draw( lpdevice );
	}

	m_Bubbles.DrawNoBreak( lpdevice );
}

// 描画
void CFinTail::DrawShader( const LPDIRECT3DDEVICE9& lpdevice )
{
	if( IsCulling( &m_mMat ) )
	{
		CEffect* lpSharder = CSShaderManager::GetInst()->GetData( "Phong.fx" );

		// 固定パイプラインの情報を獲得する
		CMatrix	mView, mProj;
		lpdevice->GetTransform( D3DTS_PROJECTION, &mProj );
		lpdevice->GetTransform( D3DTS_VIEW, &mView );

		// 座標変換
		lpSharder->SetValue( "g_WVP", 
			&(m_mMat * mView * mProj),
			sizeof( D3DXMATRIX ) );

		// ライト位置計算
		lpSharder->SetValue( "g_LightDir",
			&GetLightPos( lpdevice, m_mMat ),
			sizeof( D3DXVECTOR4 ) );

		// カメラ位置計算
		lpSharder->SetValue( "g_EyePos",
			&GetViewPos( lpdevice, m_mMat ),
			sizeof( D3DXVECTOR4 ) );

		// 描画
		lpSharder->Begin();

		m_lpMesh->DrawShader( lpSharder, "g_Diffuse", "g_Tex" );

		lpSharder->End();
	}

	m_Bubbles.DrawNoBreak( lpdevice );
}