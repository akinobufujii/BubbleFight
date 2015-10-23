//////////////////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////////////////
#include "BubbleManager.h"

#include "Fintail.h"
#include "Screw.h"

#include "RippleManager.h"
#include "ShotManager.h"
#include "ShotBase.h"
#include "StrongDolphin.h"

#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "GameMain.h"

#include "DamageObjectBase.h"

#include "WaterSpray.h"

#include "PointSpriteManager.h"
#include "EnemyBase.h"

//////////////////////////////////////////////////
// ���O��Ԃ̏ȗ�
//////////////////////////////////////////////////
using namespace Enemy;

//////////////////////////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////////////////////////
Player::CDolphin*	CBase::m_lpPlayer	= NULL;
CStoneManager*		CBase::m_lpStone	= NULL;
CFishManager*		CBase::m_lpFishes	= NULL;

const float CBase::PARTICLE_POWER		= 0.5f;
const float CBase::FLOAT_POWER_LINE		= 0.2f;
const float CBase::FLOAT_SPEED			= 0.01f;
const float CBase::SCALE_UP_SPEED		= 0.01f;
const float CBase::DEFAULT_FLAOT_POWER	= 0.065f;
const float CBase::DEFAULT_HITPOWER_OFF	= 0.2f;
const float	CBase::DEFAULT_ROTSPEED		= 10.f;
const BYTE	CBase::ERASE_LENGTH			= 25;
const BYTE	CBase::BUBBLE_SCALE_RATE	= 30;

const BYTE	CBase::ERASE		= 0x01;
const BYTE	CBase::BREAK		= 0x02;
const BYTE	CBase::BUBBLE_HIT	= 0x04;
const BYTE	CBase::SEARCH		= 0x08;
const BYTE	CBase::ATTACK		= 0x10;
const BYTE	CBase::BUBBLE_WRAP	= 0x20;
const BYTE	CBase::JUMP			= 0x40;

const int	CBase::BURST_WIDTH				= 256;
const int	CBase::BURST_HEIGHT				= 256;
const int	CBase::BURST_ANIME_SIZE			= 32;
const int	CBase::BURST_ANIME_WIDTH_NUM	= 8;
const int	CBase::BURST_ANIME_HEIGHT_NUM	= 8;
const int	CBase::BURST_ANIME_NUM			= CBase::BURST_ANIME_WIDTH_NUM * (CBase::BURST_ANIME_HEIGHT_NUM - 1);

const LPSTR	CBase::BUBBLE_MESH_PASS	= "Bubble.x";

////////////////////////////////
// ����
////////////////////////////////

// �R���X�g���N�^
CBase::CBase():
m_Flg( 0 ),
m_FloatPower( 0.f ),
m_AnimeCnt( 0 ),
m_NowFloatSpeed( 0.f ),
m_NowScale( 0.f ),
m_HitPower( 0.f ),
m_lpTargetFish( NULL ),
m_JumpSpeed( 0.f ),
m_lpSpray( NEW CWaterSpray() )
{
	// �g��̐ݒ�
	m_Ripples.SetDefaultTexture();
}

// �f�X�g���N�^
CBase::~CBase()
{
	SAFE_DELETE( m_lpSpray );
}

// �v���C���[�Ɍ������ĉ�]����
void CBase::UpdateRot( float rotmax )
{
	// ��]���Ɗp�x�����߂�
	D3DXVECTOR3 RotAxis;
	float Ang = GetAxisAndRot( &RotAxis, m_lpPlayer->GetPos(), m_mMat );

	// ��]�p�x�ő�l�v�Z
	if( Ang > rotmax )
	{
		Ang = rotmax;
	}

	// �w�莲�ŉ�]
	m_mMat.RotateLocalAxis( &RotAxis, Ang );
}

// �j�󂳂ꂽ�G��`��
void CBase::DrawBreakEnemy( const LPDIRECT3DDEVICE9 lpdevice )
{
	CCamera Camera = *Sequence::CGameMain::GetCamera();

	CMatrix Mat, mTrans;

	lpdevice->SetFVF( CBoard::FVF_VERTEX );

	Camera.GetBillboard( &Mat, &m_mMat );

	mTrans.SetTrans( &m_mMat.GetPos(  ) );

	Mat = Mat * mTrans;
	lpdevice->SetTransform( D3DTS_WORLD, &Mat );

	// ���C�e�B���O��؂�
	lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpdevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	lpdevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	lpdevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	
	lpdevice->SetFVF( CBoard::FVF_VERTEX );

	// �p�[�e�B�N���`��
	m_Particle.Draw( lpdevice );

	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	lpdevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	// �A�`��
	m_Bubbles.DrawNoBreak( lpdevice );
	m_Bubbles.DrawBreak( lpdevice );
}

// �v���O���}�u���V�F�[�_�ŕ`��
void CBase::DrawShader(const LPDIRECT3DDEVICE9 lpdevice, CXfile* mesh)
{
	CEffect* lpSharder = CSShaderManager::GetInst()->GetData( "Phong.fx" );

	// �Œ�p�C�v���C���̏����l������
	CMatrix	mView, mProj;
	lpdevice->GetTransform( D3DTS_PROJECTION, &mProj );
	lpdevice->GetTransform( D3DTS_VIEW, &mView );

	// ���W�ϊ�
	lpSharder->SetValue( "g_WVP",
		&(m_mMat * mView * mProj),
		sizeof( D3DXMATRIX ) );

	// ���C�g�ʒu�v�Z
	lpSharder->SetValue( "g_LightDir",
		&GetLightPos( lpdevice, m_mMat ),
		sizeof( D3DXVECTOR4 ) );

	// �J�����ʒu�v�Z
	lpSharder->SetValue( "g_EyePos",
		&GetViewPos( lpdevice, m_mMat ),
		sizeof( D3DXVECTOR4 ) );

	// �`��
	lpSharder->Begin();

	mesh->DrawShader( lpSharder, "g_Diffuse", "g_Tex" );

	lpSharder->End();
}

// �e�ɑ΂��Ă̍s��
void CBase::Action( Shot::CBase& object )
{
	//m_Flg |= BREAK;	// ����

	// ���˕Ԃ��ꂽ�e�Ȃ�
	if( object.GetFlg() & Shot::CBase::FLG_FIN_HIT )
	{
		m_Flg |= BREAK;	// ����

		m_Particle.SetNum( 100 );
		m_Particle.SetPos( m_mMat.GetPos(  ) );
		m_Particle.SetScaleMax( 2.f );
		m_Particle.SetEraseTime( 1000 );
		m_Particle.SetMove( D3DXVECTOR3( PARTICLE_POWER, PARTICLE_POWER, PARTICLE_POWER ) );
		m_Particle.SetTexture( CSTexManager::GetInst()->GetData( "Sprite.jpg" )->GetTexture() );

	}else{

		m_FloatPower += object.GetPower() * DEFAULT_FLAOT_POWER;	// ���͉��Z

		// ���ȏ�H�������
		if( m_FloatPower >= FLOAT_POWER_LINE )
		{
			m_Flg |= BUBBLE_WRAP;
		}
	}
}

// �G���m�̍s��
void CBase::Action(Enemy::CBase &object)
{
	// ������ѕ�������
	m_HitDir = m_mMat.GetPos() - object.GetPos();

	// ���K��
	m_HitDir.GetNormalVector();

	// ������ї̓��Z�b�g(���)
	m_HitPower = 1.f;

	if( m_FloatPower < FLOAT_POWER_LINE )
	{
		m_Flg &= ~BUBBLE_WRAP;
	}
}

// ���W��Ԃ�
D3DXVECTOR3 CBase::GetPos()
{
	return m_mMat.GetPos();
}

// ���͍X�V
void CBase::UpdateFloatPower()
{
	// ����
	if( (m_NowFloatSpeed += FLOAT_SPEED) > m_FloatPower )
	{
		m_NowFloatSpeed = m_FloatPower;
	}

	CMatrix Power( 0.f, m_NowFloatSpeed, 0.f );

	m_mMat = m_mMat * Power;

	// ���������f
	D3DXVECTOR3 Pos( 0, 0, 0 ), EPos( 0, 0, 0 );
	D3DXVec3TransformCoord( &Pos, &Pos, &GetPlayerInfo()->GetMatrix() );
	D3DXVec3TransformCoord( &EPos, &EPos, &m_mMat );

	if( abs( Pos.y - EPos.y ) > ERASE_LENGTH			||
		EPos.y > Sequence::CGameMain::SURFACESEA_HEIGHT )
	{
		m_Particle.SetNum( 100 );
		m_Particle.SetPos( m_mMat.GetPos() );
		m_Particle.SetScaleMax( 2.f );
		m_Particle.SetEraseTime( 1000 );
		m_Particle.SetMove( D3DXVECTOR3( PARTICLE_POWER, PARTICLE_POWER, PARTICLE_POWER ) );

		CTexture* lpTex = CSTexManager::GetInst()->GetData( "Sprite.jpg" );

		m_Particle.SetTexture( *lpTex );
		
		m_Flg |= BREAK;
	}

	//// �C�ʂɏo���������
	//if( EPos.y > Sequence::CGameMain::SURFACESEA_HEIGHT )
	//{
	//	m_Flg |= BREAK;

	//	CTexture* lpTex = CSTexManager::GetInst()->GetData( "Sprite.jpg" );
	//	m_Particle.SetTexture( lpTex->GetTexture() );
	//}
}

// �A(�G���ޖA)�`��
void CBase::DrawBubble( const LPDIRECT3DDEVICE9 lpdevice )
{
	if( m_FloatPower > 0.f )
	{
		lpdevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
		lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCALPHA );
	
		lpdevice->SetTransform( D3DTS_WORLD, &m_mBalloon );
		
		CSXfileManager::GetInst()->GetData( BUBBLE_MESH_PASS )->Draw( lpdevice );
	
		lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	}
}

// �A�̊g��X�V
void CBase::UpdateBubbleScale()
{
	CMatrix mScale;

	// ���X�Ɋg�債�Ă���
	if( (m_NowScale += SCALE_UP_SPEED) > m_FloatPower )
	{
		m_NowScale = m_FloatPower;
	}

	mScale.SetScale( 1.f + m_NowScale * BUBBLE_SCALE_RATE );

	m_mBalloon = mScale * m_mMat;
}

// �A���z�����(BubbleBreaker�������g��)
void CBase::VacuumBubble( float rate )
{
	CMatrix mScale;

	if( (m_FloatPower -= rate) < 0 )
	{
		m_FloatPower = 0;
	}

	if( (m_FloatPower -= SCALE_UP_SPEED) < 0 )
	{
		m_FloatPower = 0;
	}

	mScale.SetScale( 1.f + m_NowScale * BUBBLE_SCALE_RATE );

	m_mBalloon = mScale * m_mMat;
}

// ������ԍX�V
void CBase::UpdateHitMove()
{
	if( m_HitPower > 0 )
	{
		CVector Temp = m_HitDir * m_HitPower;

		UpdateCollisionMove( &Temp, 3  );
		//m_mMat.MoveLocal( Temp.x, Temp.y, Temp.z );

		m_HitPower -= DEFAULT_HITPOWER_OFF;
	}
}

// �p���悪�K���Ăяo���Ăق�������
void CBase::UpdateMust()
{
	// �C�ʈȏ�Ȃ�
	//if( m_mMat._42 > Sequence::CGameMain::SURFACESEA_HEIGHT )
	//{
	//	m_mMat._42 = Sequence::CGameMain::SURFACESEA_HEIGHT;
	//
	//}else 

	if( m_mMat._42 < Sequence::CGameMain::BOTTOMSEA_HEIGHT ){
		
		// �C��ȉ��Ȃ�
		m_mMat._42 = Sequence::CGameMain::BOTTOMSEA_HEIGHT;
	}

	// ������ɏC������(�p���𐳂�)
	// �ǂꂭ�炢�X���Ă��邩�v�Z����
	CVector Vec( 0, 1, 0 );

	// �t�s��쐬
	CMatrix mInv = m_mMat;
	mInv.GetInverse();

	// ���������߂�
	Vec.GetTransNormal( &mInv );

	// �X���݂̂̃x�N�g���ɂ���
	Vec.z = 0;
	Vec.GetNormalVector();

	// �p�x���o
	float Degree = Vec.GetDotDegree( &CVector( 0, 1, 0 ) );

	// ��]
	if( abs( Degree ) > 1 )
	{
		if( Vec.x < 0 )
		{
			m_mMat.RotateLocalZ( 1 );

		}else{

			m_mMat.RotateLocalZ( -1 );
		}
	}

	// �g��̍X�V
	m_Ripples.Update( m_mMat );

	// �����Ԃ�
	m_lpSpray->Update();
}

// �Փ˔���s���Ȃ���ړ�
bool CBase::UpdateCollisionMove( const D3DXVECTOR3* move, const float limit )
{
	// �����ƕ����𒊏o
	float Length = D3DXVec3Length( move );

	CVector Direction;
	D3DXVec3Normalize( &Direction, move );

	Direction.GetTransNormal( &m_mMat );

	bool IsMove = true;

	CMatrix mScale;

	DWORD PolyNo;

	// ��Ƃ̏Փ˔���
	BOOST_FOREACH( CStoneManager::STONEDATA* Stone, *m_lpStone->GetList() )
	{
		mScale.SetScale( Stone->Scale );

		// ����
		float Distance = IntersectByMesh( 
			&m_mMat,
			&Direction,
			&( mScale * CMatrix( Stone->Pos ) ),
			*Stone->lpMesh->GetMeshData(),
			&PolyNo);

		// ����
		if( Distance > 0 && Distance - Length < 2 )
		{
			// ���̑O�������𒲂ׂ�
			CVector	NowFront( 0, 0, 1 );

			NowFront.GetTransNormal( &m_mMat );

			// ����x�N�g���v�Z
			D3DXVECTOR3 SlipVec = GetSlipVec( NowFront, Stone->lpMesh->GetNormal( PolyNo ) );

			// ����x�N�g���ł�����x����
			DWORD SlipPoly;

			float SlipDistance = IntersectByMesh( 
				&m_mMat,
				&SlipVec,
				&( mScale * CMatrix( Stone->Pos ) ),
				*Stone->lpMesh->GetMeshData(),
				&SlipPoly );

			D3DXVECTOR3 TempVec;

			if( SlipDistance > 0 && SlipDistance - Length < 2 )
			{
				// ����x�N�g���œ��������Ȃ�~�߂�
				TempVec = D3DXVECTOR3( 0, 0, 0 );
		
			}else{
				
				// �����łȂ���΍ŏ��̃x�N�g�����g��
				TempVec = SlipVec;
			}
			
			// �ړ�
			m_mMat._41 += TempVec.x;
			m_mMat._42 += TempVec.y;
			m_mMat._43 += TempVec.z;

			IsMove = false;
			break;
		}
	}

	// ������Ȃ瓮��
	if( IsMove )
	{
		// ��܂�Ă���A�̕��A��������
		float Temp = (FLOAT_POWER_LINE - m_FloatPower) / FLOAT_POWER_LINE;
		m_mMat.MoveLocal( &(*move * max( Temp, 0.5f )) );
	}

	// �������������ĂȂ�����Ԃ�
	return IsMove;
}

// �ǂ�������ڕW�����肷��
void CBase::SearchTarget()
{
	CFishManager::DAMAGEFISHLIST* lpList = m_lpFishes->GetDamageList();
	CVector Pos = m_mMat.GetPos();

	BOOST_FOREACH( CDamageObjectBase* Fish, *lpList )
	{
		// �ڕW�������Ă��Ȃ��Ȃ�
		if( m_lpTargetFish == NULL )
		{
			// �Ƃ肠�����ڕW��
			m_lpTargetFish = Fish;

		}else{

			// �������߂�
			CVector	Old = Pos - m_lpTargetFish->GetPos();
			CVector	New = Pos - Fish->GetPos();

			// �������Z����ΐV���ȖڕW��
			if( New.GetLength() < Old.GetLength() )
			{
				m_lpTargetFish = Fish;
			}
		}
	}
}

// �W�����v�X�V
void CBase::UpdateJump( float speed )
{
	// �i��
	m_mMat.MoveLocal( 0, 0, speed + (m_JumpSpeed += 0.01f) );

	// ��Ύ���Y�����ɗ��Ƃ�
	CMatrix mTrans( 0, -0.01f, 0 );
	m_mMat *= mTrans;

	// ������ɏC������(�p���𐳂�)
	// �ǂꂭ�炢�X���Ă��邩�v�Z����
	CVector Vec( 0, 1, 0 );

	// �t�s��쐬
	CMatrix mInv = m_mMat;
	mInv.GetInverse();

	// ���������߂�
	Vec.GetTransNormal( &mInv );

	// �X���݂̂̃x�N�g���ɂ���
	Vec.z = 0;
	Vec.GetNormalVector();

	// �p�x���o
	float Degree = Vec.GetDotDegree( &CVector( 0, 1, 0 ) );

	// ��]
	if( abs( Degree ) > 1 )
	{
		if( Vec.x < 0 )
		{
			m_mMat.RotateLocalZ( 5 );

		}else{

			m_mMat.RotateLocalZ( -5 );
		}
	}

	// X�����Ȃ���
	m_mMat.RotateLocalX( 2 );

	// ���ʂ�艺�ɂȂ�΃t���O��������
	if( m_mMat.GetPos().y < Sequence::CGameMain::SURFACESEA_HEIGHT )
	{
		m_JumpSpeed = 0.f;

		// �����Ԃ��ǉ�
		m_lpSpray->Create( m_mMat.GetPos() );

		m_Flg &= ~JUMP;
	}
}

// �W�����v���f
void CBase::JudgeJump()
{
	if( Sequence::CGameMain::SURFACESEA_HEIGHT < m_mMat.GetPos().y )
	{
		m_lpSpray->Create( m_mMat.GetPos() );
		m_Flg |= JUMP;
	}
}