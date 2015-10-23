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

		CEffect		m_CausticShader;	// コースティックシェーダ
		CEffect		m_WaveShader;		// 波シェーダ
		CEffect		m_BlurShader;		// ぼかしシェーダ
		CXfile		m_Mesh;
		CXfile		m_Stage;
		CCamera		m_Camera;
		CProj		m_Proj;
		CBoard		m_Board;

		LPDIRECT3DSURFACE9	m_lpBackBuffer;
		LPDIRECT3DSURFACE9	m_lpZBuffer;

		CTexture	m_WaveTex;		// 波のテクスチャ
		CTexture	m_BumpTex;		// バンプテクスチャ
		CTexture	m_CausticTex;	// コースティックテクスチャ
		CTexture	m_BlurTex;		// ぼかしテクスチャ
	};
}