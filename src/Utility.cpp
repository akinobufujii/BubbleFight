#include "StdAfx.h"
#include "Utility.h"

// グローバル変数
static CCamera	g_Camera;	// カメラ
static CProj	g_Proj;		// 投影

// 球の当たり判定
bool IsHitByCircle( const D3DXVECTOR3& v1, float r1, const D3DXVECTOR3& v2, float r2 )
{
	// 二点間の距離を求める
	D3DXVECTOR3 vLength = v1 - v2;
	
	/*if( pow( v2.x - v1.x, 2 ) + pow( v2.y - v1.y, 2 ) + pow( v2.z - v1.z, 2 ) <= pow( r1 + r2, 2 ) )
	{
		return true;
	}*/

	// 判定
	if( D3DXVec3Length( &vLength ) < r1 + r2 )
	{
		return true;
	}

	return false;
}

// 視錐台カリング
bool IsCulling( const CCamera* lp_camera, const CProj* lp_proj, const D3DXMATRIX* lp_mat )
{
	// 判断材料
	D3DXPLANE	Top, Bottom, Right, Left;
	CVector		Pos( *lp_mat );
	CCamera		Camera	= *lp_camera;
	CProj		Proj	= *lp_proj;

	// カメラ行列から座標変換
	Pos.GetTransCoord( Camera.GetMatrix() );

	// 平面計算
	CVector	V1, V2;

	// 上
	V1.x = -Proj.GetFarClip() * ( tan( Proj.GetFovy() / 2 ) * Proj.GetAspect() );
	V1.y =  Proj.GetFarClip() * tan( Proj.GetFovy() / 2 );
	V1.z =  Proj.GetFarClip();

	V2 = V1;
	V2.x *= -1;

	D3DXPlaneFromPoints( &Top, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// 下
	V1.x =  Proj.GetFarClip() * ( tan( Proj.GetFovy() / 2 ) * Proj.GetAspect() );
	V1.y = -Proj.GetFarClip() * tan( Proj.GetFovy() / 2 );
	V1.z =  Proj.GetFarClip();

	V2 = V1;
	V2.x *= -1;

	D3DXPlaneFromPoints( &Bottom, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );
	
	// 右
	V1.x = Proj.GetFarClip() * ( tan( Proj.GetFovy() / 2 ) * Proj.GetAspect() );
	V1.y = Proj.GetFarClip() * tan( Proj.GetFovy() / 2 );
	V1.z = Proj.GetFarClip();

	V2 = V1;
	V2.y *= -1;

	D3DXPlaneFromPoints( &Right, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// 左
	V1.x = Proj.GetFarClip() * ( tan( Proj.GetFovy() / 2 ) * Proj.GetAspect() );
	V1.y = Proj.GetFarClip() * tan( Proj.GetFovy() / 2 );
	V1.z = Proj.GetFarClip();

	V2 = V1;
	V2.y *= -1;

	D3DXPlaneFromPoints( &Left, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// 判定かけて描画を省略していく

	// ニアクリップ判定
	if( Pos.z < Proj.GetNearClip() )
	{
		return false;	// 描画しない
	}

	// ファークリップ
	if( Pos.z > Proj.GetFarClip() )
	{
		return false;	// 描画しない
	}

	// 上
	float Result = ( Pos.y * Top.b ) + ( Pos.z * Top.c );
	
	if( Result > 0.f )
	{
		return false;	// 描画しない
	}

	// 下
	Result = ( Pos.y * Bottom.b ) + ( Pos.z * Bottom.c );

	if( Result > 0.f )
	{
		return false;	// 描画しない
	}

	// 右
	Result = ( Pos.x * Right.a ) + ( Pos.z * Right.c );

	if( Result > 0.f )
	{
		return false;	// 描画しない
	}

	// 左
	Result = ( Pos.x * Left.a ) + ( Pos.z * Left.c );

	if( Result > 0.f )
	{
		return false;	// 描画しない
	}

	return true;	// 描画する
}

// すでに設定されたカメラと投影で視錐台かリング判定
bool IsCulling( const D3DXMATRIX* lp_mat )
{
	// 判断材料
	D3DXPLANE	Top, Bottom, Right, Left;
	CVector		Pos( *lp_mat );
	
	// カメラ行列から座標変換
	Pos.GetTransCoord( g_Camera.GetMatrix() );

	// 平面計算
	CVector	V1, V2;

	// 上
	V1.x = -g_Proj.GetFarClip() * ( tan( g_Proj.GetFovy() / 2 ) * g_Proj.GetAspect() );
	V1.y =  g_Proj.GetFarClip() * tan( g_Proj.GetFovy() / 2 );
	V1.z =  g_Proj.GetFarClip();

	V2 = V1;
	V2.x *= -1;

	D3DXPlaneFromPoints( &Top, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// 下
	V1.x =  g_Proj.GetFarClip() * ( tan( g_Proj.GetFovy() / 2 ) * g_Proj.GetAspect() );
	V1.y = -g_Proj.GetFarClip() * tan( g_Proj.GetFovy() / 2 );
	V1.z =  g_Proj.GetFarClip();

	V2 = V1;
	V2.x *= -1;

	D3DXPlaneFromPoints( &Bottom, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );
	
	// 右
	V1.x = g_Proj.GetFarClip() * ( tan( g_Proj.GetFovy() / 2 ) * g_Proj.GetAspect() );
	V1.y = g_Proj.GetFarClip() * tan( g_Proj.GetFovy() / 2 );
	V1.z = g_Proj.GetFarClip();

	V2 = V1;
	V2.y *= -1;

	D3DXPlaneFromPoints( &Right, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// 左
	V1.x = g_Proj.GetFarClip() * ( tan( g_Proj.GetFovy() / 2 ) * g_Proj.GetAspect() );
	V1.y = g_Proj.GetFarClip() * tan( g_Proj.GetFovy() / 2 );
	V1.z = g_Proj.GetFarClip();

	V2 = V1;
	V2.y *= -1;

	D3DXPlaneFromPoints( &Left, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// 判定かけて描画を省略していく

	// ニアクリップ判定
	if( Pos.z < g_Proj.GetNearClip() )
	{
		return false;	// 描画しない
	}

	// ファークリップ
	if( Pos.z > g_Proj.GetFarClip() )
	{
		return false;	// 描画しない
	}

	// 上
	float Result = ( Pos.y * Top.b ) + ( Pos.z * Top.c );
	
	if( Result > 0.f )
	{
		return false;	// 描画しない
	}

	// 下
	Result = ( Pos.y * Bottom.b ) + ( Pos.z * Bottom.c );

	if( Result > 0.f )
	{
		return false;	// 描画しない
	}

	// 右
	Result = ( Pos.x * Right.a ) + ( Pos.z * Right.c );

	if( Result > 0.f )
	{
		return false;	// 描画しない
	}

	// 左
	Result = ( Pos.x * Left.a ) + ( Pos.z * Left.c );

	if( Result > 0.f )
	{
		return false;	// 描画しない
	}

	return true;	// 描画する
}

// 判定用カメラ設定
void SetCullingCamera( const CCamera* lp_camera )
{
	g_Camera = *lp_camera;
}

// 判定用投影設定
void SetCullingProj( const CProj* lp_proj )
{
	g_Proj = *lp_proj;
}

// メッシュに対してのレイ判定
// (当たっていなければ負の値を返す)
float IntersectByMesh( const D3DXMATRIX* lp_mpos, const D3DXVECTOR3* lay_dir, const D3DXMATRIX* lp_mesh_local, const LPD3DXMESH mesh, LPDWORD lp_polygon_no )
{
	D3DXMATRIX mInv = *lp_mesh_local;	// 逆行列用
	D3DXMATRIX mPos = *lp_mpos;			// レイ発射行列

	// ローカル前提行列を逆行列にする
	D3DXMatrixInverse( &mInv, NULL, &mInv );

	D3DXVECTOR3 LocalPos, LayDir;	// ローカル座標、レイ発射方向

	// ワールド座標抽出
	D3DXVec3TransformCoord( &LocalPos, &D3DXVECTOR3( 0, 0, 0 ), &mPos );

	// ローカル座標でレイ発射位置確定
	D3DXVec3TransformCoord( &LocalPos, &LocalPos, &mInv );

	// レイ発射方向確定
	D3DXVec3TransformNormal( &LayDir, lay_dir, &mInv );

	// 判定
	BOOL	IsHit	= 0;	// 結果(使わないが関数実行のため)
	float	Len		= -1.f;	// 距離

	D3DXIntersect(
		mesh,			// メッシュデータ
		&LocalPos,		// 発射位置
		&LayDir,		// 発射方向
		&IsHit,			// 判定結果格納変数
		lp_polygon_no,	// 当たったポリゴン
		NULL,
		NULL,
		&Len,		// 距離格納用
		NULL,
		NULL);

	return Len;
}

// 回転アニメーション計算関数
// 回転行列を返す
// start・・・開始行列
// end・・・終点行列
// anime・・・アニメーションカウンタ(0.f～1.f)
// rot_only・・・回転行列のみを返す
D3DXMATRIX RotationAnimeMatarix( const LPD3DXMATRIX start, const LPD3DXMATRIX end, float anime, bool rot_only )
{
	D3DXQUATERNION	qStart, qEnd, qNow;	// 開始、終点、今のクォータニオン
	D3DXMATRIX mRot;					// 回転行列

	// 渡された情報からクォータニオン作成
	D3DXQuaternionRotationMatrix( &qStart, start );
	D3DXQuaternionRotationMatrix( &qEnd, end );

	// 中間状態のクォータニオン作成
	D3DXQuaternionSlerp( &qNow, &qStart, &qEnd, anime );

	// 行列に直す
	D3DXMatrixRotationQuaternion( &mRot, &qNow );

	// 回転行列のみならここで値を返す
	if( rot_only )
	{
		return mRot;
	}

	D3DXVECTOR3 Pos;	// 位置格納用

	// 位置情報抽出
	D3DXVec3Lerp( &Pos, &CVector( *start ), &CVector( *end ), anime );

	return mRot * CMatrix( Pos );
}

// 追尾する回転軸と回転角度を求める
float GetAxisAndRot(D3DXVECTOR3*		out_axis,
					const D3DXVECTOR3&	target,
					const D3DXMATRIX&	mat,
					const D3DXVECTOR3&	target_front )
{
	// 逆行列を求める
	D3DXMATRIX mInv;
	D3DXMatrixInverse( &mInv, NULL, &mat );

	// 行列から見た位置を求める
	D3DXVECTOR3 TargetPos;
	D3DXVec3TransformCoord( &TargetPos, &target, &mInv );

	// 正規化
	D3DXVec3Normalize( &TargetPos, &TargetPos );

	// 回転軸を求める
	D3DXVec3Cross( out_axis, &target_front, &TargetPos );

	// 内積を求める
	float Dot = D3DXVec3Dot( &target_front, &TargetPos );

	if( Dot >  1.f )
	{
		Dot =  1.f;
	
	}else if( Dot < -1.f ){

		Dot = -1.f;
	}

	// 角度を返す
	return D3DXToDegree( acos( Dot ) );
}

// エラー関数(エラーが起こればtrueを返す)
bool ErrorMessage( HRESULT hr, LPCSTR msg )
{
	if( FAILED( hr ) )
	{
		MessageBox( NULL, msg, "ERROR", MB_OK );
		return true;
	}

	return false;
}

// 矩形当たり判定
bool IsHitbyCross(int x1, int y1, int w1, int h1,
				  int x2, int y2, int w2, int h2)
{
	if( x1 < x2 + w2 && x1 + w1 > x2 &&
		y1 < y2 + h2 && y1 + h1 > y2 )
	{
		return true;

	}else{

		return false;
	}
}

// ライト位置を計算する
D3DXVECTOR4 GetLightPos(LPDIRECT3DDEVICE9	lp_device, 
						const D3DXMATRIX&	mat,
						DWORD				light_index)
{
	// ライト情報獲得
	D3DLIGHT9 Light;
	lp_device->GetLight( light_index, &Light );
	
	// 行列に対するライト位置計算
	D3DXVECTOR4 LightPos( Light.Direction, 1.f );
	D3DXMATRIX	mInv;

	D3DXMatrixInverse( &mInv, NULL, &mat );
	D3DXVec4Transform( &LightPos, &LightPos, &mInv );
	
	return LightPos;
}

// 視点位置を求める
D3DXVECTOR4 GetViewPos(LPDIRECT3DDEVICE9 lp_device, 
					   const D3DXMATRIX& mat )
{
	// 視点行列を獲得
	D3DXMATRIX mView, mInv;

	lp_device->GetTransform( D3DTS_VIEW, &mView );

	// 視点位置を求める
	D3DXVECTOR3 Pos = *g_Camera.GetPos();
	D3DXVECTOR4 ViewPos( Pos.x, Pos.y, Pos.z , 1 );

	D3DXMatrixInverse( &mInv, NULL, &(mat * mView) );
	D3DXVec4Transform( &ViewPos, &ViewPos, &mInv );

	return ViewPos;
}

// スクリーン座標を求める
D3DXVECTOR2 GetTexPos(LPDIRECT3DDEVICE9 lpdevice,
					  const D3DXVECTOR3& pos )
{
	D3DXMATRIX mTemp1, mTemp2;

	// 固定パイプラインから視点と投影行列を取得
	lpdevice->GetTransform( D3DTS_VIEW, &mTemp1 );
	lpdevice->GetTransform( D3DTS_PROJECTION, &mTemp2 );

	// 座標変換する
	D3DXVECTOR3 Pos;
	D3DXVec3TransformCoord( &Pos, &pos, &(mTemp1 * mTemp2) );

	// 範囲を0～1に変換する
	D3DXMatrixScaling( &mTemp1, 0.5f, -0.5, 1.f );
	D3DXMatrixTranslation( &mTemp2, 0.5f, 0.5f, 0 );

	D3DXVec3TransformCoord( &Pos, &Pos, &(mTemp1 * mTemp2) );

	// 求めたx,yがテクスチャ座標
	return D3DXVECTOR2( Pos.x, Pos.y );
}

D3DXVECTOR2 GetTexPos(	const D3DXMATRIX&	view,
						const D3DXMATRIX&	proj,
						const D3DXVECTOR3&	pos )
{
	D3DXMATRIX mTemp1, mTemp2;

	// 座標変換する
	D3DXVECTOR3 Pos;
	D3DXVec3TransformCoord( &Pos, &pos, &(view * proj) );

	// 範囲を0～1に変換する
	D3DXMatrixScaling( &mTemp1, 0.5f, -0.5, 1.f );
	D3DXMatrixTranslation( &mTemp2, 0.5f, 0.5f, 0 );

	D3DXVec3TransformCoord( &Pos, &Pos, &(mTemp1 * mTemp2) );

	// 求めたx,yがテクスチャ座標
	return D3DXVECTOR2( Pos.x, Pos.y );
}