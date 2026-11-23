
#ifndef FILA_HPP
#define FILA_HPP

#include <string>

struct ProdutoFila {
    int cod;
    std::string nome;
    float preco;
    int paciencia;
};

struct NoFila {
    NoFila *prox;
    ProdutoFila dado;
};

struct FilaEnc {
    NoFila *frente;
    NoFila *final;
};

void inicializa(FilaEnc& f);
bool estaVazia(const FilaEnc& f);
bool enfileira(FilaEnc& f, const ProdutoFila& valor);
bool desenfileira(FilaEnc& f, ProdutoFila& valorRemovido);
int tamanho(const FilaEnc& f);

#endif
