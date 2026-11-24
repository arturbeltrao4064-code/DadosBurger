
#ifndef PERECIVEIS_HPP
#define PERECIVEIS_HPP

struct Ingrediente {
    char comida;
    int turno_validade;
    int qnt;
};

struct heap {
    char comida;
    Ingrediente *vetor;
    int tamanho;
    int capacidade;
    int validade_config;
};

void inicializarHeap(heap& a, char comida, int capacidade, int validadeConfig);
bool estaVazio(const heap& a);
bool inserir(heap& a, const Ingrediente& novo, int turnoAtual);
Ingrediente remover_min(heap& a);
void heapify(heap& a, int i);
void siftUp(heap& a, int i);
void buildHeap(heap& a);
void destruirHeap(heap& a);

#endif
