#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <cstring>
#include <chrono>


#define NUM_THREADS 3


typedef unsigned (WINAPI* CAST_FUNCTION)(LPVOID);
typedef unsigned* CAST_LPDWORD;

DWORD TID[3];

DWORD WINAPI LeituraSCADAFunc(LPVOID);
DWORD WINAPI MensagensFunc(LPVOID);
DWORD WINAPI AlarmesFunc(LPVOID);
long float generateRandom(int inicial, int final);

HANDLE
hThreads[3];

extern char listaCircular[100][50];

int i, seq = 0, seq2 = 0;

using namespace std;

int main() {
    DWORD dwThreadId;
    DWORD dwExitCode = 0;
    DWORD dwRet;

    //Criação Thread Leitura do SCADA

    hThreads[0] = (HANDLE)_beginthreadex(
        NULL,
        0,
        (CAST_FUNCTION)LeituraSCADAFunc,
        (LPVOID)i,
        0,
        (CAST_LPDWORD)&dwThreadId
    );
    TID[0] = dwThreadId;
    if (hThreads[0]) printf("ThreadLeituraSCADA Id= %0x\n", dwThreadId);

    //Criação Thread Mensagens

    hThreads[1] = (HANDLE)_beginthreadex(
        NULL,
        0,
        (CAST_FUNCTION)MensagensFunc,
        (LPVOID)i,
        0,
        (CAST_LPDWORD)&dwThreadId
    );
    TID[1] = dwThreadId;
    if (hThreads[1]) printf("ThreadMensagens Id= %0x\n", dwThreadId);

    //Criação Thread Alarmes

    hThreads[2] = (HANDLE)_beginthreadex(
        NULL,
        0,
        (CAST_FUNCTION)AlarmesFunc,
        (LPVOID)i,
        0,
        (CAST_LPDWORD)&dwThreadId
    );
    TID[2] = dwThreadId;
    if (hThreads[2]) printf("ThreadAlarmes Id= %0x\n", dwThreadId);

    // Espera todas as threads terminarem
    dwRet = WaitForMultipleObjects(NUM_THREADS, hThreads, TRUE, INFINITE);
    //ADD CHECKFORERROR

    for (i = 0; i < NUM_THREADS; ++i) {
        GetExitCodeThread(hThreads[i], &dwExitCode);
        printf("Thread %02d terminou\n", i);
        CloseHandle(hThreads[i]);	// apaga referência ao objeto
    }

    printf("\nAcione uma tecla para terminar\n");
    _getch(); // // Pare aqui, caso não esteja executando no ambiente MDS

    return EXIT_SUCCESS;
};

DWORD WINAPI LeituraSCADAFunc(LPVOID index) {
    char Mensagem[50];
    char NSEQ[6], ID[4], PRIORIDADE[3], tStamp[8], TIME[9] = "";
    char NSEQ2[6], TEMP_PANELA[7], TEMP_CAMARA[7], PRESSAO_ARG[7], PRESSAO_CAMARA[7], TIME2[9] = "";
    int buffer;

    while (1) {
        if (seq == 99999) seq = 0;
        if (seq2 == 99999) seq2 = 0;
        if (!generateRandom(0, 1)) {

            sprintf_s(NSEQ2, "%d", ++seq2);
            if (strlen(NSEQ2) == 1) {
                sprintf_s(NSEQ2, "0000%d", seq2);
            }
            else if (strlen(NSEQ2) == 2) {
                sprintf_s(NSEQ2, "000%d", seq2);
            }
            else if (strlen(NSEQ2) == 3) {
                sprintf_s(NSEQ2, "00%d", seq2);
            }
            else if (strlen(NSEQ2) == 4) {
                sprintf_s(NSEQ, "0%d", seq2);
            }

            sprintf_s(TEMP_PANELA, "%.1lf", generateRandom(10000, 16000) / 10);

            sprintf_s(TEMP_CAMARA, "%.1lf", generateRandom(1000, 16000) / 10);
            if (strlen(TEMP_CAMARA) == 5) {
                sprintf_s(TEMP_CAMARA, "0%s", TEMP_CAMARA);
            }

            sprintf_s(PRESSAO_ARG, "%.1lf", generateRandom(10000, 50000) / 10);

            double aux = generateRandom(0, 10000) / 10;
            sprintf_s(PRESSAO_CAMARA, "%.1lf", aux);
            if (strlen(PRESSAO_CAMARA) == 3) {
                sprintf_s(PRESSAO_CAMARA, "000%.1lf", aux);
            }
            else if (strlen(PRESSAO_CAMARA) == 4) {
                sprintf_s(PRESSAO_CAMARA, "00%.1lf", aux);
            }
            else if (strlen(PRESSAO_CAMARA) == 5) {
                sprintf_s(PRESSAO_CAMARA, "0%.1lf", aux);
            }

            chrono::system_clock::time_point p = chrono::system_clock::now();
            time_t t = chrono::system_clock::to_time_t(p);
            char str[26];
            ctime_s(str, sizeof str, &t);
            for (int i = 11; i <= 18; i++) {
                TIME2[i - 11] = str[i];
            }

            sprintf_s(Mensagem, "555;%s;%s;%s;%s;%s;%s", NSEQ2, TEMP_PANELA, TEMP_CAMARA, PRESSAO_ARG, PRESSAO_CAMARA, TIME2);
            cout << Mensagem << endl;
        }
        if (generateRandom(0, 1)) {
            sprintf_s(NSEQ, "%d", ++seq);
            if (strlen(NSEQ) == 1) {
                sprintf_s(NSEQ, "0000%d", seq);
            }
            else if (strlen(NSEQ) == 2) {
                sprintf_s(NSEQ, "000%d", seq);
            }
            else if (strlen(NSEQ) == 3) {
                sprintf_s(NSEQ, "00%d", seq);
            }
            else if (strlen(NSEQ) == 4) {
                sprintf_s(NSEQ, "0%d", seq);
            }

            sprintf_s(ID, "%d", 123);

            buffer = generateRandom(1, 99);
            sprintf_s(PRIORIDADE, "%d", buffer);
            if (strlen(PRIORIDADE) == 1) {
                sprintf_s(PRIORIDADE, "0%d", buffer);
            }

            chrono::system_clock::time_point p = chrono::system_clock::now();
            time_t t = chrono::system_clock::to_time_t(p);
            char str[26];
            ctime_s(str, sizeof str, &t);
            for (int i = 11; i <= 18; i++) {
                TIME[i - 11] = str[i];
            }

            sprintf_s(Mensagem, "999;%s;%s;%s;%s", NSEQ, ID, PRIORIDADE, TIME);
            cout << Mensagem << endl;
        }


        Sleep(generateRandom(1000, 5000));

    }

    return(0);
};

DWORD WINAPI MensagensFunc(LPVOID index) {
    return(0);
};

DWORD WINAPI AlarmesFunc(LPVOID index) {
    return(0);
};

long float generateRandom(int inicial, int final) {

    int random = inicial + (rand() % (final - inicial + 1));

    return random;
};

