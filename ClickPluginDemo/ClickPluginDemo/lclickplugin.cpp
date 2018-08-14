#include "stdafx.h"
#include "lclickplugin.h"

LClickPlugin::LClickPlugin()
	: m_xPos(0)
	, m_yPos(0)
	, m_status(Waiting)
	, m_mode(DEFAULT)
	, m_maxLimit(1000)
	, m_intervalMilliSec(200)
{
}

LClickPlugin::~LClickPlugin()
{

}

void LClickPlugin::start()
{
	while (m_status == Working)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		switch (m_mode)
		{
		case LClickPlugin::DEFAULT:
			break;
		case LClickPlugin::RANDOM:
			m_intervalMilliSec = rand() % m_maxLimit;
			break;
		default:
			break;
		}
		Sleep(m_intervalMilliSec);
	}
}

void LClickPlugin::stop()
{
	m_status = Ready;
}

LClickPlugin::INTERVAL_MODE LClickPlugin::mode()
{
	return m_mode;
}

int LClickPlugin::intervalMilliSec()
{
	return m_intervalMilliSec;
}

int LClickPlugin::maxLimitMilliSec()
{
	return m_maxLimit;
}

LClickPlugin::Status LClickPlugin::nowStatus()
{
	return m_status;
}

void LClickPlugin::setStatus(Status status)
{
	m_status = status;
}

void LClickPlugin::setMode(INTERVAL_MODE mode)
{
	m_mode = mode;
}

void LClickPlugin::setAbsolutePos(int x, int y)
{
	if (x < 0 || y < 0)
		return;
	m_xPos = x;
	m_yPos = y;
}

void LClickPlugin::setMaxLimitTime(int millisecond)
{
	if (millisecond < 0)
		return;
	m_maxLimit = millisecond;
}

void LClickPlugin::setIntervalTime(int millisecond)
{
	if (millisecond < 0)
		return;
	m_intervalMilliSec = millisecond;
}
