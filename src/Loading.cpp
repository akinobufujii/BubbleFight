////////////////////////////////////
// �C���N���[�h
////////////////////////////////////
#include "Loading.h"

////////////////////////////////////
// �ÓI�����o���̉�
////////////////////////////////////
const LPSTR CLoading::TEXPASS_LOADFONT	= "Loading.png";
const BYTE	CLoading::FONT_ANIMEMAX		= 5;
const short CLoading::FONT_WIDTH		= 420;
const short	CLoading::FONT_HEIGHT		= 300 / FONT_ANIMEMAX;
const BYTE	CLoading::ANIME_TIME		= 200;

////////////////////////////////////
// ����
////////////////////////////////////

// �R���X�g���N�^
CLoading::CLoading():
m_IsRun( true ),
m_AnimeCnt( 0 ),
m_Sprite( D3D::GetDevice() ),
m_lpFontTex( CSTexManager::GetInst()->GetData( TEXPASS_LOADFONT ) ),
m_SaveTime( timeGetTime() )
{
}

// ���\�b�h���s�֐�
void CLoading::operator ()()
{
	// �I���ʍ�������܂Ŗ������[�v
	while( m_IsRun )
	{
		// �����J�n
		//CSSyncro::GetInst()->Lock();

		// �`��J�n
		D3D::Begin();

		// ��ʃN���A
		D3D::GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.f, 0 );

		// �X�v���C�g�`��J�n
		m_Sprite.Begin();

		m_Sprite.Draw(
			m_lpFontTex->GetTexture(),
			WINDOW_WIDTH - FONT_WIDTH, WINDOW_HEIGHT - FONT_HEIGHT, 0, 
			0, FONT_HEIGHT * m_AnimeCnt,
			FONT_WIDTH, FONT_HEIGHT * (m_AnimeCnt + 1) );

		// �X�v���C�g�`��I��
		m_Sprite.End();

		// �`��I��
		D3D::End();

		// �����J�n
		//CSSyncro::GetInst()->UnLock();

		// �҂�
		if( timeGetTime() < m_SaveTime )
		{

			Sleep( ANIME_TIME );

		}else{

			Sleep( ANIME_TIME - (timeGetTime() - m_SaveTime) );
		}
		
		m_SaveTime = timeGetTime();

		++m_AnimeCnt %= FONT_ANIMEMAX;
	}
}

// �I���ʍ�
void CLoading::End()
{
	m_IsRun = false;
}

// �f�X�g���N�^
CLoading::~CLoading()
{
}
