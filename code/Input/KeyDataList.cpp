#include"VirtualController.h"

//===============================================================
//���z�{�^����ݒ肷��֐�
//===============================================================
void VCon::SetVirtualButton(void)
{
	/*
	�g������
	�EKeyCode�Őݒ肷����@

	//�������O�Őݒ肷��Ɛݒ肵���ǂ��炩�̃{�^����������Ă����ꍇtrue���Ԃ��Ă���
	//Axis�̏ꍇ�͑�����(scale)�Őݒ肵���l���Ԃ��Ă���
	//������(scale)�͈�ԍŏ��Ɏw�肵���l���ݒ肳���
	SetVB("Jump", 1.0f, KeyCode::W());
	SetVB("Jump", 1.0f, KeyCode::UpArrow());

	//�������ɂ͕��̒l������
	SetVB("Left", -1.0f, KeyCode::LeftArrow());

	//�L�[�̒ǉ��͉��ł��ł���@���������z�{�^�������݂��Ȃ��ꍇ�͒ǉ�����Ȃ�
	//��xSetVB�œo�^���Ă����Ȃ��Ƃ����Ȃ�
	AddVB("Left", -1.0f, KeyCode::A());

	//XINPUT�ŃX�e�B�b�N���g���ꍇ
	//scale�͉��ł�����
	SetVB("Stick", 0.0f, KeyCode::XInputAxisStickLX());

	//��
	//�L�[�𕡐��ݒ肷���scale�����ꂼ��قȂ�ꍇ�͓��͂���Ă����������Z����-1.0�`1.0�̊Ԃŕ␳�����
	SetVB("Vertical", 1.0f, KeyCode::W());//��
	AddVB("Vertical", -1.0f, KeyCode::S());//��
	AddVB("Vertical", 0.0f, KeyCode::XInputAxisStickLY());//�X�e�B�b�N
	*/

	//�����ɍ쐬���������z�{�^�����Z�b�g����
	{
		
	}

	//�悭�g���C�����鉼�z�{�^���Z�b�g(�����Ă�����)
	{
		
		//�ړ�
		SetVB("Up", 1.0f, KeyCode::W());

		SetVB("Down", -1.0f, KeyCode::S());

		SetVB("Left", -1.0f, KeyCode::A());

		SetVB("Right", 1.0f, KeyCode::D());

		//�W�����v
		SetVB("Jump", 1.0f, KeyCode::Space());

		//�J�����ړ�
		//����]
		SetVB("CameraSide", 1.0f, KeyCode::RightArrow());//�E
		AddVB("CameraSide", -1.0f, KeyCode::LeftArrow());//��
		AddVB("CameraSide", 0.0f, KeyCode::XInputAxisStickRX());//�X�e�B�b�N

		SetVB("CameraVertical", 1.0f, KeyCode::UpArrow());//��
		AddVB("CameraVertical", -1.0f, KeyCode::DownArrow());//��
		AddVB("CameraVertical", 0.0f, KeyCode::XInputAxisStickRY());//�X�e�B�b�N
		
	}

	//���z�{�^�����g�킸KeyCode�����g�������ꍇ�͂�����
	//CreateInputDevice(INTERFACETYPE)�œo�^�o����
	{
		CreateInputDevice(INTERFACETYPE::KEYBOARD);
		CreateInputDevice(INTERFACETYPE::XINPUT);
	}
}