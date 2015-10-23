#pragma once

namespace Sequence
{
	class CTestCaustic : public IBase
	{
	public:
		CTestCaustic(void);
		~CTestCaustic(void);

		IBase* GetNext()
		{
			return NULL;
		}

		bool RunProc();

	private:

		CEffect		m_CausticShader;	// �R�[�X�e�B�b�N�V�F�[�_
		CEffect		m_WaveShader;		// �g�V�F�[�_
		CEffect		m_BlurShader;		// �ڂ����V�F�[�_
		CXfile		m_Mesh;
		CXfile		m_Stage;
		CCamera		m_Camera;
		CProj		m_Proj;
		CBoard		m_Board;

		LPDIRECT3DSURFACE9	m_lpBackBuffer;
		LPDIRECT3DSURFACE9	m_lpZBuffer;

		CTexture	m_WaveTex;		// �g�̃e�N�X�`��
		CTexture	m_BumpTex;		// �o���v�e�N�X�`��
		CTexture	m_CausticTex;	// �R�[�X�e�B�b�N�e�N�X�`��
		CTexture	m_BlurTex;		// �ڂ����e�N�X�`��
	};
}