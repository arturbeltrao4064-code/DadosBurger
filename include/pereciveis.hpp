#ifndef PERECIVEIS_HPP
#define PERECIVEIS_HPP

struct ingrediente{
    int turno_validade;
};

struct heap{
    char comida;
    ingrediente *vetor;
    int turno_preparo;
    int validade_config;
};

void heapify(heap& a, int ativos, int i);
void siftUp(heap& a, int i);
void inserir(heap& a, int ativos, ingrediente& novo);
ingrediente remover_min(heap& a);
void buildHeap(heap& a, int ativos);
#endif