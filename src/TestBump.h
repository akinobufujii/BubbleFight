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

		CEffect		m_BumpShader;	// バンプマップシェーダー

		CCamera		m_Camera;		// カメラ
		CProj		m_Proj;			// 投影

		CTexture	m_Tex;			// テクスチャ
		CTexture	m_NormalTex;	// 法線テクスチャ

		CBoard		m_BumpBoard;	// 板
	};
}
