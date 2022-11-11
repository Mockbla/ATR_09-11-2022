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
#include <cstring>     // _getch()
#define	ESC 27
#define C 99
#define S 115
#define M 109
#define A 97
#define P 112
#define Z 122
#define X 120

//DECLARACOES DOS HANDLES QUE SERAO USADOS PARA O TECLADO

HANDLE hteclac;
HANDLE hteclas;
HANDLE hteclam;
HANDLE hteclaa;
HANDLE hteclap;
HANDLE hteclaz;
HANDLE hteclax;
HANDLE hteclaESC;
HANDLE hevent;


using namespace std;
unsigned __stdcall teclado(void* pArgument) {

    //PARA FINS DE ORGANIZAÇÃO, VAMOS INICIAR OS EVENTOS DO TECLADO AQUI MESMO. E PARA FINS DE SINCRONIZAÇÃO, NAO SÓ CONTINUAREMOS O PROCESSO QUANDO TODOS OS EVENTOS FOREM CRIADOS

    hteclac = CreateEventA(NULL, TRUE, TRUE, "tecla_c");
    hteclas = CreateEventA(NULL, TRUE, TRUE, "tecla_s");
    hteclam = CreateEventA(NULL, TRUE, TRUE, "tecla_m");
    hteclaa = CreateEventA(NULL, TRUE, TRUE, "tecla_a");
    hteclap = CreateEventA(NULL, TRUE, TRUE, "tecla_p");
    hteclaz = CreateEventA(NULL, TRUE, FALSE, "tecla_z");
    hteclax = CreateEventA(NULL, TRUE, FALSE, "tecla_x");
    hteclaESC = CreateEventA(NULL, TRUE, FALSE, "tecla_ESC");
    cout << "Eventos Criados!" << endl;
    PulseEvent(hevent);//SINALIZA PARA A MAIN, QUE PODE INICIAR AS OUTRAS THREAS QUE OS EVENTOS JA FORAM CRIADOS.


    int tecla;
    bool tecla_c = true, tecla_s = true, tecla_m = true, tecla_a = true, tecla_p = true;
    while (true) {
        tecla = _getch();
        switch (tecla) {
        case ESC:
            cout << "TECLA ESC APERTADA\n";
            SetEvent(hteclaESC);
            PulseEvent(hteclac);
            PulseEvent(hteclas);
            PulseEvent(hteclam);
            PulseEvent(hteclaa);
            PulseEvent(hteclap);



            break;
        case C:
            cout << "TECLA C APERTADA\n";
            if (tecla_c) {
                tecla_c = !tecla_c;
                ResetEvent(hteclac);
            }
            else {
                tecla_c=!tecla_c;
                SetEvent(hteclac);
            }
            break;
        case S:
            cout << "TECLA S APERTADA\n";
            if (tecla_s) {
                tecla_s = !tecla_s;
                ResetEvent(hteclas);
            }
            else {
                tecla_s = !tecla_s;
                SetEvent(hteclas);
            }
            break;
        case M:
            cout << "TECLA M APERTADA\n";//PARTE 2
            if (tecla_m) {
                tecla_m = !tecla_m;
                ResetEvent(hteclam);
            }
            else {
                tecla_m = !tecla_m;
                SetEvent(hteclam);
            }
            break;
        case A: //PARTE 2
            cout << "TECLA A APERTADA\n";
            if (tecla_a) {
                tecla_a = !tecla_a;
                ResetEvent(hteclaa);
            }
            else {
                tecla_a = !tecla_a;
                SetEvent(hteclaa);
            }
            break;
        case P:
            cout << "TECLA P APERTADA\n";
            if (tecla_p) {
                tecla_p = !tecla_p;
                ResetEvent(hteclap);
            }
            else {
                tecla_p = !tecla_p;
                SetEvent(hteclap);
            }
            break;
        case Z:
            cout << "TECLA Z APERTADA\n";
            PulseEvent(hteclaz);
            break;
        case X:
            cout << "TECLA X APERTADA\n";
            PulseEvent(hteclax);
            break;
        default:
            cout << "TECLA INVALIDA\n";
            break;
        }

    }
}
double generateRandom(int inicial, int final) {
    int range = final - inicial + 1;
    int random = inicial + (rand() % range);
    return random;
};
unsigned __stdcall computador_processo(void* pArgument) {
    int nseq = 0;
    BOOL bStatus; 
    while (1) {
        HANDLE Events[2] = { hteclaESC,hteclac };
        int tipo_evento = 1;
        do {
            int ret = WaitForMultipleObjects(2, Events, FALSE, INFINITE);//AQUI ESPERA OU PELO ESC, OU PELO EVENTO DA TECLA C (PADRAO TRUE), PARA QUE POSSA OU ENCERRAR(ESC),CONTINUAR(C),BLOQUEAR(!C)
            tipo_evento = ret - WAIT_OBJECT_0;
            if (tipo_evento == 1) {
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
                WaitForSingleObject(hteclac, INFINITE);//ESSE SEGUNDO WAITFORSINGLEOBJECT SERVE PARA EVITAR QUE SE A THREAD JA ESTIVER EM EXECUCAO QUANDO A TECLA FOR APERTADA, NAO INSERIR NA LISTA 
                std::cout << mensagem << endl;
                Sleep(generateRandom(0,5000));

            }
        } while (tipo_evento != 0);
        cout << "Thread computador de processo encerrada...\n";
        _endthreadex(0);
        return(0);
    }
}
unsigned __stdcall LeituraSCADAFunc(LPVOID index) {
    int seq = 0, seq2 = 0;
    char Mensagem[50];
    char NSEQ[6], ID[4], PRIORIDADE[3], tStamp[8], TIME[9] = "";
    char NSEQ2[6], TEMP_PANELA[7], TEMP_CAMARA[7], PRESSAO_ARG[7], PRESSAO_CAMARA[7], TIME2[9] = "";
    int buffer;
    DWORD dwBytesEnviados;
    BOOL bStatus;
    while (1) {
        HANDLE Events[2] = { hteclaESC,hteclas };
        int tipo_evento = 1;
        do {
            int ret = WaitForMultipleObjects(2, Events, FALSE, INFINITE);//AQUI ESPERA OU PELO ESC, OU PELO EVENTO DA TECLA C (PADRAO TRUE), PARA QUE POSSA OU ENCERRAR(ESC),CONTINUAR(C),BLOQUEAR(!C)
            tipo_evento = ret - WAIT_OBJECT_0;
            if (tipo_evento == 1) {
                WaitForSingleObject(hteclas, INFINITE);
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
                    WaitForSingleObject(hteclas, INFINITE);//ESSE SEGUNDO WAITFORSINGLEOBJECT SERVE PARA EVITAR QUE SE A THREAD JA ESTIVER EM EXECUCAO QUANDO A TECLA FOR APERTADA, NAO INSERIR NA LISTA 
                    cout << Mensagem << endl;
                }



                Sleep(generateRandom(1000, 5000));
            }


        } while (tipo_evento != 0);

        cout << "Thread leitura do SCADA encerrada...\n";
        _endthreadex(0);
        return(0);
    }
}
int main()
{
    BOOL bStatus;
    STARTUPINFO si;            // StartUpInformation para novo processo
    PROCESS_INFORMATION NewProcess;    // Informações sobre novo processo criado
    HANDLE hMailslot;
    DWORD dwBytesLidos;

    printf("Iniciando ...\n");
    //CRIAR THREAD TECLADO
    printf("Criando Thread leitura do teclado...\n");
    hevent = CreateEventA(NULL, FALSE, FALSE, "sincroniza_eventos_teclado");
    HANDLE ThreadLeituraTeclado;
    unsigned ThreadLeituraTecladoID;
    ThreadLeituraTeclado = (HANDLE)_beginthreadex(
        NULL,
        0,
        teclado,
        NULL,
        0,
        &ThreadLeituraTecladoID
    );
    if (GetLastError() != 0)cout << "Thread não foi criada, erro: " << GetLastError() << endl;
    else cout << "Thread leitura do teclado criada com sucesso TID: " << ThreadLeituraTecladoID << endl;
    //CRIAR THREAD PC PROCESSO
    WaitForSingleObject(hevent, INFINITE);//AQUI ESPERA A CRIACAO DE TODOS OS EVENTOS PARA CRIAR AS THREADS RESTANTES.
    printf("Criando Thread leitura comptuador procsso...\n");
    HANDLE ThreadComputadorProcesso;
    unsigned ThreadComputadorProcessoID;
    ThreadComputadorProcesso = (HANDLE)_beginthreadex(
        NULL,
        0,
        computador_processo,
        NULL,
        0,
        &ThreadComputadorProcessoID
    );
    if (GetLastError() != 0)cout << "Thread não foi criada, erro: " << GetLastError() << endl;
    else cout << "Thread leitura computador processo criada com sucesso TID: " << ThreadComputadorProcessoID << endl;
    //THREAD LEITURA SCADA
    printf("Criando Thread leitura comptuador procsso...\n");
    HANDLE ThreadLeituraScada;
    unsigned ThreadLeituraScadaID;
    ThreadLeituraScada = (HANDLE)_beginthreadex(
        NULL,
        0,
        LeituraSCADAFunc,
        NULL,
        0,
        &ThreadLeituraScadaID
    );
    if (GetLastError() != 0)cout << "Thread não foi criada, erro: " << GetLastError() << endl;
    else cout << "Thread leitura SCADA criada com sucesso TID: " << ThreadLeituraScadaID << endl;

    //CRIAR PROCESSO EXIBIR COMPUTADOR DO PROCESSO
    cout << "Criando processo para exibição do computador de processo" << endl;
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
    if (GetLastError() != 0)cout << "Processo não foi criada, erro: " << GetLastError() << endl;
    else cout << "Processo de exibicao do comptuador do processo criado: " <<endl;

    //CRIAR PROCESSO EXIBIR COMPUTADOR DO SCADA
    cout << "Criando processo para exibição SCADA" << endl;
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
        "..\\..\\Processo\\x64\\Debug",
        (LPSTARTUPINFOA)&si,            // lpStartUpInfo
        &NewProcess);    // lpProcessInformation 
    if (GetLastError() != 0)cout << "Processo não foi criada, erro: " << GetLastError() << endl;
    else cout << "Processo de exibicao do comptuador do processo criado: " << endl;


    while (1) {

    }
}







    
















    /*hMailslot = CreateMailslotA(
        "\\\\.\\mailslot\\MyMailslot",
        0,
        MAILSLOT_WAIT_FOREVER,
        NULL);*/
    // Cria processo servidor
    //ZeroMemory(&si, sizeof(si));
    //si.cb = sizeof(si);    // Tamanho da estrutura em bytes
    //bStatus = CreateProcessA(
    //    "..\\..\\SCADA\\x64\\Debug\\SCADA.exe",
    //    NULL,    // linha de comando
    //    NULL,    // atributos de segurança: Processo
    //    NULL,    // atributos de segurança: Thread
    //    FALSE,    // herança de handles
    //    CREATE_NEW_CONSOLE,    // CreationFlags
    //    NULL,    // lpEnvironment
    //    "..\\..\\SCADA\\x64\\Debug",
    //    (LPSTARTUPINFOA) & si,            // lpStartUpInfo
    //    &NewProcess);    // lpProcessInformation
    //int a= GetLastError();
    //if (a != 0) std::cout << "ERRO: " << a<<std::endl;

    //ZeroMemory(&si, sizeof(si));
    //si.cb = sizeof(si);    // Tamanho da estrutura em bytes
    //bStatus = CreateProcessA(
    //    "..\\..\\Processo\\x64\\Debug\\Processo.exe",
    //    NULL,    // linha de comando
    //    NULL,    // atributos de segurança: Processo
    //    NULL,    // atributos de segurança: Thread
    //    FALSE,    // herança de handles
    //    CREATE_NEW_CONSOLE,    // CreationFlags
    //    NULL,    // lpEnvironment
    //    "..\\..\\Processo\\x64\\Debug",
    //    (LPSTARTUPINFOA)&si,            // lpStartUpInfo
    //    &NewProcess);    // lpProcessInformation
    //a = GetLastError();
    //if (a != 0) std::cout << "ERRO: " << a << std::endl;
    ///*while (1) {
    //    char mensagem[50];
    //    while (indice < 100) {
    //        bStatus = ReadFile(hMailslot, &mensagem, sizeof(mensagem), &dwBytesLidos, NULL);
    //        int erro = GetLastError();
    //        if (erro != 0)std::cout << erro << std::endl;
    //        else {
    //            strcpy_s(lista_circular[indice], mensagem);
    //            std::cout<<"INDICE: "<<indice << lista_circular[indice] << std::endl;
    //            indice++;
    //        }
    //    }
    //    Sleep(10000);
    //    indice = 0;*/
    //    

    //}

    // Espera sincronismo para servidor e mailslot serem criados


 // FormatMsg