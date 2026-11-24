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

    // Menu principal e integracao da partida.
    int estado = 0;
    Partida partida = {};

    while (estado != 5)
    {
        if (estado == 0)
        {
            std::cout << "\n========== DADOS BURGER ==========\n";
            std::cout << "[1] Novo jogo\n";
            std::cout << "[2] Como jogar\n";
            std::cout << "[3] Carregar partida\n";
            std::cout << "[4] Historico\n";
            std::cout << "[5] Sair\n";
            std::cout << "Escolha: ";
            std::cin >> estado;
        }

        if (estado == 1)
        {
            inicializarPartida(partida, raiz, partida.id);
            std::cout << "Nova partida inicializada!\n";
            executarPartida(partida);
            estado = 0;
        }
        else if (estado == 2)
        {
            std::cout << "\nObjetivo: atender todos os clientes antes que percam a paciencia.\n";
            std::cout << "Cada cliente comeca com 5 turnos de paciencia.\n";
            std::cout << "[I] prepara ingrediente; [A] adiciona ao hamburguer;\n";
            std::cout << "[R] remove do topo; [M] entrega o pedido;\n";
            std::cout << "[E] consulta estoque; [P] consulta pedido; [S] salva e volta ao menu.\n";
            std::cout << "Digite 0 para voltar ao menu.\n";
            std::cin >> estado;
            if (estado != 0)
                estado = 0;
        }
        else if (estado == 3)
        {
            if (carregarPartida(partida, "partida.txt"))
            {
                std::cout << "Partida carregada com sucesso!\n";
                executarPartida(partida);
            }
            else
            {
                std::cout << "Nao foi possivel carregar a partida.\n";
            }
            estado = 0;
        }
        else if (estado == 4)
        {
            std::cout << "\n========== HISTORICO ==========\n";
            if (historico.tamanho == 0)
                std::cout << "Ainda nao ha partidas registradas.\n";
            for (int i = 0; i < historico.tamanho; ++i)
            {
                std::cout << i + 1 << ". " << historico.historico[i].nome_jogador
                          << " - " << historico.historico[i].pontuacao << " pontos\n";
            }
            estado = 0;
        }
        else if (estado != 0 && estado != 5)
        {
            std::cout << "Opcao invalida.\n";
            estado = 0;
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
       Y88b  d88P       d88Px
        "Y8888P"       d88P)";
    }

    return 0;
}
