#include "partida.hpp"
#include <iostream>
#include "montagem.hpp"

void inicializarPartida(Partida& p, const NoAVL* pedidos, int maiorId){
    p.id = maiorId + 1;
    p.turnoAtual = 0;
    p.pontuacao = 0;
    NoAVL* tmp;
    for(int i = 0; i < CLIENTES; i++){
        int x = std::rand() % 87000;
        tmp = Buscar(pedidos, x);
        enfileira(p.filaClientes, tmp->prod);
    }
    inicializa(p.hamburguer);
    
}