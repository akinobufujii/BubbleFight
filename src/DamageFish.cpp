/////////////////////////////////
// インクルード
/////////////////////////////////
#include "DamageObjectBase.h"

#include "BubbleManager.h"
#include "ShotManager.h"
#include "RippleManager.h"
#include "EnemyBase.h"

#include "EnemyManager.h"
#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "GameMain.h"

#include "DamageFish.h"

/////////////////////////////////
// 静的メンバ実体化
/////////////////////////////////
const float	CDamageFish::MOVESPEED	= 0.5f;
const float	CDamageFish::MOVEHEIGHT	= Sequence::CGameMain::SURFACESEA_HEIGHT - 5;
const float	CDamageFish::TURNSPEED	= 1.5f;

/////////////////////////////////
// 実装
/////////////////////////////////

// コンストラクタ
CDamageFish::CDamageFish()
{
}

// デストラクタ
CDamageFish::~CDamageFish()
{
}

// 更新
void CDamageFish::Update()
{
	// ※常に高さ固定
	m_mMat._42 = MOVEHEIGHT;

	// 移動
	m_mMat.MoveLocal( 0, 0, m_Speed );

	// もし範囲から出ていれば
	D3DXVECTOR3 Pos = m_mMat.GetPos();

	if( abs( Pos.x ) > Sequence::CGameMain::SEA_MOVELIMIT ||
		abs( Pos.z ) > Sequence::CGameMain::SEA_MOVELIMIT )
	{
		// 回す
		D3DXVECTOR3 Axis;
		float Rot = GetAxisAndRot( &Axis, D3DXVECTOR3( 0, MOVEHEIGHT, 0 ), m_mMat );
		
		if( Rot > TURNSPEED )
		{
			Rot = TURNSPEED;
		}

		m_mMat.RotateLocalAxis( &Axis, Rot );
	}

	MustUpdate();
}

// 描画
void CDamageFish::Draw(const LPDIRECT3DDEVICE9 lp_device, bool is_shader)
{
	// 視錐台カリングしない
	//if( IsCulling( &m_mMat ) )
	{
		CMatrix mScale;

		mScale.SetScale( m_Scale );

		// シェーダ描画するなら
		// ※なんかおかしいので保留
		if( false )
		{
			// リムライトシェーディング
			// シェーダデバイス獲得
			CEffect* lpShader = CSShaderManager::GetInst()->GetData( "Rim.fx" );

			// 座標変換
			D3DXMATRIX mView, mProj, mWorld = mScale * m_mMat;
			lp_device->GetTransform( D3DTS_VIEW, &mView );
			lp_device->GetTransform( D3DTS_PROJECTION, &mProj );

			lpShader->SetValue( "g_WVP", 
				&(mWorld * mView * mProj),
				sizeof( D3DXMATRIX ) );

			// ライト位置
			lpShader->SetValue( "g_LightDir", 
				&GetLightPos( lp_device, mWorld ),
				sizeof( D3DXVECTOR4 ) );

			// 視点位置
			lpShader->SetValue( "g_EyePos", 
				&GetViewPos( lp_device, mWorld ),
				sizeof( D3DXVECTOR4 ) );

			lpShader->Begin();
	
			m_lpMesh->DrawShader( lpShader, "g_Diffuse", "g_Tex" );

			lpShader->End();

		}else{

			// 単なる描画
			lp_device->SetTransform( D3DTS_WORLD, &(mScale * m_mMat) );

			m_lpMesh->Draw( lp_device );
		}
	}

	// 泡
	m_lpBubbles->DrawNoBreak( lp_device );
}

// 敵に対する行動
void CDamageFish::Action(Enemy::CBase &object)
{
	CVector Dir = m_mMat.GetPos() - object.GetPos();

	Dir.GetNormalVector();
	Dir *= 0.5f;

	m_mMat.MoveLocal( &Dir );
}