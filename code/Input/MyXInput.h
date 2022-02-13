//=============================================================
//XInput
//嘉手納春慶
//=============================================================
#pragma once
#include<Windows.h>


#include<XInput.h>
#pragma comment(lib,"xinput.lib")

#include "InputBace.h"

namespace XINPUT
{
	//コントローラ番号
	enum CONTROLLER
	{
		GAMEPAD01,
		GAMEPAD02,
		GAMEPAD03,
		GAMEPAD04,
	};

	//ゲームパッド入力
	enum GAMEPADINPUT
	{
		LRFTTRIGGER,
		RIGHTTRIGGER,
		STICKLX,
		STICKLY,
		STICKRX,
		STICKRY,
	};
}

class CXInput : public C_InputBace
{
private:
	WORD m_gamepadindex[XUSER_MAX_COUNT];

	WORD m_ButtonStateTrigger[XUSER_MAX_COUNT];//トリガー
	WORD m_ButtonStateRelease[XUSER_MAX_COUNT];//リリース
	WORD m_ButtonState[XUSER_MAX_COUNT];//プレス

	float m_LeftTrigger[XUSER_MAX_COUNT];//左トリガー
	float m_RightTrigger[XUSER_MAX_COUNT];//右トリガー

	float m_stickLX[XUSER_MAX_COUNT];//左スティックX
	float m_stickLY[XUSER_MAX_COUNT];//左スティックY

	float m_stickRX[XUSER_MAX_COUNT];//右スティックX
	float m_stickRY[XUSER_MAX_COUNT];//右スティックY

	int m_lookpad;//見るパッド

public:
	CXInput();
	virtual ~CXInput();

	virtual bool Init(void);
	virtual void Uninit(void);
	virtual void Update(void);

	virtual float IsAxis(int nKey);
	virtual bool IsPress(int nKey);
	virtual bool IsTrigger(int nKey);
	virtual bool IsRelease(int nKey);

	inline void SetLookPad(int lookpad) { m_lookpad = lookpad; }


	void SetStatus(bool status)override;
};

