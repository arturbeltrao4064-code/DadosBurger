#ifndef FILA_HPP
#define FILA_HPP
#define CLIENTES 10
#include "avl.hpp"

struct Nodo {
    Nodo *prox;
    Pedido dado;
};

struct FilaEnc{
    Nodo *frente;
    Nodo *final;
};

void inicializa(FilaEnc& f);
bool estaVazia(FilaEnc& f);
bool enfileira(FilaEnc& f, Pedido valor);
int  desenfileira(FilaEnc& f, Pedido &valorRemovido);
int  tamanho(FilaEnc& f);

#endif