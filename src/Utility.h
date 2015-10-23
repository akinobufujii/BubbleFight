#ifndef __AKI_UTILITY_H__
#define __AKI_UTILITY_H__

//////////////////////////////////////////////////
//	・関数名	IsHitByCircle
//	・説明		球の当たり判定を行う
//	・引数	
//		- v1	1つめの位置
//		- r1	1つめの半径
//		- v2	2つめの位置
//		- r2	2つめの半径
//	・戻り値	衝突していればtrue、
//				そうでなければfalseを返す
//////////////////////////////////////////////////
bool IsHitByCircle(const D3DXVECTOR3& v1,
				   float r1,
				   const D3DXVECTOR3& v2,
				   float r2 );

//////////////////////////////////////////////////
//	・関数名	IsCulling
//	・説明		視錐台カリング判定を行う
//	・引数	
//		- lp_camera	判定を行うカメラ
//		- lp_proj	判定を行う投影
//		- lp_mat	判定を行う行列
//	・戻り値	視錐台の中にいればtrue、
//				そうでなければfalseを返す
//////////////////////////////////////////////////
bool IsCulling(const CCamera* lp_camera,
			   const CProj* lp_proj,
			   const D3DXMATRIX* lp_mat );

//////////////////////////////////////////////////
//	・関数名	IsCulling
//	・説明		設定してあるカメラと投影で
//				視錐台カリング判定を行う
//	・引数	
//		- lp_mat	判定を行う行列
//	・戻り値	視錐台の中にいればtrue、
//				そうでなければfalseを返す
//////////////////////////////////////////////////
bool IsCulling( const D3DXMATRIX* lp_mat );

//////////////////////////////////////////////////
//	・関数名	SetCullingCamera
//	・説明		IsCullingで使用するカメラを設定する
//	・引数	
//		- lp_camera	判定を行うカメラ
//	・戻り値	なし
//////////////////////////////////////////////////
void SetCullingCamera( const CCamera* lp_camera );

//////////////////////////////////////////////////
//	・関数名	SetCullingProj
//	・説明		IsCullingで使用する投影を設定する
//	・引数	
//		- lp_proj	判定を行う投影
//	・戻り値	なし
//////////////////////////////////////////////////
void SetCullingProj( const CProj* lp_proj );

//////////////////////////////////////////////////
//	・関数名	IntersectByMesh
//	・説明		メッシュに対してのレイ判定を行う
//	・引数	
//		- lp_mpos			レイ発射位置情報の入った行列
//		- lay_dir			レイ発射方向
//		- lp_mesh_local_mat	メッシュのローカル行列
//		- lp_mesh			メッシュ情報
//		- lp_polygon_no		衝突した場合、
//							ポリゴンのインデックス番号を返す
//	・戻り値	衝突した場合、距離が
//				衝突しなければ、-1を返す
//////////////////////////////////////////////////
float IntersectByMesh(const D3DXMATRIX* lp_mpos,
					  const D3DXVECTOR3* lay_dir,
					  const D3DXMATRIX* lp_mesh_local_mat,
					  const LPD3DXMESH lp_mesh,
					  LPDWORD lp_polygon_no = NULL );

//////////////////////////////////////////////////
//	・関数名	RotationAnimeMatarix
//	・説明		最初の行列から最後の行列の
//				アニメーションカウンタ量の行列を計算する
//	・引数	
//		- lp_start	アニメーション開始時の行列
//		- lp_end	アニメーション終了時の行列
//		- anime		アニメーションカウンタ(0.0～1.0まで)
//		- rot_only	回転行列のみを返すかどうか
//	・戻り値	rot_onlyがtrueならば回転行列のみを、
//				rot_onlyがfalseならばカウンタに合わせた
//				行列を返す
//////////////////////////////////////////////////
D3DXMATRIX RotationAnimeMatarix(const LPD3DXMATRIX lp_start,
								const LPD3DXMATRIX lp_end,
								float anime,
								bool rot_only = false );

//////////////////////////////////////////////////
//	・関数名	GetAxisAndRot
//	・説明		目標に対しての回転軸と回転量を返す
//	・引数	
//		- out_axis		計算した回転軸を返す
//		- target		目標位置
//		- mat			目標に行くための行列
//		- target_front	元々向いていた位置
//	・戻り値	回転量を返す
//////////////////////////////////////////////////
float GetAxisAndRot(D3DXVECTOR3*		out_axis,
					const D3DXVECTOR3&	target,
					const D3DXMATRIX&	mat,
					const D3DXVECTOR3&	target_front = D3DXVECTOR3( 0, 0, 1 ) );

//////////////////////////////////////////////////
//	・関数名	ErrorMessage
//	・説明		引数の値がエラーであれば、
//				メッセージボックスを表示する
//	・引数	
//		- hr	関数実行結果の値を渡す
//		- msg	メッセージボックスに表示する
//				文字列
//	・戻り値	エラーであればtrue
//				そうでなければfalse
//////////////////////////////////////////////////
bool ErrorMessage( HRESULT hr, LPCSTR msg );

//////////////////////////////////////////////////
//	・関数名	IsHitbyCross
//	・説明		メッセージボックスを表示する
//	・引数	
//		- x1	x座標
//		- y1	y座標
//		- w1	幅
//		- h1	高さ
//		- x2	x座標
//		- y2	y座標
//		- w2	幅
//		- h2	高さ
//	・戻り値	当たってればtrue,そうでなければfalseを返す
//////////////////////////////////////////////////
bool IsHitbyCross(int x1, int y1, int w1, int h1,
				  int x2, int y2, int w2, int h2);

//////////////////////////////////////////////////
//	・関数名	GetMousePos
//	・説明		このWindowのマウス位置を取ってくる
//	・引数		なし
//	・戻り値	マウス位置をPOINT型で返す
//////////////////////////////////////////////////
inline POINT GetMousePos()
{
	POINT CursorPos;
	GetCursorPos( &CursorPos );

	ScreenToClient(	FindWindow( CLASSNAME, CLASSNAME ), &CursorPos );

	return CursorPos;
}

//////////////////////////////////////////////////
//	・関数名	GetLightPos
//	・説明		固定パイプラインからライト情報を取り出し
//				行列に対するライト位置の計算を行う
//	・引数	
//		- lp_device		デバイス
//		- mat			ライト位置を求めるための行列
//		- light_index	参照するライトインデックス
//						何も指定しなければ0番目が参照される
//	・戻り値	ライト位置を返す
//////////////////////////////////////////////////
D3DXVECTOR4 GetLightPos(LPDIRECT3DDEVICE9	lp_device, 
						const D3DXMATRIX&	mat,
						DWORD				light_index = 0);

//////////////////////////////////////////////////
//	・関数名	GetViewPos
//	・説明		固定パイプラインから視点行列を取り出し
//				行列に対する視点位置の計算を行う
//	・引数	
//		- lp_device		デバイス
//		- mat			視点位置を求めるための行列
//	・戻り値	視点位置を返す
//////////////////////////////////////////////////
D3DXVECTOR4 GetViewPos( LPDIRECT3DDEVICE9	lp_device, 
						const D3DXMATRIX&	mat );

//////////////////////////////////////////////////
//	・関数名	GetTexPos
//	・説明		3D座標系からテクスチャ座標系に変換する
//	・引数		
//		- lpdevice	デバイス
//		- pos		求める座標(3D)
//	・戻り値	渡された座標に対するテクスチャ座標を返す
//////////////////////////////////////////////////
D3DXVECTOR2 GetTexPos(	LPDIRECT3DDEVICE9 lpdevice,
						const D3DXVECTOR3& pos );

D3DXVECTOR2 GetTexPos(	const D3DXMATRIX&	view,
						const D3DXMATRIX&	proj,
						const D3DXVECTOR3&	pos );

//////////////////////////////////////////////////
//	・関数名	GetPosiOrNega
//	・説明		正の値か負の値を獲得する
//	・引数		なし
//	・戻り値	ランダムで1か-1を返す
//////////////////////////////////////////////////
inline int GetPosiOrNega()
{
	return rand() % 2 * 2 - 1;
}

//////////////////////////////////////////////////
//	・関数名	GetSlipVec
//	・説明		渡された値から、滑りベクトルを計算する
//	・引数		
//		- in_vec		進んでいる方向ベクトル
//		- normal_vec	衝突対象の法線ベクトル
//	・戻り値	計算した滑りベクトルを返す
//////////////////////////////////////////////////
inline D3DXVECTOR3 GetSlipVec( const D3DXVECTOR3& in_vec, const D3DXVECTOR3& normal_vec )
{
	return in_vec - ( D3DXVec3Dot( &in_vec, &normal_vec ) / pow( D3DXVec3Length( &normal_vec ), 2 ) ) * normal_vec;
}

//////////////////////////////////////////////////
//	・関数名	GetAddress
//	・説明		渡された浮動小数点値をDWORDに変換する
//	・引数		
//		- f		変換する浮動小数点値
//	・戻り値	変換した浮動小数点値を返す
//////////////////////////////////////////////////
inline DWORD GetAddress( float f )
{
	return *( reinterpret_cast<DWORD*>( &f ) );
}

#endif // __AKI_UTILITY_H__