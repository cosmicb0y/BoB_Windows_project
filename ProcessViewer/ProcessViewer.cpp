// ProcessViewer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


bool proc_tree_callback(_In_ process& process_info, _In_ DWORD_PTR callback_tag)
{
	printf("pid = %u, %u, %ws %llu\n", process_info.pid(), process_info.ppid(), process_info.process_name().c_str(), process_info.creation_time());
		return true;
}

int main()
{
	/*LPTSTR szNotepad = _tcsdup(TEXT("notepad"));
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

	CloseHandle(hSnap);*/

	cprocess_tree proc_tree;
	if (!proc_tree.build_process_tree()) return false;

	// 프로세스 열거 테스트 (by callback)
	//proc_tree.iterate_process(proc_tree_callback, 0);
	//proc_tree.iterate_process_tree(proc_tree.find_process(L"System"), proc_tree_callback, 0);

	// print 
	proc_tree.print_process_tree((DWORD)596);

	// 프로세스 종료 테스트	
	//proc_tree.kill_process_tree(proc_tree.find_process(L"cmd.exe"));

	return true;
	
}

