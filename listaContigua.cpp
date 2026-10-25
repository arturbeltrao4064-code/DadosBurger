#include "listaContigua.hpp"
#include <iostream>

void inicializar(ListaContDin& l, int capacidadeIni){
    l.tamanho = 0;
    l.capacidade = capacidadeIni;
    l.dados = new PRODUTO[l.capacidade];
}

int conta_tamanho(const ListaContDin& l){
    return l.tamanho;
}

PRODUTO acessar(const ListaContDin& l, int pos){
    PRODUTO pAux;
    if(pos >= l.tamanho || pos < 0)
        pAux.cod = -1;
    else
        pAux = l.dados[pos];
    
    return pAux;
}

bool inserir(ListaContDin& l, PRODUTO novo, int pos){
    if(pos > l.tamanho || pos < 0) return false;

    if(l.tamanho == l.capacidade){
        int novaCapacidade = l.capacidade * 2;
        PRODUTO* novoVetor = new PRODUTO[novaCapacidade];

        for(int i = 0; i < l.tamanho; i++){
            novoVetor[i] = l.dados[i];
        }

        delete[] l.dados;
        l.dados = novoVetor;
        l.capacidade = novaCapacidade;
    }

    for(int i = l.tamanho; i > pos; i--){
        l.dados[i] = l.dados[i-1];
    }

    l.dados[pos] = novo;
    l.tamanho++;

    return true;
}

bool remover(ListaContDin& l, int pos){
    if(pos >= l.tamanho || pos < 0) return false;

    for(int i = pos; i < l.tamanho - 1; i++){
        l.dados[i] = l.dados[i + 1];
    }

    l.tamanho--;

    return true;
}

int buscar(const ListaContDin& l, int cod){
    for(int i = 0; i < l.tamanho; i++){
        if(l.dados[i].cod == cod){
            return i;
        }
    }
    return -1;
}

void destruir(ListaContDin& l){
    delete[] l.dados;
    l.dados = nullptr;
    l.tamanho = 0;
    l.capacidade = 0; 
}