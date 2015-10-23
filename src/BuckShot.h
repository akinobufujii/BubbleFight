#ifndef __AKI_BUCKSHOT_H__
#define __AKI_BUCKSHOT_H__

namespace Shot
{
	//////////////////////////////////////////////////
	// �U�e�N���X(���ۂ͕��ʂ̒e�h��)
	//////////////////////////////////////////////////
	class CBuck : public CBase
	{
	public:	// �O�����J

		//////////////////////////////////////////////////
		// �萔
		//////////////////////////////////////////////////
		CONSTANT BYTE	BUCKMAX_RAND;		// �Ԃ��W��
		CONSTANT LPSTR	SHOT_MESH_PASS;		// �e�̃��b�V��
		CONSTANT float	SPEED;				// �e�̑���
		CONSTANT short	ERASE_TIME;			// ������Ƃ��̎���
		CONSTANT float	BUBBLE_SPEED_MAX;	// �A�̏�ւ̍ő�ړ�

		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////

		// �R���X�g���N�^
		CBuck( const D3DXMATRIX* mat );

		// �f�X�g���N�^
		~CBuck();

		//////////////////////////////////////////////////
		//	�E�֐���	Update
		//	�E����		�X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Update();

		//////////////////////////////////////////////////
		//	�E�֐���	Draw
		//	�E����		�`����s��
		//	�E����		
		//		- lpDevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Draw( const LPDIRECT3DDEVICE9 lpdevice );

		// ���𑗂����
		void Dispatch( IDoubleDispatch* object );

		// �G�ɑ΂���s��
		void Action( Enemy::CBase& object );

	private:	// ����J

		//////////////////////////////
		// �t�B�[���h
		//////////////////////////////
		DWORD	m_StartTime;		// ���ꂽ���̎���
		float	m_BubbleSpeed;		// �A�̑���
	};

}

#endif // __AKI_BUCKSHOT_H__