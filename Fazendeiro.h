#ifndef FAZENDEIRO_H
#define FAZENDEIRO_H

#include "Fruta.h"
#include "Celeiro.h"
#include <string>
#include <thread>

class Fazendeiro {
private:
    std::string nome;
    TipoFruta frutaProduzida;
    Celeiro* celeiro;
    std::thread* thread;

    void executar();

public:
    Fazendeiro(const std::string& n, TipoFruta fruta, Celeiro* c);
    ~Fazendeiro();

    void iniciar();
    void parar();
};

#endif
