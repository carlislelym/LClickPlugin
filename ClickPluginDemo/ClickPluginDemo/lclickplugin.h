#ifndef __LCLICKPLUGIN_H__
#define __LCLICKPLUGIN_H__

/*
*****实现功能*****
* 按 esc 退出点击模式
* 按 ctrl+s 进行选择点击位置
* 点击之后马上开始按照设置好的模式进行点击
* 需要一个类似截屏软件的小窗口进行模式设置之类的操作
* 
*****增强功能*****
* 按 ctrl+m 进行多重点击选择
* 类似录制宏一样，保存刚刚的一段操作
* 支持按路径，沿着用户画的路径进行点击
* 
*****待办方案*****
* 1. 多线程进行
* 2. 安装hook捕获键盘按键 来进行控制
* 3. SendInput()进行发送点击或者键盘
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