/////////////////////////////////
// �C���N���[�h
/////////////////////////////////
#include "BubbleManager.h"

#include "FinTail.h"

/////////////////////////////////
// �ÓI�����o���̉�
/////////////////////////////////
const BYTE	CFinTail::FLG_CNT_PLUS	= 0x01;

const BYTE			CFinTail::MOVE_MAX			= 30;
const BYTE			CFinTail::SPEED_RATE		= 12;
const float			CFinTail::DEFAULT_MOVE_SPEED= 0.05f;
const D3DXVECTOR3	CFinTail::DEFAULT_FARPOS	= D3DXVECTOR3( 0, 0, -0.8f );
const LPSTR			CFinTail::MESHPASS_FIN		= "Fin.x";

/////////////////////////////////
// ����
/////////////////////////////////

// �R���X�g���N�^
CFinTail::CFinTail():
m_lpMesh( CSXfileManager::GetInst()->GetData( MESHPASS_FIN ) ),
m_MoveCnt( 0.f ),
m_Flg( 0 ),
m_MoveType( MOVE_SIDE ),
m_FarPos( DEFAULT_FARPOS ),
m_NowRotMax( MOVE_MAX ),
m_IsCreateBubble( true )
{
}

// �f�X�g���N�^
CFinTail::~CFinTail()
{
}

// �X�V
void CFinTail::Update( const D3DXMATRIX& mat, float speed )
{
	CMatrix RotY, Trans;

	// �����Ă����
	if( speed > 0.2f )
	{
		if( m_IsCreateBubble == false )
		{
			m_IsCreateBubble = true;
		
		}else{

			// ��]
			if( m_MoveType == MOVE_SIDE )
			{
				// �A�̏o���ʒu�����߂�
				CMatrix mTemp = m_mMat;

				mTemp.MoveLocal( 0, -m_lpMesh->GetRadius() / 2, -m_lpMesh->GetRadius() / 2 );

				m_Bubbles.Add( &mTemp, 1 );

				mTemp.MoveLocal( 0, m_lpMesh->GetRadius(), 0 );

				m_Bubbles.Add( &mTemp, 1 );

			}else if( m_MoveType == MOVE_TOP ){

				// �A�̏o���ʒu�����߂�
				CMatrix mTemp = m_mMat;

				mTemp.MoveLocal( -m_lpMesh->GetRadius() / 2, 0, -m_lpMesh->GetRadius() / 2 );

				m_Bubbles.Add( &mTemp, 1 );

				mTemp.MoveLocal( m_lpMesh->GetRadius(), 0, 0 );

				m_Bubbles.Add( &mTemp, 1 );
			}

			m_IsCreateBubble = false;
		}
	}

	// �e��藣���
	Trans.SetTrans( &m_FarPos );

	speed += DEFAULT_MOVE_SPEED;

	// �t���O�ŉ��Z�����Z�����߂�
	if( m_Flg & FLG_CNT_PLUS )
	{
		if( (m_MoveCnt += abs( speed ) * SPEED_RATE) > m_NowRotMax )
		{
			m_Flg ^= FLG_CNT_PLUS;
		}
	
	}else{

		if( (m_MoveCnt -= abs( speed ) * SPEED_RATE) < -m_NowRotMax )
		{
			m_Flg ^= FLG_CNT_PLUS;
		}
	}

	// ��]
	if( m_MoveType == MOVE_SIDE )
	{
		RotY.RotateLocalY( m_MoveCnt );

	}else if( m_MoveType == MOVE_TOP )
	{
		RotY.RotateLocalX( m_MoveCnt );
	}
	
	m_mMat = Trans * RotY * mat;

	m_Bubbles.Update();
}

// �`��
void CFinTail::Draw( const LPDIRECT3DDEVICE9& lpdevice )
{
	if( IsCulling( &m_mMat ) )
	{
		lpdevice->SetTransform( D3DTS_WORLD, &m_mMat );
	
		m_lpMesh->Draw( lpdevice );
	}

	m_Bubbles.DrawNoBreak( lpdevice );
}

// �`��
void CFinTail::DrawShader( const LPDIRECT3DDEVICE9& lpdevice )
{
	if( IsCulling( &m_mMat ) )
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

		m_lpMesh->DrawShader( lpSharder, "g_Diffuse", "g_Tex" );

		lpSharder->End();
	}

	m_Bubbles.DrawNoBreak( lpdevice );
}