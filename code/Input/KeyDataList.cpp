#include"VirtualController.h"

//===============================================================
//仮想ボタンを設定する関数
//===============================================================
void VCon::SetVirtualButton(void)
{
	/*
	使い方例
	・KeyCodeで設定する方法

	//同じ名前で設定すると設定したどちらかのボタンが押されていた場合trueが返ってくる
	//Axisの場合は第二引数(scale)で設定した値が返ってくる
	//第二引数(scale)は一番最初に指定した値が設定される
	SetVB("Jump", 1.0f, KeyCode::W());
	SetVB("Jump", 1.0f, KeyCode::UpArrow());

	//第二引数には負の値も入る
	SetVB("Left", -1.0f, KeyCode::LeftArrow());

	//キーの追加は下でもできる　ただし仮想ボタンが存在しない場合は追加されない
	//一度SetVBで登録しておかないといけない
	AddVB("Left", -1.0f, KeyCode::A());

	//XINPUTでスティックを使う場合
	//scaleは何でもいい
	SetVB("Stick", 0.0f, KeyCode::XInputAxisStickLX());

	//例
	//キーを複数設定する際scaleがそれぞれ異なる場合は入力されているやつだけ加算され-1.0～1.0の間で補正される
	SetVB("Vertical", 1.0f, KeyCode::W());//上
	AddVB("Vertical", -1.0f, KeyCode::S());//下
	AddVB("Vertical", 0.0f, KeyCode::XInputAxisStickLY());//スティック
	*/

	//ここに作成したい仮想ボタンをセットする
	{
		
	}

	//よく使う気がする仮想ボタンセット(消してもいい)
	{
		
		//移動
		SetVB("Up", 1.0f, KeyCode::W());

		SetVB("Down", -1.0f, KeyCode::S());

		SetVB("Left", -1.0f, KeyCode::A());

		SetVB("Right", 1.0f, KeyCode::D());

		//ジャンプ
		SetVB("Jump", 1.0f, KeyCode::Space());

		//カメラ移動
		//横回転
		SetVB("CameraSide", 1.0f, KeyCode::RightArrow());//右
		AddVB("CameraSide", -1.0f, KeyCode::LeftArrow());//左
		AddVB("CameraSide", 0.0f, KeyCode::XInputAxisStickRX());//スティック

		SetVB("CameraVertical", 1.0f, KeyCode::UpArrow());//上
		AddVB("CameraVertical", -1.0f, KeyCode::DownArrow());//下
		AddVB("CameraVertical", 0.0f, KeyCode::XInputAxisStickRY());//スティック
		
	}

	//仮想ボタンを使わずKeyCodeだけ使いたい場合はここに
	//CreateInputDevice(INTERFACETYPE)で登録出来る
	{
		CreateInputDevice(INTERFACETYPE::KEYBOARD);
		CreateInputDevice(INTERFACETYPE::XINPUT);
	}
}