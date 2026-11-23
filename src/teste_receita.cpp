
#include <iostream>
#include "receita.hpp"

int main() {
    ListaReceita receita;
    inicializar(receita);

    IngredienteReceita pao = {1, "Pao", 2.0f};
    IngredienteReceita queijo = {2, "Queijo", 3.0f};
    IngredienteReceita carne = {3, "Carne", 5.0f};

    inserirFim(receita, pao);
    inserirFim(receita, queijo);
    inserirInicio(receita, carne);

    std::cout << "Ingredientes da receita:\n";
    imprimir(receita);

    std::cout << "\nTamanho: " << tamanho(receita) << '\n';

    IngredienteReceita encontrado = buscar(receita, 2);
    std::cout << "\nBusca pelo codigo 2: "
              << encontrado.nome << '\n';

    removerPorCod(receita, 2);

    std::cout << "\nDepois da remocao:\n";
    imprimir(receita);

    destruir(receita);

    std::cout << "\nTamanho final: " << tamanho(receita) << '\n';

    return 0;
}
