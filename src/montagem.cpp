
#include <new>
#include "montagem.hpp"

void inicializa(PilhaEnc& p) {
    p.topo = nullptr;
}

bool estaVazia(const PilhaEnc& p) {
    return p.topo == nullptr;
}

bool empilha(PilhaEnc& p, const IngredientePilha& valor) {
    NoPilha *novo = new (std::nothrow) NoPilha;

    if (novo == nullptr)
        return false;

    novo->dado = valor;
    novo->prox = p.topo;

    p.topo = novo;

    return true;
}

bool desempilha(PilhaEnc& p, IngredientePilha& valorRemovido) {
    if (estaVazia(p))
        return false;

    NoPilha *removido = p.topo;

    valorRemovido = removido->dado;
    p.topo = removido->prox;

    delete removido;

    return true;
}

bool topo(const PilhaEnc& p, IngredientePilha& valor) {
    if (estaVazia(p))
        return false;

    valor = p.topo->dado;

    return true;
}

int tamanho(const PilhaEnc& p) {
    const NoPilha *aux = p.topo;
    int contador = 0;

    while (aux != nullptr) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}
