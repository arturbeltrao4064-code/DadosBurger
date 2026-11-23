#ifndef HISTORICO_HPP
#define HISTORICO_HPP
#include <string>

struct PARTIDA{
    std::string nome_jogador;
    int pontuacao;
};

struct ListaContDin {
    int tamanho;
    int capacidade;
    PARTIDA* historico;
};

void inicializar(ListaContDin& l, int capacidadeIni = 10);
int conta_tamanho(const ListaContDin& l);
PARTIDA acessar(const ListaContDin& l, int pos);
bool inserir(ListaContDin& l, PARTIDA novo, int pos);
bool remover(ListaContDin& l, int pos);
int buscar(const ListaContDin& l, const std::string nome);
void destruir(ListaContDin& l);

#endif