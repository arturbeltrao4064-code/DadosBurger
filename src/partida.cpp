#include "partida.hpp"
#include "receita.hpp"
#include "timer.hpp"
#include <iostream>
#include <cstdlib>

double tempoTotalBuscas = 0.0;

static int menorCodigo(const NoAVL *no) {
    if (no == nullptr) return 0;
    while (no->esq != nullptr) no = no->esq;
    return no->prod.cod;
}

static int maiorCodigo(const NoAVL *no) {
    if (no == nullptr) return 0;
    while (no->dir != nullptr) no = no->dir;
    return no->prod.cod;
}

static void limparHamburguer(PilhaEnc& pilha) {
    Ingrediente removido;
    while (desempilha(pilha, removido)) {}
}

static void limparVencidos(Partida& p) {
    int i;
    for (i = 0; i < 3; i++) {
        while (!estaVazio(p.e.pereciveis[i]) &&
               p.e.pereciveis[i].vetor[0].turno_validade <= p.turnoAtual) {
            remover_min(p.e.pereciveis[i]);
        }
    }
}

static void avancarTurno(Partida& p) {
    p.pacienciaAtual--;
    p.turnoAtual++;
}

static bool receitaCorreta(const std::string& texto, const PilhaEnc& pilha) {
    ListaReceita receita;
    PilhaEnc auxiliar;
    inicializar(receita);
    inicializa(auxiliar);

    int i;
    for (i = 0; i < (int)texto.size(); i++) {
        if (!inserirFim(receita, texto[i])) {
            destruir(receita);
            limparHamburguer(auxiliar);
            return false;
        }
    }

    const NoPilha *atual = pilha.topo;
    while (atual != nullptr) {
        if (!empilha(auxiliar, atual->dado)) {
            destruir(receita);
            limparHamburguer(auxiliar);
            return false;
        }
        atual = atual->prox;
    }

    NoReceita *item = receita.ini;
    Ingrediente ingrediente;
    bool correta = true;

    while (item != nullptr) {
        if (!desempilha(auxiliar, ingrediente) ||
            ingrediente.comida != item->ingrediente) {
            correta = false;
            break;
        }
        item = item->prox;
    }

    if (item != nullptr || !estaVazia(auxiliar)) correta = false;

    destruir(receita);
    limparHamburguer(auxiliar);
    return correta;
}

void inicializarPartida(Partida& p, const NoAVL* pedidos, int maiorId) {
    inicializa(p.filaClientes);
    inicializa(p.hamburguer);

    p.e = {};
    p.cache = {};
    p.id = maiorId + 1;
    p.turnoAtual = 1;
    p.pontuacao = 0;
    p.pacienciaAtual = 15;

    p.e.queijo_suico.comida = 's';
    p.e.queijo_suico.turno_validade = 0;
    p.e.queijo_suico.qnt = 0;
    p.e.cheddar.comida = 'q';
    p.e.cheddar.turno_validade = 0;
    p.e.cheddar.qnt = 0;
    p.e.cebola.comida = 'c';
    p.e.cebola.turno_validade = 0;
    p.e.cebola.qnt = 0;
    p.e.tomate.comida = 't';
    p.e.tomate.turno_validade = 0;
    p.e.tomate.qnt = 0;

    inicializarHeap(p.e.pereciveis[0], 'n', 1000, 3);
    inicializarHeap(p.e.pereciveis[1], 'o', 1000, 3);
    inicializarHeap(p.e.pereciveis[2], 'b', 1000, 3);
    encInicializar(p.cache, 101);

    int menor = menorCodigo(pedidos);
    int maior = maiorCodigo(pedidos);
    int clientes = 0;

    while (clientes < CLIENTES && maior >= menor) {
        int codigo = menor + std::rand() % (maior - menor + 1);
        std::string nome = encBuscarReceita(p.cache, codigo);

        if (nome != "") {
            totalHits++;
        } else {
            totalMisses++;
            Timer cronometro;
            timer_iniciar(&cronometro);
            const NoAVL *encontrado = Buscar(pedidos, codigo);
            tempoTotalBuscas += timer_parar(&cronometro);

            if (encontrado == nullptr) continue;
            nome = encontrado->prod.nome;
            encInserirReceita(p.cache, codigo, nome);
        }

        Pedido pedido;
        pedido.cod = codigo;
        pedido.nome = nome;
        if (enfileira(p.filaClientes, pedido)) clientes++;
    }
}

bool executarPartida(Partida& p) {
    bool finalizada = false;

    while (!estaVazia(p.filaClientes)) {
        limparVencidos(p);

        std::cout << "\n========== DADO BURGER ==========\n";
        std::cout << "Turno: " << p.turnoAtual << "\n";
        std::cout << "Pontuacao: " << p.pontuacao << "\n";
        std::cout << "Pedido: " << p.filaClientes.frente->dado.nome << "\n";
        std::cout << "Paciencia restante: " << p.pacienciaAtual << "\n";
        std::cout << "Hamburguer (topo para base): ";
        if (estaVazia(p.hamburguer)) std::cout << "(vazio)";
        const NoPilha *parte = p.hamburguer.topo;
        while (parte != nullptr) {
            std::cout << parte->dado.comida << " ";
            parte = parte->prox;
        }
        std::cout << "\n";
        std::cout << "[I] Preparar ingrediente\n";
        std::cout << "[A] Adicionar ingrediente\n";
        std::cout << "[R] Remover ingrediente do topo\n";
        std::cout << "[E] Consultar estoque (nao consome turno)\n";
        std::cout << "[P] Consultar pedido (nao consome turno)\n";
        std::cout << "[S] Encerrar partida\n";
        std::cout << "Acao: ";

        char acao;
        std::cin >> acao;
        if (acao >= 'a' && acao <= 'z') acao = acao - 'a' + 'A';

        if (acao == 'S') {
            finalizada = true;
            break;
        }

        if (acao == 'E') {
            printaEstoque(p.e);
            continue;
        }

        if (acao == 'P') {
            std::cout << "Codigo: " << p.filaClientes.frente->dado.cod << "\n";
            std::cout << "Receita: " << p.filaClientes.frente->dado.nome << "\n";
            continue;
        }

        if (acao == 'I') {
            char ingrediente;
            std::cout << "Preparar [S] suico [Q] cheddar [C] cebola [T] tomate [N] carne [O] ovo [B] bacon: ";
            std::cin >> ingrediente;
            if (ingrediente >= 'A' && ingrediente <= 'Z') ingrediente += 'a' - 'A';

            if (ingrediente == 's') p.e.queijo_suico.qnt++;
            else if (ingrediente == 'q') p.e.cheddar.qnt++;
            else if (ingrediente == 'c') p.e.cebola.qnt++;
            else if (ingrediente == 't') p.e.tomate.qnt++;
            else if (ingrediente == 'n' || ingrediente == 'o' || ingrediente == 'b') {
                int indice = 0;
                if (ingrediente == 'o') indice = 1;
                if (ingrediente == 'b') indice = 2;
                Ingrediente novo;
                novo.comida = ingrediente;
                novo.turno_validade = 0;
                novo.qnt = 1;
                if (!inserir(p.e.pereciveis[indice], novo, p.turnoAtual)) {
                    std::cout << "Nao foi possivel preparar: estoque cheio.\n";
                    continue;
                }
            } else {
                std::cout << "Ingrediente invalido.\n";
                continue;
            }
            std::cout << "Ingrediente preparado.\n";
            avancarTurno(p);
        }
        else if (acao == 'A') {
            char ingrediente;
            Ingrediente novo;
            novo.comida = '\0';
            novo.turno_validade = 0;
            novo.qnt = 1;

            if (!estaVazia(p.hamburguer) &&
                p.hamburguer.topo->dado.comida == 'z') {
                std::cout << "O pao de cima ja foi colocado. Remova-o antes de continuar.\n";
                continue;
            }

            std::cout << "Adicionar [K] ketchup [M] mostarda [P] picles [A] alface [Z] pao [S] suico [Q] cheddar [C] cebola [T] tomate [N] carne [O] ovo [B] bacon: ";
            std::cin >> ingrediente;
            if (ingrediente >= 'A' && ingrediente <= 'Z') ingrediente += 'a' - 'A';

            bool disponivel = true;
            int indice = -1;

            if (ingrediente == 'k' || ingrediente == 'm' ||
                ingrediente == 'p' || ingrediente == 'a' || ingrediente == 'z') {
                novo.comida = ingrediente;
            }
            else if (ingrediente == 's' && p.e.queijo_suico.qnt > 0) {
                novo = p.e.queijo_suico;
            }
            else if (ingrediente == 'q' && p.e.cheddar.qnt > 0) {
                novo = p.e.cheddar;
            }
            else if (ingrediente == 'c' && p.e.cebola.qnt > 0) {
                novo = p.e.cebola;
            }
            else if (ingrediente == 't' && p.e.tomate.qnt > 0) {
                novo = p.e.tomate;
            }
            else if (ingrediente == 'n' || ingrediente == 'o' || ingrediente == 'b') {
                if (ingrediente == 'o') indice = 1;
                else if (ingrediente == 'b') indice = 2;
                else indice = 0;
                if (estaVazio(p.e.pereciveis[indice])) disponivel = false;
                else novo = remover_min(p.e.pereciveis[indice]);
            }
            else disponivel = false;

            if (!disponivel) {
                std::cout << "Ingrediente invalido ou sem estoque.\n";
                continue;
            }

            if (!empilha(p.hamburguer, novo)) {
                std::cout << "Falha ao adicionar ingrediente.\n";
                continue;
            }

            if (ingrediente == 's') p.e.queijo_suico.qnt--;
            if (ingrediente == 'q') p.e.cheddar.qnt--;
            if (ingrediente == 'c') p.e.cebola.qnt--;
            if (ingrediente == 't') p.e.tomate.qnt--;

            avancarTurno(p);

            if (ingrediente == 'z') {
                if (receitaCorreta(p.filaClientes.frente->dado.nome, p.hamburguer)) {
                    Pedido atendido;
                    desenfileira(p.filaClientes, atendido);
                    p.pontuacao++;
                    limparHamburguer(p.hamburguer);
                    p.pacienciaAtual = 15;
                    std::cout << "Pedido correto! Cliente atendido.\n";
                    if (estaVazia(p.filaClientes)) {
                        std::cout << "VITORIA! Todos os clientes foram atendidos.\n";
                        finalizada = true;
                        break;
                    }
                } else {
                    std::cout << "Receita incorreta. Remova ingredientes ou o pao e tente novamente.\n";
                }
            }
        }
        else if (acao == 'R') {
            Ingrediente removido;
            if (!desempilha(p.hamburguer, removido)) {
                std::cout << "O hamburguer esta vazio.\n";
                continue;
            }
            std::cout << "Ingrediente removido: " << removido.comida << "\n";
            avancarTurno(p);
        }
        else {
            std::cout << "Opcao invalida.\n";
            continue;
        }

        if (p.pacienciaAtual <= 0) {
            std::cout << "O cliente perdeu a paciencia. Fim de jogo.\n";
            finalizada = true;
            break;
        }
    }

    return finalizada;
}
