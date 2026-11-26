#ifndef PARTIDA_HPP
#define PARTIDA_HPP

#define CLIENTES 10
#include "fila.hpp"
#include "montagem.hpp"
#include "cache.hpp"
#include "avl.hpp"

struct Partida {
    int id;
    int turnoAtual;
    int pontuacao;
    int pacienciaAtual;
    FilaEnc filaClientes;
    PilhaEnc hamburguer;
    Estoque e;
    TabelaEnc cache;
};

extern double tempoTotalBuscas;

void inicializarPartida(Partida& p, const NoAVL* pedidos, int maiorId);
bool executarPartida(Partida& p);
int compararPartidas(const PARTIDA &a, const PARTIDA &b, int criterio);

#endif
