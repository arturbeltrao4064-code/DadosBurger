#include "avl.hpp"
#include <iostream>
#include <string>
#include <algorithm>

int totalrotacoes = 0;

NoAVL::NoAVL(Produto p) : 
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

    totalrotacoes++;

    return b;
}

NoAVL *L(NoAVL *no){
    NoAVL *b = no->dir;
    NoAVL *subarvore = b->esq;

    b->esq = no;
    no->dir = subarvore;

    AtualizaAlturaNodo(no);
    AtualizaAlturaNodo(b);

    totalrotacoes++;

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

NoAVL* Inserir(NoAVL *no, Produto p) {
    if (no == nullptr) {
        return new NoAVL(p);
    }

    if (p.cod < no->prod.cod) {
        no->esq = Inserir(no->esq, p);
    }
    else if (p.cod > no->prod.cod) {
        no->dir = Inserir(no->dir, p);
    }
    else {
        return no; // Código já existente
    }

    return Balancear(no);
}


NoAVL *Buscar(NoAVL *no, int codigo){
    if(no == nullptr || no->prod.cod == codigo){
        return no;
    }

    if(no->prod.cod > codigo){
        return Buscar(no->esq, codigo);
    }

    
    return Buscar(no->dir, codigo);
    
}