////////////////////////////////
// �C���N���[�h
////////////////////////////////
#include "DebugFont.h"

////////////////////////////////
// ����
////////////////////////////////
CDebugFont::CDebugFont( const LPDIRECT3DDEVICE9 lpdevice )
{
	D3DXCreateFont( 
		lpdevice,
		14,
		6,
		FW_BOLD,
		NULL,                   //�~�b�v�}�b�v���f���̐�
        FALSE,                  //�Α̂ɂ��邩�ǂ���
        SHIFTJIS_CHARSET,       //�����Z�b�g
        OUT_DEFAULT_PRECIS,
        PROOF_QUALITY,
        FIXED_PITCH | FF_MODERN,
        "tahoma",               //�t�H���g�̎��
        &m_lpFont);
}

// ������`��
void CDebugFont::Draw( int x, int y, LPSTR str )
{
	RECT rect = {x, y, 0, 0}; //�\���̈�

    //������T�C�Y���v�Z
    m_lpFont->DrawText(
        NULL,
        str,
        -1,             //�\���T�C�Y(-1�őS��)
        &rect,          //�\���͈�
        DT_CALCRECT,    //�\���͈͂ɒ���
        NULL);

    //���̃T�C�Y�Ń����_�����O
    m_lpFont->DrawText(
        NULL,
        str,
        -1,							//�\���T�C�Y(-1�őS��)
        &rect,						//�\���͈�
        DT_LEFT | DT_BOTTOM,		//���l�߂ŗ��[����
		D3DCOLOR_XRGB( 0, 0, 0 ) ); //�F
}

// �f�X�g���N�^
CDebugFont::~CDebugFont()
{
	SAFE_RELEASE( m_lpFont );
}