#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "avl.hpp"
#include "historico.hpp"
#include "arquivos.hpp"
#include "partida.hpp"
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    std::srand(std::time(nullptr));

    if (argc != 3)
    {
        std::cerr << "Uso: " << argv[0]
                  << " <pedidos.txt> <historico.txt>\n";
        return 1;
    }

    NoAVL *raiz = nullptr;
    raiz = carregarPedidos(argv[1], raiz);

    if (raiz == nullptr)
    {
        std::cerr << "Erro: nenhum pedido foi carregado.\n";
        return 1;
    }

    std::ifstream arquivoHistorico(argv[2]);
    if (!arquivoHistorico.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de histórico: "
                  << argv[2] << '\n';
        return 1;
    }

    ListaContDin historico;
    inicializar(historico);

    if (!carregarHistorico(arquivoHistorico, historico))
    {
        std::cerr << "Erro ao carregar o histórico.\n";
        destruir(historico);
        return 1;
    }
    arquivoHistorico.close();

    Partida partida = {};

    // Executa uma partida e registra o resultado somente quando ela termina.
    auto executarERegistrar = [&](Partida& jogo)
    {
        bool finalizada = executarPartida(jogo);

        if (!finalizada)
            return;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        PARTIDA resultado;
        std::cout << "Nome do jogador: ";
        std::getline(std::cin, resultado.nome_jogador);

        if (resultado.nome_jogador.empty())
            resultado.nome_jogador = "Jogador";

        resultado.pontuacao = jogo.pontuacao;

        if (!salvarHistorico(argv[2], resultado))
        {
            std::cout << "Nao foi possivel salvar o resultado no arquivo.\n";
            return;
        }

        if (!inserir(historico, resultado, historico.tamanho))
        {
            std::cout << "Resultado salvo no arquivo, mas nao foi possivel atualizar a lista em memoria.\n";
            return;
        }

        std::cout << "Resultado registrado no historico!\n";
    };

    int estado = 0;

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
            executarERegistrar(partida);
            estado = 0;
        }
        else if (estado == 2)
        {
            std::cout << "\nObjetivo: atender todos os clientes antes que percam a paciencia.\n";
            std::cout << "Cada cliente comeca com 15 turnos de paciencia.\n";
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
                executarERegistrar(partida);
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

    destruir(historico);
    return 0;
}
