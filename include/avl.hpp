#ifndef AVL_HPP
#define AVL_HPP

#include <string>
#include "receita.hpp"

struct Pedido {
    int cod;
    std::string nome;
    ListaReceita receita;
};

struct NoAVL {
    Pedido prod;
    NoAVL *esq;
    NoAVL *dir;
    int altura;
    NoAVL(Pedido p);
};

int AlturaNodo(NoAVL *no);
void AtualizaAlturaNodo(NoAVL *no);
int FatorBalanceamento(NoAVL *no);
NoAVL* R(NoAVL *no);
NoAVL* L(NoAVL *no);
NoAVL* LR(NoAVL *no);
NoAVL* RL(NoAVL *no);
NoAVL* Balancear(NoAVL *no);
NoAVL* Inserir(NoAVL *no, Pedido p);
const NoAVL* Buscar(const NoAVL *no, int codigo);
void DestruirAVL(NoAVL *no);
int menorCodigo(const NoAVL *no);
int maiorCodigo(const NoAVL *no);


extern int totalrotacoes;
extern int totalNosInseridos;
extern int totalComparacoesBusca;
extern int totalBuscas;

#endif
