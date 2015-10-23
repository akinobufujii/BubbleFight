//////////////////////////////////////////////////
// �C���N���[�h
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
// ���O��ԏȗ�
//////////////////////////////////////////////////
using namespace Enemy;

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �X�V
void CManager::Update()
{
	// ���r���폜������̂Œʏ탋�[�v
	ENEMYLIST::iterator It = m_EnemyList.begin();

	while( It != m_EnemyList.end() )
	{
		(*It)->Update();
		
		// ��������Ȃ��Ȃ����
		if( (*It)->GetFlg() & CBase::ERASE )
		{
			// �X�R�A���Z
			m_Score += (*It)->GetScore();

			SAFE_DELETE( (*It) );
			It = m_EnemyList.erase( It );
			continue;
		}

		++It;
	}
}

// �����Ă�����̕`��
void CManager::DrawAlive(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	BOOST_FOREACH( CBase* It, m_EnemyList )
	{
		// ����
		if( ~(It->GetFlg() & CBase::BREAK) )
		{
			It->Draw( lpdevice, is_shader );
		}
	}
}

// ����ł�G�`��
void CManager::DrawDie(const LPDIRECT3DDEVICE9 lpdevice, bool is_shader)
{
	BOOST_FOREACH( CBase* It, m_EnemyList )
	{
		// ����
		if( It->GetFlg() & CBase::BREAK )
		{
			It->Draw( lpdevice, false );
		}
	}
}

// �g���`��
void CManager::DrawRipples(const LPDIRECT3DDEVICE9 lpdevice)
{
	BOOST_FOREACH( CBase* It, m_EnemyList )
	{
		// �j�󂳂�Ă��Ȃ���Ε`��
		if( !(It->GetFlg() & Enemy::CBase::BREAK) )
		{
			It->DrawRipples( lpdevice );
		}
	}
}

// �G�t�F�N�g�`��
void CManager::DrawEffect(const LPDIRECT3DDEVICE9 lpdevice)
{
	BOOST_FOREACH( CBase* It, m_EnemyList )
	{
		It->DrawEffect( lpdevice );
	}
}

// �G�ǉ�
bool CManager::Add(int kind, const D3DXVECTOR3& pos )
{
	// �L�����ǂ����𔻒f����
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

// �t�@�C������ǂݍ���
bool CManager::LoadFromFile( LPCSTR filename )
{
	std::ifstream In( filename );

	if( In.is_open() )
	{
		int		Kind = -1;
		CVector	Pos;

		// �J�����Ȃ疖�[�܂œǂݍ���
		while( In.eof() == false )
		{
			// ���
			In >> Kind;
			
			// �ʒu
			In >> Pos.x;
			In >> Pos.y;
			In >> Pos.z;

			// �o�^
			Add( Kind, Pos );
		}
	
	}else{

		return false;
	
	}

	return true;
}

// ��ɂ���
void CManager::AllClear()
{
	BOOST_FOREACH( CBase* It, m_EnemyList )
	{	
		SAFE_DELETE( It );
	}

	m_EnemyList.clear();
}

// �f�X�g���N�^
CManager::~CManager()
{
	AllClear();
}