// ProcessViewer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


int main()
{
	LPTSTR szNotepad = _tcsdup(TEXT("notepad"));
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));

	CreateProcess(NULL, szNotepad, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);

	ShellExecute(NULL, _tcsdup(TEXT("open")), szNotepad, NULL, NULL, SW_SHOW);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 ppe;
	ppe.dwSize = sizeof(PROCESSENTRY32);

	BOOL b = Process32First(hSnap, &ppe);

	while (b)
	{
		printf_s("process Name:%S\n", ppe.szExeFile);
		printf_s("process ID:%6d\n", ppe.th32ProcessID);
		b = Process32Next(hSnap, &ppe);
	}

	CloseHandle(hSnap);
	return 0;
}

