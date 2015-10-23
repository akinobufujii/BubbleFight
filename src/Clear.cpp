///////////////////////////////
// インクルード
///////////////////////////////
#include "Title.h"
#include "FishManager.h"

#include "RippleManager.h"
#include "BubbleManager.h"
#include "FinTail.h"
#include "BubbleManager.h"
#include "ShotManager.h"
#include "StrongDolphin.h"

#include "UI.h"

#include "Clear.h"

///////////////////////////////
// 名前空間省略
///////////////////////////////
using namespace Sequence;

///////////////////////////////
// 静的メンバ実体化
///////////////////////////////
const LPSTR	CClear::PASS_BACK_GROUND_MESH	= "stage.x";
const LPSTR	CClear::TEX_BUBBLE				= "Sprite3.png";
const LPSTR CClear::TEXPASS_FONT			= "ClearFont.png";
const short	CClear::BUBBLE_ADD_TIME			= 300;		
const BYTE	CClear::BUBBLE_NUM				= 50;				
const BYTE	CClear::BUBBLE_ADD_POS_LIMIT	= 3;	
const int	CClear::FONT_WIDTH				= 400;
const int	CClear::FONT_HEIGHT				= 190 / 2;
const BYTE	CClear::FADEIN_OUT_SPEED		= 5;
const BYTE	CClear::FADEOUT_MIN				= 80;
const BYTE	CClear::NUMBER_SIZE				= 60;
const BYTE	CClear::GRAPHNUM_COLON			= 10;
const int	CClear::DEST_SCOREFONT_X		= WINDOW_WIDTH / 2 - FONT_WIDTH / 3 * 2;
const int	CClear::DEST_SCOREFONT_Y		= FONT_HEIGHT * 1.5f;
const int	CClear::DEST_NUMBER_X			= WINDOW_WIDTH / 2 - 20;
const int	CClear::DEST_TOTALSCORENUM_X	= WINDOW_WIDTH / 2 + 30;
const int	CClear::DEST_TOTALSCORENUM_Y	= DEST_TOTALSCORE_Y + 20;
const int	CClear::DEST_SOCRE_Y			= FONT_HEIGHT * 1.8f;
const int	CClear::DEST_CLEARTIME_Y		= FONT_HEIGHT * 2.8f;
const int	CClear::DEST_TOTALSCORE_Y		= DEST_SCOREFONT_Y + FONT_HEIGHT * 2.5f;

///////////////////////////////
// 実装
///////////////////////////////

// コンストラクタ
CClear::CClear( ULONG score, double clear_time ):
m_IsRun( true ),
m_BubbleSaveTime( 0 ),
m_Alpha( 0 ),
m_IsFadeIn( true ),
m_lpDolphin( NULL ),
m_Score( score ),
m_ClearSecond( static_cast<int>( clear_time ) )
{ 
}

// デストラクタ
CClear::~CClear()
{
	// 解放
	BOOST_FOREACH( CPointSpriteManager* It, m_PointBubbleList )
	{
		SAFE_DELETE( It );
	}

	m_PointBubbleList.clear();

	SAFE_DELETE( m_lpDolphin );

	Release();
}

// 読み込み
void CClear::FileLoad()
{
	Load();

	m_Sprite.Create( D3D::GetDevice() );

	CSoundManager::GetInst()->Play( "Sound/ClearBGM.wav", true );

	m_Fishes.SetNumAndLimit( 10, D3DXVECTOR3( 10, 2, 2 ), 0 );
	m_Fishes.SetRot( 180 );

	m_Fishes.SetMatrix( CMatrix( -25, 0, 0 ) );
	m_FishMoveTime = timeGetTime();

	// ライト設定
	D3DLIGHT9	Light;
	ZeroMemory( &Light, sizeof( Light ) );

	Light.Type	= D3DLIGHT_DIRECTIONAL;	// 平行光源
	Light.Diffuse.r	= 1.0f;
	Light.Diffuse.g	= 1.0f;
	Light.Diffuse.b	= 0.0f;

	Light.Position.x= 0.0f;
	Light.Position.y= 1.0f;
	Light.Position.z= 0.0f;

	Light.Range	= 10.f;

	Light.Direction = D3DXVECTOR3( 1.0f, -1.0f, 1.0f);

	D3D::GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	D3D::GetDevice()->SetRenderState( D3DRS_FOGENABLE, FALSE );
	D3D::GetDevice()->SetLight( 0, &Light );
	D3D::GetDevice()->LightEnable( 0, TRUE );

	CXfile::SetDrawShader( false );
	CBoard::SetDrawShader( false );

	// 時間を分と秒に分ける
	/*while( true )
	{
		if( m_ClearSecond >= 60 )
		{
			m_ClearMinit++;
			m_ClearSecond -= 60;
		
		}else{

			break;
		}
	}*/

	// イルカ作成
	/*m_lpDolphin = NEW Player::CDolphin();

	m_lpDolphin->SetMatrix( CMatrix( 0, 0, 0 ) );*/
}

// 実行関数
bool CClear::RunProc()
{
	Update();

	Draw();

	return m_IsRun;
}

// 更新
void CClear::Update()
{
	if( Input::Keyboard::IsState( DIK_Z, Input::PUSH ) )
	{
		m_IsRun = false;
	}

	// 条件満たせば
	if( timeGetTime() - m_BubbleSaveTime > BUBBLE_ADD_TIME )
	{
		// 泡作成
		CPointSpriteManager* lpTemp = NEW CPointSpriteManager();

		lpTemp->SetNum( BUBBLE_NUM );
		lpTemp->SetTexture( CSTexManager::GetInst()->GetData( TEX_BUBBLE )->GetTexture() );
		lpTemp->SetMove( D3DXVECTOR3( 1.f, -1.f, 1.f ) );
		
		D3DXVECTOR3 Pos(
			GetPosiOrNega() * BUBBLE_ADD_POS_LIMIT,
			GetPosiOrNega() * BUBBLE_ADD_POS_LIMIT,
			GetPosiOrNega() * BUBBLE_ADD_POS_LIMIT );

		lpTemp->SetPos( Pos );
		
		m_PointBubbleList.push_back( lpTemp );

		m_BubbleSaveTime = timeGetTime();	// 時間保存
	}

	// イルカ
	CVector Target = m_Target;

	CMatrix mInv = m_mDolphin;
	mInv.GetInverse();

	Target.GetTransCoord( &mInv );

	// 泡更新(途中削除があるので通常ループ)
	BUBBLELIST::iterator It = m_PointBubbleList.begin();

	while( It != m_PointBubbleList.end() )
	{
		// 更新
		(*It)->Update();

		// 削除するなら削除
		if( (*It)->IsErase() )
		{
			SAFE_DELETE( (*It) );
			It = m_PointBubbleList.erase( It );

			continue;
		}

		++It;
	}

	// ファードイン・アウト
	if( m_IsFadeIn )
	{
		if( (m_Alpha += FADEIN_OUT_SPEED) > 0xff )
		{
			m_Alpha = 0xff;
			m_IsFadeIn = false;
		}

	}else{

		if( (m_Alpha -= FADEIN_OUT_SPEED) < FADEOUT_MIN )
		{
			m_Alpha = FADEOUT_MIN;
			m_IsFadeIn = true;
		}
	}

	m_Fishes.Update( false );

	if( timeGetTime() - m_FishMoveTime > 10000 )
	{
		m_Fishes.SetRot( 180 );

		m_FishMoveTime = timeGetTime();
	}
}

// 描画
void CClear::Draw()
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// 描画開始
	D3D::Begin();

	// 画面クリア
	lpDevice->Clear( 0,	NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0x00, 0x00, 0xff ),
		1.f, 0 );

	// 投影
	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );

	// カメラ
	m_Camera.SetCamera(
		&D3DXVECTOR3( 0, 0, -10 ),
		&D3DXVECTOR3( 0, 0, 0 ),
		&D3DXVECTOR3( 0, 1, 0 ) );

	lpDevice->SetTransform( D3DTS_VIEW, m_Camera.GetMatrix() );

	// カリング材料登録
	SetCullingCamera( &m_Camera );
	SetCullingProj( &m_Proj );

	// ステージ
	lpDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	CMatrix mScale;

	mScale.SetScale( 5 );

	lpDevice->SetTransform( D3DTS_WORLD, &(mScale * CMatrix( 0, -20, 0 ) ) );

	CSXfileManager::GetInst()->GetData( PASS_BACK_GROUND_MESH )->Draw( lpDevice );

	// イルカ
	//lpDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	//m_lpDolphin->Draw( lpDevice );
	//lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	
	// 泡
	// レンダリング設定
	lpDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	lpDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	lpDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	lpDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );

	// 描画
	BOOST_FOREACH( CPointSpriteManager* It, m_PointBubbleList )
	{
		It->Draw( lpDevice );
	}

	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	lpDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	m_Fishes.DrawObjectFish( lpDevice );

	// スプライト描画開始
	m_Sprite.Begin();

	// フォント描画
	CTexture& FontTex = *CSTexManager::GetInst()->GetData( TEXPASS_FONT );

	// Scene
	m_Sprite.Draw(
		FontTex,
		0, 0, 0, 
		0, FONT_HEIGHT * FONTKIND_SCENE, 
		FONT_WIDTH, FONT_HEIGHT * (FONTKIND_SCENE + 1 ) );

	// clear
	m_Sprite.Draw(
		FontTex,
		WINDOW_WIDTH / 2, 0, 0, 
		0, FONT_HEIGHT * FONTKIND_CLEAR,
		FONT_WIDTH, FONT_HEIGHT * (FONTKIND_CLEAR + 1 ) );

	// Score
	m_Sprite.Draw(
		FontTex,
		DEST_SCOREFONT_X, DEST_SCOREFONT_Y, 0, 
		0, FONT_HEIGHT * FONTKIND_SCORE_AND_TIME,
		FONT_WIDTH / 2, FONT_HEIGHT * (FONTKIND_SCORE_AND_TIME + 1) );

	// Time
	m_Sprite.Draw(
		FontTex,
		DEST_SCOREFONT_X, DEST_SCOREFONT_Y + FONT_HEIGHT, 0, 
		FONT_WIDTH / 2, FONT_HEIGHT * FONTKIND_SCORE_AND_TIME,
		FONT_WIDTH, FONT_HEIGHT * (FONTKIND_SCORE_AND_TIME + 1) );

	// Total Score
	m_Sprite.Draw(
		FontTex,
		0, DEST_TOTALSCORE_Y, 0, 
		0, FONT_HEIGHT * FONTKIND_TOTALSCORE,
		FONT_WIDTH, FONT_HEIGHT * (FONTKIND_TOTALSCORE + 1) );

	// 次へ
	m_Sprite.SetAlpha( m_Alpha );

	m_Sprite.Draw(
		FontTex,
		WINDOW_WIDTH / 2 - FONT_WIDTH / 2, WINDOW_HEIGHT - FONT_HEIGHT * 1.5f, 0, 
		0, FONT_HEIGHT * FONTKIND_ZKEY,
		FONT_WIDTH, FONT_HEIGHT * (FONTKIND_ZKEY + 1) );

	m_Sprite.SetAlpha( 0xff );

	// 数字テクスチャ取得
	CTexture& NumTex = *CSTexManager::GetInst()->GetData( "number.png" );

	// スコア得点
	char Str[ 0xff ];

	// 文字列に変換
	sprintf_s( Str, sizeof( Str ), "%d", m_Score );

	for( BYTE i = 0, Num = 0; Str[ i ] != '\0'; i++ )
	{
		// 文字列を数字に変換
		Num = Str[ i ] - '0';

		// 描画
		m_Sprite.Draw(
			NumTex,
			DEST_NUMBER_X + NUMBER_SIZE * i, DEST_SOCRE_Y, 0,
			NUMBER_SIZE * Num, 0, NUMBER_SIZE * (Num + 1), NUMBER_SIZE );
	}

	// クリア時間
	sprintf_s( Str, sizeof( Str ), "%d %02d", m_ClearSecond / 60, m_ClearSecond % 60 );

	for( BYTE i = 0, Num = 0; Str[ i ] != '\0'; i++ )
	{
		// 文字列を数字に変換
		Num = Str[ i ] - '0';

		// 数字以外なら記号を描画
		if( Num < 0 || Num > 9 )
		{
			Num = GRAPHNUM_COLON;
		}
		
		m_Sprite.Draw(
			NumTex,
			DEST_NUMBER_X + NUMBER_SIZE * i, DEST_CLEARTIME_Y, 0,
			NUMBER_SIZE * Num, 0, NUMBER_SIZE * (Num + 1), NUMBER_SIZE );
	}

	// トータルスコア
	sprintf_s( Str, sizeof( Str ), "%d", m_Score * 10 - m_ClearSecond );

	for( BYTE i = 0, Num = 0; Str[ i ] != '\0'; i++ )
	{
		// 文字列を数字に変換
		Num = Str[ i ] - '0';

		// 数字以外なら記号を描画
		if( Num < 0 || Num > 9 )
		{
			Num = GRAPHNUM_COLON;
		}
		
		m_Sprite.Draw(
			NumTex,
			DEST_TOTALSCORENUM_X + NUMBER_SIZE * i, DEST_TOTALSCORENUM_Y, 0,
			NUMBER_SIZE * Num, 0, NUMBER_SIZE * (Num + 1), NUMBER_SIZE );
	}

	// スプライト描画終了
	m_Sprite.End();

	// 描画終了
	D3D::End();
}

// 次のシーケンス
IBase* CClear::GetNext()
{
	return NEW CTitle();
}

// 読み込み
void CClear::Load()
{
	// テクスチャ読み込みリストから読み込み
	CSTexManager::GetInst()->LoadFromTextList( D3D::GetDevice(), "LoadData/Clear/Tex.txt" );

	// Xファイル読み込みリストから読み込み
	CSXfileManager::GetInst()->LoadFromTextList( D3D::GetDevice(), "LoadData/Clear/Xdata.txt" );
}

// 解放
void CClear::Release()
{
	CSTexManager::GetInst()->ReleaseFromTextList( "LoadData/Clear/Tex.txt" );

	CSXfileManager::GetInst()->ReleaseFromTextList( "LoadData/Clear/Xdata.txt" );

	CSoundManager::GetInst()->Stop( "Sound/ClearBGM.wav" );
}