#ifndef __AKI_UTILITY_H__
#define __AKI_UTILITY_H__

//////////////////////////////////////////////////
//	�E�֐���	IsHitByCircle
//	�E����		���̓����蔻����s��
//	�E����	
//		- v1	1�߂̈ʒu
//		- r1	1�߂̔��a
//		- v2	2�߂̈ʒu
//		- r2	2�߂̔��a
//	�E�߂�l	�Փ˂��Ă����true�A
//				�����łȂ����false��Ԃ�
//////////////////////////////////////////////////
bool IsHitByCircle(const D3DXVECTOR3& v1,
				   float r1,
				   const D3DXVECTOR3& v2,
				   float r2 );

//////////////////////////////////////////////////
//	�E�֐���	IsCulling
//	�E����		������J�����O������s��
//	�E����	
//		- lp_camera	������s���J����
//		- lp_proj	������s�����e
//		- lp_mat	������s���s��
//	�E�߂�l	������̒��ɂ����true�A
//				�����łȂ����false��Ԃ�
//////////////////////////////////////////////////
bool IsCulling(const CCamera* lp_camera,
			   const CProj* lp_proj,
			   const D3DXMATRIX* lp_mat );

//////////////////////////////////////////////////
//	�E�֐���	IsCulling
//	�E����		�ݒ肵�Ă���J�����Ɠ��e��
//				������J�����O������s��
//	�E����	
//		- lp_mat	������s���s��
//	�E�߂�l	������̒��ɂ����true�A
//				�����łȂ����false��Ԃ�
//////////////////////////////////////////////////
bool IsCulling( const D3DXMATRIX* lp_mat );

//////////////////////////////////////////////////
//	�E�֐���	SetCullingCamera
//	�E����		IsCulling�Ŏg�p����J������ݒ肷��
//	�E����	
//		- lp_camera	������s���J����
//	�E�߂�l	�Ȃ�
//////////////////////////////////////////////////
void SetCullingCamera( const CCamera* lp_camera );

//////////////////////////////////////////////////
//	�E�֐���	SetCullingProj
//	�E����		IsCulling�Ŏg�p���铊�e��ݒ肷��
//	�E����	
//		- lp_proj	������s�����e
//	�E�߂�l	�Ȃ�
//////////////////////////////////////////////////
void SetCullingProj( const CProj* lp_proj );

//////////////////////////////////////////////////
//	�E�֐���	IntersectByMesh
//	�E����		���b�V���ɑ΂��Ẵ��C������s��
//	�E����	
//		- lp_mpos			���C���ˈʒu���̓������s��
//		- lay_dir			���C���˕���
//		- lp_mesh_local_mat	���b�V���̃��[�J���s��
//		- lp_mesh			���b�V�����
//		- lp_polygon_no		�Փ˂����ꍇ�A
//							�|���S���̃C���f�b�N�X�ԍ���Ԃ�
//	�E�߂�l	�Փ˂����ꍇ�A������
//				�Փ˂��Ȃ���΁A-1��Ԃ�
//////////////////////////////////////////////////
float IntersectByMesh(const D3DXMATRIX* lp_mpos,
					  const D3DXVECTOR3* lay_dir,
					  const D3DXMATRIX* lp_mesh_local_mat,
					  const LPD3DXMESH lp_mesh,
					  LPDWORD lp_polygon_no = NULL );

//////////////////////////////////////////////////
//	�E�֐���	RotationAnimeMatarix
//	�E����		�ŏ��̍s�񂩂�Ō�̍s���
//				�A�j���[�V�����J�E���^�ʂ̍s����v�Z����
//	�E����	
//		- lp_start	�A�j���[�V�����J�n���̍s��
//		- lp_end	�A�j���[�V�����I�����̍s��
//		- anime		�A�j���[�V�����J�E���^(0.0�`1.0�܂�)
//		- rot_only	��]�s��݂̂�Ԃ����ǂ���
//	�E�߂�l	rot_only��true�Ȃ�Ή�]�s��݂̂��A
//				rot_only��false�Ȃ�΃J�E���^�ɍ��킹��
//				�s���Ԃ�
//////////////////////////////////////////////////
D3DXMATRIX RotationAnimeMatarix(const LPD3DXMATRIX lp_start,
								const LPD3DXMATRIX lp_end,
								float anime,
								bool rot_only = false );

//////////////////////////////////////////////////
//	�E�֐���	GetAxisAndRot
//	�E����		�ڕW�ɑ΂��Ẳ�]���Ɖ�]�ʂ�Ԃ�
//	�E����	
//		- out_axis		�v�Z������]����Ԃ�
//		- target		�ڕW�ʒu
//		- mat			�ڕW�ɍs�����߂̍s��
//		- target_front	���X�����Ă����ʒu
//	�E�߂�l	��]�ʂ�Ԃ�
//////////////////////////////////////////////////
float GetAxisAndRot(D3DXVECTOR3*		out_axis,
					const D3DXVECTOR3&	target,
					const D3DXMATRIX&	mat,
					const D3DXVECTOR3&	target_front = D3DXVECTOR3( 0, 0, 1 ) );

//////////////////////////////////////////////////
//	�E�֐���	ErrorMessage
//	�E����		�����̒l���G���[�ł���΁A
//				���b�Z�[�W�{�b�N�X��\������
//	�E����	
//		- hr	�֐����s���ʂ̒l��n��
//		- msg	���b�Z�[�W�{�b�N�X�ɕ\������
//				������
//	�E�߂�l	�G���[�ł����true
//				�����łȂ����false
//////////////////////////////////////////////////
bool ErrorMessage( HRESULT hr, LPCSTR msg );

//////////////////////////////////////////////////
//	�E�֐���	IsHitbyCross
//	�E����		���b�Z�[�W�{�b�N�X��\������
//	�E����	
//		- x1	x���W
//		- y1	y���W
//		- w1	��
//		- h1	����
//		- x2	x���W
//		- y2	y���W
//		- w2	��
//		- h2	����
//	�E�߂�l	�������Ă��true,�����łȂ����false��Ԃ�
//////////////////////////////////////////////////
bool IsHitbyCross(int x1, int y1, int w1, int h1,
				  int x2, int y2, int w2, int h2);

//////////////////////////////////////////////////
//	�E�֐���	GetMousePos
//	�E����		����Window�̃}�E�X�ʒu������Ă���
//	�E����		�Ȃ�
//	�E�߂�l	�}�E�X�ʒu��POINT�^�ŕԂ�
//////////////////////////////////////////////////
inline POINT GetMousePos()
{
	POINT CursorPos;
	GetCursorPos( &CursorPos );

	ScreenToClient(	FindWindow( CLASSNAME, CLASSNAME ), &CursorPos );

	return CursorPos;
}

//////////////////////////////////////////////////
//	�E�֐���	GetLightPos
//	�E����		�Œ�p�C�v���C�����烉�C�g�������o��
//				�s��ɑ΂��郉�C�g�ʒu�̌v�Z���s��
//	�E����	
//		- lp_device		�f�o�C�X
//		- mat			���C�g�ʒu�����߂邽�߂̍s��
//		- light_index	�Q�Ƃ��郉�C�g�C���f�b�N�X
//						�����w�肵�Ȃ����0�Ԗڂ��Q�Ƃ����
//	�E�߂�l	���C�g�ʒu��Ԃ�
//////////////////////////////////////////////////
D3DXVECTOR4 GetLightPos(LPDIRECT3DDEVICE9	lp_device, 
						const D3DXMATRIX&	mat,
						DWORD				light_index = 0);

//////////////////////////////////////////////////
//	�E�֐���	GetViewPos
//	�E����		�Œ�p�C�v���C�����王�_�s������o��
//				�s��ɑ΂��鎋�_�ʒu�̌v�Z���s��
//	�E����	
//		- lp_device		�f�o�C�X
//		- mat			���_�ʒu�����߂邽�߂̍s��
//	�E�߂�l	���_�ʒu��Ԃ�
//////////////////////////////////////////////////
D3DXVECTOR4 GetViewPos( LPDIRECT3DDEVICE9	lp_device, 
						const D3DXMATRIX&	mat );

//////////////////////////////////////////////////
//	�E�֐���	GetTexPos
//	�E����		3D���W�n����e�N�X�`�����W�n�ɕϊ�����
//	�E����		
//		- lpdevice	�f�o�C�X
//		- pos		���߂���W(3D)
//	�E�߂�l	�n���ꂽ���W�ɑ΂���e�N�X�`�����W��Ԃ�
//////////////////////////////////////////////////
D3DXVECTOR2 GetTexPos(	LPDIRECT3DDEVICE9 lpdevice,
						const D3DXVECTOR3& pos );

D3DXVECTOR2 GetTexPos(	const D3DXMATRIX&	view,
						const D3DXMATRIX&	proj,
						const D3DXVECTOR3&	pos );

//////////////////////////////////////////////////
//	�E�֐���	GetPosiOrNega
//	�E����		���̒l�����̒l���l������
//	�E����		�Ȃ�
//	�E�߂�l	�����_����1��-1��Ԃ�
//////////////////////////////////////////////////
inline int GetPosiOrNega()
{
	return rand() % 2 * 2 - 1;
}

//////////////////////////////////////////////////
//	�E�֐���	GetSlipVec
//	�E����		�n���ꂽ�l����A����x�N�g�����v�Z����
//	�E����		
//		- in_vec		�i��ł�������x�N�g��
//		- normal_vec	�ՓˑΏۂ̖@���x�N�g��
//	�E�߂�l	�v�Z��������x�N�g����Ԃ�
//////////////////////////////////////////////////
inline D3DXVECTOR3 GetSlipVec( const D3DXVECTOR3& in_vec, const D3DXVECTOR3& normal_vec )
{
	return in_vec - ( D3DXVec3Dot( &in_vec, &normal_vec ) / pow( D3DXVec3Length( &normal_vec ), 2 ) ) * normal_vec;
}

//////////////////////////////////////////////////
//	�E�֐���	GetAddress
//	�E����		�n���ꂽ���������_�l��DWORD�ɕϊ�����
//	�E����		
//		- f		�ϊ����镂�������_�l
//	�E�߂�l	�ϊ��������������_�l��Ԃ�
//////////////////////////////////////////////////
inline DWORD GetAddress( float f )
{
	return *( reinterpret_cast<DWORD*>( &f ) );
}

#endif // __AKI_UTILITY_H__