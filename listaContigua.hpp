#ifndef LISTACONTIGUA_HPP
#define LISTACONTIGUA_HPP
#include "produto.hpp"
#include <iostream>

struct ListaContDin {
    int tamanho;
    int capacidade;
    PRODUTO* dados;
};

void inicializar(ListaContDin& l, int capacidadeIni = 10);
int conta_tamanho(const ListaContDin& l);
PRODUTO acessar(const ListaContDin& l, int pos);
bool inserir(ListaContDin& l, PRODUTO novo, int pos);
bool remover(ListaContDin& l, int pos);
int buscar(ListaContDin& l, int cod);
void destruir(ListaContDin& l);

#endif