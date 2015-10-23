#ifndef __AKI_USING_H__
#define __AKI_USING_H__


//////////////////////////////////////////
// マクロ定義
//////////////////////////////////////////

// メモリの解放
#define SAFE_DELETE(p)		{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)		{ if(p) { (p)->Release(); (p)=NULL; } }

// 定数
#define CONSTANT static const 

// メモリリーク用
#ifdef	_DEBUG
#define NEW  ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )	// メモリリークチェック
#else
#define NEW  ::new(std::nothrow)						// NULLポインタを返す
#endif

//////////////////////////////////////////////////
//	・関数名	DebugMessageBox
//	・説明		デバッグ時のみ表示するメッセージボックス
//	・引数		
//		- info	表示する内容
//	・戻り値	なし
//////////////////////////////////////////////////
inline void DebugMessageBox( LPCSTR info )
{
#ifdef	_DEBUG
	//MessageBox( NULL, info, "Info", MB_OK );
#endif
}

//////////////////////////////////////////////////
// 型宣言
//////////////////////////////////////////////////

// 配列用
typedef boost::shared_array<D3DXVECTOR3>		LPVECTOR3ARRAY;
typedef boost::shared_array<D3DMATERIAL9>		LPMATERIALARRAY;
typedef boost::shared_array<LPDIRECT3DTEXTURE9>	LPTEXTUREARRAY;

#endif // __AKI_USING_H__