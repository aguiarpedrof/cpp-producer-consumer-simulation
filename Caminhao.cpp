#include "Caminhao.h"
#include "Configuracao.h"
#include <sstream>

using namespace std;

Caminhao::Caminhao(const string& dono, int capacidade)
    : proprietario(dono), capacidadeTotal(capacidade), quantidadeTotal(0) {

    int capacidadePorFruta = capacidade / 3;

    for (auto tipo : {TipoFruta::MELANCIA, TipoFruta::UVA, TipoFruta::MORANGO}) {
        carga[tipo] = 0;
        semaforosEspacos[tipo] = new Semaforo(capacidadePorFruta);
        semaforosItens[tipo] = new Semaforo(0);
        mutexFrutas[tipo] = new mutex();
    }
}

Caminhao::~Caminhao() {
    for (auto& par : semaforosEspacos) {
        delete par.second;
    }
    for (auto& par : semaforosItens) {
        delete par.second;
    }
    for (auto& par : mutexFrutas) {
        delete par.second;
    }
}

void Caminhao::carregar(TipoFruta tipo) {
    semaforosEspacos[tipo]->acquire();

    {
        lock_guard<mutex> lock(*mutexFrutas[tipo]);
        carga[tipo]++;

        lock_guard<mutex> lockTotal(mutexCaminhao);
        quantidadeTotal++;

        ostringstream msg;
        msg << "[CAMINHAO-" << proprietario << "] Carregou "
            << tipoFrutaParaString(tipo)
            << " (Carga " << tipoFrutaParaString(tipo) << ": "
            << carga[tipo] << ", Total: " << quantidadeTotal << ")";
        Configuracao::log(msg.str());
    }

    semaforosItens[tipo]->release();
}

bool Caminhao::descarregar(TipoFruta tipo) {
    semaforosItens[tipo]->acquire();

    {
        lock_guard<mutex> lock(*mutexFrutas[tipo]);

        if (carga[tipo] > 0) {
            carga[tipo]--;

            lock_guard<mutex> lockTotal(mutexCaminhao);
            quantidadeTotal--;

            ostringstream msg;
            msg << "[CAMINHAO-" << proprietario << "] Descarregou "
                << tipoFrutaParaString(tipo)
                << " (Carga " << tipoFrutaParaString(tipo) << ": "
                << carga[tipo] << ", Total: " << quantidadeTotal << ")";
            Configuracao::log(msg.str());

            semaforosEspacos[tipo]->release();
            return true;
        }
    }

    return false;
}

int Caminhao::getQuantidadeTotal() {
    lock_guard<mutex> lock(mutexCaminhao);
    return quantidadeTotal;
}

bool Caminhao::estaVazio() {
    lock_guard<mutex> lock(mutexCaminhao);
    return quantidadeTotal == 0;
}
