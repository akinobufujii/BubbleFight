#ifndef __AKI_SHOTMANAGER_H__
#define __AKI_SHOTMANAGER_H__

//////////////////////////////////////////////////
// �C���N���[�h(��O) 
//////////////////////////////////////////////////
#include "ShotBase.h"
#include "NormalShot.h"
#include "PowerShot.h"
#include "BigShot.h"
#include "BubbleRing.h"

namespace Shot
{
	//////////////////////////////////////////////////
	// �e�}�l�[�W���[
	//////////////////////////////////////////////////
	class CManager
	{
	public:	// �O�����J

		//////////////////////////////////////////////////
		// �^�錾
		//////////////////////////////////////////////////
		typedef std::list<CBase*> SHOTLIST;

		//////////////////////////////////////////////////
		// �萔
		//////////////////////////////////////////////////

		// ���
		enum KIND
		{
			NORMAL,
			POWER,
			BIG,
			BUBBLERING
		};

		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////

		// �R���X�g���N�^
		CManager(){}

		// �f�X�g���N�^
		~CManager();

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

		//////////////////////////////////////////////////
		//	�E�֐���	DrawEffect
		//	�E����		�G�t�F�N�g�`����s��
		//	�E����		
		//		- lpDevice	�f�o�C�X
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DrawEffect( const LPDIRECT3DDEVICE9 lpdevice );

		//////////////////////////////////////////////////
		//	�E�֐���	Add
		//	�E����		�e��ǉ�����
		//	�E����		
		//		- kind	�ǉ�����e�̎��
		//		- mat	�o���ʒu�̍s��
		//		- power	����(�o�u�������O�ȊO�͖���)
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void Add( KIND kind, const D3DXMATRIX* mat, const float power = 0.f );

		//////////////////////////////////////////////////
		//	�E�֐���	GetList
		//	�E����		�e���̓��������X�g���l��
		//	�E����		�Ȃ�
		//	�E�߂�l	�e���̓��������X�g��Ԃ�
		//////////////////////////////////////////////////
		SHOTLIST* GetList()
		{
			return &m_ShotList;
		}

	private:	// ����J

		//////////////////////////////////////////////////
		// �t�B�[���h
		//////////////////////////////////////////////////
		SHOTLIST	m_ShotList;		// �e���X�g

		//////////////////////////////////////////////////
		// �������[�v�[���t�B�[���h
		//////////////////////////////////////////////////
		boost::object_pool<CNormal>		m_mpNormal;		// �m�[�}���V���b�g�p�������[�v�[��
		boost::object_pool<CPower>		m_mpPower;		// �p���[�V���b�g�p�������[�v�[��
		boost::object_pool<CBig>		m_mpBig;		// �r�b�O�V���b�g�p�������[�v�[��
		boost::object_pool<CBubbleRing>	m_mpBubbleRing;	// �o�u�������O�p�������[�v�[��

		//////////////////////////////////////////////////
		// ���\�b�h
		//////////////////////////////////////////////////

		//////////////////////////////////////////////////
		//	�E�֐���	DeleteObjectByPool
		//	�E����		�������[�v�[������I�u�W�F�N�g���폜����
		//				dynamic_cast���s���Ȃ���폜����
		//	�E����		
		//		- object	�e�I�u�W�F�N�g
		//	�E�߂�l	�Ȃ�
		//////////////////////////////////////////////////
		void DeleteObjectByPool( CBase* object );
	};
}

#endif // __AKI_SHOTMANAGER_H__