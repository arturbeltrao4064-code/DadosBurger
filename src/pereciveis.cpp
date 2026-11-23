#include "pereciveis.hpp"

void heapify(heap& a, int ativos, int i){
    int menor = i;
    int esq = 2*i + 1;
    int dir = 2*i + 1;

    if(esq < ativos && a.vetor[esq].turno_validade < a.vetor[menor].turno_validade) menor = esq;
    if(dir < ativos && a.vetor[dir].turno_validade < a.vetor[menor].turno_validade) menor = dir;

    if(menor != i){
        ingrediente tmp = a.vetor[i];
        a.vetor[i] = a.vetor[menor];
        a.vetor[menor] = tmp;
        heapify(a, ativos, menor);
    }
};

void siftUp(heap& a, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (a.vetor[i].turno_validade < a.vetor[pai].turno_validade) {
            ingrediente tmp = a.vetor[i];
            a.vetor[i].turno_validade = a.vetor[pai].turno_validade;
            a.vetor[pai] = tmp;
            i = pai;
        } else {
            break;
        }
    }
};

void inserir(heap& a, int ativos, ingrediente& novo){
    a.vetor[ativos] = novo;
    a.vetor[ativos].turno_validade = a.turno_preparo + a.validade_config;
    siftUp(a, ativos);
};

ingrediente remover_min(heap& a, int ativos){
    ingrediente min = a.vetor[0];
    a.vetor[0] = a.vetor[ativos];
    ativos--;
    heapify(a, ativos, 0);
};

void buildHeap(heap& a, int ativos){
    for(int i = ativos/2 - 1; i >= 0; i--){
        heapify(a, ativos, i);
    }
};