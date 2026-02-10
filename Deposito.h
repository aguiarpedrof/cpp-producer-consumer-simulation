#ifndef DEPOSITO_H
#define DEPOSITO_H

#include "Fruta.h"
#include "Semaforo.h"
#include <mutex>
#include <map>

class Deposito {
private:
    int capacidadeTotal;
    std::map<TipoFruta, int> estoque;
    std::map<TipoFruta, Semaforo*> semaforosItens;
    std::map<TipoFruta, Semaforo*> semaforosEspacos;
    std::map<TipoFruta, std::mutex*> mutexFrutas;
    std::mutex mutexDeposito;
    int quantidadeTotal;

public:
    Deposito(int capacidade);
    ~Deposito();

    void armazenar(TipoFruta tipo, const std::string& transportador);
    bool retirar(TipoFruta tipo, const std::string& cliente);
    void imprimirEstoque();
};

#endif
