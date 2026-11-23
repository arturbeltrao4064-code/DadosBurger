
#include <iostream>
#include "pereciveis.hpp"

int main()
{
    heap tomates;

    // Validade de 3 turnos e capacidade para 10 ingredientes
    inicializarHeap(tomates, 'T', 10, 3);

    ingrediente t1 = {0};
    ingrediente t2 = {0};
    ingrediente t3 = {0};
    ingrediente t4 = {0};

    // Preparados em turnos diferentes
    inserir(tomates, t1, 2);  // vence no turno 5
    inserir(tomates, t2, 7);  // vence no turno 10
    inserir(tomates, t3, 1);  // vence no turno 4
    inserir(tomates, t4, 4);  // vence no turno 7

    std::cout << "Tamanho do heap: "
              << tomates.tamanho << "\n\n";

    std::cout << "Removendo por ordem de validade:\n";

    while (!estaVazio(tomates))
    {
        ingrediente removido = remover_min(tomates);

        std::cout << "Turno de validade: "
                  << removido.turno_validade << '\n';
    }

    destruirHeap(tomates);

    return 0;
}
