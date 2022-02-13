//==============================================================
//
//	�L�[�{�[�h�C���v�b�g
//	�Î�[�t�c
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
	//�L�[�{�[�h�f�o�C�X�쐬
	//==============================================================
	virtual bool CreateInputDevice(LPDIRECTINPUT8 input);

	//==============================================================
	//���̓f�[�^�`���Z�b�g
	//==============================================================
	virtual bool SetDataFormat(void);

	//==============================================================
	//�r������
	//==============================================================
	virtual bool SetCooperativeLevel(HWND hwnd);

public:

	C_KeyboardInput();
	virtual ~C_KeyboardInput();

	//==============================================================
	//�����ݒ�
	//==============================================================
	/*
	hInstance	�C���X�^���X�n���h��
	hwnd		�E�B���h�E�n���h��
	*/
	virtual bool Init(HWND hwnd, LPDIRECTINPUT8 input);

	//==============================================================
	//�X�V
	//==============================================================
	virtual void Update(void);

	//==============================================================
	//���͏�Ԏ擾
	//==============================================================
	/*
	�߂�l�Ŏw�肵���L�[��
	Press	������Ă���
	Trigger	�������u��
	Release	�������u��
	�̏ꍇtrue��Ԃ�
	*/
	virtual bool IsPress(int nKey);
	virtual bool IsTrigger(int nKey);
	virtual bool IsRelease(int nKey);
};
