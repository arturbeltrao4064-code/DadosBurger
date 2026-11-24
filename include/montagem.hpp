
#ifndef MONTAGEM_HPP
#define MONTAGEM_HPP
#include "pereciveis.hpp"
#include <string>

struct Estoque{
    heap pereciveis[3];
    Ingrediente queijo_suico;
    Ingrediente cheddar;
    Ingrediente cebola;
    Ingrediente tomate;
};

struct NoPilha {
    NoPilha *prox;
    Ingrediente dado;
};

struct PilhaEnc {
    NoPilha *topo;
};

void inicializa(PilhaEnc& p);
bool estaVazia(const PilhaEnc& p);
bool empilha(PilhaEnc& p, const Ingrediente& valor);
bool desempilha(PilhaEnc& p, Ingrediente& valorRemovido);
bool topo(const PilhaEnc& p, Ingrediente& valor);
int tamanho(const PilhaEnc& p);

void printaEstoque(Estoque& e);
#endif
