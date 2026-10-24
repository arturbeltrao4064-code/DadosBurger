
#ifndef AVL_HPP
#define AVL_HPP

#include <string>
#include "produto.hpp"

struct NoAVL {
    PRODUTO prod;

    NoAVL *esq;
    NoAVL *dir;

    int altura;

    NoAVL(PRODUTO p);
};

// Altura e fator de balanceamento
int AlturaNodo(NoAVL *no);
void AtualizaAlturaNodo(NoAVL *no);
int FatorBalanceamento(NoAVL *no);

// Rotações
NoAVL* R(NoAVL *no);
NoAVL* L(NoAVL *no);
NoAVL* LR(NoAVL *no);
NoAVL* RL(NoAVL *no);

// Balanceamento, inserção e busca
NoAVL* Balancear(NoAVL *no);
NoAVL* Inserir(NoAVL *no, PRODUTO p);
NoAVL* Buscar(NoAVL *no, int codigo);

#endif
