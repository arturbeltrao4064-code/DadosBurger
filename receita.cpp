#include <iostream>
#include "receita.hpp"

void inicializar(ListaEnc& lista) {
    lista.ini = nullptr;
}

int tamanho(const ListaEnc& lista) {
    Nodo *aux = lista.ini;
    int contador = 0;

    while (aux != nullptr) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}

void imprimir(const ListaEnc& lista) {
    Nodo *aux;
    aux = lista.ini;

    while (aux != nullptr) {
        std::cout << "Codigo: " << aux->dado.cod<< '\n';
        std::cout << "Nome: " << aux->dado.nome << '\n';
        std::cout << "Preco: " << aux->dado.preco << '\n';
        aux = aux->prox;
    }
}

Produto buscar(const ListaEnc& lista, int cod) {
    Nodo *aux;
    Produto prod = {0, "", 0.0f};
    aux = lista.ini;

    while (aux != nullptr) {
        if (aux->dado.cod == cod)
            return aux->dado;
        aux = aux->prox;
    }

    return prod; // não encontrado: produto "vazio"
}

int inserirInicio(ListaEnc& lista, Produto prod) {
    Nodo *novo = new (std::nothrow) Nodo;
    if (novo == nullptr)
        return 0; // falha na alocação

    novo->dado = prod;
    novo->prox = lista.ini; // o novo nodo aponta para o antigo início
    lista.ini = novo;        // o novo nodo passa a ser o início

    return 1;
}

int inserirFim(ListaEnc& lista, Produto prod) {
    Nodo *novo;
    Nodo *aux;

    novo = new (std::nothrow)Nodo;
    if (novo == nullptr)
        return 0;

    novo->dado = prod;
    novo->prox = nullptr;

    if (lista.ini == nullptr) { // lista vazia: o novo nodo é o único (e o início)
        lista.ini = novo;
    } else {
        aux = lista.ini;
        while (aux->prox != nullptr) // percorre até o último nodo
            aux = aux->prox;
        aux->prox = novo;
    }

    return 1;
}

int removerPorCod(ListaEnc& lista, int cod) {
    Nodo *ant;
    Nodo *aux;

    ant = nullptr;
    aux = lista.ini;

    while (aux != nullptr && aux->dado.cod != cod) {
        ant = aux;
        aux = aux->prox;
    }

    if (aux == nullptr) // não encontrado
        return 0;

    if (ant == nullptr) // removendo o primeiro nodo
        lista.ini = aux->prox;
    else             // removendo do meio ou do final
        ant->prox = aux->prox;

    delete aux;
    return 1;
}

void destruir(ListaEnc& lista) {
    Nodo *ant;
    Nodo *aux;
    aux = lista.ini;

    while (aux != nullptr) {
        ant = aux;
        aux = aux->prox;
        delete ant;
    }

    lista.ini = nullptr;
}
