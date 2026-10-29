#ifndef RECEITA_HPP
#define RECEITA_HPP
#include <iostream>

struct Produto{
    int cod;
    char nome[50];
    float preco;
};

struct Nodo {
    Nodo *prox;
    Produto dado;
};

struct ListaEnc{
    Nodo *ini;
};

void    inicializar(ListaEnc& lista);
void    imprimir(const ListaEnc& lista);
Produto buscar(const ListaEnc& lista, int cod);
int     inserirInicio(ListaEnc& lista, Produto prod);
int     inserirFim(ListaEnc& lista, Produto prod);
int     removerPorCod(ListaEnc& lista, int cod);
void    destruir(ListaEnc& lista);
int     tamanho(const ListaEnc& lista);

#endif