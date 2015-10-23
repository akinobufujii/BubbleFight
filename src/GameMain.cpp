//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "SequenceBase.h"
#include "Bubble.h"
#include "BubbleManager.h"

#include "RippleManager.h"
#include "Fintail.h"
#include "Screw.h"

#include "ShotManager.h"
#include "StrongDolphin.h"
#include "ShotBase.h"
#include "EnemyBase.h"
#include "NormalEnemy.h"
#include "EnemyManager.h"

#include "BubbleBreaker.h"

#include "BossBase.h"
#include "HardBorder.h"

#include "FishManager.h"
#include "GameOver.h"
#include "Clear.h"

#include "UI.h"
#include "LimitWalls.h"
#include "PlanktonManager.h"
#include "StoneManager.h"

#include "DamageObjectBase.h"
#include "FishManager.h"

#include "SunLineManager.h"

#include "SurfaceSea.h"

#include "GameMain.h"

//////////////////////////////////////////////////
// 名前空間省略
//////////////////////////////////////////////////
using namespace Sequence;

//////////////////////////////////////////////////
// 静的メンバ実体化
//////////////////////////////////////////////////
const LPSTR		CGameMain::PATH_STONEDATA			= "/StoneData.txt";
const LPSTR		CGameMain::PATH_DAMAGEFISHDATA		= "/DamageFish.txt";
const LPSTR		CGameMain::SKY_MESH_PASS			= "Sky.x";
const LPSTR		CGameMain::PATH_ENEMYDATA			= "/EnemyData.txt";
const float		CGameMain::STAGE_SCALE				= 10.f;
const BYTE		CGameMain::BUBBLES_NUM				= 200;
const float		CGameMain::SURFACESEA_HEIGHT		= 50.f;
const float		CGameMain::BOTTOMSEA_HEIGHT			= -50.f;
const float		CGameMain::SEA_MOVELIMIT			= 150;
const float		CGameMain::SEA_COLOR_ADD			= 200.f / SURFACESEA_HEIGHT / 2;
const float		CGameMain::FOG_DEPTH_ADD			= (45.f / 100.f);
const float		CGameMain::SURFACE_ALPHA_RATE		= (0.3 / SURFACESEA_HEIGHT);
const float		CGameMain::SCROLL_SPEED				= 1.f / 1024.f;
const float		CGameMain::JUMP_CAMERA_HEIGHT		= CGameMain::SURFACESEA_HEIGHT + 5.f;
const float		CGameMain::FOCUS_RATE				= 0.7f / ( abs( CGameMain::BOTTOMSEA_HEIGHT ) + abs( CGameMain::SURFACESEA_HEIGHT ) );
const short		CGameMain::DEFAUL_GAMESCENE_ALPHA	= 210;

CCamera			CGameMain::m_Camera;

const CGameMain::FLGTYPE	CGameMain::FLG_RESULT		= 0x01;
const CGameMain::FLGTYPE	CGameMain::FLG_RUN			= 0x02;
const CGameMain::FLGTYPE	CGameMain::FLG_BOSSBATTLE	= 0x04;
const CGameMain::FLGTYPE	CGameMain::FLG_BOSSENTER	= 0x08;

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CGameMain::CGameMain( BYTE stage ):
m_DamageAlphaCnt( 0 ),
m_GameSceneAlpha( DEFAUL_GAMESCENE_ALPHA ),
m_Flg( FLG_RUN ),
m_lpSurfaceSea( NULL ),
m_lpPlayer( NULL ),
m_lpBackBuffer( NULL ),
m_lpEnemys( NULL ),
m_lpSky( NULL ),
m_lpSprite( NULL ),
m_lpUI( NULL ),
m_lpZBuffer( NULL ),
m_lpPlanktons( NULL ),
m_lpStones( NULL ),
m_lpFishes( NULL ),
m_lpBoss( NULL ),
m_StageIndex( stage ),
m_EnemyNum( 0 )
{
	char Str[ 0xff ];

	sprintf_s( Str,  "CallGameMain\nByteSize = %d", sizeof( CGameMain ) );
	DebugMessageBox( Str );
}

// デストラクタ
CGameMain::~CGameMain()
{
	// BGMストップ
	CSoundManager::GetInst()->Stop( m_BGMPath.c_str() );

	if( m_lpBoss )
	{
		CSoundManager::GetInst()->Stop( m_lpBoss->GetBGMName() );
	}
	
	// メモリ解放
	SAFE_RELEASE( m_lpZBuffer );
	SAFE_RELEASE( m_lpBackBuffer );
	SAFE_DELETE( m_lpPlayer );
	SAFE_DELETE( m_lpSprite );
	SAFE_DELETE( m_lpUI );
	SAFE_DELETE( m_lpEnemys );
	SAFE_DELETE( m_lpStones );
	SAFE_DELETE( m_lpPlanktons );
	SAFE_DELETE( m_lpSurfaceSea );
	SAFE_DELETE( m_lpFishes );
	SAFE_DELETE( m_lpBoss );
	SAFE_DELETE( m_lpSunLines );
}

// 読み込み
void CGameMain::FileLoad()
{
	// 読み込み
	Load();

	// 動的メモリ確保
	Allocs();

	// ステージによって異なる読み込み
	LoadStage( m_StageIndex );

	// ステージと空のメッシュを保存
	m_lpSky	= CSXfileManager::GetInst()->GetData( SKY_MESH_PASS );

	// ライト設定
	D3DLIGHT9	Light;
	ZeroMemory( &Light, sizeof( Light ) );

	Light.Type	= D3DLIGHT_DIRECTIONAL;	// 平行光源
	Light.Diffuse.r	= 1.0f;
	Light.Diffuse.g	= 1.0f;
	Light.Diffuse.b	= 1.0f;

	Light.Direction = D3DXVECTOR3( 1.0f, -1.0f, 1.0f);

	D3D::GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	D3D::GetDevice()->SetLight( 0, &Light );
	D3D::GetDevice()->LightEnable( 0, TRUE );

	// バックバッファ情報を保存
	D3D::GetDevice()->GetDepthStencilSurface( &m_lpZBuffer );
	D3D::GetDevice()->GetRenderTarget( 0, &m_lpBackBuffer );
	D3D::GetDevice()->GetViewport( &m_VP );

	// ※敵にプレイヤーの情報を知らせておく
	Enemy::CBase::SetPlayerInfo( m_lpPlayer );

	// フォグ設定(使わないかも)
	D3D::GetDevice()->SetRenderState( D3DRS_FOGENABLE, TRUE );
	D3D::GetDevice()->SetRenderState( D3DRS_FOGCOLOR, D3DCOLOR_XRGB( 0, 0x00, 0x80 ) );
	D3D::GetDevice()->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_LINEAR );
	D3D::GetDevice()->SetRenderState( D3DRS_FOGSTART, GetAddress( 50.f ) );
	D3D::GetDevice()->SetRenderState( D3DRS_FOGEND,	GetAddress( 150.f ) );

	D3D::GetDevice()->SetRenderState( D3DRS_SPECULARENABLE, TRUE );

	// 海面
	m_lpSurfaceSea->SetHeight( SURFACESEA_HEIGHT );
	m_lpSurfaceSea->SetSize( SEA_MOVELIMIT * 2 );

	// 海底
	m_BottomSea.SetVertex( SEA_MOVELIMIT * 2, false );

	// プランクトン
	m_lpPlanktons->SetNum( 1000 );

	// プレイヤーに岩マネージャーを設定する
	m_lpPlayer->SetStoneManager( m_lpStones );

	// 敵にも岩マネージャーを設定する
	Enemy::CBase::SetStoneManager( m_lpStones );
	Enemy::CBase::SetFishManager( m_lpFishes );

	// 魚にも
	CFishManager::SetStoneManager( m_lpStones );

	// ボスにも
	Boss::CBase::SetStoneManager( m_lpStones );

	// 壁を設定する
	m_Walls.SetLimit( SEA_MOVELIMIT );
	m_Walls.SetScale( SEA_MOVELIMIT * 2,
		abs( SURFACESEA_HEIGHT ) + abs( BOTTOMSEA_HEIGHT ) );

	// 魚を生み出す
	m_lpFishes->SetNumAndLimit( 50, D3DXVECTOR3(
		SEA_MOVELIMIT,
		SURFACESEA_HEIGHT - 10,
		SEA_MOVELIMIT ) );

	// レンダーターゲット作成
	// シーン描画用
	m_SceneTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT );
	
	// 縮小用
	m_MiniSizeTex.CreateRenderTarget(
		D3D::GetDevice(), WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4 );

	// ぼかし用
	m_BlurTex.CreateRenderTarget(
		D3D::GetDevice(), WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4 );

	// 深度保存用
	m_DepthTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT );

	// 最終結果用
	m_ResultTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT, D3DFMT_A16B16G16R16F );

	// シェーダー作成
	// ぼかし用シェーダー
	m_BlurShader.Load( D3D::GetDevice(), "FX/Blur.fx", "Tec" );
	D3DXVECTOR2 TexPos( 0.5f / (WINDOW_WIDTH / 4), 0.5f / (WINDOW_HEIGHT / 4) );
	m_BlurShader.SetValue( "g_Offset", &TexPos, sizeof(D3DXVECTOR2) );

	// 深度計算用
	m_DepthShader.Load( D3D::GetDevice(), "FX/Depth.fx", "Tec" );

	// 被写界深度シェーダー
	m_DOFShader.Load( D3D::GetDevice(), "FX/DOF.fx", "Tec" );

	// フォンシェーダ
	CSShaderManager::GetInst()->Load( D3D::GetDevice(), "Phong.fx" );

	// リムライティングシェーダ
	CSShaderManager::GetInst()->Load( D3D::GetDevice(), "Rim.fx" );

	// 爆発ブラーシェーダー
	m_BombBlurShader.Load( D3D::GetDevice(), "FX/BombBlur.fx", "Tec" );

	// メッシュに使用するシェーダーを設定する
	CXfile::SetShaderDevice( &m_DepthShader, "g_World" );
	CBoard::SetShaderDevice( &m_DepthShader, "g_World" );

	// 太陽線
	m_lpSunLines->SetRandom( 150 );

	// マウス位置初期化
	RECT Rect;
	GetWindowRect( GetDesktopWindow(), &Rect );
	SetCursorPos( Rect.right / 2, Rect.bottom / 2 );

	// 音楽
	m_BGMPath = "Sound/GameBGM.wav";
	CSoundManager::GetInst()->Play( m_BGMPath.c_str(), true );

	m_Timer.restart();
}

// 次のシーケンス
IBase* CGameMain::GetNext()
{
	if( m_lpPlayer->GetFlg() & Player::CDolphin::FLG_DIE )
	{
		return NEW CGameOver();

	}else{

		// スコア合成
		ULONG Score = m_lpEnemys->GetScore();

		if( m_lpBoss )
		{
			Score += m_lpBoss->GetScore();
		}

		return NEW CClear( Score, m_Timer.elapsed() );
	}
}

// 処理本体
bool CGameMain::RunProc()
{

#ifdef _DEBUG

	// 敵の読み込みしなおす
	if( Input::Keyboard::IsState( DIK_E, Input::PUSH ) )
	{
		// 空にしてから読み込む
		m_lpEnemys->AllClear();
		m_lpEnemys->LoadFromFile( PATH_ENEMYDATA );
	}

	// 時間表示
	char Str[ 0xff ];

	sprintf_s( Str, sizeof( Str ), "%lf秒 \n" ,m_Timer.elapsed() );

	//OutputDebugStr( Str );

	if( Input::Keyboard::IsState( DIK_W, Input::PUSH ) )
	{
		m_lpEnemys->AllClear();
	}

#endif

	// デバッグキー
	if( Input::Keyboard::IsState( DIK_W, Input::PUSH ) ){ m_lpEnemys->AllClear(); }

	// 更新
	Update();

	// 衝突判定
	if( !(m_Flg & FLG_RESULT) )
	{
		Collision();
	}

	// 描画
	//if( FPS::IsSlowly() == false )
	{
		Draw();
	}

	return (m_Flg & FLG_RUN)? true : false;
}

// 更新
void CGameMain::Update()
{
	// 敵がいなくなるか、ライフが０になれば
	if( (m_lpEnemys->IsNoneEnemy() && ((m_lpBoss)? m_lpBoss->IsDie() : true) ) ||  
		(m_lpPlayer->GetFlg() & Player::CDolphin::FLG_DIE) )
	{
		// 終了
		m_Flg |= FLG_RESULT;
	}

	// 結果が決まったなら
	if( m_Flg & FLG_RESULT )
	{
		if( (m_GameSceneAlpha -= 5) < 0 )
		{
			m_GameSceneAlpha = 0;
			m_Flg &= ~FLG_RUN;
		}
	}

	// キャラ
	// ボスが居て演出時じゃなければ更新
	if( !(m_Flg & FLG_BOSSENTER) )
	{
		m_lpPlayer->Update();
	}
	
	// 敵
	m_lpEnemys->Update();

	// 敵がいなくなればボスを出す
	if( m_lpEnemys->IsNoneEnemy() && !(m_Flg & (FLG_BOSSBATTLE | FLG_BOSSENTER)) )
	{
		m_Flg |= FLG_BOSSBATTLE;

		// もしボスがいれば
		if( m_lpBoss )
		{
			CSoundManager::GetInst()->Stop( m_BGMPath.c_str() );
			CSoundManager::GetInst()->Play( "Sound/BossEnter.wav" );
			m_Flg |= FLG_BOSSENTER;
		}

		m_SaveEnterBossTime = timeGetTime();
	}

	// ボス演出
	if( (m_Flg & FLG_BOSSENTER) && m_lpBoss )
	{
		// ※最低5秒間
		if( m_lpBoss->UpdateEnterScene() &&
			timeGetTime() - m_SaveEnterBossTime > 5000 )
		{
			CSoundManager::GetInst()->Play( m_lpBoss->GetBGMName(), true );
			m_Flg &= ~FLG_BOSSENTER;
		}

	}else if( (m_Flg & FLG_BOSSBATTLE) && m_lpBoss ){

		Boss::CBase::SetPlayerMat( &m_lpPlayer->GetMatrix() );
		m_lpBoss->Update();
	}

	// 波
	static const float TEXPOS = 1.f / 1024;
	static int Cnt = 0;
	float Scroll = TEXPOS * Cnt++;

	Cnt %= 1024;

	D3DXVECTOR3 Pos = m_lpPlayer->GetPos();

	const float UV = 0.5f / SEA_MOVELIMIT;

	// 海面
	m_lpSurfaceSea->Update( Pos );

	// ダメージフェードアウト
	if( (m_DamageAlphaCnt -= 5) < 0 )
	{
		m_DamageAlphaCnt = 0;
	}
	
	// 魚
	m_lpFishes->Update();

	// インターフェース
	m_lpUI->Update(
		m_lpEnemys->GetScore(),
		m_lpPlayer->GetLife(),
		m_lpPlayer->GetPower(),
		m_lpPlayer->GetBless(),
		m_lpEnemys->GetList()->size(),
		m_EnemyNum );
}

// 衝突判定
void CGameMain::Collision()
{
	// 全て総当たり
	Enemy::CManager::ENEMYLIST*				EnemyList	= m_lpEnemys->GetList(); 
	Enemy::CManager::ENEMYLIST::iterator	Enemy		= EnemyList->begin();
	
	while( Enemy != EnemyList->end() )
	{
		// もし爆発してたら行わない
		if( (*Enemy)->GetFlg() & Enemy::CBase::BREAK )
		{
			++Enemy;
			continue;
		}

		// 敵と弾
		BOOST_FOREACH( Shot::CBase* Shot, *m_lpPlayer->GetShots()->GetList() )
		{
			// 弾と判定
			if( IsHitByCircle( 
				(*Enemy)->GetPos(), 1.f,
				Shot->GetPos(), Shot->GetRadius() ) )
			{
				if( !( (*Enemy)->GetFlg() & Enemy::CBase::BREAK ) &&
					( Shot->GetFlg() & Shot::CBase::FLG_ALIVE ) )
				{
					// 応答
					(*Enemy)->Dispatch( Shot );
					Shot->Dispatch( (*Enemy) );
				}
			}
		}

		// 自機と敵の当たり判定
		// もし敵が泡につつまれていたら判定しない
		if( !( (*Enemy)->GetFlg() & Enemy::CBase::BUBBLE_WRAP ) )
		{
			if( IsHitByCircle( m_lpPlayer->GetPos(), 0.5f, 
				(*Enemy)->GetPos(), (*Enemy)->GetMesh()->GetRadius() ) )
			{
				(*Enemy)->Dispatch( m_lpPlayer );
				m_lpPlayer->Dispatch( (*Enemy) );
			}
		}

		// 自機と敵の出している弾との当たり判定
		if( (*Enemy)->GetShotManager() != NULL )
		{
			BOOST_FOREACH( Shot::CBase* Shot, *(*Enemy)->GetShotManager()->GetList() )
			{
				// 自機との衝突検知
				if( IsHitByCircle( m_lpPlayer->GetPos(), 0.5f,
					Shot->GetPos(), Shot->GetRadius() ) )
				{
					// 衝突応答
					Shot->Dispatch( m_lpPlayer );
					m_lpPlayer->Dispatch( Shot );
				}

				// 自機の出している弾との衝突判定
				BOOST_FOREACH( Shot::CBase* PlayerShot, 
					*m_lpPlayer->GetShots()->GetList() )
				{
					// 衝突検知
					if( IsHitByCircle( PlayerShot->GetPos(), PlayerShot->GetRadius(),
						Shot->GetPos(), Shot->GetRadius() ) )
					{
						// 衝突応答
						Shot->Dispatch( PlayerShot );
						PlayerShot->Dispatch( Shot );
					}
				}

				// 敵自身と敵の出している弾の当たり判定(跳ね返されたもの限定)
				if( Shot->GetFlg() & Shot::CBase::FLG_FIN_HIT )
				{
					if( IsHitByCircle(
						(*Enemy)->GetPos(), (*Enemy)->GetMesh()->GetRadius(),
						Shot->GetPos(), Shot->GetRadius() ) )
					{
						Shot->Dispatch( (*Enemy) );
						(*Enemy)->Dispatch( Shot );
					}
				}
			}
		}

		// 敵と襲うオブジェクトとの判定
		BOOST_FOREACH( CDamageObjectBase* Fish, *m_lpFishes->GetDamageList() )
		{
			if( IsHitByCircle( 
				(*Enemy)->GetPos(), (*Enemy)->GetMesh()->GetRadius(),
				Fish->GetPos(), 1.f ) )
			{
				Fish->Dispatch( (*Enemy) );
				(*Enemy)->Dispatch( Fish );
			}
		}

		// 敵同士の当たり判定
		Enemy::CManager::ENEMYLIST::iterator Enemy2 = Enemy;
		
		// ※1個進めておく
		++Enemy2;
		
		while( Enemy2 != EnemyList->end() )
		{
			if( IsHitByCircle( (*Enemy)->GetPos(), (*Enemy)->GetMesh()->GetRadius(), 
				(*Enemy2)->GetPos(), (*Enemy2)->GetMesh()->GetRadius() ) )
			{
				(*Enemy)->Dispatch( (*Enemy2) );
				(*Enemy2)->Dispatch( (*Enemy) );
			}
			++Enemy2;
		}

		// BubbleBreakerの独特の処理(危険)
		Enemy::CBubbleBreaker* lpBubbleBreaker =
			dynamic_cast<Enemy::CBubbleBreaker*>( (*Enemy) );

		// 内容が入れば
		if( lpBubbleBreaker != NULL )
		{
			// 総当たりで検査
			BOOST_FOREACH( Enemy::CBase* TargetEnemy, *EnemyList )
			{
				// 泡に包まれていれば
				if( TargetEnemy->GetBubbleScale() > 0 )
				{
					lpBubbleBreaker->SetDirEnemy( TargetEnemy );
				}
			}
		}

		++Enemy;
	}

	// ボスとの当たり判定
	if( (m_Flg & FLG_BOSSBATTLE) && m_lpBoss )
	{
		BOOST_FOREACH( Shot::CBase* PlayerShot, *m_lpPlayer->GetShots()->GetList() )
		{
			// プレイヤーの弾とボスの衝突判定
			if( IsHitByCircle( m_lpBoss->GetPos(), m_lpBoss->GetRadius(), 
				PlayerShot->GetPos(), PlayerShot->GetRadius() ) )
			{
				PlayerShot->Dispatch( m_lpBoss );
				m_lpBoss->Dispatch( PlayerShot );
			}
		}

		BOOST_FOREACH( Shot::CBase* BossShot, *m_lpBoss->GetShotManager()->GetList() )
		{
			// プレイヤーとボスの弾との衝突判定
			if( IsHitByCircle( m_lpPlayer->GetPos(), 0.5f, 
				BossShot->GetPos(), BossShot->GetRadius() ) )
			{
				BossShot->Dispatch( m_lpPlayer );
				m_lpPlayer->Dispatch( BossShot );
			}

			// ボスと跳ね返された弾との判定
			if( IsHitByCircle( m_lpBoss->GetPos(), m_lpBoss->GetRadius(),
								BossShot->GetPos(), BossShot->GetRadius() ) )
			{
				BossShot->Dispatch( m_lpBoss );
				m_lpBoss->Dispatch( BossShot );
			}
		}

		BOOST_FOREACH( Shot::CBase* PlayerShot, *m_lpPlayer->GetShots()->GetList() )
		{
			BOOST_FOREACH( Shot::CBase* BossShot, *m_lpBoss->GetShotManager()->GetList() )
			{
				// プレイヤーの弾とボスの弾の当たり判定
				if( IsHitByCircle( BossShot->GetPos(), BossShot->GetRadius(), 
									PlayerShot->GetPos(), PlayerShot->GetRadius() ) )
				{
					BossShot->Dispatch( PlayerShot );
					PlayerShot->Dispatch( BossShot );
				}
			}
		}

		// プレイヤー自身とボス自身の衝突判定
		if( IsHitByCircle( m_lpPlayer->GetPos(), 0.5f, 
			m_lpBoss->GetPos(), m_lpBoss->GetRadius() ) )
		{
			m_lpPlayer->Dispatch( m_lpBoss );
			m_lpBoss->Dispatch( m_lpPlayer );
		}
	}
}

// 描画
void CGameMain::Draw()
{
	// デバイス獲得
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// 描画開始
	D3D::Begin();
	
	// 投影
	m_Proj.SetFar( 500 );
	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );

	// カメラ(位置はプレイヤーの状態によって変える)
	CVector	VecTrans( 0.0f, 2.0f, -7.0f );
	CVector	VecLook( 0.0f, 0.0f, 100.0f );
	CVector VecUp( 0.f, 1.f, 0.f );

	// カメラ設定変更
	if( m_lpBoss && (m_Flg & FLG_BOSSENTER) )
	{
		VecLook = m_lpBoss->GetPos();
		
		// ※回す
		CMatrix mRotY, mTrans( 0, 2.0f, -6 + -2.0f * cos( D3DXToRadian(timeGetTime() / 10 % 360) ) );
		D3DXVECTOR4 Pos;

		mRotY.SetRotY( timeGetTime() / 10 % 360 );

		//VecTrans.GetTransCoord( &(m_lpBoss->GetMatrix()) );

		VecTrans.Set( 0, 0, 0 );

		D3DXVec3Transform( &Pos, &VecTrans, &(mTrans * mRotY * m_lpBoss->GetMatrix()) );

		VecTrans.Set( Pos.x, Pos.y, Pos.z );
	
	}else{

		if( m_lpPlayer->GetFlg() & Player::CDolphin::FLG_OVER )
		{
			VecTrans	= CVector( m_lpPlayer->GetSaveMat() );
			VecLook		= CVector( m_lpPlayer->GetMatrix() );

			VecTrans.y += 5;

		}else if( m_lpPlayer->GetFlg() & Player::CDolphin::FLG_JUMP ){

			// 水面ジャンプ中
			VecTrans.Set( 15, 0, 0 );

			VecLook	= CVector( m_lpPlayer->GetMatrix() );

			VecTrans.GetTransCoord( &m_lpPlayer->GetMatrix() );

			// Y軸だけ固定
			VecTrans.y = JUMP_CAMERA_HEIGHT;

		}else{

			VecTrans.GetTransCoord	( &m_lpPlayer->GetAfterMatrix() );
			VecLook.GetTransCoord	( &m_lpPlayer->GetAfterMatrix() );
			VecUp.GetTransNormal	( &m_lpPlayer->GetAfterMatrix() );
		}
	}

	m_Camera.SetCamera( &VecTrans, &VecLook, &VecUp );

	lpDevice->SetTransform( D3DTS_VIEW, m_Camera.GetMatrix() );

	// 視錐台カリング判定材料として登録
	SetCullingCamera( &m_Camera );
	SetCullingProj( &m_Proj );

	// 画面クリア
	lpDevice->Clear( 0,
		NULL,
		D3DCLEAR_ZBUFFER | ((m_Flg & FLG_RESULT)? D3DCLEAR_TARGET : 0),
		D3DCOLOR_XRGB( 0, 0, 0 ),
		1.f, 0 );

	// テクスチャに描画
	lpDevice->SetRenderTarget( 0, m_SceneTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_SceneTex.GetZbuffer() );
	lpDevice->SetViewport( &m_VP );

	// メッシュは固定パイプラインで描画
	CXfile::SetDrawShader( false );
	CBoard::SetDrawShader( false );

	// 海の色変化(実際は画面クリア)
	// 高さによって色を変える
	float PlayerY = m_lpPlayer->GetPos().y;

	// 実数→整数
	int Color = static_cast<int>( SEA_COLOR_ADD * PlayerY );
	
	// 実際に色を変える(フォグも)
	lpDevice->Clear( 0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0x00, (Color < 0)? 0 : Color, 0x80 + Color ),
		1.f, 0 );

	lpDevice->SetRenderState( 
		D3DRS_FOGCOLOR, 
		D3DCOLOR_XRGB( 0x00, (Color < 0)? 0 : Color, 0x80 + Color ) );

	m_lpSurfaceSea->SetColor( 0x00, (Color < 0)? 0 : Color, 0x80 + Color );

	// 濃度も変える
	float Temp = 60.f + FOG_DEPTH_ADD * m_lpPlayer->GetPos().y;
	lpDevice->SetRenderState( D3DRS_FOGSTART, GetAddress( Temp ) );

	// レンダリング
	Rendering( true );

	// 縮小してぼかす
	// レンダーターゲットを変える
	D3DVIEWPORT9 Vp = { 0, 0, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, 0.f, 1.f };
	lpDevice->SetRenderTarget( 0, m_MiniSizeTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_MiniSizeTex.GetZbuffer() );
	lpDevice->SetViewport( &Vp );

	lpDevice->Clear( 0,
		NULL,
		D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_XRGB( 0, 0, 0 ),
		1.f, 0 );

	lpDevice->SetTransform( D3DTS_VIEW, &CMatrix() );
	lpDevice->SetTransform( D3DTS_PROJECTION, &CMatrix() );
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );
	lpDevice->SetFVF( CBoard::FVF_VERTEX );

	// シェーダーに値を渡す
	m_BlurShader.SetTexture( "g_Texture", m_SceneTex.GetTexture() );

	// シェーダー描画開始
	m_BlurShader.Begin();
	m_BlurShader.BeginPass( 0 );

	m_SceneBorad.SetVertex( 2 );
	m_SceneBorad.Draw( lpDevice );

	m_BlurShader.EndPass();
	m_BlurShader.End();

	// レンダーターゲットを変える
	lpDevice->SetRenderTarget( 0, m_BlurTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_BlurTex.GetZbuffer() );
	m_BlurShader.SetTexture( "g_Texture", m_MiniSizeTex.GetTexture() );

	// シェーダー描画開始
	m_BlurShader.Begin();
	m_BlurShader.BeginPass( 1 );

	m_SceneBorad.SetVertex( 2 );
	m_SceneBorad.Draw( lpDevice );

	m_BlurShader.EndPass();
	m_BlurShader.End();
	
	// 深度計算する
	// レンダーターゲットを変える
	lpDevice->SetRenderTarget( 0, m_DepthTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_DepthTex.GetZbuffer() );
	Vp.Width = WINDOW_WIDTH;
	Vp.Height= WINDOW_HEIGHT;
	lpDevice->SetViewport( &Vp );

	lpDevice->Clear( 0,
		NULL,
		D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_XRGB( 0, 0, 0 ),
		1.f, 0 );

	// シェーダーに値を渡す
	m_DepthShader.SetValue( "g_View", m_Camera.GetMatrix(), sizeof(D3DXMATRIX) );
	m_DepthShader.SetValue( "g_Proj", m_Proj.GetMatrix(), sizeof(D3DXMATRIX) );

	// メッシュはシェーダーで描画する
	CXfile::SetDrawShader( true );
	CBoard::SetDrawShader( true );

	// シェーダー描画開始
	m_DepthShader.Begin();
	m_DepthShader.BeginPass( 0 );

	lpDevice->Clear( 0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0, 0, 0 ),
		1.f, 0 );

	lpDevice->SetRenderState( 
		D3DRS_FOGCOLOR, 
		D3DCOLOR_XRGB( 0, 0, 0 ) );

	Rendering( false );
	
	m_DepthShader.EndPass();
	m_DepthShader.End();

	// シーン、ぼかし、深度計算を使って被写界深度適用
	// もとのレンダリングターゲットに戻す
	lpDevice->SetRenderTarget( 0, m_ResultTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_ResultTex.GetZbuffer() );
	
	// シェーダーにテクスチャを流す
	m_DOFShader.SetTexture( "g_SceneTex", m_SceneTex.GetTexture() );	// シーン
	m_DOFShader.SetTexture( "g_BlurTex", m_BlurTex.GetTexture() );		// ぼかし
	m_DOFShader.SetTexture( "g_DepthTex", m_DepthTex.GetTexture() );	// 深度

	// プレイヤーの位置によってフォーカスを変える
	float FocusRate = (m_lpPlayer->GetPos().y + abs( BOTTOMSEA_HEIGHT )) * FOCUS_RATE;

	m_DOFShader.SetValue( "g_Focus", &FocusRate, sizeof(float) );

	// 板ポリゴンでシーン描画
	lpDevice->SetTransform( D3DTS_VIEW, &CMatrix() );
	lpDevice->SetTransform( D3DTS_PROJECTION, &CMatrix() );
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );

	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	
	m_DOFShader.Begin();
	m_DOFShader.BeginPass( 0 );

	m_SceneBorad.Draw( lpDevice );

	m_DOFShader.EndPass();
	m_DOFShader.End();

	// ボスバトル中
	if( (m_Flg & FLG_BOSSBATTLE) && m_lpBoss )
	{
		// 爆発ブラーをかける
		// レンダーターゲット変更
		lpDevice->SetRenderTarget( 0, m_SceneTex.GetSurface() );
		lpDevice->SetDepthStencilSurface( NULL );

		// 爆発位置設定
		m_BombBlurShader.SetValue(
			"g_CenterTexPos",
			&GetTexPos( *m_Camera.GetMatrix(), 
						*m_Proj.GetMatrix(),
						m_lpBoss->GetPos() ),
			sizeof( D3DXVECTOR2 ) );

		// 画面サイズ指定
		m_BombBlurShader.SetValue(
			"g_TexOffset",
			&D3DXVECTOR2( 1.f / WINDOW_WIDTH, 1.f / WINDOW_HEIGHT ),
			sizeof( D3DXVECTOR2 ) );

		float Temp = m_lpBoss->GetBlurPower();
		// 爆発の強さ
		m_BombBlurShader.SetValue(
			"g_BlurPower",
			&Temp,
			sizeof( float ) );

		Temp = m_lpBoss->GetAddColorRate();

		// 爆発の光の強さ
		m_BombBlurShader.SetValue(
			"g_AddColorRate",
			&Temp,
			sizeof( float ) );

		// テクスチャ設定
		m_BombBlurShader.SetTexture( "g_Tex", m_ResultTex );

		m_BombBlurShader.Begin();
		m_BombBlurShader.BeginPass( 0 );

		m_SceneBorad.Draw( lpDevice );

		m_BombBlurShader.EndPass();
		m_BombBlurShader.End();

		lpDevice->SetRenderTarget( 0, m_ResultTex.GetSurface() );
		lpDevice->SetDepthStencilSurface( m_ResultTex.GetZbuffer() );

		lpDevice->SetTexture( 0, m_SceneTex );
		m_SceneBorad.Draw( lpDevice );
	}

	// 元のレンダーターゲットに戻す
	lpDevice->SetRenderTarget( 0, m_lpBackBuffer );
	lpDevice->SetDepthStencilSurface( m_lpZBuffer );

	CBoard::SetDrawShader( false );

	// スプライト描画
	DrawSprite();

	// 描画終了
	D3D::End();
}

// メッシュを描画
void CGameMain::DrawMesh( bool is_effect )
{
	// デバイス獲得
	LPDIRECT3DDEVICE9	lpDevice = D3D::GetDevice();

	D3DXVECTOR3 PlayerPos = m_lpPlayer->GetPos();

	// 行列
	CMatrix mWorld;
	CMatrix mTrans( m_lpPlayer->GetPos() );
	CMatrix mScale;

	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	// 空
	// 海面と同じ所に動かす
	D3DXVECTOR3 TempVec = m_lpPlayer->GetPos();
	TempVec.y = SURFACESEA_HEIGHT;
	mTrans.SetTrans( &TempVec );

	// 少し小さくする
	mScale.SetScale( STAGE_SCALE );

	// 少しずつ回転させる
	mWorld = mScale * mTrans;

	lpDevice->SetTransform( D3DTS_WORLD, &mWorld );

	// 海面に近ければフォグを無効に
	//if( m_lpPlayer->GetPos().y >= SURFACESEA_HEIGHT / 1.1f )
	{
		lpDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	}

	m_lpSky->Draw( lpDevice );

	// フォグを有効に
	lpDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );

	// 岩を描画
	if( is_effect )
	{
		m_lpStones->DrawShader( lpDevice );
	
	}else{

		m_lpStones->Draw( lpDevice );
	}

	// 死んでいる敵
	m_lpEnemys->DrawDie( lpDevice, is_effect );

	// ※ライティング処理のあるもの
	lpDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	// 生きている敵
	m_lpEnemys->DrawAlive( lpDevice, is_effect );

	// いじめられる魚
	m_lpFishes->DrawDamageFish( lpDevice, is_effect );

	// ボス
	if( (m_Flg & FLG_BOSSBATTLE) && m_lpBoss )
	{
		if( is_effect )
		{
			m_lpBoss->DrawShader( lpDevice );
		
		}else{

			m_lpBoss->Draw( lpDevice );
		}
	}

	// キャラ
	lpDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_lpPlayer->Draw( lpDevice );
}

// メッシュじゃない3Dオブジェクトを描画
void CGameMain::DrawPolygon( bool is_effect )
{
	// デバイス獲得
	LPDIRECT3DDEVICE9	lpDevice = D3D::GetDevice();

	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	// 魚
	m_lpFishes->DrawObjectFish( lpDevice );

	// プレイヤーの座標獲得
	D3DXVECTOR3 PlayerPos = m_lpPlayer->GetPos();

	lpDevice->SetFVF( CBoard::FVF_VERTEX );
	//lpDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
	lpDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	lpDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// ※テスト
	m_lpSunLines->Update();
	//m_lpSunLines->Draw( lpDevice );
	
	// 海面
	if( is_effect )
	{
		m_lpSurfaceSea->DrawShader( lpDevice );
	
	}else{

		m_lpSurfaceSea->Draw( lpDevice );
	}

	// 魚
	m_lpFishes->DrawObjectFish( lpDevice );

	// 壁
	m_Walls.Draw( lpDevice );

	// ※テスト
	m_lpSunLines->Draw( lpDevice );

	// プランクトン
	m_lpPlanktons->Draw( lpDevice, &m_Camera );

	// プレイヤーの波紋
	m_lpPlayer->DrawRipples( lpDevice );
}

// エフェクト類描画
void CGameMain::DrawEffect()
{
	// デバイス獲得
	LPDIRECT3DDEVICE9	lpDevice = D3D::GetDevice();

	// 加算合成で波紋を表示
	lpDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	lpDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

	// 敵の波紋
	m_lpEnemys->DrawRipples( lpDevice );

	// 敵のエフェクト描画
	m_lpEnemys->DrawEffect( lpDevice );

	// ボスのエフェクト
	if( m_lpBoss )
	{
		m_lpBoss->DrawEffect( lpDevice );
	}

	// プレイヤーのエフェクト
	m_lpPlayer->DrawEffect( lpDevice );

	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	lpDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
}

// スプライト描画
void CGameMain::DrawSprite()
{
	// スプライト描画
	LPD3DXSPRITE lpSprite = m_lpSprite->Begin( D3DXSPRITE_ALPHABLEND );

	RECT Rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	// ゲーム画面
	lpSprite->SetTransform( &CMatrix() );

	// ジャンプ中はブラーをかけない
	int NowAlpha = m_GameSceneAlpha;

	if( m_lpPlayer->GetFlg() & Player::CDolphin::FLG_JUMP )
	{
		NowAlpha = 0xff;
	}

	lpSprite->Draw(
		m_ResultTex,
		&Rc, NULL, NULL,
		D3DCOLOR_ARGB( NowAlpha, 0xff, 0xff, 0xff ) );

	// 結果が見えたらフェードアウト
	if( m_Flg & FLG_RESULT )
	{
		m_lpSprite->SetAlpha( m_GameSceneAlpha );
		m_lpUI->SetAlpha( m_GameSceneAlpha );
	}

	// UI描画
	m_lpUI->Draw( m_lpSprite->GetDevice() );
	m_lpUI->DrawRadar( 
		m_lpSprite->GetDevice(),
		&m_lpPlayer->GetMatrix(),
		m_lpEnemys,
		(m_Flg & FLG_BOSSBATTLE)? m_lpBoss : NULL );

	// ダメージ
	if( m_lpPlayer->GetFlg() & Player::CDolphin::FLG_DAMAGE )
	{
		m_DamageAlphaCnt = 0xff;
	}

	// ダメージ描画
	if( m_DamageAlphaCnt > 0 && !(m_lpPlayer->GetFlg() & Player::CDolphin::FLG_JUMP) )
	{
		m_lpSprite->SetAlpha( m_DamageAlphaCnt );
		
		m_lpSprite->Draw(
			CSTexManager::GetInst()->GetData( "Damage.png" )->GetTexture(),
			0, 0, 0,
			0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );

		m_lpSprite->SetAlpha( 0xff );
	}

	// スプライト描画終了
	m_lpSprite->End();
}

// 読み込み
void CGameMain::Load()
{
	// テクスチャロードファイルオープン
	CSTexManager::GetInst()->LoadFromTextList( D3D::GetDevice(), "LoadData/GameMain/Tex.txt" );

	// Xファイルロードファイルオープン
	CSXfileManager::GetInst()->LoadFromTextList( D3D::GetDevice(), "LoadData/GameMain/Xdata.txt" );

	// 音楽読み込み
	CSoundManager::GetInst()->LoadFromTextList( "LoadData/GameMain/Sound.txt" );
}

// 読み込み
void CGameMain::Release()
{
	// テクスチャロードファイルオープン
	CSTexManager::GetInst()->ReleaseFromTextList( "LoadData/GameMain/Tex.txt" );

	// Xファイルロードファイルオープン
	CSXfileManager::GetInst()->ReleaseFromTextList( "LoadData/GameMain/Xdata.txt" );
}

// ステージごとの読み込み
void CGameMain::LoadStage(BYTE stage)
{
	std::string Path( "LoadData/GameMain/Stage" );
	std::string Temp;

	// 実数→文字
	Path += stage + '0';

	// コピー
	Temp = Path;

	// 読み込むファイル名を追加
	Temp += PATH_ENEMYDATA;

	// 敵を読み込む
	m_lpEnemys->LoadFromFile( Temp.c_str() );

	// 敵数記憶
	m_EnemyNum = m_lpEnemys->GetList()->size();

	Temp = Path;

	Temp += PATH_STONEDATA;

	// 岩の読み込み
	m_lpStones->LoadFromData( D3D::GetDevice(), Temp.c_str() );

	Temp = Path;

	Temp += PATH_DAMAGEFISHDATA;

	// ダメージを受ける魚読み込む
	m_lpFishes->LoadDamageFish( Temp.c_str() );

	Temp = Path;

	Temp += "/IsBoss.txt";

	// ボス
	std::ifstream FileIn( Temp.c_str() );
	
	// テキストファイルがあれば読み込み
	if( FileIn.is_open() )
	{
		m_lpBoss = NEW Boss::CHardBorder( D3D::GetDevice() );
		CSoundManager::GetInst()->Load( m_lpBoss->GetBGMName() );
	}
}

// 動的メモリ確保を行う
void CGameMain::Allocs()
{
	// スプライト
	DebugMessageBox( "スプライトデバイスを作成します" );
	m_lpSprite = NEW CSprite( D3D::GetDevice() );

	// ユーザーインターフェイス
	DebugMessageBox( "UIを作成します" );
	m_lpUI = NEW CUI();

	// プレイヤー
	DebugMessageBox( "プレイヤーを作成します" );
	m_lpPlayer = NEW Player::CDolphin();

	// 敵
	DebugMessageBox( "敵マネージャーを作成します" );
	m_lpEnemys = NEW Enemy::CManager();

	// 海面
	DebugMessageBox( "海面を作成します" );
	m_lpSurfaceSea = NEW CSurfaceSea();

	// プランクトン
	DebugMessageBox( "プランクトンマネージャーを作成します" );
	m_lpPlanktons = NEW CPlanktonManager();

	// 岩
	DebugMessageBox( "岩マネージャーを作成します" );
	m_lpStones = NEW CStoneManager();

	// 魚
	DebugMessageBox( "魚マネージャーを作成します" );
	m_lpFishes = NEW CFishManager();

	// 太陽線
	m_lpSunLines = NEW CSunLineManager();

	DebugMessageBox( "動的メモリ確保完了" );
}

// レンダリング
void CGameMain::Rendering( bool is_effect )
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// メッシュ類を描画
	DrawMesh( is_effect );
	
	// メッシュ以外の3D描画
	DrawPolygon( is_effect );

	// エフェクト類描画
	DrawEffect();
}