
#include "partida.hpp"

#include <fstream>
#include <iostream>
#include <string>

// Verifica se um nome pode ser gravado no formato atual.
// O carregador atual le nomes usando operador >>,
// portanto nomes com espacos nao sao suportados.
bool nomeValidoParaArquivo(const std::string& nome) {
    if (nome.empty())
        return false;

    for (int i = 0; nome[i] != '\0'; i++) {
        if (nome[i] == ' ' || nome[i] == '\t' ||
            nome[i] == '\n' || nome[i] == '\r') {
            return false;
        }
    }

    return true;
}

bool salvarPartida(const Partida& p, const std::string& caminho) {

    std::ofstream arquivo(caminho.c_str());

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para salvar a partida.\n";
        return false;
    }

    // 1. Identificacao e versao do arquivo.
    arquivo << "DADOSBURGER_SAVE 1\n";

    // 2. Dados principais da partida.
    arquivo << "DADOS\n";
    arquivo << p.id << ' '
            << p.turnoAtual << ' '
            << p.pontuacao << '\n';

    // 3. Fila de clientes.
    arquivo << "FILA\n";

    int quantidadeClientes = 0;
    const NoFila* cliente = p.filaClientes.frente;

    while (cliente != nullptr) {
        if (!nomeValidoParaArquivo(cliente->dado.nome)) {
            std::cerr << "Nome de pedido invalido para o formato atual.\n";
            arquivo.close();
            return false;
        }

        quantidadeClientes++;
        cliente = cliente->prox;
    }

    arquivo << quantidadeClientes << '\n';

    cliente = p.filaClientes.frente;

    while (cliente != nullptr) {
        arquivo << cliente->dado.cod << ' '
                << cliente->dado.nome << '\n';

        cliente = cliente->prox;
    }

    // 4. Pilha do hamburguer.
    arquivo << "PILHA\n";

    int quantidadeIngredientes = 0;
    const NoPilha* ingrediente = p.hamburguer.topo;

   while (ingrediente != nullptr) {
    quantidadeIngredientes++;
    ingrediente = ingrediente->prox;
}

    arquivo << quantidadeIngredientes << '\n';

    // Grava do topo para a base.
    ingrediente = p.hamburguer.topo;

    while (ingrediente != nullptr) {
    arquivo << ingrediente->dado.cod << '\n';

    ingrediente = ingrediente->prox;
}

    // 5. Estoque.
    arquivo << "ESTOQUE\n";

    arquivo << p.e.queijo_suico << ' '
            << p.e.cheddar << ' '
            << p.e.cebola << ' '
            << p.e.tomate << '\n';

    // 6. Heap de pereciveis.
    const heap& h = p.e.pereciveis;

    if (h.tamanho < 0 ||
        h.capacidade < 0 ||
        h.tamanho > h.capacidade ||
        (h.tamanho > 0 && h.vetor == nullptr)) {

        std::cerr << "Estado invalido do heap de pereciveis.\n";
        arquivo.close();
        return false;
    }

    arquivo << "HEAP\n";

    arquivo << h.comida << ' '
            << h.validade_config << ' '
            << h.capacidade << ' '
            << h.tamanho << '\n';

    // Gravamos as validades armazenadas no vetor.
    // O carregador reconstruira a propriedade de heap.
    for (int i = 0; i < h.tamanho; i++) {
    arquivo << h.vetor[i].comida << ' '
            << h.vetor[i].turno_validade << '\n';
}

    // 7. Cache.
    const TabelaEnc& tabela = p.cache;

    if (tabela.m < 0 || tabela.n < 0 ||
        (tabela.m > 0 && tabela.tabela == nullptr)) {

        std::cerr << "Estado invalido do cache.\n";
        arquivo.close();
        return false;
    }

    arquivo << "CACHE\n";
    arquivo << tabela.m << ' ' << tabela.n << '\n';

    if (tabela.tabela != nullptr) {
        for (int i = 0; i < tabela.m; i++) {
            const No* atual = tabela.tabela[i];

            while (atual != nullptr) {
                arquivo << atual->chave << ' '
                        << atual->valor << '\n';

                atual = atual->prox;
            }
        }
    }

    // 8. Marcador de final.
    arquivo << "FIM\n";

    if (!arquivo.good()) {
        std::cerr << "Erro durante a escrita do arquivo.\n";
        arquivo.close();
        return false;
    }

    arquivo.close();

    return true;
}
