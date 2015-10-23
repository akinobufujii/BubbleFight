///////////////////////////////
// �C���N���[�h
///////////////////////////////
#include "Title.h"
#include "FishManager.h"

#include "RippleManager.h"
#include "BubbleManager.h"
#include "FinTail.h"
#include "BubbleManager.h"
#include "ShotManager.h"
#include "StrongDolphin.h"

#include "UI.h"

#include "Clear.h"

///////////////////////////////
// ���O��ԏȗ�
///////////////////////////////
using namespace Sequence;

///////////////////////////////
// �ÓI�����o���̉�
///////////////////////////////
const LPSTR	CClear::PASS_BACK_GROUND_MESH	= "stage.x";
const LPSTR	CClear::TEX_BUBBLE				= "Sprite3.png";
const LPSTR CClear::TEXPASS_FONT			= "ClearFont.png";
const short	CClear::BUBBLE_ADD_TIME			= 300;		
const BYTE	CClear::BUBBLE_NUM				= 50;				
const BYTE	CClear::BUBBLE_ADD_POS_LIMIT	= 3;	
const int	CClear::FONT_WIDTH				= 400;
const int	CClear::FONT_HEIGHT				= 190 / 2;
const BYTE	CClear::FADEIN_OUT_SPEED		= 5;
const BYTE	CClear::FADEOUT_MIN				= 80;
const BYTE	CClear::NUMBER_SIZE				= 60;
const BYTE	CClear::GRAPHNUM_COLON			= 10;
const int	CClear::DEST_SCOREFONT_X		= WINDOW_WIDTH / 2 - FONT_WIDTH / 3 * 2;
const int	CClear::DEST_SCOREFONT_Y		= FONT_HEIGHT * 1.5f;
const int	CClear::DEST_NUMBER_X			= WINDOW_WIDTH / 2 - 20;
const int	CClear::DEST_TOTALSCORENUM_X	= WINDOW_WIDTH / 2 + 30;
const int	CClear::DEST_TOTALSCORENUM_Y	= DEST_TOTALSCORE_Y + 20;
const int	CClear::DEST_SOCRE_Y			= FONT_HEIGHT * 1.8f;
const int	CClear::DEST_CLEARTIME_Y		= FONT_HEIGHT * 2.8f;
const int	CClear::DEST_TOTALSCORE_Y		= DEST_SCOREFONT_Y + FONT_HEIGHT * 2.5f;

///////////////////////////////
// ����
///////////////////////////////

// �R���X�g���N�^
CClear::CClear( ULONG score, double clear_time ):
m_IsRun( true ),
m_BubbleSaveTime( 0 ),
m_Alpha( 0 ),
m_IsFadeIn( true ),
m_lpDolphin( NULL ),
m_Score( score ),
m_ClearSecond( static_cast<int>( clear_time ) )
{ 
}

// �f�X�g���N�^
CClear::~CClear()
{
	// ���
	BOOST_FOREACH( CPointSpriteManager* It, m_PointBubbleList )
	{
		SAFE_DELETE( It );
	}

	m_PointBubbleList.clear();

	SAFE_DELETE( m_lpDolphin );

	Release();
}

// �ǂݍ���
void CClear::FileLoad()
{
	Load();

	m_Sprite.Create( D3D::GetDevice() );

	CSoundManager::GetInst()->Play( "Sound/ClearBGM.wav", true );

	m_Fishes.SetNumAndLimit( 10, D3DXVECTOR3( 10, 2, 2 ), 0 );
	m_Fishes.SetRot( 180 );

	m_Fishes.SetMatrix( CMatrix( -25, 0, 0 ) );
	m_FishMoveTime = timeGetTime();

	// ���C�g�ݒ�
	D3DLIGHT9	Light;
	ZeroMemory( &Light, sizeof( Light ) );

	Light.Type	= D3DLIGHT_DIRECTIONAL;	// ���s����
	Light.Diffuse.r	= 1.0f;
	Light.Diffuse.g	= 1.0f;
	Light.Diffuse.b	= 0.0f;

	Light.Position.x= 0.0f;
	Light.Position.y= 1.0f;
	Light.Position.z= 0.0f;

	Light.Range	= 10.f;

	Light.Direction = D3DXVECTOR3( 1.0f, -1.0f, 1.0f);

	D3D::GetDevice()->SetRenderState( D3DRS_LIGHTING, TRUE );
	D3D::GetDevice()->SetRenderState( D3DRS_FOGENABLE, FALSE );
	D3D::GetDevice()->SetLight( 0, &Light );
	D3D::GetDevice()->LightEnable( 0, TRUE );

	CXfile::SetDrawShader( false );
	CBoard::SetDrawShader( false );

	// ���Ԃ𕪂ƕb�ɕ�����
	/*while( true )
	{
		if( m_ClearSecond >= 60 )
		{
			m_ClearMinit++;
			m_ClearSecond -= 60;
		
		}else{

			break;
		}
	}*/

	// �C���J�쐬
	/*m_lpDolphin = NEW Player::CDolphin();

	m_lpDolphin->SetMatrix( CMatrix( 0, 0, 0 ) );*/
}

// ���s�֐�
bool CClear::RunProc()
{
	Update();

	Draw();

	return m_IsRun;
}

// �X�V
void CClear::Update()
{
	if( Input::Keyboard::IsState( DIK_Z, Input::PUSH ) )
	{
		m_IsRun = false;
	}

	// ������������
	if( timeGetTime() - m_BubbleSaveTime > BUBBLE_ADD_TIME )
	{
		// �A�쐬
		CPointSpriteManager* lpTemp = NEW CPointSpriteManager();

		lpTemp->SetNum( BUBBLE_NUM );
		lpTemp->SetTexture( CSTexManager::GetInst()->GetData( TEX_BUBBLE )->GetTexture() );
		lpTemp->SetMove( D3DXVECTOR3( 1.f, -1.f, 1.f ) );
		
		D3DXVECTOR3 Pos(
			GetPosiOrNega() * BUBBLE_ADD_POS_LIMIT,
			GetPosiOrNega() * BUBBLE_ADD_POS_LIMIT,
			GetPosiOrNega() * BUBBLE_ADD_POS_LIMIT );

		lpTemp->SetPos( Pos );
		
		m_PointBubbleList.push_back( lpTemp );

		m_BubbleSaveTime = timeGetTime();	// ���ԕۑ�
	}

	// �C���J
	CVector Target = m_Target;

	CMatrix mInv = m_mDolphin;
	mInv.GetInverse();

	Target.GetTransCoord( &mInv );

	// �A�X�V(�r���폜������̂Œʏ탋�[�v)
	BUBBLELIST::iterator It = m_PointBubbleList.begin();

	while( It != m_PointBubbleList.end() )
	{
		// �X�V
		(*It)->Update();

		// �폜����Ȃ�폜
		if( (*It)->IsErase() )
		{
			SAFE_DELETE( (*It) );
			It = m_PointBubbleList.erase( It );

			continue;
		}

		++It;
	}

	// �t�@�[�h�C���E�A�E�g
	if( m_IsFadeIn )
	{
		if( (m_Alpha += FADEIN_OUT_SPEED) > 0xff )
		{
			m_Alpha = 0xff;
			m_IsFadeIn = false;
		}

	}else{

		if( (m_Alpha -= FADEIN_OUT_SPEED) < FADEOUT_MIN )
		{
			m_Alpha = FADEOUT_MIN;
			m_IsFadeIn = true;
		}
	}

	m_Fishes.Update( false );

	if( timeGetTime() - m_FishMoveTime > 10000 )
	{
		m_Fishes.SetRot( 180 );

		m_FishMoveTime = timeGetTime();
	}
}

// �`��
void CClear::Draw()
{
	LPDIRECT3DDEVICE9 lpDevice = D3D::GetDevice();

	// �`��J�n
	D3D::Begin();

	// ��ʃN���A
	lpDevice->Clear( 0,	NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0x00, 0x00, 0xff ),
		1.f, 0 );

	// ���e
	lpDevice->SetTransform( D3DTS_PROJECTION, m_Proj.GetMatrix() );

	// �J����
	m_Camera.SetCamera(
		&D3DXVECTOR3( 0, 0, -10 ),
		&D3DXVECTOR3( 0, 0, 0 ),
		&D3DXVECTOR3( 0, 1, 0 ) );

	lpDevice->SetTransform( D3DTS_VIEW, m_Camera.GetMatrix() );

	// �J�����O�ޗ��o�^
	SetCullingCamera( &m_Camera );
	SetCullingProj( &m_Proj );

	// �X�e�[�W
	lpDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	lpDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	CMatrix mScale;

	mScale.SetScale( 5 );

	lpDevice->SetTransform( D3DTS_WORLD, &(mScale * CMatrix( 0, -20, 0 ) ) );

	CSXfileManager::GetInst()->GetData( PASS_BACK_GROUND_MESH )->Draw( lpDevice );

	// �C���J
	//lpDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	//m_lpDolphin->Draw( lpDevice );
	//lpDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	
	// �A
	// �����_�����O�ݒ�
	lpDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	lpDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	lpDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	lpDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );

	// �`��
	BOOST_FOREACH( CPointSpriteManager* It, m_PointBubbleList )
	{
		It->Draw( lpDevice );
	}

	lpDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	lpDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	m_Fishes.DrawObjectFish( lpDevice );

	// �X�v���C�g�`��J�n
	m_Sprite.Begin();

	// �t�H���g�`��
	CTexture& FontTex = *CSTexManager::GetInst()->GetData( TEXPASS_FONT );

	// Scene
	m_Sprite.Draw(
		FontTex,
		0, 0, 0, 
		0, FONT_HEIGHT * FONTKIND_SCENE, 
		FONT_WIDTH, FONT_HEIGHT * (FONTKIND_SCENE + 1 ) );

	// clear
	m_Sprite.Draw(
		FontTex,
		WINDOW_WIDTH / 2, 0, 0, 
		0, FONT_HEIGHT * FONTKIND_CLEAR,
		FONT_WIDTH, FONT_HEIGHT * (FONTKIND_CLEAR + 1 ) );

	// Score
	m_Sprite.Draw(
		FontTex,
		DEST_SCOREFONT_X, DEST_SCOREFONT_Y, 0, 
		0, FONT_HEIGHT * FONTKIND_SCORE_AND_TIME,
		FONT_WIDTH / 2, FONT_HEIGHT * (FONTKIND_SCORE_AND_TIME + 1) );

	// Time
	m_Sprite.Draw(
		FontTex,
		DEST_SCOREFONT_X, DEST_SCOREFONT_Y + FONT_HEIGHT, 0, 
		FONT_WIDTH / 2, FONT_HEIGHT * FONTKIND_SCORE_AND_TIME,
		FONT_WIDTH, FONT_HEIGHT * (FONTKIND_SCORE_AND_TIME + 1) );

	// Total Score
	m_Sprite.Draw(
		FontTex,
		0, DEST_TOTALSCORE_Y, 0, 
		0, FONT_HEIGHT * FONTKIND_TOTALSCORE,
		FONT_WIDTH, FONT_HEIGHT * (FONTKIND_TOTALSCORE + 1) );

	// ����
	m_Sprite.SetAlpha( m_Alpha );

	m_Sprite.Draw(
		FontTex,
		WINDOW_WIDTH / 2 - FONT_WIDTH / 2, WINDOW_HEIGHT - FONT_HEIGHT * 1.5f, 0, 
		0, FONT_HEIGHT * FONTKIND_ZKEY,
		FONT_WIDTH, FONT_HEIGHT * (FONTKIND_ZKEY + 1) );

	m_Sprite.SetAlpha( 0xff );

	// �����e�N�X�`���擾
	CTexture& NumTex = *CSTexManager::GetInst()->GetData( "number.png" );

	// �X�R�A���_
	char Str[ 0xff ];

	// ������ɕϊ�
	sprintf_s( Str, sizeof( Str ), "%d", m_Score );

	for( BYTE i = 0, Num = 0; Str[ i ] != '\0'; i++ )
	{
		// ������𐔎��ɕϊ�
		Num = Str[ i ] - '0';

		// �`��
		m_Sprite.Draw(
			NumTex,
			DEST_NUMBER_X + NUMBER_SIZE * i, DEST_SOCRE_Y, 0,
			NUMBER_SIZE * Num, 0, NUMBER_SIZE * (Num + 1), NUMBER_SIZE );
	}

	// �N���A����
	sprintf_s( Str, sizeof( Str ), "%d %02d", m_ClearSecond / 60, m_ClearSecond % 60 );

	for( BYTE i = 0, Num = 0; Str[ i ] != '\0'; i++ )
	{
		// ������𐔎��ɕϊ�
		Num = Str[ i ] - '0';

		// �����ȊO�Ȃ�L����`��
		if( Num < 0 || Num > 9 )
		{
			Num = GRAPHNUM_COLON;
		}
		
		m_Sprite.Draw(
			NumTex,
			DEST_NUMBER_X + NUMBER_SIZE * i, DEST_CLEARTIME_Y, 0,
			NUMBER_SIZE * Num, 0, NUMBER_SIZE * (Num + 1), NUMBER_SIZE );
	}

	// �g�[�^���X�R�A
	sprintf_s( Str, sizeof( Str ), "%d", m_Score * 10 - m_ClearSecond );

	for( BYTE i = 0, Num = 0; Str[ i ] != '\0'; i++ )
	{
		// ������𐔎��ɕϊ�
		Num = Str[ i ] - '0';

		// �����ȊO�Ȃ�L����`��
		if( Num < 0 || Num > 9 )
		{
			Num = GRAPHNUM_COLON;
		}
		
		m_Sprite.Draw(
			NumTex,
			DEST_TOTALSCORENUM_X + NUMBER_SIZE * i, DEST_TOTALSCORENUM_Y, 0,
			NUMBER_SIZE * Num, 0, NUMBER_SIZE * (Num + 1), NUMBER_SIZE );
	}

	// �X�v���C�g�`��I��
	m_Sprite.End();

	// �`��I��
	D3D::End();
}

// ���̃V�[�P���X
IBase* CClear::GetNext()
{
	return NEW CTitle();
}

// �ǂݍ���
void CClear::Load()
{
	// �e�N�X�`���ǂݍ��݃��X�g����ǂݍ���
	CSTexManager::GetInst()->LoadFromTextList( D3D::GetDevice(), "LoadData/Clear/Tex.txt" );

	// X�t�@�C���ǂݍ��݃��X�g����ǂݍ���
	CSXfileManager::GetInst()->LoadFromTextList( D3D::GetDevice(), "LoadData/Clear/Xdata.txt" );
}

// ���
void CClear::Release()
{
	CSTexManager::GetInst()->ReleaseFromTextList( "LoadData/Clear/Tex.txt" );

	CSXfileManager::GetInst()->ReleaseFromTextList( "LoadData/Clear/Xdata.txt" );

	CSoundManager::GetInst()->Stop( "Sound/ClearBGM.wav" );
}