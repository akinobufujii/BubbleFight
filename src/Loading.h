#ifndef __AKI_LOADING_H__
#define __AKI_LOADING_H__

////////////////////////////////////
// ���[�f�B���O��ʕ`��N���X
// (�}���`�X���b�h�ɑ���p�N���X)
////////////////////////////////////
class CLoading
{
public:	// �O�����J

	//////////////////////////////
	// �萔
	//////////////////////////////
	CONSTANT LPSTR	TEXPASS_LOADFONT;	// ���[�f�B���O�t�H���g
	CONSTANT BYTE	FONT_ANIMEMAX;		// �ő�A�j����
	CONSTANT short	FONT_WIDTH;			// �t�H���g��
	CONSTANT short	FONT_HEIGHT;		// �t�H���g����
	CONSTANT BYTE	ANIME_TIME;			// �A�j���[�V�������鎞��

	//////////////////////////////
	// ���\�b�h
	//////////////////////////////

	// �R���X�g���N�^
	CLoading();

	// �}���`�X���b�h�Ŏ��s�֐�
	void operator()();

	// �I���ʍ�
	void End();

	// �f�X�g���N�^
	~CLoading();

private:	// ����J

	////////////////////////////////////
	// �t�B�[���h
	////////////////////////////////////
	bool		m_IsRun;	// �����𑖂邩�ǂ���
	CSprite		m_Sprite;	// �X�v���C�g�f�o�C�X
	BYTE		m_AnimeCnt;	// �A�j���J�E���^
	CTexture*	m_lpFontTex;// �t�H���g�e�N�X�`��
	DWORD		m_SaveTime;	// ���ԕۑ��p
};

#endif	// __AKI_LOADING_H__