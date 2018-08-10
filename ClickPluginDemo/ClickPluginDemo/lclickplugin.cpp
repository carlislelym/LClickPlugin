#include "stdafx.h"
#include "lclickplugin.h"

LClickPlugin::LClickPlugin()
	: m_xPos(0)
	, m_yPos(0)
	, m_mode(DEFAULT)
	, m_maxLimit(10000)
	, m_intervalMilliSec(50)
{
}

LClickPlugin::~LClickPlugin()
{

}

void LClickPlugin::start()
{
	SetCursorPos(m_xPos, m_yPos);
	while (true)
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

}

void LClickPlugin::setAbsolutePos(int x, int y)
{
	if (x < 0 || y < 0)
		return;
	m_xPos = x;
	m_yPos = y;
}

void LClickPlugin::setIntervalTime(int millisecond, INTERVAL_MODE mode, int maxLimit)
{
	if (millisecond < 0 || maxLimit < 0)
		return;
	m_intervalMilliSec = millisecond;
	m_maxLimit = maxLimit;
}