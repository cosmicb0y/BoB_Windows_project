// ProcessViewer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


bool proc_tree_callback(_In_ process& process_info, _In_ DWORD_PTR callback_tag)
{
	printf("pid = %u, %u, %ws %llu\n", process_info.pid(), process_info.ppid(), process_info.process_name().c_str(), process_info.creation_time());
		return true;
}
//I'M BBAKBBAK
int main()
{
	HANDLE hSnap;
	PROCESSENTRY32 ppe;
	ppe.dwSize = sizeof(PROCESSENTRY32);
	DWORD suspend_pid;
	
	DWORD ppids[1024] = {0};

	char pname[1024] = { 0 };
	size_t convertedChars = 0;
	int process_cnt = 0,i;
	BOOL b;
	cprocess_tree proc_tree;
	process proc;

	DWORD choice;
	int num, cnt=0;
	
	while (1)
	{
		printf_s("\n1-list    2-list process tree    3-kill    4-suspend    5-resume\ninput : ");
		scanf_s("%d", &num);
		if (num == 1)
		{
			hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			b = Process32First(hSnap, &ppe);
			while (b)
			{
				ppids[cnt++] = ppe.th32ProcessID;
				b = Process32Next(hSnap, &ppe);
			}
			process_cnt = cnt;
			b = Process32First(hSnap, &ppe);
			while (b)
			{
				for (i = 0; i < cnt; i++)
				{
					if (ppids[i] < ppe.th32ParentProcessID)
					{
						continue;
					}
					else if (ppids[i] == ppe.th32ParentProcessID)
						break;

					//root 노드
					else
					{
						printf_s("Name:%S", ppe.szExeFile);
						printf_s("    PID:%6d\n", ppe.th32ProcessID);
						break;
					}
				}
				b = Process32Next(hSnap, &ppe);
			}
			cnt = 0;
			printf("the number of process : %d\n\n", process_cnt);
			/*
			hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			b = Process32First(hSnap, &ppe);
			while (b)//초반에 전부 보이진 않음
			{
				printf_s("process Name:%S", ppe.szExeFile);
				printf_s("    PID:%6d\n", ppe.th32ProcessID);
				b = Process32Next(hSnap, &ppe);
				cnt++;
				wcstombs_s(&convertedChars, pname, sizeof(ppe.szExeFile), ppe.szExeFile, _TRUNCATE);
				if (strcmp(pname, "PING.EXE") == 0)
				{
					process_cnt++;
					suspend_pid = ppe.th32ProcessID;
				}
			}
			printf("The number of Prccesses : %d\n", cnt);
			cnt = 0;
			*/
		}
		
		else if (num == 2)//process 
		{
			int check = 0;
			printf_s("select process : ");
			scanf_s("%d",&choice);
			hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			b = Process32First(hSnap, &ppe);
			while (b)
			{
				ppids[cnt++] = ppe.th32ProcessID;
				b = Process32Next(hSnap, &ppe);
			}
			b = Process32First(hSnap, &ppe);
			while (b)
			{
				for (i = 0; i < cnt; i++)
				{
					if (ppids[i] < ppe.th32ParentProcessID)
					{
						continue;
					}
					else if (ppids[i] == ppe.th32ParentProcessID)
					{
						if (check == 1)
							break;
						if (ppe.th32ParentProcessID == choice)
						{
							printf_s("\n");
							proc_tree.build_process_tree();
							proc_tree.print_process_tree((DWORD)choice);
							printf_s("\n");
							check = 1;
						}
						break;
					}

					//root 노드
					else
					{
						printf_s("Name:%S", ppe.szExeFile);
						printf_s("    PID:%6d\n", ppe.th32ProcessID);
						break;
					}
				}
				b = Process32Next(hSnap, &ppe);
			}
			cnt = 0;
			
		}
		else if (num == 3)//kill
		{
			printf_s("kill process input : ");
			scanf_s("%d",&choice);
			//proc.kill((DWORD)choice);
			proc_tree.kill_process_tree((DWORD)choice);
			printf_s("killed ok\n");
		}
		else if (num == 4)//suspend
		{
			printf_s("suspend process input : ");
			scanf_s("%d", &choice);
			proc.suspend((DWORD)choice);
			printf_s("suspend ok\n");
		}
		else if (num == 5)//resume
		{
			printf_s("resume process input : ");
			scanf_s("%d", &choice);
			proc.resume((DWORD)choice);
			printf_s("resume ok\n");
		}
		
	}

	CloseHandle(hSnap);

	
	if (!proc_tree.build_process_tree()) return false;

	// 프로세스 열거 테스트 (by callback)
	//proc_tree.iterate_process(proc_tree_callback, 0);
	//proc_tree.iterate_process_tree(proc_tree.find_process(L"System"), proc_tree_callback, 0);

	// print 
	proc_tree.print_process_tree((DWORD)596);

	// 프로세스 종료 테스트	
	//proc_tree.kill_process_tree(proc_tree.find_process(L"cmd.exe"));

	//프로세스 일시정지 테스트
	if (process_cnt > 0)
	{
		printf("suspending ping.exe...\n\n");
		if (!proc.suspend(suspend_pid))
		{
			printf("suspend fail!\n");
			return -1;
		}
		printf("success!! wait...\n");
		Sleep(5000);
		//프로세스 일시정지후 다시 실행
		printf("resuming...\n");
		if (!proc.resume(suspend_pid))
		{
			printf("resume fail!\n");
			return -1;
		}
		printf("success!!!\n");
	}
	else
	{
		printf("process가 존재하지 않습니다!\n");
	}
	return true;
	
}

