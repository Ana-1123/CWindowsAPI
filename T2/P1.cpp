#define _CRT_SECURE_NO_WARNINGS 
#include <Windows.h>
#include<tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include<tchar.h>
#define BUFFER_SIZE 256
void DirectoryHierarchy(char* directoryName)
{
	if (CreateDirectoryA(directoryName, NULL) == 0)
	{
		printf("Eroare la crearea directorului.%d\n", GetLastError());
	}
}

int main()
{  //1.a
	HANDLE htoken;
	HANDLE processP1 = GetCurrentProcess();
	BOOL openToken = OpenProcessToken(processP1, TOKEN_ADJUST_PRIVILEGES, &htoken);
	if (!openToken)
	{
		printf("Eroare la deschiderea token-ului. %d", GetLastError());
		return 0;
	}
	LUID privID;
	if (LookupPrivilegeValue(NULL, SE_SYSTEM_PROFILE_NAME, &privID)) {

		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = privID;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		BOOL result = AdjustTokenPrivileges(htoken, FALSE, &tp,sizeof(TOKEN_PRIVILEGES), NULL, NULL);
		if (!result)
		{
			printf(" Eroare la AdjustTokenPrivileges . %d \n", GetLastError());
			if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

			{
				printf("Token-ul nu are privilegiu specificat. \n");

			}
			return 0;
		}
	}
	else
	{
		printf(" Eroare la LookupPrivilegeValue . %d \n", GetLastError());
		return 0;
	}
	CloseHandle(htoken);

	//1.b
	char directoryName[1024];
	strcpy_s(directoryName, "C:\\Facultate");
	DirectoryHierarchy(directoryName);

	strcat(directoryName, "\\CSSO");
	DirectoryHierarchy(directoryName);

	strcat(directoryName, "\\Laboratoare");
	DirectoryHierarchy(directoryName);

	strcat(directoryName, "\\Week2");
	DirectoryHierarchy(directoryName);
	//1.c
	DWORD kprocese, kfire, kmodule;
	char currentFile[200];
	strcpy(currentFile, "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\procese.txt");
    HANDLE h;
    h = CreateFile(currentFile, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) {
			printf("Eroare la creare fisier procese.txt: %d\n", GetLastError());
			return 0;
			}

	char fileText[200];
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	//cer un snapshot la procese
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("Eroare la CreateToolhelp32Snapshot. %d \n", GetLastError());
		return(-1);
	}
	//inițializez dwSize cu dimensiunea structurii.
	pe32.dwSize = sizeof(PROCESSENTRY32);
	//obțin informații despre primul proces
	if (!Process32First(hProcessSnap, &pe32))
	{
		printf("Eroare la Process32First. %d \n", GetLastError());
		CloseHandle(hProcessSnap); 
		return(-1);
	}
	kprocese = 0;
	do
	{
		++kprocese;
		sprintf(fileText,"Parent process Id: %d, Process Id: %d, Process name: %s \n", pe32.th32ParentProcessID,pe32.th32ProcessID, pe32.szExeFile);
		DWORD dwBytesToWrite = (DWORD)strlen(fileText);
		DWORD dwBytesWritten = 0;
		if (!WriteFile(h, fileText, dwBytesToWrite, &dwBytesWritten, NULL))
		{
			printf("Eroare la scrierea in fisier procese.txt.\n");
		}
		memset(fileText, 0, sizeof(fileText));
	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	CloseHandle(h);
	//1.d
	strcpy(currentFile, "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\fire.txt");
	HANDLE h1;
	h1 = CreateFile(currentFile, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h1 == INVALID_HANDLE_VALUE) {
		printf("Eroare la creare fisier fire.txt: %d\n", GetLastError());
		return 0;
	}
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;
 
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return(FALSE);
	
	te32.dwSize = sizeof(THREADENTRY32);
	if (!Thread32First(hThreadSnap, &te32))
	{
		printf("Eroare la Thread32First. %d \n", GetLastError());
		CloseHandle(hThreadSnap); 
		return(-1);
	}

	kfire = 0;
	do
	{
		++kfire;
		sprintf(fileText, "Thread Id: %d, Owner Process Id: %d\n", te32.th32ThreadID, te32.th32OwnerProcessID);
		DWORD dwBytesToWrite = (DWORD)strlen(fileText);
		DWORD dwBytesWritten = 0;
		if (!WriteFile(h, fileText, dwBytesToWrite, &dwBytesWritten, NULL))
		{
			printf("Eroare la scrierea in fisier fire.txt %d.\n", GetLastError());
			return(-1);
		}
		memset(fileText, 0, sizeof(fileText));
		
	} while (Thread32Next(hThreadSnap, &te32));

	CloseHandle(hThreadSnap);
	CloseHandle(h1);
	//1.e
	DWORD processP1ID=GetProcessId(processP1);
	strcpy(currentFile, "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\module_process.txt");
	HANDLE h2;
	h2 = CreateFile(currentFile, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h2 == INVALID_HANDLE_VALUE) {
		printf("Eroare la creare fisier module_process.txt: %d\n", GetLastError());
		return 0;
	}
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;
	
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processP1ID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return(FALSE);
	
	me32.dwSize = sizeof(MODULEENTRY32);

	// obtinem informatii despre primul modul
	if (!Module32First(hModuleSnap, &me32))
	{
		printf("Eroare la Module32First.%d \n", GetLastError());
		CloseHandle(hModuleSnap); 
		return(-1);
	}
	
	kmodule = 0;
	do
	{   
		++kmodule;
		sprintf(fileText, "Modul Id: %d, Process Id: %d, Module name: %s, Executable: %s\n", me32.th32ModuleID,
			me32.th32ProcessID, me32.szModule, me32.szExePath);
		DWORD dwBytesToWrite = (DWORD)strlen(fileText);
		DWORD dwBytesWritten = 0;
		if (!WriteFile(h2, fileText, dwBytesToWrite, &dwBytesWritten, NULL))
		{
			printf("Eroare la scrierea in fisier module_process.txt.\n");
		}
		memset(fileText, 0, sizeof(fileText));

	} while (Module32Next(hModuleSnap, &me32));

	CloseHandle(hModuleSnap);
	CloseHandle(h2);
	//1.f
	HANDLE fileMapping=OpenFileMappingA(FILE_MAP_ALL_ACCESS,TRUE,"cssow2basicsync");
	sprintf(fileText, "Module: %d\nProcese: %d\nFire: %d\n",kmodule,kprocese,kfire);
    char* pBuf;
    pBuf = (char*) MapViewOfFile(fileMapping,FILE_MAP_ALL_ACCESS,0,0, BUFFER_SIZE);

   if (pBuf == NULL)
   {
      printf("Eroare la MapViewOfFile %d.\n", GetLastError());
       CloseHandle(fileMapping);
      return 0;
   }
   CopyMemory((PVOID)pBuf, fileText, (strlen(fileText) * sizeof(char)));
   UnmapViewOfFile(pBuf);
   CloseHandle(fileMapping);
   CloseHandle(processP1);
   return 0;
}