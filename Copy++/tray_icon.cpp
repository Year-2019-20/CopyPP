#include "tray_icon.h"
#include "Commctrl.h"
#include "resource.h"
#include "stdafx.h"

tray_icon::tray_icon()
{
	nid = { 0 };
}

void tray_icon::destroy_tray_icon()
{
	DestroyIcon(nid.hIcon);
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

void tray_icon::create_tray_icon(HWND hWnd, HINSTANCE hInstance)
{
	ni_hInstance = hInstance;
	nid.cbSize = sizeof(nid);
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.hWnd = hWnd;
	nid.uCallbackMessage = WM_TRAYICON;
	nid.hIcon = LoadIcon(ni_hInstance, MAKEINTRESOURCE(IDI_INACTIVE));
	nid.uVersion = NOTIFYICON_VERSION_4;
	strcpy(nid.szTip, "Copy++ - Inactive");
	Shell_NotifyIcon(NIM_ADD, &nid);
}

void tray_icon::switch_icon(bool enabled)
{
	DestroyIcon(nid.hIcon);
	if (enabled)
	{
		nid.hIcon = LoadIcon(ni_hInstance, MAKEINTRESOURCE(IDI_ACTIVE));
		strcpy(nid.szTip, "Copy++ - Active");
	}
	else
	{
		nid.hIcon = LoadIcon(ni_hInstance, MAKEINTRESOURCE(IDI_INACTIVE));
		strcpy(nid.szTip, "Copy++ - Inactive");
	}

	Shell_NotifyIcon(NIM_MODIFY, &nid);
}