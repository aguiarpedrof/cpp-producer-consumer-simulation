#include "Celeiro.h"
#include "Configuracao.h"
#include <sstream>

using namespace std;

Celeiro::Celeiro(int capacidade)
    : capacidadeTotal(capacidade), quantidadeTotal(0) {

    int capacidadePorFruta = capacidade / 3;

    for (auto tipo : {TipoFruta::MELANCIA, TipoFruta::UVA, TipoFruta::MORANGO}) {
        estoque[tipo] = 0;
        semaforosEspacos[tipo] = new Semaforo(capacidadePorFruta);
        semaforosItens[tipo] = new Semaforo(0);
        mutexFrutas[tipo] = new mutex();
    }
}

Celeiro::~Celeiro() {
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

void Celeiro::armazenar(TipoFruta tipo, const string& produtor) {
    semaforosEspacos[tipo]->acquire();

    {
        lock_guard<mutex> lock(*mutexFrutas[tipo]);
        estoque[tipo]++;

        lock_guard<mutex> lockTotal(mutexCeleiro);
        quantidadeTotal++;

        ostringstream msg;
        msg << "[CELEIRO] " << produtor << " armazenou "
            << tipoFrutaParaString(tipo)
            << " (Estoque " << tipoFrutaParaString(tipo) << ": "
            << estoque[tipo] << ", Total: " << quantidadeTotal << ")";
        Configuracao::log(msg.str());
    }

    semaforosItens[tipo]->release();
}

bool Celeiro::retirar(TipoFruta tipo, const string& transportador) {
    semaforosItens[tipo]->acquire();

    {
        lock_guard<mutex> lock(*mutexFrutas[tipo]);

        if (estoque[tipo] > 0) {
            estoque[tipo]--;

            lock_guard<mutex> lockTotal(mutexCeleiro);
            quantidadeTotal--;

            ostringstream msg;
            msg << "[CELEIRO] " << transportador << " retirou "
                << tipoFrutaParaString(tipo)
                << " (Estoque " << tipoFrutaParaString(tipo) << ": "
                << estoque[tipo] << ", Total: " << quantidadeTotal << ")";
            Configuracao::log(msg.str());

            semaforosEspacos[tipo]->release();
            return true;
        }
    }

    return false;
}

void Celeiro::imprimirEstoque() {
    lock_guard<mutex> lock(mutexCeleiro);
    ostringstream msg;
    msg << "[CELEIRO] Estoque atual - Melancia: " << estoque[TipoFruta::MELANCIA]
        << ", Uva: " << estoque[TipoFruta::UVA]
        << ", Morango: " << estoque[TipoFruta::MORANGO]
        << " (Total: " << quantidadeTotal << ")";
    Configuracao::log(msg.str());
}
