/////////////////////////////////
// �C���N���[�h
/////////////////////////////////
#include "DamageObjectBase.h"

#include "BubbleManager.h"
#include "ShotManager.h"
#include "RippleManager.h"
#include "EnemyBase.h"

#include "EnemyManager.h"
#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "PlanktonManager.h"
#include "GameMain.h"

#include "DamageFish.h"

/////////////////////////////////
// �ÓI�����o���̉�
/////////////////////////////////
const float	CDamageFish::MOVESPEED	= 0.5f;
const float	CDamageFish::MOVEHEIGHT	= Sequence::CGameMain::SURFACESEA_HEIGHT - 5;
const float	CDamageFish::TURNSPEED	= 1.5f;

/////////////////////////////////
// ����
/////////////////////////////////

// �R���X�g���N�^
CDamageFish::CDamageFish()
{
}

// �f�X�g���N�^
CDamageFish::~CDamageFish()
{
}

// �X�V
void CDamageFish::Update()
{
	// ����ɍ����Œ�
	m_mMat._42 = MOVEHEIGHT;

	// �ړ�
	m_mMat.MoveLocal( 0, 0, m_Speed );

	// �����͈͂���o�Ă����
	D3DXVECTOR3 Pos = m_mMat.GetPos();

	if( abs( Pos.x ) > Sequence::CGameMain::SEA_MOVELIMIT ||
		abs( Pos.z ) > Sequence::CGameMain::SEA_MOVELIMIT )
	{
		// ��
		D3DXVECTOR3 Axis;
		float Rot = GetAxisAndRot( &Axis, D3DXVECTOR3( 0, MOVEHEIGHT, 0 ), m_mMat );
		
		if( Rot > TURNSPEED )
		{
			Rot = TURNSPEED;
		}

		m_mMat.RotateLocalAxis( &Axis, Rot );
	}

	MustUpdate();
}

// �`��
void CDamageFish::Draw(const LPDIRECT3DDEVICE9 lp_device, bool is_shader)
{
	// ������J�����O���Ȃ�
	//if( IsCulling( &m_mMat ) )
	{
		CMatrix mScale;

		mScale.SetScale( m_Scale );

		// �V�F�[�_�`�悷��Ȃ�
		// ���Ȃ񂩂��������̂ŕۗ�
		if( false )
		{
			// �������C�g�V�F�[�f�B���O
			// �V�F�[�_�f�o�C�X�l��
			CEffect* lpShader = CSShaderManager::GetInst()->GetData( "Rim.fx" );

			// ���W�ϊ�
			D3DXMATRIX mView, mProj, mWorld = mScale * m_mMat;
			lp_device->GetTransform( D3DTS_VIEW, &mView );
			lp_device->GetTransform( D3DTS_PROJECTION, &mProj );

			lpShader->SetValue( "g_WVP", 
				&(mWorld * mView * mProj),
				sizeof( D3DXMATRIX ) );

			// ���C�g�ʒu
			lpShader->SetValue( "g_LightDir", 
				&GetLightPos( lp_device, mWorld ),
				sizeof( D3DXVECTOR4 ) );

			// ���_�ʒu
			lpShader->SetValue( "g_EyePos", 
				&GetViewPos( lp_device, mWorld ),
				sizeof( D3DXVECTOR4 ) );

			lpShader->Begin();
	
			m_lpMesh->DrawShader( lpShader, "g_Diffuse", "g_Tex" );

			lpShader->End();

		}else{

			// �P�Ȃ�`��
			lp_device->SetTransform( D3DTS_WORLD, &(mScale * m_mMat) );

			m_lpMesh->Draw( lp_device );
		}
	}

	// �A
	m_lpBubbles->DrawNoBreak( lp_device );
}

// �G�ɑ΂���s��
void CDamageFish::Action(Enemy::CBase &object)
{
	CVector Dir = m_mMat.GetPos() - object.GetPos();

	Dir.GetNormalVector();
	Dir *= 0.5f;

	m_mMat.MoveLocal( &Dir );
}