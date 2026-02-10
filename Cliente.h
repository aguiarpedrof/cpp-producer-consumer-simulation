#ifndef CLIENTE_H
#define CLIENTE_H

#include "Deposito.h"
#include <string>
#include <thread>

class Cliente {
private:
    std::string nome;
    Deposito* deposito;
    std::thread* thread;

    void executar();

public:
    Cliente(const std::string& n, Deposito* d);
    ~Cliente();

    void iniciar();
    void parar();
};

#endif
