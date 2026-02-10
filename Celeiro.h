#ifndef CELEIRO_H
#define CELEIRO_H

#include "Fruta.h"
#include "Semaforo.h"
#include <mutex>
#include <map>

class Celeiro {
private:
    int capacidadeTotal;
    std::map<TipoFruta, int> estoque;
    std::map<TipoFruta, Semaforo*> semaforosItens;
    std::map<TipoFruta, Semaforo*> semaforosEspacos;
    std::map<TipoFruta, std::mutex*> mutexFrutas;
    std::mutex mutexCeleiro;
    int quantidadeTotal;

public:
    Celeiro(int capacidade);
    ~Celeiro();

    void armazenar(TipoFruta tipo, const std::string& produtor);
    bool retirar(TipoFruta tipo, const std::string& transportador);
    void imprimirEstoque();
};

#endif
