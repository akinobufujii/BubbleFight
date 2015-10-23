#pragma once

namespace Sequence
{
	class CMapping : public CBase
	{
	public:
		// �R���X�g���N�^
		CMapping();

		// �{��
		bool RunProc();

		// �f�X�g���N�^
		~CMapping();

	private:
		// �X�V
		void Update();

		// �`��
		void Draw();

		LPD3DXMESH			m_lpTeapot;
		LPDIRECT3DTEXTURE9	m_lpBackTex;
		CBoard*				m_lpBoard;
	};
}