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
    FilaEnc filaClientes;
    PilhaEnc hamburguer;
    Estoque e;
    TabelaEnc cache;
};

void inicializarPartida(Partida& p, const NoAVL* pedidos, int maiorId);
void executarPartida(Partida& p);

#endif