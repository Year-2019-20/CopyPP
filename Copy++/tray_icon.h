#ifdef _MSC_VER
#	pragma once
#endif
#include "stdafx.h"
#include <shellapi.h>

#define WM_TRAYICON ( WM_USER + 1 )

class tray_icon
{
private:
	NOTIFYICONDATA nid;
	HINSTANCE ni_hInstance;
	HWND ni_hWnd;
public:
	tray_icon();
	void create_tray_icon(HWND hWnd, HINSTANCE hInstance);
	void switch_icon(bool enabled);
	void destroy_tray_icon();
};

