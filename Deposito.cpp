#include "Deposito.h"
#include "Configuracao.h"
#include <sstream>

using namespace std;

Deposito::Deposito(int capacidade)
    : capacidadeTotal(capacidade), quantidadeTotal(0) {

    int capacidadePorFruta = capacidade / 3;

    for (auto tipo : {TipoFruta::MELANCIA, TipoFruta::UVA, TipoFruta::MORANGO}) {
        estoque[tipo] = 0;
        semaforosEspacos[tipo] = new Semaforo(capacidadePorFruta);
        semaforosItens[tipo] = new Semaforo(0);
        mutexFrutas[tipo] = new mutex();
    }
}

Deposito::~Deposito() {
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

void Deposito::armazenar(TipoFruta tipo, const string& transportador) {
    semaforosEspacos[tipo]->acquire();

    {
        lock_guard<mutex> lock(*mutexFrutas[tipo]);
        estoque[tipo]++;

        lock_guard<mutex> lockTotal(mutexDeposito);
        quantidadeTotal++;

        ostringstream msg;
        msg << "[DEPOSITO] " << transportador << " armazenou "
            << tipoFrutaParaString(tipo)
            << " (Estoque " << tipoFrutaParaString(tipo) << ": "
            << estoque[tipo] << ", Total: " << quantidadeTotal << ")";
        Configuracao::log(msg.str());
    }

    semaforosItens[tipo]->release();
}

bool Deposito::retirar(TipoFruta tipo, const string& cliente) {
    semaforosItens[tipo]->acquire();

    {
        lock_guard<mutex> lock(*mutexFrutas[tipo]);

        if (estoque[tipo] > 0) {
            estoque[tipo]--;

            lock_guard<mutex> lockTotal(mutexDeposito);
            quantidadeTotal--;

            ostringstream msg;
            msg << "[DEPOSITO] " << cliente << " comprou "
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

void Deposito::imprimirEstoque() {
    lock_guard<mutex> lock(mutexDeposito);
    ostringstream msg;
    msg << "[DEPOSITO] Estoque atual - Melancia: " << estoque[TipoFruta::MELANCIA]
        << ", Uva: " << estoque[TipoFruta::UVA]
        << ", Morango: " << estoque[TipoFruta::MORANGO]
        << " (Total: " << quantidadeTotal << ")";
    Configuracao::log(msg.str());
}
