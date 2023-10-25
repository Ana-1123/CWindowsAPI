#define _CRT_SECURE_NO_WARNINGS
#define BUFFER_SIZE 1024
#include <windows.h>
#include <stdio.h>
#define MAX_KEY_LENGTH 256
using namespace std;
void DirectoryHierarchy(char* directoryName)
{
	if (CreateDirectoryA(directoryName, NULL) == 0)
	{
		printf("Eroare la crearea directorului.%d\n", GetLastError());
	}
}

int main()
{   //1
	char directoryName[1024];
	strcpy_s(directoryName, "C:\\Facultate");
	DirectoryHierarchy(directoryName);

	strcat(directoryName, "\\CSSO");
	DirectoryHierarchy(directoryName);

	strcat(directoryName, "\\Laboratoare");
	DirectoryHierarchy(directoryName);

	strcat(directoryName, "\\Week1");
	DirectoryHierarchy(directoryName);

	char d[1024];
	strcpy_s(d, directoryName);
	strcat(directoryName, "\\Extensii");
	DirectoryHierarchy(directoryName);

	strcat(d, "\\Rezultate");
	DirectoryHierarchy(d);
	//2
	const char* filesName[] = {"HKLM.txt","HKCC.txt","HKCU.txt"};
	HKEY chei[] = { HKEY_LOCAL_MACHINE,HKEY_CURRENT_CONFIG,HKEY_CURRENT_USER};
	char s[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\";
	for (DWORD i = 0; i < 3; i++)
	{
		char curentFile[200];
		strcpy_s(curentFile, s);
		strcat(curentFile, filesName[i]);
		HANDLE h;
		h = CreateFile(curentFile, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h == INVALID_HANDLE_VALUE) {
			printf("Eroare la creare fisier: %d\n", GetLastError());
			return 0;
		}
		char fileText[200];
		DWORD numarChei = 0;
		DWORD maxSubKeyLen = 0;
		FILETIME lastWriteTime;
	    DWORD errorCode = 0;
		DWORD errorTime = 0;
		SYSTEMTIME lpSystemTime;
		HKEY hKey;
		if (RegOpenKeyEx(chei[i], NULL, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
			errorCode = RegQueryInfoKey(hKey, NULL, NULL, NULL, &numarChei, &maxSubKeyLen, NULL, NULL, NULL, NULL, NULL, &lastWriteTime);
		else
		{
			printf("Eroare la deschidere hive: %d\n", GetLastError());
			return 0;
		}
		if (errorCode == ERROR_SUCCESS)
		{
			errorTime = FileTimeToSystemTime( &lastWriteTime, &lpSystemTime);
			//GetSystemTime(&lpSystemTime);
			char timeHumanReadable[100];
			sprintf(timeHumanReadable, "%02d/%02d/%02d %02d:%02d:%02d", lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond);
			if (errorTime != 0)
			{
				sprintf(fileText, "%d\n%d\n%s", numarChei, maxSubKeyLen, timeHumanReadable);
			}
			else
				printf("Eroare la filetimesystem");
		}
		else
		{
			printf("Eroare la info query: %d\n", GetLastError());
			return 0;
		}
		DWORD dwBytesToWrite = (DWORD)strlen(fileText);
		DWORD dwBytesWritten = 0;
		if (!WriteFile(h,fileText, dwBytesToWrite, &dwBytesWritten, NULL))
		{
			printf("Eroare la scrierea in fisier.\n");
		}
		CloseHandle(h);
		
	}
	//3
	WIN32_FIND_DATA data;
	LARGE_INTEGER filesize;
	char sumarText[1000];
	memset(sumarText, 0, sizeof(sumarText));
	HANDLE hFind = FindFirstFile("C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\*.txt", &data);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		printf("Eroare la gasirea primului fisier.\n");
		return 0;
	}

	do
	{
     		filesize.LowPart = data.nFileSizeLow;
		filesize.HighPart = data.nFileSizeHigh;
			char curentFileData[200];
	 	char path[100];
			memset(curentFileData, 0, sizeof(curentFileData));
			memset(path, 0, sizeof(path));
			strcpy(path,s);
			strcat(path, data.cFileName);
			sprintf(curentFileData,"%s   %ld bytes\n",path, filesize.QuadPart);
			strcat(sumarText, curentFileData);
	} while (FindNextFile(hFind, &data) != 0);
	HANDLE h2;
	char s1[200];
	strcpy_s(s1, s);
	strcat(s1, "sumar.txt");
	h2 = CreateFile(s1, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h2 == INVALID_HANDLE_VALUE) {
		printf("Eroare la creare fisier: %d\n", GetLastError());
		return 0;
	}
	DWORD dwBytesToWrite = (DWORD)strlen(sumarText);
	DWORD dwBytesWritten = 0;
	if (!WriteFile(h2, sumarText, dwBytesToWrite, &dwBytesWritten, NULL))
	{
		printf("Eroare la scrierea in fisierul sumar.txt.\n");
	}
	CloseHandle(h2);

	 //4
    char numeSubcheie[MAX_KEY_LENGTH];
	DWORD nameLen;
	HKEY hCRKey;
	HKEY subKey;
	char filePath[200];
	char   subData[257] = { 0 };
	DWORD size;

	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, NULL, 0, KEY_READ, &hCRKey) == ERROR_SUCCESS)
	{
		for (DWORD j = 2; j < 5; j++)
		{
			nameLen = MAX_KEY_LENGTH;
			if (RegEnumKeyEx(hCRKey, j, numeSubcheie, &nameLen, 0, NULL, NULL, NULL) == ERROR_SUCCESS)
			{
				strcpy_s(filePath, "C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Extensii\\");
				strcat_s(filePath, numeSubcheie);
				strcat_s(filePath, ".txt");

				HANDLE h = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (h == INVALID_HANDLE_VALUE)
				{
					printf("Eroare la crearea fisierului.Cod eroare %d\n", GetLastError()); return 0;
				}
				char s90[1024];
				strcpy(s90, "HKEY_CLASSES_ROOT\\");
				strcat(s90, numeSubcheie);
				if (!RegOpenKeyEx(HKEY_CLASSES_ROOT, s90, 0, KEY_READ, &subKey))
				{
					printf("Eroare la deschidere subcheie.Cod eroare %d\n", GetLastError()); return 0;
				}
				else
				{
					DWORD dtype;
					dtype = REG_SZ;
					if (RegQueryValueEx(subKey, NULL, 0,&dtype, (BYTE *)subData, &size) == ERROR_SUCCESS)
					{
						char charData[1024];
						sprintf(charData, "%s", subData);
						printf("%s\n", subData);
						if (!WriteFile(h, charData, size, &dwBytesWritten, NULL))
						{
							printf("Eroare la scrierea in fisierul nou creat.\n");
						}
					}
					else
					{
					printf("Eroare la citirea valorii subcheii. Cod eroare %d\n", GetLastError());
					}
			    }
				CloseHandle(h);
				memset(numeSubcheie, 0, sizeof(numeSubcheie));
				memset(filePath, 0, sizeof(filePath));
			}
			else
				printf("Eroare la enumerare subcheie.Cod eroare %d\n", GetLastError());
		}
	}

	//5
	HKEY keyCU;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\CSSO\\WEEK1", 0, KEY_WRITE, &keyCU) == ERROR_SUCCESS)
	{  
		char valueP[] = "C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\sumar.txt";
		if (RegSetValueEx(keyCU, "valuePath", 0, REG_SZ, (CONST BYTE*)valueP, sizeof(CHAR) * (strlen(valueP) + 1)) != ERROR_SUCCESS)
		{
			printf("Eroare la creare valoare valuePath: %d\n", GetLastError());
			return 0;
		}
		HANDLE h;
		h = CreateFile("C:\\Facultate\\CSSO\\Laboratoare\\Week1\\Rezultate\\sumar.txt", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h == INVALID_HANDLE_VALUE) {
			printf("Error la deschidere fisier: %d\n", GetLastError());
			return 0;
		}
		_LARGE_INTEGER sizeSumar;
		if (!GetFileSizeEx(h,&sizeSumar))
		{
			printf("Error la aflare dimensiune fisier: %d\n", GetLastError());
			return 0;
		}
		DWORD size = sizeSumar.QuadPart;
		if (RegSetValueEx(keyCU, "valueSize", 0, REG_DWORD, (const BYTE*)&size, sizeof(size)) != ERROR_SUCCESS)
		{
			printf("Eroare la creare valoare valueSize: %d\n", GetLastError());
			return 0;
		}
		CloseHandle(h);
	}
	else
	{
		printf("Eroare la deschidere cheie: %d\n", GetLastError());
		return 0;
	}

}
