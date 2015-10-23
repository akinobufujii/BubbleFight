#ifndef __AKI_SPRITE_H__
#define __AKI_SPRITE_H__

/////////////////////////////////////
// �X�v���C�g�f�o�C�X���b�s���O�N���X
/////////////////////////////////////
class CSprite
{
public:	// �O�����J

	/////////////////////////////////////
	// ���\�b�h
	/////////////////////////////////////

	// �R���X�g���N�^
	CSprite();
	CSprite( const LPDIRECT3DDEVICE9 lpdevice );

	// �f�o�C�X�쐬
	bool Create( const LPDIRECT3DDEVICE9 lpdevice );

	// �f�o�C�X�l��
	inline const LPD3DXSPRITE GetDevice()
	{
		return m_lpSprite;
	}

	// �`��J�n
	inline const LPD3DXSPRITE Begin( int flg = D3DXSPRITE_ALPHABLEND )
	{
		m_lpSprite->Begin( flg );
		return m_lpSprite;
	} 

	// �`��I��
	inline void End()
	{
		m_lpSprite->End();
	}

	// �`��
	void Draw(
		const LPDIRECT3DTEXTURE9 lptex,
		float dest_x,
		float dest_y,
		float rot,
		int src_left,
		int src_top, 
		int src_right,
		int src_bottom );

	// ���łɂ���f�o�C�X�ŕ`��
	static void Draw(
		const LPD3DXSPRITE lpsprite,
		const LPDIRECT3DTEXTURE9 lptex,
		float dest_x,
		float dest_y,
		float rot,
		int src_left,
		int src_top,
		int src_right,
		int src_bottom,
		int alpha = 0xff );

	// �s�����x�ݒ�
	inline void SetAlpha( int alpha )
	{
		m_Alpha = alpha;
	}

	// �f�X�g���N�^
	~CSprite();

private:	// ����J

	/////////////////////////////////////
	// �t�B�[���h
	/////////////////////////////////////
	LPD3DXSPRITE	m_lpSprite;	// �X�v���C�g�f�o�C�X
	int				m_Alpha;	// �s�����x
};

#endif // __AKI_SPRITE_H__