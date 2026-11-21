#ifndef CACHE_HPP
#define CACHE_HPP
#include <string>
/* ================================================================
 * Funcoes de hash (aula 19)
 * ================================================================ */

/* Hash por divisao: h(k) = k mod m
 * Recomendacao: m primo, nao proximo de potencia de 2 ou 10. */
int hashDivisao(int k, int m);

/* Hash multiplicativo de Knuth: h(k) = floor(m * {k*A}), A ~ 0,618.
 * O carater irracional de A espalha os indices uniformemente. */
int hashMultiplicativo(int k, int m);

/* Hash polinomial para strings via Regra de Horner, base B = 31.
 * h(0) = 0,  h(i) = h(i-1)*B + s[i]  (mod m),  h(s) = h_L mod m */
int hashString(const std::string& s, int m);


/* ================================================================
 * Encadeamento separado (aula 20)
 * ================================================================
 * Cada slot e a cabeca de uma lista encadeada.
 * Suporta fator de carga lambda > 1.
 * Custo medio de busca: O(1 + lambda).
 */

struct No {
    int chave;
    int valor;
    No *prox;
};

struct TabelaEnc{
    No **tabela;   /* vetor de ponteiros para listas */
    int m;         /* numero de slots */
    int n;         /* numero de elementos */
};

void encInicializar(TabelaEnc& t, int m);
void encInserir(TabelaEnc& t, int chave, int valor);
int  encBuscar(const TabelaEnc& t, int chave);
int  encRemover(TabelaEnc &t, int chave);
void encDestruir(TabelaEnc& t);

#endif 
