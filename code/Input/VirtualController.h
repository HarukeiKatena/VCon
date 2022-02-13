//===============================================================
//
//	�C���v�b�g�}�l�[�W���[
//	�Î�[�t�c
//
//===============================================================
/*
�g����

�E�ŏ��̏���
//��p��SetVirtualButton�֐��Ȃ��Ɏg�p���鉼�z�{�^�����Z�b�g����
//��p�̊֐����ŉ��z�{�^���̃Z�b�g�����Ȃ��ꍇ��Init�̑O�ɉ��z�{�^�����Z�b�g����
C_VController::SetVB(const char*, K_KeyCode, float);
C_VController::AddVB(const char*, K_KeyCode);

//����������
C_VController::Init(HWND, HINSTANCE);

//�X�V�����̈�ԏ�ɏ���
C_VController::Update();

//�I�������̂Ƃ���ɏ���
C_VController::Clear();
*/
#pragma once
#include<string>
#include<list>
#include<unordered_map>
#include<memory>

#include"DirectInput8.h"
#include"KeyboardInput.h"
#include"Mouse.h"
#include"MyXInput.h"

#pragma warning(push)
#pragma warning(disable:4100)

#define GENERATE_ENUM_ITERATOR(T) \
inline T operator++(T& x) { return x = (T)(std::underlying_type<T>::type(x) + 1); } \
inline T operator*(T c) { return c; } \
inline T begin(T r) { return static_cast<T>(0); } \
inline T end(T r) { T l = T::MAX; return l; }
enum class INTERFACETYPE
{
	KEYBOARD,	//DirectInput
	MOUSE,		//DirectInput
	GAMEPAD,	//DirectInput
	XINPUT,		//XInput
	MAX
};
GENERATE_ENUM_ITERATOR(INTERFACETYPE)

enum class INPUTTYPE
{
	BUTTON,
	AXIS,
};

namespace N_VIRTUALBUTTON
{
	//�{�^�����
	struct K_KeyCode
	{
		INTERFACETYPE interfacetype;//�C���^�[�t�F�[�X�̎��
		int keycode;//�L�[�R�[�h
		INPUTTYPE inputtype;//���͂̎��
		float scale;//�L�[���Ƃɐݒ肷�镝

	public:
		K_KeyCode() : interfacetype(INTERFACETYPE::MAX), keycode(0), inputtype(INPUTTYPE::BUTTON), scale(0.0f) {}
		K_KeyCode(const K_KeyCode& code) : interfacetype(code.interfacetype), keycode(code.keycode), inputtype(code.inputtype), scale(code.scale) {}
		K_KeyCode(const K_KeyCode* code) : interfacetype(code->interfacetype), keycode(code->keycode), inputtype(code->inputtype), scale(code->scale) {}
		K_KeyCode(INTERFACETYPE type, int code, INPUTTYPE _inputtype = INPUTTYPE::BUTTON) : interfacetype(type), keycode(code), inputtype(_inputtype), scale(0.0f) {}
	};

	//���z�{�^��
	struct K_VButton
	{
		float scale;//��
		//std::map<int, K_KeyCode> buttonmap;//key�ɃL�[�R�[�hID  valve�ɃL�[�R�[�h�f�[�^
		std::list<K_KeyCode> buttonlist;

		bool Press;		//�����Ă�Ƃ�
		bool Trigger;	//�������Ƃ�
		bool Release;	//�������Ƃ�
		float Axis;

		bool Status;

		K_VButton() :
			scale(0.0f), 
			Press(false), 
			Trigger(false), 
			Release(false), 
			Axis(0.0f), 
			Status(true) 
		{}
	};
}


//===============================================================
//���̓}�l�[�W���[
//===============================================================
class VCon
{
private:
	using K_KeyCode = N_VIRTUALBUTTON::K_KeyCode;
	using K_VButton = N_VIRTUALBUTTON::K_VButton;

	enum class GETINPUT
	{
		PRESS,
		TRIGGER,
		RELEASE,
	};
private:
	static HWND m_hwnd;

	static std::unordered_map<std::string, K_VButton> m_inputmap;

	static std::shared_ptr<C_KeyboardInput> m_keyboard;
	static std::shared_ptr<C_Mouse> m_mouse;
	static std::shared_ptr<CXInput> m_xinput;

	static bool m_MasterButtonStatus;//���ׂẴ{�^������

	//===============================================================
	//���z�{�^����ݒ肷��֐�
	//===============================================================
	static void SetVirtualButton(void);

	//===============================================================
	//���̓f�o�C�X�쐬
	//===============================================================
	static void CreateInputDevice(INTERFACETYPE inputtype);

	//===============================================================
	//�w�肵�����InputBace�ŋA���Ă���
	//===============================================================
	static C_InputBace* ReturnInputDevice(INTERFACETYPE inputtype);

	//===============================================================
	//���͏�Ԏ擾
	//===============================================================
	static bool GetInput(GETINPUT getinput, K_KeyCode keycode);
public:
	//===============================================================
	//�R���X�g���N�^�f�X�g���N�^
	//===============================================================
	VCon();
	~VCon();

	//===============================================================
	//�����ݒ�
	//===============================================================
	static bool Init(HWND hwnd, HINSTANCE hinstance);

	//===============================================================
	//�I������
	//===============================================================
	static bool Uninit();

	//===============================================================
	//�X�V
	//===============================================================
	static void Update(void);


	//===============================================================
	//���z�{�^�����c�����܂ܐݒ肳��Ă���L�[���폜
	//===============================================================
	/*
	�߂�l�@�w�肵�����O�L�[�R�[�h���ݒ肳��Ă��Ȃ��ꍇfalse
	name	�ݒ肳��Ă���L�[�R�[�h���폜���������z�{�^���̖��O
	keycode	�w�肵���L�[�R�[�h�������폜����
	allclear �S�폜����ꍇ�͂�����-�P�ɂ���
	*/
	static bool ClearVB(const char* name, K_KeyCode keycode);
	static bool AllClearVB(const char* name);
	static bool DeleteVB(const char* name);


	//===============================================================
	//���z�{�^���Z�b�g�֐�
	//===============================================================
	/*
	name	���O
	scale	��
	button	�{�^�����
	*/
	static void SetVB(const char* name, float scale, K_KeyCode button);

	//===============================================================
	//���z�{�^���ɃL�[��V�����ǉ�����
	//===============================================================
	/*
	�߂�l �w�肵�����z�{�^�����Ȃ��ꍇ��false
	name	�L�[��ǉ����������z�{�^��
	keycode	�L�[�R�[�h
	*/
	static bool AddVB(const char* name, float scale, K_KeyCode keycode);

	//===============================================================
	//�{�^���̓��͏�Ԑ��䁕�擾
	//===============================================================
	/*
	name	��Ԃ�ύX�������L�[(""�̏ꍇ��MasterFlag�𐧌䂷��)
	status	�ύX�����ԁ@true�œ��쒆�@false�œ����~

	�EGet
	�߂�l�@�w�肵���L�[�̏��(""�̏ꍇ��MasterFlag���擾����)
	*/
	static void SetStopStatus(const char* name, bool status);
	static bool GetStopStatus(const char* name = "");

	//===============================================================
	//��
	//===============================================================
	/*
	���Ŏ󂯎��
	Scale�̕���
	GetAxis��0~Scale
	*/
	static float GetAxis(const char* ButtonName);
	static float GetAxis(K_KeyCode keycode);

	//===============================================================
	//�擾
	//===============================================================
	static bool GetPress(const char* ButtonName);
	static bool GetTrigger(const char* ButtonName);
	static bool GetRelease(const char* ButtonName);
	
	static bool GetPress(K_KeyCode keycode);
	static bool GetTrigger(K_KeyCode keycode);
	static bool GetRelease(K_KeyCode keycode);
	
	static void GetMouseVec(LONG* x, LONG* y);
	static void GetMousePoint(LONG* x, LONG* y);
	static float GetMouseWheel(void);
};

//�L�[�R�[�h�\
class KeyCode
{
private:
	using K_KeyCode = N_VIRTUALBUTTON::K_KeyCode;

public:

	//===============================================================
	//�L�[�{�[�h
	//===============================================================
	//�L�[�{�[�h�̏�̐���0~9
	static K_KeyCode Alpha1() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_1); }
	static K_KeyCode Alpha2() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_2); }
	static K_KeyCode Alpha3() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_3); }
	static K_KeyCode Alpha4() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_4); }
	static K_KeyCode Alpha5() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_5); }
	static K_KeyCode Alpha6() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_6); }
	static K_KeyCode Alpha7() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_7); }
	static K_KeyCode Alpha8() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_8); }
	static K_KeyCode Alpha9() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_9); }
	static K_KeyCode Alpha0() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_0); }

	//�A���t�@�x�b�gA~Z
	static K_KeyCode A() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_A); }
	static K_KeyCode B() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_B); }
	static K_KeyCode C() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_C); }
	static K_KeyCode D() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_D); }
	static K_KeyCode E() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_E); }
	static K_KeyCode F() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F); }
	static K_KeyCode G() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_G); }
	static K_KeyCode H() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_H); }
	static K_KeyCode I() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_I); }
	static K_KeyCode J() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_J); }
	static K_KeyCode K() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_K); }
	static K_KeyCode L() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_L); }
	static K_KeyCode M() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_M); }
	static K_KeyCode N() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_N); }
	static K_KeyCode O() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_O); }
	static K_KeyCode P() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_P); }
	static K_KeyCode Q() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_Q); }
	static K_KeyCode R() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_R); }
	static K_KeyCode S() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_S); }
	static K_KeyCode T() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_T); }
	static K_KeyCode U() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_U); }
	static K_KeyCode V() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_V); }
	static K_KeyCode W() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_W); }
	static K_KeyCode X() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_X); }
	static K_KeyCode Y() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_Y); }
	static K_KeyCode Z() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_Z); }

	//F1~15
	static K_KeyCode F1() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F1); }
	static K_KeyCode F2() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F2); }
	static K_KeyCode F3() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F3); }
	static K_KeyCode F4() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F4); }
	static K_KeyCode F5() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F5); }
	static K_KeyCode F6() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F6); }
	static K_KeyCode F7() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F7); }
	static K_KeyCode F8() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F8); }
	static K_KeyCode F9() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F9); }
	static K_KeyCode F10() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F10); }
	static K_KeyCode F11() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F11); }
	static K_KeyCode F12() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F12); }
	static K_KeyCode F13() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F13); }
	static K_KeyCode F14() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F14); }
	static K_KeyCode F15() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_F15); }

	//���
	static K_KeyCode UpArrow() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_UP); }
	static K_KeyCode DownArrow() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_DOWN); }
	static K_KeyCode LeftArrow() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_LEFT); }
	static K_KeyCode RightArrow() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_RIGHT); }

	//Esc�L�[
	static K_KeyCode Escape() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_ESCAPE); }

	//-(�}�C�i�X�L�[)
	static K_KeyCode Minus() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_MINUS); }

	//=(�C�R�[���L�[)
	static K_KeyCode Equals() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_EQUALS); }

	//BackSpace
	static K_KeyCode BackSpace() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_BACK); }

	//Tab
	static K_KeyCode Tab() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_TAB); }

	//[](�p������)
	static K_KeyCode LBracket() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_LBRACKET); }
	static K_KeyCode RBracket() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_RBRACKET); }

	//Enter
	static K_KeyCode Return() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_RETURN); }

	//Ctrl(���E)
	static K_KeyCode LControl() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_LCONTROL); }
	static K_KeyCode RControl() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_RCONTROL); }

	//Shift(���E)
	static K_KeyCode LShift() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_LSHIFT); }
	static K_KeyCode RShift() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_RSHIFT); }

	//Alt(���E)
	static K_KeyCode LAlt() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_LALT); }
	static K_KeyCode RAlt() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_RALT); }

	//Windows
	static K_KeyCode LWindows() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_LWIN); }
	static K_KeyCode RWindows() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_RWIN); }

	//;(�Z�~�R����)
	static K_KeyCode Semicolon() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_SEMICOLON); }

	//'(�A�|�X�g���t�B)
	static K_KeyCode Apostrophe() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_APOSTROPHE); }

	//�_(�o�b�N�X���b�V��)
	static K_KeyCode BackSlash() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_BACKSLASH); }

	//,(�R���})
	static K_KeyCode Comma() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_COMMA); }

	//.(�s���I�h)
	static K_KeyCode Period() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_PERIOD); }

	// /(�X���b�V��)
	static K_KeyCode Slash() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_SLASH); }

	//Space
	static K_KeyCode Space() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_SPACE); }

	//CapsLock
	static K_KeyCode CapsLock() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_CAPITAL); }

	//NumLock
	static K_KeyCode NumLock() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMLOCK); }

	//ScrollLock
	static K_KeyCode ScrollLock() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_SCROLL); }

	//�J�i
	static K_KeyCode KANA() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_KANA); }

	//�ϊ�
	static K_KeyCode Convert() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_CONVERT); }

	//���ϊ�
	static K_KeyCode NoConvert() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NOCONVERT); }

	//\(������)
	static K_KeyCode Yen() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_YEN); }

	//^
	static K_KeyCode Caret() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_CIRCUMFLEX); }

	//@
	static K_KeyCode At() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_AT); }

	//:
	static K_KeyCode Colon() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_COLON); }

	//_
	static K_KeyCode UnderLine() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_UNDERLINE); }

	//����
	static K_KeyCode KANJI() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_KANJI); }

	//Stop
	static K_KeyCode Stop() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_STOP); }

	//(JAPAN�@AX)
	static K_KeyCode JapanAX() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_AX); }

	//(J3100)
	static K_KeyCode Unlabeled() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_UNLABELED); }

	//Sys Rq
	static K_KeyCode SysRq() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_SYSRQ); }

	//Pause
	static K_KeyCode Pause() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_PAUSE); }

	//Home
	static K_KeyCode Home() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_HOME); }

	//PageUpDown
	static K_KeyCode PageUp() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_PRIOR); }
	static K_KeyCode PageDown() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NEXT); }

	//End
	static K_KeyCode End() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_END); }

	//Insert
	static K_KeyCode Insett() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_INSERT); }

	//Delete
	static K_KeyCode Delete() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_DELETE); }

	//Menu
	static K_KeyCode Menu() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_APPS); }

	//Power
	static K_KeyCode Power() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_POWER); }

	//Sleep
	static K_KeyCode Sleep() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_SLEEP); }

	//Numpad 0~9
	static K_KeyCode Numpad0() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPAD0); }
	static K_KeyCode Numpad1() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPAD1); }
	static K_KeyCode Numpad2() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPAD2); }
	static K_KeyCode Numpad3() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPAD3); }
	static K_KeyCode Numpad4() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPAD4); }
	static K_KeyCode Numpad5() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPAD5); }
	static K_KeyCode Numpad6() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPAD6); }
	static K_KeyCode Numpad7() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPAD7); }
	static K_KeyCode Numpad8() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPAD8); }
	static K_KeyCode Numpad9() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPAD9); }

	//*(Numpad)
	static K_KeyCode NumpadAsterisk() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_MULTIPLY); }

	//.(Numpad)
	static K_KeyCode NumpadPeriod() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_DECIMAL); }

	//Enter(Numpad)
	static K_KeyCode NumpadEnter() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPADENTER); }

	//,(Numpad)
	static K_KeyCode NumpadComma() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPADCOMMA); }

	// /(�X���b�V��Numpad)
	static K_KeyCode NumpadSlash() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_DIVIDE); }

	//=(Numpad)
	static K_KeyCode NumpadEquals() { return K_KeyCode(INTERFACETYPE::KEYBOARD, DIK_NUMPADEQUALS); }


	//===============================================================
	//�}�E�X
	//===============================================================
	//���N���b�N
	static K_KeyCode MouseLeft() { return K_KeyCode(INTERFACETYPE::MOUSE, DIM_LEFT); }

	//�E�N���b�N
	static K_KeyCode MouseRight() { return K_KeyCode(INTERFACETYPE::MOUSE, DIM_RIGHT); }

	//�z�C�[���N���b�N
	static K_KeyCode MouseSenter() { return K_KeyCode(INTERFACETYPE::MOUSE, DIM_SENTER); }


	//===============================================================
	//XInput
	//===============================================================
	//�\���L�[
	static K_KeyCode XInputDPadUp() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_DPAD_UP); }
	static K_KeyCode XInputDPadDown() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_DPAD_DOWN); }
	static K_KeyCode XInputDPadLeft() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_DPAD_LEFT); }
	static K_KeyCode XInputDPadRight() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_DPAD_RIGHT); }

	//�X�^�[�g
	static K_KeyCode XInputStart() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_START); }

	//�o�b�N
	static K_KeyCode XInputBack() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_BACK); }

	//�X�e�B�b�N��������
	static K_KeyCode XInputStickLeft() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_LEFT_THUMB); }
	static K_KeyCode XInputStickRitht() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_RIGHT_THUMB); }

	//LB�@RB
	static K_KeyCode XInputLeftB() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_LEFT_SHOULDER); }
	static K_KeyCode XInputRightB() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_RIGHT_SHOULDER); }

	//�F�{�^��(�����{�^��)
	static K_KeyCode XInputA() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_A); }
	static K_KeyCode XInputB() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_B); }
	static K_KeyCode XInputX() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_X); }
	static K_KeyCode XInputY() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT_GAMEPAD_Y); }

	//Axis�L�[�R�[�h�R���t�B�O
	static K_KeyCode XInputAxisLeftTrigger() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT::LRFTTRIGGER, INPUTTYPE::AXIS); }
	static K_KeyCode XInputAxisRightTrigger() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT::LRFTTRIGGER, INPUTTYPE::AXIS); }
	static K_KeyCode XInputAxisStickLX() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT::STICKLX, INPUTTYPE::AXIS); }
	static K_KeyCode XInputAxisStickLY() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT::STICKLY, INPUTTYPE::AXIS); }
	static K_KeyCode XInputAxisStickRX() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT::STICKRX, INPUTTYPE::AXIS); }
	static K_KeyCode XInputAxisStickRY() { return K_KeyCode(INTERFACETYPE::XINPUT, XINPUT::STICKRY, INPUTTYPE::AXIS); }
};

#pragma warning(pop)
