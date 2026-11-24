
#ifndef RECEITA_HPP
#define RECEITA_HPP
#include "pereciveis.hpp"

#include <string>

struct NoReceita {
    NoReceita *prox;
    Ingrediente dado;
};

struct ListaReceita {
    NoReceita *ini;
};

void inicializar(ListaReceita& lista);
void imprimir(const ListaReceita& lista);
Ingrediente buscar(const ListaReceita& lista, int cod);
int inserirInicio(ListaReceita& lista, Ingrediente ingrediente);
int inserirFim(ListaReceita& lista, Ingrediente ingrediente);
int removerPorCod(ListaReceita& lista, int cod);
void destruir(ListaReceita& lista);
int tamanho(const ListaReceita& lista);

#endif
