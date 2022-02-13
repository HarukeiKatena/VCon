●仮想コントローラーライブラリ
VCon

●利用しているライブラリ
DirectInput
XInput
std

●VCon導入方法
Inputの中身をすべてプロジェクトに入れる
VirtualController.hをインクルードする。

KeyDataList内のSetVirtualButton関数に使いそうな仮想ボタンが最初からセットされている

●簡単な使い方
//仮想コントローラー登録例
VCon::SetVB("Up", 1.0f, KeyCode::UpArrow());
VCon::AddVB("Up", 1.0f, KeyCode::W());

VCon::SetVB("SideMove", -1.0f, KeyCode::A());//左
VCon::AddVB("SideMove", 1.0f, KeyCode::D());//右
VCon::AddVB("SideMove", 0.0f, KeyCode::XInputAxisStickLX());//スティック

//仮想コントローラー使用例
VCon::GetAxis(KeyCode::XInputAxisStickLX());//パッド
VCon::GetAxis(KeyCode::W());
VCon::GetPress(KeyCode::A());
VCon::GetTrigger(KeyCode::S());
VCon::GetRelease(KeyCode::D());

VCon::GetAxis("Up");
VCon::GetAxis("SideMove");
