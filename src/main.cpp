#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "avl.hpp"
#include "historico.hpp"
#include "arquivos.hpp"
#include "partida.hpp"
#include "timer.hpp"

void escreverRelatorio(const NoAVL *raiz, int historicoCarregado)
{
    double media = 0.0;
    if (totalBuscas > 0)
        media = (double)totalComparacoesBusca / totalBuscas;

    int altura = AlturaNodo((NoAVL *)raiz) + 1;
    double taxa = 0.0;
    if (totalHits + totalMisses > 0)
        taxa = 100.0 * totalHits / (totalHits + totalMisses);

    std::ofstream log("medicoes.log", std::ios::app);
    std::cout << "\n========== RELATORIO DE DESEMPENHO ==========\n";
    std::cout << "Nos inseridos na AVL: " << totalNosInseridos << "\n";
    std::cout << "Altura da AVL: " << altura << "\n";
    std::cout << "Rotacoes na insercao: " << totalrotacoes << "\n";
    std::cout << "Entradas carregadas no historico: " << historicoCarregado << "\n";
    std::cout << "Buscas na AVL: " << totalBuscas << "\n";
    std::cout << "Comparacoes nas buscas AVL: " << totalComparacoesBusca << "\n";
    std::cout << "Media de comparacoes por busca: " << media << "\n";
    std::cout << "Tempo total das buscas AVL: " << tempoTotalBuscas << " segundos\n";
    std::cout << "Hash - tamanho da tabela: 101\n";
    std::cout << "Hash - entradas ocupadas: " << totalEntradasCache << "\n";
    std::cout << "Hash - hits: " << totalHits << ", misses: " << totalMisses << "\n";
    std::cout << "Hash - taxa de acerto: " << taxa << "%\n";
    std::cout << "Hash - fator de carga: " << (totalHits + totalMisses > 0 ? (double)totalEntradasCache / 101.0 : 0.0) << "\n";
    std::cout << "Hash - colisoes: " << totalColisoes << "\n";

    if (log.is_open())
    {
        log << "RELATORIO DE DESEMPENHO\n";
        log << "Nos inseridos na AVL: " << totalNosInseridos << "\n";
        log << "Altura da AVL: " << altura << "\n";
        log << "Rotacoes na insercao: " << totalrotacoes << "\n";
        log << "Entradas carregadas no historico: " << historicoCarregado << "\n";
        log << "Buscas na AVL: " << totalBuscas << "\n";
        log << "Comparacoes nas buscas AVL: " << totalComparacoesBusca << "\n";
        log << "Media de comparacoes por busca: " << media << "\n";
        log << "Tempo total das buscas AVL: " << tempoTotalBuscas << " segundos\n";
        log << "Hash - tamanho da tabela: 101\n";
        log << "Hash - entradas ocupadas: " << totalEntradasCache << "\n";
        log << "Hash - hits: " << totalHits << ", misses: " << totalMisses << "\n";
        log << "Hash - taxa de acerto: " << taxa << "%\n";
        log << "Hash - fator de carga: " << (double)totalEntradasCache / 101.0 << "\n";
        log << "Hash - colisoes: " << totalColisoes << "\n\n";
    }
}

int main(int argc, char *argv[])
{
    std::srand((unsigned int)std::time(nullptr));

    if (argc != 3)
    {
        std::cerr << "Uso: " << argv[0] << " <pedidos.txt> <historico.txt>\n";
        return 1;
    }

    std::ofstream logInicial("medicoes.log");
    if (!logInicial.is_open())
    {
        std::cerr << "Nao foi possivel criar medicoes.log.\n";
        return 1;
    }
    logInicial << "MEDICOES DO DADO BURGER\n";
    logInicial.close();

    NoAVL *raiz = nullptr;
    Timer timerCarga;
    timer_iniciar(&timerCarga);
    raiz = carregarPedidos(argv[1], raiz);
    double tempoCarga = timer_parar(&timerCarga);

    if (raiz == nullptr)
    {
        std::cerr << "Erro: nenhum pedido foi carregado.\n";
        return 1;
    }

    ListaContDin historico;
    inicializar(historico);
    std::ifstream arquivoHistorico(argv[2]);

    if (arquivoHistorico.is_open())
    {
        if (!carregarHistorico(arquivoHistorico, historico))
        {
            std::cerr << "Erro ao carregar o historico.\n";
            destruir(historico);
            DestruirAVL(raiz);
            return 1;
        }
        arquivoHistorico.close();
    }
    else
    {
        std::ofstream criarHistorico(argv[2]);
        if (!criarHistorico.is_open())
        {
            std::cerr << "Nao foi possivel criar o arquivo de historico.\n";
            destruir(historico);
            DestruirAVL(raiz);
            return 1;
        }
    }

    {
        std::ofstream log("medicoes.log", std::ios::app);
        log << "CARREGAMENTO INICIAL\n";
        log << "Nos inseridos na AVL: " << totalNosInseridos << "\n";
        log << "Altura da AVL: " << AlturaNodo(raiz) + 1 << "\n";
        log << "Rotacoes na insercao: " << totalrotacoes << "\n";
        log << "Tempo de carregamento da AVL: " << tempoCarga << " segundos\n";
        log << "Entradas carregadas no historico: " << historico.tamanho << "\n\n";
    }

    int historicoInicial = historico.tamanho;

    int opcao = -1;
    while (opcao != 0)
    {
        std::cout << "\n========== DADO BURGER ==========\n";
        std::cout << "[1] Novo jogo\n";
        std::cout << "[2] Como jogar\n";
        std::cout << "[3] Historico\n";
        std::cout << "[0] Sair\n";
        std::cout << "Escolha: ";

        if (!(std::cin >> opcao))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            opcao = -1;
            std::cout << "Digite uma opcao valida.\n";
            continue;
        }

        if (opcao == 1)
        {
            totalComparacoesBusca = 0;
            totalBuscas = 0;
            tempoTotalBuscas = 0.0;
            totalHits = 0;
            totalMisses = 0;
            totalColisoes = 0;
            totalEntradasCache = 0;

            Partida partida = {};
            inicializarPartida(partida, raiz, maiorCodigo(raiz));
            executarPartida(partida);

            std::cin.ignore(10000, '\n');
            std::cout << "Nome do jogador: ";
            PARTIDA resultado;
            std::getline(std::cin, resultado.nome_jogador);
            if (resultado.nome_jogador.empty())
                resultado.nome_jogador = "Jogador";
            if (resultado.nome_jogador.size() > 20)
                resultado.nome_jogador = resultado.nome_jogador.substr(0, 20);
            resultado.pontuacao = partida.pontuacao;

            if (salvarHistorico(argv[2], resultado) &&
                inserir(historico, resultado, historico.tamanho))
            {
                std::cout << "Resultado salvo no historico.\n";
            }
            else
            {
                std::cout << "Nao foi possivel registrar o resultado.\n";
            }

            escreverRelatorio(raiz, historicoInicial);

            Pedido removido;
            while (desenfileira(partida.filaClientes, removido))
            {
                destruir(removido.receita);
            }
            Ingrediente ingrediente;
            while (desempilha(partida.hamburguer, ingrediente))
            {
            }
            for (int i = 0; i < 3; i++)
                destruirHeap(partida.e.pereciveis[i]);
            encDestruir(partida.cache);
        }
        else if (opcao == 2)
        {
            std::cout << "\n========== COMO JOGAR ==========\n";
            std::cout << "Objetivo: atender todos os clientes antes que percam a paciencia.\n";
            std::cout << "Cada cliente comeca com 15 turnos de paciencia. A partida tem 10 clientes.\n";
            std::cout << "I prepara, A adiciona e R remove ingrediente; cada acao consome 1 turno.\n";
            std::cout << "E consulta o estoque e P consulta o pedido; essas consultas nao consomem turno.\n";
            std::cout << "Ingredientes: k ketchup, m mostarda, p picles, a alface, z pao de cima,\n";
            std::cout << "s queijo suico, q cheddar, c cebola, t tomate, n carne, o ovo e b bacon.\n";
            std::cout << "Prepare os ingredientes que precisam de preparo antes de adiciona-los.\n";
            std::cout << "O pao de baixo ja esta no prato e nao faz parte da receita.\n";
            std::cout << "O pao de cima fecha o hamburguer; depois dele nao se adiciona mais nada.\n";
            std::cout << "Carne, ovo e bacon vencem apos 3 turnos; o que vence primeiro sai primeiro.\n";
            std::cout << "Vitoria: atender todos os clientes. Derrota: paciencia chegar a zero.\n";
        }
        else if (opcao == 3)
        {
            mostrarHistorico(historico);
        }
        else if (opcao != 0)
        {
            std::cout << "Opcao invalida.\n";
        }
    }

    destruir(historico);
    DestruirAVL(raiz);
    std::cout << "Programa encerrado.\n";
    return 0;
}
