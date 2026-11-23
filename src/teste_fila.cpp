#include <iostream>
#include "fila.hpp"

int main() {
    FilaEnc fila;
    Pedido p;

    inicializa(fila);

    Pedido p1 = {1, "Pao"};
    Pedido p2 = {2, "Queijo"};

    enfileira(fila, p1);
    enfileira(fila, p2);

    std::cout << "Tamanho: " << tamanho(fila) << '\n';

    while (desenfileira(fila, p)) {
        std::cout << p.cod << " - " << p.nome << '\n';
    }

    std::cout << "Fila vazia? " << estaVazia(fila) << '\n';

    return 0;
}