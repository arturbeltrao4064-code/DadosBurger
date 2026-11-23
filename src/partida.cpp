#include "partida.hpp"
#include <iostream>

namespace {
void limparFila(FilaEnc& fila) {
    Pedido pedido;
    while (desenfileira(fila, pedido)) {
    }
}
}

void inicializarPartida(Partida& p, const NoAVL* pedidos, int maiorId) {
    inicializa(p.filaClientes);
    inicializa(p.hamburguer);
    p.e = {};
    p.cache = {};

    p.id = maiorId + 1;
    p.turnoAtual = 0;
    p.pontuacao = 0;

    if (pedidos == nullptr) {
        std::cerr << "Erro: nao ha pedidos para iniciar a partida.\n";
        return;
    }

    for (int i = 0; i < CLIENTES; i++) {
        const int x = std::rand() % 87000;
        const NoAVL* tmp = Buscar(pedidos, x);
        if (tmp == nullptr) {
            std::cerr << "Erro: pedido " << x << " nao encontrado.\n";
            limparFila(p.filaClientes);
            return;
        }
        if (!enfileira(p.filaClientes, tmp->prod)) {
            std::cerr << "Erro ao adicionar pedido na fila de clientes.\n";
            limparFila(p.filaClientes);
            return;
        }
    }
}
