﻿#pragma once
#include "Mihoyosdk.h"
#include "Bsgsdk.h"
#include <sstream>
#include <fstream>
class LoginBili 
	: public Bsgsdk
{
public:
	int loginBiliKey(std::string& realName);
	int loginBiliPwd(std::string Account, std::string Pwd, std::string& message);
	void openConfig();
	void updateConfig();
	void setAutoStart(bool state);
	bool getAutoStart();
	void setAutoExit(bool Exit);
	bool getAutoExit();
	int uid;
	std::string access_key;
private:
	std::ifstream inFile;
	json::Json configJson;
	std::string loginData;
	json::Json userInfo;
	bool autoExit = false;
};
