
#ifndef PERECIVEIS_HPP
#define PERECIVEIS_HPP

struct ingrediente {
    char comida;
    int turno_validade;
};

struct heap {
    char comida;
    ingrediente *vetor;
    int tamanho;
    int capacidade;
    int validade_config;
};

void inicializarHeap(heap& a, char comida, int capacidade, int validadeConfig);
bool estaVazio(const heap& a);
bool inserir(heap& a, const ingrediente& novo, int turnoAtual);
ingrediente remover_min(heap& a);
void heapify(heap& a, int i);
void siftUp(heap& a, int i);
void buildHeap(heap& a);
void destruirHeap(heap& a);

#endif
