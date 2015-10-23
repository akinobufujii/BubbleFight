#ifndef __AKI_SHOTBASE_H__
#define __AKI_SHOTBASE_H__

namespace Shot
{
	//////////////////////////////////////////////////
	// �e�̊��N���X
	//////////////////////////////////////////////////
	class CBase : virtual public IDoubleDispatch
	{
	public:	// �O�����J

		//////////////////////////////////////////////////
		// �^�錾
		//////////////////////////////////////////////////
		typedef BYTE	FLGTYPE;	// �t���O�^

		//////////////////////////////////////////////////
		// �t���O�萔
		//////////////////////////////////////////////////
		CONSTANT FLGTYPE	FLG_ALIVE;		// �����Ă�
		CONSTANT FLGTYPE	FLG_ERASE;		// ����
		CONSTANT FLGTYPE	FLG_BREAK;		// ����
		CONSTANT FLGTYPE	FLG_BUBBLE_WRAP;// �A�ɕ�܂�Ă���
		CONSTANT FLGTYPE	FLG_FIN_HIT;	// �t�B���œ��Ă�ꂽ
		
		//////////////////////////////////////////////////
		// �萔
		//////////////////////////////////////////////////
		CONSTANT USHORT ERASE_TIME;			// ��������
		CONSTANT USHORT CREATE_BUBBLE_TIME;	// �A����鎞��
		CONSTANT USHORT BREAK_ANIME_NUM;	// �����A�j���R�}��
		CONSTANT USHORT BREAK_GRAPH_WIDTH;	// �����摜��
		CONSTANT USHORT BREAK_GRAPH_HEIGHT;	// �����摜����
		CONSTANT USHORT BREAK_GRAPH_SIZE;	// ��������1�R�}�T�C�Y
		CONSTANT LPSTR	BREAK_TEX_PASS;		// �����̃e�N�X�`���p�X
		CONSTANT LPSTR	SPRITE_TEX_PASS;	// �|�C���g�X�v���C�g�v�e�N�X�`���p�X

		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////

		// �R���X�g���N�^
		CBase();
		CBase( const D3DXMATRIX* mat );

		// ���z�f�X�g���N�^
		virtual ~CBase();

		//////////////////////////////////////////////////
		//	�E�֐���	Update
		//	�E����		�X�V���s��(�h����Œ�`)
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		virtual void Update() = 0;

		//////////////////////////////////////////////////
		//	�E�֐���	Draw
		//	�E����		�`����s��(�h����Œ�`)
		//	�E����		
		//		- lpdevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		virtual void Draw( const LPDIRECT3DDEVICE9 lpdevice ) = 0;

		//////////////////////////////////////////////////
		//	�E�֐���	DrawEffect
		//	�E����		�`����s��(�K�v�Ȃ�Δh����Œ�`)
		//	�E����		
		//		- lpdevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		virtual void DrawEffect( const LPDIRECT3DDEVICE9 lpdevice ){}

		//////////////////////////////////////////////////
		//	�E�֐���	BreakBubble
		//	�E����		�A��j�􂳂���
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		virtual void BreakBubble();

		//////////////////////////////////////////////////
		//	�E�֐���	SetMatrix
		//	�E����		�o��������ʂ̈ʒu��ݒ肷��
		//	�E����		
		//		- mat	�ݒ肷��s��
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		virtual void SetMatrix( const D3DXMATRIX& mat )
		{
			m_mMat = mat;
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetMatrix
		//	�E����		���݂̍s����l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�s����|�C���^�ŕԂ�
		//////////////////////////////////////////////////
		inline const D3DXMATRIX* GetMatrix() const
		{
			return &m_mMat;
		}
		
		//////////////////////////////////////////////////
		//	�E�֐���	GetFlg
		//	�E����		���݂̏�Ԃ��l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���݂̏�Ԃ�Ԃ�
		//////////////////////////////////////////////////
		inline FLGTYPE GetFlg() const 
		{
			return m_Flg; 
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetPos
		//	�E����		���݂̈ʒu�����l��(�h����Œ�`)
		//	�E����		�Ȃ�
		//	�E�߂�l	���݂̈ʒu��Ԃ�Ԃ�
		//////////////////////////////////////////////////
		virtual D3DXVECTOR3 GetPos() const = 0;

		//////////////////////////////////////////////////
		//	�E�֐���	GetRadius
		//	�E����		���b�V���̔��a���l��
		//				(���b�V�����Ⴄ�̂Ŕh����Œ�`)
		//	�E����		�Ȃ�
		//	�E�߂�l	���b�V���̔��a��Ԃ�
		//////////////////////////////////////////////////
		virtual float GetRadius() const = 0;

		//////////////////////////////////////////////////
		//	�E�֐���	GetPower
		//	�E����		�������l��
		//				(�K�v�Ȃ�Δh����Œ�`)
		//	�E����		�Ȃ�
		//	�E�߂�l	������Ԃ�
		//////////////////////////////////////////////////
		virtual inline float GetPower() const 
		{
			return NULL;
		}

	protected:	// �p����Œ�`����ł��낤�����o

		//////////////////////////////
		// �t�B�[���h
		//////////////////////////////
		CMatrix					m_mMat;			// �s��(���[�J�����W�O��)
		BYTE					m_Flg;			// �t���O
		CBubbleManager			m_Bubbles;		// �A�}�l�[�W���[
		BYTE					m_AnimeCnt;		// �A�j���J�E���^
		CBoard					m_Board;		// �|���S��

		//////////////////////////////
		// ���\�b�h
		//////////////////////////////

		//////////////////////////////////////////////////
		//	�E�֐���	UpdateBreak
		//	�E����		�����X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void UpdateBreak();

		//////////////////////////////////////////////////
		//	�E�֐���	DrawBreak
		//	�E����		�����`����s��
		//	�E����		
		//		- lpdevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawBreak( const LPDIRECT3DDEVICE9 lpdevice );
	};
}

#endif // __AKI_SHOTBASE_H__