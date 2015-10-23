//////////////////////////////
// インクルード
//////////////////////////////
#include "RippleManager.h"
#include "BubbleManager.h"
#include "ShotManager.h"
#include "Fintail.h"
#include "StrongDolphin.h"
#include "EnemyBase.h"
#include "EnemyManager.h"

#include "BossBase.h"

#include "FishManager.h"
#include "Clear.h"

#include "UI.h"

//////////////////////////////
// 静的メソッド実体化
//////////////////////////////
const int	CUI::FONT_WIDTH			= 100;
const int	CUI::FONT_HEIGHT		= 40;
const int	CUI::NUMBER_WIDTH		= 40;
const int	CUI::NUMBER_HEIGHT		= 40;
const int	CUI::LIFEGAGE_WIDTH		= 200;
const int	CUI::LIFEGAGE_HEIGHT	= 40;
const int	CUI::POWERGAGE_WIDTH	= 200;
const int	CUI::POWERGAGE_HEIGHT	= 21;
const int	CUI::ENEMYPOSTEX_SIZE	= 8;
const int	CUI::RADAR_SIZE			= 180;
const int	CUI::BLESSGAGE_WIDTH	= 20;
const int	CUI::BLESSGAGE_HEIGHT	= 400;
const float	CUI::SEARCH_RADIUS		= 90.f;

//////////////////////////////
// 実装
//////////////////////////////

// コンストラクタ
CUI::CUI():
m_lpTexManager( CSTexManager::GetInst() ),
m_Score( 0 ),
m_HP( 0 ),
m_Power( 0.f ),
m_Bless( 0 ),
m_Alpha( 0xff ),
m_NowEnemys( 0 ),
m_AllEnemys( 0 )
{
}

// 更新
void CUI::Update(ULONG	score,
				 USHORT hp,
				 float	power,
				 USHORT bless,
				 BYTE	now_enemys,
				 BYTE	all_enemys)
{
	m_Score = score;
	m_HP	= hp;
	m_Power = power;
	m_Bless = bless;
	m_NowEnemys = now_enemys;
	m_AllEnemys	= all_enemys;
}

// 描画
void CUI::Draw( const LPD3DXSPRITE lpsprite )
{
	// 文字
	CSprite::Draw(
		lpsprite,
		m_lpTexManager->GetData( "UI_Font.png" )->GetTexture(),
		0, 10, 0,
		0, 0, FONT_WIDTH, FONT_HEIGHT,
		m_Alpha );

	// スコア
	/*char Str[ sizeof( int ) * 8 ];

	sprintf( Str, "%d", m_Score );

	int i = 0; 

	while( true )
	{
		if( Str[ i ] == '\0' )
		{
			break;
		}

		BYTE Num = Str[ i ] - '0';

		CSprite::Draw(
			lpsprite,
			m_lpTexTexManager->GetData( "number.png" )->GetTexture(),
			FONT_WIDTH + NUMBER_WIDTH * i, 0, 0,
			NUMBER_WIDTH * Num, 0, NUMBER_WIDTH * (Num + 1), NUMBER_HEIGHT );

		i++;
	}*/

	// ライフゲージ
	// 下側
	CSprite::Draw(
		lpsprite,
		m_lpTexManager->GetData( "LifeGage.png" )->GetTexture(),
		FONT_WIDTH, 10, 0,
		0, 0, LIFEGAGE_WIDTH, LIFEGAGE_HEIGHT,
		m_Alpha );

	// 上側
	CSprite::Draw(
		lpsprite,
		m_lpTexManager->GetData( "LifeGage.png" )->GetTexture(),
		FONT_WIDTH, 10, 0,
		0, LIFEGAGE_HEIGHT, 
		(static_cast<float>(LIFEGAGE_WIDTH) / Player::CDolphin::HP_MAX) * m_HP, LIFEGAGE_HEIGHT * 2,
		m_Alpha );

	//// パワーゲージ
	//// 下側
	//CSprite::Draw(
	//	lpsprite,
	//	m_lpTexTexManager->GetData( "PowerGage.png" )->GetTexture(),
	//	FONT_WIDTH, LIFEGAGE_HEIGHT + 10, 0,
	//	0, 0, POWERGAGE_WIDTH, POWERGAGE_HEIGHT,
	//	m_Alpha);

	//// 上側
	//CSprite::Draw(
	//	lpsprite,
	//	m_lpTexTexManager->GetData( "PowerGage.png" )->GetTexture(),
	//	FONT_WIDTH, LIFEGAGE_HEIGHT + 10, 0,
	//	0, POWERGAGE_HEIGHT, 
	//	(static_cast<float>(POWERGAGE_WIDTH) / Player::CDolphin::POWER_MAX) * m_Power, POWERGAGE_HEIGHT * 2,
	//	m_Alpha);

	// 息
	float Temp = (static_cast<float>(BLESSGAGE_HEIGHT) / Player::CDolphin::BLESS_MAX) * (Player::CDolphin::BLESS_MAX - m_Bless);
	
	CSprite::Draw(
		lpsprite,
		m_lpTexManager->GetData( "Bless.png" )->GetTexture(),
		10, WINDOW_HEIGHT - BLESSGAGE_HEIGHT - 10, 0,
		0, 0, 
		BLESSGAGE_WIDTH, BLESSGAGE_HEIGHT,
		m_Alpha );

	CSprite::Draw(
		lpsprite,
		m_lpTexManager->GetData( "Bless.png" )->GetTexture(),
		10, WINDOW_HEIGHT - BLESSGAGE_HEIGHT - 10 + Temp, 0,
		BLESSGAGE_WIDTH, Temp, 
		BLESSGAGE_WIDTH * 2, BLESSGAGE_HEIGHT,
		m_Alpha );

	// 残り敵数
	char Str[ 0xff ];

	sprintf_s( Str, sizeof( Str ), "%d/%d", m_NowEnemys, m_AllEnemys );

	BYTE Num, NumberSize = Sequence::CClear::NUMBER_SIZE;

	for( int i = 0, Len = strlen( Str ); Str[ i ] != '\0'; i++ )
	{
		// 数値変換
		Num = Str[ i ] - '0';

		if( Num < 0 || Num > 9 )
		{
			Num = 12;	// スラッシュ描画
		}
		
		// 描画
		CSprite::Draw(
			lpsprite,
			m_lpTexManager->GetData( "Number.png" )->GetTexture(),
			WINDOW_WIDTH - (Len - i) * NumberSize, 
			WINDOW_HEIGHT - NumberSize,
			0,
			NumberSize * Num, 0,
			NumberSize * (Num + 1), NumberSize );
	}
}

// レーダー描画
void CUI::DrawRadar( 
					const LPD3DXSPRITE lpsprite,
					const D3DXMATRIX* lp_player_mat,
					Enemy::CManager* lp_enemys,
					const Boss::CBase* lp_boss )
{
	// レーダー下
	CSprite::Draw(
		lpsprite,
		m_lpTexManager->GetData( "Radar.png" )->GetTexture(),
		WINDOW_WIDTH - RADAR_SIZE, 0, 0,
		0, 0, RADAR_SIZE, RADAR_SIZE,
		m_Alpha );

	// プレイヤーの位置と方向を割り出す
	// 位置
	CVector	PlayerPos = *lp_player_mat;

	// 方向
	CVector PlayerDir( 0, 0, 1 );
	PlayerDir.GetTransNormal( lp_player_mat );

	// 上下は関係無いのでYを0にして正規化する
	PlayerDir.y = 0.f;
	PlayerDir.GetNormalVector();

	// もともと向いていた方向と今向いている方向で内積を取る
	float Degree = PlayerDir.GetDotDegree( &D3DXVECTOR3( 0, 0, 1 ) );

	// どっち向きの角度かを求める
	if( PlayerDir.x > 0 )
	{
		Degree *= -1;
	}

	// 内積分の回転行列を作る
	CMatrix mRotY;

	// ある程度の大きさ以上なら回転する
	if( abs( Degree ) > 1 )
	{
		mRotY.SetRotY( Degree );
	}
	
	Enemy::CManager::ENEMYLIST* lpEnemys = lp_enemys->GetList();

	const int Harf = RADAR_SIZE / 2 - ENEMYPOSTEX_SIZE / 2 - 1;

	// 総当たり判定
	BOOST_FOREACH( Enemy::CBase* It, *lpEnemys )
	{
		CVector EnemyPos = *(It->GetMatrix());
			
		// 高さを同じにする
		EnemyPos.y = PlayerPos.y;

		//if( IsHitByCircle( PlayerPos, SEARCH_RADIUS, EnemyPos, 0 ) )
		{
			// 方向を求めると同時に、長さも求める
			CVector Vec = EnemyPos - PlayerPos;

			// 長さには制限もかける
			float Length = min( Vec.GetLength(), SEARCH_RADIUS );

			// 正規化ベクトルにする
			Vec.GetTransNormal( &mRotY );
			Vec.GetNormalVector();

			Vec *= Length;
			
			// 方向を出すために最大値修正
			// 描画
			CSprite::Draw(
				lpsprite,
				m_lpTexManager->GetData( "EnemyPos.png" )->GetTexture(),
				WINDOW_WIDTH - RADAR_SIZE + Harf + Vec.x, Harf - Vec.z, 0,
				0, 0, ENEMYPOSTEX_SIZE, ENEMYPOSTEX_SIZE,
				m_Alpha );
		}
	}

	// もしボスがあれば描画
	if( lp_boss )
	{
		CVector BossPos = lp_boss->GetPos();

		BossPos.y = PlayerPos.y;

		if( IsHitByCircle( PlayerPos, SEARCH_RADIUS, BossPos, 0 ) )
		{
			CVector Vec = BossPos - PlayerPos;

			Vec.GetTransCoord( &mRotY );

			// 描画
			CSprite::Draw(
				lpsprite,
				m_lpTexManager->GetData( "EnemyPos.png" )->GetTexture(),
				WINDOW_WIDTH - RADAR_SIZE + Harf + Vec.x, Harf - Vec.z, 0,
				ENEMYPOSTEX_SIZE, 0, ENEMYPOSTEX_SIZE * 2, ENEMYPOSTEX_SIZE,
				m_Alpha );
		}
	}
}

// デストラクタ
CUI::~CUI()
{
}
