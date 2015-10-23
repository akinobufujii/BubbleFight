//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
//#include "Dispatch.h"
#include "Bubble.h"
#include "BubbleManager.h"

#include "RippleManager.h"
#include "Fintail.h"
#include "ShotBase.h"
#include "NormalShot.h"
#include "BuckShot.h"
#include "ShotManager.h"

#include "EnemyBase.h"
#include "EnemyManager.h"

#include "BossBase.h"

#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "UI.h"
#include "GameMain.h"

#include "SandSpray.h"

#include "StrongDolphin.h"

//////////////////////////////////////////////////
// 名前空間省略
//////////////////////////////////////////////////
using namespace Player;
using namespace Input;

//////////////////////////////////////////////////
// 静的メンバ変数実体化
//////////////////////////////////////////////////
const BYTE		CDolphin::AFTER_IMAGE_MAX	= 5;

const CDolphin::FLGTYPE	CDolphin::FLG_RIGHT_EVASION	= 0x0001;
const CDolphin::FLGTYPE	CDolphin::FLG_OVER			= 0x0002;
const CDolphin::FLGTYPE	CDolphin::FLG_LEFT_EVASION	= 0x0004;
const CDolphin::FLGTYPE	CDolphin::FLG_SHOT			= 0x0008;
const CDolphin::FLGTYPE	CDolphin::FLG_EVASION		= 0x0010;
const CDolphin::FLGTYPE	CDolphin::FLG_ATTACK		= 0x0020;
const CDolphin::FLGTYPE	CDolphin::FLG_ATTACK_OK		= 0x0040;
const CDolphin::FLGTYPE	CDolphin::FLG_DAMAGE		= 0x0080;
const CDolphin::FLGTYPE	CDolphin::FLG_HIT_STONE		= 0x0100;
const CDolphin::FLGTYPE	CDolphin::FLG_JUMP			= 0x0200;
const CDolphin::FLGTYPE	CDolphin::FLG_CHARGE		= 0x0400;
const CDolphin::FLGTYPE	CDolphin::FLG_CHARGEMAX		= 0x0800;
const CDolphin::FLGTYPE	CDolphin::FLG_DRAW_CHARGE	= 0x1000;
const CDolphin::FLGTYPE	CDolphin::FLG_MAX_EMISSIVE	= 0x2000;
const CDolphin::FLGTYPE	CDolphin::FLG_DIE			= 0x4000;

const float		CDolphin::SPEED						= 0.02f;
const float		CDolphin::SPEED_MAX					= 0.7f;
const float		CDolphin::POWERCHARGE_SPEED			= 1.f;
const float		CDolphin::EVASION_MOVE_SPEED		= 0.3f;
const float		CDolphin::ATTACK_SPEED				= 360.f / CFPS::FPS;
const float		CDolphin::HITPOWER_PLUS				= 0.3f;
const float		CDolphin::POINTER_ANIME_RATE		= 1.f / CDolphin::POINTER_ANIME_NUM;
const float		CDolphin::RECOVERY_SPEED_LINE		= SPEED_MAX - SPEED_MAX * 0.1;
const float		CDolphin::CHARGE_ANIME_RATE			= 1.f / CDolphin::CHARGE_ANIME_NUM;
const float		CDolphin::CHARGEMAX_SCALE			= 1.15f;
const float		CDolphin::CHARGESCALE_RATE			= (CHARGEMAX_SCALE - 1) / POWER_MAX;
const float		CDolphin::FLOAT_SPEED				= 0.002f;
const short		CDolphin::LEARROT_MAX				= 180;
const USHORT	CDolphin::BLESS_LOST_TIME			= 3000;	
const USHORT	CDolphin::HP_MAX					= 100;
const USHORT	CDolphin::ATTACK_ROT_BEGIN			= 150;
const USHORT	CDolphin::ATTACK_ROT_END			= 190;
const USHORT	CDolphin::EVASION_EFFECTERACE_TIME	= 1000;
const BYTE		CDolphin::MOVE_BUBBLE_MAX			= 1;
const BYTE		CDolphin::ROT_MAX					= 2;
const BYTE		CDolphin::LEARROT_SPEED				= 2;
const BYTE		CDolphin::POWER_MAX					= 100;
const BYTE		CDolphin::BLESS_MAX					= 100;
const BYTE		CDolphin::SHOTLOST_BLESS_RATE		= 10;
const BYTE		CDolphin::EVASION_ROT_SPEED			= 15;
const BYTE		CDolphin::OVERRUN_ROT				= 5;
const BYTE		CDolphin::RETURN_POS				= Sequence::CGameMain::SEA_MOVELIMIT - 20;
const BYTE		CDolphin::POINTER_ANIME_NUM			= 8;
const BYTE		CDolphin::CHARGE_ANIME_NUM			= 7;
const BYTE		CDolphin::REFORM_SPEED				= 3;
const BYTE		CDolphin::DIE_ROT_SPEED				= 5;
const BYTE		CDolphin::RECOVERYBLESS_SPEED		= 2;

/////////////////////////////////
// 実装
/////////////////////////////////

// コンストラクタ
CDolphin::CDolphin():
m_lpmAfter( NEW CMatrix[ AFTER_IMAGE_MAX ] ),
//m_lpSandSplay( NEW CSandSpray() ),
m_lpMesh( CSXfileManager::GetInst()->GetData( "Dolphin.x" ) ),
m_HP( HP_MAX ),
m_Bless( BLESS_MAX ),
m_Flg( FLG_JUMP ),
m_RotCnt( 0 ),
m_Power( 0.f ),
m_NowSpeed( SPEED_MAX - SPEED ),
m_NowRotSpeed( 0.f ),
m_HitPower( 0.f ),
m_SaveBlessTime( 0 ),
m_lpStones( NULL ),
m_PointerAnime( 0 ),
m_PointerSaveTime( 0 ),
m_NowPlayerScale( 1 ),
m_NowFloatPower( 0 ),
m_mMat( 0, Sequence::CGameMain::SURFACESEA_HEIGHT + 1, 0 )
{
	// 傾ける
	m_mMat.RotateLocalX( -80 );

	// 残像初期化
	for( int i = 0; i < AFTER_IMAGE_MAX; i++ )
	{
		m_lpmAfter[ i ] = m_mMat;
	}
	
	// フィンの設定
	m_Fin.SetMesh( CSXfileManager::GetInst()->GetData( "CharaFin.x" ) );
	m_Fin.SetMove( CFinTail::MOVE_TOP );
	m_Fin.SetFarPos( &D3DXVECTOR3( 0, 0, 0 ) );
	m_Fin.SetRotMax( 15 );

	// 波紋の設定
	m_Ripples.SetCreateHeight( m_lpMesh->GetRadius() );
	m_Ripples.SetDefaultTexture();
	m_Ripples.SetSize( m_lpMesh->GetRadius(), m_lpMesh->GetRadius() + 1.f );

	m_Spray.SetNum( 150 );
	m_Spray.SetTexture( CSTexManager::GetInst()->GetData( "Spray.png" )->GetTexture() );
	m_Spray.SetEraseTime( 1000 );
	m_Spray.SetPos( D3DXVECTOR3( 100, 100, 100 ) );

	// ポインタ設定
	m_Pointer.SetVertex( m_lpMesh->GetRadius(), 10.f );

	// チャージアニメーション
	m_ChargeEffect.SetVertex( m_lpMesh->GetRadius() * 4, m_lpMesh->GetRadius() * 2 );

	m_Evasion.SetTexture( CSTexManager::GetInst()->GetData( "Spray.png" )->GetTexture() );

	m_ChargeMaxAnime.SetAnimeSize( 512, 1024, 512, 147 );
	m_ChargeMaxAnime.SetAnimeNum( CHARGE_ANIME_NUM, 1, CHARGE_ANIME_NUM );
	m_ChargeMaxAnime.SetAnimetionType( false );
	m_ChargeMaxAnime.SetLoop();
	m_ChargeMaxAnime.SetTime( 40 );

	m_ChargeAnime.SetAnimeSize( 1024, 1024, 205, 205 );
	m_ChargeAnime.SetAnimeNum( 25, 5, 5 );
	m_ChargeAnime.SetLoop();
	m_ChargeAnime.SetTime( 20 );

	// マウス初期位置設定
	RECT Rect;
	GetWindowRect( GetDesktopWindow(), &Rect );
	SetCursorPos( Rect.right / 2, Rect.bottom / 2 );
	GetCursorPos( &m_SaveMousePos );
}

// 仮想デストラクタ
CDolphin::~CDolphin()
{
	// 動的配列解放
	SAFE_DELETE_ARRAY( m_lpmAfter );
	//SAFE_DELETE( m_lpSandSplay );
}

// 更新
void CDolphin::Update()
{
#ifdef _DEBUG
	
	if( Input::Keyboard::IsState( DIK_Q, Input::PUSH  ) )
	{
		m_Flg |= FLG_DIE;
	}
#endif

	// ダメージ受けたことを消す
	m_Flg &= ~(FLG_DAMAGE | FLG_HIT_STONE);

	// 死んでるかどうか判断する
	if( m_HP <= 0 )
	{
		m_Flg |= FLG_DIE;
	}

	// 死んでいたら
	if( m_Flg & FLG_DIE )
	{
		UpdateDie();
	
	}else{
	
		if( m_Flg & FLG_OVER )
		{
			UpdateOverRun();

		}else if( m_Flg & FLG_JUMP ){

			UpdateSurfaceSea();

		}else{

			// キー更新
			UpdateKey();

			// 動き更新
			UpdateMove();

			// パワー更新
			UpdatePowerAndShot();

			// 行動更新
			UpdateAction();
		}
	}
	
	// 息
	UpdateBless();

	// 弾
	m_Shots.Update();

	// 泡
	m_Bubbles.Update();

	// 尾ひれ
	m_Fin.Update( m_mMat, (m_Flg & FLG_JUMP)? 0 : m_NowSpeed );

	// 波紋
	m_Ripples.Update( m_mMat );

	// 水しぶき
	m_Spray.Update();

	// 砂しぶき
	//m_lpSandSplay->Update();

	// エフェクト更新
	m_Evasion.Update();

	//// ポインター
	//UpdatePointer();

	//// チャージ
	//UpdateCharge();
}

// 描画
void CDolphin::Draw( const LPDIRECT3DDEVICE9 lpDevice )
{
	// 自機描画
	DrawPlayer( lpDevice );

	// 泡描画
	m_Bubbles.DrawNoBreak( lpDevice );

	// 弾描画
	m_Shots.Draw( lpDevice );
}

// エフェクト描画
void CDolphin::DrawEffect( const LPDIRECT3DDEVICE9 lpdevice )
{
	//m_lpSandSplay->Draw( lpdevice );
	m_Bubbles.DrawBreak( lpdevice );
	m_Shots.DrawEffect( lpdevice );

	// 特定の状態じゃなければ描画
	if( ( m_Flg & (FLG_OVER | FLG_JUMP) ) == 0 )
	{
		//if( m_Flg & FLG_CHARGE )
		//{
		//	// ポインタ
		//	CMatrix mRotX, mTrans( 0, 0, 10 );

		//	mRotX.RotateLocalX( 90 );

		//	lpdevice->SetFVF( CBoard::FVF_VERTEX );
		//	lpdevice->SetTransform( D3DTS_WORLD, &(mRotX * mTrans * m_mMat) );
		//	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		//	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		//	lpdevice->SetTexture( 0, CSTexManager::GetInst()->GetData( "Pointer.png" )->GetTexture() );
		//	lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );

		//	m_Pointer.Draw( lpdevice );
		//}

		// チャージ中
		/*if( m_Flg & FLG_DRAW_CHARGE )
		{
			lpdevice->SetFVF( CBoard::FVF_VERTEX );
			lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
			lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

			if( m_Flg & FLG_CHARGEMAX )
			{
				lpdevice->SetTexture( 0, CSTexManager::GetInst()->GetData( "Charge.png" )->GetTexture() );
			}else{

				lpdevice->SetTexture( 0, CSTexManager::GetInst()->GetData( "Gather.png" )->GetTexture() );
			}

			lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );
			lpdevice->SetRenderState( D3DRS_ZENABLE, FALSE );

			m_ChargeEffect.Draw( lpdevice );

			lpdevice->SetRenderState( D3DRS_ZENABLE, TRUE );
			lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		}*/
	}
	
	// 回避エフェクト
	lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	m_Evasion.Draw( lpdevice );

	if( m_Spray.IsErase() == false )
	{
		m_Spray.Draw( lpdevice );
	}

	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

// 動き更新
void CDolphin::UpdateMove()
{
	// スピード減少
	if( m_NowSpeed != 0.f )
	{
		m_NowSpeed -= (m_NowSpeed > 0)? SPEED * 0.5f : -SPEED * 0.5f;

		// 渡されたスピードの半分を減少
		if( m_NowSpeed < SPEED * 0.5f && m_NowSpeed > -SPEED * 0.5f )
		{
			m_NowSpeed = 0.f;
		}
	}

	// 座標獲得
	D3DXVECTOR3 vPos = m_mMat.GetPos();

	// 海面、海底の座標制限
	if( vPos.y > Sequence::CGameMain::SURFACESEA_HEIGHT )
	{
		// もし、スピードMAXで海面に浮上したら
		if( m_NowSpeed >= RECOVERY_SPEED_LINE )
		{
			// ある程度の角度であれば
			//if( m_mMat.GetRotX() < -45 )
			{
				CSoundManager::GetInst()->Play( "Sound/JumpOut.wav", false, true );
				m_Flg |= FLG_JUMP;
				m_Fin.ResetCnt();
			
			}
		/*else{

				m_mMat._42 = Sequence::CGameMain::SURFACESEA_HEIGHT;
			}*/

		}else{

			m_mMat._42 = Sequence::CGameMain::SURFACESEA_HEIGHT;
		}
		
	}else if( vPos.y < Sequence::CGameMain::BOTTOMSEA_HEIGHT + m_lpMesh->GetRadius() ){

		m_mMat._42 = Sequence::CGameMain::BOTTOMSEA_HEIGHT + m_lpMesh->GetRadius();
	}

	// 空気回復
	if( m_mMat._42 > Sequence::CGameMain::SURFACESEA_HEIGHT - 1 )
	{
		if( ++m_Bless > BLESS_MAX )
		{
			m_Bless = BLESS_MAX;
		}
	}
}

// 移動
void CDolphin::UpdateKey()
{
	CMatrix mTrans, mRotY, mRotX;

	// マウス座標獲得
	POINT TempMouse;
	GetCursorPos( &TempMouse );

	// キー操作出来る状態なら
	if( !( m_Flg & (FLG_ATTACK | FLG_EVASION) ) )
	{
		// アクセル
		if( Keyboard::IsState( DIK_Z, HOLD | PUSH ) )
		{
			m_NowSpeed += SPEED;

			CSoundManager::GetInst()->Play( "Sound/Move.wav", false, true );

			// 速さ上限
			if( m_NowSpeed > SPEED_MAX || m_NowSpeed < -SPEED_MAX )
			{
				m_NowSpeed = (m_NowSpeed < -SPEED_MAX)? -SPEED_MAX : SPEED_MAX;
			}
		}

		// 右方向に回避
		if( Keyboard::IsState( DIK_X, PUSH ) )
		{
			m_mSave		= m_lpmAfter[ 0 ];
			m_RotCnt	= 0;

			// 回避方向決定
			//m_EvasionVec.Set( -1, 0, 0 );
			m_EvasionVec.Set( 0, 0, 1 );

			m_EvasionVec.GetTransNormal( &m_mSave );
			m_EvasionVec *= EVASION_MOVE_SPEED;

			m_Flg |= FLG_RIGHT_EVASION | FLG_EVASION;

			m_Evasion.SetNum( 100 );
			m_Evasion.SetMove( D3DXVECTOR3( 0.5f, -1, 0.5f ) );
			m_Evasion.SetEraseTime( EVASION_EFFECTERACE_TIME );
			m_Evasion.SetPos( m_mMat.GetPos() );
			m_Evasion.SetScaleMax( 3.f );

			CSoundManager::GetInst()->Play( "Sound/Evasion.wav" );
		}

		//// 左方向に回避
		//if( Keyboard::IsState( DIK_C, PUSH ) )
		//{
		//	m_mSave		= m_lpmAfter[ 0 ];
		//	m_RotCnt	= 0;

		//	// 回避方向決定
		//	//m_EvasionVec.Set( 1, 0, 0 );
		//	m_EvasionVec.Set( 0, 0, 1 );

		//	m_EvasionVec.GetTransNormal( &m_mSave );
		//	m_EvasionVec *= EVASION_MOVE_SPEED;

		//	m_Flg |= FLG_RIGHT_EVASION | FLG_EVASION;

		//	m_Evasion.SetNum( 100 );
		//	m_Evasion.SetMove( D3DXVECTOR3( 0.5f, -1, 0.5f ) );
		//	m_Evasion.SetEraseTime( EVASION_EFFECTERACE_TIME );
		//	m_Evasion.SetPos( m_mMat.GetPos() );
		//	m_Evasion.SetScaleMax( 3.f );

		//	CSoundManager::GetInst()->Play( "Sound/Evasion.wav" );
		//}

		// 攻撃
		if( Input::Keyboard::IsState( DIK_C, Input::PUSH ) &&
			m_mMat.GetPos().y < Sequence::CGameMain::SURFACESEA_HEIGHT - 2 )
		{
			m_mSave		=	m_lpmAfter[ 0 ];
			m_RotCnt	=	0;
			//m_NowSpeed	=	0;
			m_Flg		|=	FLG_ATTACK;

			// 方向を計算する
			m_AttackMoveDir.Set( 0, 0, 1 );

			m_AttackMoveDir.GetTransNormal( &m_mMat );
		}

		// 移動数保存
		float MouseMoveX = TempMouse.x - m_SaveMousePos.x;
		float MouseMoveY = TempMouse.y - m_SaveMousePos.y;

		if( abs( MouseMoveX ) > ROT_MAX )
		{
			MouseMoveX = (MouseMoveX > 0)? ROT_MAX : -ROT_MAX;
		}

		// 上限を決める
		if( abs( MouseMoveY ) > ROT_MAX )
		{
			MouseMoveY = (MouseMoveY > 0)? ROT_MAX : -ROT_MAX;
		}

		// 姿勢を正す
		ReformPose();

		// 回転
		mRotY.SetRotY( MouseMoveX );
		mRotX.SetRotX( MouseMoveY );

#ifdef _DEBUG
		char Str[ 0xff ];

		sprintf_s( Str, sizeof( Str ), "X = %f  Y = %f Z = %f\n" ,m_mMat._41 ,m_mMat._42, m_mMat._43 );

		//OutputDebugStr( Str );
#endif

		// 泡を生み出す数を決める
		int CreateBubbleNum = abs( MouseMoveX + MouseMoveY );

		if( CreateBubbleNum > MOVE_BUBBLE_MAX )
		{
			CreateBubbleNum = MOVE_BUBBLE_MAX;
		}

		for( int i = CreateBubbleNum; i > 0; i-- )
		{
			m_Bubbles.Add( &m_Fin.GetMatrix(), 1 );
		}
	}

	// 移動時に、岩との衝突判定をする
	if( !(m_Flg & FLG_ATTACK) )
	{
		UpdateCollisionMove( &D3DXVECTOR3( 0, 0, m_NowSpeed ) );
	}
	// 吹っ飛び更新
	// 岩との判定もする
	CVector Vec		= m_MoveVec * m_HitPower;
	//float	Length	= Vec.GetLength();
	UpdateCollisionMove( &Vec );
	

	// 吹っ飛び率減少
	if( (m_HitPower -= 0.1f) < 0 )
	{
		m_HitPower = 0.f;
	}

	if( !(m_Flg & FLG_HIT_STONE) )
	{
		UpdateCollisionMove( &Vec );	
	}

	RECT Rect;
	GetWindowRect( GetDesktopWindow(), &Rect );
	SetCursorPos( Rect.right / 2, Rect.bottom / 2 );

	// 合成
	m_mMat = mRotX * mRotY * mTrans * m_mMat;

	// ステージからはみ出たら
	CVector Pos = m_mMat;
	
	if( abs( Pos.x ) > Sequence::CGameMain::SEA_MOVELIMIT ||
		abs( Pos.z ) > Sequence::CGameMain::SEA_MOVELIMIT )
	{
		// フラグを立てる
		m_Flg |= FLG_OVER;

		CVector Pos = m_mMat;

		if( abs( Pos.x ) > Sequence::CGameMain::SEA_MOVELIMIT )
		{
			Pos.x = ( Pos.x > 0 )? RETURN_POS : -RETURN_POS;
		}

		if( abs( Pos.z ) > Sequence::CGameMain::SEA_MOVELIMIT )
		{
			Pos.z = ( Pos.z > 0 )? RETURN_POS : -RETURN_POS;
		}

		m_mSave = Pos;
	}

	// 残像格納
	for( int i = AFTER_IMAGE_MAX - 2; i >= 0; i-- )
	{
		m_lpmAfter[ i + 1 ] = m_lpmAfter[ i ];
	}

	// 状態によって保存する行列を変える
	if( !( m_Flg & (FLG_ATTACK | FLG_EVASION | FLG_ATTACK) ) )
	{
		m_lpmAfter[ 0 ] = m_mMat;
	
	}else{

		m_lpmAfter[ 0 ] = m_mSave;
	}
}

// 行動更新
void CDolphin::UpdateAction()
{
	if( m_Flg & FLG_EVASION )		// 回避中なら
	{
		UpdateEvasion();
	
	}else if( m_Flg & FLG_ATTACK ){	// 攻撃中なら

		UpdateAttack();
	}
}

// 回転移動更新
void CDolphin::UpdateEvasion()
{
	CMatrix mTrans( m_EvasionVec );

	if( m_Flg & FLG_RIGHT_EVASION )			// 右に回避
	{
		m_mMat.RotateLocalZ( -EVASION_ROT_SPEED );
	
	}else if( m_Flg & FLG_LEFT_EVASION ){	// 左に回避
		
		m_mMat.RotateLocalZ( EVASION_ROT_SPEED );
	}

	// 向いている方向に横に動かす
	CVector Dir = mTrans.GetPos();
	CMatrix	mInv = m_mMat;

	mInv.GetInverse();
	Dir.GetTransNormal( &mInv );

	UpdateCollisionMove( &Dir );	

	// 直書き(直すべき)
	m_mSave._41 = m_mMat._41;
	m_mSave._42 = m_mMat._42;
	m_mSave._43 = m_mMat._43;

	// 一回転すれば終了
	if( (m_RotCnt += EVASION_ROT_SPEED) >= 360 )
	{
		m_Flg &= ~(FLG_RIGHT_EVASION | FLG_LEFT_EVASION | FLG_EVASION);
	}
}

// 攻撃更新
void CDolphin::UpdateAttack()
{
	// 1回転したら終了
	if( (m_RotCnt += ATTACK_SPEED) > 360.f )
	{
		m_mMat.RotateLocalX( ATTACK_SPEED - (m_RotCnt - 360.f) );
		m_Flg &= ~FLG_ATTACK;

	}else{

		m_mMat.RotateLocalX( ATTACK_SPEED );
	}

	// 指定角度内なら攻撃判定可能
	if( m_RotCnt > ATTACK_ROT_BEGIN && m_RotCnt < ATTACK_ROT_END )
	{
		m_Flg |= FLG_ATTACK_OK;

	}else{

		m_Flg &= ~FLG_ATTACK_OK;
	}

	// 進ませる(直書き)
	D3DXVECTOR3 Temp = m_AttackMoveDir * m_NowSpeed;

	m_mSave._41 = m_mMat._41 += Temp.x;
	m_mSave._42 = m_mMat._42 += Temp.y;
	m_mSave._43 = m_mMat._43 += Temp.z;
}

// パワーチャージ更新
void CDolphin::UpdatePowerAndShot()
{
	// まだ息が続いているなら
	if( m_Bless > 0 )
	{
		// パワーを貯める
		//if( Keyboard::IsState( DIK_X, PUSH ) )
		if( (m_Flg & FLG_CHARGE) == 0 && GetAsyncKeyState( VK_LBUTTON ) )
		{
			m_Flg |= FLG_CHARGE;
			m_PointerAnime = 0;
			m_PointerSaveTime = timeGetTime();
			CSoundManager::GetInst()->Play( "Sound/charge.wav", true );

			m_ChargeAnime.Reset();

		}else if( (m_Flg & FLG_CHARGE) && !GetAsyncKeyState( VK_LBUTTON ) ){

			// 離された瞬間に撃つ
			CSoundManager::GetInst()->Stop( "Sound/ChargeMax.wav" );
			CSoundManager::GetInst()->Stop( "Sound/charge.wav" );
			m_Flg |= FLG_SHOT;
			m_Flg &= ~(FLG_CHARGE | FLG_CHARGEMAX | FLG_DRAW_CHARGE);
			m_NowPlayerScale = 1;
		}

		// チャージ処理
		if( m_Flg & FLG_CHARGE )
		{
			// 大きくする
			m_NowPlayerScale += CHARGESCALE_RATE;

			// もう息が尽きたなら
			if( m_Bless <= 0 )
			{
				CSoundManager::GetInst()->Stop( "Sound/ChargeMax.wav" );
				CSoundManager::GetInst()->Stop( "Sound/charge.wav" );
				m_Flg |= FLG_SHOT;
				m_Flg &= ~(FLG_CHARGE | FLG_CHARGEMAX | FLG_DRAW_CHARGE);
				m_NowPlayerScale = 1;
			}

			// 最大溜めなら
			if(	(m_Power += POWERCHARGE_SPEED) > POWER_MAX )
			{
				CSoundManager::GetInst()->Stop( "Sound/charge.wav" );
				CSoundManager::GetInst()->Play( "Sound/ChargeMax.wav", true, true );
				m_Power				= POWER_MAX;

				m_NowPlayerScale = CHARGEMAX_SCALE;

				if( !(m_Flg & FLG_CHARGEMAX) )
				{
					m_Flg |= FLG_CHARGEMAX;
					m_ChargeMaxAnime.Reset();
					m_EmissiveSaveTime = timeGetTime();
				}
			}
		}
	}

	// 弾処理
	if( m_Flg & FLG_SHOT )
	{
		float PowerResult = Shot::CNormal::POWER_MAX / POWER_MAX * m_Power;

		// 込める
		m_Shots.Add( Shot::CManager::BUBBLERING, &m_mMat, PowerResult );

		long Pan = CSoundManager::GetInst()->GetPan();
		
		CSoundManager::GetInst()->Play( "Sound/BubbleRing.wav" );
		
		// 息の消費処理
		if( m_Power / SHOTLOST_BLESS_RATE < 0 )
		{
			if( --m_Bless < 0 )
			{
				m_Bless = 0;
			}
		
		}else{

			if( (m_Bless -= m_Power / SHOTLOST_BLESS_RATE) < 0 )
			{
				m_Bless = 0;
			}
		}

		m_Flg &= ~FLG_SHOT;
		m_Power = 0.f;
	}

	// チャージしていない時はしぼませる
	if( !(m_Flg & FLG_CHARGE) )
	{
		// 小さくしていく
		if( (m_NowPlayerScale -= CHARGESCALE_RATE) < 1.f )
		{
			m_NowPlayerScale = 1.f;
		}
	}
}

// 範囲超えた時の更新
void CDolphin::UpdateOverRun()
{
	// 逆行列を作る
	CMatrix mInv = m_mMat;

	mInv.GetInverse();

	// 目標方向を決定する
	CVector Target( 0, 0, 0 );
	
	Target.GetTransCoord( &mInv );
	Target.GetNormalVector();

	// 回転軸を求める
	CVector RotAxis( 0, 0, 1 );

	RotAxis.GetCross( &Target );

	// 角度を求める
	CVector Vec( 0, 0, 1 );
	float Degree = Vec.GetDotDegree( &Target );

	// 実際に回転
	if( Degree > OVERRUN_ROT )
	{
		Degree = OVERRUN_ROT;
	}

	m_mMat.RotateLocalAxis( &RotAxis, Degree );

	// 進む
	UpdateCollisionMove( &D3DXVECTOR3( 0, 0, SPEED_MAX ) );

	// 姿勢を正す
	ReformPose();

	// 位置確認
	CVector Pos = m_mMat;

	if( Degree	< OVERRUN_ROT		&&
		abs( Pos.x ) < RETURN_POS	&&
		abs( Pos.z ) < RETURN_POS	)
	{
		m_Flg &= ~FLG_OVER;

		// 残像をリセットする
		for( int i = 0; i < AFTER_IMAGE_MAX; i++ )
		{
			m_lpmAfter[ i ] = m_mMat;
		}
	}
}

// 海面から出た時の処理
void CDolphin::UpdateSurfaceSea()
{
	// 移動
	UpdateCollisionMove( &D3DXVECTOR3( 0, 0, m_NowSpeed ) );

	// 定速だが重力
	CMatrix mGravity( 0, -0.1f, 0 );

	m_mMat *= mGravity;

	// 姿勢を正す
	ReformPose();

	m_NowSpeed += SPEED;

	// X軸を徐々に回転
	m_mMat.RotateLocalX( 3.5f );

	// 息も回復
	RecoveryBless();

	// 終了判定
	if( m_mMat.GetPos().y < Sequence::CGameMain::SURFACESEA_HEIGHT )
	{
		// 水しぶきの出す位置を決定する
		CVector Pos = m_mMat;
		CVector Dir( 0, 0, 1 );

		Dir.GetTransNormal( &m_mMat );

		Pos += Dir * 5;

		// 水に勢い良く入った時の水しぶき
		m_Spray.SetEraseTime( 1000 );
		m_Spray.SetMove( D3DXVECTOR3( 1, 1, 1 ) );
		m_Spray.SetPos( Pos );

		m_NowSpeed = SPEED_MAX;
		m_Flg &= ~FLG_JUMP;

		CSoundManager::GetInst()->Play( "Sound/JumpIn.wav", false, true );
	}

	// 残像格納
	for( int i = AFTER_IMAGE_MAX - 2; i >= 0; i-- )
	{
		m_lpmAfter[ i + 1 ] = m_lpmAfter[ i ];
	}

	m_lpmAfter[ 0 ] = m_mMat;
}

// 尾ひれの方向を返す
D3DXVECTOR3 CDolphin::GetFinDir() const
{
	// 尾ひれの行列から座標を入手する
	D3DXVECTOR3 vFinPos;
	
	D3DXVec3TransformCoord( &vFinPos, &D3DXVECTOR3( 0, 0, 0 ), &m_Fin.GetMatrix() );
	
	// 引き算して尾ひれの方向ベクトルを算出する
	D3DXVECTOR3 Vec = m_mMat.GetPos() - vFinPos;

	// 正規化
	D3DXVec3Normalize( &Vec, &Vec );

	return Vec;
}

// 息更新
void CDolphin::UpdateBless()
{
	// 一定時間超えれば
	if( timeGetTime() - m_SaveBlessTime > BLESS_LOST_TIME )
	{
		m_SaveBlessTime = timeGetTime();

		if( --m_Bless < 0 )
		{
			m_Bless = 0;
		}
	}
}

// 敵に対する行動
void CDolphin::Action(Enemy::CBase& object)
{
	if( m_Flg & FLG_EVASION )
	{
		return;
	}

	m_Flg |= FLG_DAMAGE;

	CSoundManager::GetInst()->Play( "Sound/Damage.wav" );

	// ダメージ
	if( --m_HP < 0 )
	{
		m_HP = 0;
	}

	// 吹き飛ぶ方向を計算する
	m_HitPower = HITPOWER_PLUS;
	m_MoveVec = m_mMat.GetPos() - object.GetPos();

	m_MoveVec.GetNormalVector();
}

// 弾に対する行動
void CDolphin::Action(Shot::CBase& object)
{
	if( m_Flg & FLG_EVASION )
	{
		return;
	}

	// 破裂していなくて、泡につつまれていなければ
	if( !(object.GetFlg() & (Shot::CBase::FLG_BREAK | Shot::CBase::FLG_BUBBLE_WRAP)) )
	{
		m_Flg |= FLG_DAMAGE;

		CSoundManager::GetInst()->Play( "Sound/Damage.wav" );

		if( --m_HP < 0 )
		{
			m_HP = 0;
		}
	}
}

// ボスに対する行動
void CDolphin::Action( Boss::CBase& object )
{
	// 回避中なら何もしない
	if( m_Flg & FLG_EVASION )
	{
		return;
	}

	m_Flg |= FLG_DAMAGE;

	CSoundManager::GetInst()->Play( "Sound/Damage.wav" );

	// ダメージ
	if( --m_HP < 0 )
	{
		m_HP = 0;
	}

	// 吹き飛ぶ方向を計算する
	m_HitPower += HITPOWER_PLUS;
	m_MoveVec = m_mMat.GetPos(  ) - object.GetPos();

	m_MoveVec.GetNormalVector();
}

// 岩との衝突判定をしながら移動
bool CDolphin::UpdateCollisionMove( const D3DXVECTOR3* lp_move, bool is_move )
{
	if( m_lpStones == NULL )
	{
		return false;
	}

	// 長さと方向を抽出
	float Length = D3DXVec3Length( lp_move );

	CVector Direction;
	D3DXVec3Normalize( &Direction, lp_move );

	// 自機の方向を調べる
	Direction.GetTransNormal( &m_mMat );

	bool IsMove = true;

	CMatrix mScale;

	DWORD PolyNo;	// 当たったポリゴンナンバー

	// 岩との衝突判定
	BOOST_FOREACH( CStoneManager::STONEDATA* Stone, *m_lpStones->GetList() )
	{
		mScale.SetScale( Stone->Scale );

		// 判定
		float Distance = IntersectByMesh( 
			&m_mMat,
			&Direction,
			&( mScale * CMatrix( Stone->Pos ) ),
			*Stone->lpMesh->GetMeshData(),
			&PolyNo );

		// 応答
		if( Distance > 0 && Distance - Length < (m_lpMesh->GetRadius() + 1) )
		{
			// 今の前方方向を調べる
			CVector	NowFront( 0, 0, 1 );

			NowFront.GetTransNormal( &m_mMat );

			// 砂しぶき追加
			//m_lpSandSplay->AddDefault( m_mMat.GetPos() + NowFront * Distance );

			// 滑りベクトル計算
			D3DXVECTOR3 SlipVec = 
				GetSlipVec( NowFront, Stone->lpMesh->GetNormal( PolyNo ) );

			// 滑りベクトルでもう一度判定
			DWORD SlipPoly;

			float SlipDistance = IntersectByMesh( 
				&m_mMat,
				&SlipVec,
				&( mScale * CMatrix( Stone->Pos ) ),
				*Stone->lpMesh->GetMeshData(),
				&SlipPoly );

			
			D3DXVECTOR3 TempVec;

			if( SlipDistance > 0 && SlipDistance - Length < (m_lpMesh->GetRadius() + 1) )
			{
				// 滑りベクトルで当たったなら止める	
				TempVec = D3DXVECTOR3( 0, 0, 0 );
		
			}else{
				
				// そうでなければ最初のベクトルを使う
				TempVec = SlipVec * m_NowSpeed;
			}
			
			// 移動
			m_mMat._41 += TempVec.x;
			m_mMat._42 += TempVec.y;
			m_mMat._43 += TempVec.z;

			IsMove = false;
			break;
		}
	}

	// 動けるなら動く
	if( IsMove )
	{
		m_mMat.MoveLocal( lp_move );
	}

	return true;
}

// 姿勢を正す
void CDolphin::ReformPose()
{
	// Z軸の角度を取得
	float Degree = m_mMat.GetRotZ();

	// 回転
	if( abs( Degree ) > 1 )
	{
		if( Degree < 0 )
		{
			Degree = max( Degree, -REFORM_SPEED );

		}else{

			Degree = min( Degree, REFORM_SPEED );
		}

		m_mMat.RotateLocalZ( -Degree );
	}
}

//// ポインター更新
//void CDolphin::UpdatePointer()
//{
//	if( m_Flg & FLG_CHARGE )
//	{
//		if( timeGetTime() - m_PointerSaveTime > 300 )
//		{
//			m_PointerAnime = ++m_PointerAnime % POINTER_ANIME_NUM;
//			
//			m_PointerSaveTime = timeGetTime();
//		}
//
//		m_Pointer.SetTexturePos(
//			&D3DXVECTOR2( POINTER_ANIME_RATE * m_PointerAnime, 0.f ),
//			&D3DXVECTOR2( POINTER_ANIME_RATE * (m_PointerAnime + 1), 0.f ),
//			&D3DXVECTOR2( POINTER_ANIME_RATE * (m_PointerAnime + 1), 1.f ),
//			&D3DXVECTOR2( POINTER_ANIME_RATE * m_PointerAnime, 1.f ) );
//	}
//}
//
//// チャージ更新
//void CDolphin::UpdateCharge()
//{
//	// 最大溜めなら
//	if( m_Flg & FLG_CHARGEMAX )
//	{
//		// アニメ更新
//		m_ChargeMaxAnime.Update();
//
//		const D3DXVECTOR2* lpTexPos = m_ChargeMaxAnime.GetTexPos();
//		
//		m_ChargeEffect.SetTexturePos(
//			&lpTexPos[ 0 ],
//			&lpTexPos[ 1 ],
//			&lpTexPos[ 2 ],
//			&lpTexPos[ 3 ] );
//	
//	}else if( m_Flg & FLG_CHARGE ){	// 普通のチャージ中なら
//	
//		// アニメ更新
//		m_ChargeAnime.Update();
//
//		// 一周したら描画
//		if( m_ChargeAnime.IsOneRound() )
//		{
//			m_Flg |= FLG_DRAW_CHARGE;
//		}
//
//		const D3DXVECTOR2* lpTexPos = m_ChargeAnime.GetTexPos();
//		
//		m_ChargeEffect.SetTexturePos(
//			&lpTexPos[ 0 ],
//			&lpTexPos[ 1 ],
//			&lpTexPos[ 2 ],
//			&lpTexPos[ 3 ] );
//	}
//}

// 自機描画
void CDolphin::DrawPlayer( const LPDIRECT3DDEVICE9 lpdevice )
{
	CMatrix mScale;
	mScale.SetScale( m_NowPlayerScale, m_NowPlayerScale, 1.f );
	lpdevice->SetTransform( D3DTS_WORLD, &(mScale * m_mMat) );

	// プレイヤー
	if( m_Flg & FLG_CHARGE )
	{
		// 最大溜めなら
		if( m_Flg & FLG_CHARGEMAX )
		{
			if( m_Flg & FLG_MAX_EMISSIVE )
			{
				m_lpMesh->SetEmissive( m_Power * ( 0.3f / POWER_MAX ) );
				m_Fin.GetMesh()->SetEmissive( m_Power * ( 0.3f / POWER_MAX ) );
			
			}else{
				
				m_lpMesh->SetDefaultMaterial();
				m_Fin.GetMesh()->SetDefaultMaterial();
			}

			if( timeGetTime() - m_EmissiveSaveTime > 100 )
			{
				m_Flg ^= FLG_MAX_EMISSIVE;
				m_EmissiveSaveTime = timeGetTime();
			}
		
		}else{

			m_lpMesh->SetEmissive( m_Power * ( 0.3f / POWER_MAX ) );
			m_Fin.GetMesh()->SetEmissive( m_Power * ( 0.3f / POWER_MAX ) );
		}
		
		
	}else{

		m_lpMesh->SetDefaultMaterial();
		m_Fin.GetMesh()->SetDefaultMaterial();
	}

#ifdef _DEBUG
	D3DLIGHT9 Light;

	lpdevice->GetLight( 0, &Light );

	lpdevice->LightEnable( 0, TRUE );
	DWORD Is;
	lpdevice->GetRenderState( D3DRS_LIGHTING, &Is );

#endif


	// 自機
	m_lpMesh->Draw( lpdevice );

	// フィン
	m_Fin.Draw( lpdevice );
}

// 死んだ更新
void CDolphin::UpdateDie()
{
	// だんだん浮かばせる
	m_NowFloatPower += FLOAT_SPEED;

	CMatrix mTrans( 0, m_NowFloatPower, 0 );

	m_mMat *= mTrans;

	// 転がす
	m_mMat.RotateLocalZ( DIE_ROT_SPEED );
}

// 波紋の描画
void CDolphin::DrawRipples( const LPDIRECT3DDEVICE9 lp_device )
{
	lp_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	m_Ripples.Draw( lp_device );
	lp_device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}