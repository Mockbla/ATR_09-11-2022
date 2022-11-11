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
HANDLE hteclaESC;
HANDLE hteclap;
HANDLE hteclax;
using namespace std;
unsigned __stdcall limpa_tela(void* pArgument) {
	
	while (1) {
		HANDLE Events[2] = { hteclaESC,hteclax };
		int tipo_evento;
		do {
			int ret = WaitForMultipleObjects(2, Events, false, INFINITE);
			tipo_evento = ret - WAIT_OBJECT_0;
			if(tipo_evento==1)
			system("cls");
			else if (tipo_evento == 0) {
				cout << "Thread de limpar a tela encerrada\n";
				_endthreadex(0);
			}
		} while (tipo_evento != 0);

	}
}
int main() {
	hteclax = OpenEventA(EVENT_ALL_ACCESS, TRUE, "tecla_x");
	if (GetLastError() != 0)cout << "O evento x nao foi aberto erro: " << GetLastError() << endl;
	hteclap = OpenEventA(EVENT_ALL_ACCESS, TRUE, "tecla_p");
	if (GetLastError() != 0)cout << "O evento p nao foi aberto erro: " << GetLastError() << endl;
	hteclaESC = OpenEventA(EVENT_ALL_ACCESS, TRUE, "tecla_ESC");
	if (GetLastError() != 0)cout << "O evento esc nao foi aberto erro: " << GetLastError() << endl;
	//CRIAR THREAD LIMPAR TELA
	printf("Criando Thread leitura do teclado...\n");
	HANDLE ThreadLeituraTeclado;
	unsigned ThreadLeituraTecladoID;
	ThreadLeituraTeclado = (HANDLE)_beginthreadex(
		NULL,
		0,
		limpa_tela,
		NULL,
		0,
		&ThreadLeituraTecladoID
	);
	if (GetLastError() != 0)cout << "Thread limpar a tela não foi criada: " << GetLastError() << endl;
	else cout << "Thread limpar a tela criada com sucesso TID: " << ThreadLeituraTecladoID << endl;


	while (1) {
		HANDLE Events[2] = { hteclaESC,hteclap };
		int tipo_evento = 1;
		do{

			int ret = WaitForMultipleObjects(2, Events, FALSE, INFINITE);//AQUI ESPERA OU PELO ESC, OU PELO EVENTO DA TECLA C (PADRAO TRUE), PARA QUE POSSA OU ENCERRAR(ESC),CONTINUAR(C),BLOQUEAR(!C)
			tipo_evento = ret - WAIT_OBJECT_0;
			if (tipo_evento == 1) {
				cout << "Tarefa de exibir mensagens do computador do processoAAA: \n" << endl; //COMO NAO ERA NECESSARIO FAZER A LIGACAO DA LISTA CIRCULAR COM ESTE PROCESSO PARA A ETAPA 1, IMPRIME TEXTO GENERICO PARA MOSTRAR QUE AS FUNCOES DE TECLADO ESTAO FUNCIONANDO
				Sleep(1000);
			}
		}while (tipo_evento != 0);
		cout << "Processo de exibir mensagens do computador do processo encerrada\nPressione qualquer tecla para fechar esta janela";
		getchar();
		return(0);
	}
}