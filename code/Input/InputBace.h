//===============================================================
//
//	インプットインターフェース
//	嘉手納春慶
//
//===============================================================
#pragma once
class C_InputBace
{
protected:
	bool mStatus = true;

public:
	C_InputBace() {}
	virtual ~C_InputBace() {}


	virtual bool Init(void) { return false; }
	virtual void Uninit(void) {}
	virtual void Update(void) = 0;

	virtual float IsAxis(int nKey) { nKey; return 0.0f; }
	virtual bool IsPress(int nKey) = 0;
	virtual bool IsTrigger(int nKey) = 0;
	virtual bool IsRelease(int nKey) = 0;

	//入力状態設定＆取得
	//trueで動作中　falseで動作停止中
	virtual void SetStatus(bool status) { mStatus = status; }
	virtual bool GetStatus(void)const { return mStatus; }
};