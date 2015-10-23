////////////////////////////////
// インクルード
////////////////////////////////
#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "EnemyManager.h"
#include "UI.h"
#include "GameMain.h"

#include "Plankton.h"

////////////////////////////////
// 静的メンバ実体化
////////////////////////////////
const float CPlankton::SIZE	= 0.2f;
CBoard		CPlankton::m_Board( SIZE );

////////////////////////////////
// 実装
////////////////////////////////

// コンストラクタ
CPlankton::CPlankton()
{
	int X = static_cast<int>( Sequence::CGameMain::SEA_MOVELIMIT );
	int Y = static_cast<int>( Sequence::CGameMain::SURFACESEA_HEIGHT );
	int Z = static_cast<int>( Sequence::CGameMain::SEA_MOVELIMIT );
	
	m_vPos.x = rand() % X * 1.f * GetPosiOrNega();
	m_vPos.y = rand() % Y * 1.f * GetPosiOrNega();
	m_vPos.z = rand() % Z * 1.f * GetPosiOrNega();

	m_Board.SetColor( 1.f, 1.f, 1.f );
	m_Board.SetAlpha( 1.f );

	m_Board.SetTexturePos(
		&D3DXVECTOR2( 0.f, 0.f ),
		&D3DXVECTOR2( 1.f, 0.f ),
		&D3DXVECTOR2( 1.f, 1.f ),
		&D3DXVECTOR2( 0.f, 1.f ) );
}

// 更新
void CPlankton::Update()
{
}

// 描画
void CPlankton::Draw( const LPDIRECT3DDEVICE9 lpdevice, const CCamera* camera )
{
	if( IsCulling( &CMatrix( m_vPos ) ) )
	{
		// ビルボードの為の回転行列獲得
		D3DXMATRIX mRot;

		camera->GetBillboard( &mRot, &CMatrix( m_vPos ) );
		lpdevice->SetTransform( D3DTS_WORLD, &( mRot * CMatrix( m_vPos ) ) );

		// 描画
		m_Board.Draw( lpdevice );
	}
}

// デストラクタ
CPlankton::~CPlankton()
{
}
