
#include <new>
#include <iostream>
#include "montagem.hpp"

void inicializa(PilhaEnc& p) {
    p.topo = nullptr;
}

bool estaVazia(const PilhaEnc& p) {
    return p.topo == nullptr;
}

bool empilha(PilhaEnc& p, const Ingrediente& valor) {
    NoPilha *novo = new NoPilha;

    if (novo == nullptr)
        return false;

    novo->dado = valor;
    novo->prox = p.topo;

    p.topo = novo;

    return true;
}

bool desempilha(PilhaEnc& p, Ingrediente& valorRemovido) {
    if (estaVazia(p))
        return false;

    NoPilha *removido = p.topo;

    valorRemovido = removido->dado;
    p.topo = removido->prox;

    delete removido;

    return true;
}

bool topo(const PilhaEnc& p, Ingrediente& valor) {
    if (estaVazia(p))
        return false;

    valor = p.topo->dado;

    return true;
}

int tamanho(const PilhaEnc& p) {
    const NoPilha *aux = p.topo;
    int contador = 0;

    while (aux != nullptr) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}

void printaEstoque(Estoque& e){
    std::cout << "Estoque atual:\n";
    std::cout << "Ketchup: infinito\\n";
    std::cout << "Mostarda: infinito\\n";
    std::cout << "Picles: infinito\\n";
    std::cout << "Alface: infinito\\n";
    std::cout << "Pao de cima: infinito\\n";
    std::cout << "Queijo suico:" << e.queijo_suico.qnt << '\n';
    std::cout << "Queijo cheddar:" << e.cheddar.qnt << '\n';
    std::cout << "Cebola:" << e.cebola.qnt << '\n';
    std::cout << "Tomate:" << e.tomate.qnt << '\n';
    std::cout << "Carne:" << e.pereciveis[0].tamanho << '\n';
    std::cout << "Ovo frito:" << e.pereciveis[1].tamanho << '\n';
    std::cout << "Bacon:" << e.pereciveis[2].tamanho << '\n';
}