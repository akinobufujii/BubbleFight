///////////////////////////////
// �C���N���[�h
///////////////////////////////
#include "ObjectFish.h"

#include "DamageObjectBase.h"
#include "DamageFish.h"

#include "FishManager.h"

///////////////////////////////
// �ÓI�����o���̉�
///////////////////////////////
CStoneManager* CFishManager::m_lpStones = NULL;

///////////////////////////////
// ����
///////////////////////////////

// �R���X�g���N�^
CFishManager::CFishManager()
{
}

// �f�X�g���N�^
CFishManager::~CFishManager()
{
	AllRelease();
}

// �X�V
void CFishManager::Update( bool is_collision )
{
	// �I�u�W�F�N�g
	BOOST_FOREACH( CObjectFish* Fish, m_FishList )
	{
		Fish->Update( is_collision );
	}

	// �_���[�W���󂯂Ă���I�u�W�F�N�g
	BOOST_FOREACH( CDamageObjectBase* Fish, m_DamageObjectList )
	{
		Fish->Update();
	}
}

// �P�Ȃ�I�u�W�F�N�g�̋��`��
void CFishManager::DrawObjectFish(const LPDIRECT3DDEVICE9 lp_device)
{
	// �I�u�W�F�N�g
	lp_device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	
	BOOST_FOREACH( CObjectFish* Fish, m_FishList )
	{
		Fish->Draw( lp_device, m_Mat );
	}

	lp_device->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
}

// �_���[�W���󂯂Ă��鋛�̕`��
void CFishManager::DrawDamageFish( const LPDIRECT3DDEVICE9 lp_device, bool is_shader )
{
	// �_���[�W���󂯂Ă���I�u�W�F�N�g
	BOOST_FOREACH( CDamageObjectBase* Fish, m_DamageObjectList )
	{
		Fish->Draw( lp_device, is_shader );
	}
}

// �����Ɣ͈͂�ݒ�
void CFishManager::SetNumAndLimit(const int num, const D3DXVECTOR3 &pos_limit, const int rot)
{
	D3DXVECTOR3 Pos;

	// �w�肳�ꂽ��������
	for( int i = 0; i < num; i++ )
	{
		CObjectFish* lpTemp = NEW CObjectFish();

		Pos.x = (rand() % static_cast<int>(pos_limit.x) + 1) * GetPosiOrNega();
		Pos.y = (rand() % static_cast<int>(pos_limit.y) + 1) * GetPosiOrNega();
		Pos.z = (rand() % static_cast<int>(pos_limit.z) + 1) * GetPosiOrNega();
		
		lpTemp->SetPos( Pos );

		if( rot == -1 )
		{
			lpTemp->SetRotY( rand() % 360 );
		
		}else{

			lpTemp->SetRotY( rot );
		}
		
		m_FishList.push_back( lpTemp );
	}
}

// �_���[�W���󂯂鋛��ǂݍ���
void CFishManager::LoadDamageFish(LPCSTR load_path)
{
	std::ifstream	File( load_path );

	if( File.is_open() )
	{
		CDamageObjectBase*	lpTemp = NULL;	// �e���|����
		D3DXVECTOR3			LoadPos;		// �ʒu
		int					Kind;			// ���
		float				Scale;			// �g�嗦
		float				Speed;			// ����
		std::string			Path;			// ���b�V���p�X

		while( File.eof() == false )
		{
			File >> Kind;

			// �G���[�`�F�b�N
			if( Kind < 0 || Kind >= KINDMAX )
			{
				continue;
			}

			// ���ݏo������
			switch( Kind )
			{
			case KIND_FISH:
				lpTemp = NEW CDamageFish();
				break;
			}

			// �ʒu�ݒ�
			File >> LoadPos.x;
			File >> LoadPos.y;
			File >> LoadPos.z;

			lpTemp->SetPos( LoadPos );

			// �g�嗦�ݒ�
			File >> Scale;

			lpTemp->SetScale( Scale );

			// �����ݒ�
			File >> Speed;

			lpTemp->SetSpeed( Speed );

			// ���b�V���ݒ�
			File >> Path;

			lpTemp->SetMeshPath( D3D::GetDevice(), Path.c_str() );

			m_DamageObjectList.push_back( lpTemp );

			// ���O�̂���
			lpTemp = NULL;
		}
	}
}

// ��]�ݒ�
void CFishManager::SetRot( const int rot )
{
	BOOST_FOREACH( CObjectFish* Fish, m_FishList )
	{
		Fish->SetRotY( rot );
	}
}

// ��ʂ���o�����ǂ���
bool CFishManager::IsAllOut()
{
	BOOST_FOREACH( CObjectFish* Fish, m_FishList )
	{
		if( IsCulling( &Fish->GetMatrix() ) )
		{
			return false;
		}	
	}

	return true;
}

// ��ɂ���
void CFishManager::AllRelease()
{
	BOOST_FOREACH( CObjectFish* Fish, m_FishList )
	{
		SAFE_DELETE( Fish );
	}

	m_FishList.clear();

	BOOST_FOREACH( CDamageObjectBase* Fish, m_DamageObjectList )
	{
		SAFE_DELETE( Fish );
	}

	m_DamageObjectList.clear();
}