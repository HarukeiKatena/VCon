//====================================================================
//インプット
//嘉手納春慶
//====================================================================
#pragma once
#include<Windows.h>

#define DIRECTINPUT_VERSION (0x0800)
#include<dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class C_Input
{
protected:
	static LPDIRECTINPUT8 m_Input;

public:
	//===============================================================
	//インターフェース作成
	//===============================================================
	/*
	hInstance	インスタンスハンドル
	*/
	static bool CreateInterface(HINSTANCE hInstance);

	//===============================================================
	//終了
	//===============================================================
	/*
	デストラクタで呼ばれる
	*/
	static bool InterfaceUninit(void);

	//===============================================================
	//インターフェース取得
	//===============================================================
	static LPDIRECTINPUT8 GetInput(void) { return m_Input; }
};