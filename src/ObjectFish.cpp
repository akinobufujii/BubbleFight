//////////////////////////////
// �C���N���[�h
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
// �ÓI�����o���̉�
//////////////////////////////
const float	CObjectFish::MOVE_SPEED		= -0.2f;
const float	CObjectFish::SIZE			= 2;
const float	CObjectFish::TEXPOS_WIDTH	= 1.f / 2;
const float	CObjectFish::TEXPOS_HEIGHT	= 1.f / 4;
const BYTE	CObjectFish::NODE_RANGE		= 4;
const BYTE	CObjectFish::KIND_NUM		= 8;

const CObjectFish::FLGTYPE	CObjectFish::FLG_TURN	= 0x01;

//////////////////////////////
// ����
//////////////////////////////

// �R���X�g���N�^
CObjectFish::CObjectFish():
m_Flg( 0 )
{
	m_Board.SetVertex( SIZE );

	// �e�N�X�`�����W����
	int Kind	= rand() % KIND_NUM;
	int Width	= Kind / 2;
	int Height	= Kind / 4;

	m_Board.SetTexturePos(
		&D3DXVECTOR2( TEXPOS_WIDTH * Width,			TEXPOS_HEIGHT * Height		),
		&D3DXVECTOR2( TEXPOS_WIDTH * (Width + 1),	TEXPOS_HEIGHT * Height		),
		&D3DXVECTOR2( TEXPOS_WIDTH * (Width + 1),	TEXPOS_HEIGHT * (Height + 1)),
		&D3DXVECTOR2( TEXPOS_WIDTH * Width,			TEXPOS_HEIGHT * (Height + 1)) );
}

// �f�X�g���N�^
CObjectFish::~CObjectFish()
{
}

// �X�V
void CObjectFish::Update( bool is_collision )
{
	// �Y��
	m_mMat.MoveLocal( MOVE_SPEED, 0, 0 );

	// �͈͔���
	CVector Pos = m_mMat;

	// ���̋��E���z������
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

	// ��Ƃ̓����蔻��
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

	// �q�I�u�W�F�N�g�ɂ�������������
	// �q�I�u�W�F�N�g�͐e��ǂ�������
	BOOST_FOREACH( CMatrix& i, m_NodeMat )
	{
		// ��]���Ɗp�x�����߂�
		D3DXVECTOR3 RotAxis;
		float Ang = GetAxisAndRot( &RotAxis, m_mMat.GetPos(), i );

		// �w�莲�ŉ�]
		//i.RotateLocalAxis( &RotAxis, Ang );

		i.MoveLocal( MOVE_SPEED, 0, 0 );
	}
}

// �`��
void CObjectFish::Draw(const LPDIRECT3DDEVICE9 lp_device, const D3DXMATRIX& mat )
{
	lp_device->SetFVF( CBoard::FVF_VERTEX );

	lp_device->SetTexture( 0, CSTexManager::GetInst()->GetData( "Fish.png" )->GetTexture() );

	// �e
	if( IsCulling( &(m_mMat * mat) ) )
	{
		lp_device->SetTransform( D3DTS_WORLD, &(m_mMat * mat) );

		m_Board.Draw( lp_device );
	}

	// �q
	//CMatrix mWorld = m_mMat;

	BOOST_FOREACH( CMatrix& i, m_NodeMat )
	{
		//if( IsCulling( &(i * mat) ) )
		{
			// ��]�𔲂��ɂ���(�e�̌����ɍ��킹��)
			/*mWorld._41 = i.GetPos().x;
			mWorld._42 = i.GetPos().y;
			mWorld._43 = i.GetPos().z;*/

			lp_device->SetTransform( D3DTS_WORLD, &(i * mat) );

			m_Board.Draw( lp_device );
		}
	}
}

// �ʒu�ݒ�
void CObjectFish::SetPos( const D3DXVECTOR3& pos )
{
	// �e�̈ʒu�ݒ�
	m_mMat.SetTrans( &pos );

	// �q�������_���Ȉʒu��
	BOOST_FOREACH( CMatrix& i, m_NodeMat )
	{
		i.SetTrans(
			pos.x + ( rand() % NODE_RANGE * GetPosiOrNega() ),
			pos.y + ( rand() % NODE_RANGE * GetPosiOrNega() ),
			pos.z + ( rand() % NODE_RANGE * GetPosiOrNega() ) );
	}
}

// ��]�ݒ�
void CObjectFish::SetRotY( const float rot )
{
	m_mMat.RotateLocalY( rot );

	// �q
	BOOST_FOREACH( CMatrix& i, m_NodeMat )
	{
		i.RotateLocalY( rot );
	}
}