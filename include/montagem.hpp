
#ifndef MONTAGEM_HPP
#define MONTAGEM_HPP

#include <string>

struct IngredientePilha {
    int cod;
    std::string nome;
    float preco;
};

struct NoPilha {
    NoPilha *prox;
    IngredientePilha dado;
};

struct PilhaEnc {
    NoPilha *topo;
};

void inicializa(PilhaEnc& p);
bool estaVazia(const PilhaEnc& p);
bool empilha(PilhaEnc& p, const IngredientePilha& valor);
bool desempilha(PilhaEnc& p, IngredientePilha& valorRemovido);
bool topo(PilhaEnc& p, IngredientePilha& valor);
int tamanho(const PilhaEnc& p);

#endif
