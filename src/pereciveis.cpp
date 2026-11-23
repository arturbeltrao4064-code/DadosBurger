
#include "pereciveis.hpp"
#include <new>
#include <utility>

void inicializarHeap(heap &a, char comida, int capacidade,
                     int validadeConfig)
{
    a.comida = comida;
    a.tamanho = 0;
    a.capacidade = capacidade > 0 ? capacidade : 1;
    a.validade_config = validadeConfig;

    a.vetor = new (std::nothrow) ingrediente[a.capacidade];

    if (a.vetor == nullptr)
        a.capacidade = 0;
}

bool estaVazio(const heap &a)
{
    return a.tamanho == 0;
}

void siftUp(heap &a, int i)
{
    while (i > 0)
    {
        int pai = (i - 1) / 2;

        if (a.vetor[i].turno_validade <
            a.vetor[pai].turno_validade)
        {
            std::swap(a.vetor[i], a.vetor[pai]);
            i = pai;
        }
        else
        {
            break;
        }
    }
}

void heapify(heap &a, int i)
{
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < a.tamanho &&
        a.vetor[esq].turno_validade <
            a.vetor[menor].turno_validade)
    {
        menor = esq;
    }

    if (dir < a.tamanho &&
        a.vetor[dir].turno_validade <
            a.vetor[menor].turno_validade)
    {
        menor = dir;
    }

    if (menor != i)
    {
        std::swap(a.vetor[i], a.vetor[menor]);
        heapify(a, menor);
    }
}

bool inserir(heap &a, const ingrediente &novo, int turnoAtual)
{
    if (a.vetor == nullptr || a.tamanho >= a.capacidade)
        return false;

    ingrediente novoIngrediente = novo;

    novoIngrediente.turno_validade =
        turnoAtual + a.validade_config;

    a.vetor[a.tamanho] = novoIngrediente;

    siftUp(a, a.tamanho);

    a.tamanho++;

    return true;
}


ingrediente remover_min(heap &a)
{
    ingrediente vazio = {-1};

    if (estaVazio(a))
        return vazio;

    ingrediente menor = a.vetor[0];

    a.tamanho--;

    if (a.tamanho > 0)
    {
        a.vetor[0] = a.vetor[a.tamanho];
        heapify(a, 0);
    }

    return menor;
}

void buildHeap(heap &a)
{
    for (int i = a.tamanho / 2 - 1; i >= 0; i--)
    {
        heapify(a, i);
    }
}

void destruirHeap(heap &a)
{
    delete[] a.vetor;

    a.vetor = nullptr;
    a.tamanho = 0;
    a.capacidade = 0;
    a.validade_config = 0;
    a.comida = '\0';
}
