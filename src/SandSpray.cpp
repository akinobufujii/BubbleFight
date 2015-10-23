/////////////////////////////////
// �C���N���[�h
/////////////////////////////////
#include "SandSpray.h"

/////////////////////////////////
// ����
/////////////////////////////////

// �R���X�g���N�^
CSandSpray::CSandSpray()
{
}

// �f�X�g���N�^
CSandSpray::~CSandSpray()
{
	BOOST_FOREACH( CPointSpriteManager* It, m_SandList )
	{
		SAFE_DELETE( It );
	}

	m_SandList.clear();
}

// �X�V
void CSandSpray::Update()
{
	// ���r���폜������̂Œʏ탋�[�v
	LISTTYPE::iterator	It = m_SandList.begin();

	while( It != m_SandList.end() )
	{
		// �X�V
		(*It)->Update();

		// �p�Ȃ��Ȃ�
		if( (*It)->IsErase() )
		{
			// ���X�g����폜
			SAFE_DELETE( *It );
			It = m_SandList.erase( It );
			continue;
		}

		++It;
	}
}

// �`��
void CSandSpray::Draw(LPDIRECT3DDEVICE9 lp_device)
{
	BOOST_FOREACH( CPointSpriteManager* It, m_SandList )
	{
		It->Draw( lp_device );
	}
}

// �ǉ�
void CSandSpray::Add(CPointSpriteManager *lp_sand)
{
	// �P���ɒǉ�
	m_SandList.push_back( lp_sand );
}

// ������Őݒ肵�Ēǉ�
void CSandSpray::AddDefault(const D3DXVECTOR3& pos)
{
	// �}�W�b�N�i���o�[���ڂ���
	CPointSpriteManager* lpTemp = NEW CPointSpriteManager();

	// �ݒ�
	lpTemp->SetType( CPointSpriteManager::TYPE_GRAVITYPOP );
	lpTemp->SetNum( 5 );
	lpTemp->SetEraseTime( 100 );
	lpTemp->SetTexture( *CSTexManager::GetInst()->GetData( "Spray.png" ) );
	lpTemp->SetPos( pos );
	lpTemp->SetMove( D3DXVECTOR3( 0.1f, 0.1f, 0.1f ) );
	lpTemp->SetScaleMax( 50 );

	// �ǉ�
	m_SandList.push_back( lpTemp );
}