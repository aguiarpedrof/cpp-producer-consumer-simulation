#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include "Configuracao.h"
#include "Celeiro.h"
#include "Deposito.h"
#include "Caminhao.h"
#include "Fazendeiro.h"
#include "Transportador.h"
#include "Cliente.h"

using namespace std;

void menuConfiguracao();
void exibirMenu();
void alterarTempos();
void exibirEstados(Celeiro* celeiro, Deposito* deposito);

int main() {
    cout << "========================================" << endl;
    cout << "SISTEMA DE CADEIA DE PRODUCAO DE FRUTAS" << endl;
    cout << "========================================" << endl;
    cout << endl;

    menuConfiguracao();

    cout << "\n[SISTEMA] Inicializando sistema..." << endl;
    cout << "[SISTEMA] Capacidade Celeiro: " << Configuracao::capacidadeCeleiro << endl;
    cout << "[SISTEMA] Capacidade Caminhao: " << Configuracao::capacidadeCaminhao << endl;
    cout << "[SISTEMA] Capacidade Deposito: " << Configuracao::capacidadeDeposito << endl;
    cout << "[SISTEMA] Quantidade Clientes: " << Configuracao::quantidadeClientes << endl;
    cout << endl;

    Celeiro celeiro(Configuracao::capacidadeCeleiro);
    Deposito deposito(Configuracao::capacidadeDeposito);

    Caminhao caminhaoP("Pedro", Configuracao::capacidadeCaminhao);
    Caminhao caminhaoB("Bino", Configuracao::capacidadeCaminhao);

    Fazendeiro tiago("Tiago", TipoFruta::MELANCIA, &celeiro);
    Fazendeiro joao("Joao", TipoFruta::UVA, &celeiro);
    Fazendeiro alice("Alice", TipoFruta::MORANGO, &celeiro);

    Transportador pedro("Pedro", &celeiro, &deposito, &caminhaoP);
    Transportador bino("Bino", &celeiro, &deposito, &caminhaoB);

    vector<Cliente*> clientes;
    for (int i = 1; i <= Configuracao::quantidadeClientes; i++) {
        ostringstream nome;
        nome << "Cliente-" << i;
        clientes.push_back(new Cliente(nome.str(), &deposito));
    }

    cout << "[SISTEMA] Iniciando threads..." << endl;
    cout << endl;

    tiago.iniciar();
    joao.iniciar();
    alice.iniciar();

    pedro.iniciar();
    bino.iniciar();

    for (auto& cliente : clientes) {
        cliente->iniciar();
    }

    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n========================================" << endl;
    cout << "SISTEMA ATIVO - Digite 'help' para ver comandos" << endl;
    cout << "========================================\n" << endl;

    string comando;
    while (Configuracao::sistemaAtivo) {
        cout << "> ";
        getline(cin, comando);

        if (comando == "help" || comando == "h") {
            exibirMenu();
        }
        else if (comando == "status" || comando == "s") {
            exibirEstados(&celeiro, &deposito);
        }
        else if (comando == "tempo" || comando == "t") {
            alterarTempos();
        }
        else if (comando == "sair" || comando == "exit" || comando == "q") {
            cout << "\n[SISTEMA] Encerrando sistema..." << endl;
            Configuracao::sistemaAtivo = false;
        }
        else if (!comando.empty()) {
            cout << "Comando desconhecido. Digite 'help' para ver os comandos." << endl;
        }
    }

    cout << "[SISTEMA] Aguardando encerramento das threads..." << endl;

    tiago.parar();
    joao.parar();
    alice.parar();
    pedro.parar();
    bino.parar();

    for (auto& cliente : clientes) {
        cliente->parar();
        delete cliente;
    }

    cout << "\n[SISTEMA] Sistema encerrado com sucesso!" << endl;

    return 0;
}

void menuConfiguracao() {
    cout << "Deseja usar configuracoes padrao? (s/n): ";
    string resposta;
    getline(cin, resposta);

    if (resposta == "n" || resposta == "N") {
        cout << "\nCapacidade do Celeiro [50]: ";
        string input;
        getline(cin, input);
        if (!input.empty()) {
            Configuracao::capacidadeCeleiro = stoi(input);
        }

        cout << "Capacidade do Caminhao [10]: ";
        getline(cin, input);
        if (!input.empty()) {
            Configuracao::capacidadeCaminhao = stoi(input);
        }

        cout << "Capacidade do Deposito [100]: ";
        getline(cin, input);
        if (!input.empty()) {
            Configuracao::capacidadeDeposito = stoi(input);
        }

        cout << "Quantidade de Clientes [5]: ";
        getline(cin, input);
        if (!input.empty()) {
            Configuracao::quantidadeClientes = stoi(input);
        }

        cout << "\nTempo de Producao em ms [2000]: ";
        getline(cin, input);
        if (!input.empty()) {
            Configuracao::tempoProducao = stoi(input);
        }

        cout << "Tempo de Transporte em ms [3000]: ";
        getline(cin, input);
        if (!input.empty()) {
            Configuracao::tempoTransporte = stoi(input);
        }

        cout << "Tempo de Compra em ms [1000]: ";
        getline(cin, input);
        if (!input.empty()) {
            Configuracao::tempoCompra = stoi(input);
        }

        cout << "Tempo de Afazeres em ms [2000]: ";
        getline(cin, input);
        if (!input.empty()) {
            Configuracao::tempoAfazeres = stoi(input);
        }
    }
}

void exibirMenu() {
    cout << "\n========== COMANDOS DISPONIVEIS ==========" << endl;
    cout << "help, h      - Exibir este menu" << endl;
    cout << "status, s    - Exibir estado dos depositos" << endl;
    cout << "tempo, t     - Alterar tempos de execucao" << endl;
    cout << "sair, exit, q - Encerrar o sistema" << endl;
    cout << "==========================================\n" << endl;
}

void alterarTempos() {
    cout << "\n========== ALTERAR TEMPOS ==========" << endl;
    cout << "Tempos atuais:" << endl;
    cout << "1. Producao: " << Configuracao::tempoProducao.load() << " ms" << endl;
    cout << "2. Transporte: " << Configuracao::tempoTransporte.load() << " ms" << endl;
    cout << "3. Compra: " << Configuracao::tempoCompra.load() << " ms" << endl;
    cout << "4. Afazeres: " << Configuracao::tempoAfazeres.load() << " ms" << endl;
    cout << "0. Voltar" << endl;
    cout << "\nEscolha o tempo a alterar: ";

    string opcao;
    getline(cin, opcao);

    if (opcao == "0") return;

    cout << "Novo valor (ms): ";
    string valor;
    getline(cin, valor);

    if (!valor.empty()) {
        int novoTempo = stoi(valor);

        if (opcao == "1") {
            Configuracao::tempoProducao = novoTempo;
            cout << "Tempo de producao alterado para " << novoTempo << " ms" << endl;
        }
        else if (opcao == "2") {
            Configuracao::tempoTransporte = novoTempo;
            cout << "Tempo de transporte alterado para " << novoTempo << " ms" << endl;
        }
        else if (opcao == "3") {
            Configuracao::tempoCompra = novoTempo;
            cout << "Tempo de compra alterado para " << novoTempo << " ms" << endl;
        }
        else if (opcao == "4") {
            Configuracao::tempoAfazeres = novoTempo;
            cout << "Tempo de afazeres alterado para " << novoTempo << " ms" << endl;
        }
    }
    cout << "===================================\n" << endl;
}

void exibirEstados(Celeiro* celeiro, Deposito* deposito) {
    cout << "\n========== ESTADO DOS DEPOSITOS ==========" << endl;
    celeiro->imprimirEstoque();
    deposito->imprimirEstoque();
    cout << "=========================================\n" << endl;
}
