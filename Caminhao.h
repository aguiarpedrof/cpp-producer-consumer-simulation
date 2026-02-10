#ifndef CAMINHAO_H
#define CAMINHAO_H

#include "Fruta.h"
#include "Semaforo.h"
#include <mutex>
#include <map>
#include <string>

class Caminhao {
private:
    std::string proprietario;
    int capacidadeTotal;
    std::map<TipoFruta, int> carga;
    std::map<TipoFruta, Semaforo*> semaforosItens;
    std::map<TipoFruta, Semaforo*> semaforosEspacos;
    std::map<TipoFruta, std::mutex*> mutexFrutas;
    std::mutex mutexCaminhao;
    int quantidadeTotal;

public:
    Caminhao(const std::string& dono, int capacidade);
    ~Caminhao();

    void carregar(TipoFruta tipo);
    bool descarregar(TipoFruta tipo);
    int getQuantidadeTotal();
    bool estaVazio();
};

#endif
