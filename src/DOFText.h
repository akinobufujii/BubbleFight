#pragma once

namespace Sequence
{
	class CDOFText : public IBase
	{
	public:
		CDOFText(void);
		~CDOFText(void);
		bool RunProc();
		IBase* GetNext()
		{
			return NULL;
		}

	private:

		CCamera m_Camera;
		CProj	m_Proj;

		CBoard	m_Scene;

		CTexture	m_SceneTex;		// シーン描画用
		CTexture	m_MiniSizeTex;	// 縮小用(4分の1)
		CTexture	m_BlurTex;		// ぼかし用
		CTexture	m_DepthTex;		// 深度計算用

		CXfile	m_Mesh;
		CXfile	m_Sphere;

		CEffect m_BlurShader;		// ぼかし用シェーダー
		CEffect	m_DepthShader;		// 深度計算用シェーダー
		CEffect m_DOFShader;		// 被写界深度シェーダー
	
		LPDIRECT3DSURFACE9	m_lpBackBuffer;
		LPDIRECT3DSURFACE9	m_lpZBuffer;

		void Draw();

		// テクスチャに描画
		void DrawMesh( LPDIRECT3DDEVICE9 lpDevice );

		// シーンを板ポリゴンに描画
		void DrawSceneByPolygon( LPDIRECT3DDEVICE9 lpDevice );
	};
}