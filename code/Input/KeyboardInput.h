//==============================================================
//
//	キーボードインプット
//	嘉手納春慶
//
//==============================================================
#pragma once
#include"DirectInputBace.h"

class C_KeyboardInput : public CDirectInputBace
{
protected:
	static const int KEY_MAX = 256;
	BYTE m_KeyState[KEY_MAX];
	BYTE m_KeyStateTrigger[KEY_MAX];
	BYTE m_KeyStateRelease[KEY_MAX];

	//==============================================================
	//キーボードデバイス作成
	//==============================================================
	virtual bool CreateInputDevice(LPDIRECTINPUT8 input);

	//==============================================================
	//入力データ形式セット
	//==============================================================
	virtual bool SetDataFormat(void);

	//==============================================================
	//排他制御
	//==============================================================
	virtual bool SetCooperativeLevel(HWND hwnd);

public:

	C_KeyboardInput();
	virtual ~C_KeyboardInput();

	//==============================================================
	//初期設定
	//==============================================================
	/*
	hInstance	インスタンスハンドル
	hwnd		ウィンドウハンドル
	*/
	virtual bool Init(HWND hwnd, LPDIRECTINPUT8 input);

	//==============================================================
	//更新
	//==============================================================
	virtual void Update(void);

	//==============================================================
	//入力状態取得
	//==============================================================
	/*
	戻り値で指定したキーが
	Press	押されている
	Trigger	押した瞬間
	Release	離した瞬間
	の場合trueを返す
	*/
	virtual bool IsPress(int nKey);
	virtual bool IsTrigger(int nKey);
	virtual bool IsRelease(int nKey);
};
