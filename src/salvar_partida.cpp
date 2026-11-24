#include "partida.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

namespace
{
bool ingredienteValido(const Ingrediente &ingrediente)
{
    return ingrediente.comida != '\0' &&
           ingrediente.turno_validade >= 0 &&
           ingrediente.qnt >= 0;
}

bool heapValido(const heap &h)
{
    return h.capacidade >= 0 &&
           h.tamanho >= 0 &&
           h.tamanho <= h.capacidade &&
           (h.tamanho == 0 || h.vetor != nullptr);
}
}

bool salvarPartida(const Partida &p, const std::string &caminho)
{
    std::ofstream arquivo(caminho.c_str());

    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo para salvar a partida.\n";
        return false;
    }

    // Versao 2: salva Ingrediente completo e os tres heaps de pereciveis.
    arquivo << "DADOSBURGER_SAVE 2\n";

    arquivo << "DADOS\n"
            << p.id << ' ' << p.turnoAtual << ' ' << p.pontuacao << '\n';

    // Fila: Pedido possui codigo e nome.
    arquivo << "FILA\n";
    arquivo << tamanho(p.filaClientes) << '\n';

    for (const NoFila *cliente = p.filaClientes.frente;
         cliente != nullptr; cliente = cliente->prox)
    {
        arquivo << cliente->dado.cod << ' '
                << std::quoted(cliente->dado.nome) << '\n';
    }

    // Pilha: grava do topo para a base, com todos os campos de Ingrediente.
    arquivo << "PILHA\n" << tamanho(p.hamburguer) << '\n';

    for (const NoPilha *atual = p.hamburguer.topo;
         atual != nullptr; atual = atual->prox)
    {
        const Ingrediente &i = atual->dado;
        if (!ingredienteValido(i))
        {
            std::cerr << "Ingrediente invalido na pilha.\n";
            return false;
        }

        arquivo << i.comida << ' ' << i.turno_validade << ' '
                << i.qnt << '\n';
    }

    // Ingredientes fixos do estoque.
    arquivo << "ESTOQUE\n";
    const Ingrediente *fixos[] = {
        &p.e.queijo_suico, &p.e.cheddar, &p.e.cebola, &p.e.tomate
    };

    for (const Ingrediente *i : fixos)
    {
        arquivo << i->comida << ' ' << i->turno_validade << ' '
                << i->qnt << ' ';
    }
    arquivo << '\n';

    // Existem tres heaps: carne, ovo e bacon.
    arquivo << "HEAPS\n3\n";
    for (int indice = 0; indice < 3; ++indice)
    {
        const heap &h = p.e.pereciveis[indice];

        if (!heapValido(h))
        {
            std::cerr << "Estado invalido no heap de pereciveis.\n";
            return false;
        }

        arquivo << h.comida << ' ' << h.validade_config << ' '
                << h.capacidade << ' ' << h.tamanho << '\n';

        for (int i = 0; i < h.tamanho; ++i)
        {
            const Ingrediente &ingrediente = h.vetor[i];
            if (!ingredienteValido(ingrediente))
            {
                std::cerr << "Ingrediente invalido em um heap.\n";
                return false;
            }

            arquivo << ingrediente.comida << ' '
                    << ingrediente.turno_validade << ' '
                    << ingrediente.qnt << '\n';
        }
    }

    // Cache: grava cada par chave/valor uma unica vez.
    const TabelaEnc &tabela = p.cache;
    if (tabela.m < 0 || tabela.n < 0 ||
        (tabela.m > 0 && tabela.tabela == nullptr) ||
        (tabela.m == 0 && tabela.n != 0))
    {
        std::cerr << "Estado invalido do cache.\n";
        return false;
    }

    arquivo << "CACHE\n" << tabela.m << ' ' << tabela.n << '\n';
    int elementosGravados = 0;

    if (tabela.tabela != nullptr)
    {
        for (int i = 0; i < tabela.m; ++i)
        {
            for (const No *atual = tabela.tabela[i];
                 atual != nullptr; atual = atual->prox)
            {
                arquivo << atual->chave << ' ' << atual->valor << '\n';
                ++elementosGravados;
            }
        }
    }

    if (elementosGravados != tabela.n)
    {
        std::cerr << "Quantidade de elementos do cache inconsistente.\n";
        return false;
    }

    arquivo << "FIM\n";
    arquivo.close();

    if (!arquivo)
    {
        std::cerr << "Erro durante a escrita do arquivo.\n";
        return false;
    }

    return true;
}
