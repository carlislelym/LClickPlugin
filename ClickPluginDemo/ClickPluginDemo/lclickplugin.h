#ifndef __LCLICKPLUGIN_H__
#define __LCLICKPLUGIN_H__

/*
*****ʵ�ֹ���*****
* �� esc �˳����ģʽ
* �� ctrl+s ����ѡ����λ��
* ���֮�����Ͽ�ʼ�������úõ�ģʽ���е��
* ��Ҫһ�����ƽ��������С���ڽ���ģʽ����֮��Ĳ���
* 
*****��ǿ����*****
* �� ctrl+m ���ж��ص��ѡ��
* ����¼�ƺ�һ��������ոյ�һ�β���
* ֧�ְ�·���������û�����·�����е��
* 
*****���췽��*****
* 1. ���߳̽���
* 2. ��װhook������̰��� �����п���
* 3. SendInput()���з��͵�����߼���
*/

class LClickPlugin
{
public:
	enum INTERVAL_MODE
	{
		DEFAULT,
		RANDOM,
	};
public:
	LClickPlugin();
	~LClickPlugin();

	void start();
	void stop();

	void setAbsolutePos(int x, int y);
	void setIntervalTime(int millisecond, INTERVAL_MODE mode, int maxLimit = 10000);

private:
	int m_xPos;
	int m_yPos;

	INTERVAL_MODE m_mode;
	int m_maxLimit;
	int m_intervalMilliSec;
};

#endif	// __LCLICKPLUGIN_H__