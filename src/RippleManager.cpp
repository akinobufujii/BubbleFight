//////////////////////////////
// �C���N���[�h
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
// �ÓI�����o���̉�
//////////////////////////////
const LPCSTR	CRippleManager::TEXPATH_DEFAULT_RIPPLE	= "Ripple.png";

//////////////////////////////
// ����
//////////////////////////////

// �R���X�g���N�^
CRippleManager::CRippleManager():
m_lpTex( NULL ),
m_CreateHeight( 0 ),
m_Width( 0 ),
m_Height( 0 )
{
}

// �f�X�g���N�^
CRippleManager::~CRippleManager()
{
	// �폜
	BOOST_FOREACH( CRipple* It, m_RippleList )
	{
		SAFE_DELETE( It );
	}

	m_RippleList.clear();
}

// �X�V
void CRippleManager::Update(const D3DXMATRIX& mat)
{
	// ��{�I�Ƀ��X�g�̓L���[�\���̂悤�Ɉ���
	// �Ȃ�STL��Queue���g��Ȃ���
	// �����o�֐��̑��삪���ɂ�������

	// �v���C���[�����ʕt�߂ɋ��邱�Ƃ��m�F����
	bool Flg = IsUpdate( CVector( mat ) );

	// �������X�g�ɉ����Ȃ����
	if( Flg && m_RippleList.empty() )
	{
		// �ǉ�
		m_RippleList.push_back( Create( mat ) );
	}

	// �Ƃ肠�����X�V
	BOOST_FOREACH( CRipple* It, m_RippleList )
	{
		It->Update();
	}

	// �ȉ��̏����̓��X�g���󂶂�Ȃ���Ώ�������
	if( m_RippleList.empty() == false )
	{
		// �ǉ�����
		// ���������̗v�f���ǉ����ė~�����Ȃ�
		if( Flg && m_RippleList.back()->IsNext() )
		{
			// �����ɒǉ�����
			m_RippleList.push_back( Create( mat ) );
		}

		// �폜����
		// �����擪�̗v�f��������ׂ��Ȃ�
		if( m_RippleList.front()->IsErase() )
		{
			// Delete���ă��X�g����폜(pop)����
			SAFE_DELETE( m_RippleList.front() );

			m_RippleList.pop_front();
		}
	}
}

// �`��
void CRippleManager::Draw(const LPDIRECT3DDEVICE9 lp_device)
{
	// ���C�e�B���O��؂�
	//lp_device->SetRenderState( D3DRS_LIGHTING, FALSE );

	// ���_�t�H�[�}�b�g
	lp_device->SetFVF( CBoard::FVF_VERTEX );

	// �e�N�X�`����ݒ�
	lp_device->SetTexture( 0, m_lpTex );

	// �s��h�~�ׂ̈ɊC�ʂ̏�Ɖ��ɕ`��
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

// �쐬����
CRipple* CRippleManager::Create(const D3DXMATRIX& mat)
{
	// �p�����[�^��ݒ肵�Ēǉ�
	CRipple* lpTemp = NEW CRipple();

	lpTemp->SetScaleRate( 0.05f );
	lpTemp->SetSize( m_Width, m_Height );

	// �p�x�𒊏o
	CVector Vec( 0, 0, 1 );
	CMatrix mInv = mat;

	mInv.GetInverse();

	Vec.GetTransNormal( &mInv );

	Vec.y = 0;
	Vec.GetNormalVector();

	float Degree = Vec.GetDotDegree( &CVector( 0, 0, 1 ) );

	// ��]��������
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

	// �ʒu�𒊏o
	CVector Pos = mat;

	// �C�ʌŒ�
	Pos.y = Sequence::CGameMain::SURFACESEA_HEIGHT;

	lpTemp->SetPos( Pos );

	return lpTemp;
}

// ���A�X�V���s���ׂ����ǂ���
bool CRippleManager::IsUpdate( const D3DXVECTOR3 &pos )
{
	// �C�ʕt�߂Ȃ�true
	if( Sequence::CGameMain::SURFACESEA_HEIGHT - m_CreateHeight < pos.y &&
		Sequence::CGameMain::SURFACESEA_HEIGHT + m_CreateHeight > pos.y )
	{
		return true;
	}

	// �����łȂ����false
	return false;
}