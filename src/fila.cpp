#include "fila.hpp"

void inicializa(FilaEnc &f)
{
    f.frente = nullptr;
    f.final = nullptr;
}

bool estaVazia(const FilaEnc &f)
{
    return f.frente == nullptr;
}

bool enfileira(FilaEnc &f, const Pedido &valor)
{
    NoFila *novo = new NoFila;
    if (novo == nullptr)
        return false;

    novo->dado = valor;
    novo->prox = nullptr;

    if (f.final != nullptr)
        f.final->prox = novo;
    else
        f.frente = novo;

    f.final = novo;
    return true;
}

bool desenfileira(FilaEnc &f, Pedido &valorRemovido)
{
    if (estaVazia(f))
        return false;

    NoFila *removido = f.frente;
    valorRemovido = removido->dado;
    f.frente = removido->prox;

    if (f.frente == nullptr)
        f.final = nullptr;

    delete removido;
    return true;
}

int tamanho(const FilaEnc &f)
{
    const NoFila *aux = f.frente;
    int contador = 0;

    while (aux != nullptr)
    {
        contador++;
        aux = aux->prox;
    }

    return contador;
}
