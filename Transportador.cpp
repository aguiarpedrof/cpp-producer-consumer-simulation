#include "Transportador.h"
#include "Configuracao.h"
#include <sstream>
#include <chrono>

using namespace std;

Transportador::Transportador(const string& n, Celeiro* c, Deposito* d, Caminhao* cam)
    : nome(n), celeiro(c), deposito(d), caminhao(cam), thread(nullptr) {}

Transportador::~Transportador() {
    parar();
}

void Transportador::carregarCaminhao() {
    ostringstream msg;
    msg << "[TRANSPORTADOR] " << nome << " começou a carregar o caminhão";
    Configuracao::log(msg.str());

    for (int i = 0; i < 3; i++) {
        for (auto tipo : {TipoFruta::MELANCIA, TipoFruta::UVA, TipoFruta::MORANGO}) {
            if (celeiro->retirar(tipo, nome)) {
                caminhao->carregar(tipo);
            }
        }
    }

    ostringstream fimMsg;
    fimMsg << "[TRANSPORTADOR] " << nome << " terminou de carregar (Total: "
           << caminhao->getQuantidadeTotal() << " unidades)";
    Configuracao::log(fimMsg.str());
}

void Transportador::descarregarCaminhao() {
    ostringstream msg;
    msg << "[TRANSPORTADOR] " << nome << " chegou em Kategatt e começou a descarregar";
    Configuracao::log(msg.str());

    while (!caminhao->estaVazio()) {
        for (auto tipo : {TipoFruta::MELANCIA, TipoFruta::UVA, TipoFruta::MORANGO}) {
            if (caminhao->descarregar(tipo)) {
                deposito->armazenar(tipo, nome);
            }
        }
    }

    ostringstream fimMsg;
    fimMsg << "[TRANSPORTADOR] " << nome << " terminou de descarregar";
    Configuracao::log(fimMsg.str());
}

void Transportador::executar() {
    ostringstream msg;
    msg << "[TRANSPORTADOR] " << nome << " iniciou operações";
    Configuracao::log(msg.str());

    while (Configuracao::sistemaAtivo) {
        carregarCaminhao();

        if (!Configuracao::sistemaAtivo) break;

        ostringstream transpMsg;
        transpMsg << "[TRANSPORTADOR] " << nome << " está transportando para Kategatt...";
        Configuracao::log(transpMsg.str());

        this_thread::sleep_for(
            chrono::milliseconds(Configuracao::tempoTransporte.load())
        );

        if (!Configuracao::sistemaAtivo) break;

        descarregarCaminhao();

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    ostringstream fimMsg;
    fimMsg << "[TRANSPORTADOR] " << nome << " encerrou operações";
    Configuracao::log(fimMsg.str());
}

void Transportador::iniciar() {
    if (!thread) {
        thread = new std::thread(&Transportador::executar, this);
    }
}

void Transportador::parar() {
    if (thread) {
        if (thread->joinable()) {
            thread->join();
        }
        delete thread;
        thread = nullptr;
    }
}
