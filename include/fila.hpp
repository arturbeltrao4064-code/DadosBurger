#ifndef FILA_HPP
#define FILA_HPP

#include "avl.hpp"

struct NoFila {
    NoFila *prox;
    Pedido dado;
};

struct FilaEnc {
    NoFila *frente;
    NoFila *final;
};

void inicializa(FilaEnc& f);
bool estaVazia(const FilaEnc& f);
bool enfileira(FilaEnc& f, const Pedido& valor);
bool desenfileira(FilaEnc& f, Pedido& valorRemovido);
int tamanho(const FilaEnc& f);

#endif
