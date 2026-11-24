

#include "partida.hpp"

#include <fstream>
#include <iostream>
#include <string>

// Libera os elementos da fila temporaria.
void limparFilaCarregamento(FilaEnc &fila)
{
    Pedido pedido;

    while (desenfileira(fila, pedido))
    {
    }
}

// Libera os elementos da pilha temporaria.
void limparPilhaCarregamento(PilhaEnc &pilha)
{
    IngredientePilha ingrediente;

    while (desempilha(pilha, ingrediente))
    {
    }
}

// Libera a tabela de hash temporaria.
void limparCacheCarregamento(TabelaEnc &tabela)
{
    if (tabela.tabela == nullptr)
    {
        tabela.m = 0;
        tabela.n = 0;
        return;
    }

    for (int i = 0; i < tabela.m; i++)
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

    tabela.tabela = nullptr;
    tabela.m = 0;
    tabela.n = 0;
}

// Libera todos os dados da partida temporaria.
void limparPartidaCarregamento(Partida &partida)
{
    limparFilaCarregamento(partida.filaClientes);
    limparPilhaCarregamento(partida.hamburguer);
    destruirHeap(partida.e.pereciveis);
    limparCacheCarregamento(partida.cache);
}

// Verifica se uma quantidade lida do arquivo e valida.
bool quantidadeValidaCarregamento(int quantidade)
{
    return quantidade >= 0 && quantidade <= 1000000;
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

    std::string marcador;
    int versao;

    // 1. Identificacao do arquivo.
    if (!(arquivo >> marcador >> versao) ||
        marcador != "DADOSBURGER_SAVE" ||
        versao != 1)
    {

        std::cerr << "Arquivo invalido ou versao desconhecida.\n";
        return false;
    }

    // 2. Dados principais.
    if (!(arquivo >> marcador) || marcador != "DADOS" ||
        !(arquivo >> temp.id >> temp.turnoAtual >> temp.pontuacao))
    {

        std::cerr << "Erro ao ler os dados da partida.\n";
        return false;
    }

    // 3. Fila de clientes.
    int quantidadeClientes;

    if (!(arquivo >> marcador) || marcador != "FILA" ||
        !(arquivo >> quantidadeClientes) ||
        !quantidadeValidaCarregamento(quantidadeClientes))
    {

        std::cerr << "Erro ao ler a fila de clientes.\n";
        return false;
    }

    for (int i = 0; i < quantidadeClientes; i++)
    {

        Pedido pedido;

        if (!(arquivo >> pedido.cod >> pedido.nome))
        {
            std::cerr << "Erro ao ler um pedido.\n";
            limparPartidaCarregamento(temp);
            return false;
        }

        if (!enfileira(temp.filaClientes, pedido))
        {
            std::cerr << "Erro ao reconstruir a fila.\n";
            limparPartidaCarregamento(temp);
            return false;
        }
    }

    // 4. Pilha do hamburguer.
    int quantidadeIngredientes;

    if (!(arquivo >> marcador) || marcador != "PILHA" ||
        !(arquivo >> quantidadeIngredientes) ||
        !quantidadeValidaCarregamento(quantidadeIngredientes))
    {

        std::cerr << "Erro ao ler a pilha.\n";
        limparPartidaCarregamento(temp);
        return false;
    }

    // Usamos uma segunda pilha temporaria para preservar a ordem.
    PilhaEnc auxiliar;
    inicializa(auxiliar);

    for (int i = 0; i < quantidadeIngredientes; i++)
    {

        IngredientePilha ingrediente;

        if (!(arquivo >> ingrediente.cod))
        {

            std::cerr << "Erro ao ler um ingrediente.\n";
            limparPilhaCarregamento(auxiliar);
            limparPartidaCarregamento(temp);
            return false;
        }

        if (!empilha(auxiliar, ingrediente))
        {
            std::cerr << "Erro ao reconstruir a pilha auxiliar.\n";
            limparPilhaCarregamento(auxiliar);
            limparPartidaCarregamento(temp);
            return false;
        }
    }

    // Transfere os ingredientes para a pilha definitiva.
    // Isso preserva a ordem original armazenada no arquivo.
    IngredientePilha ingrediente;

    while (desempilha(auxiliar, ingrediente))
    {

        if (!empilha(temp.hamburguer, ingrediente))
        {
            std::cerr << "Erro ao reconstruir a pilha do hamburguer.\n";
            limparPilhaCarregamento(auxiliar);
            limparPartidaCarregamento(temp);
            return false;
        }
    }

    // 5. Estoque.
    if (!(arquivo >> marcador) || marcador != "ESTOQUE" ||
        !(arquivo >> temp.e.queijo_suico >> temp.e.cheddar >> temp.e.cebola >> temp.e.tomate))
    {

        std::cerr << "Erro ao ler o estoque.\n";
        limparPartidaCarregamento(temp);
        return false;
    }

    // 6. Heap de pereciveis.
    char comida;
    int validadeConfig;
    int capacidade;
    int tamanhoHeap;

    if (!(arquivo >> marcador) || marcador != "HEAP" ||
        !(arquivo >> comida >> validadeConfig >> capacidade >> tamanhoHeap) ||
        capacidade <= 0 ||
        tamanhoHeap < 0 ||
        tamanhoHeap > capacidade)
    {

        std::cerr << "Erro ao ler os dados do heap.\n";
        limparPartidaCarregamento(temp);
        return false;
    }

    inicializarHeap(temp.e.pereciveis,
                    comida,
                    capacidade,
                    validadeConfig);

    if (temp.e.pereciveis.vetor == nullptr)
    {
        std::cerr << "Erro ao alocar o heap.\n";
        limparPartidaCarregamento(temp);
        return false;
    }

    // Recupera as validades sem recalcula-las.
    // Recupera a comida e a validade de cada ingrediente.
    for (int i = 0; i < tamanhoHeap; i++)
    {

        if (!(arquivo >> temp.e.pereciveis.vetor[i].comida >> temp.e.pereciveis.vetor[i].turno_validade))
        {
            std::cerr << "Erro ao ler um ingrediente do heap.\n";
            limparPartidaCarregamento(temp);
            return false;
        }
    }
    temp.e.pereciveis.tamanho = tamanhoHeap;
    buildHeap(temp.e.pereciveis);

    // 7. Cache.
    int capacidadeCache;
    int quantidadeCache;

    if (!(arquivo >> marcador) || marcador != "CACHE" ||
        !(arquivo >> capacidadeCache >> quantidadeCache) ||
        capacidadeCache < 0 ||
        !quantidadeValidaCarregamento(quantidadeCache) ||
        (capacidadeCache == 0 && quantidadeCache != 0))
    {

        std::cerr << "Erro ao ler o cache.\n";
        limparPartidaCarregamento(temp);
        return false;
    }

    if (capacidadeCache > 0)
    {

        encInicializar(temp.cache, capacidadeCache);

        for (int i = 0; i < quantidadeCache; i++)
        {

            int chave;
            int valor;

            if (!(arquivo >> chave >> valor))
            {
                std::cerr << "Erro ao ler um elemento do cache.\n";
                limparPartidaCarregamento(temp);
                return false;
            }

            encInserir(temp.cache, chave, valor);
        }
    }

    // 8. Final do arquivo.
    if (!(arquivo >> marcador) || marcador != "FIM")
    {
        std::cerr << "Arquivo de partida incompleto.\n";
        limparPartidaCarregamento(temp);
        return false;
    }

    arquivo.close();

    // Transfere os dados reconstruidos para a partida recebida.
    p = temp;

    return true;
}
