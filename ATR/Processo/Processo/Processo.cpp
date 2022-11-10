// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define WNT_LEAN_AND_MEAN
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <process.h>
#include <ctime>
#include <math.h>
#include <string>
#include <sstream>
#include<chrono>
#include <format>
#include <cstring>


//DEFINICAO DO VALOR NUMERICO DE CADA TECLA PARA A FUNCAO _GETCH
#define	ESC 27
#define C 99
#define S 115
#define M 109
#define A 97
#define P 112
#define Z 122
#define X 120
HANDLE hMailslot;



using namespace std;
int tecla;
double generateRandom(int inicial, int final) {
    int range = final - inicial + 1;
    int random = inicial + (rand() % range);
    return random;
};
unsigned __stdcall teclado(void* pArgument) {
    while (true) {
        tecla = _getch();
        cout << tecla << "\n";
        switch (tecla) {
        case ESC:
            cout << "TECLA ESC APERTADA\n";
            break;
        case C:
            cout << "TECLA C APERTADA\n";
            break;
        case S:
            cout << "TECLA S APERTADA\n";
            break;
        case M:
            cout << "TECLA M APERTADA\n";
            break;
        case A:
            cout << "TECLA A APERTADA\n";
            break;
        case P:
            cout << "TECLA P APERTADA\n";
            break;
        case Z:
            cout << "TECLA Z APERTADA\n";
            break;
        case X:
            cout << "TECLA X APERTADA\n";
            break;
        default:
            cout << "TECLA INVALIDA\n";
            break;
        }

    }
}
unsigned __stdcall computador_processo(void* pArgument) {
    int nseq = 0;
    DWORD dwBytesEnviados;
    BOOL bStatus;
    while (true) {
        int random_time = rand() % 5000;
        Sleep(random_time);
        char mensagem[38] = "";
        if (nseq == 99999) nseq = 0;
        nseq++;
        double sp_press_arg = generateRandom(10000, 50000) / 10;
        double sp_temp = generateRandom(9000, 16000) / 10;//900 - 1600
        double sp_press = generateRandom(10, 999) / 10; //1 99.9
        chrono::system_clock::time_point p = chrono::system_clock::now();
        time_t t = chrono::system_clock::to_time_t(p);
        char str[26] = "";
        ctime_s(str, sizeof str, &t);
        char time[9] = "";
        for (int i = 11; i <= 18; i++) {
            time[i - 11] = str[i];
        }
        char sp_press_arg_s[7] = "", sp_temp_s[7] = "", sp_press_s[5] = "", nseq_s[6] = "";
        sprintf_s(sp_press_arg_s, "%.1lf", sp_press_arg);
        sprintf_s(sp_temp_s, "%.1lf", sp_temp);
        sprintf_s(sp_press_s, "%.1lf", sp_press);
        sprintf_s(nseq_s, "%d", nseq);
        if (strlen(sp_temp_s) == 5) sprintf_s(sp_temp_s, "0%.1lf", sp_temp);
        if (strlen(sp_press_s) == 3) sprintf_s(sp_press_s, "0%.1lf", sp_press);
        if (strlen(nseq_s) == 1) {
            sprintf_s(nseq_s, "0000%d", nseq);
        }
        else if (strlen(nseq_s) == 2) {
            sprintf_s(nseq_s, "000%d", nseq);
        }
        else if (strlen(nseq_s) == 3) {
            sprintf_s(nseq_s, "00%d", nseq);
        }
        else if (strlen(nseq_s) == 4) {
            sprintf_s(nseq_s, "0%d", nseq);
        }
        sprintf_s(mensagem, "001|%s|%s|%s|%s|%s", nseq_s, sp_press_arg_s, sp_temp_s, sp_press_s, time);
        cout << mensagem << endl;
        bStatus = WriteFile(hMailslot, &mensagem, sizeof(mensagem), &dwBytesEnviados, NULL);
        printf("Bytes enviados= %d\n", dwBytesEnviados);
    }

}
int main()
{
    hMailslot = CreateFileA(
        "\\\\.\\mailslot\\MyMailslot",
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    HANDLE TreadTeclado;
    unsigned dwTreadID;
    TreadTeclado = (HANDLE)_beginthreadex(
        NULL,
        0,
        teclado,
        NULL,
        0,
        &dwTreadID
    );
    HANDLE ThreadComputadorProcesso;
    TreadTeclado = (HANDLE)_beginthreadex(
        NULL,
        0,
        computador_processo,
        NULL,
        0,
        &dwTreadID
    );
    //TRATAMENTO DE ERRO AQUI !
    while (tecla != ESC) {
    }
}