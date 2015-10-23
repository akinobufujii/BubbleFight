#ifndef __AKI_GRAPHICANIME_H__
#define __AKI_GRAPHICANIME_H__

//////////////////////////////////////////////////
// �摜�A�j���[�V�����Ǘ��N���X
//////////////////////////////////////////////////
class CGraphicAnime
{
public:	// �O�����J

	//////////////////////////////////////////////////
	// �t���O�萔
	//////////////////////////////////////////////////
	CONSTANT BYTE	FLG_MOVE_WIDTH;	// ���ɃA�j���[�V����
	CONSTANT BYTE	FLG_LOOP_ANIME;	// ���[�v�A�j���[�V����
	CONSTANT BYTE	FLG_BACK_LOOP;	// �܂�Ԃ����[�v
	CONSTANT BYTE	FLG_NOW_BACK;	// �o�b�N�A�j���[�V������

	//////////////////////////////////////////////////
	// ���\�b�h
	//////////////////////////////////////////////////

	// �R���X�g���N�^
	CGraphicAnime();

	// �f�X�g���N�^
	~CGraphicAnime();

	//////////////////////////////////////////////////
	//	�E�֐���	Update
	//	�E����		�X�V���s��
	//	�E����		�Ȃ�
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Update();

	// �A�j���[�V�����ݒ�
	void SetAnimeSize( 
		const float width,
		const float height,
		const float anime_width,
		const float anime_height );

	// �A�j���[�V�������ݒ�
	void SetAnimeNum(
		const BYTE num,
		const BYTE width_num,
		const BYTE height_num );

	// �A�j���[�V�������@�ݒ�
	void SetAnimetionType( const bool is_move_width );
	
	// ���[�v�ݒ�
	void SetLoop( const bool is_loop = true, const bool is_back = false );

	// ���Ԑݒ�
	inline void SetTime( const DWORD time )
	{
		m_NextAnimeTime = time;
	}

	// �e�N�X�`���A�j���[�V�����̈ʒu�l��
	inline const D3DXVECTOR2* GetTexPos() const 
	{
		return m_TexPos;
	}

	// 1���������ǂ���
	bool IsOneRound();

	// ���Z�b�g
	void Reset();

private:	// ����J

	//////////////////////////////////////////////////
	// �t�B�[���h
	//////////////////////////////////////////////////
	DWORD		m_SaveTime;		// ���ԕۑ��p
	DWORD		m_NextAnimeTime;// ���̃A�j���[�V�����ɍs��
	BYTE		m_AnimeCnt;		// �A�j���J�E���^
	BYTE		m_WidthNum;		// ����
	BYTE		m_HeightNum;	// ������
	BYTE		m_AnimeNum;		// �A�j���[�V������
	float		m_WidthRate;	// �����[�g
	float		m_HeightRate;	// �������[�g
	BYTE		m_Flg;			// �t���O
	D3DXVECTOR2	m_TexPos[ 4 ];	// �e�N�X�`���ʒu

	//CBoard	m_Board;		// �|���S��
};

#endif	// __AKI_GRAPHICANIME_H__