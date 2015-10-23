#ifndef __AKI_BOARD_H__
#define __AKI_BOARD_H__

/////////////////////////////////
// �|���S���N���X
/////////////////////////////////
class CBoard
{
public:	// �O�����J

	/////////////////////////////////
	// �萔
	/////////////////////////////////
	CONSTANT int FVF_VERTEX;	// ���_�t�H�[�}�b�g

	//////////////////////////////
	// ���ʃ��\�b�h
	//////////////////////////////

	// �V�F�[�_�[�f�o�C�X�ݒ�(����)
	static void SetShaderDevice( CEffect* lp_effect, D3DXHANDLE world )
	{
		m_lpEffect	= lp_effect;
		m_lphWorld	= world;
	}

	// �V�F�[�_�[�ŕ`�悷�邩�ǂ���
	static void SetDrawShader( const bool is_draw )
	{
		m_IsShaderDraw = is_draw;
	}

	/////////////////////////////////
	// ���\�b�h
	/////////////////////////////////

	// �R���X�g���N�^
	CBoard();
	CBoard( float size, bool is_wall = true );
	CBoard( float w, float h );
	CBoard( float left, float top, float right, float bottom );
	CBoard(	const D3DXVECTOR3& v1, 
		const D3DXVECTOR3& v2,
		const D3DXVECTOR3& v3,
		const D3DXVECTOR3& v4 );


	// ���_�ݒ�
	// �����`
	void SetVertex( float size, bool is_wall = true );
	
	// �����`	
	void SetVertex( float w, float h );

	// ��`
	void SetVertex( float left, float top, float right, float bottom );

	// ���S�w��
	void SetVertex(
		const D3DXVECTOR3& v1, 
		const D3DXVECTOR3& v2,
		const D3DXVECTOR3& v3,
		const D3DXVECTOR3& v4 );

	// �F�ݒ�
	void SetColor( const D3DXCOLOR& color );
	void SetColor( float r, float g, float b );

	// �ׂ��F�����߂�
	void SetVertexColors(
		const D3DCOLOR v1,
		const D3DCOLOR v2,
		const D3DCOLOR v3,
		const D3DCOLOR v4 );
	
	// �P�F�ݒ�
	void SetRed		( float r );
	void SetGreen	( float g );
	void SetBlue	( float b );
	void SetAlpha	( float a );

	// �e�N�X�`�����W�w��
	void SetTexturePos( const D3DXVECTOR2* pos1, const D3DXVECTOR2* pos2, const D3DXVECTOR2* pos3, const D3DXVECTOR2* pos4 ); 

	// �`��
	bool Draw( const LPDIRECT3DDEVICE9 lpDevice ) const;
	
	// �f�X�g���N�^
	~CBoard();

private:	// ����J
	
	/////////////////////////////////
	// �\���̐錾
	/////////////////////////////////
	struct Vertex
	{
		D3DXVECTOR3	Pos;	// ���W
		D3DCOLOR	Color;	// �F
		D3DXVECTOR2	Tex;	// �e�N�X�`�����W
	};

	/////////////////////////////////
	// ���ʃt�B�[���h
	/////////////////////////////////
	static bool			m_IsShaderDraw;	// �V�F�[�_�[�ŕ`�悷�邩�ǂ���
	static CEffect*		m_lpEffect;		// �V�F�[�_�[
	static D3DXHANDLE	m_lphWorld;		// �s���n���n���h��

	/////////////////////////////////
	// �t�B�[���h
	/////////////////////////////////
	LPDIRECT3DTEXTURE9	m_lpTex;		// �e�N�X�`��
	Vertex				m_Vertex[ 4 ];	// ���_���
	D3DXCOLOR			m_Color;		// �F���
};

#endif // __AKI_BOARD_H__