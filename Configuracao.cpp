#include "Configuracao.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>

using namespace std;

int Configuracao::capacidadeCeleiro = 50;
int Configuracao::capacidadeCaminhao = 10;
int Configuracao::capacidadeDeposito = 100;
int Configuracao::quantidadeClientes = 5;

atomic<int> Configuracao::tempoProducao(2000);
atomic<int> Configuracao::tempoTransporte(3000);
atomic<int> Configuracao::tempoCompra(1000);
atomic<int> Configuracao::tempoAfazeres(2000);

atomic<bool> Configuracao::sistemaAtivo(true);

mutex Configuracao::mutexLog;

void Configuracao::log(const string& mensagem) {
    lock_guard<mutex> lock(mutexLog);

    auto now = chrono::system_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;

    auto timer = chrono::system_clock::to_time_t(now);
    tm bt;
    localtime_s(&bt, &timer);

    cout << put_time(&bt, "%H:%M:%S")
         << "." << setfill('0') << setw(3) << ms.count()
         << " | " << mensagem << endl;
}
