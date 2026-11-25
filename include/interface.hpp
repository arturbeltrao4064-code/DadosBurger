#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "partida.hpp"
#include <string>

// Menu principal: 1 novo jogo, 2 historico, 0 sair.
int iniciarInterface();

void interfaceAbrirJogo();
void interfaceFecharJogo();
void interfaceAtualizar(const Partida& partida);
char interfaceLerAcao(const Partida& partida);
char interfaceLerIngrediente(const Partida& partida, bool preparar);
void interfaceMensagem(const char *mensagem);
void interfaceMostrarEstoque(const Estoque& estoque);
void interfaceMostrarPedido(const Pedido& pedido);
std::string interfaceLerNome();
void interfaceMostrarResultado(const std::string& nome, int pontuacao);

#endif
