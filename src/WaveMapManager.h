#ifndef __WAVEMAPMANAGER_H__
#define __WAVEMAPMANAGER_H__

//////////////////////////////////////////////////
// �g�}�b�v�}�l�[�W���[
// �g�}�b�v�̍X�V�{�@���}�b�v�쐬���s��
// �����Ń����_�[�^�[�Q�b�g�����
//////////////////////////////////////////////////
class CWaveMapManager : public boost::noncopyable
{
public: // �O�����J

	//////////////////////////////////////////////////
	// ���\�b�h
	//////////////////////////////////////////////////

	// �R���X�g���N�^
	CWaveMapManager();

	// �f�X�g���N�^
	~CWaveMapManager();

	//////////////////////////////////////////////////
	//	�E�֐���	Create
	//	�E����		�g�}�b�v�p�̃����_�[�^�[�Q�b�g�����
	//				�����_�[�^�[�Q�b�g��3�������
	//	�E����		
	//		- lpdevice	�f�o�C�X
	//		- size		�}�b�v�̃T�C�Y
	//	�E�߂�l	�����Ȃ�true���s�Ȃ�false��Ԃ�
	//////////////////////////////////////////////////
	bool Create( LPDIRECT3DDEVICE9 lpdevice, UINT size );

	//////////////////////////////////////////////////
	//	�E�֐���	Update
	//	�E����		�g�}�b�v�X�V���s��
	//				�K���`�撆�ɌĂяo��
	//	�E����		
	//		- lpdevice	�f�o�C�X
	//	�E�߂�l	�X�V�����@���}�b�v��Ԃ�
	//////////////////////////////////////////////////
	const LPDIRECT3DTEXTURE9 Update( LPDIRECT3DDEVICE9 lpdevice );

	//////////////////////////////////////////////////
	//	�E�֐���	GetMap
	//	�E����		�����I�ɖ@���}�b�v���l��(�X�V�͂��Ȃ�)
	//	�E����		�Ȃ�
	//	�E�߂�l	�@���}�b�v��Ԃ�
	//////////////////////////////////////////////////
	inline const LPDIRECT3DTEXTURE9 GetMap() const
	{
		return m_NormalMap;
	}

	//////////////////////////////////////////////////
	//	�E�֐���	operator const LPDIRECT3DTEXTURE9
	//	�E����		�Öقɖ@���}�b�v���l��(�X�V�͂��Ȃ�)
	//	�E����		�Ȃ�
	//	�E�߂�l	�@���}�b�v��Ԃ�
	//////////////////////////////////////////////////
	inline operator const LPDIRECT3DTEXTURE9() const
	{
		return GetMap();
	}

private:	// ����J

	//////////////////////////////////////////////////
	// �t�B�[���h
	//////////////////////////////////////////////////
	CTexture	m_WaveMap[ 2 ];	// �g�}�b�v�X�V�p�����_�[�^�[�Q�b�g
	CTexture	m_NormalMap;	// �@���}�b�v�p�����_�[�^�[�Q�b�g
	CEffect*	m_lpWaveShader;	// �g�}�b�v�{�@���}�b�v�p�V�F�[�_
	CBoard		m_Board;		// �`��p�̔|���S��
	bool		m_IsFirst;		// ��ԏ��߂̕`�悩�ǂ���

	//////////////////////////////////////////////////
	// ���\�b�h
	//////////////////////////////////////////////////

	//////////////////////////////////////////////////
	//	�E�֐���	UpdateWave
	//	�E����		�g�}�b�v�̍X�V
	//	�E����		
	//		- lpdevice	�f�o�C�X
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void UpdateWaveMap( LPDIRECT3DDEVICE9 lpdevice );

	//////////////////////////////////////////////////
	//	�E�֐���	UpdateNormalMap
	//	�E����		�@���}�b�v�̍X�V
	//	�E����		
	//		- lpdevice	�f�o�C�X
	//	�E�߂�l	�Ȃ�
	//////////////////////////////////////////////////
	void UpdateNormalMap( LPDIRECT3DDEVICE9 lpdevice );
};

#endif	// __WAVEMAPMANAGER_H__