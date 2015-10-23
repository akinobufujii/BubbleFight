#pragma once

namespace Sequence
{
	class CMapping : public CBase
	{
	public:
		// コンストラクタ
		CMapping();

		// 本体
		bool RunProc();

		// デストラクタ
		~CMapping();

	private:
		// 更新
		void Update();

		// 描画
		void Draw();

		LPD3DXMESH			m_lpTeapot;
		LPDIRECT3DTEXTURE9	m_lpBackTex;
		CBoard*				m_lpBoard;
	};
}