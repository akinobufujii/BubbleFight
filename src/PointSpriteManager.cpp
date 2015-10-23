///////////////////////////////
// �C���N���[�h
///////////////////////////////
#include "FishManager.h"
#include "LimitWalls.h"
#include "StoneManager.h"
#include "UI.h"
#include "PlanktonManager.h"
#include "EnemyManager.h"
#include "GameMain.h"

#include "PointSpriteManager.h"

///////////////////////////////
// �ÓI�����o���̉�
///////////////////////////////
const DWORD CPointSpriteManager::CUSTOM_FVF		= (D3DFVF_DIFFUSE | D3DFVF_PSIZE | D3DFVF_XYZ);
const int	CPointSpriteManager::BYTE_SIZE		= sizeof(POINTSPRITE);
const float	CPointSpriteManager::DEFAULT_MAXSIZE= 2.f;
const float CPointSpriteManager::GRAVITY_SPEED	= 0.1f;

///////////////////////////////
// ����
///////////////////////////////

// �R���X�g���N�^
CPointSpriteManager::CPointSpriteManager():
m_lpTex( NULL ),
m_ScaleMax( DEFAULT_MAXSIZE ),
m_EraseTime( 0xffffffff ),
m_SaveTime( 0 ),
m_Num( 0 ),
m_UpdateNum( 0 ),
m_NowType( TYPE_POP ),
m_NowGravity( 0 ),
m_fpUpdateFunc( &CPointSpriteManager::UpdatePop ),
m_lpShader( NULL )
{
}

// �X�V
void CPointSpriteManager::Update()
{
	if( m_fpUpdateFunc )
	{
		(this->*m_fpUpdateFunc)();

	}else{

		return;
	}	
}

// �`��
void CPointSpriteManager::Draw(const LPDIRECT3DDEVICE9& lpdevice)
{
	if( m_Num == 0 )
	{
		return;
	}

	// �f�o�C�X�l��
	if( m_lpShader == NULL )
	{
		m_lpShader = CSShaderManager::GetInst()->Load( lpdevice, "PointSprite.fx" );
	}

	// �e�N�X�`���ݒ�
	m_lpShader->SetTexture( "g_Texture", m_lpTex );

	// �����ݒ�
	m_lpShader->SetValue( "g_PointC", &m_ScaleMax, sizeof( m_ScaleMax ) );

	// �Œ�p�C�v���C������s����擾
	D3DXMATRIX mView, mProj;
	lpdevice->GetTransform( D3DTS_VIEW, &mView );
	lpdevice->GetTransform( D3DTS_PROJECTION, &mProj );

	// �s����V�F�[�_�ɗ���
	m_lpShader->SetValue( "g_VP", &(mView * mProj), sizeof( D3DXMATRIX ) );
	
	// �`��
	m_lpShader->Begin();
	m_lpShader->BeginPass( 0 );

	lpdevice->SetFVF( CUSTOM_FVF );
	lpdevice->DrawPrimitiveUP( D3DPT_POINTLIST, m_Num, &m_Sprite.front(), BYTE_SIZE );
	m_lpShader->EndPass();
	m_lpShader->End();
}

// �����鎞�Ԑݒ�
void CPointSpriteManager::SetEraseTime( const DWORD time )
{
	m_EraseTime = time;

	switch( m_NowType )
	{
	case TYPE_POP:

		m_AlphaCnt = 255.f / time;

		m_SaveTime = timeGetTime();

		m_EndTime = time;

		break;

	case TYPE_SPOUT:

		BOOST_FOREACH( DWORD& Time, m_StartTimes)
		{
			Time = timeGetTime();
		}

		break;

	case TYPE_GRAVITYPOP:

		BOOST_FOREACH( DWORD& Time, m_StartTimes)
		{
			Time = timeGetTime();
		}

		break;
	}
}

// �p�[�e�B�N�����ݒ�
void CPointSpriteManager::SetNum( const int num )
{
	// �ő吔�X�V
	m_Num = num;

	m_UpdateNum = 0;

	// ��x���
	m_Sprite.clear();
	m_MoveDir.clear();

	// �m��
	m_Sprite.resize( m_Num );
	m_MoveDir.resize( m_Num );

	if( m_NowType == TYPE_SPOUT )
	{
		m_StartTimes.resize( m_Num );
	}
}

// �ʒu�ݒ�
void CPointSpriteManager::SetPos( const D3DXVECTOR3& pos )
{
	m_SavePos = pos;

	BOOST_FOREACH( POINTSPRITE& Sprite, m_Sprite )
	{
		Sprite.Pos = pos;
	}
}

// �f�X�g���N�^
CPointSpriteManager::~CPointSpriteManager()
{
	// ���
	m_StartTimes.clear();
	m_Sprite.clear();
	m_MoveDir.clear();
}

// �e����`�ōX�V
void CPointSpriteManager::UpdatePop()
{
	// ���Ԓ����Ă���΍X�V���Ȃ�
	if( timeGetTime() - m_SaveTime > m_EndTime )
	{
		// �O�̂��ߓ����ɂ��Ă���
		for( int i = 0; i < m_Num; i++ )
		{
			m_Sprite[ i ].Color = D3DCOLOR_ARGB( 0, 0xff, 0xff, 0xff );
		}

		return;
	}

	DWORD NowTime = timeGetTime() - m_SaveTime;

	// ������
	for( int i = 0; i < m_Num; i++ )
	{
		m_MoveDir[ i ] *= 0.95f;	// ����

		m_MoveDir[ i ].x	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);
		m_MoveDir[ i ].y	+= static_cast<float>( rand() % 10 ) / 100.f * 2;
		m_MoveDir[ i ].z	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);

		m_Sprite[ i ].Pos += m_MoveDir[ i ];

		// �C�ʂ���o����
		if( m_Sprite[ i ].Pos.y > Sequence::CGameMain::SURFACESEA_HEIGHT - 1)
		{
			// �Ƃǂ܂�
			m_Sprite[ i ].Pos.y = Sequence::CGameMain::SURFACESEA_HEIGHT - 1;
		}

		m_Sprite[ i ].Color = D3DCOLOR_ARGB( 0xff - static_cast<int>(NowTime * m_AlphaCnt), 0xff, 0xff, 0xff );
	}
}

// �����n�ōX�V
void CPointSpriteManager::UpdateSpout()
{
	// �X�V���鐔�����X�ɑ��₷
	if( m_Num < ++m_UpdateNum )
	{
		m_UpdateNum = m_Num;
	
	}else{

		// �J�n���ԋL��
		m_StartTimes[ m_UpdateNum - 1 ] = timeGetTime();
	}

	DWORD NowTime, MoveTime;// ���̎��ԁA����������	

	// ������
	for( int i = 0; i < m_UpdateNum; i++ )
	{
		// ���Ԋi�[
		NowTime = timeGetTime();
		MoveTime = NowTime - m_StartTimes[ i ];

		m_MoveDir[ i ] *= 0.95f;	// ����

		float a = (float)(m_EraseTime - MoveTime) / (float)m_EraseTime;

		m_MoveDir[ i ].x	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);
		m_MoveDir[ i ].y	+= static_cast<float>( rand() % 10 ) / 100.f;
		m_MoveDir[ i ].z	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);

		m_Sprite[ i ].Pos += m_MoveDir[ i ];

		//m_lpSprite[ i ].Scale -= m_ScaleRate;

		m_Sprite[ i ].Color = D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff );

		// �莞���߂����烊�Z�b�g
		if( MoveTime > m_EraseTime )
		{
			m_Sprite[ i ].Pos = m_SavePos;

			m_StartTimes[ i ] = timeGetTime();

			m_MoveDir[ i ].x = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.x - (m_SaveMoveMax.x / 2);
			m_MoveDir[ i ].y = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.y;
			m_MoveDir[ i ].z = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.z - (m_SaveMoveMax.z / 2);
		}
	}
}

// �d�͕t���e����X�V
void CPointSpriteManager::UpdateGravityPop()
{
	// ���Ԓ����Ă���΍X�V���Ȃ�
	if( timeGetTime() - m_SaveTime > m_EndTime )
	{
		// �O�̂��ߓ����ɂ��Ă���
		for( int i = 0; i < m_Num; i++ )
		{
			m_Sprite[ i ].Color = D3DCOLOR_ARGB( 0, 0xff, 0xff, 0xff );
		}

		return;
	}

	DWORD NowTime = timeGetTime() - m_SaveTime;

	// ������
	for( int i = 0; i < m_Num; i++ )
	{
		m_MoveDir[ i ] *= 0.95f;	// ����

		m_MoveDir[ i ].x	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);
		m_MoveDir[ i ].y	+= static_cast<float>( rand() % 10 ) / 100.f - ( m_NowGravity + GRAVITY_SPEED );
		m_MoveDir[ i ].z	+= static_cast<float>( rand() % 10 ) / 100.f * (rand() % 2 * 2 - 1);

		m_Sprite[ i ].Pos += m_MoveDir[ i ];

		// �C�ʂ���o����
		if( m_Sprite[ i ].Pos.y > Sequence::CGameMain::SURFACESEA_HEIGHT - 1)
		{
			// �Ƃǂ܂�
			m_Sprite[ i ].Pos.y = Sequence::CGameMain::SURFACESEA_HEIGHT - 1;
		}

		m_Sprite[ i ].Scale += 0.5f;

		//m_Sprite[ i ].Color = D3DCOLOR_ARGB( 0xff - static_cast<int>(NowTime * m_AlphaCnt), 0xff, 0xff, 0xff );
	}
}

// �X�V���@�ݒ�
void CPointSpriteManager::SetType( UPDATETYPE type )
{
	m_NowType = type;

	switch( m_NowType )
	{
	case TYPE_POP:
		m_fpUpdateFunc = &CPointSpriteManager::UpdatePop;
		break;

	case TYPE_SPOUT:
		m_fpUpdateFunc = &CPointSpriteManager::UpdateSpout;
		break;

	case TYPE_GRAVITYPOP:
		m_fpUpdateFunc = &CPointSpriteManager::UpdateGravityPop;
		break;
	}
}

// �����ݒ�
void CPointSpriteManager::SetMove( const D3DXVECTOR3& move_max )
{
	m_SaveMoveMax = move_max;

	BOOST_FOREACH( D3DXVECTOR3& Move, m_MoveDir )
	{
		Move.x = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.x - (m_SaveMoveMax.x / 2);
		Move.y = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.y;
		Move.z = (float)rand() / (float)SHRT_MAX * m_SaveMoveMax.z - (m_SaveMoveMax.z / 2);
	}
}

// ���������ǂ���
bool CPointSpriteManager::IsErase()
{
	// ���Ԓ������
	if( timeGetTime() - m_SaveTime > m_EndTime )
	{
		return true;
	}

	// ��ʓ�����S�ď����ĂȂ���΂܂�
	BOOST_FOREACH( POINTSPRITE& Sprite, m_Sprite )
	{
		if( IsCulling( &CMatrix( Sprite.Pos ) ) )
		{
			return false;
		}
	}

	return true;
}