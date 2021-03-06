#ifndef __AKI_KEYBOARD_H__
#define __AKI_KEYBOARD_H__

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace Input
{
	// 入力状態列挙体
	enum INPUTSTATE
	{
		FREE	= 0x00,	// 離されてる
		PUSH	= 0x01,	// 押された	
		HOLD	= 0x02,	// 押されている
		PULL	= 0x04	// 離された
	};

	//////////////////////////////////////////////////
	//	・関数名	CreateInterFace
	//	・説明		DirectInputのインターフェースを作成する
	//	・引数		
	//		- hinst	インスタンスハンドル
	//	・戻り値	成功すればtrue
	//				失敗すればfalseを返す
	//////////////////////////////////////////////////
	bool CreateInterFace( HINSTANCE hinst );

	//////////////////////////////////////////////////
	//	・関数名	DestroyInterFase
	//	・説明		DirectInputのインターフェースを破壊する
	//	・引数		なし
	//	・戻り値	なし
	//////////////////////////////////////////////////
	void DestroyInterFase();

	//////////////////////////////////////////////////
	// キーボード関連
	//////////////////////////////////////////////////
	namespace Keyboard
	{
		//////////////////////////////////////////////////
		//	・関数名	Init
		//	・説明		キーボードデバイスの初期化を行う
		//				CreateInterFaceを呼んでから実行する
		//	・引数		
		//		- hwnd	ウィンドウハンドル
		//	・戻り値	なし
		//////////////////////////////////////////////////
		bool Init( HWND hwnd );
		
		//////////////////////////////////////////////////
		//	・関数名	UpdateState
		//	・説明		キーボードの状態を更新する
		//				毎フレーム1回呼び出す
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void UpdateState();

		//////////////////////////////////////////////////
		//	・関数名	SetAcquire
		//	・説明		入力制御をおこなう
		//	・引数		
		//		- flg	trueで入力制御をおこなう
		//				falseで入力制御を行わない
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void SetAcquire( bool flg );

		//////////////////////////////////////////////////
		//	・関数名	IsState
		//	・説明		入力状態を取得する
		//	・引数		
		//		- key	状態を知りたいキー番号を渡す
		//				DIK_から始まる定数を渡す
		//		- state	判断する入力状態を渡す
		//				INPUTSTATE列挙体から渡す
		//	・戻り値	渡された引数の状態に当てはまれば
		//				true、そうでなければfalseを返す
		//////////////////////////////////////////////////
		bool IsState( BYTE key, BYTE state );
		
		//////////////////////////////////////////////////
		//	・関数名	Release
		//	・説明		キーボードデバイスを破壊する
		//				DestroyInterFaseを呼ぶ前に実行する
		//	・引数		なし
		//	・戻り値	なし
		//////////////////////////////////////////////////
		void Release();
	}

	//////////////////////////////////////////////////
	// マウス関連
	//////////////////////////////////////////////////
	//namespace Mouse
	//{
	//	// 入力キー
	//	enum MOUSBUTTON
	//	{
	//		BUTTON_L,	// 左
	//		BUTTON_R,	// 右

	//	};

	//	// 初期化
	//	bool Init( HWND hWnd );

	//	// マウス入力状態更新
	//	void UpdateState();

	//	// 入力制御
	//	void IsState( bool flg );

	//	// 入力状態取得
	//	bool IsState( BYTE button, BYTE State );

	//	// 解放
	//	void Release();
	//}
}

#endif // __AKI_KEYBOARD_H__