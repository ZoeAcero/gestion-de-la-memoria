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