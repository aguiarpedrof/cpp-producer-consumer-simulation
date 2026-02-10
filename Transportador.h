#ifndef TRANSPORTADOR_H
#define TRANSPORTADOR_H

#include "Celeiro.h"
#include "Deposito.h"
#include "Caminhao.h"
#include <string>
#include <thread>

class Transportador {
private:
    std::string nome;
    Celeiro* celeiro;
    Deposito* deposito;
    Caminhao* caminhao;
    std::thread* thread;

    void executar();
    void carregarCaminhao();
    void descarregarCaminhao();

public:
    Transportador(const std::string& n, Celeiro* c, Deposito* d, Caminhao* cam);
    ~Transportador();

    void iniciar();
    void parar();
};

#endif
