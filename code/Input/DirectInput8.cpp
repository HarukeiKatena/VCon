#include "DirectInput8.h"

LPDIRECTINPUT8 C_Input::m_Input = nullptr;

//===============================================================
//�C���^�[�t�F�[�X�쐬
//===============================================================
bool C_Input::CreateInterface(HINSTANCE hInstance)
{
	HRESULT hr = DirectInput8Create(
		hInstance,				//�C���X�^���X�n���h��
		DIRECTINPUT_VERSION,	//�o�[�W����
		IID_IDirectInput8,		//�擾�C���^�[�t�F�[�X
		(VOID**)&m_Input,		//�C���^�[�t�F�[�X�|�C���^
		NULL					//�Ȃ�
	);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

//===============================================================
//�I��
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