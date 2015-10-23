#ifndef __BOARDPOLYGON_H__
#define __BOARDPOLYGON_H__

#include "using.h"

// �|���N���X
class CBoardPolygon
{
private:
	static const int VERTEX_FVF;	// ���_�t�H�[�}�b�g

	// �����\���̒�`
	struct Vertex
	{
		D3DXVECTOR3	Pos;	// ���W
		D3DCOLOR	Color;	// �F
		D3DXVECTOR2	Tex;	// �e�N�X�`�����W
	};

	LPDIRECT3DTEXTURE9		m_lpTex;// �e�N�X�`��
	LPDIRECT3DVERTEXBUFFER9	m_lpVB;	// ���_�o�b�t�@

public:
	// �R���X�g���N�^
	CBoardPolygon( const LPDIRECT3DDEVICE9 lpDevice );
	CBoardPolygon( const LPDIRECT3DDEVICE9 lpDevice, float w, float h );

	// �|���S���̕�������ݒ�
	void SetWH( float w, float h );
	
	// �e�N�X�`���ݒ�
	inline void SetTexture(const LPDIRECT3DTEXTURE9 tex ){ m_lpTex = tex; }

	// �`��
	void Draw( const LPDIRECT3DDEVICE9 lpDevice );

	// �f�X�g���N�^
	~CBoardPolygon();
};

#endif // __BOARDPOLYGON_H__