#include <iostream>
#include <string>
#include <math.h>
#include "cache.hpp"

/* ================================================================
 * Funcoes de hash
 * ================================================================ */

int hashDivisao(int k, int m) {
    return k % m;
}

int hashMultiplicativo(int k, int m) {
    const double A = 0.6180339887;   /* (sqrt(5)-1)/2 - Knuth */
    double prod = (double)k * A;
    double frac = prod - floor(prod);
    return (int)(m * frac);
}

int hashString(const std::string& s, int m) {
    const int B = 31;
    int h = 0;
    for (unsigned char c : s)
        h = (h * B + c) % m;
    return h;
}

/* ================================================================
 * Encadeamento separado
 * ================================================================ */

void encInicializar(TabelaEnc& t, int m) {
    t.m      = m;
    t.n      = 0;
    t.tabela = new No* [m];
    for (int i = 0; i < m; i++)
        t.tabela[i] = nullptr;
}

void encInserir(TabelaEnc& t, int chave, int valor) {
    int i    = hashDivisao(chave, t.m);
    No *novo = new No;
    novo->chave  = chave;
    novo->valor  = valor;
    novo->prox   = t.tabela[i];
    t.tabela[i] = novo;
    t.n++;
}

int encBuscar(const TabelaEnc& t, int chave) {
    int i = hashDivisao(chave, t.m);
    No *p = t.tabela[i];
    while (p != nullptr) {
        if (p->chave == chave) return p->valor;
        p = p->prox;
    }
    return -1;
}

int encRemover(TabelaEnc& t, int chave) {
    int i   = hashDivisao(chave, t.m);
    No *p   = t.tabela[i];
    No *ant = nullptr;
    while (p != nullptr) {
        if (p->chave == chave) {
            if (ant == nullptr)
                t.tabela[i] = p->prox;
            else
                ant->prox = p->prox;
            delete p;
            t.n--;
            return 1;
        }
        ant = p;
        p   = p->prox;
    }
    return 0;
}

void encDestruir(TabelaEnc& t) {
    for (int i = 0; i < t.m; i++) {
        No *p = t.tabela[i];
        while (p) {
            No *prox = p->prox;
            delete p;
            p = prox;
        }
    }
    delete t.tabela;
    t.tabela = nullptr;
    t.m = t.n = 0;
}