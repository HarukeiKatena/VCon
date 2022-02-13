//==============================================================
//
//	�L�[�{�[�h�C���v�b�g
//	�Î�[�t�c
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
//�L�[�{�[�h�f�o�C�X�쐬
//==============================================================
bool C_KeyboardInput::CreateInputDevice(LPDIRECTINPUT8 input)
{
	if (FAILED(input->CreateDevice(GUID_SysKeyboard, &m_pInputDevice, NULL)))
	{
		MessageBox(NULL, "�L�[�{�[�h�f�o�C�X���쐬�ł��܂���ł���", "�G���[", MB_OK);
		return false;
	}
	return true;
}

//==============================================================
//���̓f�[�^�`���Z�b�g
//==============================================================
bool C_KeyboardInput::SetDataFormat(void)
{
	if (FAILED(m_pInputDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(NULL, "�L�[�{�[�h�̓��͌`���̃Z�b�g�Ɏ��s", "�G���[", MB_OK);
		this->Uninit();
		return false;
	}
	return true;
}

//==============================================================
//�r������
//==============================================================
bool C_KeyboardInput::SetCooperativeLevel(HWND hwnd)
{
	if (FAILED(m_pInputDevice->SetCooperativeLevel(hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(NULL, "�L�[�{�[�h�̔r������̃Z�b�g�Ɏ��s", "�G���[", MB_OK);
		this->Uninit();
		return false;
	}
	return true;
}

//==============================================================
//�����ݒ�
//==============================================================
bool C_KeyboardInput::Init(HWND hwnd, LPDIRECTINPUT8 input)
{
	//�f�o�C�X�쐬
	if (!CreateInputDevice(input))
	{
		return false;
	}

	//���͌`���Z�b�g
	if (!SetDataFormat())
	{
		return false;
	}

	//�r������Z�b�g
	if (!SetCooperativeLevel(hwnd))
	{
		return false;
	}

	//����J�n
	m_pInputDevice->Acquire();

	return true;
}

//==============================================================
//�X�V
//==============================================================
void C_KeyboardInput::Update(void)
{
	BYTE KeyState[KEY_MAX];

	//�f�[�^�擾
	if (SUCCEEDED(m_pInputDevice->GetDeviceState(sizeof(KeyState), KeyState)))
	{
		if (!mStatus)//false�̎����ׂ�0�ɂ���
		{
			ZeroMemory(&KeyState, sizeof(BYTE) * KEY_MAX);
		}

		for (int CnKey = 0; CnKey < KEY_MAX; CnKey++)
		{
			//�g���K�[�ƃ����[�X
			m_KeyStateTrigger[CnKey] = (m_KeyState[CnKey] ^ KeyState[CnKey]) & KeyState[CnKey];
			m_KeyStateRelease[CnKey] = (m_KeyState[CnKey] ^ KeyState[CnKey]) & m_KeyState[CnKey];

			//�v���X
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
//���͏�Ԏ擾
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