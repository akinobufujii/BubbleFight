#ifndef __AKI_PROJ_H__
#define __AKI_PROJ_H__

/////////////////////////////////
// 投影クラス
/////////////////////////////////
class CProj
{
public:	// 外部公開

	/////////////////////////////////
	// メソッド
	/////////////////////////////////

	// コンストラクタ
	CProj( float fovy = 45.f, float aspect = 4.f / 3.f, float zn = 1.f, float zf = 100.f );

	// ニアクリップ設定
	const D3DXMATRIX* SetNear( float zn );

	// ファークリップ設定
	const D3DXMATRIX* SetFar( float zf );

	// 画角設定
	const D3DXMATRIX* SetFovy( float fovy );

	// アスペクト比を設定
	const D3DXMATRIX* SetAspect( float aspect );

	// 全部設定
	const D3DXMATRIX* SetProj( float fovy, float aspect, float zn, float zf );

	// ゲッター
	// 画角
	inline float GetFovy() const
	{
		return m_Fovy;
	} 

	// ニアクリップ
	inline float GetNearClip() const
	{
		return m_Near;
	}

	// ファークリップ
	inline float GetFarClip() const 
	{
		return m_Far;
	}

	// アスペクト比
	inline float GetAspect() const
	{
		return m_Aspect;
	}

	// 投影行列
	inline const D3DXMATRIX* GetMatrix() const
	{
		return &m_mMat;
	}

	// デストラクタ
	~CProj();

private:	// 非公開

	/////////////////////////////////
	// フィールド
	/////////////////////////////////
	D3DXMATRIX	m_mMat;		// 投影行列

	float		m_Fovy;		// 画角
	float		m_Near;		// ニアクリップ
	float		m_Far;		// ファークリップ
	float		m_Aspect;	// アスペクト比
};

#endif // __AKI_PROJ_H__