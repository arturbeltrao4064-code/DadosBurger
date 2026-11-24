#include "arquivos.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include "historico.hpp"

NoAVL* carregarPedidos(const std::string& caminho, NoAVL* raiz)
{
    std::ifstream arquivo(caminho);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de pedidos: "
                  << caminho << '\n';
        return raiz;
    }

    std::string linhaCodigo;
    std::string linhaReceita;

    while (std::getline(arquivo, linhaCodigo))
    {
        if (!std::getline(arquivo, linhaReceita))
        {
            std::cerr << "Erro: pedido com receita ausente no arquivo.\n";
            break;
        }

        try
        {
            size_t pos;
            int codigo = std::stoi(linhaCodigo, &pos);

            if (pos != linhaCodigo.size())
            {
                std::cerr << "Erro: código inválido: "
                          << linhaCodigo << '\n';
                continue;
            }

            Pedido novoProduto;
            novoProduto.cod = codigo;
            novoProduto.nome = linhaReceita;

            raiz = Inserir(raiz, novoProduto);
        }
        catch (const std::invalid_argument&)
        {
            std::cerr << "Erro: código inválido: "
                      << linhaCodigo << '\n';
        }
        catch (const std::out_of_range&)
        {
            std::cerr << "Erro: código fora do limite: "
                      << linhaCodigo << '\n';
        }
    }

    arquivo.close();
    return raiz;
}

bool carregarHistorico(std::ifstream& arquivo, ListaContDin& historico)
{
    std::string nome;
    std::string linhaPontuacao;

    while (std::getline(arquivo, nome))
    {
        if (!std::getline(arquivo, linhaPontuacao))
        {
            std::cerr << "Erro: pontuação ausente para o jogador "
                      << nome << '\n';
            return false;
        }

        try
        {
            size_t pos;
            int pontuacao = std::stoi(linhaPontuacao, &pos);

            if (pos != linhaPontuacao.size())
            {
                std::cerr << "Pontuação inválida: "
                          << linhaPontuacao << '\n';
                return false;
            }

            PARTIDA novaPartida;
            novaPartida.nome_jogador = nome;
            novaPartida.pontuacao = pontuacao;

            if (!inserir(historico, novaPartida, historico.tamanho))
                return false;
        }
        catch (...)
        {
            std::cerr << "Erro ao converter pontuação: "
                      << linhaPontuacao << '\n';
            return false;
        }
    }

    return true;
}

bool salvarHistorico(const std::string& caminho, const PARTIDA& partida)
{
    std::ofstream arquivo(caminho, std::ios::app);

    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de histórico para gravação: "
                  << caminho << '\n';
        return false;
    }

    arquivo << partida.nome_jogador << '\n';
    arquivo << partida.pontuacao << '\n';

    if (!arquivo.good())
    {
        std::cerr << "Erro ao gravar o resultado no histórico.\n";
        return false;
    }

    return true;
}
