#ifndef MONTAGEM_HPP
#define MONTAGEM_HPP
#include <iostream>

struct Produto{
    int cod;
    std::string nome;
    float preco;
};

struct Nodo {
    Nodo *prox;
    Produto dado;
};

struct PilhaEnc{
    Nodo *topo;
};

void inicializa(PilhaEnc& p);
bool estaVazia(PilhaEnc& p);
bool empilha(PilhaEnc& p, Produto valor);
bool desempilha(PilhaEnc& p, Produto& valorRemovido);
bool topo(PilhaEnc& p, Produto& valor);
int  tamanho(PilhaEnc& p);

#endif