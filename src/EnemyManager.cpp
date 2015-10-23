//////////////////////////////////////////////////
// インクルード
//////////////////////////////////////////////////
#include "Bubble.h"
#include "BubbleManager.h"

#include "FinTail.h"

#include "RippleManager.h"
#include "ShotManager.h"
#include "EnemyBase.h"
#include "NormalEnemy.h"
#include "ShotEnemy.h"
#include "RushEnemy.h"
#include "BubbleBreaker.h"

#include "EnemyManager.h"

//////////////////////////////////////////////////
// 名前空間省略
//////////////////////////////////////////////////
using namespace Enemy;

//////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////

// 更新
void CManager::Update()
{
	// ※途中削除があるので通常ループ
	ENEMYLIST::iterator It = m_EnemyList.begin();

	while( It != m_EnemyList.end() )
	{
		(*It)->Update();
		
		// もういらないなら消す
		if( (*It)->GetFlg() & CBase::ERASE )
		{
			// スコア加算
			m_Score += (*It)->GetScore();

			SAFE_DELETE( (*It) );
			It = m_EnemyList.erase( It );
			continue;
		}

		++It;
	}
}

// 生きているもの描画
void CManager::DrawAlive(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	BOOST_FOREACH( CBase* It, m_EnemyList )
	{
		// 判定
		if( ~(It->GetFlg() & CBase::BREAK) )
		{
			It->Draw( lpdevice, is_shader );
		}
	}
}

// 死んでる敵描画
void CManager::DrawDie(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	BOOST_FOREACH( CBase* It, m_EnemyList )
	{
		// 判定
		if( It->GetFlg() & CBase::BREAK )
		{
			It->Draw( lpdevice, false );
		}
	}
}

// 波紋を描画
void CManager::DrawRipples(const LPDIRECT3DDEVICE9 lpdevice)
{
	BOOST_FOREACH( CBase* It, m_EnemyList )
	{
		// 破壊されていなければ描画
		if( !(It->GetFlg() & Enemy::CBase::BREAK) )
		{
			It->DrawRipples( lpdevice );
		}
	}
}

// エフェクト描画
void CManager::DrawEffect(const LPDIRECT3DDEVICE9 lpdevice)
{
	BOOST_FOREACH( CBase* It, m_EnemyList )
	{
		It->DrawEffect( lpdevice );
	}
}

// 敵追加
bool CManager::Add(int kind, const D3DXVECTOR3& pos )
{
	// 有効かどうかを判断する
	if( kind < 0 || kind >= KIND_MAX )
	{
		return false;
	}

	switch( kind )
	{
	case KIND_NORMAL:
		m_EnemyList.push_back( new CNormal( pos.x, pos.y, pos.z ) );
		break;

	case KIND_SHOT:
		m_EnemyList.push_back( new CShoot( pos.x, pos.y, pos.z ) );
		break;

	case KIND_RUSH:
		m_EnemyList.push_back( new CRush( pos.x, pos.y, pos.z ) );
		break;
		
	case KIND_BUBBLEBREAK:
		m_EnemyList.push_back( new CBubbleBreaker( pos.x, pos.y, pos.z ) );
		break;
	}

	return true;
}

// ファイルから読み込み
bool CManager::LoadFromFile( LPCSTR filename )
{
	std::ifstream In( filename );

	if( In.is_open() )
	{
		int		Kind = -1;
		CVector	Pos;

		// 開けたなら末端まで読み込み
		while( In.eof() == false )
		{
			// 種類
			In >> Kind;
			
			// 位置
			In >> Pos.x;
			In >> Pos.y;
			In >> Pos.z;

			// 登録
			Add( Kind, Pos );
		}
	
	}else{

		return false;
	
	}

	return true;
}

// 空にする
void CManager::AllClear()
{
	BOOST_FOREACH( CBase* It, m_EnemyList )
	{	
		SAFE_DELETE( It );
	}

	m_EnemyList.clear();
}

// デストラクタ
CManager::~CManager()
{
	AllClear();
}