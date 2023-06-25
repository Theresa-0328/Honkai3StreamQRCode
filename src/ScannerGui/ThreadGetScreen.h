﻿#pragma once
#include <QThread>
#include <QMutex>
#include "ScreenScan.h"
#include "Mihoyosdk.h"
class ThreadGetScreen
	: public QThread
{

	Q_OBJECT
public:
	ThreadGetScreen(QObject* parent);
	~ThreadGetScreen();
	bool isExit = true;
	void InitScreen(const int& uid, const std::string& access_key, std::string uname);
	void Init0(const std::string& uid, const std::string& token);
	void run();
	void stop();
signals:
	void loginResults(const bool& b);
private:
	void serverType0();
	QMutex m_mux;
	int serverType = 0;
	std::string LoginData;
	Mihoyosdk m;
	std::string uid;
	std::string gameToken;
};