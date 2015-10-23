#ifndef __AKI_MATRIX_H__
#define __AKI_MATRIX_H__

///////////////////////////
// 行列ラッピングクラス
///////////////////////////
class CMatrix :	public D3DXMATRIX
{
public:	// 外部公開

	///////////////////////////
	// メソッド
	///////////////////////////

	// コンストラクタ
	CMatrix();
	CMatrix( const D3DXMATRIX& m );
	CMatrix( float x, float y, float z );
	CMatrix( const D3DXVECTOR3& vec ); 

	// デストラクタ
	~CMatrix(){}

	// 移動
	void SetTrans( float x, float y, float z );
	void SetTrans( const D3DXVECTOR3* vec );

	// 移動(ローカル座標)
	void MoveLocal( float x, float y, float z );
	void MoveLocal( const D3DXVECTOR3* vec );

	// 回転
	void SetRotX( float x );
	void SetRotY( float y );
	void SetRotZ( float z );
	void SetRotAxis( const D3DXVECTOR3* vec, float rot );

	// 回転(ローカル座標)
	void RotateLocalX( float x );
	void RotateLocalY( float y );
	void RotateLocalZ( float z );
	void RotateLocalAxis( const D3DXVECTOR3* vec, float rot );

	// 拡大
	void SetScale( float x, float y, float z );
	void SetScale( float scale );

	// 拡大(ローカル座標)
	void ScaleLocal( float scale );
	void ScaleLocalX( float x );
	void ScaleLocalY( float y );
	void ScaleLocalZ( float z );
	
	// 逆行列
	CMatrix& GetInverse();

	// 座標獲得
	D3DXVECTOR3	GetPos() const;

	// 代入
	D3DXMATRIX& operator= ( const D3DXMATRIX& m );

	// それぞれの回転角度を計算して返す
	float GetRotX() const; 
	float GetRotY() const; 
	float GetRotZ() const; 
};

#endif // __AKI_MATRIX_H__