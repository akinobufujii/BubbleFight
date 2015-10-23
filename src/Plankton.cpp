////////////////////////////////
// �C���N���[�h
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
// �ÓI�����o���̉�
////////////////////////////////
const float CPlankton::SIZE	= 0.2f;
CBoard		CPlankton::m_Board( SIZE );

////////////////////////////////
// ����
////////////////////////////////

// �R���X�g���N�^
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

// �X�V
void CPlankton::Update()
{
}

// �`��
void CPlankton::Draw( const LPDIRECT3DDEVICE9 lpdevice, const CCamera* camera )
{
	if( IsCulling( &CMatrix( m_vPos ) ) )
	{
		// �r���{�[�h�ׂ̈̉�]�s��l��
		D3DXMATRIX mRot;

		camera->GetBillboard( &mRot, &CMatrix( m_vPos ) );
		lpdevice->SetTransform( D3DTS_WORLD, &( mRot * CMatrix( m_vPos ) ) );

		// �`��
		m_Board.Draw( lpdevice );
	}
}

// �f�X�g���N�^
CPlankton::~CPlankton()
{
}
