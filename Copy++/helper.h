#include "stdafx.h"
namespace helper
{
	HRESULT get_last_error_hres();
	vector<string> split(const string &s, char delim);
	wstring path_combine(const wstring& path1, const wstring& path2);
};