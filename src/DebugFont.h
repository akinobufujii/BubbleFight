#ifndef __AKI_DEBUGFONT_H__
#define __AKI_DEBUGFONT_H__

////////////////////////////////
// �f�o�b�O�p�t�H���g
////////////////////////////////
class CDebugFont
{
public:	// �O�����J

	////////////////////////////////
	// ���\�b�h
	////////////////////////////////

	// �R���X�g���N�^
	CDebugFont( const LPDIRECT3DDEVICE9 lpdevice );

	// �����`��
	void Draw( int x, int y, LPSTR str );

	// �f�X�g���N�^
	~CDebugFont();

private:	// ����J

	////////////////////////////////
	// �t�B�[���h
	////////////////////////////////
	LPD3DXFONT	m_lpFont;	// �t�H���g�f�o�C�X
};

#endif // __AKI_DEBUGFONT_H__