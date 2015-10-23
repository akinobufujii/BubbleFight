//////////////////////////////
// �C���N���[�h
//////////////////////////////
#include "Plankton.h"
#include "PlanktonManager.h"

//////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////
const LPSTR	CPlanktonManager::TEX_PASS	= "Plankton.png";

//////////////////////////////
// ����
//////////////////////////////

// �R���X�g���N�^
CPlanktonManager::CPlanktonManager()
{
}

// �X�V
void CPlanktonManager::Update()
{
	//PLANKTONLIST::iterator It = m_PlanktonList.begin();

	//// �X�V
	//while( It != m_PlanktonList.end() )
	//{
	//	(*It)->Update( player_pos );

	//	++It;
	//}
}

// �`��
void CPlanktonManager::Draw(const LPDIRECT3DDEVICE9 lpdevice, const CCamera *camera)
{
	lpdevice->SetFVF( CBoard::FVF_VERTEX );
	lpdevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	// �e�N�X�`���ݒ�
	lpdevice->SetTexture( 0, CSTexManager::GetInst()->GetData( TEX_PASS )->GetTexture() );

	// �`��
	BOOST_FOREACH( CPlankton* It, m_PlanktonList )
	{
		It->Draw( lpdevice, camera );
	}
}

// �v�����N�g�������
void CPlanktonManager::SetNum(const UINT num)
{
	// �w�肳�ꂽ���������ꍇ
	if( num > m_PlanktonList.size() )
	{
		// �ǉ�����
		for( int i = num - m_PlanktonList.size(); i > 0; i-- )
		{
			m_PlanktonList.push_back( NEW CPlankton() );
		}
	
	}else if( num < m_PlanktonList.size() ){

		// ���Ȃ��ꍇ�͍폜����
		PLANKTONLIST::iterator It = m_PlanktonList.begin();

		for( int i = m_PlanktonList.size() - num; i > 0 || It != m_PlanktonList.end(); i-- )
		{
			SAFE_DELETE( (*It) );

			It = m_PlanktonList.erase( It );
		}
	}
}

// �f�X�g���N�^
CPlanktonManager::~CPlanktonManager()
{
	// ���
	BOOST_FOREACH( CPlankton* It, m_PlanktonList )
	{
		SAFE_DELETE( It );
	}
}
