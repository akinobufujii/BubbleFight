#ifndef __AKI_BIGSHOT_H__
#define __AKI_BIGSHOT_H__

namespace Shot
{
	//////////////////////////////////////////////////
	// �傫���e(�ђʒe)
	//////////////////////////////////////////////////
	class CBig : public CBase
	{
	public:	// �O�����J

		//////////////////////////////////////////////////
		// �萔
		//////////////////////////////////////////////////
		CONSTANT float	SPEED;			// �i�ޑ���
		CONSTANT DWORD	ERASE_TIME;		// �����鎞��
		CONSTANT LPSTR	SHOT_MESH_PASS;	// �e�̃��b�V���p�X

		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////

		// �R���X�g���N�^
		CBig();
		CBig( const D3DXMATRIX* mat );

		// �f�X�g���N�^
		~CBig();

		//////////////////////////////////////////////////
		//	�E�֐���	Update
		//	�E����		�X�V���s��
		//	�E����		�Ȃ�
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Update();

		//////////////////////////////////////////////////
		//	�E�֐���	Draw
		//	�E����		�����`����s��
		//	�E����		
		//		- lpdevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Draw( const LPDIRECT3DDEVICE9 lpdevice );

		//////////////////////////////////////////////////
		//	�E�֐���	GetPos
		//	�E����		���݂̈ʒu�����l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���݂̈ʒu��Ԃ�Ԃ�
		//////////////////////////////////////////////////
		inline D3DXVECTOR3 GetPos() const
		{
			return m_mMat.GetPos() - m_lpMesh->GetCenter();
		}

		//////////////////////////////////////////////////
		//	�E�֐���	GetRadius
		//	�E����		���b�V���̔��a���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	���b�V���̔��a��Ԃ�
		//////////////////////////////////////////////////
		inline float GetRadius() const
		{
			return m_lpMesh->GetRadius();
		}

		//////////////////////////////////////////////////
		//	�E�֐���	Dispatch
		//	�E����		�����Ă����I�u�W�F�N�g�Ɏ����𑗂����
		//	�E����		
		//		- object	�s�����N�����ė~�����I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Dispatch( IDoubleDispatch* object )
		{
			object->Action( *this );
		}

	private:	// ����J

		//////////////////////////////////////////////////
		// �t�B�[���h
		//////////////////////////////////////////////////
		DWORD	m_StartTime;	// ��肾���ꂽ����
		CXfile*	m_lpMesh;		// ���b�V���f�[�^
	};
}

#endif	// __AKI_BIGSHOT_H__