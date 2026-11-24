#include "avl.hpp"

int totalrotacoes = 0;
int totalNosInseridos = 0;
int totalComparacoesBusca = 0;
int totalBuscas = 0;

NoAVL::NoAVL(Pedido p) {
    prod = p;
    esq = nullptr;
    dir = nullptr;
    altura = 0;
}

int AlturaNodo(NoAVL *no) {
    if (no == nullptr) return -1;
    return no->altura;
}

void AtualizaAlturaNodo(NoAVL *no) {
    int esquerda = AlturaNodo(no->esq);
    int direita = AlturaNodo(no->dir);
    if (esquerda > direita) no->altura = esquerda + 1;
    else no->altura = direita + 1;
}

NoAVL *R(NoAVL *no) {
    NoAVL *b = no->esq;
    NoAVL *subarvore = b->dir;
    b->dir = no;
    no->esq = subarvore;
    AtualizaAlturaNodo(no);
    AtualizaAlturaNodo(b);
    totalrotacoes++;
    return b;
}

NoAVL *L(NoAVL *no) {
    NoAVL *b = no->dir;
    NoAVL *subarvore = b->esq;
    b->esq = no;
    no->dir = subarvore;
    AtualizaAlturaNodo(no);
    AtualizaAlturaNodo(b);
    totalrotacoes++;
    return b;
}

NoAVL *LR(NoAVL *no) {
    no->esq = L(no->esq);
    return R(no);
}

NoAVL *RL(NoAVL *no) {
    no->dir = R(no->dir);
    return L(no);
}

int FatorBalanceamento(NoAVL *no) {
    if (no == nullptr) return 0;
    return AlturaNodo(no->esq) - AlturaNodo(no->dir);
}

NoAVL *Balancear(NoAVL *no) {
    if (no == nullptr) return nullptr;
    AtualizaAlturaNodo(no);

    if (FatorBalanceamento(no) > 1) {
        if (FatorBalanceamento(no->esq) < 0) return LR(no);
        return R(no);
    }
    if (FatorBalanceamento(no) < -1) {
        if (FatorBalanceamento(no->dir) > 0) return RL(no);
        return L(no);
    }
    return no;
}

NoAVL* Inserir(NoAVL *no, Pedido p) {
    if (no == nullptr) {
        totalNosInseridos++;
        return new NoAVL(p);
    }
    if (p.cod < no->prod.cod) no->esq = Inserir(no->esq, p);
    else if (p.cod > no->prod.cod) no->dir = Inserir(no->dir, p);
    else return no;
    return Balancear(no);
}

const NoAVL* Buscar(const NoAVL *no, int codigo) {
    totalBuscas++;
    while (no != nullptr) {
        totalComparacoesBusca++;
        if (codigo == no->prod.cod) return no;
        if (codigo < no->prod.cod) no = no->esq;
        else no = no->dir;
    }
    return nullptr;
}

void DestruirAVL(NoAVL *no) {
    if (no == nullptr) return;
    DestruirAVL(no->esq);
    DestruirAVL(no->dir);
    delete no;
}
