//////////////////////////////////////////////////
// �C���N���[�h
//////////////////////////////////////////////////
#include "SequenceBase.h"
#include "Bubble.h"
#include "BubbleManager.h"

#include "RippleManager.h"
#include "Fintail.h"
#include "Screw.h"

#include "ShotManager.h"
#include "StrongDolphin.h"
#include "ShotBase.h"
#include "EnemyBase.h"
#include "NormalEnemy.h"
#include "EnemyManager.h"

#include "BubbleBreaker.h"

#include "BossBase.h"
#include "HardBorder.h"

#include "FishManager.h"
#include "GameOver.h"
#include "Clear.h"

#include "UI.h"
#include "LimitWalls.h"
#include "PlanktonManager.h"
#include "StoneManager.h"

#include "DamageObjectBase.h"
#include "FishManager.h"

#include "SunLineManager.h"

#include "SurfaceSea.h"

#include "GameMain.h"

//////////////////////////////////////////////////
// ���O��ԏȗ�
//////////////////////////////////////////////////
using namespace Sequence;

//////////////////////////////////////////////////
// �ÓI�����o���̉�
//////////////////////////////////////////////////
const LPSTR		CGameMain::PATH_STONEDATA			= "/StoneData.txt";
const LPSTR		CGameMain::PATH_DAMAGEFISHDATA		= "/DamageFish.txt";
const LPSTR		CGameMain::SKY_MESH_PASS			= "Sky.x";
const LPSTR		CGameMain::PATH_ENEMYDATA			= "/EnemyData.txt";
const float		CGameMain::STAGE_SCALE				= 10.f;
const BYTE		CGameMain::BUBBLES_NUM				= 200;
const float		CGameMain::SURFACESEA_HEIGHT		= 50.f;
const float		CGameMain::BOTTOMSEA_HEIGHT			= -50.f;
const float		CGameMain::SEA_MOVELIMIT			= 150;
const float		CGameMain::SEA_COLOR_ADD			= 200.f / SURFACESEA_HEIGHT / 2;
const float		CGameMain::FOG_DEPTH_ADD			= (45.f / 100.f);
const float		CGameMain::SURFACE_ALPHA_RATE		= (0.3 / SURFACESEA_HEIGHT);
const float		CGameMain::SCROLL_SPEED				= 1.f / 1024.f;
const float		CGameMain::JUMP_CAMERA_HEIGHT		= CGameMain::SURFACESEA_HEIGHT + 5.f;
const float		CGameMain::FOCUS_RATE				= 0.7f / ( abs( CGameMain::BOTTOMSEA_HEIGHT ) + abs( CGameMain::SURFACESEA_HEIGHT ) );
const short		CGameMain::DEFAUL_GAMESCENE_ALPHA	= 210;

CCamera			CGameMain::m_Camera;

const CGameMain::FLGTYPE	CGameMain::FLG_RESULT		= 0x01;
const CGameMain::FLGTYPE	CGameMain::FLG_RUN			= 0x02;
const CGameMain::FLGTYPE	CGameMain::FLG_BOSSBATTLE	= 0x04;
const CGameMain::FLGTYPE	CGameMain::FLG_BOSSENTER	= 0x08;

//////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////

// �R���X�g���N�^
CGameMain::CGameMain( BYTE stage ):
m_DamageAlphaCnt( 0 ),
m_GameSceneAlpha( DEFAUL_GAMESCENE_ALPHA ),
m_Flg( FLG_RUN ),
m_lpSurfaceSea( NULL ),
m_lpPlayer( NULL ),
m_lpBackBuffer( NULL ),
m_lpEnemys( NULL ),
m_lpSky( NULL ),
m_lpSprite( NULL ),
m_lpUI( NULL ),
m_lpZBuffer( NULL ),
m_lpPlanktons( NULL ),
m_lpStones( NULL ),
m_lpFishes( NULL ),
m_lpBoss( NULL ),
m_StageIndex( stage ),
m_EnemyNum( 0 )
{
	char Str[ 0xff ];

	sprintf_s( Str,  "CallGameMain\nByteSize = %d", sizeof( CGameMain ) );
	DebugMessageBox( Str );
}

// �f�X�g���N�^
CGameMain::~CGameMain()
{
	// BGM�X�g�b�v
	CSoundManager::GetInst()->Stop( m_BGMPath.c_str() );

	if( m_lpBoss )
	{
		CSoundManager::GetInst()->Stop( m_lpBoss->GetBGMName() );
	}
	
	// ���������
	SAFE_RELEASE( m_lpZBuffer );
	SAFE_RELEASE( m_lpBackBuffer );
	SAFE_DELETE( m_lpPlayer );
	SAFE_DELETE( m_lpSprite );
	SAFE_DELETE( m_lpUI );
	SAFE_DELETE( m_lpEnemys );
	SAFE_DELETE( m_lpStones );
	SAFE_DELETE( m_lpPlanktons );
	SAFE_DELETE( m_lpSurfaceSea );
	SAFE_DELETE( m_lpFishes );
	SAFE_DELETE( m_lpBoss );
	SAFE_DELETE( m_lpSunLines );
}

// �ǂݍ���
void CGameMain::FileLoad()
{
	// �ǂݍ���
	Load();

	// ���I�������m��
	Allocs();

	// �X�e�[�W�ɂ���ĈقȂ�ǂݍ���
	LoadStage( m_StageIndex );

	// �X�e�[�W�Ƌ�̃��b�V����ۑ�
	m_lpSky	= CSXfileManager::GetInst()->GetData( SKY_MESH_PASS );

	// ���C�g�ݒ�
	D3DLIGHT9	Light;
	ZeroMemory( &Light, sizeof( Light ) );

	Light.Type	= D3DLIGHT_DIRECTIONAL;	// ���s����
	Light.Diffuse.r	= 1.0f;
	Light.Diffuse.g	= 1.0f;
	Light.Diffuse.b	= 1.0f;

	Light.Direction = D3DXVECTOR3( 1.0f, -1.0f, 1.0f);

	D3D::GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	D3D::GetDevice()->SetLight( 0, &Light );
	D3D::GetDevice()->LightEnable( 0, TRUE );

	// �o�b�N�o�b�t�@����ۑ�
	D3D::GetDevice()->GetDepthStencilSurface( &m_lpZBuffer );
	D3D::GetDevice()->GetRenderTarget( 0, &m_lpBackBuffer );
	D3D::GetDevice()->GetViewport( &m_VP );

	// ���G�Ƀv���C���[�̏���m�点�Ă���
	Enemy::CBase::SetPlayerInfo( m_lpPlayer );

	// �t�H�O�ݒ�(�g��Ȃ�����)
	D3D::GetDevice()->SetRenderState( D3DRS_FOGENABLE, TRUE );
	D3D::GetDevice()->SetRenderState( D3DRS_FOGCOLOR, D3DCOLOR_XRGB( 0, 0x00, 0x80 ) );
	D3D::GetDevice()->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_LINEAR );
	D3D::GetDevice()->SetRenderState( D3DRS_FOGSTART, GetAddress( 50.f ) );
	D3D::GetDevice()->SetRenderState( D3DRS_FOGEND,	GetAddress( 150.f ) );

	D3D::GetDevice()->SetRenderState( D3DRS_SPECULARENABLE, TRUE );

	// �C��
	m_lpSurfaceSea->SetHeight( SURFACESEA_HEIGHT );
	m_lpSurfaceSea->SetSize( SEA_MOVELIMIT * 2 );

	// �C��
	m_BottomSea.SetVertex( SEA_MOVELIMIT * 2, false );

	// �v�����N�g��
	m_lpPlanktons->SetNum( 1000 );

	// �v���C���[�Ɋ�}�l�[�W���[��ݒ肷��
	m_lpPlayer->SetStoneManager( m_lpStones );

	// �G�ɂ���}�l�[�W���[��ݒ肷��
	Enemy::CBase::SetStoneManager( m_lpStones );
	Enemy::CBase::SetFishManager( m_lpFishes );

	// ���ɂ�
	CFishManager::SetStoneManager( m_lpStones );

	// �{�X�ɂ�
	Boss::CBase::SetStoneManager( m_lpStones );

	// �ǂ�ݒ肷��
	m_Walls.SetLimit( SEA_MOVELIMIT );
	m_Walls.SetScale( SEA_MOVELIMIT * 2,
		abs( SURFACESEA_HEIGHT ) + abs( BOTTOMSEA_HEIGHT ) );

	// ���𐶂ݏo��
	m_lpFishes->SetNumAndLimit( 50, D3DXVECTOR3(
		SEA_MOVELIMIT,
		SURFACESEA_HEIGHT - 10,
		SEA_MOVELIMIT ) );

	// �����_�[�^�[�Q�b�g�쐬
	// �V�[���`��p
	m_SceneTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT );
	
	// �k���p
	m_MiniSizeTex.CreateRenderTarget(
		D3D::GetDevice(), WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4 );

	// �ڂ����p
	m_BlurTex.CreateRenderTarget(
		D3D::GetDevice(), WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4 );

	// �[�x�ۑ��p
	m_DepthTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT );

	// �ŏI���ʗp
	m_ResultTex.CreateRenderTarget( D3D::GetDevice(), WINDOW_WIDTH, WINDOW_HEIGHT, D3DFMT_A16B16G16R16F );

	// �V�F�[�_�[�쐬
	// �ڂ����p�V�F�[�_�[
	m_BlurShader.Load( D3D::GetDevice(), "FX/Blur.fx", "Tec" );
	D3DXVECTOR2 TexPos( 0.5f / (WINDOW_WIDTH / 4), 0.5f / (WINDOW_HEIGHT / 4) );
	m_BlurShader.SetValue( "g_Offset", &TexPos, sizeof(D3DXVECTOR2) );

	// �[�x�v�Z�p
	m_DepthShader.Load( D3D::GetDevice(), "FX/Depth.fx", "Tec" );

	// ��ʊE�[�x�V�F�[�_�[
	m_DOFShader.Load( D3D::GetDevice(), "FX/DOF.fx", "Tec" );

	// �t�H���V�F�[�_
	CSShaderManager::GetInst()->Load( D3D::GetDevice(), "Phong.fx" );

	// �������C�e�B���O�V�F�[�_
	CSShaderManager::GetInst()->Load( D3D::GetDevice(), "Rim.fx" );

	// �����u���[�V�F�[�_�[
	m_BombBlurShader.Load( D3D::GetDevice(), "FX/BombBlur.fx", "Tec" );

	// ���b�V���Ɏg�p����V�F�[�_�[��ݒ肷��
	CXfile::SetShaderDevice( &m_DepthShader, "g_World" );
	CBoard::SetShaderDevice( &m_DepthShader, "g_World" );

	// ���z��
	m_lpSunLines->SetRandom( 150 );

	// �}�E�X�ʒu������
	RECT Rect;
	GetWindowRect( GetDesktopWindow(), &Rect );
	SetCursorPos( Rect.right / 2, Rect.bottom / 2 );

	// ���y
	m_BGMPath = "Sound/GameBGM.wav";
	CSoundManager::GetInst()->Play( m_BGMPath.c_str(), true );

	m_Timer.restart();
}

// ���̃V�[�P���X
IBase* CGameMain::GetNext()
{
	if( m_lpPlayer->GetFlg() & Player::CDolphin::FLG_DIE )
	{
		return NEW CGameOver();

	}else{

		// �X�R�A����
		ULONG Score = m_lpEnemys->GetScore();

		if( m_lpBoss )
		{
			Score += m_lpBoss->GetScore();
		}

		return NEW CClear( Score, m_Timer.elapsed() );
	}
}

// �����{��
bool CGameMain::RunProc()
{

#ifdef _DEBUG

	// �G�̓ǂݍ��݂��Ȃ���
	if( Input::Keyboard::IsState( DIK_E, Input::PUSH ) )
	{
		// ��ɂ��Ă���ǂݍ���
		m_lpEnemys->AllClear();
		m_lpEnemys->LoadFromFile( PATH_ENEMYDATA );
	}

	// ���ԕ\��
	char Str[ 0xff ];

	sprintf_s( Str, sizeof( Str ), "%lf�b \n" ,m_Timer.elapsed() );

	//OutputDebugStr( Str );

	if( Input::Keyboard::IsState( DIK_W, Input::PUSH ) )
	{
		m_lpEnemys->AllClear();
	}

#endif

	// �f�o�b�O�L�[
	if( Input::Keyboard::IsState( DIK_W, Input::PUSH ) ){ m_lpEnemys->AllClear(); }

	// �X�V
	Update();

	// �Փ˔���
	if( !(m_Flg & FLG_RESULT) )
	{
		Collision();
	}

	// �`��
	//if( FPS::IsSlowly() == false )
	{
		Draw();
	}

	return (m_Flg & FLG_RUN)? true : false;
}

// �X�V
void CGameMain::Update()
{
	// �G�����Ȃ��Ȃ邩�A���C�t���O�ɂȂ��
	if( (m_lpEnemys->IsNoneEnemy() && ((m_lpBoss)? m_lpBoss->IsDie() : true) ) ||  
		(m_lpPlayer->GetFlg() & Player::CDolphin::FLG_DIE) )
	{
		// �I��
		m_Flg |= FLG_RESULT;
	}

	// ���ʂ����܂����Ȃ�
	if( m_Flg & FLG_RESULT )
	{
		if( (m_GameSceneAlpha -= 5) < 0 )
		{
			m_GameSceneAlpha = 0;
			m_Flg &= ~FLG_RUN;
		}
	}

	// �L����
	// �{�X�����ĉ��o������Ȃ���΍X�V
	if( !(m_Flg & FLG_BOSSENTER) )
	{
		m_lpPlayer->Update();
	}
	
	// �G
	m_lpEnemys->Update();

	// �G�����Ȃ��Ȃ�΃{�X���o��
	if( m_lpEnemys->IsNoneEnemy() && !(m_Flg & (FLG_BOSSBATTLE | FLG_BOSSENTER)) )
	{
		m_Flg |= FLG_BOSSBATTLE;

		// �����{�X�������
		if( m_lpBoss )
		{
			CSoundManager::GetInst()->Stop( m_BGMPath.c_str() );
			CSoundManager::GetInst()->Play( "Sound/BossEnter.wav" );
			m_Flg |= FLG_BOSSENTER;
		}

		m_SaveEnterBossTime = timeGetTime();
	}

	// �{�X���o
	if( (m_Flg & FLG_BOSSENTER) && m_lpBoss )
	{
		// ���Œ�5�b��
		if( m_lpBoss->UpdateEnterScene() &&
			timeGetTime() - m_SaveEnterBossTime > 5000 )
		{
			CSoundManager::GetInst()->Play( m_lpBoss->GetBGMName(), true );
			m_Flg &= ~FLG_BOSSENTER;
		}

	}else if( (m_Flg & FLG_BOSSBATTLE) && m_lpBoss ){

		Boss::CBase::SetPlayerMat( &m_lpPlayer->GetMatrix() );
		m_lpBoss->Update();
	}

	// �g
	static const float TEXPOS = 1.f / 1024;
	static int Cnt = 0;
	float Scroll = TEXPOS * Cnt++;

	Cnt %= 1024;

	D3DXVECTOR3 Pos = m_lpPlayer->GetPos();

	const float UV = 0.5f / SEA_MOVELIMIT;

	// �C��
	m_lpSurfaceSea->Update( Pos );

	// �_���[�W�t�F�[�h�A�E�g
	if( (m_DamageAlphaCnt -= 5) < 0 )
	{
		m_DamageAlphaCnt = 0;
	}
	
	// ��
	m_lpFishes->Update();

	// �C���^�[�t�F�[�X
	m_lpUI->Update(
		m_lpEnemys->GetScore(),
		m_lpPlayer->GetLife(),
		m_lpPlayer->GetPower(),
		m_lpPlayer->GetBless(),
		m_lpEnemys->GetList()->size(),
		m_EnemyNum );
}

// �Փ˔���
void CGameMain::Collision()
{
	// �S�đ�������
	Enemy::CManager::ENEMYLIST*				EnemyList	= m_lpEnemys->GetList(); 
	Enemy::CManager::ENEMYLIST::iterator	Enemy		= EnemyList->begin();
	
	while( Enemy != EnemyList->end() )
	{
		// �����������Ă���s��Ȃ�
		if( (*Enemy)->GetFlg() & Enemy::CBase::BREAK )
		{
			++Enemy;
			continue;
		}

		// �G�ƒe
		BOOST_FOREACH( Shot::CBase* Shot, *m_lpPlayer->GetShots()->GetList() )
		{
			// �e�Ɣ���
			if( IsHitByCircle( 
				(*Enemy)->GetPos(), 1.f,
				Shot->GetPos(), Shot->GetRadius() ) )
			{
				if( !( (*Enemy)->GetFlg() & Enemy::CBase::BREAK ) &&
					( Shot->GetFlg() & Shot::CBase::FLG_ALIVE ) )
				{
					// ����
					(*Enemy)->Dispatch( Shot );
					Shot->Dispatch( (*Enemy) );
				}
			}
		}

		// ���@�ƓG�̓����蔻��
		// �����G���A�ɂ܂�Ă����画�肵�Ȃ�
		if( !( (*Enemy)->GetFlg() & Enemy::CBase::BUBBLE_WRAP ) )
		{
			if( IsHitByCircle( m_lpPlayer->GetPos(), 0.5f, 
				(*Enemy)->GetPos(), (*Enemy)->GetMesh()->GetRadius() ) )
			{
				(*Enemy)->Dispatch( m_lpPlayer );
				m_lpPlayer->Dispatch( (*Enemy) );
			}
		}

		// ���@�ƓG�̏o���Ă���e�Ƃ̓����蔻��
		if( (*Enemy)->GetShotManager() != NULL )
		{
			BOOST_FOREACH( Shot::CBase* Shot, *(*Enemy)->GetShotManager()->GetList() )
			{
				// ���@�Ƃ̏Փˌ��m
				if( IsHitByCircle( m_lpPlayer->GetPos(), 0.5f,
					Shot->GetPos(), Shot->GetRadius() ) )
				{
					// �Փˉ���
					Shot->Dispatch( m_lpPlayer );
					m_lpPlayer->Dispatch( Shot );
				}

				// ���@�̏o���Ă���e�Ƃ̏Փ˔���
				BOOST_FOREACH( Shot::CBase* PlayerShot, 
					*m_lpPlayer->GetShots()->GetList() )
				{
					// �Փˌ��m
					if( IsHitByCircle( PlayerShot->GetPos(), PlayerShot->GetRadius(),
						Shot->GetPos(), Shot->GetRadius() ) )
					{
						// �Փˉ���
						Shot->Dispatch( PlayerShot );
						PlayerShot->Dispatch( Shot );
					}
				}

				// �G���g�ƓG�̏o���Ă���e�̓����蔻��(���˕Ԃ��ꂽ���̌���)
				if( Shot->GetFlg() & Shot::CBase::FLG_FIN_HIT )
				{
					if( IsHitByCircle(
						(*Enemy)->GetPos(), (*Enemy)->GetMesh()->GetRadius(),
						Shot->GetPos(), Shot->GetRadius() ) )
					{
						Shot->Dispatch( (*Enemy) );
						(*Enemy)->Dispatch( Shot );
					}
				}
			}
		}

		// �G�ƏP���I�u�W�F�N�g�Ƃ̔���
		BOOST_FOREACH( CDamageObjectBase* Fish, *m_lpFishes->GetDamageList() )
		{
			if( IsHitByCircle( 
				(*Enemy)->GetPos(), (*Enemy)->GetMesh()->GetRadius(),
				Fish->GetPos(), 1.f ) )
			{
				Fish->Dispatch( (*Enemy) );
				(*Enemy)->Dispatch( Fish );
			}
		}

		// �G���m�̓����蔻��
		Enemy::CManager::ENEMYLIST::iterator Enemy2 = Enemy;
		
		// ��1�i�߂Ă���
		++Enemy2;
		
		while( Enemy2 != EnemyList->end() )
		{
			if( IsHitByCircle( (*Enemy)->GetPos(), (*Enemy)->GetMesh()->GetRadius(), 
				(*Enemy2)->GetPos(), (*Enemy2)->GetMesh()->GetRadius() ) )
			{
				(*Enemy)->Dispatch( (*Enemy2) );
				(*Enemy2)->Dispatch( (*Enemy) );
			}
			++Enemy2;
		}

		// BubbleBreaker�̓Ɠ��̏���(�댯)
		Enemy::CBubbleBreaker* lpBubbleBreaker =
			dynamic_cast<Enemy::CBubbleBreaker*>( (*Enemy) );

		// ���e�������
		if( lpBubbleBreaker != NULL )
		{
			// ��������Ō���
			BOOST_FOREACH( Enemy::CBase* TargetEnemy, *EnemyList )
			{
				// �A�ɕ�܂�Ă����
				if( TargetEnemy->GetBubbleScale() > 0 )
				{
					lpBubbleBreaker->SetDirEnemy( TargetEnemy );
				}
			}
		}

		++Enemy;
	}

	// �{�X�Ƃ̓����蔻��
	if( (m_Flg & FLG_BOSSBATTLE) && m_lpBoss )
	{
		BOOST_FOREACH( Shot::CBase* PlayerShot, *m_lpPlayer->GetShots()->GetList() )
		{
			// �v���C���[�̒e�ƃ{�X�̏Փ˔���
			if( IsHitByCircle( m_lpBoss->GetPos(), m_lpBoss->GetRadius(), 
				PlayerShot->GetPos(), PlayerShot->GetRadius() ) )
			{
				PlayerShot->Dispatch( m_lpBoss );
				m_lpBoss->Dispatch( PlayerShot );
			}
		}

		BOOST_FOREACH( Shot::CBase* BossShot, *m_lpBoss->GetShotManager()->GetList() )
		{
			// �v���C���[�ƃ{�X�̒e�Ƃ̏Փ˔���
			if( IsHitByCircle( m_lpPlayer->GetPos(), 0.5f, 
				BossShot->GetPos(), BossShot->GetRadius() ) )
			{
				BossShot->Dispatch( m_lpPlayer );
				m_lpPlayer->Dispatch( BossShot );
			}

			// �{�X�ƒ��˕Ԃ��ꂽ�e�Ƃ̔���
			if( IsHitByCircle( m_lpBoss->GetPos(), m_lpBoss->GetRadius(),
								BossShot->GetPos(), BossShot->GetRadius() ) )
			{
				BossShot->Dispatch( m_lpBoss );
				m_lpBoss->Dispatch( BossShot );
			}
		}

		BOOST_FOREACH( Shot::CBase* PlayerShot, *m_lpPlayer->GetShots()->GetList() )
		{
			BOOST_FOREACH( Shot::CBase* BossShot, *m_lpBoss->GetShotManager()->GetList() )
			{
				// �v���C���[�̒e�ƃ{�X�̒e�̓����蔻��
				if( IsHitByCircle( BossShot->GetPos(), BossShot->GetRadius(), 
									PlayerShot->GetPos(), PlayerShot->GetRadius() ) )
				{
					BossShot->Dispatch( PlayerShot );
					PlayerShot->Dispatch( BossShot );
				}
			}
		}

		// �v���C���[���g�ƃ{�X���g�̏Փ˔���
		if( IsHitByCircle( m_lpPlayer->GetPos(), 0.5f, 
			m_lpBoss->GetPos(), m_lpBoss->GetRadius() ) )
		{
			m_lpPlayer->Dispatch( m_lpBoss );
			m_lpBoss->Dispatch( m_lpPlayer );
		}
	}
}

// �`��
void CGameMain::Draw()
{
	// �f�o�C�X�l��
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// �`��J�n
	D3D::Begin();
	
	// ���e
	m_Proj.SetFar( 500 );
	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );

	// �J����(�ʒu�̓v���C���[�̏�Ԃɂ���ĕς���)
	CVector	VecTrans( 0.0f, 2.0f, -7.0f );
	CVector	VecLook( 0.0f, 0.0f, 100.0f );
	CVector VecUp( 0.f, 1.f, 0.f );

	// �J�����ݒ�ύX
	if( m_lpBoss && (m_Flg & FLG_BOSSENTER) )
	{
		VecLook = m_lpBoss->GetPos();
		
		// ����
		CMatrix mRotY, mTrans( 0, 2.0f, -6 + -2.0f * cos( D3DXToRadian(timeGetTime() / 10 % 360) ) );
		D3DXVECTOR4 Pos;

		mRotY.SetRotY( timeGetTime() / 10 % 360 );

		//VecTrans.GetTransCoord( &(m_lpBoss->GetMatrix()) );

		VecTrans.Set( 0, 0, 0 );

		D3DXVec3Transform( &Pos, &VecTrans, &(mTrans * mRotY * m_lpBoss->GetMatrix()) );

		VecTrans.Set( Pos.x, Pos.y, Pos.z );
	
	}else{

		if( m_lpPlayer->GetFlg() & Player::CDolphin::FLG_OVER )
		{
			VecTrans	= CVector( m_lpPlayer->GetSaveMat() );
			VecLook		= CVector( m_lpPlayer->GetMatrix() );

			VecTrans.y += 5;

		}else if( m_lpPlayer->GetFlg() & Player::CDolphin::FLG_JUMP ){

			// ���ʃW�����v��
			VecTrans.Set( 15, 0, 0 );

			VecLook	= CVector( m_lpPlayer->GetMatrix() );

			VecTrans.GetTransCoord( &m_lpPlayer->GetMatrix() );

			// Y�������Œ�
			VecTrans.y = JUMP_CAMERA_HEIGHT;

		}else{

			VecTrans.GetTransCoord	( &m_lpPlayer->GetAfterMatrix() );
			VecLook.GetTransCoord	( &m_lpPlayer->GetAfterMatrix() );
			VecUp.GetTransNormal	( &m_lpPlayer->GetAfterMatrix() );
		}
	}

	m_Camera.SetCamera( &VecTrans, &VecLook, &VecUp );

	lpDevice->SetTransform( D3DTS_VIEW, m_Camera.GetMatrix() );

	// ������J�����O����ޗ��Ƃ��ēo�^
	SetCullingCamera( &m_Camera );
	SetCullingProj( &m_Proj );

	// ��ʃN���A
	lpDevice->Clear( 0,
		NULL,
		D3DCLEAR_ZBUFFER | ((m_Flg & FLG_RESULT)? D3DCLEAR_TARGET : 0),
		D3DCOLOR_XRGB( 0, 0, 0 ),
		1.f, 0 );

	// �e�N�X�`���ɕ`��
	lpDevice->SetRenderTarget( 0, m_SceneTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_SceneTex.GetZbuffer() );
	lpDevice->SetViewport( &m_VP );

	// ���b�V���͌Œ�p�C�v���C���ŕ`��
	CXfile::SetDrawShader( false );
	CBoard::SetDrawShader( false );

	// �C�̐F�ω�(���ۂ͉�ʃN���A)
	// �����ɂ���ĐF��ς���
	float PlayerY = m_lpPlayer->GetPos().y;

	// ����������
	int Color = static_cast<int>( SEA_COLOR_ADD * PlayerY );
	
	// ���ۂɐF��ς���(�t�H�O��)
	lpDevice->Clear( 0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0x00, (Color < 0)? 0 : Color, 0x80 + Color ),
		1.f, 0 );

	lpDevice->SetRenderState( 
		D3DRS_FOGCOLOR, 
		D3DCOLOR_XRGB( 0x00, (Color < 0)? 0 : Color, 0x80 + Color ) );

	m_lpSurfaceSea->SetColor( 0x00, (Color < 0)? 0 : Color, 0x80 + Color );

	// �Z�x���ς���
	float Temp = 60.f + FOG_DEPTH_ADD * m_lpPlayer->GetPos().y;
	lpDevice->SetRenderState( D3DRS_FOGSTART, GetAddress( Temp ) );

	// �����_�����O
	Rendering( true );

	// �k�����Ăڂ���
	// �����_�[�^�[�Q�b�g��ς���
	D3DVIEWPORT9 Vp = { 0, 0, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, 0.f, 1.f };
	lpDevice->SetRenderTarget( 0, m_MiniSizeTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_MiniSizeTex.GetZbuffer() );
	lpDevice->SetViewport( &Vp );

	lpDevice->Clear( 0,
		NULL,
		D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_XRGB( 0, 0, 0 ),
		1.f, 0 );

	lpDevice->SetTransform( D3DTS_VIEW, &CMatrix() );
	lpDevice->SetTransform( D3DTS_PROJECTION, &CMatrix() );
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );
	lpDevice->SetFVF( CBoard::FVF_VERTEX );

	// �V�F�[�_�[�ɒl��n��
	m_BlurShader.SetTexture( "g_Texture", m_SceneTex.GetTexture() );

	// �V�F�[�_�[�`��J�n
	m_BlurShader.Begin();
	m_BlurShader.BeginPass( 0 );

	m_SceneBorad.SetVertex( 2 );
	m_SceneBorad.Draw( lpDevice );

	m_BlurShader.EndPass();
	m_BlurShader.End();

	// �����_�[�^�[�Q�b�g��ς���
	lpDevice->SetRenderTarget( 0, m_BlurTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_BlurTex.GetZbuffer() );
	m_BlurShader.SetTexture( "g_Texture", m_MiniSizeTex.GetTexture() );

	// �V�F�[�_�[�`��J�n
	m_BlurShader.Begin();
	m_BlurShader.BeginPass( 1 );

	m_SceneBorad.SetVertex( 2 );
	m_SceneBorad.Draw( lpDevice );

	m_BlurShader.EndPass();
	m_BlurShader.End();
	
	// �[�x�v�Z����
	// �����_�[�^�[�Q�b�g��ς���
	lpDevice->SetRenderTarget( 0, m_DepthTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_DepthTex.GetZbuffer() );
	Vp.Width = WINDOW_WIDTH;
	Vp.Height= WINDOW_HEIGHT;
	lpDevice->SetViewport( &Vp );

	lpDevice->Clear( 0,
		NULL,
		D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_XRGB( 0, 0, 0 ),
		1.f, 0 );

	// �V�F�[�_�[�ɒl��n��
	m_DepthShader.SetValue( "g_View", m_Camera.GetMatrix(), sizeof(D3DXMATRIX) );
	m_DepthShader.SetValue( "g_Proj", m_Proj.GetMatrix(), sizeof(D3DXMATRIX) );

	// ���b�V���̓V�F�[�_�[�ŕ`�悷��
	CXfile::SetDrawShader( true );
	CBoard::SetDrawShader( true );

	// �V�F�[�_�[�`��J�n
	m_DepthShader.Begin();
	m_DepthShader.BeginPass( 0 );

	lpDevice->Clear( 0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0, 0, 0 ),
		1.f, 0 );

	lpDevice->SetRenderState( 
		D3DRS_FOGCOLOR, 
		D3DCOLOR_XRGB( 0, 0, 0 ) );

	Rendering( false );
	
	m_DepthShader.EndPass();
	m_DepthShader.End();

	// �V�[���A�ڂ����A�[�x�v�Z���g���Ĕ�ʊE�[�x�K�p
	// ���Ƃ̃����_�����O�^�[�Q�b�g�ɖ߂�
	lpDevice->SetRenderTarget( 0, m_ResultTex.GetSurface() );
	lpDevice->SetDepthStencilSurface( m_ResultTex.GetZbuffer() );
	
	// �V�F�[�_�[�Ƀe�N�X�`���𗬂�
	m_DOFShader.SetTexture( "g_SceneTex", m_SceneTex.GetTexture() );	// �V�[��
	m_DOFShader.SetTexture( "g_BlurTex", m_BlurTex.GetTexture() );		// �ڂ���
	m_DOFShader.SetTexture( "g_DepthTex", m_DepthTex.GetTexture() );	// �[�x

	// �v���C���[�̈ʒu�ɂ���ăt�H�[�J�X��ς���
	float FocusRate = (m_lpPlayer->GetPos().y + abs( BOTTOMSEA_HEIGHT )) * FOCUS_RATE;

	m_DOFShader.SetValue( "g_Focus", &FocusRate, sizeof(float) );

	// �|���S���ŃV�[���`��
	lpDevice->SetTransform( D3DTS_VIEW, &CMatrix() );
	lpDevice->SetTransform( D3DTS_PROJECTION, &CMatrix() );
	lpDevice->SetTransform( D3DTS_WORLD, &CMatrix() );

	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	
	m_DOFShader.Begin();
	m_DOFShader.BeginPass( 0 );

	m_SceneBorad.Draw( lpDevice );

	m_DOFShader.EndPass();
	m_DOFShader.End();

	// �{�X�o�g����
	if( (m_Flg & FLG_BOSSBATTLE) && m_lpBoss )
	{
		// �����u���[��������
		// �����_�[�^�[�Q�b�g�ύX
		lpDevice->SetRenderTarget( 0, m_SceneTex.GetSurface() );
		lpDevice->SetDepthStencilSurface( NULL );

		// �����ʒu�ݒ�
		m_BombBlurShader.SetValue(
			"g_CenterTexPos",
			&GetTexPos( *m_Camera.GetMatrix(), 
						*m_Proj.GetMatrix(),
						m_lpBoss->GetPos() ),
			sizeof( D3DXVECTOR2 ) );

		// ��ʃT�C�Y�w��
		m_BombBlurShader.SetValue(
			"g_TexOffset",
			&D3DXVECTOR2( 1.f / WINDOW_WIDTH, 1.f / WINDOW_HEIGHT ),
			sizeof( D3DXVECTOR2 ) );

		float Temp = m_lpBoss->GetBlurPower();
		// �����̋���
		m_BombBlurShader.SetValue(
			"g_BlurPower",
			&Temp,
			sizeof( float ) );

		Temp = m_lpBoss->GetAddColorRate();

		// �����̌��̋���
		m_BombBlurShader.SetValue(
			"g_AddColorRate",
			&Temp,
			sizeof( float ) );

		// �e�N�X�`���ݒ�
		m_BombBlurShader.SetTexture( "g_Tex", m_ResultTex );

		m_BombBlurShader.Begin();
		m_BombBlurShader.BeginPass( 0 );

		m_SceneBorad.Draw( lpDevice );

		m_BombBlurShader.EndPass();
		m_BombBlurShader.End();

		lpDevice->SetRenderTarget( 0, m_ResultTex.GetSurface() );
		lpDevice->SetDepthStencilSurface( m_ResultTex.GetZbuffer() );

		lpDevice->SetTexture( 0, m_SceneTex );
		m_SceneBorad.Draw( lpDevice );
	}

	// ���̃����_�[�^�[�Q�b�g�ɖ߂�
	lpDevice->SetRenderTarget( 0, m_lpBackBuffer );
	lpDevice->SetDepthStencilSurface( m_lpZBuffer );

	CBoard::SetDrawShader( false );

	// �X�v���C�g�`��
	DrawSprite();

	// �`��I��
	D3D::End();
}

// ���b�V����`��
void CGameMain::DrawMesh( bool is_effect )
{
	// �f�o�C�X�l��
	LPDIRECT3DDEVICE9	lpDevice = D3D::GetDevice();

	D3DXVECTOR3 PlayerPos = m_lpPlayer->GetPos();

	// �s��
	CMatrix mWorld;
	CMatrix mTrans( m_lpPlayer->GetPos() );
	CMatrix mScale;

	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	// ��
	// �C�ʂƓ������ɓ�����
	D3DXVECTOR3 TempVec = m_lpPlayer->GetPos();
	TempVec.y = SURFACESEA_HEIGHT;
	mTrans.SetTrans( &TempVec );

	// ��������������
	mScale.SetScale( STAGE_SCALE );

	// ��������]������
	mWorld = mScale * mTrans;

	lpDevice->SetTransform( D3DTS_WORLD, &mWorld );

	// �C�ʂɋ߂���΃t�H�O�𖳌���
	//if( m_lpPlayer->GetPos().y >= SURFACESEA_HEIGHT / 1.1f )
	{
		lpDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	}

	m_lpSky->Draw( lpDevice );

	// �t�H�O��L����
	lpDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );

	// ���`��
	if( is_effect )
	{
		m_lpStones->DrawShader( lpDevice );
	
	}else{

		m_lpStones->Draw( lpDevice );
	}

	// ����ł���G
	m_lpEnemys->DrawDie( lpDevice, is_effect );

	// �����C�e�B���O�����̂������
	lpDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	// �����Ă���G
	m_lpEnemys->DrawAlive( lpDevice, is_effect );

	// �����߂��鋛
	m_lpFishes->DrawDamageFish( lpDevice, is_effect );

	// �{�X
	if( (m_Flg & FLG_BOSSBATTLE) && m_lpBoss )
	{
		if( is_effect )
		{
			m_lpBoss->DrawShader( lpDevice );
		
		}else{

			m_lpBoss->Draw( lpDevice );
		}
	}

	// �L����
	lpDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_lpPlayer->Draw( lpDevice );
}

// ���b�V������Ȃ�3D�I�u�W�F�N�g��`��
void CGameMain::DrawPolygon( bool is_effect )
{
	// �f�o�C�X�l��
	LPDIRECT3DDEVICE9	lpDevice = D3D::GetDevice();

	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	// ��
	m_lpFishes->DrawObjectFish( lpDevice );

	// �v���C���[�̍��W�l��
	D3DXVECTOR3 PlayerPos = m_lpPlayer->GetPos();

	lpDevice->SetFVF( CBoard::FVF_VERTEX );
	//lpDevice->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
	lpDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	lpDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	// ���e�X�g
	m_lpSunLines->Update();
	//m_lpSunLines->Draw( lpDevice );
	
	// �C��
	if( is_effect )
	{
		m_lpSurfaceSea->DrawShader( lpDevice );
	
	}else{

		m_lpSurfaceSea->Draw( lpDevice );
	}

	// ��
	m_lpFishes->DrawObjectFish( lpDevice );

	// ��
	m_Walls.Draw( lpDevice );

	// ���e�X�g
	m_lpSunLines->Draw( lpDevice );

	// �v�����N�g��
	m_lpPlanktons->Draw( lpDevice, &m_Camera );

	// �v���C���[�̔g��
	m_lpPlayer->DrawRipples( lpDevice );
}

// �G�t�F�N�g�ޕ`��
void CGameMain::DrawEffect()
{
	// �f�o�C�X�l��
	LPDIRECT3DDEVICE9	lpDevice = D3D::GetDevice();

	// ���Z�����Ŕg���\��
	lpDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	lpDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

	// �G�̔g��
	m_lpEnemys->DrawRipples( lpDevice );

	// �G�̃G�t�F�N�g�`��
	m_lpEnemys->DrawEffect( lpDevice );

	// �{�X�̃G�t�F�N�g
	if( m_lpBoss )
	{
		m_lpBoss->DrawEffect( lpDevice );
	}

	// �v���C���[�̃G�t�F�N�g
	m_lpPlayer->DrawEffect( lpDevice );

	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	lpDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
}

// �X�v���C�g�`��
void CGameMain::DrawSprite()
{
	// �X�v���C�g�`��
	LPD3DXSPRITE lpSprite = m_lpSprite->Begin( D3DXSPRITE_ALPHABLEND );

	RECT Rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	// �Q�[�����
	lpSprite->SetTransform( &CMatrix() );

	// �W�����v���̓u���[�������Ȃ�
	int NowAlpha = m_GameSceneAlpha;

	if( m_lpPlayer->GetFlg() & Player::CDolphin::FLG_JUMP )
	{
		NowAlpha = 0xff;
	}

	lpSprite->Draw(
		m_ResultTex,
		&Rc, NULL, NULL,
		D3DCOLOR_ARGB( NowAlpha, 0xff, 0xff, 0xff ) );

	// ���ʂ���������t�F�[�h�A�E�g
	if( m_Flg & FLG_RESULT )
	{
		m_lpSprite->SetAlpha( m_GameSceneAlpha );
		m_lpUI->SetAlpha( m_GameSceneAlpha );
	}

	// UI�`��
	m_lpUI->Draw( m_lpSprite->GetDevice() );
	m_lpUI->DrawRadar( 
		m_lpSprite->GetDevice(),
		&m_lpPlayer->GetMatrix(),
		m_lpEnemys,
		(m_Flg & FLG_BOSSBATTLE)? m_lpBoss : NULL );

	// �_���[�W
	if( m_lpPlayer->GetFlg() & Player::CDolphin::FLG_DAMAGE )
	{
		m_DamageAlphaCnt = 0xff;
	}

	// �_���[�W�`��
	if( m_DamageAlphaCnt > 0 && !(m_lpPlayer->GetFlg() & Player::CDolphin::FLG_JUMP) )
	{
		m_lpSprite->SetAlpha( m_DamageAlphaCnt );
		
		m_lpSprite->Draw(
			CSTexManager::GetInst()->GetData( "Damage.png" )->GetTexture(),
			0, 0, 0,
			0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );

		m_lpSprite->SetAlpha( 0xff );
	}

	// �X�v���C�g�`��I��
	m_lpSprite->End();
}

// �ǂݍ���
void CGameMain::Load()
{
	// �e�N�X�`�����[�h�t�@�C���I�[�v��
	CSTexManager::GetInst()->LoadFromTextList( D3D::GetDevice(), "LoadData/GameMain/Tex.txt" );

	// X�t�@�C�����[�h�t�@�C���I�[�v��
	CSXfileManager::GetInst()->LoadFromTextList( D3D::GetDevice(), "LoadData/GameMain/Xdata.txt" );

	// ���y�ǂݍ���
	CSoundManager::GetInst()->LoadFromTextList( "LoadData/GameMain/Sound.txt" );
}

// �ǂݍ���
void CGameMain::Release()
{
	// �e�N�X�`�����[�h�t�@�C���I�[�v��
	CSTexManager::GetInst()->ReleaseFromTextList( "LoadData/GameMain/Tex.txt" );

	// X�t�@�C�����[�h�t�@�C���I�[�v��
	CSXfileManager::GetInst()->ReleaseFromTextList( "LoadData/GameMain/Xdata.txt" );
}

// �X�e�[�W���Ƃ̓ǂݍ���
void CGameMain::LoadStage(BYTE stage)
{
	std::string Path( "LoadData/GameMain/Stage" );
	std::string Temp;

	// ����������
	Path += stage + '0';

	// �R�s�[
	Temp = Path;

	// �ǂݍ��ރt�@�C������ǉ�
	Temp += PATH_ENEMYDATA;

	// �G��ǂݍ���
	m_lpEnemys->LoadFromFile( Temp.c_str() );

	// �G���L��
	m_EnemyNum = m_lpEnemys->GetList()->size();

	Temp = Path;

	Temp += PATH_STONEDATA;

	// ��̓ǂݍ���
	m_lpStones->LoadFromData( D3D::GetDevice(), Temp.c_str() );

	Temp = Path;

	Temp += PATH_DAMAGEFISHDATA;

	// �_���[�W���󂯂鋛�ǂݍ���
	m_lpFishes->LoadDamageFish( Temp.c_str() );

	Temp = Path;

	Temp += "/IsBoss.txt";

	// �{�X
	std::ifstream FileIn( Temp.c_str() );
	
	// �e�L�X�g�t�@�C��������Γǂݍ���
	if( FileIn.is_open() )
	{
		m_lpBoss = NEW Boss::CHardBorder( D3D::GetDevice() );
		CSoundManager::GetInst()->Load( m_lpBoss->GetBGMName() );
	}
}

// ���I�������m�ۂ��s��
void CGameMain::Allocs()
{
	// �X�v���C�g
	DebugMessageBox( "�X�v���C�g�f�o�C�X���쐬���܂�" );
	m_lpSprite = NEW CSprite( D3D::GetDevice() );

	// ���[�U�[�C���^�[�t�F�C�X
	DebugMessageBox( "UI���쐬���܂�" );
	m_lpUI = NEW CUI();

	// �v���C���[
	DebugMessageBox( "�v���C���[���쐬���܂�" );
	m_lpPlayer = NEW Player::CDolphin();

	// �G
	DebugMessageBox( "�G�}�l�[�W���[���쐬���܂�" );
	m_lpEnemys = NEW Enemy::CManager();

	// �C��
	DebugMessageBox( "�C�ʂ��쐬���܂�" );
	m_lpSurfaceSea = NEW CSurfaceSea();

	// �v�����N�g��
	DebugMessageBox( "�v�����N�g���}�l�[�W���[���쐬���܂�" );
	m_lpPlanktons = NEW CPlanktonManager();

	// ��
	DebugMessageBox( "��}�l�[�W���[���쐬���܂�" );
	m_lpStones = NEW CStoneManager();

	// ��
	DebugMessageBox( "���}�l�[�W���[���쐬���܂�" );
	m_lpFishes = NEW CFishManager();

	// ���z��
	m_lpSunLines = NEW CSunLineManager();

	DebugMessageBox( "���I�������m�ۊ���" );
}

// �����_�����O
void CGameMain::Rendering( bool is_effect )
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// ���b�V���ނ�`��
	DrawMesh( is_effect );
	
	// ���b�V���ȊO��3D�`��
	DrawPolygon( is_effect );

	// �G�t�F�N�g�ޕ`��
	DrawEffect();
}