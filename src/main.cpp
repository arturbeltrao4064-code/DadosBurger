#include <iostream>
#include <fstream>
#include <string>
#include "avl.hpp"
#include "cache.hpp"
#include "fila.hpp"
#include "historico.hpp"
#include "montagem.hpp"
#include "pereciveis.hpp"
#include "receita.hpp"
#include "timer.hpp"

int main(int argc, char* argv[])
{
    // --- Parte do seu amigo (validação dos arquivos de entrada) ---
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0]
                  << " <pedidos.txt> <historico.txt>\n";
        return 1;
    }

    std::ifstream arquivoPedidos(argv[1]);
    if (!arquivoPedidos.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de pedidos: "
                  << argv[1] << '\n';
        return 1;
    }

    std::ifstream arquivoHistorico(argv[2]);
    if (!arquivoHistorico.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de histórico: "
                  << argv[2] << '\n';
        return 1;
    }

    // --- Sua parte (loop principal do menu) ---
    int estado = 0;

    while (estado == 0 || estado == 1 || estado == 2 || estado == 3)
    {
        if (estado == 0)
        {
            std::cout << "Novo Jogo: Pressione [1]\nComo Jogar: Pressione [2]\nHistorico: Pressione [3]\n";
            std::cin >> estado;
        }
        if (estado == 1)
        {

        }
        if (estado == 2)
        {
            while (estado != 0)
            {
                std::cout << "Bem vindo ao Dados Burger!\nObjetivo: Atender todos os clientes sem que nenhum perca a paciencia.\n"
                             "O jogo é baseado em turnos. Cada acao do jogador equivale a um turno.\n"
                             "Cada cliente tem uma paciencia maxima, definida em numero de turnos.\n"
                             "Os clientes sao organiazados em uma fila, e so e possivel atender o pedido do primeiro cliente da fila.\n"
                             "Acoes do jogador que consomem 1 turno cada:\n"
                             "[I]: Preparar ingrediente\n[A]: Adicionar ingrediente\n[R]: Remover ingrediente do topo do hamburguer.\n"
                             "Acoes de consulta (nao consomem turnos):\n"
                             "[E]: Ver estoque\n[P]: Ver pedido.\n"
                             "Pressione [0] para voltar ao menu.\n";
                std::cin >> estado;
            }
        }
        if (estado == 3)
        {

        }
    }

    return 0;
}