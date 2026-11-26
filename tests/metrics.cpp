#include "arquivos.hpp"
#include "avl.hpp"
#include "interface.hpp"
#include "partida.hpp"
#include "timer.hpp"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

// Interface simulada: executa a logica real de partida sem depender de
// mouse, teclado ou janela grafica. A receita e seguida na ordem do arquivo.
static const NoFila *clienteAtual = nullptr;
static const NoReceita *ingredienteAtual = nullptr;
static char ingredientePendente = '\0';
static int fase = 0;

static bool precisaPreparar(char ingrediente) {
    return ingrediente == 's' || ingrediente == 'q' ||
           ingrediente == 'c' || ingrediente == 't' ||
           ingrediente == 'n' || ingrediente == 'o' ||
           ingrediente == 'b';
}

void interfaceAtualizar(const Partida&) {}
void interfaceMensagem(const char*) {}
void interfaceMostrarEstoque(const Estoque&) {}
void interfaceMostrarPedido(const Pedido&) {}

char interfaceLerIngrediente(const Partida&, bool) {
    return ingredientePendente;
}

char interfaceLerAcao(const Partida& partida) {
    const NoFila *frente = partida.filaClientes.frente;

    if (frente != clienteAtual) {
        clienteAtual = frente;
        ingredienteAtual = frente == nullptr ? nullptr : frente->dado.receita.ini;
        fase = 0;
    }

    if (frente == nullptr) return 'S';

    if (fase == 1) {
        fase = 2;
        return 'A';
    }

    if (fase == 2) {
        ingredienteAtual = ingredienteAtual == nullptr ? nullptr : ingredienteAtual->prox;
        fase = 0;
    }

    if (ingredienteAtual == nullptr) return 'S';

    ingredientePendente = ingredienteAtual->ingrediente;
    if (precisaPreparar(ingredientePendente)) {
        fase = 1;
        return 'I';
    }

    fase = 2;
    return 'A';
}

static void limparPartida(Partida& p) {
    Pedido pedido;
    while (desenfileira(p.filaClientes, pedido)) destruir(pedido.receita);

    Ingrediente ingrediente;
    while (desempilha(p.hamburguer, ingrediente)) {}

    for (int i = 0; i < 3; i++)
        destruirHeap(p.e.pereciveis[i]);

    encDestruir(p.cache);
}

static int maiorCodigo(const NoAVL *raiz) {
    if (raiz == nullptr) return 0;
    while (raiz->dir != nullptr) raiz = raiz->dir;
    return raiz->prod.cod;
}

static int compararPartidas(const PARTIDA& a, const PARTIDA& b) {
    if (a.nome_jogador < b.nome_jogador) return -1;
    if (a.nome_jogador > b.nome_jogador) return 1;
    return 0;
}

// Mesma implementacao de Quick Sort recursivo e pivo central usada em main.cpp.
static void quickSort(PARTIDA vetor[], int inicio, int fim) {
    int i = inicio;
    int j = fim;
    PARTIDA pivo = vetor[(inicio + fim) / 2];

    while (i <= j) {
        while (compararPartidas(vetor[i], pivo) < 0) i++;
        while (compararPartidas(vetor[j], pivo) > 0) j--;

        if (i <= j) {
            PARTIDA temporaria = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temporaria;
            i++;
            j--;
        }
    }

    if (inicio < j) quickSort(vetor, inicio, j);
    if (i < fim) quickSort(vetor, i, fim);
}

// Usa registros reais do historico de desenvolvimento. A copia e refeita
// antes de cada repeticao e fica fora do intervalo cronometrado.
static double medirQuickSort(const PARTIDA dados[], int quantidade) {
    PARTIDA *copia = new PARTIDA[quantidade];
    const int repeticoes = 31;
    double tempos[repeticoes];

    for (int r = 0; r < repeticoes; r++) {
        for (int i = 0; i < quantidade; i++)
            copia[i] = dados[i];

        Timer cronometro;
        timer_iniciar(&cronometro);
        quickSort(copia, 0, quantidade - 1);
        tempos[r] = timer_parar(&cronometro);

        for (int i = 1; i < quantidade; i++) {
            if (copia[i - 1].nome_jogador > copia[i].nome_jogador) {
                std::cerr << "Erro: Quick Sort produziu ordem incorreta.\n";
                delete[] copia;
                return -1.0;
            }
        }
    }

    // Ordenacao simples dos 31 tempos para obter a mediana.
    for (int i = 1; i < repeticoes; i++) {
        double valor = tempos[i];
        int j = i - 1;
        while (j >= 0 && tempos[j] > valor) {
            tempos[j + 1] = tempos[j];
            j--;
        }
        tempos[j + 1] = valor;
    }

    double mediana = tempos[repeticoes / 2];
    delete[] copia;
    return mediana;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: metrics pedidos.txt historico.txt medicoes.log\n";
        return 1;
    }

    std::srand(20261126);

    std::ofstream log(argv[3]);
    if (!log.is_open()) {
        std::cerr << "Nao foi possivel criar o arquivo de medicoes.\n";
        return 1;
    }

    NoAVL *raiz = nullptr;
    Timer carga;
    timer_iniciar(&carga);
    raiz = carregarPedidos(argv[1], raiz);
    double tempoCarga = timer_parar(&carga);

    if (raiz == nullptr) {
        std::cerr << "Nenhum pedido foi carregado.\n";
        return 1;
    }

    const int nos = totalNosInseridos;
    const int altura = AlturaNodo(raiz) + 1;
    const int rotacoes = totalrotacoes;
    const int limiteTeorico = (int)(1.44 * std::log2((double)nos));

    ListaContDin historico;
    inicializar(historico);
    std::ifstream arquivoHistorico(argv[2]);
    if (!arquivoHistorico.is_open() ||
        !carregarHistorico(arquivoHistorico, historico) ||
        historico.tamanho < 10000) {
        std::cerr << "Nao foi possivel carregar pelo menos 10.000 registros de historico.\n";
        arquivoHistorico.close();
        destruir(historico);
        DestruirAVL(raiz);
        return 1;
    }
    arquivoHistorico.close();

    // Cria uma partida real e verifica tambem o mecanismo extra de salvar/carregar.
    Partida inicial = {};
    inicializarPartida(inicial, raiz, maiorCodigo(raiz));
    if (tamanho(inicial.filaClientes) != CLIENTES ||
        !salvarPartida(inicial, "output/partida_teste.txt")) {
        std::cerr << "Falha ao preparar ou salvar a partida de teste.\n";
        limparPartida(inicial);
        destruir(historico);
        DestruirAVL(raiz);
        return 1;
    }

    Partida partida = {};
    if (!carregarPartida(partida, "output/partida_teste.txt") ||
        tamanho(partida.filaClientes) != CLIENTES) {
        std::cerr << "Falha ao carregar a partida salva.\n";
        limparPartida(inicial);
        limparPartida(partida);
        destruir(historico);
        DestruirAVL(raiz);
        return 1;
    }
    limparPartida(inicial);
    std::remove("output/partida_teste.txt");

    clienteAtual = nullptr;
    ingredienteAtual = nullptr;
    fase = 0;

    bool salvar = false;
    bool finalizada = executarPartida(partida, salvar);
    if (!finalizada || salvar || !estaVazia(partida.filaClientes) ||
        partida.pontuacao != CLIENTES) {
        std::cerr << "A simulacao nao conseguiu atender todos os clientes. Pontos: "
                  << partida.pontuacao << "\n";
        limparPartida(partida);
        destruir(historico);
        DestruirAVL(raiz);
        return 1;
    }

    const int buscas = totalBuscas;
    const int comparacoes = totalComparacoesBusca;
    const double media = buscas > 0 ? (double)comparacoes / buscas : 0.0;
    const double taxa = (totalHits + totalMisses) > 0
        ? 100.0 * totalHits / (totalHits + totalMisses) : 0.0;
    const int entradas = partida.cache.n;
    const double fator = (double)entradas / partida.cache.m;
    const int hits = totalHits;
    const int misses = totalMisses;
    const int colisoes = totalColisoes;
    const int entradasContadas = totalEntradasCache;
    const double tempoBuscas = tempoTotalBuscas;

    log << std::fixed << std::setprecision(9);
    log << "MEDICOES AUTOMATIZADAS - VERSAO DE DESENVOLVIMENTO\n";
    log << "Arquivo de pedidos: " << argv[1] << "\n";
    log << "Arquivo de historico: " << argv[2] << "\n";
    log << "Semente aleatoria: 20261126\n";
    log << "Clientes atendidos na simulacao: " << partida.pontuacao << "\n\n";
    log << "AVL - nos inseridos: " << nos << "\n";
    log << "AVL - altura: " << altura << "\n";
    log << "AVL - limite teorico floor(1.44 log2(n)): " << limiteTeorico << "\n";
    log << "AVL - rotacoes na insercao: " << rotacoes << "\n";
    log << "AVL - tempo de carga: " << tempoCarga << " segundos\n";
    log << "AVL - total de buscas: " << buscas << "\n";
    log << "AVL - total de comparacoes nas buscas: " << comparacoes << "\n";
    log << "AVL - media de comparacoes por busca: " << media << "\n";
    log << "AVL - tempo total das buscas: " << tempoBuscas << " segundos\n\n";
    log << "HASH - tamanho da tabela (m): " << partida.cache.m << "\n";
    log << "HASH - entradas ocupadas (n): " << entradas << "\n";
    log << "HASH - fator de carga (n/m): " << fator << "\n";
    log << "HASH - hits: " << hits << "\n";
    log << "HASH - misses: " << misses << "\n";
    log << "HASH - taxa de acerto: " << taxa << "%\n";
    log << "HASH - colisoes em insercoes: " << colisoes << "\n";
    log << "HASH - contador de entradas: " << entradasContadas << "\n\n";

    limparPartida(partida);
    DestruirAVL(raiz);

    const int tamanhos[] = {1000, 5000, 10000};
    log << "ORDENACAO DO HISTORICO DE DESENVOLVIMENTO - QUICK SORT ALFABETICO\n";
    log << "Registros reais de " << argv[2]
        << "; 31 repeticoes por tamanho; mediana dos tempos.\n";
    for (int i = 0; i < 3; i++) {
        double tempo = medirQuickSort(historico.historico, tamanhos[i]);
        if (tempo < 0.0) {
            log.close();
            destruir(historico);
            return 1;
        }
        log << tamanhos[i] << " registros: " << tempo << " segundos\n";
    }

    destruir(historico);
    log.close();
    std::cout << "Simulacao concluida: " << CLIENTES
              << " clientes atendidos. Medicoes em " << argv[3] << "\n";
    return 0;
}
