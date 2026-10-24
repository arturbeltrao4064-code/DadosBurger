
#ifndef AVL_HPP
#define AVL_HPP

#include <string>

struct NoAVL {
    int codigo;
    std::string nome;

    NoAVL *esq;
    NoAVL *dir;

    int altura;

    NoAVL(int cod, const std::string& str);
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
NoAVL* Inserir(NoAVL *no, int codigo, const std::string& nome);
NoAVL* Buscar(NoAVL *no, int codigo);

#endif
