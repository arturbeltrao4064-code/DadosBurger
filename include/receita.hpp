#ifndef RECEITA_HPP
#define RECEITA_HPP

#include <string>

struct IngredienteReceita {
    int cod;
    std::string nome;
    float preco;
};

struct NoReceita {
    NoReceita *prox;
    IngredienteReceita dado;
};

struct ListaReceita {
    NoReceita *ini;
};

void inicializar(ListaReceita& lista);
void imprimir(const ListaReceita& lista);
IngredienteReceita buscar(const ListaReceita& lista, int cod);
int inserirInicio(ListaReceita& lista, IngredienteReceita ingrediente);
int inserirFim(ListaReceita& lista, IngredienteReceita ingrediente);
int removerPorCod(ListaReceita& lista, int cod);
void destruir(ListaReceita& lista);
int tamanho(const ListaReceita& lista);

#endif
