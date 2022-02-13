#include"VirtualController.h"
#include"../main.h"



HWND VCon::m_hwnd = nullptr;

std::unordered_map<std::string, N_VIRTUALBUTTON::K_VButton> VCon::m_inputmap;

std::shared_ptr<C_KeyboardInput> VCon::m_keyboard;
std::shared_ptr<C_Mouse> VCon::m_mouse;
std::shared_ptr<CXInput> VCon::m_xinput;

bool VCon::m_MasterButtonStatus = true;

//���̓f�o�C�X�쐬
void VCon::CreateInputDevice(INTERFACETYPE inputtype)
{
	switch (inputtype)
	{
	case INTERFACETYPE::KEYBOARD:
		m_keyboard = std::make_shared<C_KeyboardInput>();
		m_keyboard->Init(m_hwnd, C_Input::GetInput());
		break;

	case INTERFACETYPE::MOUSE:
		m_mouse = std::make_shared<C_Mouse>();
		m_mouse->Init(m_hwnd, C_Input::GetInput());
		break;

	case INTERFACETYPE::GAMEPAD:
		break;

	case INTERFACETYPE::XINPUT:
		m_xinput = std::make_shared<CXInput>();
		m_xinput->Init();
		break;
	}
}

//�w�肵�����InputBace�ŋA���Ă���
C_InputBace* VCon::ReturnInputDevice(INTERFACETYPE inputtype)
{
	switch (inputtype)
	{
	case INTERFACETYPE::KEYBOARD:	return m_keyboard.get();
	case INTERFACETYPE::MOUSE:		return m_mouse.get();
	case INTERFACETYPE::GAMEPAD:	return nullptr;
	case INTERFACETYPE::XINPUT:		return m_xinput.get();
	default:					return nullptr;
	}
}

//�����ݒ�
bool VCon::Init(HWND hwnd, HINSTANCE hInstance)
{
	m_hwnd = hwnd;
	C_Input::CreateInterface(hInstance);

	SetVirtualButton();
	return true;
}



//�I��
bool VCon::Uninit(void)
{
	//�{�^�����폜
	for (auto i = m_inputmap.begin(); i != m_inputmap.end(); ++i)
	{
		i->second.buttonlist.clear();
	}
	m_inputmap.clear();

	C_Input::InterfaceUninit();

	return true;
}

//�X�V
void VCon::Update()
{
	//���͍X�V����
	for (auto type : INTERFACETYPE())
	{
		C_InputBace* bace = ReturnInputDevice(type);
		if (bace != nullptr)
		{
			bace->SetStatus(m_MasterButtonStatus);
			bace->Update();
		}
	}

	//�{�^����������Ă��邩���Ȃ���
	for (auto v = m_inputmap.begin(); v != m_inputmap.end(); ++v)
	{
		//������
		v->second.Press = v->second.Trigger = v->second.Release = false;
		v->second.Axis = 0.0f;

		//�L�[���͏�Ԏ擾
		if (!m_MasterButtonStatus || !v->second.Status)
		{
			continue;
		}

		//�L�[��Ԏ擾���Z�b�g
		for (auto b = v->second.buttonlist.begin(); b != v->second.buttonlist.end(); ++b)
		{
			switch (b->interfacetype)
			{
			case INTERFACETYPE::KEYBOARD://�L�[�{�[�h
				if (m_keyboard->IsPress(b->keycode))
				{
					v->second.Press |= true;
					v->second.Axis += b->scale;
				}
				v->second.Trigger |= m_keyboard->IsTrigger(b->keycode);
				v->second.Release |= m_keyboard->IsRelease(b->keycode);
				break;

			case INTERFACETYPE::MOUSE://�}�E�X
				if (m_mouse->IsPress(b->keycode))
				{
					v->second.Press |= true;
					v->second.Axis += b->scale;
				}
				v->second.Trigger |= m_mouse->IsTrigger(b->keycode);
				v->second.Release |= m_mouse->IsRelease(b->keycode);
				break;

			case INTERFACETYPE::XINPUT://XINPUT
				if (b->inputtype == INPUTTYPE::AXIS)
				{
					v->second.Axis += m_xinput->IsAxis(b->keycode);
					break;
				}

				if (m_xinput->IsPress(b->keycode))
				{
					v->second.Press |= true;
					v->second.Axis += b->scale;
				}
				v->second.Trigger |= m_xinput->IsTrigger(b->keycode);
				v->second.Release |= m_xinput->IsRelease(b->keycode);
				break;

			default:
				break;
			}
		}

		//AXIS�̐��l��␳
		if (v->second.Axis >= 1.0f)//1.0���傫���ꍇ
		{
			v->second.Axis = 1.0f;
		}

		if (v->second.Axis <= -1.0f)//-1.0��菬�����ꍇ
		{
			v->second.Axis = -1.0f;
		}
	}
}


//���z�{�^�����c�����܂ܐݒ肳��Ă���L�[���폜
bool VCon::ClearVB(const char* name, K_KeyCode keycode)
{
	//�폜����L�[�擾
	auto itr = m_inputmap.find(name);
	if (itr == m_inputmap.end())//������Ȃ��ꍇ
	{
		return false;
	}

	//�w�肵���L�[�R�[�h���폜
	for (auto itrkey = itr->second.buttonlist.begin(); itrkey != itr->second.buttonlist.end(); ++itrkey)
	{
		if (itrkey->interfacetype == keycode.interfacetype && itrkey->keycode == keycode.keycode)
		{
			itr->second.buttonlist.erase(itrkey);//�w��폜
			return true;
		}
	}


	return false;
}

//�w��̉��z�{�^���̃L�[�S�폜
bool VCon::AllClearVB(const char* name)
{
	
	auto itr = m_inputmap.find(name);
	if (itr == m_inputmap.end())//������Ȃ��ꍇ
	{
		return false;
	}

	itr->second.buttonlist.clear();
	return true;
}

//���z�{�^���폜
bool VCon::DeleteVB(const char* name)
{
	m_inputmap.erase(name);
	return true;
}

//���z�{�^����V�����Z�b�g����
void VCon::SetVB(const char* name, float scale, K_KeyCode keycode)
{
	//�������O�����łɓo�^����Ă��邩
	if (AddVB(name, scale, keycode))
	{
		return;
	}

	//�V�K�o�^
	K_KeyCode setkey = keycode;
	setkey.scale = scale;

	K_VButton vbset;
	vbset.scale = 0.0f;
	vbset.buttonlist.push_back(setkey);
	vbset.Press = false;
	vbset.Trigger = false;
	vbset.Release = false;
	vbset.Axis = 0.0f;

	m_inputmap[name] = vbset;

	CreateInputDevice(keycode.interfacetype);
}

//���z�{�^���ɃL�[��V�����ǉ�����
bool VCon::AddVB(const char* name, float scale, K_KeyCode keycode)
{
	//�������O�����łɓo�^����Ă��邩
	auto itr = m_inputmap.find(name);
	if (itr == m_inputmap.end())//�Ȃ��ꍇ
	{
		return false;
	}

	//���łɎw�肳�ꂽ�L�[���o�^����Ă���ꍇ�͂��̂܂܏㏑��
	K_KeyCode setkey = keycode;
	setkey.scale = scale;
	for (auto itrkey = itr->second.buttonlist.begin(); itrkey != itr->second.buttonlist.end(); ++itrkey)
	{
		if (itrkey->interfacetype != setkey.interfacetype || itrkey->keycode != setkey.keycode)
		{
			itr->second.buttonlist.push_back(setkey);
			break;
		}
	}
	return true;
}

//���擾�@���̒l�̂�
float VCon::GetAxis(const char* ButtonName)
{
	return m_inputmap.at(ButtonName).Axis;
}
float VCon::GetAxis(K_KeyCode keycode)
{
	if (keycode.interfacetype == INTERFACETYPE::XINPUT && keycode.inputtype == INPUTTYPE::AXIS)
	{
		return m_xinput->IsAxis(keycode.keycode);
	}
	return GetInput(GETINPUT::PRESS, keycode) ? 1.0f : 0.0f;
}

//�{�^�������Ă���Ƃ�
bool VCon::GetPress(const char* ButtonName)
{
	return m_inputmap.at(ButtonName).Press ? true : false;
}
bool VCon::GetPress(K_KeyCode keycode)
{
	return GetInput(GETINPUT::PRESS, keycode);
}

//�������Ƃ�
bool VCon::GetTrigger(const char* ButtonName)
{
	return m_inputmap.at(ButtonName).Trigger ? true : false;
}
bool VCon::GetTrigger(K_KeyCode keycode)
{
	return GetInput(GETINPUT::TRIGGER, keycode);
}

//�������Ƃ�
bool VCon::GetRelease(const char* ButtonName)
{
	return m_inputmap.at(ButtonName).Release ? true : false;
}
bool VCon::GetRelease(K_KeyCode keycode)
{
	return GetInput(GETINPUT::RELEASE, keycode);
}

//�}�E�X�ړ���
void VCon::GetMouseVec(LONG* x, LONG* y)
{
	m_mouse->IsVelocity(x, y);
}

//�}�E�X���W
void VCon::GetMousePoint(LONG* x, LONG* y)
{
	m_mouse->IsPoint(x, y);
}

//�}�E�X�z�C�[��
float VCon::GetMouseWheel(void)
{
	return m_mouse->IsWheel();
}


//���͏�Ԏ擾
bool VCon::GetInput(GETINPUT getinput, K_KeyCode keycode)
{
	//���̓f�o�C�X�󂯎��
	C_InputBace* inputbace = ReturnInputDevice(keycode.interfacetype);

	//���͏�Ԏ擾
	switch (getinput)
	{
	case VCon::GETINPUT::PRESS: return inputbace->IsPress(keycode.keycode) ? true : false;
	case VCon::GETINPUT::TRIGGER: return inputbace->IsTrigger(keycode.keycode) ? true : false;
	case VCon::GETINPUT::RELEASE: return inputbace->IsRelease(keycode.keycode) ? true : false;
	default: return false;
	}
}


void VCon::SetStopStatus(const char* name, bool status)
{
	std::string str = name;
	if (str != "")//�w�肳��Ă����邩
	{
		m_inputmap.at(str).Status = status;
		return;
	}

	m_MasterButtonStatus = status;
}

bool VCon::GetStopStatus(const char* name)
{
	std::string str = name;
	if (str.empty())//�w�肳��Ă����邩
	{
		return m_inputmap.at(str).Status;
	}

	return m_MasterButtonStatus;
}