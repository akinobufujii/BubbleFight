#ifndef __AKI_USING_H__
#define __AKI_USING_H__


//////////////////////////////////////////
// �}�N����`
//////////////////////////////////////////

// �������̉��
#define SAFE_DELETE(p)		{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)		{ if(p) { (p)->Release(); (p)=NULL; } }

// �萔
#define CONSTANT static const 

// ���������[�N�p
#ifdef	_DEBUG
#define NEW  ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )	// ���������[�N�`�F�b�N
#else
#define NEW  ::new(std::nothrow)						// NULL�|�C���^��Ԃ�
#endif

//////////////////////////////////////////////////
//	�E�֐���	DebugMessageBox
//	�E����		�f�o�b�O���̂ݕ\�����郁�b�Z�[�W�{�b�N�X
//	�E����		
//		- info	�\��������e
//	�E�߂�l	�Ȃ�
//////////////////////////////////////////////////
inline void DebugMessageBox( LPCSTR info )
{
#ifdef	_DEBUG
	//MessageBox( NULL, info, "Info", MB_OK );
#endif
}

//////////////////////////////////////////////////
// �^�錾
//////////////////////////////////////////////////

// �z��p
typedef boost::shared_array<D3DXVECTOR3>		LPVECTOR3ARRAY;
typedef boost::shared_array<D3DMATERIAL9>		LPMATERIALARRAY;
typedef boost::shared_array<LPDIRECT3DTEXTURE9>	LPTEXTUREARRAY;

#endif // __AKI_USING_H__