#define _CRT_SECURE_NO_WARNINGS 
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include<tlhelp32.h>
#include<tchar.h>
#include <strsafe.h>

DWORD WINAPI nrnewlines(LPVOID param)
{   
    char c;
    char path[200];
    DWORD nrlines=0;
    DWORD dwBytesRead=0;
    HANDLE hfile = (HANDLE)param;
    if (GetFinalPathNameByHandle(hfile, path,200, FILE_NAME_NORMALIZED) == 0)
    {
        printf("Eroare la getpath.\n");
        return 0;
    }
    char* ptr;
    ptr = strrchr(path, '\\')+1;

    do {
        if (!ReadFile(hfile, &c, 1, &dwBytesRead, NULL))
        {
            printf("Eroare la citirea din fisier.\n");
            return 1;
        }
        if (c == '\n')
            ++nrlines;
        if (dwBytesRead == 0)
            break;
        dwBytesRead = 0;
    } while (TRUE);
    printf("%s %d\n", ptr, nrlines);
    CloseHandle(hfile);
    return 0;
}

int main()
{
    const char* filesName[] = { "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\procese.txt",
        "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\fire.txt",
        "C:\\Facultate\\CSSO\\Laboratoare\\Week2\\module_process.txt" };
    HANDLE  hThreadArray[3];
    DWORD   dwThreadIdArray[3];
    for (DWORD i = 0; i < 3; i++)
    {
        HANDLE h;
        h = CreateFile(filesName[i], GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (h == INVALID_HANDLE_VALUE) {
            printf("Eroare la creare fisier: %d\n", GetLastError());
            return 0;
        }
       
        hThreadArray[i] = CreateThread(NULL, 0, nrnewlines, h, 0, &dwThreadIdArray[i]);  
 
        if (hThreadArray[i] == NULL)
        {
            printf("Eroare la creare thread: %d\n", GetLastError());
            ExitProcess(3);
        }
        CloseHandle(h);
    }
       
    // Wait until all threads have terminated.
    WaitForMultipleObjects(3, hThreadArray, TRUE, INFINITE);
    for (int i = 0; i < 3; i++)
        CloseHandle(hThreadArray[i]);
	return 0;
}