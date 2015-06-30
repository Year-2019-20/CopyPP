#include "helper.h"
#include "stdafx.h"
	
HRESULT helper::get_last_error_hres()
{
	DWORD last_err = GetLastError();
	return HRESULT_FROM_WIN32(last_err);
}

vector<string> helper::split(const string &s, char delim)
{
	vector<string> elems;
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return move(elems);
}

wstring helper::path_combine(const wstring& path1, const wstring& path2)
{
	wchar_t out_buf[MAX_PATH + 1];
	if (PathCombineW(out_buf, path1.c_str(), path2.c_str()) == 0)
		return L"";
	return wstring(out_buf);
}