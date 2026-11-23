#include <iostream>
#include "fila.hpp"

int main() {
    FilaEnc fila;
    ProdutoFila p;

    inicializa(fila);

    ProdutoFila p1 = {1, "Pao", 2.0};
    ProdutoFila p2 = {2, "Queijo", 3.0};

    enfileira(fila, p1);
    enfileira(fila, p2);

    std::cout << "Tamanho: " << tamanho(fila) << '\n';

    while (desenfileira(fila, p)) {
        std::cout << p.cod << " - " << p.nome << '\n';
    }

    std::cout << "Fila vazia? " << estaVazia(fila) << '\n';

    return 0;
}