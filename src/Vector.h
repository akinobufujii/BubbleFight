#ifndef __AKI_VECTOR_H__
#define __AKI_VECTOR_H__

///////////////////////////////
// ベクトルラッピングクラス
///////////////////////////////
class CVector : public D3DXVECTOR3
{
public:	// 外部公開

	///////////////////////////////
	// メソッド
	///////////////////////////////

	// コンストラクタ
	CVector();
	CVector( float x, float y, float z );
	CVector( const D3DXVECTOR3& v );
	CVector( const D3DXMATRIX&	mat );

	// デストラクタ(特に無し)
	~CVector(){}

	// 内積をラジアンで返す
	float GetDotRadian( const D3DXVECTOR3* v);

	// 内積を角度で返す
	float GetDotDegree( const D3DXVECTOR3* v );

	// 外積を返す
	CVector& GetCross( const D3DXVECTOR3* v );

	// 長さを返す
	float GetLength();

	// 距離を返す
	float GetDistance( const D3DXVECTOR3* v );

	// 正規化
	CVector& GetNormalVector();

	// 座標設定
	void Set( float x, float y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// 演算子
	CVector& operator= ( const D3DXVECTOR3& v );
	CVector operator- ( const D3DXVECTOR3& v ) const ;
	CVector operator+ ( const D3DXVECTOR3& v ) const ;

	// 位置ベクトル
	CVector& GetTransCoord( const D3DXMATRIX* m );

	// 方向ベクトル
	CVector& GetTransNormal( const D3DXMATRIX* m );
};

#endif // __AKI_VECTOR_H__