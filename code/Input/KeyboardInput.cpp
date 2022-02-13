//==============================================================
//
//	キーボードインプット
//	嘉手納春慶
//
//==============================================================
#include "KeyboardInput.h"

C_KeyboardInput::C_KeyboardInput()
{
	for (int i = 0; i < KEY_MAX; i++)
	{
		m_KeyState[i] = 0;
		m_KeyStateTrigger[i] = 0;
		m_KeyStateRelease[i] = 0;
	}
}

C_KeyboardInput::~C_KeyboardInput()
{
	this->Uninit();
}

//==============================================================
//キーボードデバイス作成
//==============================================================
bool C_KeyboardInput::CreateInputDevice(LPDIRECTINPUT8 input)
{
	if (FAILED(input->CreateDevice(GUID_SysKeyboard, &m_pInputDevice, NULL)))
	{
		MessageBox(NULL, "キーボードデバイスが作成できませんでした", "エラー", MB_OK);
		return false;
	}
	return true;
}

//==============================================================
//入力データ形式セット
//==============================================================
bool C_KeyboardInput::SetDataFormat(void)
{
	if (FAILED(m_pInputDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(NULL, "キーボードの入力形式のセットに失敗", "エラー", MB_OK);
		this->Uninit();
		return false;
	}
	return true;
}

//==============================================================
//排他制御
//==============================================================
bool C_KeyboardInput::SetCooperativeLevel(HWND hwnd)
{
	if (FAILED(m_pInputDevice->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(NULL, "キーボードの排他制御のセットに失敗", "エラー", MB_OK);
		this->Uninit();
		return false;
	}
	return true;
}

//==============================================================
//初期設定
//==============================================================
bool C_KeyboardInput::Init(HWND hwnd, LPDIRECTINPUT8 input)
{
	//デバイス作成
	if (!CreateInputDevice(input))
	{
		return false;
	}

	//入力形式セット
	if (!SetDataFormat())
	{
		return false;
	}

	//排他制御セット
	if (!SetCooperativeLevel(hwnd))
	{
		return false;
	}

	//動作開始
	m_pInputDevice->Acquire();

	return true;
}

//==============================================================
//更新
//==============================================================
void C_KeyboardInput::Update(void)
{
	BYTE KeyState[KEY_MAX];

	//データ取得
	if (SUCCEEDED(m_pInputDevice->GetDeviceState(sizeof(KeyState), KeyState)))
	{
		if (!mStatus)//falseの時すべて0にする
		{
			ZeroMemory(&KeyState, sizeof(BYTE) * KEY_MAX);
		}

		for (int CnKey = 0; CnKey < KEY_MAX; CnKey++)
		{
			//トリガーとリリース
			m_KeyStateTrigger[CnKey] = (m_KeyState[CnKey] ^ KeyState[CnKey]) & KeyState[CnKey];
			m_KeyStateRelease[CnKey] = (m_KeyState[CnKey] ^ KeyState[CnKey]) & m_KeyState[CnKey];

			//プレス
			m_KeyState[CnKey] = KeyState[CnKey];
		}

	}
	else
	{
		memset(m_KeyStateTrigger, 0, sizeof(m_KeyStateTrigger));
		memset(m_KeyStateRelease, 0, sizeof(m_KeyStateRelease));
		memset(m_KeyState, 0, sizeof(m_KeyState));
		m_pInputDevice->Acquire();
	}
}

//==============================================================
//入力状態取得
//==============================================================
bool C_KeyboardInput::IsPress(int nKey)
{
	return (m_KeyState[nKey] & 0x80) ? true : false;
}

bool C_KeyboardInput::IsTrigger(int nKey)
{
	return (m_KeyStateTrigger[nKey] & 0x80) ? true : false;
}

bool C_KeyboardInput::IsRelease(int nKey)
{
	return (m_KeyStateRelease[nKey] & 0x80) ? true : false;
}