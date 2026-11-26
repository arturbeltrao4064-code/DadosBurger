#include "historico.hpp"
#include "timer.hpp"
#include <fstream>
#include <iostream>

void inicializar(ListaContDin &l, int capacidadeIni)
{
    if (capacidadeIni < 1)
        capacidadeIni = 1;

    l.tamanho = 0;
    l.capacidade = capacidadeIni;
    l.historico = new PARTIDA[l.capacidade];

    if (l.historico == nullptr)
        l.capacidade = 0;
}

int conta_tamanho(const ListaContDin &l)
{
    return l.tamanho;
}

PARTIDA acessar(const ListaContDin &l, int pos)
{
    PARTIDA pAux;
    if (pos >= l.tamanho || pos < 0)
    {
        pAux.nome_jogador = "";
        pAux.pontuacao = -1;
    }
    else
    {
        pAux = l.historico[pos];
    }
    return pAux;
}

bool inserir(ListaContDin &l, PARTIDA novo, int pos)
{
    if (pos > l.tamanho || pos < 0)
        return false;

    if (l.tamanho == l.capacidade)
    {
        int novaCapacidade = (l.capacidade == 0) ? 1 : l.capacidade * 2;

        PARTIDA *novoVetor = new PARTIDA[novaCapacidade];

        if (novoVetor == nullptr)
            return false;

        for (int i = 0; i < l.tamanho; i++)
        {
            novoVetor[i] = l.historico[i];
        }

        delete[] l.historico;
        l.historico = novoVetor;
        l.capacidade = novaCapacidade;
    }

    for (int i = l.tamanho; i > pos; i--)
    {
        l.historico[i] = l.historico[i - 1];
    }

    l.historico[pos] = novo;
    l.tamanho++;

    return true;
}

bool remover(ListaContDin &l, int pos)
{
    if (pos >= l.tamanho || pos < 0)
        return false;

    for (int i = pos; i < l.tamanho - 1; i++)
    {
        l.historico[i] = l.historico[i + 1];
    }

    l.tamanho--;

    return true;
}

int buscar(const ListaContDin &l, const std::string nome)
{
    for (int i = 0; i < l.tamanho; i++)
    {
        if (l.historico[i].nome_jogador == nome)
        {
            return i;
        }
    }
    return -1;
}

void destruir(ListaContDin &l)
{
    delete[] l.historico;
    l.historico = nullptr;
    l.tamanho = 0;
    l.capacidade = 0;
}

int compararPartidas(const PARTIDA &a, const PARTIDA &b, int criterio)
{
    if (criterio == 1)
    {
        if (a.nome_jogador < b.nome_jogador)
            return -1;
        if (a.nome_jogador > b.nome_jogador)
            return 1;
        return 0;
    }
    if (a.pontuacao > b.pontuacao)
        return -1;
    if (a.pontuacao < b.pontuacao)
        return 1;
    return 0;
}

void registrarOrdenacao(const std::string &nome, int quantidade, double tempo)
{
    std::ofstream log("medicoes.log", std::ios::app);
    if (log.is_open())
    {
        log << "Ordenacao do historico (" << nome << ")\n";
        log << "Entradas: " << quantidade << "\n";
        log << "Tempo: " << tempo << " segundos\n\n";
    }
}

void quickSort(PARTIDA vetor[], int inicio, int fim, int criterio)
{
    int i = inicio;
    int j = fim;
    PARTIDA pivo = vetor[(inicio + fim) / 2];

    while (i <= j)
    {
        while (compararPartidas(vetor[i], pivo, criterio) < 0)
            i++;
        while (compararPartidas(vetor[j], pivo, criterio) > 0)
            j--;

        if (i <= j)
        {
            PARTIDA temporaria = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temporaria;
            i++;
            j--;
        }
    }

    if (inicio < j)
        quickSort(vetor, inicio, j, criterio);
    if (i < fim)
        quickSort(vetor, i, fim, criterio);
}

void mostrarHistorico(const ListaContDin &historico)
{
    if (historico.tamanho == 0)
    {
        std::cout << "Historico vazio.\n";
        return;
    }

    std::cout << "[1] Ordem alfabetica\n";
    std::cout << "[2] Pontuacao decrescente\n";
    std::cout << "Ordenar por: ";
    int criterio;
    std::cin >> criterio;

    if (criterio != 1 && criterio != 2)
    {
        std::cout << "Opcao invalida.\n";
        return;
    }

    PARTIDA *copia = new PARTIDA[historico.tamanho];
    if (copia == nullptr)
    {
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
    for (i = 0; i < historico.tamanho; i++)
    {
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