#ifndef RECEITA_HPP
#define RECEITA_HPP

struct NoReceita {
    char ingrediente;
    NoReceita *prox;
};

struct ListaReceita {
    NoReceita *ini;
};

void inicializar(ListaReceita& lista);
bool inserirFim(ListaReceita& lista, char ingrediente);
void destruir(ListaReceita& lista);

#endif
