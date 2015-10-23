#ifndef __AKI_DOUBLEDISPATCH_H__
#define __AKI_DOUBLEDISPATCH_H__

/////////////////////////////////////
// �s���S�^�錾
/////////////////////////////////////
namespace Player
{
	class CDolphin;
}

namespace Enemy
{
	class CBase;	
}

namespace Shot
{
	class CBase;
}

namespace Boss
{
	class CBase;
}

class CBubble;
class CDamageObjectBase;

/////////////////////////////////////
// �����蔻��p�C���^�[�t�F�[�X
// �����蔻�肠�镨�S�ĂɌp��������
// Dispatch�͕K����`
// Action�͕K�v�ɉ����ăI�[�o�[���C�h����
/////////////////////////////////////
interface IDoubleDispatch
{
public:
	// �������
	virtual void Dispatch( IDoubleDispatch* object ) = 0;
	
	/////////////////////////////////////
	// �ȉ��s���֐�
	// �K�v�ɉ����ăI�[�o�[���C�h����
	/////////////////////////////////////
	// ���@�ɑ΂���
	virtual void Action( Player::CDolphin& object ){}

	// �G�ɑ΂���
	virtual void Action( Enemy::CBase& object ){}
	
	// �e�ɑ΂���
	virtual void Action( Shot::CBase& object ){}

	// �{�X�ɑ΂���
	virtual void Action( Boss::CBase& object ){}

	// �A�ɑ΂���
	virtual void Action( CBubble& object ){}

	// �����߂��鋛�ɑ΂���
	virtual void Action( CDamageObjectBase& object ){}
};

#endif // __AKI_DOUBLEDISPATCH_H__