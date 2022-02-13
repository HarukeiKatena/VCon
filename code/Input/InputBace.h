//===============================================================
//
//	�C���v�b�g�C���^�[�t�F�[�X
//	�Î�[�t�c
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

	//���͏�Ԑݒ聕�擾
	//true�œ��쒆�@false�œ����~��
	virtual void SetStatus(bool status) { mStatus = status; }
	virtual bool GetStatus(void)const { return mStatus; }
};