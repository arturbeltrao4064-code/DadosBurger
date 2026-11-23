#ifndef PARTIDA_HPP
#define PARTIDA_HPP

#include <fstream>
#include "fila.hpp"
#include "montagem.hpp"
#include "pereciveis.hpp"
#include "cache.hpp"
#include "avl.hpp"
#include <cstdlib>

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