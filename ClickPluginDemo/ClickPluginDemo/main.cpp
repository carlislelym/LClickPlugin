#include "stdafx.h"
#include <iostream>

#include "resource.h"
#include "lclickplugin.h"

#define ID_BUTTON_NORMAL 50001
#define ID_BUTTON_MORE 50002
#define ID_BUTTON_PATH 50003
#define ID_BUTTON_MACRO 50004

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK KeyboardProc(int, WPARAM, LPARAM);
LRESULT CALLBACK MouseProc(int, WPARAM, LPARAM);

INT_PTR CALLBACK Setting(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI ThreadProc(
	_In_ LPVOID lpParameter
);

// 全局变量
LClickPlugin plugin;

HINSTANCE hInst;
HWND hwnd_global;
HHOOK keyboardHook;
HHOOK mouseHook;
DWORD threadId;

int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					PSTR szCmdLine,
					int nCmdShow)
{
	// 调试用
	//AllocConsole();
	//freopen("conout$", "w", stdout);

	hInst = hInstance;
	HWND     hwnd;  //窗口句柄
	MSG      msg;  //消息
	WNDCLASS wndclass;  //窗口类
	static TCHAR szClassName[] = TEXT("MainWindow");  //窗口类名

	/**********第1步：注册窗口类**********/
	//为窗口类的各个字段赋值
	wndclass.style = CS_HREDRAW | CS_VREDRAW;  //窗口风格
	wndclass.lpfnWndProc = WndProc;  //窗口过程
	wndclass.cbClsExtra = 0;  //暂时不需要理解
	wndclass.cbWndExtra = 0;  //暂时不需要理解
	wndclass.hInstance = hInstance;  //当前窗口句柄
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));  //窗口图标
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);  //鼠标样式
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //窗口背景画刷
	wndclass.lpszMenuName = nullptr;  //窗口菜单
	//wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);  //窗口菜单
	wndclass.lpszClassName = szClassName;  //窗口类名
	RegisterClass(&wndclass);			   //注册窗口


	/*********第2步：创建窗口(并让窗口显示出来)*****/
	hwnd = CreateWindow(
		szClassName,  //窗口类的名字
		TEXT("ClickPlugin"),  //窗口标题（出现在标题栏）
		WS_OVERLAPPEDWINDOW,  //窗口风格
		200,  //初始化时x轴的位置
		100,  //初始化时y轴的位置
		400,  //窗口宽度
		200,  //窗口高度
		nullptr,  //父窗口句柄
		LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU)),  //窗口菜单句柄
		hInstance,  //当前窗口的句柄
		nullptr  //不使用该值
	);
	DWORD t = GetLastError();
	if (!hwnd)
		return -1;
	hwnd_global = hwnd;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	mouseHook = SetWindowsHookEx(
		WH_MOUSE_LL,
		MouseProc,
		hInstance,
		0
	);
	if (mouseHook == nullptr)
	{
		printf("install mouseHook failed!\n");
	}
	keyboardHook = SetWindowsHookEx(
		WH_KEYBOARD_LL,
		KeyboardProc,
		hInstance,
		0
	);
	if (keyboardHook == nullptr)
	{
		printf("install keyboardHook failed!\n");
	}


	/**********第3步：消息循环**********/
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int xStart = 25;
	int yStart = 20;
	static HFONT hFont;
	static HWND buttonNormal;
	static HBITMAP buttonNormalBitmap;
	//static HICON buttonNormalIcon;
	static HWND buttonMore;
	static HWND buttonPath;
	static HWND buttonMacro;

	switch (message)
	{
	case WM_CREATE:
		hFont = CreateFont(
			-15/*高度*/, -7/*宽度*/, 0, 0, 400 /*一般这个值设为400*/,
			FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
			DEFAULT_CHARSET,  //使用默认字符集
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
			DEFAULT_QUALITY,  //默认输出质量
			FF_DONTCARE,  //不指定字体族*/
			TEXT("微软雅黑")  //字体名
		);
		// 创建主页面button
		buttonNormal = CreateWindow(
			TEXT("button"),
			nullptr,
			WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_FLAT/*扁平样式*/,
			//WS_CHILD | WS_VISIBLE | BS_ICON | BS_FLAT/*扁平样式*/,
			xStart, yStart, 100, 100,
			hwnd,
			(HMENU)ID_BUTTON_NORMAL,
			hInst,
			nullptr
		);
		if (!buttonNormal)
			return 0;
		buttonNormalBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		SendMessage(buttonNormal, BM_SETIMAGE, NULL, (WPARAM)buttonNormalBitmap);
		//buttonNormalIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON3));
		//SendMessage(buttonNormal, BM_SETIMAGE, NULL, (LPARAM)buttonNormalIcon);

		buttonMore = CreateWindow(
			TEXT("button"),
			TEXT("多鼠标点击"),
			WS_CHILD | WS_VISIBLE | BS_FLAT/*扁平样式*/,
			xStart+100+20, yStart, 100, 40,
			hwnd,
			(HMENU)ID_BUTTON_MORE,
			hInst,
			nullptr
		);
		if (!buttonMore)
			return 0;
		SendMessage(buttonMore, WM_SETFONT, (WPARAM)hFont, NULL);

		buttonPath = CreateWindow(
			TEXT("button"),
			TEXT("沿路径点击"),
			WS_CHILD | WS_VISIBLE | BS_FLAT/*扁平样式*/,
			xStart+100+20+100+20, yStart, 100, 40,
			hwnd,
			(HMENU)ID_BUTTON_PATH,
			hInst,
			nullptr
		);
		if (!buttonPath)
			return 0;
		SendMessage(buttonPath, WM_SETFONT, (WPARAM)hFont, NULL);

		buttonMacro = CreateWindow(
			TEXT("button"),
			TEXT("录制鼠标宏"),
			WS_CHILD | WS_VISIBLE | BS_FLAT/*扁平样式*/,
			xStart+100+20, yStart+40+20, 100, 40,
			hwnd,
			(HMENU)ID_BUTTON_MACRO,
			hInst,
			nullptr
		);
		if (!buttonMacro)
			return 0;
		SendMessage(buttonMacro, WM_SETFONT, (WPARAM)hFont, NULL);
		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
			// home Button
		case ID_BUTTON_NORMAL:		// buttonNormal
			ShowWindow(hwnd, SW_HIDE);
			if (plugin.nowStatus() != LClickPlugin::Waiting)
				plugin.setStatus(LClickPlugin::Waiting);
			else
				plugin.setStatus(LClickPlugin::Ready);
			break;

		case ID_BUTTON_MORE:	// buttonMore
			MessageBox(hwnd, "尚在开发中...", "敬请期待", MB_OK);
			break;

		case ID_BUTTON_PATH:	// buttonPath
			MessageBox(hwnd, "尚在开发中...", "敬请期待", MB_OK);
			break;

		case ID_BUTTON_MACRO:	// buttonMacro
			MessageBox(hwnd, "尚在开发中...", "敬请期待", MB_OK);
			break;

			// Menu
		case IDM_Setting:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, Setting);
			break;
		case IDM_Exit:
			DestroyWindow(hwnd);
			break;
		case IDM_Usage:
		{
			static const char usageString[400] = 
				"使用说明: \n"
				"0. 开始前请点击 [选项]-[设置], 进行相关参数设置\n"
				"1. 在窗体内点击首个图标进入准备状态，同时隐藏主程序\n"
				"2. 在准备状态中 按“C”键开始按照预设速度进行点击\n"
				"3. 点击开始之后 按“Q”键结束点击\n"
				"4. 任何时候可以 按“ESC”键结束键盘监听，并显示主程序\n"
				"设置说明：\n"
				"1. 固定模式： 每次模拟点击的间隔为设置的时间间隔，固定不变\n"
				"2. 随机模式： 每次点击的间隔时间在 0~最大间隔时间 中取得，每次都不一样"
				;
			MessageBox(hwnd, usageString, "Usage", MB_ICONQUESTION);
		}
			break;
		case IDM_About:
			MessageBox(hwnd, "ClickPlugin Demo\n\nCopyright 2018-2018, lym", "About", MB_ICONINFORMATION);
			break;
		default:
			break;
		}
	}
	break;

	case WM_DESTROY:	//窗口销毁消息
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (plugin.nowStatus() != LClickPlugin::Waiting)
	{
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
		if (nCode >= 0)
		{
			switch (p->vkCode)
			{
			case 27:	// ESC --> exit and don't listen the key
				if (plugin.nowStatus() != LClickPlugin::Waiting)
				{
					plugin.setStatus(LClickPlugin::Waiting);
					ShowWindow(hwnd_global, SW_SHOW);
				}
				break;
			case 67:	// C --> click directly
				if (plugin.nowStatus() == LClickPlugin::Ready)
				{
					plugin.setStatus(LClickPlugin::Working);
					HANDLE handle = CreateThread(
						nullptr,
						0,
						ThreadProc,
						nullptr,
						0,
						&threadId
					);
					if (!handle)
					{
						printf("create thread error!\n");
					}
				}
				break;
			case 81:	// Q --> quit clicking
				if (plugin.nowStatus() == LClickPlugin::Working)
				{
					plugin.stop();
				}
				break;
			
			}
		}
	}
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (plugin.nowStatus() != LClickPlugin::Waiting)
	{
		LPMSLLHOOKSTRUCT p = (LPMSLLHOOKSTRUCT)lParam;
		POINT point = p->pt;
		DWORD mouseData = p->mouseData;

		if (nCode >= 0)
		{
			if (wParam == WM_RBUTTONDOWN)
			{
				if (plugin.nowStatus() == LClickPlugin::Working)
				{
					plugin.stop();
					printf("cancle\n");
				}
				else if (plugin.nowStatus() == LClickPlugin::Ready)
				{
					plugin.setStatus(LClickPlugin::Working);
					HANDLE handle = CreateThread(
						nullptr,
						0,
						ThreadProc,
						nullptr,
						0,
						&threadId
					);
					if (!handle)
					{
						printf("create thread error!\n");
					}
				}
			}
		}
	}
	return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

INT_PTR CALLBACK Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		// 根据plugin的默认设置初始化设置页面
		if (plugin.mode() == LClickPlugin::DEFAULT)
			SendMessage(FindWindowEx(hDlg, nullptr, "Button", "固定点击模式"), BM_SETCHECK, true, 0);
		else if (plugin.mode() == LClickPlugin::RANDOM)
			SendMessage(FindWindowEx(hDlg, nullptr, "Button", "随机点击模式"), BM_SETCHECK, true, 0);
		SetDlgItemInt(hDlg, IDC_EDIT1, plugin.intervalMilliSec(), true);
		SetDlgItemInt(hDlg, IDC_EDIT2, plugin.maxLimitMilliSec(), true);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDOK:
		{
			// 开始读取参数进行设置
			if (SendMessage(FindWindowEx(hDlg, nullptr, "Button", "固定点击模式"), BM_GETCHECK, 0, 0) == BST_CHECKED)
				plugin.setMode(LClickPlugin::DEFAULT);
			else if (SendMessage(FindWindowEx(hDlg, nullptr, "Button", "随机点击模式"), BM_GETCHECK, 0, 0) == BST_CHECKED)
				plugin.setMode(LClickPlugin::RANDOM);

			plugin.setIntervalTime(GetDlgItemInt(hDlg, IDC_EDIT1, nullptr, true));
			plugin.setMaxLimitTime(GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, true));
		}
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
{
	plugin.start();
	return 0;
}
