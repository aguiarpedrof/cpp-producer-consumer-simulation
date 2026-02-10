#ifndef CONFIGURACAO_H
#define CONFIGURACAO_H

#include <mutex>
#include <atomic>

class Configuracao {
public:
    static int capacidadeCeleiro;
    static int capacidadeCaminhao;
    static int capacidadeDeposito;
    static int quantidadeClientes;

    static std::atomic<int> tempoProducao;
    static std::atomic<int> tempoTransporte;
    static std::atomic<int> tempoCompra;
    static std::atomic<int> tempoAfazeres;

    static std::atomic<bool> sistemaAtivo;

    static std::mutex mutexLog;

    static void log(const std::string& mensagem);
};

#endif
