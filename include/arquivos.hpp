#ifndef ARQUIVOS_HPP
#define ARQUIVOS_HPP

#include <string>
#include "avl.hpp"
#include "historico.hpp"
#include <fstream>

bool carregarHistorico(std::ifstream& arquivo, ListaContDin& historico);


NoAVL* carregarPedidos(const std::string& caminho, NoAVL* raiz);

#endif