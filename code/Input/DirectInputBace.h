#pragma once
#include"InputBace.h"
#include"DirectInput8.h"

class CDirectInputBace : public C_InputBace
{
protected:
	//���̓f�o�C�X
	LPDIRECTINPUTDEVICE8 m_pInputDevice;

	//�쐬�֐��Ȃ�
	virtual bool CreateInputDevice(LPDIRECTINPUT8 input) { input; return false; }
	virtual bool SetDataFormat(void) { return false; }
	virtual bool SetCooperativeLevel(HWND hwnd) { hwnd; return false; }
public:
	CDirectInputBace() : m_pInputDevice(nullptr) {}
	virtual ~CDirectInputBace()
	{
		if (m_pInputDevice != nullptr)
		{
			m_pInputDevice->Unacquire();
			m_pInputDevice->Release();
			m_pInputDevice = nullptr;
		}
	}

	virtual bool Init(void) { return false; }
	virtual void Uninit(void) {}
	virtual void Update(void) = 0;

	virtual bool IsPress(int nKey) = 0;
	virtual bool IsTrigger(int nKey) = 0;
	virtual bool IsRelease(int nKey) = 0;
};

