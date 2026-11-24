
#include <iostream>
#include <new>
#include "receita.hpp"

void inicializar(ListaReceita& lista) {
    lista.ini = nullptr;
}

int tamanho(const ListaReceita& lista) {
    const NoReceita *aux = lista.ini;
    int contador = 0;

    while (aux != nullptr) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}

void imprimir(const ListaReceita& lista) {
    const NoReceita *aux = lista.ini;

    while (aux != nullptr) {
        std::cout << "Codigo: " << aux->dado.cod << '\n';
        std::cout << "Nome: " << aux->dado.nome << '\n';
        std::cout << "Preco: " << aux->dado.preco << '\n';

        aux = aux->prox;
    }
}

IngredienteReceita buscar(const ListaReceita& lista, int cod) {
    const NoReceita *aux = lista.ini;

    IngredienteReceita ingrediente = {0, "", 0.0f};

    while (aux != nullptr) {
        if (aux->dado.cod == cod)
            return aux->dado;

        aux = aux->prox;
    }

    return ingrediente;
}

int inserirInicio(ListaReceita& lista, IngredienteReceita ingrediente) {
    NoReceita *novo = new (std::nothrow) NoReceita;

    if (novo == nullptr)
        return 0;

    novo->dado = ingrediente;
    novo->prox = lista.ini;

    lista.ini = novo;

    return 1;
}

int inserirFim(ListaReceita& lista, IngredienteReceita ingrediente) {
    NoReceita *novo = new (std::nothrow) NoReceita;

    if (novo == nullptr)
        return 0;

    novo->dado = ingrediente;
    novo->prox = nullptr;

    if (lista.ini == nullptr) {
        lista.ini = novo;
    } else {
        NoReceita *aux = lista.ini;

        while (aux->prox != nullptr)
            aux = aux->prox;

        aux->prox = novo;
    }

    return 1;
}

int removerPorCod(ListaReceita& lista, int cod) {
    NoReceita *ant = nullptr;
    NoReceita *aux = lista.ini;

    while (aux != nullptr && aux->dado.cod != cod) {
        ant = aux;
        aux = aux->prox;
    }

    if (aux == nullptr)
        return 0;

    if (ant == nullptr)
        lista.ini = aux->prox;
    else
        ant->prox = aux->prox;

    delete aux;

    return 1;
}

void destruir(ListaReceita& lista) {
    NoReceita *aux = lista.ini;

    while (aux != nullptr) {
        NoReceita *removido = aux;
        aux = aux->prox;

        delete removido;
    }

    lista.ini = nullptr;
}
