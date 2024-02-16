#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>
class WwiseAPI
{
public:
	bool Init();
	void LoadBank(const wchar_t* bankName, AkBankID& bankID);

private:
	void Log(const wchar_t* logMsg);
};

