#include "arquivos.hpp"
#include <iostream>
#include <string>

static bool converterInteiro(const std::string& texto, int& valor) {
    if (texto.empty()) return false;

    int sinal = 1;
    int inicio = 0;
    if (texto[0] == '-') {
        sinal = -1;
        inicio = 1;
    } else if (texto[0] == '+') {
        inicio = 1;
    }

    if (inicio == (int)texto.size()) return false;

    int numero = 0;
    for (int i = inicio; i < (int)texto.size(); i++) {
        if (texto[i] < '0' || texto[i] > '9') return false;
        numero = numero * 10 + (texto[i] - '0');
    }

    valor = numero * sinal;
    return true;
}

static void retirarCR(std::string& texto) {
    if (!texto.empty() && texto[texto.size() - 1] == '\r')
        texto.erase(texto.size() - 1);
}

NoAVL* carregarPedidos(const std::string& caminho, NoAVL* raiz) {
    std::ifstream arquivo(caminho.c_str());
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de pedidos: " << caminho << '\n';
        return raiz;
    }

    std::string linhaCodigo;
    std::string linhaReceita;

    while (std::getline(arquivo, linhaCodigo)) {
        if (!std::getline(arquivo, linhaReceita)) {
            std::cerr << "Erro: pedido com receita ausente no arquivo.\n";
            break;
        }

        retirarCR(linhaCodigo);
        retirarCR(linhaReceita);

        int codigo;
        if (!converterInteiro(linhaCodigo, codigo)) {
            std::cerr << "Codigo invalido: " << linhaCodigo << '\n';
            continue;
        }

        Pedido novoProduto;
        novoProduto.cod = codigo;
        novoProduto.nome = linhaReceita;
        raiz = Inserir(raiz, novoProduto);
    }

    arquivo.close();
    return raiz;
}

bool carregarHistorico(std::ifstream& arquivo, ListaContDin& historico) {
    std::string nome;
    std::string linhaPontuacao;

    while (std::getline(arquivo, nome)) {
        if (!std::getline(arquivo, linhaPontuacao)) {
            std::cerr << "Erro: pontuacao ausente para o jogador " << nome << '\n';
            return false;
        }

        retirarCR(nome);
        retirarCR(linhaPontuacao);

        int pontuacao;
        if (!converterInteiro(linhaPontuacao, pontuacao)) {
            std::cerr << "Pontuacao invalida: " << linhaPontuacao << '\n';
            return false;
        }

        PARTIDA novaPartida;
        novaPartida.nome_jogador = nome;
        novaPartida.pontuacao = pontuacao;

        if (!inserir(historico, novaPartida, historico.tamanho))
            return false;
    }

    return true;
}

bool salvarHistorico(const std::string& caminho, const PARTIDA& partida) {
    std::ofstream arquivo(caminho.c_str(), std::ios::app);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de historico para gravacao: "
                  << caminho << '\n';
        return false;
    }

    arquivo << partida.nome_jogador << '\n';
    arquivo << partida.pontuacao << '\n';

    if (!arquivo.good()) {
        std::cerr << "Erro ao gravar o resultado no historico.\n";
        return false;
    }

    arquivo.close();
    return true;
}
