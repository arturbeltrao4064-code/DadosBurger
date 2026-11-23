
#include <iostream>
#include "partida.hpp"

using namespace std;

int main() {

    // ==========================================
    // 1. CRIAR A PARTIDA ORIGINAL
    // ==========================================

    Partida original = {};

    original.id = 42;
    original.turnoAtual = 7;
    original.pontuacao = 125;

    inicializa(original.filaClientes);
    inicializa(original.hamburguer);

    inicializarHeap(original.e.pereciveis, 'B', 10, 5);

    original.e.queijo_suico = 10;
    original.e.cheddar = 15;
    original.e.cebola = 20;
    original.e.tomate = 25;

    encInicializar(original.cache, 11);

    // Fila
    Pedido p1 = {101, "Pao"};
    Pedido p2 = {205, "Queijo"};

    enfileira(original.filaClientes, p1);
    enfileira(original.filaClientes, p2);

    // Pilha
    IngredientePilha i1 = {1, "Pao", 2.5f};
    IngredientePilha i2 = {2, "Carne", 8.0f};

    empilha(original.hamburguer, i1);
    empilha(original.hamburguer, i2);

    // Heap
    ingrediente perecivel = {0};
    inserir(original.e.pereciveis, perecivel, 4);

    // Cache
    encInserir(original.cache, 10, 500);

    cout << "Partida original criada.\n";


    // ==========================================
    // 2. SALVAR A PARTIDA
    // ==========================================

    if (!salvarPartida(original, "partida_teste.txt")) {
        cout << "ERRO AO SALVAR A PARTIDA!\n";
        return 1;
    }

    cout << "Partida salva com sucesso.\n";


    // ==========================================
    // 3. CARREGAR A PARTIDA
    // ==========================================

    Partida recuperada = {};

    if (!carregarPartida(recuperada, "partida_teste.txt")) {
        cout << "ERRO AO CARREGAR A PARTIDA!\n";
        return 1;
    }

    cout << "Partida carregada com sucesso.\n";


    // ==========================================
    // 4. VERIFICAR OS DADOS
    // ==========================================

    bool dadosCorretos = true;

    // Dados principais
    if (recuperada.id != original.id ||
        recuperada.turnoAtual != original.turnoAtual ||
        recuperada.pontuacao != original.pontuacao) {

        dadosCorretos = false;
    }

    // Estoque
    if (recuperada.e.queijo_suico != original.e.queijo_suico ||
        recuperada.e.cheddar != original.e.cheddar ||
        recuperada.e.cebola != original.e.cebola ||
        recuperada.e.tomate != original.e.tomate) {

        dadosCorretos = false;
    }

    // Fila
    NoFila* filaOriginal = original.filaClientes.frente;
    NoFila* filaRecuperada = recuperada.filaClientes.frente;

    while (filaOriginal != nullptr && filaRecuperada != nullptr) {

        if (filaOriginal->dado.cod != filaRecuperada->dado.cod ||
            filaOriginal->dado.nome != filaRecuperada->dado.nome) {

            dadosCorretos = false;
        }

        filaOriginal = filaOriginal->prox;
        filaRecuperada = filaRecuperada->prox;
    }

    if (filaOriginal != nullptr || filaRecuperada != nullptr) {
        dadosCorretos = false;
    }

    // Pilha
    NoPilha* pilhaOriginal = original.hamburguer.topo;
    NoPilha* pilhaRecuperada = recuperada.hamburguer.topo;

    while (pilhaOriginal != nullptr && pilhaRecuperada != nullptr) {

        if (pilhaOriginal->dado.cod != pilhaRecuperada->dado.cod ||
            pilhaOriginal->dado.nome != pilhaRecuperada->dado.nome ||
            pilhaOriginal->dado.preco != pilhaRecuperada->dado.preco) {

            dadosCorretos = false;
        }

        pilhaOriginal = pilhaOriginal->prox;
        pilhaRecuperada = pilhaRecuperada->prox;
    }

    if (pilhaOriginal != nullptr || pilhaRecuperada != nullptr) {
        dadosCorretos = false;
    }

    // Heap
    if (recuperada.e.pereciveis.tamanho != original.e.pereciveis.tamanho ||
        recuperada.e.pereciveis.capacidade != original.e.pereciveis.capacidade ||
        recuperada.e.pereciveis.validade_config != original.e.pereciveis.validade_config ||
        recuperada.e.pereciveis.comida != original.e.pereciveis.comida) {

        dadosCorretos = false;
    }

    for (int i = 0; i < original.e.pereciveis.tamanho; i++) {

        if (recuperada.e.pereciveis.vetor[i].turno_validade !=
            original.e.pereciveis.vetor[i].turno_validade) {

            dadosCorretos = false;
        }
    }

    // Cache
    if (recuperada.cache.m != original.cache.m ||
        recuperada.cache.n != original.cache.n ||
        encBuscar(recuperada.cache, 10) != 500) {

        dadosCorretos = false;
    }


    // ==========================================
    // 5. RESULTADO
    // ==========================================

    cout << "\n========== RESULTADO DO TESTE ==========\n";

    if (dadosCorretos) {
        cout << "TESTE APROVADO!\n";
        cout << "Todos os dados foram salvos e recuperados corretamente.\n";
    } else {
        cout << "TESTE FALHOU!\n";
        cout << "Algum dado foi perdido ou alterado.\n";
    }

    cout << "========================================\n";

    return dadosCorretos ? 0 : 1;
}
