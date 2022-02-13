//================================================================
// 
// �}�E�X(DirectInput)
// �Î�[�t�c
// 
//================================================================
#pragma once
#include"DirectInputBace.h"

#define DIM_LEFT (0)	//���N���b�N
#define DIM_RIGHT (1)	//�E�N���b�N
#define DIM_SENTER (2)	//�����N���b�N

class C_Mouse : public CDirectInputBace
{
private:
	HWND m_hwnd;

	LONG m_lx;
	LONG m_ly;
	LONG m_wheel;

	LONG m_x;
	LONG m_y;

	static const int BUTTONNUM = 4;
	BYTE m_StatePress[BUTTONNUM];
	BYTE m_StateTrigger[BUTTONNUM];
	BYTE m_StateRelease[BUTTONNUM];

	virtual bool CreateInputDevice(LPDIRECTINPUT8 input);
	virtual bool SetDataFormat(void);
	virtual bool SetCooperativeLevel(HWND hwnd);

public:

	C_Mouse();
	virtual ~C_Mouse();

	virtual bool Init(HWND hwnd, LPDIRECTINPUT8 input);
	virtual void Update(void);

	virtual void IsVelocity(LONG* x, LONG* y);
	virtual LONG IsVelocityX(void);
	virtual LONG IsVelocityY(void);

	virtual void IsPoint(LONG* x, LONG* y);
	virtual LONG IsPointX(void);
	virtual LONG IsPointY(void);

	/*
	�}�E�X�z�C�[��
	��ɉ񂵂���@1.0f
	���ɉ񂵂���  -1.0f
	�񂵂ĂȂ������� 0.0f
	*/
	virtual float IsWheel(void);

	virtual bool IsPress(int nKey);
	virtual bool IsTrigger(int nKey);
	virtual bool IsRelease(int nKey);
};