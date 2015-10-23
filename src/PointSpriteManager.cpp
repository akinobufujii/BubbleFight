///////////////////////////////
// インクルード
///////////////////////////////
#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "UI.h"
#include "PlanktonManager.h"
#include "EnemyManager.h"
#include "GameMain.h"

#include "PointSpriteManager.h"

///////////////////////////////
// 静的メンバ実体化
///////////////////////////////
const DWORD CPointSpriteManager::CUSTOM_FVF		= (D3DFVF_DIFFUSE | D3DFVF_PSIZE | D3DFVF_XYZ);
const int	CPointSpriteManager::BYTE_SIZE		= sizeof(POINTSPRITE);
const float	CPointSpriteManager::DEFAULT_MAXSIZE= 2.f;
const float CPointSpriteManager::GRAVITY_SPEED	= 0.1f;

///////////////////////////////
// 実装
///////////////////////////////

// コンストラクタ
CPointSpriteManager::CPointSpriteManager():
m_lpTex( NULL ),
m_ScaleMax( DEFAULT_MAXSIZE ),
m_EraseTime( 0xffffffff ),
m_SaveTime( 0 ),
m_Num( 0 ),
m_UpdateNum( 0 ),
m_NowType( TYPE_POP ),
m_NowGravity( 0 ),
m_fpUpdateFunc( &CPointSpriteManager::UpdatePop ),
m_lpShader( NULL )
{
}

// 更新
void CPointSpriteManager::Update()
{
	if( m_fpUpdateFunc )
	{
		(this->*m_fpUpdateFunc)();

	}else{

		return;
	}	
}

// 描画
void CPointSpriteManager::Draw(const LPDIRECT3DDEVICE9& lpdevice)
{
	if( m_Num == 0 )
	{
		return;
	}

	// デバイス獲得
	if( m_lpShader == NULL )
	{
		m_lpShader = CSShaderManager::GetInst()->Load( lpdevice, "PointSprite.fx" );
	}

	// テクスチャ設定
	m_lpShader->SetTexture( "g_Texture", m_lpTex );

	// 距離設定
	m_lpShader->SetValue( "g_PointC", &m_ScaleMax, sizeof( m_ScaleMax ) );

	// 固定パイプラインから行列を取得
	D3DXMATRIX mView, mProj;
	lpdevice->GetTransform( D3DTS_VIEW, &mView );
	lpdevice->GetTransform( D3DTS_PROJECTION, &mProj );

	// 行列をシェーダに流す
	m_lpShader->SetValue( "g_VP", &(mView * mProj), sizeof( D3DXMATRIX ) );
	
	// 描画
	m_lpShader->Begin();
	m_lpShader->BeginPass( 0 );

	lpdevice->SetFVF( CUSTOM_FVF );
	lpdevice->DrawPrimitiveUP( D3DPT_POINTLIST, m_Num, &m_Sprite.front(), BYTE_SIZE );
	m_lpShader->EndPass();
	m_lpShader->End();
}

// 消える時間設定
void CPointSpriteManager::SetEraseTime( const DWORD time )
{
	m_EraseTime = time;

	switch( m_NowType )
	{
	case TYPE_POP:

		m_AlphaCnt = 255.f / time;

		m_SaveTime = timeGetTime();

		m_EndTime = time;

		break;

	case TYPE_SPOUT:

		BOOST_FOREACH( DWORD& Time, m_StartTimes)
		{
			Time = timeGetTime();
		}

		break;

	case TYPE_GRAVITYPOP:

		BOOST_FOREACH( DWORD& Time, m_StartTimes)
		{
			Time = timeGetTime();
		}

		break;
	}
}

// パーティクル数設定
void CPointSpriteManager::SetNum( const int num )
{
	// 最大数更新
	m_Num = num;

	m_UpdateNum = 0;

	// 一度解放
	m_Sprite.clear();
	m_MoveDir.clear();

	// 確保
	m_Sprite.resize( m_Num );
	m_MoveDir.resize( m_Num );

	if( m_NowType == TYPE_SPOUT )
	{
		m_StartTimes.resize( m_Num );
	}
}

// 位置設定
void CPointSpriteManager::SetPos( const D3DXVECTOR3& pos )
{
	m_SavePos = pos;

	BOOST_FOREACH( POINTSPRITE& Sprite, m_Sprite )
	{
		Sprite.Pos = pos;
	}
}

// デストラクタ
CPointSpriteManager::~CPointSpriteManager()
{
	// 解放
	m_StartTimes.clear();
	m_Sprite.clear();
	m_MoveDir.clear();
}

// 弾ける形で更新
void CPointSpriteManager::UpdatePop()
{
	// 時間超えていれば更新しない
	if( timeGetTime() - m_SaveTime > m_EndTime )
	{
		// 念のため透明にしておく
		for( int i = 0; i < m_Num; i++ )
		{
			m_Sprite[ i ].Color = D3DCOLOR_ARGB( 0, 0xff, 0xff, 0xff );
		}

		return;
	}

	DWORD NowTime = timeGetTime() - m_SaveTime;

	// 動かす
	for( int i = 0; i < m_Num; i++ )
	{
		m_MoveDir[ i ] *= 0.95f;	// 減速

		m_MoveDir[ i ].x	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);
		m_MoveDir[ i ].y	+= static_cast<float>( rand() % 10 ) / 100.f * 2;
		m_MoveDir[ i ].z	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);

		m_Sprite[ i ].Pos += m_MoveDir[ i ];

		// 海面から出たら
		if( m_Sprite[ i ].Pos.y > Sequence::CGameMain::SURFACESEA_HEIGHT - 1)
		{
			// とどまる
			m_Sprite[ i ].Pos.y = Sequence::CGameMain::SURFACESEA_HEIGHT - 1;
		}

		m_Sprite[ i ].Color = D3DCOLOR_ARGB( 0xff - static_cast<int>(NowTime * m_AlphaCnt), 0xff, 0xff, 0xff );
	}
}

// 噴水系で更新
void CPointSpriteManager::UpdateSpout()
{
	// 更新する数を徐々に増やす
	if( m_Num < ++m_UpdateNum )
	{
		m_UpdateNum = m_Num;
	
	}else{

		// 開始時間記憶
		m_StartTimes[ m_UpdateNum - 1 ] = timeGetTime();
	}

	DWORD NowTime, MoveTime;// 今の時間、動いた時間	

	// 動かす
	for( int i = 0; i < m_UpdateNum; i++ )
	{
		// 時間格納
		NowTime = timeGetTime();
		MoveTime = NowTime - m_StartTimes[ i ];

		m_MoveDir[ i ] *= 0.95f;	// 減速

		float a = (float)(m_EraseTime - MoveTime) / (float)m_EraseTime;

		m_MoveDir[ i ].x	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);
		m_MoveDir[ i ].y	+= static_cast<float>( rand() % 10 ) / 100.f;
		m_MoveDir[ i ].z	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);

		m_Sprite[ i ].Pos += m_MoveDir[ i ];

		//m_lpSprite[ i ].Scale -= m_ScaleRate;

		m_Sprite[ i ].Color = D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff );

		// 定時を過ぎたらリセット
		if( MoveTime > m_EraseTime )
		{
			m_Sprite[ i ].Pos = m_SavePos;

			m_StartTimes[ i ] = timeGetTime();

			m_MoveDir[ i ].x = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.x - (m_SaveMoveMax.x / 2);
			m_MoveDir[ i ].y = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.y;
			m_MoveDir[ i ].z = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.z - (m_SaveMoveMax.z / 2);
		}
	}
}

// 重力付き弾ける更新
void CPointSpriteManager::UpdateGravityPop()
{
	// 時間超えていれば更新しない
	if( timeGetTime() - m_SaveTime > m_EndTime )
	{
		// 念のため透明にしておく
		for( int i = 0; i < m_Num; i++ )
		{
			m_Sprite[ i ].Color = D3DCOLOR_ARGB( 0, 0xff, 0xff, 0xff );
		}

		return;
	}

	DWORD NowTime = timeGetTime() - m_SaveTime;

	// 動かす
	for( int i = 0; i < m_Num; i++ )
	{
		m_MoveDir[ i ] *= 0.95f;	// 減速

		m_MoveDir[ i ].x	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);
		m_MoveDir[ i ].y	+= static_cast<float>( rand() % 10 ) / 100.f - ( m_NowGravity + GRAVITY_SPEED );
		m_MoveDir[ i ].z	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);

		m_Sprite[ i ].Pos += m_MoveDir[ i ];

		// 海面から出たら
		if( m_Sprite[ i ].Pos.y > Sequence::CGameMain::SURFACESEA_HEIGHT - 1)
		{
			// とどまる
			m_Sprite[ i ].Pos.y = Sequence::CGameMain::SURFACESEA_HEIGHT - 1;
		}

		m_Sprite[ i ].Scale += 0.5f;

		//m_Sprite[ i ].Color = D3DCOLOR_ARGB( 0xff - static_cast<int>(NowTime * m_AlphaCnt), 0xff, 0xff, 0xff );
	}
}

// 更新方法設定
void CPointSpriteManager::SetType( UPDATETYPE type )
{
	m_NowType = type;

	switch( m_NowType )
	{
	case TYPE_POP:
		m_fpUpdateFunc = &CPointSpriteManager::UpdatePop;
		break;

	case TYPE_SPOUT:
		m_fpUpdateFunc = &CPointSpriteManager::UpdateSpout;
		break;

	case TYPE_GRAVITYPOP:
		m_fpUpdateFunc = &CPointSpriteManager::UpdateGravityPop;
		break;
	}
}

// 動き設定
void CPointSpriteManager::SetMove( const D3DXVECTOR3& move_max )
{
	m_SaveMoveMax = move_max;

	BOOST_FOREACH( D3DXVECTOR3& Move, m_MoveDir )
	{
		Move.x = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.x - (m_SaveMoveMax.x / 2);
		Move.y = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.y;
		Move.z = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.z - (m_SaveMoveMax.z / 2);
	}
}

// 消えたかどうか
bool CPointSpriteManager::IsErase()
{
	// 時間超えれば
	if( timeGetTime() - m_SaveTime > m_EndTime )
	{
		return true;
	}

	// 画面内から全て消えてなければまだ
	BOOST_FOREACH( POINTSPRITE& Sprite, m_Sprite )
	{
		if( IsCulling( &CMatrix( Sprite.Pos ) ) )
		{
			return false;
		}
	}

	return true;
}