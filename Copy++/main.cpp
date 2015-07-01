#include "main.h"
#include "stdafx.h"

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow) {

	int key;
	MSG Msg;
	std::map<string, int> keys = { { "F1", VK_F1 }, { "F2", VK_F2 }, { "F3", VK_F3 }, { "F4", VK_F4 },
	{ "F5", VK_F5 }, { "F6", VK_F6 }, { "F7", VK_F7 }, { "F8", VK_F8 },
	{ "F9", VK_F9 }, { "F10", VK_F10 }, { "F11", VK_F11 }, { "F12", VK_F12 } };

	wchar_t* p_app_data;
	size_t len;
	errno_t err = _wdupenv_s(&p_app_data, &len, L"AppData");
	wstring app_data(p_app_data);
	wstring dir_name = L"CopyPP";
	delete p_app_data;

	auto dir_path = path_combine(app_data, dir_name);
	if (dir_path == L"")
	{
		auto hres = get_last_error_hres();
		_com_error err(hres);
		MessageBoxExW(NULL, L"Could not get path.", L"Error", MB_ICONERROR | MB_OK, 0);
		PostQuitMessage(hres);
	}

	if (!(CreateDirectoryW(dir_path.c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError()))
	{
		auto hres = get_last_error_hres();
		_com_error err(hres);
		MessageBoxEx(NULL, err.ErrorMessage(), "Directory creation failed.", MB_ICONERROR | MB_OK, 0);
		PostQuitMessage(hres);
	}

	auto conf_path = path_combine(dir_path, L"config.txt");
	ifstream conf_str(conf_path);
	if (!conf_str) // We have that ifstream declared above and can directly use it. No need for a helper-method
	{
		ofstream file(conf_path);
		file << "hotkey=F12";
		file.flush();
		file.close();

		key = VK_F12;
	}
	else
	{
		string content((std::istreambuf_iterator<char>(conf_str)),
			(std::istreambuf_iterator<char>()));
		vector<string> parts = split(content, '=');
		if (parts.front() == "hotkey")
		{
			key = keys[parts.back()];
		}
	}

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "MessageWindow";

	if (!RegisterClassEx(&wc))
	{
		MessageBoxExW(NULL, L"Window Registration Failed.", L"Error.",
			MB_ICONERROR | MB_OK, 0);
		return 0;
	}

	HWND hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"MessageWindow",
		"CopyPP Message Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (hWnd == NULL)
	{
		MessageBoxExW(NULL, L"Window Creation Failed.", L"Error.",
			MB_ICONERROR | MB_OK, 0);
		return 0;
	}

	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);

	icon.create_tray_icon(hWnd, hInstance);
	if (!RegisterHotKey(
		hWnd,
		1,
		MOD_NOREPEAT,
		key))
	{
		auto hres = get_last_error_hres();
		_com_error err(hres);
		MessageBoxEx(NULL, err.ErrorMessage(), "Registering the hotkey failed.", MB_ICONERROR | MB_OK, 0);
		PostQuitMessage(hres);
	}

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	if (!UnregisterHotKey(hWnd, 1))
	{
		auto hres = get_last_error_hres();
		_com_error err(hres);
		MessageBoxEx(hWnd, err.ErrorMessage(), "Unregistering the hotkey failed.", MB_ICONERROR | MB_OK, 0);
		PostQuitMessage(hres);
	}

	clear_topmost_handle();
	icon.destroy_tray_icon();

	return Msg.wParam;
}

HWND get_foreground_handle()
{
	return GetForegroundWindow();
}

void clear_topmost_handle()
{
	SetWindowPos(current_topmost_hwnd,
		HWND_NOTOPMOST,
		0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_TRAYICON:
		switch (LOWORD(lParam))
		{
		case WM_RBUTTONDOWN:
			UINT uFlag = MF_BYPOSITION | MF_STRING;
			POINT clickPoint;
			GetCursorPos(&clickPoint);
			HMENU hPopMenu = CreatePopupMenu();

			InsertMenu(hPopMenu, 0,
				MF_BYPOSITION, 0, "About");
			InsertMenu(hPopMenu, 1,
				MF_BYPOSITION, 1, "Exit");

			int id = TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN | TPM_RETURNCMD,
				clickPoint.x, clickPoint.y + 50, 0, hwnd, NULL);
			switch (id)
			{
			case 0:
				MessageBoxExW(hwnd, L"Copy++ by Dominic Beger (Trade/ProgTrade) 2015", L"About", MB_ICONINFORMATION | MB_OK, 0);
				break;
			case 1:
				PostQuitMessage(0);
				break;
			}
			break;
		}
		break;
	case WM_HOTKEY:
		switch_hwnd_topmost();
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void switch_hwnd_topmost()
{
	enabled = !enabled;
	if (enabled)
	{
		current_topmost_hwnd = get_foreground_handle();
		SetWindowPos(current_topmost_hwnd,
			HWND_TOPMOST,
			0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);
	}
	else
	{
		SetWindowPos(current_topmost_hwnd,
			HWND_NOTOPMOST,
			0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE);
	}

	icon.switch_icon(enabled);
}