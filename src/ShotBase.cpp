//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

#include "ShotBase.h"

//////////////////////////////////////////////////
// 名前空間省略
//////////////////////////////////////////////////
using namespace Shot;

//////////////////////////////////////////////////
// 静的変数実体化
//////////////////////////////////////////////////
const CBase::FLGTYPE	CBase::FLG_ALIVE		= 0x01;
const CBase::FLGTYPE	CBase::FLG_ERASE		= 0x02;
const CBase::FLGTYPE	CBase::FLG_BREAK		= 0x04;
const CBase::FLGTYPE	CBase::FLG_BUBBLE_WRAP	= 0x08;
const CBase::FLGTYPE	CBase::FLG_FIN_HIT		= 0x10;

const USHORT	CBase::ERASE_TIME			= 1000;
const USHORT	CBase::CREATE_BUBBLE_TIME	= CBase::ERASE_TIME / 10;

const USHORT	CBase::BREAK_ANIME_NUM		= 10;
const USHORT	CBase::BREAK_GRAPH_WIDTH	= 800;
const USHORT	CBase::BREAK_GRAPH_HEIGHT	= 80;
const USHORT	CBase::BREAK_GRAPH_SIZE		= 80;

const LPSTR		CBase::BREAK_TEX_PASS		= "SandSmoke.png";
const LPSTR		CBase::SPRITE_TEX_PASS		= "Sprite3.png";

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// コンストラクタ
CBase::CBase():
m_Flg( FLG_ALIVE ),
m_AnimeCnt( 0 )
{
}

CBase::CBase( const D3DXMATRIX* mat ):
m_mMat( *mat ),
m_Flg( FLG_ALIVE ),
m_AnimeCnt( 0 )
{
}

// デストラクタ
CBase::~CBase()
{
}

// 泡爆破
void CBase::BreakBubble()
{
	m_Bubbles.BreakAll();
}

// 爆発更新
void CBase::UpdateBreak()
{
	// 定数以上なら消す
	if( ++m_AnimeCnt >= BREAK_ANIME_NUM )
	{
		m_Flg &= ~FLG_ALIVE;
		return;
	}

	float X = static_cast<float>(BREAK_GRAPH_SIZE) / BREAK_GRAPH_WIDTH;

	/*m_Board.SetTexturePos(
		&D3DXVECTOR2( 0,		0 ),
		&D3DXVECTOR2( 1.f,	0 ),
		&D3DXVECTOR2( 1.f,	1.f ),
		&D3DXVECTOR2(0,		1.f ) );*/

	m_Board.SetTexturePos(
		&D3DXVECTOR2( m_AnimeCnt * X,		0 ),
		&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	0 ),
		&D3DXVECTOR2( (m_AnimeCnt + 1) * X,	1.f ),
		&D3DXVECTOR2( m_AnimeCnt * X,		1.f ) );
}

// 爆発描画
void CBase::DrawBreak(const LPDIRECT3DDEVICE9 lpdevice)
{
	lpdevice->SetFVF( CBoard::FVF_VERTEX );
	lpdevice->SetTexture( 0, CSTexManager::GetInst()->GetData( BREAK_TEX_PASS )->GetTexture() );
	lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpdevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	m_Board.Draw( lpdevice );

	//lpdevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	//lpdevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}