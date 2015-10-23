#ifndef __AKI_CAMERA_H__
#define __AKI_CAMERA_H__

//////////////////////
// カメラクラス
//////////////////////
class CCamera
{
public:	// 外部公開

	//////////////////////
	// メソッド
	//////////////////////

	// コンストラクタ
	CCamera();
	CCamera( const D3DXVECTOR3* pos, const D3DXVECTOR3* look, const D3DXVECTOR3* up );

	// 視点位置を設定
	const D3DXMATRIX* SetCameraPos( const D3DXVECTOR3* pos );

	// 注視点の設定
	const D3DXMATRIX* SetCameraLook( const D3DXVECTOR3* look );

	// 上方向の設定
	const D3DXMATRIX* SetCameraUp( const D3DXVECTOR3* up );

	// カメラを設定(すべての内容)
	const D3DXMATRIX* SetCamera( const D3DXVECTOR3* pos, const D3DXVECTOR3* look, const D3DXVECTOR3* up );

	// ビルボードの為の行列獲得
	void GetBillboard( LPD3DXMATRIX out, const LPD3DXMATRIX pos ) const;

	// カメラ位置
	inline const D3DXVECTOR3* GetPos() const
	{
		return &m_vPos;
	} 

	// 注視点
	inline const D3DXVECTOR3* GetLook() const 
	{
		return &m_vlook;
	}

	// 上方向
	inline const D3DXVECTOR3* GetUp() const
	{
		return &m_vUp;
	}

	// 視点行列
	inline const D3DXMATRIX* GetMatrix() const
	{
		return &m_mMat;
	}

	// 今向いているベクトル
	D3DXVECTOR3 GetScreenVector() const;

	// デストラクタ
	~CCamera();

private:	// 非公開

	//////////////////////
	// フィールド
	//////////////////////
	CMatrix	m_mMat;	// カメラ行列
	CVector	m_vPos;	// カメラ位置
	CVector	m_vlook;// カメラ注視点
	CVector	m_vUp;	// カメラ上方向
};

#endif // __AKI_CAMERA_H__