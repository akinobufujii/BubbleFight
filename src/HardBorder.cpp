//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "BubbleManager.h"

#include "ShotBase.h"
#include "ShotManager.h"

#include "FinTail.h"
#include "RippleManager.h"

#include "LimitWalls.h"
#include "StoneManager.h"
#include "GameMain.h"

#include "BossBase.h"
#include "HardBorder.h"

//////////////////////////////////////////////////
// 名前空間省略
//////////////////////////////////////////////////
using namespace Boss;

//////////////////////////////////////////////////
// 静的メンバ実体化
//////////////////////////////////////////////////
const int		CHardBorder::HP_MAX				= 50;//250;
const float		CHardBorder::MOVE_SPEED			= 1.f;
const float		CHardBorder::ROT_MAX			= 3.f;
const float		CHardBorder::COLORFADEOUT_SPEED	= 0.05f;
const USHORT	CHardBorder::SHOT_INTERVAL		= 2500;
const USHORT	CHardBorder::SLUG_TIME			= 3000;
const USHORT	CHardBorder::NEXT_MOVEMENT_TIME	= 5000;
const BYTE		CHardBorder::SHOT_NUM			= 5;
const USHORT	CHardBorder::STOP_TIME			= 3000;

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CHardBorder::CHardBorder( const LPDIRECT3DDEVICE9 lpdevice ):
m_ShotCnt( SHOT_NUM ),
m_lpmParents( NULL ),
m_lpMeshPath( NULL ),
m_lpMeshPos( NULL ),
m_lpTails( NULL ),
m_PartsMax( 0 ),
m_Basis( 0 ),
m_NowMoveSpeed( 0 ),
m_SaveStopTime( 0 ),
m_AddColor( 0, 0, 0, 0 ),
m_AnimeCnt( -1 ),
m_BreakAddTime( 0 ),
m_Board( 3 )
{
	m_lpMesh = CSXfileManager::GetInst()->GetData( "BigMachineFish.x" );

	m_HP = HP_MAX;

	m_Flg |= FLG_ATTACK;

	//SetMovement( static_cast<LPMOVEMENTFANC>(&CHardBorder::Test) );

	// 行動関数登録
	m_MovementList.push_back( &CHardBorder::UpdateMove );
	m_MovementList.push_back( &CHardBorder::UpdateShot );
	//m_MovementList.push_back( &CHardBorder::UpdateSlug );

	// 行動関数初期化
	m_NowMovement = m_MovementList.begin();

	// ランダムで行動決定
	m_SaveMovement = rand() % m_MovementList.size();
	for( int i = m_SaveMovement; i > 0; i-- )
	{
		m_NowMovement++;
	}

	// 読み込み開始
	LoadData();
	
	m_lpmParents[ m_Basis ].MoveLocal( 1, 1, 1 );

	m_SaveTime = timeGetTime();
}

// デストラクタ
CHardBorder::~CHardBorder()
{
	SAFE_DELETE_ARRAY( m_lpMeshPos );
	SAFE_DELETE_ARRAY( m_lpmParents );
	SAFE_DELETE_ARRAY( m_lpMeshPath );
	SAFE_DELETE_ARRAY( m_lpTails );

	// ポイントスプライト解放
	BOOST_FOREACH( CPointSpriteManager* It, m_PointSpriteList )
	{
		SAFE_DELETE( It );
	}

	m_PointSpriteList.clear();
}

// 更新
void CHardBorder::Update()
{
#ifdef _DEBUG

	if( Input::Keyboard::IsState( DIK_E, Input::PUSH ) )
	{
		LoadData();
	}

#endif

	// 毎ループ行うこと
	InitLoop();

	if( m_Flg & FLG_ALIVE )
	{
		// ※常に自機方に向く
		if( !(m_Flg & FLG_STICK) )
		{
			Turn();
		}
		
		// 行動実行してその行動が終了したとき
		if( (this->*(*m_NowMovement))() == false )
		{
			// 行動を入れ替える
			m_NowMovement = m_MovementList.begin();

			// ランダムで行動決定
			int Temp = (rand() % m_MovementList.size());

			// 前回と行動が同じなら
			if( m_SaveMovement == Temp )
			{
				m_SaveMovement = ++Temp % m_MovementList.size();
			}

			for( int i = m_SaveMovement; i > 0; i-- )
			{
				m_NowMovement++;
			}

			m_SaveTime = timeGetTime();
		}

		// 親構造更新
		for( int i = 0; i < m_PartsMax; i++ )
		{
			// 親なら何もしない
			if( i == m_Basis )
			{
				continue;
			}

			m_lpmParents[ i ] = CMatrix( m_lpMeshPos[ i ] ) * m_lpmParents[ m_Basis ];
		}

		// 尻尾(子)を更新
		for( int i = 0; i < m_TailsMax; i++ )
		{
			// 最初なら
			if( i == 0 )
			{
				// 親の行列を元に更新
				m_lpTails[ i ].Update( m_lpmParents[ m_Basis ], m_NowMoveSpeed );
			
			}else{

				// それ以外は前の尻尾を元に更新
				m_lpTails[ i ].Update( m_lpTails[ i - 1 ].GetMatrix(), m_NowMoveSpeed );
			}
		}

	}else if( m_Flg & FLG_BREAK ){

		// 5秒間パーティクル
		if( timeGetTime() - m_StartBreakTime > 5000 )
		{
			CSoundManager::GetInst()->Play( "Sound/BossBreak.wav", false, true );
			if( ++m_AnimeCnt > CBubble::BURST_ANIME_NUM )
			{
				m_Flg |= FLG_ERASE;
			}
			
		}else{

			// 一定時間おきにパーティクルを出す
			if( timeGetTime() - m_BreakAddTime > 80 )
			{
				CSoundManager::GetInst()->Play( "Sound/Break.wav" );

				// 適当にばらす
				D3DXVECTOR3 Pos = m_lpmParents[ m_Basis ].GetPos();

				int Temp = static_cast< int >( m_lpMesh->GetRadius() * 100 );
				Pos.x += rand() % Temp * 1.f * GetPosiOrNega();
				Pos.y += rand() % Temp * 1.f * GetPosiOrNega();
				Pos.z += rand() % Temp * 1.f * GetPosiOrNega();

				AddSprite( Pos );

				m_BreakAddTime = timeGetTime();
			}
		}
	}

	// 弾更新
	m_Shots.Update();

	// エフェクト更新
	BOOST_FOREACH( CPointSpriteManager* It, m_PointSpriteList )
	{
		It->Update();
	}
}

// 登場更新
bool CHardBorder::UpdateEnterScene()
{
	// 前へ進ませる
	MoveCollision( D3DXVECTOR3( 0, 0, 0.5f ), true );

	// 親構造更新
	for( int i = 0; i < m_PartsMax; i++ )
	{
		// 親なら何もしない
		if( i == m_Basis )
		{
			continue;
		}

		m_lpmParents[ i ] = CMatrix( m_lpMeshPos[ i ] ) * m_lpmParents[ m_Basis ];
	}

	// 尻尾(子)を更新
	for( int i = 0; i < m_TailsMax; i++ )
	{
		// 最初なら
		if( i == 0 )
		{
			// 親の行列を元に更新
			m_lpTails[ i ].Update( m_lpmParents[ m_Basis ], m_NowMoveSpeed );

		}else{

			// それ以外は前の尻尾を元に更新
			m_lpTails[ i ].Update( m_lpTails[ i - 1 ].GetMatrix(), m_NowMoveSpeed );
		}
	}

	return true;
}

// 描画
void CHardBorder::Draw(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( m_Flg & FLG_ALIVE )
	{
		// ボス
		if( IsCulling( &m_lpmParents[ m_Basis ] ) )
		{
			lpdevice->SetTransform( D3DTS_WORLD, &m_lpmParents[ m_Basis ] );

			CSXfileManager::GetInst()->GetData( m_lpMeshPath[ m_Basis ].c_str() )->Draw( lpdevice );
		}


		// 尻尾
		for( int i = 0; i < m_TailsMax; i++ )
		{
			m_lpTails[ i ].Draw( lpdevice );
		}

		// 弾
		m_Shots.Draw( lpdevice );
	
	}else{

		// 板ポリが描画されてなくて、まだ爆発していないなら描画
		if( m_AnimeCnt == -1 )
		{
			// ボス
			if( IsCulling( &m_lpmParents[ m_Basis ] ) )
			{
				lpdevice->SetTransform( D3DTS_WORLD, &m_lpmParents[ m_Basis ] );

				CSXfileManager::GetInst()->GetData( m_lpMeshPath[ m_Basis ].c_str() )->Draw( lpdevice );
			}


			// 尻尾
			for( int i = 0; i < m_TailsMax; i++ )
			{
				m_lpTails[ i ].Draw( lpdevice );
			}

			// 弾
			m_Shots.Draw( lpdevice );
		}
	}
}

// エフェクト描画
void CHardBorder::DrawEffect(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( m_Flg & FLG_BREAK )
	{
		DrawBreak( lpdevice );
	}

	// ポイントスプライト描画
	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	BOOST_FOREACH( CPointSpriteManager* It, m_PointSpriteList )
	{
		It->Draw( lpdevice );
	}

	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}

// シェーダ描画
void CHardBorder::DrawShader(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( m_Flg & FLG_ALIVE )
	{
		CEffect* lpSharder = CSShaderManager::GetInst()->GetData( "Phong.fx" );

		// 固定パイプラインの情報を獲得する
		CMatrix	mView, mProj;
		lpdevice->GetTransform( D3DTS_PROJECTION, &mProj );
		lpdevice->GetTransform( D3DTS_VIEW, &mView );

		// 座標変換
		lpSharder->SetValue( "g_WVP", 
			&(m_lpmParents[ m_Basis ] * mView * mProj),
			sizeof( D3DXMATRIX ) );

		// ライト位置計算
		lpSharder->SetValue( "g_LightDir",
			&GetLightPos( lpdevice, m_lpmParents[ m_Basis ] ),
			sizeof( D3DXVECTOR4 ) );

		// カメラ位置計算
		lpSharder->SetValue( "g_EyePos",
			&GetViewPos( lpdevice, m_lpmParents[ m_Basis ] ),
			sizeof( D3DXVECTOR4 ) );

		// 加算色
		lpSharder->SetValue( "g_AddColor",
			&m_AddColor,
			sizeof( D3DXCOLOR ) );
		
		// ボス
		if( IsCulling( &m_lpmParents[ m_Basis ] ) )
		{
			lpSharder->Begin();

			CSXfileManager::GetInst()->GetData( m_lpMeshPath[ m_Basis ].c_str() )->DrawShader( lpSharder, "g_Diffuse", "g_Tex" );

			lpSharder->End();
		}

		// 尻尾
		for( int i = 0; i < m_TailsMax; i++ )
		{
			m_lpTails[ i ].DrawShader( lpdevice );
		}

		// 弾
		m_Shots.Draw( lpdevice );
	
	}else{

		// 板ポリが描画されてなくて、まだ爆発していないなら描画
		if( m_AnimeCnt == -1 )
		{
			// ボス
			if( IsCulling( &m_lpmParents[ m_Basis ] ) )
			{
				lpdevice->SetTransform( D3DTS_WORLD, &m_lpmParents[ m_Basis ] );

				CSXfileManager::GetInst()->GetData( m_lpMeshPath[ m_Basis ].c_str() )->Draw( lpdevice );
			}


			// 尻尾
			for( int i = 0; i < m_TailsMax; i++ )
			{
				m_lpTails[ i ].Draw( lpdevice );
			}

			// 弾
			m_Shots.Draw( lpdevice );
		}
	}
}

// 自機の方に向く
void CHardBorder::Turn()
{
	// 自機の方に向く
	CMatrix mInv = m_lpmParents[ m_Basis ];

	// 自分（ボス）の逆行列
	mInv.GetInverse();

	// 目標(プレイヤー)の位置入れる
	CVector Target( m_PlayerMat.GetPos() );
	Target.GetTransCoord( &mInv );

	// 正規化
	Target.GetNormalVector();

	// 回転軸を求める
	CVector RotAxis = Target;

	RotAxis.GetCross( &D3DXVECTOR3( 0, 0, -1 ) );

	// 内積を求める
	float Ang = Target.GetDotDegree( &D3DXVECTOR3( 0, 0, 1 ) );

	if( Ang > ROT_MAX )
	{
		Ang = ROT_MAX;
	}

	m_lpmParents[ m_Basis ].RotateLocalAxis( &RotAxis, Ang );
}

// 動き更新
bool CHardBorder::UpdateMove()
{
	// 刺さっているなら
	if( m_Flg & FLG_STICK )
	{
		// 刺さっている更新
		UpdateStick();
	
	}else{

		// 突進する
		UpdateRush();
	}

	// 一定時間超えたら
	if( timeGetTime() - m_SaveTime > NEXT_MOVEMENT_TIME )
	{
		m_Flg &= ~FLG_STICK;
		m_NowMoveSpeed = 0;

		return false;	// 終了
	}

	return true;
}

// 突進
void CHardBorder::UpdateRush()
{
	// 行動が止まっている
	if( m_Flg & FLG_STOP )
	{
		if( timeGetTime() - m_SaveStopTime > STOP_TIME )
		{
			m_Flg &= ~FLG_STOP;
		}

		// 予備動作
		MoveCollision( D3DXVECTOR3( 0, 0, -MOVE_SPEED / 2 ), true );
	
	}else{

		// 常に一定
		m_NowMoveSpeed = MOVE_SPEED;

		// もし当たったなら
		if( MoveCollision( D3DXVECTOR3( 0, 0, m_NowMoveSpeed ), false ) )
		{
			// 刺さった時の初期化
			m_SaveTime = timeGetTime();
			m_Flg |= FLG_STICK;
			AddSprite( m_lpmParents[ m_Basis ].GetPos() );
		}
	}
}

// 刺さっている
void CHardBorder::UpdateStick()
{
	AddSprite( m_lpmParents[ m_Basis ].GetPos() );

	if( timeGetTime() - m_SaveTime > NEXT_MOVEMENT_TIME )
	{
		m_Flg &= ~FLG_STICK;
		m_Flg |= FLG_STOP;
		m_SaveStopTime = timeGetTime();
	}
}

// 弾更新
bool CHardBorder::UpdateShot()
{
	// 撃つ
	if( m_ShotCnt > 0 )
	{
		if( timeGetTime() - m_SaveTime > SHOT_INTERVAL )
		{
			m_Shots.Add( Shot::CManager::NORMAL, &m_lpmParents[ m_Basis ] );

			m_ShotCnt--;

			m_SaveTime = timeGetTime();

			m_AddColor.g = m_AddColor.b = 0;

			//m_Flg |= ATTACK;
		
		}else{

			// 予備動作中は色を変える
			m_AddColor.g = m_AddColor.b = 
				(timeGetTime() - m_SaveTime) / static_cast<float>(SHOT_INTERVAL);
		}
	
	}else{

		m_ShotCnt = SHOT_NUM;
		return false;	// 終了
	}


	return true;
}

// 散弾
bool CHardBorder::UpdateSlug()
{
	// 予備動作(紫色に変色)
	if( m_AddColor.b < 1 && m_AddColor.r < 1 )
	{
		m_AddColor.r += 0.1f;
		m_AddColor.b += 0.1f;

		if( m_AddColor.b >= 1 && m_AddColor.r >= 1 )
		{
			m_SaveTime = timeGetTime();
		}
	
	}else{

		m_AddColor.r = m_AddColor.b = 1.1f;

		// あらゆる方向に弾を飛ばしてみる
		if( timeGetTime() - m_SaveTime < SLUG_TIME )
		{
			CMatrix	mMat = m_lpmParents[ m_Basis ];

			mMat.RotateLocalX( static_cast<float>( rand() % 45 - 30 ) );
			mMat.RotateLocalY( static_cast<float>( rand() % 45 - 30 ) );
			//mMat.RotateLocalZ( static_cast<float>( rand() % 360 ) );

			m_Shots.Add( Shot::CManager::POWER, &mMat );

		}else{

			// 色をもとに戻す
			m_AddColor.r = m_AddColor.b = 0.f;
			return false;	// 終了
		}
	}
	
	return true;
}

// 弾に対して
void CHardBorder::Action( Shot::CBase& object )
{
	// 自分の打ちだした球かどうかを調べる
	if( typeid( object ) != typeid( Shot::CBubbleRing ) )
	{
		// もしバブルリングじゃなければ
		// 跳ね返されたかどうかを調べる
		if( !(object.GetFlg() & Shot::CBase::FLG_FIN_HIT) )
		{
			// 跳ね返されていなければ終了
			// 何もしない
			return;
		}
	}

	// ダメージを食らったなら
	if( (object.GetFlg() & Shot::CBase::FLG_ALIVE) )
	{
		AddSprite( m_lpmParents[ m_Basis ].GetPos() );

		m_Flg |= FLG_DAMAGE;

		m_AddColor.r = 1;

		if( --m_HP < 0 )
		{
			m_HP = 0;

			m_Flg |= FLG_BREAK;
			m_Flg &= ~FLG_ALIVE;

			m_Board.SetVertex( 25 );
		}
	}
}

// プレイヤーに対して
void CHardBorder::Action( Player::CDolphin& object )
{
	// 動きを止める
	m_Flg |= FLG_STOP;
	m_NowMoveSpeed = 0;
	m_SaveStopTime = timeGetTime();
}

// テキストデータから読み込む
void CHardBorder::LoadData()
{
	SAFE_DELETE_ARRAY( m_lpmParents );
	SAFE_DELETE_ARRAY( m_lpMeshPath );
	SAFE_DELETE_ARRAY( m_lpTails );
	SAFE_DELETE_ARRAY( m_lpMeshPos );

	std::ifstream In( "LoadData/GameMain/BigFishData.txt" );

	if( In.is_open() )
	{
		// 最大数読み込み
		In >> m_PartsMax;
		m_PartsMax -= '0';

		// 配列確保
		m_lpmParents= NEW CMatrix[ m_PartsMax ];
		m_lpMeshPos	= NEW CVector[ m_PartsMax ];
		m_lpMeshPath= NEW std::string[ m_PartsMax ];

		// 親を決定する
		In >> m_Basis;
		m_Basis -= '0';

		// 尻尾の数を決定し、確保する
		In >> m_TailsMax;
		m_TailsMax -= '0';

		m_lpTails = NEW CFinTail[ m_TailsMax ];

		// ローカル行列の初期位置とメッシュパスを設定する
		for( int i = 0; i < m_PartsMax; i++ )
		{
			In >> m_lpMeshPos[ i ].x;
			In >> m_lpMeshPos[ i ].y;
			In >> m_lpMeshPos[ i ].z;

			m_lpmParents[ i ].SetTrans( &m_lpMeshPos[ i ] );

			In >> m_lpMeshPath[ i ];
		}

		// それぞれの尻尾位置とメッシュを設定する
		for( int i = 0; i < m_TailsMax; i++ )
		{
			m_lpTails[ i ].SetRotMax( 12 );
			m_lpTails[ i ].SetFarPos( &m_lpMeshPos[ i + 1 ] );
			m_lpTails[ i ].SetMesh( CSXfileManager::GetInst()->GetData( m_lpMeshPath[ i + 1 ].c_str() ) );
		}
	}

	In.close();
}

// ポイントスプライト追加
void CHardBorder::AddSprite( const D3DXVECTOR3& pos )
{
	// リセットできるものがないか調べる
	BOOST_FOREACH( CPointSpriteManager* It, m_PointSpriteList )
	{
		if( It->IsErase() )
		{
			It->SetPos( m_lpmParents[ m_Basis ].GetPos() );
			It->Reset();

			return;
		}
	}

	CPointSpriteManager* lpTemp = NEW CPointSpriteManager();

	lpTemp->SetNum( 10 );
	lpTemp->SetPos( pos );
	lpTemp->SetScaleMax( 2.f );
	lpTemp->SetEraseTime( 2000 );
	lpTemp->SetMove( D3DXVECTOR3( 1, -1, 1 ) );

	CTexture* lpTex = CSTexManager::GetInst()->GetData( "Sprite.jpg" );

	lpTemp->SetTexture( *lpTex );

	m_PointSpriteList.push_back( lpTemp );
}

// 衝突判定
bool CHardBorder::MoveCollision(const D3DXVECTOR3 &move, bool is_move)
{
	if( GetStones() == NULL )
	{
		return false;
	}

	// 長さと方向を抽出
	float Length = D3DXVec3Length( &move );

	CVector Direction;
	D3DXVec3Normalize( &Direction, &move );

	// 自機の方向を調べる
	Direction.GetTransNormal( &m_lpmParents[ m_Basis ] );

	bool IsMove = true;

	CMatrix mScale;

	DWORD PolyNo;	// 当たったポリゴンナンバー

	// 岩との衝突判定
	BOOST_FOREACH( CStoneManager::STONEDATA* Stone, *GetStones()->GetList() )
	{
		mScale.SetScale( Stone->Scale );

		// 判定
		float Distance = IntersectByMesh( 
			&m_lpmParents[ m_Basis ],
			&Direction,
			&( mScale * CMatrix( Stone->Pos ) ),
			*Stone->lpMesh->GetMeshData(),
			&PolyNo );

		// 応答
		if( Distance > 0 && Distance - Length < (1) )
		{
			// 動くなら
			if( is_move )
			{
				// 今の前方方向を調べる
				CVector	NowFront( 0, 0, 1 );

				NowFront.GetTransNormal( &m_lpmParents[ m_Basis ] );

				// 滑りベクトル計算
				D3DXVECTOR3 SlipVec = 
					GetSlipVec( NowFront, Stone->lpMesh->GetNormal( PolyNo ) );

				// 滑りベクトルでもう一度判定
				DWORD SlipPoly;

				float SlipDistance = IntersectByMesh( 
					&m_lpmParents[ m_Basis ],
					&SlipVec,
					&( mScale * CMatrix( Stone->Pos ) ),
					*Stone->lpMesh->GetMeshData(),
					&SlipPoly );


				D3DXVECTOR3 TempVec;

				if( SlipDistance > 0 && SlipDistance - Length < (1) )
				{
					// 滑りベクトルで当たったなら止める	
					TempVec = D3DXVECTOR3( 0, 0, 0 );

				}else{

					// そうでなければ最初のベクトルを使う
					TempVec = SlipVec * Length;
				}

				// 移動
				m_lpmParents[ m_Basis ]._41 += TempVec.x;
				m_lpmParents[ m_Basis ]._42 += TempVec.y;
				m_lpmParents[ m_Basis ]._43 += TempVec.z;

				IsMove = false;
				break;
			
			}else{
			
				//m_lpmParents[ m_Basis ].MoveLocal( &(Direction * (Distance - Length)));
				// 当たったことだけ伝える
				return true;
			}
		}
	}

	// 動けるなら動く
	if( IsMove )
	{
		m_lpmParents[ m_Basis ].MoveLocal( &move );
	}

	return false;
}

// 姿勢を正す
void CHardBorder::ReformPose()
{
	// Z軸回転を見る
	float Degree = m_lpmParents[ m_Basis ].GetRotZ();

	// 回転が必要であれば回転する
	if( abs( Degree ) > 1 )
	{
		if( Degree > 0 )
		{
			m_lpmParents[ m_Basis ].RotateLocalZ( -1 );

		}else{

			m_lpmParents[ m_Basis ].RotateLocalZ( 1 );
		}
	}
}

// 毎ループ行う初期化
void CHardBorder::InitLoop()
{
	// ダメージフラグを消す
	m_Flg &= ~FLG_DAMAGE;

	// 色の減算
	if( (m_AddColor.r -= COLORFADEOUT_SPEED) < 0 )
	{
		m_AddColor.r = 0;
	}
	if( (m_AddColor.g -= COLORFADEOUT_SPEED) < 0 )
	{
		m_AddColor.g = 0;
	}
	if( (m_AddColor.b -= COLORFADEOUT_SPEED) < 0 )
	{
		m_AddColor.b = 0;
	}
	if( (m_AddColor.a -= COLORFADEOUT_SPEED) < 0 )
	{
		m_AddColor.a = 0;
	}

	// 死んでるかどうか
	if( !(m_Flg & FLG_BREAK) && m_HP <= 0 )
	{
		m_Flg |= FLG_BREAK;

		m_BreakAddTime = m_StartBreakTime = timeGetTime();
	}

	// 姿勢を正す
	ReformPose();
}

// 破壊時の描画
void CHardBorder::DrawBreak(LPDIRECT3DDEVICE9 lpdevice)
{
	// 最後の爆発なら
	if( m_AnimeCnt >= 0 && m_AnimeCnt < CBubble::BURST_ANIME_NUM )
	{
		m_Board.SetTexturePos(
				&D3DXVECTOR2(
				(m_AnimeCnt % CBubble::BURST_ANIME_WIDTH_NUM * (float)CBubble::BURST_ANIME_SIZE) / CBubble::BURST_WIDTH,
				(m_AnimeCnt / CBubble::BURST_ANIME_HEIGHT_NUM* (float)CBubble::BURST_ANIME_SIZE) / CBubble::BURST_HEIGHT ),

				&D3DXVECTOR2( 
				((m_AnimeCnt % CBubble::BURST_ANIME_WIDTH_NUM + 1)	* (float)CBubble::BURST_ANIME_SIZE) / CBubble::BURST_WIDTH,
				(m_AnimeCnt	 / CBubble::BURST_ANIME_HEIGHT_NUM		* (float)CBubble::BURST_ANIME_SIZE) / CBubble::BURST_HEIGHT ),

				&D3DXVECTOR2(
				((m_AnimeCnt % CBubble::BURST_ANIME_WIDTH_NUM + 1)	* (float)CBubble::BURST_ANIME_SIZE) / CBubble::BURST_WIDTH,
				((m_AnimeCnt / CBubble::BURST_ANIME_HEIGHT_NUM+ 1)	* (float)CBubble::BURST_ANIME_SIZE)	/ CBubble::BURST_HEIGHT ),

				&D3DXVECTOR2( 
				(m_AnimeCnt	 % CBubble::BURST_ANIME_WIDTH_NUM		* (float)CBubble::BURST_ANIME_SIZE)	/ CBubble::BURST_WIDTH,
				((m_AnimeCnt / CBubble::BURST_ANIME_HEIGHT_NUM + 1)  * (float)CBubble::BURST_ANIME_SIZE)	/ CBubble::BURST_HEIGHT )
				);

		// ビルボード
		CMatrix	mMat, mTrans;
		CCamera Camera = *Sequence::CGameMain::GetCamera();
		Camera.GetBillboard( &mMat, &m_lpmParents[ m_Basis ] );
		mTrans.SetTrans( &m_lpmParents[ m_Basis ].GetPos() );

		// 合成
		mMat = mMat * mTrans;
		lpdevice->SetTransform( D3DTS_WORLD, &mMat );

		// テクスチャと頂点フォーマット設定
		lpdevice->SetTexture( 0, *CSTexManager::GetInst()->GetData( "Smoke.png" ) );
		lpdevice->SetFVF( CBoard::FVF_VERTEX );

		//lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );

		lpdevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		lpdevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

		// 描画
		m_Board.Draw( lpdevice );

		//lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	}
}

// 爆発ブラーの強さ
float CHardBorder::GetBlurPower()
{
	// 爆発中なら
	if( m_AnimeCnt >= 0 && m_AnimeCnt < CBubble::BURST_ANIME_NUM )
	{
		return CBubble::BURST_ANIME_NUM / 2 - m_AnimeCnt + CBubble::BURST_ANIME_NUM / 2;
	}

	return 0;
}

// 爆発ブラーの光の強さ
float CHardBorder::GetAddColorRate()
{
	// 爆発中なら
	if( m_AnimeCnt >= 0 && m_AnimeCnt < CBubble::BURST_ANIME_NUM )
	{
		return 1 + GetBlurPower() / CBubble::BURST_ANIME_NUM / 2;
	}

	return 1;
}