#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstring>

#define SIZE 4096

int main() {

    HANDLE hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            SIZE,
            L"MemoriaCompartida");

    if (hMapFile == NULL) {
        std::cerr << "Error en CreateFileMapping: " << GetLastError() << std::endl;
        return 1;
    }


    LPCTSTR pBuf = (LPTSTR)MapViewOfFile(
            hMapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            SIZE);

    if (pBuf == NULL) {
        std::cerr << "Error en MapViewOfFile: " << GetLastError() << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    CopyMemory((PVOID)pBuf, L"Hola desde el proceso padre", sizeof(L"Hola desde el proceso padre"));

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));


