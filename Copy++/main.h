#ifdef _MSC_VER
#	pragma once
#endif

#include "helper.h"
#include "tray_icon.h"
#include <shellapi.h>
#include <Windows.h>
#include <algorithm>
#include "stdafx.h"

using namespace helper;

/*
	Variables
*/
bool enabled = false;
HWND current_topmost_hwnd;
tray_icon icon;

/*
	Functions
*/
HWND get_foreground_handle();
void clear_topmost_handle();
void switch_hwnd_topmost();
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow);