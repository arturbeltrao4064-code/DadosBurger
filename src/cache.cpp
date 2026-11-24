#include "cache.hpp"
#include <math.h>

int totalHits = 0;
int totalMisses = 0;
int totalColisoes = 0;
int totalEntradasCache = 0;

int hashDivisao(int k, int m) {
    int resultado = k % m;
    if (resultado < 0) resultado += m;
    return resultado;
}

int hashMultiplicativo(int k, int m) {
    const double A = 0.6180339887;
    double prod = (double)k * A;
    double frac = prod - floor(prod);
    return (int)(m * frac);
}

int hashString(const std::string& s, int m) {
    const int B = 31;
    int h = 0;
    for (int i = 0; i < (int)s.size(); i++)
        h = (h * B + (unsigned char)s[i]) % m;
    return h;
}

void encInicializar(TabelaEnc& t, int m) {
    if (m < 1) m = 1;
    t.m = m;
    t.n = 0;
    t.tabela = new No*[m];
    for (int i = 0; i < m; i++) t.tabela[i] = nullptr;
}

void encInserir(TabelaEnc& t, int chave, int valor) {
    int i = hashDivisao(chave, t.m);
    if (t.tabela[i] != nullptr) totalColisoes++;
    No *novo = new No;
    novo->chave = chave;
    novo->valor = valor;
    novo->receita = "";
    novo->prox = t.tabela[i];
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

void encInserirReceita(TabelaEnc& t, int chave, const std::string& receita) {
    int i = hashDivisao(chave, t.m);
    No *p = t.tabela[i];
    while (p != nullptr) {
        if (p->chave == chave) {
            p->receita = receita;
            return;
        }
        p = p->prox;
    }
    if (t.tabela[i] != nullptr) totalColisoes++;
    No *novo = new No;
    novo->chave = chave;
    novo->valor = chave;
    novo->receita = receita;
    novo->prox = t.tabela[i];
    t.tabela[i] = novo;
    t.n++;
    totalEntradasCache++;
}

std::string encBuscarReceita(const TabelaEnc& t, int chave) {
    int i = hashDivisao(chave, t.m);
    No *p = t.tabela[i];
    while (p != nullptr) {
        if (p->chave == chave) return p->receita;
        p = p->prox;
    }
    return "";
}

int encRemover(TabelaEnc& t, int chave) {
    int i = hashDivisao(chave, t.m);
    No *p = t.tabela[i];
    No *ant = nullptr;
    while (p != nullptr) {
        if (p->chave == chave) {
            if (ant == nullptr) t.tabela[i] = p->prox;
            else ant->prox = p->prox;
            delete p;
            t.n--;
            return 1;
        }
        ant = p;
        p = p->prox;
    }
    return 0;
}

void encDestruir(TabelaEnc& t) {
    if (t.tabela == nullptr) return;
    for (int i = 0; i < t.m; i++) {
        No *p = t.tabela[i];
        while (p != nullptr) {
            No *prox = p->prox;
            delete p;
            p = prox;
        }
    }
    delete[] t.tabela;
    t.tabela = nullptr;
    t.m = 0;
    t.n = 0;
}
