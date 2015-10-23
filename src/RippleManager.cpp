//////////////////////////////
// インクルード
//////////////////////////////
#include "Ripple.h"

#include "FishManager.h"
#include "UI.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "EnemyManager.h"
#include "GameMain.h"

#include "RippleManager.h"

//////////////////////////////
// 静的メンバ実体化
//////////////////////////////
const LPCSTR	CRippleManager::TEXPATH_DEFAULT_RIPPLE	= "Ripple.png";

//////////////////////////////
// 実装
//////////////////////////////

// コンストラクタ
CRippleManager::CRippleManager():
m_lpTex( NULL ),
m_CreateHeight( 0 ),
m_Width( 0 ),
m_Height( 0 )
{
}

// デストラクタ
CRippleManager::~CRippleManager()
{
	// 削除
	BOOST_FOREACH( CRipple* It, m_RippleList )
	{
		SAFE_DELETE( It );
	}

	m_RippleList.clear();
}

// 更新
void CRippleManager::Update(const D3DXMATRIX& mat)
{
	// 基本的にリストはキュー構造のように扱う
	// なぜSTLのQueueを使わないか
	// メンバ関数の操作がしにくいから

	// プレイヤーが水面付近に居ることを確認する
	bool Flg = IsUpdate( CVector( mat ) );

	// もしリストに何もなければ
	if( Flg && m_RippleList.empty() )
	{
		// 追加
		m_RippleList.push_back( Create( mat ) );
	}

	// とりあえず更新
	BOOST_FOREACH( CRipple* It, m_RippleList )
	{
		It->Update();
	}

	// 以下の処理はリストが空じゃなければ処理する
	if( m_RippleList.empty() == false )
	{
		// 追加判定
		// もし末尾の要素が追加して欲しいなら
		if( Flg && m_RippleList.back()->IsNext() )
		{
			// 末尾に追加する
			m_RippleList.push_back( Create( mat ) );
		}

		// 削除判定
		// もし先頭の要素が消えるべきなら
		if( m_RippleList.front()->IsErase() )
		{
			// Deleteしてリストから削除(pop)する
			SAFE_DELETE( m_RippleList.front() );

			m_RippleList.pop_front();
		}
	}
}

// 描画
void CRippleManager::Draw(const LPDIRECT3DDEVICE9 lp_device)
{
	// ライティングを切る
	//lp_device->SetRenderState( D3DRS_LIGHTING, FALSE );

	// 頂点フォーマット
	lp_device->SetFVF( CBoard::FVF_VERTEX );

	// テクスチャを設定
	lp_device->SetTexture( 0, m_lpTex );

	// 不具合防止の為に海面の上と下に描画
	BOOST_FOREACH( CRipple* It, m_RippleList )
	{
		It->DrawUp( lp_device );
	}

	BOOST_FOREACH( CRipple* It, m_RippleList )
	{
		It->DrawDown( lp_device );
	}

	//lp_device->SetRenderState( D3DRS_LIGHTING, TRUE );
}

// 作成する
CRipple* CRippleManager::Create(const D3DXMATRIX& mat)
{
	// パラメータを設定して追加
	CRipple* lpTemp = NEW CRipple();

	lpTemp->SetScaleRate( 0.05f );
	lpTemp->SetSize( m_Width, m_Height );

	// 角度を抽出
	CVector Vec( 0, 0, 1 );
	CMatrix mInv = mat;

	mInv.GetInverse();

	Vec.GetTransNormal( &mInv );

	Vec.y = 0;
	Vec.GetNormalVector();

	float Degree = Vec.GetDotDegree( &CVector( 0, 0, 1 ) );

	// 回転方向決定
	if( abs( Degree ) > 1 )
	{
		if( Vec.x > 0 )
		{
			Degree *= -1;

		}
	
	}else{

		Degree = 0;
	}

	lpTemp->SetRotY( Degree );

	// 位置を抽出
	CVector Pos = mat;

	// 海面固定
	Pos.y = Sequence::CGameMain::SURFACESEA_HEIGHT;

	lpTemp->SetPos( Pos );

	return lpTemp;
}

// 今、更新を行うべきかどうか
bool CRippleManager::IsUpdate( const D3DXVECTOR3 &pos )
{
	// 海面付近ならtrue
	if( Sequence::CGameMain::SURFACESEA_HEIGHT - m_CreateHeight < pos.y &&
		Sequence::CGameMain::SURFACESEA_HEIGHT + m_CreateHeight > pos.y )
	{
		return true;
	}

	// そうでなければfalse
	return false;
}