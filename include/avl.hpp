
#ifndef AVL_HPP
#define AVL_HPP

#include <string>


struct Pedido {
    int cod;
    std::string nome;
};

struct NoAVL {
    Pedido prod;

    NoAVL *esq;
    NoAVL *dir;

    int altura;

    NoAVL(Pedido p);
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
NoAVL* Inserir(NoAVL *no, Pedido p);
const NoAVL* Buscar(const NoAVL *no, int codigo);

extern int totalrotacoes;

#endif
