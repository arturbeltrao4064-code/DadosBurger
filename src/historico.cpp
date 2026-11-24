#include "historico.hpp"
#include <iostream>
#include <new>

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

    bool inserir(ListaContDin & l, PARTIDA novo, int pos)
    {
        if (pos > l.tamanho || pos < 0)
            return false;

        if (l.tamanho == l.capacidade)
        {
            int novaCapacidade = (l.capacidade == 0) ? 1 : l.capacidade * 2;

            PARTIDA *novoVetor = new (std::nothrow) PARTIDA[novaCapacidade];

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

    bool remover(ListaContDin & l, int pos)
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

    void destruir(ListaContDin & l)
    {
        delete[] l.historico;
        l.historico = nullptr;
        l.tamanho = 0;
        l.capacidade = 0;
    }