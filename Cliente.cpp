#include "Cliente.h"
#include "Configuracao.h"
#include <sstream>
#include <chrono>
#include <random>

using namespace std;

Cliente::Cliente(const string& n, Deposito* d)
    : nome(n), deposito(d), thread(nullptr) {}

Cliente::~Cliente() {
    parar();
}

void Cliente::executar() {
    ostringstream msg;
    msg << "[CLIENTE] " << nome << " chegou ao mercado";
    Configuracao::log(msg.str());

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distFruta(0, 2);

    while (Configuracao::sistemaAtivo) {
        ostringstream afazeresMsg;
        afazeresMsg << "[CLIENTE] " << nome << " está fazendo seus afazeres diários";
        Configuracao::log(afazeresMsg.str());

        this_thread::sleep_for(
            chrono::milliseconds(Configuracao::tempoAfazeres.load())
        );

        if (!Configuracao::sistemaAtivo) break;

        TipoFruta frutaEscolhida;
        int escolha = distFruta(gen);
        switch (escolha) {
            case 0: frutaEscolhida = TipoFruta::MELANCIA; break;
            case 1: frutaEscolhida = TipoFruta::UVA; break;
            case 2: frutaEscolhida = TipoFruta::MORANGO; break;
            default: frutaEscolhida = TipoFruta::MELANCIA;
        }

        ostringstream compraMsg;
        compraMsg << "[CLIENTE] " << nome << " quer comprar "
                  << tipoFrutaParaString(frutaEscolhida);
        Configuracao::log(compraMsg.str());

        deposito->retirar(frutaEscolhida, nome);

        this_thread::sleep_for(
            chrono::milliseconds(Configuracao::tempoCompra.load())
        );
    }

    ostringstream fimMsg;
    fimMsg << "[CLIENTE] " << nome << " saiu do mercado";
    Configuracao::log(fimMsg.str());
}

void Cliente::iniciar() {
    if (!thread) {
        thread = new std::thread(&Cliente::executar, this);
    }
}

void Cliente::parar() {
    if (thread) {
        if (thread->joinable()) {
            thread->join();
        }
        delete thread;
        thread = nullptr;
    }
}
