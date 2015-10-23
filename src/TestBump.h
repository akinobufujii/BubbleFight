#pragma once

namespace Sequence 
{
	class CTestBump : public IBase
	{
	public:
		CTestBump(void);
		~CTestBump(void);

		bool RunProc();

		IBase* GetNext()
		{
			return NULL;
		}

	private:

		CEffect		m_BumpShader;	// �o���v�}�b�v�V�F�[�_�[

		CCamera		m_Camera;		// �J����
		CProj		m_Proj;			// ���e

		CTexture	m_Tex;			// �e�N�X�`��
		CTexture	m_NormalTex;	// �@���e�N�X�`��

		CBoard		m_BumpBoard;	// ��
	};
}
