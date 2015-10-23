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

		CTexture	m_SceneTex;		// �V�[���`��p
		CTexture	m_MiniSizeTex;	// �k���p(4����1)
		CTexture	m_BlurTex;		// �ڂ����p
		CTexture	m_DepthTex;		// �[�x�v�Z�p

		CXfile	m_Mesh;
		CXfile	m_Sphere;

		CEffect m_BlurShader;		// �ڂ����p�V�F�[�_�[
		CEffect	m_DepthShader;		// �[�x�v�Z�p�V�F�[�_�[
		CEffect m_DOFShader;		// ��ʊE�[�x�V�F�[�_�[
	
		LPDIRECT3DSURFACE9	m_lpBackBuffer;
		LPDIRECT3DSURFACE9	m_lpZBuffer;

		void Draw();

		// �e�N�X�`���ɕ`��
		void DrawMesh( LPDIRECT3DDEVICE9 lpDevice );

		// �V�[����|���S���ɕ`��
		void DrawSceneByPolygon( LPDIRECT3DDEVICE9 lpDevice );
	};
}