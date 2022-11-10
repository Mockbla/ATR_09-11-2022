#define WNT_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <filesystem>
#include<iostream>
#include <conio.h>        // _getch()



int main()
{
    BOOL bStatus;
    STARTUPINFO si;            // StartUpInformation para novo processo
    PROCESS_INFORMATION NewProcess;    // Informações sobre novo processo criado
    HANDLE hMailslot;
    DWORD dwBytesLidos;
    char lista_circular[100][50];
    int indice = 0;
    hMailslot = CreateMailslotA(
        "\\\\.\\mailslot\\MyMailslot",
        0,
        MAILSLOT_WAIT_FOREVER,
        NULL);



    // Cria um objeto do tipo evento para sincronizar dois processos
    // O evento será sinalizado quando o servidor tiver criado o mailslot
    printf("Iniciando ...\n");

    // Cria processo servidor
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);    // Tamanho da estrutura em bytes
    bStatus = CreateProcessA(
        "..\\..\\SCADA\\x64\\Debug\\SCADA.exe",
        NULL,    // linha de comando
        NULL,    // atributos de segurança: Processo
        NULL,    // atributos de segurança: Thread
        FALSE,    // herança de handles
        CREATE_NEW_CONSOLE,    // CreationFlags
        NULL,    // lpEnvironment
        "..\\..\\SCADA\\x64\\Debug",
        (LPSTARTUPINFOA) & si,            // lpStartUpInfo
        &NewProcess);    // lpProcessInformation
    int a= GetLastError();
    if (a != 0) std::cout << "ERRO: " << a<<std::endl;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);    // Tamanho da estrutura em bytes
    bStatus = CreateProcessA(
        "..\\..\\Processo\\x64\\Debug\\Processo.exe",
        NULL,    // linha de comando
        NULL,    // atributos de segurança: Processo
        NULL,    // atributos de segurança: Thread
        FALSE,    // herança de handles
        CREATE_NEW_CONSOLE,    // CreationFlags
        NULL,    // lpEnvironment
        "..\\..\\Processo\\x64\\Debug",
        (LPSTARTUPINFOA)&si,            // lpStartUpInfo
        &NewProcess);    // lpProcessInformation
    a = GetLastError();
    if (a != 0) std::cout << "ERRO: " << a << std::endl;
    while (1) {
        char mensagem[50];
        while (indice < 100) {
            bStatus = ReadFile(hMailslot, &mensagem, sizeof(mensagem), &dwBytesLidos, NULL);
            int erro = GetLastError();
            if (erro != 0)std::cout << erro << std::endl;
            else {
                strcpy_s(lista_circular[indice], mensagem);
                std::cout<<"INDICE: "<<indice << lista_circular[indice] << std::endl;
                indice++;
            }
        }
        Sleep(10000);
        indice = 0;
        

    }

    // Espera sincronismo para servidor e mailslot serem criados


} // FormatMsg