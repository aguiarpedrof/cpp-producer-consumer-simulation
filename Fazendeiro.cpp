#include "Fazendeiro.h"
#include "Configuracao.h"
#include <sstream>
#include <chrono>

using namespace std;

Fazendeiro::Fazendeiro(const string& n, TipoFruta fruta, Celeiro* c)
    : nome(n), frutaProduzida(fruta), celeiro(c), thread(nullptr) {}

Fazendeiro::~Fazendeiro() {
    parar();
}

void Fazendeiro::executar() {
    ostringstream msg;
    msg << "[FAZENDEIRO] " << nome << " começou a produzir "
        << tipoFrutaParaString(frutaProduzida);
    Configuracao::log(msg.str());

    while (Configuracao::sistemaAtivo) {
        this_thread::sleep_for(
            chrono::milliseconds(Configuracao::tempoProducao.load())
        );

        if (!Configuracao::sistemaAtivo) break;

        ostringstream prodMsg;
        prodMsg << "[FAZENDEIRO] " << nome << " produziu "
                << tipoFrutaParaString(frutaProduzida);
        Configuracao::log(prodMsg.str());

        celeiro->armazenar(frutaProduzida, nome);
    }

    ostringstream fimMsg;
    fimMsg << "[FAZENDEIRO] " << nome << " encerrou a produção";
    Configuracao::log(fimMsg.str());
}

void Fazendeiro::iniciar() {
    if (!thread) {
        thread = new std::thread(&Fazendeiro::executar, this);
    }
}

void Fazendeiro::parar() {
    if (thread) {
        if (thread->joinable()) {
            thread->join();
        }
        delete thread;
        thread = nullptr;
    }
}
