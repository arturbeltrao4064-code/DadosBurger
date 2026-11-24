#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include "avl.hpp"
#include "historico.hpp"
#include "arquivos.hpp"
#include "partida.hpp"
#include "timer.hpp"

static int maiorCodigo(const NoAVL *no) {
    if (no == nullptr) return 0;
    while (no->dir != nullptr) no = no->dir;
    return no->prod.cod;
}

static int compararPartidas(const PARTIDA& a, const PARTIDA& b, int criterio) {
    if (criterio == 1) {
        if (a.nome_jogador < b.nome_jogador) return -1;
        if (a.nome_jogador > b.nome_jogador) return 1;
        return 0;
    }
    if (a.pontuacao > b.pontuacao) return -1;
    if (a.pontuacao < b.pontuacao) return 1;
    return 0;
}

static void quickSort(PARTIDA vetor[], int inicio, int fim, int criterio) {
    int i = inicio;
    int j = fim;
    PARTIDA pivo = vetor[(inicio + fim) / 2];

    while (i <= j) {
        while (compararPartidas(vetor[i], pivo, criterio) < 0) i++;
        while (compararPartidas(vetor[j], pivo, criterio) > 0) j--;

        if (i <= j) {
            PARTIDA temporaria = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temporaria;
            i++;
            j--;
        }
    }

    if (inicio < j) quickSort(vetor, inicio, j, criterio);
    if (i < fim) quickSort(vetor, i, fim, criterio);
}

static void registrarOrdenacao(const std::string& nome, int quantidade, double tempo) {
    std::ofstream log("medicoes.log", std::ios::app);
    if (log.is_open()) {
        log << "Ordenacao do historico (" << nome << ")\n";
        log << "Entradas: " << quantidade << "\n";
        log << "Tempo: " << tempo << " segundos\n\n";
    }
}

static void mostrarHistorico(const ListaContDin& historico) {
    if (historico.tamanho == 0) {
        std::cout << "Historico vazio.\n";
        return;
    }

    std::cout << "[1] Ordem alfabetica\n";
    std::cout << "[2] Pontuacao decrescente\n";
    std::cout << "Ordenar por: ";
    int criterio;
    std::cin >> criterio;

    if (criterio != 1 && criterio != 2) {
        std::cout << "Opcao invalida.\n";
        return;
    }

    PARTIDA *copia = new PARTIDA[historico.tamanho];
    if (copia == nullptr) {
        std::cout << "Nao foi possivel criar copia do historico.\n";
        return;
    }

    int i;
    for (i = 0; i < historico.tamanho; i++)
        copia[i] = historico.historico[i];

    Timer timer;
    timer_iniciar(&timer);
    quickSort(copia, 0, historico.tamanho - 1, criterio);
    double tempo = timer_parar(&timer);

    std::cout << "\n========== HISTORICO =========\n";
    for (i = 0; i < historico.tamanho; i++) {
        std::cout << i + 1 << ". " << copia[i].nome_jogador
                  << " - " << copia[i].pontuacao << " pontos\n";
    }

    if (criterio == 1)
        registrarOrdenacao("Quick Sort alfabetico", historico.tamanho, tempo);
    else
        registrarOrdenacao("Quick Sort por pontuacao", historico.tamanho, tempo);

    std::cout << "Tempo do Quick Sort: " << tempo << " segundos\n";
    delete[] copia;
}

static void mostrarComoJogar() {
    std::cout << "\n========== COMO JOGAR ==========\n";
    std::cout << "Objetivo: atender todos os clientes antes que percam a paciencia.\n";
    std::cout << "Cada cliente comeca com 15 turnos de paciencia.\n";
    std::cout << "Acoes que consomem turno: preparar, adicionar e remover ingrediente.\n";
    std::cout << "Consultas de pedido e estoque nao consomem turno.\n";
    std::cout << "Ingredientes: k ketchup, m mostarda, p picles, a alface, z pao de cima,\n";
    std::cout << "s queijo suico, q cheddar, c cebola, t tomate, n carne, o ovo e b bacon.\n";
    std::cout << "Prepare os ingredientes que precisam de preparo antes de adiciona-los.\n";
    std::cout << "O pao de cima fecha o hamburguer. A receita deve corresponder exatamente.\n";
    std::cout << "Vitoria: atender todos os clientes. Derrota: paciencia chegar a zero.\n";
}

static void escreverRelatorio(const NoAVL *raiz, int historicoCarregado) {
    double media = 0.0;
    if (totalBuscas > 0)
        media = (double)totalComparacoesBusca / totalBuscas;

    int altura = AlturaNodo((NoAVL*)raiz) + 1;
    double taxa = 0.0;
    if (totalHits + totalMisses > 0)
        taxa = 100.0 * totalHits / (totalHits + totalMisses);

    std::ofstream log("medicoes.log", std::ios::app);
    std::cout << "\n========== RELATORIO DE DESEMPENHO ==========\n";
    std::cout << "Nos inseridos na AVL: " << totalNosInseridos << "\n";
    std::cout << "Altura da AVL: " << altura << "\n";
    std::cout << "Rotacoes na insercao: " << totalrotacoes << "\n";
    std::cout << "Entradas carregadas no historico: " << historicoCarregado << "\n";
    std::cout << "Comparacoes nas buscas AVL: " << totalComparacoesBusca << "\n";
    std::cout << "Media de comparacoes por busca: " << media << "\n";
    std::cout << "Tempo total das buscas AVL: " << tempoTotalBuscas << " segundos\n";
    std::cout << "Hash - hits: " << totalHits << ", misses: " << totalMisses << "\n";
    std::cout << "Hash - taxa de acerto: " << taxa << "%\n";
    std::cout << "Hash - fator de carga: " << (totalHits + totalMisses > 0 ? (double)totalEntradasCache / 101.0 : 0.0) << "\n";
    std::cout << "Hash - colisoes: " << totalColisoes << "\n";

    if (log.is_open()) {
        log << "RELATORIO DE DESEMPENHO\n";
        log << "Nos inseridos na AVL: " << totalNosInseridos << "\n";
        log << "Altura da AVL: " << altura << "\n";
        log << "Rotacoes na insercao: " << totalrotacoes << "\n";
        log << "Entradas carregadas no historico: " << historicoCarregado << "\n";
        log << "Comparacoes nas buscas AVL: " << totalComparacoesBusca << "\n";
        log << "Media de comparacoes por busca: " << media << "\n";
        log << "Tempo total das buscas AVL: " << tempoTotalBuscas << " segundos\n";
        log << "Hash - hits: " << totalHits << ", misses: " << totalMisses << "\n";
        log << "Hash - taxa de acerto: " << taxa << "%\n";
        log << "Hash - fator de carga: " << (double)totalEntradasCache / 101.0 << "\n";
        log << "Hash - colisoes: " << totalColisoes << "\n\n";
    }
}

int main(int argc, char *argv[]) {
    std::srand((unsigned int)std::time(nullptr));

    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <pedidos.txt> <historico.txt>\n";
        return 1;
    }

    std::ofstream logInicial("medicoes.log");
    if (!logInicial.is_open()) {
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

    if (raiz == nullptr) {
        std::cerr << "Erro: nenhum pedido foi carregado.\n";
        return 1;
    }

    ListaContDin historico;
    inicializar(historico);
    std::ifstream arquivoHistorico(argv[2]);

    if (arquivoHistorico.is_open()) {
        if (!carregarHistorico(arquivoHistorico, historico)) {
            std::cerr << "Erro ao carregar o historico.\n";
            destruir(historico);
            DestruirAVL(raiz);
            return 1;
        }
        arquivoHistorico.close();
    } else {
        std::ofstream criarHistorico(argv[2]);
        if (!criarHistorico.is_open()) {
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

    int opcao = 0;
    while (opcao != 0) {
        std::cout << "\n========== DADO BURGER ==========\n";
        std::cout << "[1] Novo jogo\n";
        std::cout << "[2] Como jogar\n";
        std::cout << "[3] Historico\n";
        std::cout << "[4] Sair\n";
        std::cout << "Escolha: ";

        if (!(std::cin >> opcao)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            opcao = 0;
            std::cout << "Digite uma opcao valida.\n";
            continue;
        }

        if (opcao == 1) {
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

            std::cout << "Nome do jogador (sem espacos): ";
            PARTIDA resultado;
            std::cin >> resultado.nome_jogador;
            if (resultado.nome_jogador.size() > 20)
                resultado.nome_jogador = resultado.nome_jogador.substr(0, 20);
            resultado.pontuacao = partida.pontuacao;

            if (salvarHistorico(argv[2], resultado) &&
                inserir(historico, resultado, historico.tamanho)) {
                std::cout << "Resultado salvo no historico.\n";
            } else {
                std::cout << "Nao foi possivel registrar o resultado.\n";
            }

            escreverRelatorio(raiz, historico.tamanho);

            Pedido removido;
            while (desenfileira(partida.filaClientes, removido)) {}
            Ingrediente ingrediente;
            while (desempilha(partida.hamburguer, ingrediente)) {}
            for (int i = 0; i < 3; i++) destruirHeap(partida.e.pereciveis[i]);
            encDestruir(partida.cache);
        }
        else if (opcao == 2) {
            mostrarComoJogar();
        }
        else if (opcao == 3) {
            mostrarHistorico(historico);
        }
        else if (opcao != 0) {
            std::cout << "Opcao invalida.\n";
        }
    }

    destruir(historico);
    DestruirAVL(raiz);
    std::cout << "Programa encerrado.\n";
    return 0;
}
