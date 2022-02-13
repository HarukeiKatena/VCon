//====================================================================
//�C���v�b�g
//�Î�[�t�c
//====================================================================
#pragma once
#include<Windows.h>

#define DIRECTINPUT_VERSION (0x0800)
#include<dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class C_Input
{
protected:
	static LPDIRECTINPUT8 m_Input;

public:
	//===============================================================
	//�C���^�[�t�F�[�X�쐬
	//===============================================================
	/*
	hInstance	�C���X�^���X�n���h��
	*/
	static bool CreateInterface(HINSTANCE hInstance);

	//===============================================================
	//�I��
	//===============================================================
	/*
	�f�X�g���N�^�ŌĂ΂��
	*/
	static bool InterfaceUninit(void);

	//===============================================================
	//�C���^�[�t�F�[�X�擾
	//===============================================================
	static LPDIRECTINPUT8 GetInput(void) { return m_Input; }
};