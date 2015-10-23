//////////////////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////////////////
#include "BubbleManager.h"

#include "ShotBase.h"
#include "ShotManager.h"

#include "FinTail.h"
#include "RippleManager.h"

#include "LimitWalls.h"
#include "StoneManager.h"
#include "GameMain.h"

#include "BossBase.h"
#include "HardBorder.h"

//////////////////////////////////////////////////
// ���O��ԏȗ�
//////////////////////////////////////////////////
using namespace Boss;

//////////////////////////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////////////////////////
const int		CHardBorder::HP_MAX				= 50;//250;
const float		CHardBorder::MOVE_SPEED			= 1.f;
const float		CHardBorder::ROT_MAX			= 3.f;
const float		CHardBorder::COLORFADEOUT_SPEED	= 0.05f;
const USHORT	CHardBorder::SHOT_INTERVAL		= 2500;
const USHORT	CHardBorder::SLUG_TIME			= 3000;
const USHORT	CHardBorder::NEXT_MOVEMENT_TIME	= 5000;
const BYTE		CHardBorder::SHOT_NUM			= 5;
const USHORT	CHardBorder::STOP_TIME			= 3000;

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �R���X�g���N�^
CHardBorder::CHardBorder( const LPDIRECT3DDEVICE9 lpdevice ):
m_ShotCnt( SHOT_NUM ),
m_lpmParents( NULL ),
m_lpMeshPath( NULL ),
m_lpMeshPos( NULL ),
m_lpTails( NULL ),
m_PartsMax( 0 ),
m_Basis( 0 ),
m_NowMoveSpeed( 0 ),
m_SaveStopTime( 0 ),
m_AddColor( 0, 0, 0, 0 ),
m_AnimeCnt( -1 ),
m_BreakAddTime( 0 ),
m_Board( 3 )
{
	m_lpMesh = CSXfileManager::GetInst()->GetData( "BigMachineFish.x" );

	m_HP = HP_MAX;

	m_Flg |= FLG_ATTACK;

	//SetMovement( static_cast<LPMOVEMENTFANC>(&CHardBorder::Test) );

	// �s���֐��o�^
	m_MovementList.push_back( &CHardBorder::UpdateMove );
	m_MovementList.push_back( &CHardBorder::UpdateShot );
	//m_MovementList.push_back( &CHardBorder::UpdateSlug );

	// �s���֐�������
	m_NowMovement = m_MovementList.begin();

	// �����_���ōs������
	m_SaveMovement = rand() % m_MovementList.size();
	for( int i = m_SaveMovement; i > 0; i-- )
	{
		m_NowMovement++;
	}

	// �ǂݍ��݊J�n
	LoadData();
	
	m_lpmParents[ m_Basis ].MoveLocal( 1, 1, 1 );

	m_SaveTime = timeGetTime();
}

// �f�X�g���N�^
CHardBorder::~CHardBorder()
{
	SAFE_DELETE_ARRAY( m_lpMeshPos );
	SAFE_DELETE_ARRAY( m_lpmParents );
	SAFE_DELETE_ARRAY( m_lpMeshPath );
	SAFE_DELETE_ARRAY( m_lpTails );

	// �|�C���g�X�v���C�g���
	BOOST_FOREACH( CPointSpriteManager* It, m_PointSpriteList )
	{
		SAFE_DELETE( It );
	}

	m_PointSpriteList.clear();
}

// �X�V
void CHardBorder::Update()
{
#ifdef _DEBUG

	if( Input::Keyboard::IsState( DIK_E, Input::PUSH ) )
	{
		LoadData();
	}

#endif

	// �����[�v�s������
	InitLoop();

	if( m_Flg & FLG_ALIVE )
	{
		// ����Ɏ��@���Ɍ���
		if( !(m_Flg & FLG_STICK) )
		{
			Turn();
		}
		
		// �s�����s���Ă��̍s�����I�������Ƃ�
		if( (this->*(*m_NowMovement))() == false )
		{
			// �s�������ւ���
			m_NowMovement = m_MovementList.begin();

			// �����_���ōs������
			int Temp = (rand() % m_MovementList.size());

			// �O��ƍs���������Ȃ�
			if( m_SaveMovement == Temp )
			{
				m_SaveMovement = ++Temp % m_MovementList.size();
			}

			for( int i = m_SaveMovement; i > 0; i-- )
			{
				m_NowMovement++;
			}

			m_SaveTime = timeGetTime();
		}

		// �e�\���X�V
		for( int i = 0; i < m_PartsMax; i++ )
		{
			// �e�Ȃ牽�����Ȃ�
			if( i == m_Basis )
			{
				continue;
			}

			m_lpmParents[ i ] = CMatrix( m_lpMeshPos[ i ] ) * m_lpmParents[ m_Basis ];
		}

		// �K��(�q)���X�V
		for( int i = 0; i < m_TailsMax; i++ )
		{
			// �ŏ��Ȃ�
			if( i == 0 )
			{
				// �e�̍s������ɍX�V
				m_lpTails[ i ].Update( m_lpmParents[ m_Basis ], m_NowMoveSpeed );
			
			}else{

				// ����ȊO�͑O�̐K�������ɍX�V
				m_lpTails[ i ].Update( m_lpTails[ i - 1 ].GetMatrix(), m_NowMoveSpeed );
			}
		}

	}else if( m_Flg & FLG_BREAK ){

		// 5�b�ԃp�[�e�B�N��
		if( timeGetTime() - m_StartBreakTime > 5000 )
		{
			CSoundManager::GetInst()->Play( "Sound/BossBreak.wav", false, true );
			if( ++m_AnimeCnt > CBubble::BURST_ANIME_NUM )
			{
				m_Flg |= FLG_ERASE;
			}
			
		}else{

			// ��莞�Ԃ����Ƀp�[�e�B�N�����o��
			if( timeGetTime() - m_BreakAddTime > 80 )
			{
				CSoundManager::GetInst()->Play( "Sound/Break.wav" );

				// �K���ɂ΂炷
				D3DXVECTOR3 Pos = m_lpmParents[ m_Basis ].GetPos();

				int Temp = static_cast< int >( m_lpMesh->GetRadius() * 100 );
				Pos.x += rand() % Temp * 1.f * GetPosiOrNega();
				Pos.y += rand() % Temp * 1.f * GetPosiOrNega();
				Pos.z += rand() % Temp * 1.f * GetPosiOrNega();

				AddSprite( Pos );

				m_BreakAddTime = timeGetTime();
			}
		}
	}

	// �e�X�V
	m_Shots.Update();

	// �G�t�F�N�g�X�V
	BOOST_FOREACH( CPointSpriteManager* It, m_PointSpriteList )
	{
		It->Update();
	}
}

// �o��X�V
bool CHardBorder::UpdateEnterScene()
{
	// �O�֐i�܂���
	MoveCollision( D3DXVECTOR3( 0, 0, 0.5f ), true );

	// �e�\���X�V
	for( int i = 0; i < m_PartsMax; i++ )
	{
		// �e�Ȃ牽�����Ȃ�
		if( i == m_Basis )
		{
			continue;
		}

		m_lpmParents[ i ] = CMatrix( m_lpMeshPos[ i ] ) * m_lpmParents[ m_Basis ];
	}

	// �K��(�q)���X�V
	for( int i = 0; i < m_TailsMax; i++ )
	{
		// �ŏ��Ȃ�
		if( i == 0 )
		{
			// �e�̍s������ɍX�V
			m_lpTails[ i ].Update( m_lpmParents[ m_Basis ], m_NowMoveSpeed );

		}else{

			// ����ȊO�͑O�̐K�������ɍX�V
			m_lpTails[ i ].Update( m_lpTails[ i - 1 ].GetMatrix(), m_NowMoveSpeed );
		}
	}

	return true;
}

// �`��
void CHardBorder::Draw(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( m_Flg & FLG_ALIVE )
	{
		// �{�X
		if( IsCulling( &m_lpmParents[ m_Basis ] ) )
		{
			lpdevice->SetTransform( D3DTS_WORLD, &m_lpmParents[ m_Basis ] );

			CSXfileManager::GetInst()->GetData( m_lpMeshPath[ m_Basis ].c_str() )->Draw( lpdevice );
		}


		// �K��
		for( int i = 0; i < m_TailsMax; i++ )
		{
			m_lpTails[ i ].Draw( lpdevice );
		}

		// �e
		m_Shots.Draw( lpdevice );
	
	}else{

		// �|�����`�悳��ĂȂ��āA�܂��������Ă��Ȃ��Ȃ�`��
		if( m_AnimeCnt == -1 )
		{
			// �{�X
			if( IsCulling( &m_lpmParents[ m_Basis ] ) )
			{
				lpdevice->SetTransform( D3DTS_WORLD, &m_lpmParents[ m_Basis ] );

				CSXfileManager::GetInst()->GetData( m_lpMeshPath[ m_Basis ].c_str() )->Draw( lpdevice );
			}


			// �K��
			for( int i = 0; i < m_TailsMax; i++ )
			{
				m_lpTails[ i ].Draw( lpdevice );
			}

			// �e
			m_Shots.Draw( lpdevice );
		}
	}
}

// �G�t�F�N�g�`��
void CHardBorder::DrawEffect(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( m_Flg & FLG_BREAK )
	{
		DrawBreak( lpdevice );
	}

	// �|�C���g�X�v���C�g�`��
	lpdevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	BOOST_FOREACH( CPointSpriteManager* It, m_PointSpriteList )
	{
		It->Draw( lpdevice );
	}

	lpdevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
}

// �V�F�[�_�`��
void CHardBorder::DrawShader(const LPDIRECT3DDEVICE9 lpdevice)
{
	if( m_Flg & FLG_ALIVE )
	{
		CEffect* lpSharder = CSShaderManager::GetInst()->GetData( "Phong.fx" );

		// �Œ�p�C�v���C���̏����l������
		CMatrix	mView, mProj;
		lpdevice->GetTransform( D3DTS_PROJECTION, &mProj );
		lpdevice->GetTransform( D3DTS_VIEW, &mView );

		// ���W�ϊ�
		lpSharder->SetValue( "g_WVP", 
			&(m_lpmParents[ m_Basis ] * mView * mProj),
			sizeof( D3DXMATRIX ) );

		// ���C�g�ʒu�v�Z
		lpSharder->SetValue( "g_LightDir",
			&GetLightPos( lpdevice, m_lpmParents[ m_Basis ] ),
			sizeof( D3DXVECTOR4 ) );

		// �J�����ʒu�v�Z
		lpSharder->SetValue( "g_EyePos",
			&GetViewPos( lpdevice, m_lpmParents[ m_Basis ] ),
			sizeof( D3DXVECTOR4 ) );

		// ���Z�F
		lpSharder->SetValue( "g_AddColor",
			&m_AddColor,
			sizeof( D3DXCOLOR ) );
		
		// �{�X
		if( IsCulling( &m_lpmParents[ m_Basis ] ) )
		{
			lpSharder->Begin();

			CSXfileManager::GetInst()->GetData( m_lpMeshPath[ m_Basis ].c_str() )->DrawShader( lpSharder, "g_Diffuse", "g_Tex" );

			lpSharder->End();
		}

		// �K��
		for( int i = 0; i < m_TailsMax; i++ )
		{
			m_lpTails[ i ].DrawShader( lpdevice );
		}

		// �e
		m_Shots.Draw( lpdevice );
	
	}else{

		// �|�����`�悳��ĂȂ��āA�܂��������Ă��Ȃ��Ȃ�`��
		if( m_AnimeCnt == -1 )
		{
			// �{�X
			if( IsCulling( &m_lpmParents[ m_Basis ] ) )
			{
				lpdevice->SetTransform( D3DTS_WORLD, &m_lpmParents[ m_Basis ] );

				CSXfileManager::GetInst()->GetData( m_lpMeshPath[ m_Basis ].c_str() )->Draw( lpdevice );
			}


			// �K��
			for( int i = 0; i < m_TailsMax; i++ )
			{
				m_lpTails[ i ].Draw( lpdevice );
			}

			// �e
			m_Shots.Draw( lpdevice );
		}
	}
}

// ���@�̕��Ɍ���
void CHardBorder::Turn()
{
	// ���@�̕��Ɍ���
	CMatrix mInv = m_lpmParents[ m_Basis ];

	// �����i�{�X�j�̋t�s��
	mInv.GetInverse();

	// �ڕW(�v���C���[)�̈ʒu�����
	CVector Target( m_PlayerMat.GetPos() );
	Target.GetTransCoord( &mInv );

	// ���K��
	Target.GetNormalVector();

	// ��]�������߂�
	CVector RotAxis = Target;

	RotAxis.GetCross( &D3DXVECTOR3( 0, 0, -1 ) );

	// ���ς����߂�
	float Ang = Target.GetDotDegree( &D3DXVECTOR3( 0, 0, 1 ) );

	if( Ang > ROT_MAX )
	{
		Ang = ROT_MAX;
	}

	m_lpmParents[ m_Basis ].RotateLocalAxis( &RotAxis, Ang );
}

// �����X�V
bool CHardBorder::UpdateMove()
{
	// �h�����Ă���Ȃ�
	if( m_Flg & FLG_STICK )
	{
		// �h�����Ă���X�V
		UpdateStick();
	
	}else{

		// �ːi����
		UpdateRush();
	}

	// ��莞�Ԓ�������
	if( timeGetTime() - m_SaveTime > NEXT_MOVEMENT_TIME )
	{
		m_Flg &= ~FLG_STICK;
		m_NowMoveSpeed = 0;

		return false;	// �I��
	}

	return true;
}

// �ːi
void CHardBorder::UpdateRush()
{
	// �s�����~�܂��Ă���
	if( m_Flg & FLG_STOP )
	{
		if( timeGetTime() - m_SaveStopTime > STOP_TIME )
		{
			m_Flg &= ~FLG_STOP;
		}

		// �\������
		MoveCollision( D3DXVECTOR3( 0, 0, -MOVE_SPEED / 2 ), true );
	
	}else{

		// ��Ɉ��
		m_NowMoveSpeed = MOVE_SPEED;

		// �������������Ȃ�
		if( MoveCollision( D3DXVECTOR3( 0, 0, m_NowMoveSpeed ), false ) )
		{
			// �h���������̏�����
			m_SaveTime = timeGetTime();
			m_Flg |= FLG_STICK;
			AddSprite( m_lpmParents[ m_Basis ].GetPos() );
		}
	}
}

// �h�����Ă���
void CHardBorder::UpdateStick()
{
	AddSprite( m_lpmParents[ m_Basis ].GetPos() );

	if( timeGetTime() - m_SaveTime > NEXT_MOVEMENT_TIME )
	{
		m_Flg &= ~FLG_STICK;
		m_Flg |= FLG_STOP;
		m_SaveStopTime = timeGetTime();
	}
}

// �e�X�V
bool CHardBorder::UpdateShot()
{
	// ����
	if( m_ShotCnt > 0 )
	{
		if( timeGetTime() - m_SaveTime > SHOT_INTERVAL )
		{
			m_Shots.Add( Shot::CManager::NORMAL, &m_lpmParents[ m_Basis ] );

			m_ShotCnt--;

			m_SaveTime = timeGetTime();

			m_AddColor.g = m_AddColor.b = 0;

			//m_Flg |= ATTACK;
		
		}else{

			// �\�����쒆�͐F��ς���
			m_AddColor.g = m_AddColor.b = 
				(timeGetTime() - m_SaveTime) / static_cast<float>(SHOT_INTERVAL);
		}
	
	}else{

		m_ShotCnt = SHOT_NUM;
		return false;	// �I��
	}


	return true;
}

// �U�e
bool CHardBorder::UpdateSlug()
{
	// �\������(���F�ɕϐF)
	if( m_AddColor.b < 1 && m_AddColor.r < 1 )
	{
		m_AddColor.r += 0.1f;
		m_AddColor.b += 0.1f;

		if( m_AddColor.b >= 1 && m_AddColor.r >= 1 )
		{
			m_SaveTime = timeGetTime();
		}
	
	}else{

		m_AddColor.r = m_AddColor.b = 1.1f;

		// ����������ɒe���΂��Ă݂�
		if( timeGetTime() - m_SaveTime < SLUG_TIME )
		{
			CMatrix	mMat = m_lpmParents[ m_Basis ];

			mMat.RotateLocalX( static_cast<float>( rand() % 45 - 30 ) );
			mMat.RotateLocalY( static_cast<float>( rand() % 45 - 30 ) );
			//mMat.RotateLocalZ( static_cast<float>( rand() % 360 ) );

			m_Shots.Add( Shot::CManager::POWER, &mMat );

		}else{

			// �F�����Ƃɖ߂�
			m_AddColor.r = m_AddColor.b = 0.f;
			return false;	// �I��
		}
	}
	
	return true;
}

// �e�ɑ΂���
void CHardBorder::Action( Shot::CBase& object )
{
	// �����̑ł������������ǂ����𒲂ׂ�
	if( typeid( object ) != typeid( Shot::CBubbleRing ) )
	{
		// �����o�u�������O����Ȃ����
		// ���˕Ԃ��ꂽ���ǂ����𒲂ׂ�
		if( !(object.GetFlg() & Shot::CBase::FLG_FIN_HIT) )
		{
			// ���˕Ԃ���Ă��Ȃ���ΏI��
			// �������Ȃ�
			return;
		}
	}

	// �_���[�W��H������Ȃ�
	if( (object.GetFlg() & Shot::CBase::FLG_ALIVE) )
	{
		AddSprite( m_lpmParents[ m_Basis ].GetPos() );

		m_Flg |= FLG_DAMAGE;

		m_AddColor.r = 1;

		if( --m_HP < 0 )
		{
			m_HP = 0;

			m_Flg |= FLG_BREAK;
			m_Flg &= ~FLG_ALIVE;

			m_Board.SetVertex( 25 );
		}
	}
}

// �v���C���[�ɑ΂���
void CHardBorder::Action( Player::CDolphin& object )
{
	// �������~�߂�
	m_Flg |= FLG_STOP;
	m_NowMoveSpeed = 0;
	m_SaveStopTime = timeGetTime();
}

// �e�L�X�g�f�[�^����ǂݍ���
void CHardBorder::LoadData()
{
	SAFE_DELETE_ARRAY( m_lpmParents );
	SAFE_DELETE_ARRAY( m_lpMeshPath );
	SAFE_DELETE_ARRAY( m_lpTails );
	SAFE_DELETE_ARRAY( m_lpMeshPos );

	std::ifstream In( "LoadData/GameMain/BigFishData.txt" );

	if( In.is_open() )
	{
		// �ő吔�ǂݍ���
		In >> m_PartsMax;
		m_PartsMax -= '0';

		// �z��m��
		m_lpmParents= NEW CMatrix[ m_PartsMax ];
		m_lpMeshPos	= NEW CVector[ m_PartsMax ];
		m_lpMeshPath= NEW std::string[ m_PartsMax ];

		// �e�����肷��
		In >> m_Basis;
		m_Basis -= '0';

		// �K���̐������肵�A�m�ۂ���
		In >> m_TailsMax;
		m_TailsMax -= '0';

		m_lpTails = NEW CFinTail[ m_TailsMax ];

		// ���[�J���s��̏����ʒu�ƃ��b�V���p�X��ݒ肷��
		for( int i = 0; i < m_PartsMax; i++ )
		{
			In >> m_lpMeshPos[ i ].x;
			In >> m_lpMeshPos[ i ].y;
			In >> m_lpMeshPos[ i ].z;

			m_lpmParents[ i ].SetTrans( &m_lpMeshPos[ i ] );

			In >> m_lpMeshPath[ i ];
		}

		// ���ꂼ��̐K���ʒu�ƃ��b�V����ݒ肷��
		for( int i = 0; i < m_TailsMax; i++ )
		{
			m_lpTails[ i ].SetRotMax( 12 );
			m_lpTails[ i ].SetFarPos( &m_lpMeshPos[ i + 1 ] );
			m_lpTails[ i ].SetMesh( CSXfileManager::GetInst()->GetData( m_lpMeshPath[ i + 1 ].c_str() ) );
		}
	}

	In.close();
}

// �|�C���g�X�v���C�g�ǉ�
void CHardBorder::AddSprite( const D3DXVECTOR3& pos )
{
	// ���Z�b�g�ł�����̂��Ȃ������ׂ�
	BOOST_FOREACH( CPointSpriteManager* It, m_PointSpriteList )
	{
		if( It->IsErase() )
		{
			It->SetPos( m_lpmParents[ m_Basis ].GetPos() );
			It->Reset();

			return;
		}
	}

	CPointSpriteManager* lpTemp = NEW CPointSpriteManager();

	lpTemp->SetNum( 10 );
	lpTemp->SetPos( pos );
	lpTemp->SetScaleMax( 2.f );
	lpTemp->SetEraseTime( 2000 );
	lpTemp->SetMove( D3DXVECTOR3( 1, -1, 1 ) );

	CTexture* lpTex = CSTexManager::GetInst()->GetData( "Sprite.jpg" );

	lpTemp->SetTexture( *lpTex );

	m_PointSpriteList.push_back( lpTemp );
}

// �Փ˔���
bool CHardBorder::MoveCollision(const D3DXVECTOR3 &move, bool is_move)
{
	if( GetStones() == NULL )
	{
		return false;
	}

	// �����ƕ����𒊏o
	float Length = D3DXVec3Length( &move );

	CVector Direction;
	D3DXVec3Normalize( &Direction, &move );

	// ���@�̕����𒲂ׂ�
	Direction.GetTransNormal( &m_lpmParents[ m_Basis ] );

	bool IsMove = true;

	CMatrix mScale;

	DWORD PolyNo;	// ���������|���S���i���o�[

	// ��Ƃ̏Փ˔���
	BOOST_FOREACH( CStoneManager::STONEDATA* Stone, *GetStones()->GetList() )
	{
		mScale.SetScale( Stone->Scale );

		// ����
		float Distance = IntersectByMesh( 
			&m_lpmParents[ m_Basis ],
			&Direction,
			&( mScale * CMatrix( Stone->Pos ) ),
			*Stone->lpMesh->GetMeshData(),
			&PolyNo );

		// ����
		if( Distance > 0 && Distance - Length < (1) )
		{
			// �����Ȃ�
			if( is_move )
			{
				// ���̑O�������𒲂ׂ�
				CVector	NowFront( 0, 0, 1 );

				NowFront.GetTransNormal( &m_lpmParents[ m_Basis ] );

				// ����x�N�g���v�Z
				D3DXVECTOR3 SlipVec = 
					GetSlipVec( NowFront, Stone->lpMesh->GetNormal( PolyNo ) );

				// ����x�N�g���ł�����x����
				DWORD SlipPoly;

				float SlipDistance = IntersectByMesh( 
					&m_lpmParents[ m_Basis ],
					&SlipVec,
					&( mScale * CMatrix( Stone->Pos ) ),
					*Stone->lpMesh->GetMeshData(),
					&SlipPoly );


				D3DXVECTOR3 TempVec;

				if( SlipDistance > 0 && SlipDistance - Length < (1) )
				{
					// ����x�N�g���œ��������Ȃ�~�߂�	
					TempVec = D3DXVECTOR3( 0, 0, 0 );

				}else{

					// �����łȂ���΍ŏ��̃x�N�g�����g��
					TempVec = SlipVec * Length;
				}

				// �ړ�
				m_lpmParents[ m_Basis ]._41 += TempVec.x;
				m_lpmParents[ m_Basis ]._42 += TempVec.y;
				m_lpmParents[ m_Basis ]._43 += TempVec.z;

				IsMove = false;
				break;
			
			}else{
			
				//m_lpmParents[ m_Basis ].MoveLocal( &(Direction * (Distance - Length)));
				// �����������Ƃ����`����
				return true;
			}
		}
	}

	// ������Ȃ瓮��
	if( IsMove )
	{
		m_lpmParents[ m_Basis ].MoveLocal( &move );
	}

	return false;
}

// �p���𐳂�
void CHardBorder::ReformPose()
{
	// Z����]������
	float Degree = m_lpmParents[ m_Basis ].GetRotZ();

	// ��]���K�v�ł���Ή�]����
	if( abs( Degree ) > 1 )
	{
		if( Degree > 0 )
		{
			m_lpmParents[ m_Basis ].RotateLocalZ( -1 );

		}else{

			m_lpmParents[ m_Basis ].RotateLocalZ( 1 );
		}
	}
}

// �����[�v�s��������
void CHardBorder::InitLoop()
{
	// �_���[�W�t���O������
	m_Flg &= ~FLG_DAMAGE;

	// �F�̌��Z
	if( (m_AddColor.r -= COLORFADEOUT_SPEED) < 0 )
	{
		m_AddColor.r = 0;
	}
	if( (m_AddColor.g -= COLORFADEOUT_SPEED) < 0 )
	{
		m_AddColor.g = 0;
	}
	if( (m_AddColor.b -= COLORFADEOUT_SPEED) < 0 )
	{
		m_AddColor.b = 0;
	}
	if( (m_AddColor.a -= COLORFADEOUT_SPEED) < 0 )
	{
		m_AddColor.a = 0;
	}

	// ����ł邩�ǂ���
	if( !(m_Flg & FLG_BREAK) && m_HP <= 0 )
	{
		m_Flg |= FLG_BREAK;

		m_BreakAddTime = m_StartBreakTime = timeGetTime();
	}

	// �p���𐳂�
	ReformPose();
}

// �j�󎞂̕`��
void CHardBorder::DrawBreak(LPDIRECT3DDEVICE9 lpdevice)
{
	// �Ō�̔����Ȃ�
	if( m_AnimeCnt >= 0 && m_AnimeCnt < CBubble::BURST_ANIME_NUM )
	{
		m_Board.SetTexturePos(
				&D3DXVECTOR2(
				(m_AnimeCnt % CBubble::BURST_ANIME_WIDTH_NUM * (float)CBubble::BURST_ANIME_SIZE) / CBubble::BURST_WIDTH,
				(m_AnimeCnt / CBubble::BURST_ANIME_HEIGHT_NUM* (float)CBubble::BURST_ANIME_SIZE) / CBubble::BURST_HEIGHT ),

				&D3DXVECTOR2( 
				((m_AnimeCnt % CBubble::BURST_ANIME_WIDTH_NUM + 1)	* (float)CBubble::BURST_ANIME_SIZE) / CBubble::BURST_WIDTH,
				(m_AnimeCnt	 / CBubble::BURST_ANIME_HEIGHT_NUM		* (float)CBubble::BURST_ANIME_SIZE) / CBubble::BURST_HEIGHT ),

				&D3DXVECTOR2(
				((m_AnimeCnt % CBubble::BURST_ANIME_WIDTH_NUM + 1)	* (float)CBubble::BURST_ANIME_SIZE) / CBubble::BURST_WIDTH,
				((m_AnimeCnt / CBubble::BURST_ANIME_HEIGHT_NUM+ 1)	* (float)CBubble::BURST_ANIME_SIZE)	/ CBubble::BURST_HEIGHT ),

				&D3DXVECTOR2( 
				(m_AnimeCnt	 % CBubble::BURST_ANIME_WIDTH_NUM		* (float)CBubble::BURST_ANIME_SIZE)	/ CBubble::BURST_WIDTH,
				((m_AnimeCnt / CBubble::BURST_ANIME_HEIGHT_NUM + 1)  * (float)CBubble::BURST_ANIME_SIZE)	/ CBubble::BURST_HEIGHT )
				);

		// �r���{�[�h
		CMatrix	mMat, mTrans;
		CCamera Camera = *Sequence::CGameMain::GetCamera();
		Camera.GetBillboard( &mMat, &m_lpmParents[ m_Basis ] );
		mTrans.SetTrans( &m_lpmParents[ m_Basis ].GetPos() );

		// ����
		mMat = mMat * mTrans;
		lpdevice->SetTransform( D3DTS_WORLD, &mMat );

		// �e�N�X�`���ƒ��_�t�H�[�}�b�g�ݒ�
		lpdevice->SetTexture( 0, *CSTexManager::GetInst()->GetData( "Smoke.png" ) );
		lpdevice->SetFVF( CBoard::FVF_VERTEX );

		//lpdevice->SetRenderState( D3DRS_LIGHTING, FALSE );

		lpdevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		lpdevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

		// �`��
		m_Board.Draw( lpdevice );

		//lpdevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	}
}

// �����u���[�̋���
float CHardBorder::GetBlurPower()
{
	// �������Ȃ�
	if( m_AnimeCnt >= 0 && m_AnimeCnt < CBubble::BURST_ANIME_NUM )
	{
		return CBubble::BURST_ANIME_NUM / 2 - m_AnimeCnt + CBubble::BURST_ANIME_NUM / 2;
	}

	return 0;
}

// �����u���[�̌��̋���
float CHardBorder::GetAddColorRate()
{
	// �������Ȃ�
	if( m_AnimeCnt >= 0 && m_AnimeCnt < CBubble::BURST_ANIME_NUM )
	{
		return 1 + GetBlurPower() / CBubble::BURST_ANIME_NUM / 2;
	}

	return 1;
}