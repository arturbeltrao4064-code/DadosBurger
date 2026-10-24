#include "avl.hpp"
#include <iostream>
#include <string>
#include <algorithm>

NoAVL::NoAVL(PRODUTO p) : 
    prod(p), 
    esq(nullptr),
    dir(nullptr),
    altura(0) {}

int AlturaNodo(NoAVL *no){
    return (no == nullptr) ? -1 : no->altura;
}   

void AtualizaAlturaNodo(NoAVL *no){
    no->altura = 1 + std::max(
        AlturaNodo(no->esq),
        AlturaNodo(no->dir)
    );
}

NoAVL *R(NoAVL *no){
    NoAVL *b = no->esq;
    NoAVL *subarvore = b->dir;

    b->dir = no;
    no->esq = subarvore;

    AtualizaAlturaNodo(no);
    AtualizaAlturaNodo(b);

    return b;
}

NoAVL *L(NoAVL *no){
    NoAVL *b = no->dir;
    NoAVL *subarvore = b->esq;

    b->esq = no;
    no->dir = subarvore;

    AtualizaAlturaNodo(no);
    AtualizaAlturaNodo(b);

    return b;
}

NoAVL *LR(NoAVL *no){
    no->esq = L(no->esq);
    return R(no);
}

NoAVL *RL(NoAVL *no){
    no->dir = R(no->dir);
    return L(no);
}

int FatorBalanceamento(NoAVL *no){
    if(no == nullptr){
        return 0;
    }

    return AlturaNodo(no->esq) - AlturaNodo(no->dir);
}

NoAVL *Balancear(NoAVL *no){
    if (no == nullptr)
        return nullptr;

    AtualizaAlturaNodo(no);

    if (FatorBalanceamento(no) > 1) {
        if (FatorBalanceamento(no->esq) < 0)
            return LR(no);
        return R(no);
    }

    if (FatorBalanceamento(no) < -1) {
        if (FatorBalanceamento(no->dir) > 0)
            return RL(no);
        return L(no);
    }

    return no;
}

// falta inserir e buscar