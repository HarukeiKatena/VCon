//=============================================================
//XInput
//嘉手納春慶
//=============================================================
#include"MyXInput.h"
#include<cmath>

#define INDEX_NULLCLEAR (0xffff)

CXInput::CXInput()
{
	for (int i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_gamepadindex[i] = INDEX_NULLCLEAR;

		m_ButtonState[i] = 0;
		m_ButtonStateRelease[i] = 0;
		m_ButtonStateTrigger[i] = 0;

		m_LeftTrigger[i] = 0;
		m_RightTrigger[i] = 0;

		m_stickLX[i] = 0.0f;
		m_stickLY[i] = 0.0f;

		m_stickRX[i] = 0.0f;
		m_stickRY[i] = 0.0f;
	}

	m_lookpad = 0;
}

CXInput::~CXInput()
{

}

bool CXInput::Init(void)
{
	return false;
}

void CXInput::Uninit(void)
{

}

void CXInput::Update(void)
{
	unsigned int padnum = 0;

	for (int k = 0; k < XUSER_MAX_COUNT; k++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(state));
		auto result = XInputGetState(k, &state);//コントローラーの状態取得

		if (result != ERROR_SUCCESS)//エラーだったらコンテ
		{
			continue;
		}

		int i = -1;
		for (int j = 0; j < XUSER_MAX_COUNT; j++)
		{
			if (m_gamepadindex[j] == state.dwPacketNumber && !(padnum & (1 << j)))
			{
				i = j;
				break;
			}
			else if (m_gamepadindex[j] == INDEX_NULLCLEAR && i == -1)//空の場合
			{
				i = j;
			}
		}

		//トリガーとリリース
		m_ButtonStateTrigger[i] = (m_ButtonState[i] ^ state.Gamepad.wButtons) & state.Gamepad.wButtons;
		m_ButtonStateRelease[i] = (m_ButtonState[i] ^ state.Gamepad.wButtons) & m_ButtonState[i];

		////プレス
		m_ButtonState[i] = state.Gamepad.wButtons;

		//左トリガー
		if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			m_LeftTrigger[i] = std::fmin((float)state.Gamepad.bLeftTrigger / 255.0f, 1.0f);
		}
		else
		{
			m_LeftTrigger[i] = 0.0f;
		}

		//右トリガー
		if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			m_RightTrigger[i] = std::fmin((float)state.Gamepad.bRightTrigger / 255.0f, 1.0f);
		}
		else
		{
			m_RightTrigger[i] = 0.0f;
		}

		//左スティック
		if (std::abs(state.Gamepad.sThumbLX) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//指定した範囲より大きい場合
		{
			m_stickLX[i] = (float)state.Gamepad.sThumbLX / 32767.0f;
			m_stickLX[i] = m_stickLX[i] > 0.0f ? std::fmin(m_stickLX[i], 1.0f) : std::fmax(m_stickLX[i], -1.0f);
		}
		else
		{
			m_stickLX[i] = 0.0f;
		}

		if (std::abs(state.Gamepad.sThumbLY) >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)//指定した範囲より大きい場合
		{
			m_stickLY[i] = (float)state.Gamepad.sThumbLY / 32767.0f;
			m_stickLY[i] = m_stickLY[i] > 0.0f ? std::fmin(m_stickLY[i], 1.0f) : std::fmax(m_stickLY[i], -1.0f);
		}
		else
		{
			m_stickLY[i] = 0.0f;
		}

		//右スティック
		if (std::abs(state.Gamepad.sThumbRX) >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)//指定した範囲より大きい場合
		{
			m_stickRX[i] = (float)state.Gamepad.sThumbRX / 32767.0f;
			m_stickRX[i] = m_stickRX[i] > 0.0f ? std::fmin(m_stickRX[i], 1.0f) : std::fmax(m_stickRX[i], -1.0f);
		}
		else
		{
			m_stickRX[i] = 0.0f;
		}

		if (std::abs(state.Gamepad.sThumbRY) >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)//指定した範囲より大きい場合
		{
			m_stickRY[state.dwPacketNumber] = (float)state.Gamepad.sThumbRY / 32767.0f;
			m_stickRY[i] = m_stickRY[i] > 0.0f ? std::fmin(m_stickRY[i], 1.0f) : std::fmax(m_stickRY[i], -1.0f);
		}
		else
		{
			m_stickRY[i] = 0.0f;
		}

		padnum = padnum | 1 << i;
	}
}

float CXInput::IsAxis(int nKey)
{
	switch (nKey)
	{
	case XINPUT::LRFTTRIGGER: return m_LeftTrigger[m_lookpad];
	case XINPUT::RIGHTTRIGGER: return m_RightTrigger[m_lookpad];
	case XINPUT::STICKLX: return m_stickLX[m_lookpad];
	case XINPUT::STICKLY: return m_stickLY[m_lookpad];
	case XINPUT::STICKRX: return m_stickRX[m_lookpad];
	case XINPUT::STICKRY: return m_stickRY[m_lookpad];
	default: return 0.0f;
	}
}

bool CXInput::IsPress(int nKey)
{
	return m_ButtonState[m_lookpad] & nKey;
}

bool CXInput::IsTrigger(int nKey)
{
	return m_ButtonStateTrigger[m_lookpad] & nKey;
}

bool CXInput::IsRelease(int nKey)
{
	return m_ButtonStateRelease[m_lookpad] & nKey;
}

void CXInput::SetStatus(bool status)
{
	mStatus = status;
#pragma warning(push)
#pragma warning(disable : 4995)
	XInputEnable(mStatus);
#pragma warning(pop)
}
