//////////////////////////////
// インクルード
//////////////////////////////
#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "UI.h"
#include "PlanktonManager.h"
#include "EnemyManager.h"
#include "GameMain.h"

#include "ObjectFish.h"

//////////////////////////////
// 静的メンバ実体化
//////////////////////////////
const float	CObjectFish::MOVE_SPEED		= -0.2f;
const float	CObjectFish::SIZE			= 2;
const float	CObjectFish::TEXPOS_WIDTH	= 1.f / 2;
const float	CObjectFish::TEXPOS_HEIGHT	= 1.f / 4;
const BYTE	CObjectFish::NODE_RANGE		= 4;
const BYTE	CObjectFish::KIND_NUM		= 8;

const CObjectFish::FLGTYPE	CObjectFish::FLG_TURN	= 0x01;

//////////////////////////////
// 実装
//////////////////////////////

// コンストラクタ
CObjectFish::CObjectFish():
m_Flg( 0 )
{
	m_Board.SetVertex( SIZE );

	// テクスチャ座標決定
	int Kind	= rand() % KIND_NUM;
	int Width	= Kind / 2;
	int Height	= Kind / 4;

	m_Board.SetTexturePos(
		&D3DXVECTOR2( TEXPOS_WIDTH * Width,			TEXPOS_HEIGHT * Height		),
		&D3DXVECTOR2( TEXPOS_WIDTH * (Width + 1),	TEXPOS_HEIGHT * Height		),
		&D3DXVECTOR2( TEXPOS_WIDTH * (Width + 1),	TEXPOS_HEIGHT * (Height + 1)),
		&D3DXVECTOR2( TEXPOS_WIDTH * Width,			TEXPOS_HEIGHT * (Height + 1)) );
}

// デストラクタ
CObjectFish::~CObjectFish()
{
}

// 更新
void CObjectFish::Update( bool is_collision )
{
	// 漂う
	m_mMat.MoveLocal( MOVE_SPEED, 0, 0 );

	// 範囲判定
	CVector Pos = m_mMat;

	// 横の境界を越えたら
	if( !(m_Flg & FLG_TURN)									&&
		(abs( Pos.x ) > Sequence::CGameMain::SEA_MOVELIMIT	||
		abs( Pos.z ) > Sequence::CGameMain::SEA_MOVELIMIT) )
	{
		m_Flg |= FLG_TURN;

		SetRotY( 180 );

	}else if(	abs( Pos.x ) > Sequence::CGameMain::SEA_MOVELIMIT	||
				abs( Pos.z ) > Sequence::CGameMain::SEA_MOVELIMIT	)
	{
		m_Flg &= ~FLG_TURN;
	}

	// 岩との当たり判定
	CStoneManager::STONELIST* StoneList = CFishManager::GetStoneManager()->GetList();

	CMatrix mScale, mTrans;
	float	Distance;

	if( is_collision )
	{
		BOOST_FOREACH( CStoneManager::STONEDATA* Stone, *StoneList )
		{
			mScale.SetScale( Stone->Scale );
			mTrans.SetTrans( &Stone->Pos );

			Distance = IntersectByMesh(
				&m_mMat,
				&D3DXVECTOR3( 1.f, 0.f, 0.f ),
				&( mScale * mTrans ),
				*Stone->lpMesh->GetMeshData() );

			if( Distance > 0 && Distance < NODE_RANGE )
			{
				SetRotY( rand() % 45 * GetPosiOrNega() + 180 );
				m_mMat.MoveLocal( MOVE_SPEED, 0, 0 );
			}
		}
	}

	// 子オブジェクトにも同じ事をする
	// 子オブジェクトは親を追いかける
	BOOST_FOREACH( CMatrix& i, m_NodeMat )
	{
		// 回転軸と角度を求める
		D3DXVECTOR3 RotAxis;
		float Ang = GetAxisAndRot( &RotAxis, m_mMat.GetPos(), i );

		// 指定軸で回転
		//i.RotateLocalAxis( &RotAxis, Ang );

		i.MoveLocal( MOVE_SPEED, 0, 0 );
	}
}

// 描画
void CObjectFish::Draw(const LPDIRECT3DDEVICE9 lp_device, const D3DXMATRIX& mat )
{
	lp_device->SetFVF( CBoard::FVF_VERTEX );

	lp_device->SetTexture( 0, CSTexManager::GetInst()->GetData( "Fish.png" )->GetTexture() );

	// 親
	if( IsCulling( &(m_mMat * mat) ) )
	{
		lp_device->SetTransform( D3DTS_WORLD, &(m_mMat * mat) );

		m_Board.Draw( lp_device );
	}

	// 子
	//CMatrix mWorld = m_mMat;

	BOOST_FOREACH( CMatrix& i, m_NodeMat )
	{
		//if( IsCulling( &(i * mat) ) )
		{
			// 回転を抜きにする(親の向きに合わせる)
			/*mWorld._41 = i.GetPos().x;
			mWorld._42 = i.GetPos().y;
			mWorld._43 = i.GetPos().z;*/

			lp_device->SetTransform( D3DTS_WORLD, &(i * mat) );

			m_Board.Draw( lp_device );
		}
	}
}

// 位置設定
void CObjectFish::SetPos( const D3DXVECTOR3& pos )
{
	// 親の位置設定
	m_mMat.SetTrans( &pos );

	// 子をランダムな位置に
	BOOST_FOREACH( CMatrix& i, m_NodeMat )
	{
		i.SetTrans(
			pos.x + ( rand() % NODE_RANGE * GetPosiOrNega() ),
			pos.y + ( rand() % NODE_RANGE * GetPosiOrNega() ),
			pos.z + ( rand() % NODE_RANGE * GetPosiOrNega() ) );
	}
}

// 回転設定
void CObjectFish::SetRotY( const float rot )
{
	m_mMat.RotateLocalY( rot );

	// 子
	BOOST_FOREACH( CMatrix& i, m_NodeMat )
	{
		i.RotateLocalY( rot );
	}
}