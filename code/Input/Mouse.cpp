//================================================================
// 
// マウス(DirectInput)
// 嘉手納春慶
// 
//================================================================
#include "Mouse.h"

bool C_Mouse::CreateInputDevice(LPDIRECTINPUT8 input)
{
	if (FAILED(input->CreateDevice(GUID_SysMouse, &m_pInputDevice, NULL)))
	{
		MessageBox(NULL, "マウスデバイスが作成できませんでした", "エラー", MB_OK);
		return false;
	}
	return true;
}

bool C_Mouse::SetDataFormat(void)
{
	if (FAILED(m_pInputDevice->SetDataFormat(&c_dfDIMouse)))
	{
		MessageBox(NULL, "マウスデータフォーマットが設定できませんでした", "エラー", MB_OK);
		this->Uninit();
		return false;
	}
	return true;
}

bool C_Mouse::SetCooperativeLevel(HWND hwnd)
{
	if (FAILED(m_pInputDevice->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(NULL, "マウスの排他制御のセットに失敗", "エラー", MB_OK);
		this->Uninit();
		return false;
	}
	return true;
}

C_Mouse::C_Mouse() :
	m_hwnd(NULL),
	m_lx(0),
	m_ly(0),
	m_x(0),
	m_y(0)
{
	for (int i = 0; i < BUTTONNUM; i++)
	{
		m_StatePress[i] = 0;
		m_StateTrigger[i] = 0;
		m_StateRelease[i] = 0;
	}
}

C_Mouse::~C_Mouse()
{
	this->Uninit();
}

bool C_Mouse::Init(HWND hwnd, LPDIRECTINPUT8 input)
{
	m_hwnd = hwnd;

	if (!CreateInputDevice(input))
	{
		return false;
	}

	if (!SetDataFormat())
	{
		return false;
	}

	if (!SetCooperativeLevel(hwnd))
	{
		return false;
	}

	m_pInputDevice->Acquire();

	return true;
}

void C_Mouse::Update(void)
{
	DIMOUSESTATE mouse = {};//マウスの状態


	if (SUCCEEDED(m_pInputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouse)))
	{
		POINT p;
		if (!mStatus)
		{
			ZeroMemory(&mouse, sizeof(mouse));
			p.x = 0;
			p.y = 0;
		}
		else
		{
			GetCursorPos(&p);
			ScreenToClient(m_hwnd, &p);
		}

		for (int i = 0; i < BUTTONNUM; i++)
		{
			m_StateTrigger[i] = (m_StatePress[i] ^ mouse.rgbButtons[i]) & mouse.rgbButtons[i];
			m_StateRelease[i] = (m_StatePress[i] ^ mouse.rgbButtons[i]) & m_StatePress[i];

			m_StatePress[i] = mouse.rgbButtons[i];
		}

		//移動量
		m_lx = mouse.lX;
		m_ly = mouse.lY;
		m_wheel = mouse.lZ;

		//座標
		m_x = p.x;
		m_y = p.y;
	}
	else
	{
		m_pInputDevice->Acquire();
	}


}

void C_Mouse::IsVelocity(LONG* x, LONG* y)
{
	*x = m_lx;
	*y = m_ly;
}

LONG C_Mouse::IsVelocityX(void)
{
	return m_lx;
}

LONG C_Mouse::IsVelocityY(void)
{
	return m_ly;
}

void C_Mouse::IsPoint(LONG* x, LONG* y)
{
	*x = m_x;
	*y = m_y;
}

LONG C_Mouse::IsPointX(void)
{
	return m_x;
}

LONG C_Mouse::IsPointY(void)
{
	return m_y;
}

float C_Mouse::IsWheel(void)
{
	if (m_wheel > 0)
	{
		return 1.0f;
	}
	else if (m_wheel < 0)
	{
		return -1.0f;
	}
	else
	{
		return 0.0f;
	}
}

bool C_Mouse::IsPress(int nKey)
{
	return (m_StatePress[nKey] & 0x80) ? true : false;
}

bool C_Mouse::IsTrigger(int nKey)
{
	return (m_StateTrigger[nKey] & 0x80) ? true : false;
}

bool C_Mouse::IsRelease(int nKey)
{
	return (m_StateRelease[nKey] & 0x80) ? true : false;
}