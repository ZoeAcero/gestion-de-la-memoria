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
            reinterpret_cast<LPCSTR>(L"MemoriaCompartida"));

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

    if (!CreateProcess(
            nullptr,
            reinterpret_cast<LPSTR>(const_cast<LPWSTR>(L"ChildProcess.exe")),
            nullptr,
            nullptr,
            FALSE,
            0,
            nullptr,
            nullptr,
            &si,
            &pi)) {
        std::cerr << "Error en CreateProcess: " << GetLastError() << std::endl;
        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    std::wcout << L"Proceso hijo - Memoria compartida: " << pBuf << std::endl;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);


    return 0;
}


