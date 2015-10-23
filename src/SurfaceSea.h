#ifndef __AKI_SURFACESEA_H__
#define __AKI_SURFACESEA_H__

//////////////////////////////////////////////////
// �s���S�^�錾
//////////////////////////////////////////////////
class CWaveMapManager;

//////////////////////////////////////////////////
// �C�ʃN���X
// �s�N�Z���t�H�O�{�o���v�}�b�s���O
//////////////////////////////////////////////////
class CSurfaceSea
{
public:	// �O�����J

	//////////////////////////////////////////////////
	// �^�錾
	//////////////////////////////////////////////////
	typedef BYTE	FLGTYPE;

	//////////////////////////////////////////////////
	// �t���O�萔
	//////////////////////////////////////////////////
	CONSTANT FLGTYPE	FLG_HEIGHTPLUS;	// �������Z���Ă��邩

	//////////////////////////////////////////////////
	// ���\�b�h
	//////////////////////////////////////////////////

	// �R���X�g���N�^
	CSurfaceSea();

	// �f�X�g���N�^
	~CSurfaceSea();

	//////////////////////////////////////////////////
	//	�E�֐���	Update
	//	�E����		�X�V���s��
	//				��Ƀv���C���[�̈ʒu�ɍ����Œ��
	//				�ʒu�X�V����
	//	�E����		
	//		- player_pos	�v���C���[�̈ʒu	
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Update( const D3DXVECTOR3& player_pos );

	//////////////////////////////////////////////////
	//	�E�֐���	Draw
	//	�E����		�Œ�p�C�v���C���ŕ`����s��
	//	�E����		
	//		- lp_device	�f�o�C�X	
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Draw( const LPDIRECT3DDEVICE9 lp_device );

	//////////////////////////////////////////////////
	//	�E�֐���	DrawShader
	//	�E����		�v���O���}�u���V�F�[�_�ŕ`����s��
	//	�E����		
	//		- lp_device	�f�o�C�X	
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void DrawShader( const LPDIRECT3DDEVICE9 lp_device );

	//////////////////////////////////////////////////
	//	�E�֐���	SetHeight
	//	�E����		�C�̍�����ݒ肷��
	//	�E����		
	//		- height	����	
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetHeight( const float height )
	{
		m_Height = height;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetSize
	//	�E����		�C�̑傫����ݒ肷��
	//	�E����		
	//		- height	����	
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetSize( const int size )
	{
		m_SeaSize = size;
		m_SeaBoard.SetVertex( m_SeaSize, false );
	}

	//////////////////////////////////////////////////
	//	�E�֐���	SetColor
	//	�E����		�C�̐F��ݒ肷��(�e0�`255�͈̔�)
	//	�E����		
	//		- r	�Ԑ���
	//		- g	�ΐ���
	//		- b	����
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	inline void SetColor( BYTE r, BYTE g, BYTE b )
	{
		float Rate = 1.f / 0xff;

		D3DXVECTOR3 Color( Rate * r, Rate * g, Rate * b );

		m_lpBumpShader->SetValue( "g_FogColor", &Color, sizeof(D3DXVECTOR3) );
	}

private:	// ����J

	////////////////////////////////
	// �t�B�[���h
	////////////////////////////////
	CBoard				m_SeaBoard;		// �|���S��
	CVector				m_Pos;			// �ʒu�i�[
	CEffect*			m_lpBumpShader;	// �o���v�}�b�s���O�p�V�F�[�_
	CWaveMapManager*	m_lpWaveMap;	// �g�}�b�v
	BYTE				m_Flg;			// �t���O
	float				m_Height;		// ����
	float				m_Wave;			// �g
	float				m_SeaSize;		// �C�̑傫��
	int					m_Speed;		// �X�N���[���̑���
	int					m_ScrollCnt;	// �X�N���[���J�E���^
};

#endif	// __AKI_SURFACESEA_H__