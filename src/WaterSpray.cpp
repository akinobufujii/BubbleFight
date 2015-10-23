////////////////////////////
// �C���N���[�h
////////////////////////////
#include "WaterSpray.h"

////////////////////////////
// ����
////////////////////////////

// �R���X�g���N�^
CWaterSpray::CWaterSpray()
{
}

// �f�X�g���N�^
CWaterSpray::~CWaterSpray()
{
	BOOST_FOREACH( CPointSpriteManager* It, m_SprayList )
	{
		SAFE_DELETE( It );
	}

	m_SprayList.clear();
}

// �X�V
void CWaterSpray::Update()
{
	// ��C�ɍX�V
	BOOST_FOREACH( CPointSpriteManager* It, m_SprayList )
	{
		It->Update();
	}
}

// �`��
void CWaterSpray::Draw(const LPDIRECT3DDEVICE9 lp_device)
{
	// ��C�ɕ`��
	BOOST_FOREACH( CPointSpriteManager* It, m_SprayList )
	{
		It->Draw( lp_device );
	}
}

// ����
void CWaterSpray::Create(const D3DXVECTOR3 &pos)
{
	// �����g����Ȃ�g���܂킷
	bool IsCreate = true;

	BOOST_FOREACH( CPointSpriteManager* It, m_SprayList )
	{
		if( It->IsErase() )
		{
			It->SetPos( pos );
			It->Reset();
			IsCreate = false;
			break;
		}
	}

	// �����ς��Ȃ�ǉ�����
	if( IsCreate )
	{
		CPointSpriteManager* lpTemp = NEW CPointSpriteManager();

		lpTemp->SetNum( 100 );
		lpTemp->SetTexture( CSTexManager::GetInst()->GetData( "Spray.png" )->GetTexture() );
		lpTemp->SetEraseTime( 1000 );
		lpTemp->SetMove( D3DXVECTOR3( 1, 1, 1 ) );
		lpTemp->SetScaleMax( 10.f );
		lpTemp->SetPos( pos );

		// �ǉ�
		m_SprayList.push_back( lpTemp );
	}
}