#ifndef CACHE_HPP
#define CACHE_HPP
#include <string>

int hashDivisao(int k, int m);
int hashMultiplicativo(int k, int m);
int hashString(const std::string& s, int m);

struct No {
    int chave;
    int valor;
    std::string receita;
    No *prox;
};

struct TabelaEnc {
    No **tabela;
    int m;
    int n;
};

extern int totalHits;
extern int totalMisses;
extern int totalColisoes;

void encInicializar(TabelaEnc& t, int m);
void encInserir(TabelaEnc& t, int chave, int valor);
int encBuscar(const TabelaEnc& t, int chave);
void encInserirReceita(TabelaEnc& t, int chave, const std::string& receita);
std::string encBuscarReceita(const TabelaEnc& t, int chave);
int encRemover(TabelaEnc& t, int chave);
void encDestruir(TabelaEnc& t);

#endif
