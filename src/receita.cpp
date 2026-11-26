#include "receita.hpp"

void inicializar(ListaReceita &lista)
{
    lista.ini = nullptr;
}

bool inserirFim(ListaReceita &lista, char ingrediente)
{
    NoReceita *novo = new NoReceita;
    if (novo == nullptr)
        return false;
    novo->ingrediente = ingrediente;
    novo->prox = nullptr;

    if (lista.ini == nullptr)
    {
        lista.ini = novo;
    }
    else
    {
        NoReceita *aux = lista.ini;
        while (aux->prox != nullptr)
            aux = aux->prox;
        aux->prox = novo;
    }
    return true;
}

void destruir(ListaReceita &lista)
{
    NoReceita *aux = lista.ini;
    while (aux != nullptr)
    {
        NoReceita *removido = aux;
        aux = aux->prox;
        delete removido;
    }
    lista.ini = nullptr;
}
