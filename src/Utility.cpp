#include "StdAfx.h"
#include "Utility.h"

// �O���[�o���ϐ�
static CCamera	g_Camera;	// �J����
static CProj	g_Proj;		// ���e

// ���̓����蔻��
bool IsHitByCircle( const D3DXVECTOR3& v1, float r1, const D3DXVECTOR3& v2, float r2 )
{
	// ��_�Ԃ̋��������߂�
	D3DXVECTOR3 vLength = v1 - v2;
	
	/*if( pow( v2.x - v1.x, 2 ) + pow( v2.y - v1.y, 2 ) + pow( v2.z - v1.z, 2 ) <= pow( r1 + r2, 2 ) )
	{
		return true;
	}*/

	// ����
	if( D3DXVec3Length( &vLength ) < r1 + r2 )
	{
		return true;
	}

	return false;
}

// ������J�����O
bool IsCulling( const CCamera* lp_camera, const CProj* lp_proj, const D3DXMATRIX* lp_mat )
{
	// ���f�ޗ�
	D3DXPLANE	Top, Bottom, Right, Left;
	CVector		Pos( *lp_mat );
	CCamera		Camera	= *lp_camera;
	CProj		Proj	= *lp_proj;

	// �J�����s�񂩂���W�ϊ�
	Pos.GetTransCoord( Camera.GetMatrix() );

	// ���ʌv�Z
	CVector	V1, V2;

	// ��
	V1.x = -Proj.GetFarClip() * ( tan( Proj.GetFovy() / 2 ) * Proj.GetAspect() );
	V1.y =  Proj.GetFarClip() * tan( Proj.GetFovy() / 2 );
	V1.z =  Proj.GetFarClip();

	V2 = V1;
	V2.x *= -1;

	D3DXPlaneFromPoints( &Top, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// ��
	V1.x =  Proj.GetFarClip() * ( tan( Proj.GetFovy() / 2 ) * Proj.GetAspect() );
	V1.y = -Proj.GetFarClip() * tan( Proj.GetFovy() / 2 );
	V1.z =  Proj.GetFarClip();

	V2 = V1;
	V2.x *= -1;

	D3DXPlaneFromPoints( &Bottom, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );
	
	// �E
	V1.x = Proj.GetFarClip() * ( tan( Proj.GetFovy() / 2 ) * Proj.GetAspect() );
	V1.y = Proj.GetFarClip() * tan( Proj.GetFovy() / 2 );
	V1.z = Proj.GetFarClip();

	V2 = V1;
	V2.y *= -1;

	D3DXPlaneFromPoints( &Right, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// ��
	V1.x = Proj.GetFarClip() * ( tan( Proj.GetFovy() / 2 ) * Proj.GetAspect() );
	V1.y = Proj.GetFarClip() * tan( Proj.GetFovy() / 2 );
	V1.z = Proj.GetFarClip();

	V2 = V1;
	V2.y *= -1;

	D3DXPlaneFromPoints( &Left, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// ���肩���ĕ`����ȗ����Ă���

	// �j�A�N���b�v����
	if( Pos.z < Proj.GetNearClip() )
	{
		return false;	// �`�悵�Ȃ�
	}

	// �t�@�[�N���b�v
	if( Pos.z > Proj.GetFarClip() )
	{
		return false;	// �`�悵�Ȃ�
	}

	// ��
	float Result = ( Pos.y * Top.b ) + ( Pos.z * Top.c );
	
	if( Result > 0.f )
	{
		return false;	// �`�悵�Ȃ�
	}

	// ��
	Result = ( Pos.y * Bottom.b ) + ( Pos.z * Bottom.c );

	if( Result > 0.f )
	{
		return false;	// �`�悵�Ȃ�
	}

	// �E
	Result = ( Pos.x * Right.a ) + ( Pos.z * Right.c );

	if( Result > 0.f )
	{
		return false;	// �`�悵�Ȃ�
	}

	// ��
	Result = ( Pos.x * Left.a ) + ( Pos.z * Left.c );

	if( Result > 0.f )
	{
		return false;	// �`�悵�Ȃ�
	}

	return true;	// �`�悷��
}

// ���łɐݒ肳�ꂽ�J�����Ɠ��e�Ŏ����䂩�����O����
bool IsCulling( const D3DXMATRIX* lp_mat )
{
	// ���f�ޗ�
	D3DXPLANE	Top, Bottom, Right, Left;
	CVector		Pos( *lp_mat );
	
	// �J�����s�񂩂���W�ϊ�
	Pos.GetTransCoord( g_Camera.GetMatrix() );

	// ���ʌv�Z
	CVector	V1, V2;

	// ��
	V1.x = -g_Proj.GetFarClip() * ( tan( g_Proj.GetFovy() / 2 ) * g_Proj.GetAspect() );
	V1.y =  g_Proj.GetFarClip() * tan( g_Proj.GetFovy() / 2 );
	V1.z =  g_Proj.GetFarClip();

	V2 = V1;
	V2.x *= -1;

	D3DXPlaneFromPoints( &Top, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// ��
	V1.x =  g_Proj.GetFarClip() * ( tan( g_Proj.GetFovy() / 2 ) * g_Proj.GetAspect() );
	V1.y = -g_Proj.GetFarClip() * tan( g_Proj.GetFovy() / 2 );
	V1.z =  g_Proj.GetFarClip();

	V2 = V1;
	V2.x *= -1;

	D3DXPlaneFromPoints( &Bottom, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );
	
	// �E
	V1.x = g_Proj.GetFarClip() * ( tan( g_Proj.GetFovy() / 2 ) * g_Proj.GetAspect() );
	V1.y = g_Proj.GetFarClip() * tan( g_Proj.GetFovy() / 2 );
	V1.z = g_Proj.GetFarClip();

	V2 = V1;
	V2.y *= -1;

	D3DXPlaneFromPoints( &Right, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// ��
	V1.x = g_Proj.GetFarClip() * ( tan( g_Proj.GetFovy() / 2 ) * g_Proj.GetAspect() );
	V1.y = g_Proj.GetFarClip() * tan( g_Proj.GetFovy() / 2 );
	V1.z = g_Proj.GetFarClip();

	V2 = V1;
	V2.y *= -1;

	D3DXPlaneFromPoints( &Left, &D3DXVECTOR3( 0, 0, 0 ), &V1, &V2 );

	// ���肩���ĕ`����ȗ����Ă���

	// �j�A�N���b�v����
	if( Pos.z < g_Proj.GetNearClip() )
	{
		return false;	// �`�悵�Ȃ�
	}

	// �t�@�[�N���b�v
	if( Pos.z > g_Proj.GetFarClip() )
	{
		return false;	// �`�悵�Ȃ�
	}

	// ��
	float Result = ( Pos.y * Top.b ) + ( Pos.z * Top.c );
	
	if( Result > 0.f )
	{
		return false;	// �`�悵�Ȃ�
	}

	// ��
	Result = ( Pos.y * Bottom.b ) + ( Pos.z * Bottom.c );

	if( Result > 0.f )
	{
		return false;	// �`�悵�Ȃ�
	}

	// �E
	Result = ( Pos.x * Right.a ) + ( Pos.z * Right.c );

	if( Result > 0.f )
	{
		return false;	// �`�悵�Ȃ�
	}

	// ��
	Result = ( Pos.x * Left.a ) + ( Pos.z * Left.c );

	if( Result > 0.f )
	{
		return false;	// �`�悵�Ȃ�
	}

	return true;	// �`�悷��
}

// ����p�J�����ݒ�
void SetCullingCamera( const CCamera* lp_camera )
{
	g_Camera = *lp_camera;
}

// ����p���e�ݒ�
void SetCullingProj( const CProj* lp_proj )
{
	g_Proj = *lp_proj;
}

// ���b�V���ɑ΂��Ẵ��C����
// (�������Ă��Ȃ���Ε��̒l��Ԃ�)
float IntersectByMesh( const D3DXMATRIX* lp_mpos, const D3DXVECTOR3* lay_dir, const D3DXMATRIX* lp_mesh_local, const LPD3DXMESH mesh, LPDWORD lp_polygon_no )
{
	D3DXMATRIX mInv = *lp_mesh_local;	// �t�s��p
	D3DXMATRIX mPos = *lp_mpos;			// ���C���ˍs��

	// ���[�J���O��s����t�s��ɂ���
	D3DXMatrixInverse( &mInv, NULL, &mInv );

	D3DXVECTOR3 LocalPos, LayDir;	// ���[�J�����W�A���C���˕���

	// ���[���h���W���o
	D3DXVec3TransformCoord( &LocalPos, &D3DXVECTOR3( 0, 0, 0 ), &mPos );

	// ���[�J�����W�Ń��C���ˈʒu�m��
	D3DXVec3TransformCoord( &LocalPos, &LocalPos, &mInv );

	// ���C���˕����m��
	D3DXVec3TransformNormal( &LayDir, lay_dir, &mInv );

	// ����
	BOOL	IsHit	= 0;	// ����(�g��Ȃ����֐����s�̂���)
	float	Len		= -1.f;	// ����

	D3DXIntersect(
		mesh,			// ���b�V���f�[�^
		&LocalPos,		// ���ˈʒu
		&LayDir,		// ���˕���
		&IsHit,			// ���茋�ʊi�[�ϐ�
		lp_polygon_no,	// ���������|���S��
		NULL,
		NULL,
		&Len,		// �����i�[�p
		NULL,
		NULL);

	return Len;
}

// ��]�A�j���[�V�����v�Z�֐�
// ��]�s���Ԃ�
// start�E�E�E�J�n�s��
// end�E�E�E�I�_�s��
// anime�E�E�E�A�j���[�V�����J�E���^(0.f�`1.f)
// rot_only�E�E�E��]�s��݂̂�Ԃ�
D3DXMATRIX RotationAnimeMatarix( const LPD3DXMATRIX start, const LPD3DXMATRIX end, float anime, bool rot_only )
{
	D3DXQUATERNION	qStart, qEnd, qNow;	// �J�n�A�I�_�A���̃N�H�[�^�j�I��
	D3DXMATRIX mRot;					// ��]�s��

	// �n���ꂽ��񂩂�N�H�[�^�j�I���쐬
	D3DXQuaternionRotationMatrix( &qStart, start );
	D3DXQuaternionRotationMatrix( &qEnd, end );

	// ���ԏ�Ԃ̃N�H�[�^�j�I���쐬
	D3DXQuaternionSlerp( &qNow, &qStart, &qEnd, anime );

	// �s��ɒ���
	D3DXMatrixRotationQuaternion( &mRot, &qNow );

	// ��]�s��݂̂Ȃ炱���Œl��Ԃ�
	if( rot_only )
	{
		return mRot;
	}

	D3DXVECTOR3 Pos;	// �ʒu�i�[�p

	// �ʒu��񒊏o
	D3DXVec3Lerp( &Pos, &CVector( *start ), &CVector( *end ), anime );

	return mRot * CMatrix( Pos );
}

// �ǔ������]���Ɖ�]�p�x�����߂�
float GetAxisAndRot(D3DXVECTOR3*		out_axis,
					const D3DXVECTOR3&	target,
					const D3DXMATRIX&	mat,
					const D3DXVECTOR3&	target_front )
{
	// �t�s������߂�
	D3DXMATRIX mInv;
	D3DXMatrixInverse( &mInv, NULL, &mat );

	// �s�񂩂猩���ʒu�����߂�
	D3DXVECTOR3 TargetPos;
	D3DXVec3TransformCoord( &TargetPos, &target, &mInv );

	// ���K��
	D3DXVec3Normalize( &TargetPos, &TargetPos );

	// ��]�������߂�
	D3DXVec3Cross( out_axis, &target_front, &TargetPos );

	// ���ς����߂�
	float Dot = D3DXVec3Dot( &target_front, &TargetPos );

	if( Dot >  1.f )
	{
		Dot =  1.f;
	
	}else if( Dot < -1.f ){

		Dot = -1.f;
	}

	// �p�x��Ԃ�
	return D3DXToDegree( acos( Dot ) );
}

// �G���[�֐�(�G���[���N�����true��Ԃ�)
bool ErrorMessage( HRESULT hr, LPCSTR msg )
{
	if( FAILED( hr ) )
	{
		MessageBox( NULL, msg, "ERROR", MB_OK );
		return true;
	}

	return false;
}

// ��`�����蔻��
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

// ���C�g�ʒu���v�Z����
D3DXVECTOR4 GetLightPos(LPDIRECT3DDEVICE9	lp_device, 
						const D3DXMATRIX&	mat,
						DWORD				light_index)
{
	// ���C�g���l��
	D3DLIGHT9 Light;
	lp_device->GetLight( light_index, &Light );
	
	// �s��ɑ΂��郉�C�g�ʒu�v�Z
	D3DXVECTOR4 LightPos( Light.Direction, 1.f );
	D3DXMATRIX	mInv;

	D3DXMatrixInverse( &mInv, NULL, &mat );
	D3DXVec4Transform( &LightPos, &LightPos, &mInv );
	
	return LightPos;
}

// ���_�ʒu�����߂�
D3DXVECTOR4 GetViewPos(LPDIRECT3DDEVICE9 lp_device, 
					   const D3DXMATRIX& mat )
{
	// ���_�s����l��
	D3DXMATRIX mView, mInv;

	lp_device->GetTransform( D3DTS_VIEW, &mView );

	// ���_�ʒu�����߂�
	D3DXVECTOR3 Pos = *g_Camera.GetPos();
	D3DXVECTOR4 ViewPos( Pos.x, Pos.y, Pos.z , 1 );

	D3DXMatrixInverse( &mInv, NULL, &(mat * mView) );
	D3DXVec4Transform( &ViewPos, &ViewPos, &mInv );

	return ViewPos;
}

// �X�N���[�����W�����߂�
D3DXVECTOR2 GetTexPos(LPDIRECT3DDEVICE9 lpdevice,
					  const D3DXVECTOR3& pos )
{
	D3DXMATRIX mTemp1, mTemp2;

	// �Œ�p�C�v���C�����王�_�Ɠ��e�s����擾
	lpdevice->GetTransform( D3DTS_VIEW, &mTemp1 );
	lpdevice->GetTransform( D3DTS_PROJECTION, &mTemp2 );

	// ���W�ϊ�����
	D3DXVECTOR3 Pos;
	D3DXVec3TransformCoord( &Pos, &pos, &(mTemp1 * mTemp2) );

	// �͈͂�0�`1�ɕϊ�����
	D3DXMatrixScaling( &mTemp1, 0.5f, -0.5, 1.f );
	D3DXMatrixTranslation( &mTemp2, 0.5f, 0.5f, 0 );

	D3DXVec3TransformCoord( &Pos, &Pos, &(mTemp1 * mTemp2) );

	// ���߂�x,y���e�N�X�`�����W
	return D3DXVECTOR2( Pos.x, Pos.y );
}

D3DXVECTOR2 GetTexPos(	const D3DXMATRIX&	view,
						const D3DXMATRIX&	proj,
						const D3DXVECTOR3&	pos )
{
	D3DXMATRIX mTemp1, mTemp2;

	// ���W�ϊ�����
	D3DXVECTOR3 Pos;
	D3DXVec3TransformCoord( &Pos, &pos, &(view * proj) );

	// �͈͂�0�`1�ɕϊ�����
	D3DXMatrixScaling( &mTemp1, 0.5f, -0.5, 1.f );
	D3DXMatrixTranslation( &mTemp2, 0.5f, 0.5f, 0 );

	D3DXVec3TransformCoord( &Pos, &Pos, &(mTemp1 * mTemp2) );

	// ���߂�x,y���e�N�X�`�����W
	return D3DXVECTOR2( Pos.x, Pos.y );
}