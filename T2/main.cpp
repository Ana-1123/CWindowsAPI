#define _CRT_SECURE_NO_WARNINGS 
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include<tlhelp32.h>
#include<tchar.h>
#define BUFFER_SIZE 256
int main()
{  
    //3.a
	HANDLE hmap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024 * 1024, "cssow2basicsync");
	if (hmap == NULL) {
		printf("Cannot create file mapping. Error code: %d", GetLastError());
		return 0;
	}
	//3.b
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
  
    if (!CreateProcess("C:\\Tema2CSSO\\P1\\Debug\\P1.exe",NULL, NULL,NULL, FALSE,0,NULL, NULL, &si, &pi))
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 0;
    }

    WaitForSingleObject(pi.hProcess, INFINITE); 
 
	//3.c
    HANDLE hMapFile;
    char* pBuf;
    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE, "cssow2basicsync");

    if (hMapFile == NULL)
    {
        printf("Could not open file mapping object (%d).\n",GetLastError());
        return 0;
    }

    pBuf = (char*)MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);

    if (pBuf == NULL)
    {
        printf("Could not map view of file (%d).\n",GetLastError());
        CloseHandle(hMapFile);
        return 0;
    }
    printf("%s",pBuf);
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
	//3.d
    STARTUPINFO si2;
    PROCESS_INFORMATION pi2;

    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    ZeroMemory(&pi2, sizeof(pi2));
    
    if (!CreateProcess("C:\\Tema2CSSO\\P2\\Debug\\P2.exe", NULL, NULL, NULL, FALSE, 0, NULL,NULL, &si, &pi) )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 0;
    }
    WaitForSingleObject(pi2.hProcess, INFINITE);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);
    CloseHandle(hmap);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
