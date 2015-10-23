//////////////////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////////////////
#include "LimitWalls.h"
#include "GameMain.h"

#include "SunLine.h"

#include "SunLineManager.h"

//////////////////////////////////////////////////
// ���O��ԏȗ�
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �R���X�g���N�^
CSunLineManager::CSunLineManager()
{
}

// �f�X�g���N�^
CSunLineManager::~CSunLineManager()
{
	// ���g���
	BOOST_FOREACH( CSunLine* It, m_SunLineList )
	{
		SAFE_DELETE( It );
	}

	m_SunLineList.clear();
}

// �X�V
void CSunLineManager::Update()
{
	// ��C�ɍX�V
	BOOST_FOREACH( CSunLine* It, m_SunLineList )
	{
		It->Update();
	}
}

// �`��
void CSunLineManager::Draw(LPDIRECT3DDEVICE9 lpdevice) const
{
	// �����_�[�X�e�[�g�ݒ�
	// ���Z�����ŕ`��
	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// ���W�ϊ��͕K�v�Ƃ��Ȃ�
	lpdevice->SetTransform( D3DTS_WORLD, &CMatrix() );

	// �e�N�X�`����\��
	lpdevice->SetTexture( 0, *CSTexManager::GetInst()->GetData( "SunLine.png" ) );

	// ��C�ɕ`��
	BOOST_FOREACH( CSunLine* It, m_SunLineList )
	{
		It->Draw( lpdevice );
	}

	// ���Z��������
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}

// �����_���z�u
void CSunLineManager::SetRandom(UINT num)
{
	// ���I�m�ۂ��Ă���
	CSunLine*	lpTemp = NULL;
	CVector		TempPos;
	UINT		Limit = static_cast<UINT>( Sequence::CGameMain::SEA_MOVELIMIT );

	srand( timeGetTime() );
	for( UINT i = 0; i < num; i++ )
	{
		lpTemp = NEW CSunLine();

		// �����Œ�ňʒu����
		TempPos.Set(
			rand() % Limit * GetPosiOrNega(),
			Sequence::CGameMain::SURFACESEA_HEIGHT,
			rand() % Limit * GetPosiOrNega() );

		// �ݒ�
		lpTemp->SetPos( TempPos );

		// �ǉ�
		m_SunLineList.push_back( lpTemp );
	}
}