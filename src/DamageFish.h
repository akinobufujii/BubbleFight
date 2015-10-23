#ifndef __AKI_DAMAGEFISH_H__
#define __AKI_DAMAGEFISH_H__

/////////////////////////////////
// �����߂��鋛
// �v���C���[��������Ώ�
/////////////////////////////////
class CDamageFish : virtual public CDamageObjectBase
{
public:	// �O�����J

	/////////////////////////////////
	// �萔
	/////////////////////////////////
	CONSTANT float	MOVESPEED;	// �ړ��̑���
	CONSTANT float	MOVEHEIGHT;	// �����܂�鍂��(�Œ�)
	CONSTANT float	TURNSPEED;	// ��鑬�x

	/////////////////////////////////
	// ���\�b�h
	/////////////////////////////////

	// �R���X�g���N�^
	CDamageFish();

	// �f�X�g���N�^
	~CDamageFish();

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
	//		- lpdevice	�f�o�C�X
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Draw( const LPDIRECT3DDEVICE9 lp_device, bool is_shader );

	//////////////////////////////////////////////////
	//	�E�֐���	Dispatch
	//	�E����		�����̏��𑗂��Ă���
	//				�I�u�W�F�N�g�ɑ������
	//	�E����		
	//		- object	���̃N���X�̏��𑗂����
	//					�I�u�W�F�N�g
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Dispatch( IDoubleDispatch* object )
	{
		object->Action( *this );
	}

	//////////////////////////////////////////////////
	//	�E�֐���	Action
	//	�E����		�G�ɑ΂��Ă̍s�����N����
	//				Dispatch����Ă΂��
	//	�E����		�s�����N�����Ώۂ̃I�u�W�F�N�g
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void Action( Enemy::CBase& object );

private:	// ����J
};

#endif // __AKI_DAMAGEFISH_H__