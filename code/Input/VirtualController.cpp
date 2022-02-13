#include"VirtualController.h"
#include"../main.h"



HWND VCon::m_hwnd = nullptr;

std::unordered_map<std::string, N_VIRTUALBUTTON::K_VButton> VCon::m_inputmap;

std::shared_ptr<C_KeyboardInput> VCon::m_keyboard;
std::shared_ptr<C_Mouse> VCon::m_mouse;
std::shared_ptr<CXInput> VCon::m_xinput;

bool VCon::m_MasterButtonStatus = true;

//入力デバイス作成
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

//指定したやつがInputBaceで帰ってくる
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

//初期設定
bool VCon::Init(HWND hwnd, HINSTANCE hInstance)
{
	m_hwnd = hwnd;
	C_Input::CreateInterface(hInstance);

	SetVirtualButton();
	return true;
}



//終了
bool VCon::Uninit(void)
{
	//ボタン情報削除
	for (auto i = m_inputmap.begin(); i != m_inputmap.end(); ++i)
	{
		i->second.buttonlist.clear();
	}
	m_inputmap.clear();

	C_Input::InterfaceUninit();

	return true;
}

//更新
void VCon::Update()
{
	//入力更新処理
	for (auto type : INTERFACETYPE())
	{
		C_InputBace* bace = ReturnInputDevice(type);
		if (bace != nullptr)
		{
			bace->SetStatus(m_MasterButtonStatus);
			bace->Update();
		}
	}

	//ボタンが押されているかいないか
	for (auto v = m_inputmap.begin(); v != m_inputmap.end(); ++v)
	{
		//初期化
		v->second.Press = v->second.Trigger = v->second.Release = false;
		v->second.Axis = 0.0f;

		//キー入力状態取得
		if (!m_MasterButtonStatus || !v->second.Status)
		{
			continue;
		}

		//キー状態取得＆セット
		for (auto b = v->second.buttonlist.begin(); b != v->second.buttonlist.end(); ++b)
		{
			switch (b->interfacetype)
			{
			case INTERFACETYPE::KEYBOARD://キーボード
				if (m_keyboard->IsPress(b->keycode))
				{
					v->second.Press |= true;
					v->second.Axis += b->scale;
				}
				v->second.Trigger |= m_keyboard->IsTrigger(b->keycode);
				v->second.Release |= m_keyboard->IsRelease(b->keycode);
				break;

			case INTERFACETYPE::MOUSE://マウス
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

		//AXISの数値を補正
		if (v->second.Axis >= 1.0f)//1.0より大きい場合
		{
			v->second.Axis = 1.0f;
		}

		if (v->second.Axis <= -1.0f)//-1.0より小さい場合
		{
			v->second.Axis = -1.0f;
		}
	}
}


//仮想ボタンを残したまま設定されているキーを削除
bool VCon::ClearVB(const char* name, K_KeyCode keycode)
{
	//削除するキー取得
	auto itr = m_inputmap.find(name);
	if (itr == m_inputmap.end())//見つからない場合
	{
		return false;
	}

	//指定したキーコードを削除
	for (auto itrkey = itr->second.buttonlist.begin(); itrkey != itr->second.buttonlist.end(); ++itrkey)
	{
		if (itrkey->interfacetype == keycode.interfacetype && itrkey->keycode == keycode.keycode)
		{
			itr->second.buttonlist.erase(itrkey);//指定削除
			return true;
		}
	}


	return false;
}

//指定の仮想ボタンのキー全削除
bool VCon::AllClearVB(const char* name)
{
	
	auto itr = m_inputmap.find(name);
	if (itr == m_inputmap.end())//見つからない場合
	{
		return false;
	}

	itr->second.buttonlist.clear();
	return true;
}

//仮想ボタン削除
bool VCon::DeleteVB(const char* name)
{
	m_inputmap.erase(name);
	return true;
}

//仮想ボタンを新しくセットする
void VCon::SetVB(const char* name, float scale, K_KeyCode keycode)
{
	//同じ名前がすでに登録されているか
	if (AddVB(name, scale, keycode))
	{
		return;
	}

	//新規登録
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

//仮想ボタンにキーを新しく追加する
bool VCon::AddVB(const char* name, float scale, K_KeyCode keycode)
{
	//同じ名前がすでに登録されているか
	auto itr = m_inputmap.find(name);
	if (itr == m_inputmap.end())//ない場合
	{
		return false;
	}

	//すでに指定されたキーが登録されている場合はそのまま上書き
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

//軸取得　正の値のみ
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

//ボタン押しているとき
bool VCon::GetPress(const char* ButtonName)
{
	return m_inputmap.at(ButtonName).Press ? true : false;
}
bool VCon::GetPress(K_KeyCode keycode)
{
	return GetInput(GETINPUT::PRESS, keycode);
}

//押したとき
bool VCon::GetTrigger(const char* ButtonName)
{
	return m_inputmap.at(ButtonName).Trigger ? true : false;
}
bool VCon::GetTrigger(K_KeyCode keycode)
{
	return GetInput(GETINPUT::TRIGGER, keycode);
}

//離したとき
bool VCon::GetRelease(const char* ButtonName)
{
	return m_inputmap.at(ButtonName).Release ? true : false;
}
bool VCon::GetRelease(K_KeyCode keycode)
{
	return GetInput(GETINPUT::RELEASE, keycode);
}

//マウス移動量
void VCon::GetMouseVec(LONG* x, LONG* y)
{
	m_mouse->IsVelocity(x, y);
}

//マウス座標
void VCon::GetMousePoint(LONG* x, LONG* y)
{
	m_mouse->IsPoint(x, y);
}

//マウスホイール
float VCon::GetMouseWheel(void)
{
	return m_mouse->IsWheel();
}


//入力状態取得
bool VCon::GetInput(GETINPUT getinput, K_KeyCode keycode)
{
	//入力デバイス受け取る
	C_InputBace* inputbace = ReturnInputDevice(keycode.interfacetype);

	//入力状態取得
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
	if (str != "")//指定されていいるか
	{
		m_inputmap.at(str).Status = status;
		return;
	}

	m_MasterButtonStatus = status;
}

bool VCon::GetStopStatus(const char* name)
{
	std::string str = name;
	if (str.empty())//指定されていいるか
	{
		return m_inputmap.at(str).Status;
	}

	return m_MasterButtonStatus;
}