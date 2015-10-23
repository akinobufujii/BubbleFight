//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "BubbleManager.h"

#include "ShotBase.h"
#include "BigShot.h"

//////////////////////////////////////////////////
// 名前空間省略
//////////////////////////////////////////////////
using namespace Shot;

//////////////////////////////////////////////////
// 静的メンバ実体化
//////////////////////////////////////////////////
const float	CBig::SPEED			= 1.f;
const DWORD	CBig::ERASE_TIME	= 10000;
const LPSTR	CBig::SHOT_MESH_PASS= "BigShot.x";

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CBig::CBig():
m_StartTime( timeGetTime() ),
m_lpMesh( CSXfileManager::GetInst()->GetData( SHOT_MESH_PASS ) )
{
}

CBig::CBig( const D3DXMATRIX* mat ):
CBase( mat ),
m_StartTime( timeGetTime() ),
m_lpMesh( CSXfileManager::GetInst()->GetData( SHOT_MESH_PASS ) )
{
}

// 更新
void CBig::Update()
{
	if( m_Flg & FLG_ALIVE )
	{
		m_mMat.MoveLocal( 0.f, 0.f, SPEED );

		// 定時を過ぎれば消える
		if( timeGetTime() - m_StartTime > ERASE_TIME )
		{
			m_Flg |= FLG_ERASE;
		}
	}
}

// 描画
void CBig::Draw(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( IsCulling( &m_mMat ) )
	{
		lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
		m_lpMesh->Draw( lpdevice );
	}
}

// デストラクタ
CBig::~CBig(void)
{
}
