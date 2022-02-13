#include "DirectInput8.h"

LPDIRECTINPUT8 C_Input::m_Input = nullptr;

//===============================================================
//インターフェース作成
//===============================================================
bool C_Input::CreateInterface(HINSTANCE hInstance)
{
	HRESULT hr = DirectInput8Create(
		hInstance,				//インスタンスハンドル
		DIRECTINPUT_VERSION,	//バージョン
		IID_IDirectInput8,		//取得インターフェース
		(VOID**)&m_Input,		//インターフェースポインタ
		NULL					//なぞ
	);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

//===============================================================
//終了
//===============================================================
bool C_Input::InterfaceUninit(void)
{
	if (m_Input != nullptr)
	{
		m_Input->Release();
		m_Input = nullptr;
	}

	return true;
}