////////////////////////////
// �C���N���[�h
////////////////////////////
#include "GraphicAnime.h"

////////////////////////////
// �ÓI�����o���̉�
////////////////////////////
const BYTE	CGraphicAnime::FLG_MOVE_WIDTH	= 0x01;
const BYTE	CGraphicAnime::FLG_LOOP_ANIME	= 0x02;
const BYTE	CGraphicAnime::FLG_BACK_LOOP	= 0x04;
const BYTE	CGraphicAnime::FLG_NOW_BACK		= 0x08;

////////////////////////////
// ����
////////////////////////////

// �R���X�g���N�^
CGraphicAnime::CGraphicAnime():
m_AnimeCnt( 0 ),
m_SaveTime( 0 ),
m_AnimeNum( 0 ),
m_WidthRate( 0.f ),
m_HeightRate( 0.f ),
m_Flg( FLG_MOVE_WIDTH )
{
}

// �f�X�g���N�^
CGraphicAnime::~CGraphicAnime()
{
}

// �X�V
void CGraphicAnime::Update()
{
	BYTE WidthAnime, HeightAnime;	// �A�j���[�V������
	
	if( m_Flg & FLG_MOVE_WIDTH )
	{
		WidthAnime = m_AnimeCnt % m_WidthNum;
		HeightAnime= m_AnimeCnt / m_HeightNum;

	}else{

		WidthAnime = m_AnimeCnt / m_WidthNum;
		HeightAnime= m_AnimeCnt % m_HeightNum;
	}

	m_TexPos[ 0 ] = D3DXVECTOR2( m_WidthRate * WidthAnime,			m_HeightRate * HeightAnime );
	m_TexPos[ 1 ] = D3DXVECTOR2( m_WidthRate * (WidthAnime + 1),	m_HeightRate * HeightAnime );
	m_TexPos[ 2 ] = D3DXVECTOR2( m_WidthRate * (WidthAnime + 1),	m_HeightRate * (HeightAnime + 1) );
	m_TexPos[ 3 ] = D3DXVECTOR2( m_WidthRate * WidthAnime,			m_HeightRate * (HeightAnime + 1) );

	// �X�V����Ȃ�
	if( timeGetTime() - m_SaveTime > m_NextAnimeTime )
	{
		// �o�b�N���Ȃ�
		if( m_Flg & FLG_NOW_BACK )
		{
			if( --m_AnimeCnt == 0 )
			{
				m_Flg &= ~FLG_NOW_BACK;
			}

		}else{

			if( ++m_AnimeCnt >= m_AnimeNum )
			{
				// �o�b�N���[�v�Ȃ�
				if( m_Flg & FLG_NOW_BACK )
				{
					m_AnimeCnt = m_AnimeNum;
					m_Flg |= FLG_NOW_BACK;

				}else if( m_Flg & FLG_LOOP_ANIME ){

					// ���[�v�A�j���[�V�����Ȃ�J�E���^��߂�
					m_AnimeCnt = 0;

				}else{

					// �����w�肪�Ȃ���΃J�E���^���~�߂�
					m_AnimeCnt = m_AnimeNum;
				}
			}
		}

		m_SaveTime = timeGetTime();
	}
}

// �A�j���[�V�����ݒ�
void CGraphicAnime::SetAnimeSize(const float width,
							 const float height,
							 const float anime_width,
							 const float anime_height)
{
	m_WidthRate = 1.f / width * anime_width;
	m_HeightRate= 1.f / height* anime_height;
}

// �A�j���[�V�������ݒ�
void CGraphicAnime::SetAnimeNum(const BYTE num,
								const BYTE width_num,
								const BYTE height_num)
{
	m_AnimeNum = num;
	m_WidthNum = width_num;
	m_HeightNum= height_num;
}

// �A�j���[�V�������@�ݒ�
void CGraphicAnime::SetAnimetionType(const bool is_move_width)
{
	if( is_move_width )
	{
		m_Flg |= FLG_MOVE_WIDTH;

	}else{

		m_Flg &= ~FLG_MOVE_WIDTH;
	}
}

// ���[�v�ݒ�
void CGraphicAnime::SetLoop(const bool is_loop, const bool is_back)
{
	if( is_back )
	{
		m_Flg |= FLG_BACK_LOOP;
	
	}else if( is_loop ){

		m_Flg |= FLG_LOOP_ANIME;
	
	}else{

		m_Flg &= ~(FLG_BACK_LOOP | FLG_LOOP_ANIME);
	}
}

// ���Z�b�g
void CGraphicAnime::Reset()
{
	m_AnimeCnt = 0;
	m_SaveTime = timeGetTime();
}

// 1���������ǂ���
bool CGraphicAnime::IsOneRound()
{
	// ���[�v�A�j���[�V�����Ȃ�
	if( m_Flg & FLG_LOOP_ANIME )
	{
		if( m_AnimeCnt == 0 )
		{
			return true;
		}
	}

	// ���[�v�o�b�N�A�j���[�V�����Ȃ�
	if( m_Flg & FLG_BACK_LOOP )
	{
		if( m_Flg & FLG_NOW_BACK )
		{
			if( m_AnimeCnt == m_AnimeNum )
			{
				return true;
			}

		}else{

			if( m_AnimeCnt == 0 )
			{
				return true;
			}
		}
	}

	// 1��݂̂̃A�j���[�V����
	if( m_AnimeCnt == m_AnimeNum )
	{
		return true;
	}

	return false;
}