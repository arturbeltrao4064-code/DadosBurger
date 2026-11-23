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
#include "arquivos.hpp"
#include <cstdlib>
#include <ctime>
#include "partida.hpp"

int main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));
    // --- Parte do seu amigo (validação dos arquivos de entrada) ---
    if (argc != 3)
    {
        std::cerr << "Uso: " << argv[0]
                  << " <pedidos.txt> <historico.txt>\n";
        return 1;
    }

    NoAVL *raiz = nullptr;
    raiz = carregarPedidos(argv[1], raiz);

    for (int i = 1; i <= 15; i++)
    {
        const NoAVL *encontrado = Buscar(raiz, i);

        if (encontrado != nullptr)
            std::cout << "Pedido " << i << ": "
                      << encontrado->prod.nome << '\n';
        else
            std::cout << "Pedido " << i << " nao encontrado!\n";
    }

    std::ifstream arquivoHistorico(argv[2]);
    if (!arquivoHistorico.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de histórico: "
                  << argv[2] << '\n';
        return 1;
    }

    // Inicializa a lista contígua
    ListaContDin historico;
    inicializar(historico);

    // Carrega os registros do arquivo
    if (!carregarHistorico(arquivoHistorico, historico))
    {
        std::cerr << "Erro ao carregar o histórico.\n";
        destruir(historico);
        return 1;
    }

    // --- Sua parte (loop principal do menu) ---
    int estado = 0;
    Partida partida = {};
    bool partidaCarregada = false;

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
            if (carregarPartida(partida, "partida.txt"))
            {
                std::cout << "Partida carregada com sucesso!\n";
                estado = 1;
            }
            else
            {
                std::cout << "Erro ao carregar partida.\n";
                estado = 0;
            }
        }
    }
    if (estado == 67)
    {
        std::cout << R"(.d8888b.       8888888888P
       d88P  Y88b            d88P
       888                  d88P
       888d888b.           d88P
       888P "Y88b         d88P
       888    888        d88P
       Y88b  d88P       d88P
        "Y8888P"       d88P)";
    }


return 0;
}
