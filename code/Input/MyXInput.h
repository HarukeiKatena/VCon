//=============================================================
//XInput
//�Î�[�t�c
//=============================================================
#pragma once
#include<Windows.h>


#include<XInput.h>
#pragma comment(lib,"xinput.lib")

#include "InputBace.h"

namespace XINPUT
{
	//�R���g���[���ԍ�
	enum CONTROLLER
	{
		GAMEPAD01,
		GAMEPAD02,
		GAMEPAD03,
		GAMEPAD04,
	};

	//�Q�[���p�b�h����
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

	WORD m_ButtonStateTrigger[XUSER_MAX_COUNT];//�g���K�[
	WORD m_ButtonStateRelease[XUSER_MAX_COUNT];//�����[�X
	WORD m_ButtonState[XUSER_MAX_COUNT];//�v���X

	float m_LeftTrigger[XUSER_MAX_COUNT];//���g���K�[
	float m_RightTrigger[XUSER_MAX_COUNT];//�E�g���K�[

	float m_stickLX[XUSER_MAX_COUNT];//���X�e�B�b�NX
	float m_stickLY[XUSER_MAX_COUNT];//���X�e�B�b�NY

	float m_stickRX[XUSER_MAX_COUNT];//�E�X�e�B�b�NX
	float m_stickRY[XUSER_MAX_COUNT];//�E�X�e�B�b�NY

	int m_lookpad;//����p�b�h

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

