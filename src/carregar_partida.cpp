#include "partida.hpp"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

namespace
{
bool quantidadeValida(int quantidade)
{
    return quantidade >= 0 && quantidade <= 1000000;
}

void limparFila(FilaEnc &fila)
{
    Pedido removido;
    while (desenfileira(fila, removido)) {}
}

void limparPilha(PilhaEnc &pilha)
{
    Ingrediente removido;
    while (desempilha(pilha, removido)) {}
}

void limparCache(TabelaEnc &tabela)
{
    if (tabela.tabela != nullptr)
    {
        for (int i = 0; i < tabela.m; ++i)
        {
            No *atual = tabela.tabela[i];
            while (atual != nullptr)
            {
                No *proximo = atual->prox;
                delete atual;
                atual = proximo;
            }
        }
        delete[] tabela.tabela;
    }

    tabela.tabela = nullptr;
    tabela.m = 0;
    tabela.n = 0;
}

void limparPartida(Partida &partida)
{
    limparFila(partida.filaClientes);
    limparPilha(partida.hamburguer);

    for (int i = 0; i < 3; ++i)
        destruirHeap(partida.e.pereciveis[i]);

    limparCache(partida.cache);
}

bool lerIngrediente(std::istream &arquivo, Ingrediente &ingrediente)
{
    return static_cast<bool>(
        arquivo >> ingrediente.comida
                >> ingrediente.turno_validade
                >> ingrediente.qnt);
}
}

bool carregarPartida(Partida &p, const std::string &caminho)
{
    std::ifstream arquivo(caminho.c_str());
    if (!arquivo.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo da partida.\n";
        return false;
    }

    Partida temp = {};
    inicializa(temp.filaClientes);
    inicializa(temp.hamburguer);

    auto falha = [&]() {
        limparPartida(temp);
        return false;
    };

    std::string marcador;
    int versao = 0;

    if (!(arquivo >> marcador >> versao) ||
        marcador != "DADOSBURGER_SAVE" || versao != 3)
    {
        std::cerr << "Arquivo invalido ou versao de salvamento desconhecida.\n";
        return falha();
    }

    if (!(arquivo >> marcador) || marcador != "DADOS" ||
        !(arquivo >> temp.id >> temp.turnoAtual >> temp.pontuacao >> temp.pacienciaAtual))
    {
        std::cerr << "Erro ao ler os dados principais da partida.\n";
        return falha();
    }

    if (temp.pacienciaAtual < 0 || temp.pacienciaAtual > 15)
    {
        std::cerr << "Paciencia do cliente invalida.\n";
        return falha();
    }

    // Fila de pedidos.
    int quantidadeClientes = 0;
    if (!(arquivo >> marcador) || marcador != "FILA" ||
        !(arquivo >> quantidadeClientes) ||
        !quantidadeValida(quantidadeClientes))
    {
        std::cerr << "Erro ao ler a fila de clientes.\n";
        return falha();
    }

    for (int i = 0; i < quantidadeClientes; ++i)
    {
        Pedido pedido;
        if (!(arquivo >> pedido.cod >> std::quoted(pedido.nome)) ||
            pedido.nome.empty() || !enfileira(temp.filaClientes, pedido))
        {
            std::cerr << "Erro ao reconstruir um pedido da fila.\n";
            return falha();
        }
    }

    // Pilha: arquivo em ordem topo-base; a pilha auxiliar restaura a ordem.
    int quantidadeIngredientes = 0;
    if (!(arquivo >> marcador) || marcador != "PILHA" ||
        !(arquivo >> quantidadeIngredientes) ||
        !quantidadeValida(quantidadeIngredientes))
    {
        std::cerr << "Erro ao ler a pilha de ingredientes.\n";
        return falha();
    }

    PilhaEnc auxiliar;
    inicializa(auxiliar);

    for (int i = 0; i < quantidadeIngredientes; ++i)
    {
        Ingrediente ingrediente;
        if (!lerIngrediente(arquivo, ingrediente) ||
            !empilha(auxiliar, ingrediente))
        {
            limparPilha(auxiliar);
            std::cerr << "Erro ao ler ingrediente da pilha.\n";
            return falha();
        }
    }

    Ingrediente ingrediente;
    while (desempilha(auxiliar, ingrediente))
    {
        if (!empilha(temp.hamburguer, ingrediente))
        {
            limparPilha(auxiliar);
            std::cerr << "Erro ao reconstruir a pilha.\n";
            return falha();
        }
    }

    // Quatro ingredientes fixos do estoque.
    if (!(arquivo >> marcador) || marcador != "ESTOQUE" ||
        !lerIngrediente(arquivo, temp.e.queijo_suico) ||
        !lerIngrediente(arquivo, temp.e.cheddar) ||
        !lerIngrediente(arquivo, temp.e.cebola) ||
        !lerIngrediente(arquivo, temp.e.tomate))
    {
        std::cerr << "Erro ao ler os ingredientes fixos do estoque.\n";
        return falha();
    }

    // Os tres heaps de pereciveis.
    int quantidadeHeaps = 0;
    if (!(arquivo >> marcador) || marcador != "HEAPS" ||
        !(arquivo >> quantidadeHeaps) || quantidadeHeaps != 3)
    {
        std::cerr << "Arquivo nao contem os tres heaps esperados.\n";
        return falha();
    }

    for (int indice = 0; indice < 3; ++indice)
    {
        char comida;
        int validadeConfig = 0;
        int capacidade = 0;
        int tamanhoHeap = 0;

        if (!(arquivo >> comida >> validadeConfig >> capacidade >> tamanhoHeap) ||
            capacidade < 0 || capacidade > 1000000 ||
            !quantidadeValida(tamanhoHeap) || tamanhoHeap > capacidade)
        {
            std::cerr << "Cabecalho de heap invalido.\n";
            return falha();
        }

        heap &h = temp.e.pereciveis[indice];

        if (capacidade > 0)
        {
            inicializarHeap(h, comida, capacidade, validadeConfig);
            if (h.vetor == nullptr || h.capacidade < capacidade)
            {
                std::cerr << "Falha ao alocar heap de pereciveis.\n";
                return falha();
            }
        }
        else
        {
            h.comida = comida;
            h.validade_config = validadeConfig;
            h.capacidade = 0;
            h.tamanho = 0;
            h.vetor = nullptr;
        }

        for (int i = 0; i < tamanhoHeap; ++i)
        {
            if (!lerIngrediente(arquivo, h.vetor[i]))
            {
                std::cerr << "Erro ao ler elemento do heap.\n";
                return falha();
            }
        }

        h.tamanho = tamanhoHeap;
        if (h.tamanho > 0)
            buildHeap(h);
    }

    // Cache: valida a quantidade de pares e reconstrui por insercao.
    int capacidadeCache = 0;
    int quantidadeCache = 0;

    if (!(arquivo >> marcador) || marcador != "CACHE" ||
        !(arquivo >> capacidadeCache >> quantidadeCache) ||
        capacidadeCache < 0 || capacidadeCache > 1000000 ||
        !quantidadeValida(quantidadeCache) ||
        (capacidadeCache == 0 && quantidadeCache != 0))
    {
        std::cerr << "Cabecalho de cache invalido.\n";
        return falha();
    }

    if (capacidadeCache > 0)
        encInicializar(temp.cache, capacidadeCache);

    for (int i = 0; i < quantidadeCache; ++i)
    {
        int chave = 0;
        int valor = 0;
        if (!(arquivo >> chave >> valor) || capacidadeCache == 0)
        {
            std::cerr << "Erro ao ler elemento do cache.\n";
            return falha();
        }
        encInserir(temp.cache, chave, valor);
    }

    if (!(arquivo >> marcador) || marcador != "FIM")
    {
        std::cerr << "Arquivo de partida incompleto.\n";
        return falha();
    }

    // A partida antiga so e liberada depois que a nova foi lida por completo.
    limparPartida(p);
    p = temp;

    // Evita que os ponteiros transferidos sejam liberados ao sair desta funcao.
    temp.filaClientes = {};
    temp.hamburguer = {};
    temp.cache = {};
    temp.e.pereciveis[0] = {};
    temp.e.pereciveis[1] = {};
    temp.e.pereciveis[2] = {};

    return true;
}
