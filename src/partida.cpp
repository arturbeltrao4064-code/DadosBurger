#include "partida.hpp"
#include <iostream>
#include <cstdlib>
#include <cctype>

void inicializarPartida(Partida& p, const NoAVL* pedidos, int maiorId) {
    inicializa(p.filaClientes);
    inicializa(p.hamburguer);
    p.e = {};
    p.cache = {};
    p.id = maiorId + 1;
    p.turnoAtual = 0;
    p.pontuacao = 0;
    p.pacienciaAtual = 15;

    p.e.queijo_suico = {'s', 0, 0};
    p.e.cheddar = {'q', 0, 0};
    p.e.cebola = {'c', 0, 0};
    p.e.tomate = {'t', 0, 0};

    inicializarHeap(p.e.pereciveis[0], 'n', 10, 3);
    inicializarHeap(p.e.pereciveis[1], 'o', 10, 3);
    inicializarHeap(p.e.pereciveis[2], 'b', 10, 3);
    encInicializar(p.cache, 11);

    for (int i = 0; i < CLIENTES; i++) {
        int x = std::rand() % 15 + 1;
        const NoAVL* tmp = Buscar(pedidos, x);
        if (tmp != nullptr)
            enfileira(p.filaClientes, tmp->prod);
    }
}

bool executarPartida(Partida& p) {
    char jogando = 'J';
    bool partidaFinalizada = false;

    while (jogando == 'J') {
        if (estaVazia(p.filaClientes)) {
            std::cout << "\nParabens! Todos os clientes foram atendidos!\n";
            partidaFinalizada = true;
            break;
        }

        std::cout << "\n========== DADOS BURGER ==========\n";
        std::cout << "Turno: " << p.turnoAtual << '\n';
        std::cout << "Pontuacao: " << p.pontuacao << '\n';
        std::cout << "Cliente: " << p.filaClientes.frente->dado.nome << '\n';
        std::cout << "Paciencia restante: " << p.pacienciaAtual << " turnos\n";
        std::cout << "[I] Preparar ingrediente (1 turno)\n";
        std::cout << "[A] Adicionar ingrediente ao hamburguer (1 turno)\n";
        std::cout << "[R] Remover ingrediente do topo (1 turno)\n";
        std::cout << "[M] Montar/entregar pedido (1 turno)\n";
        std::cout << "[E] Consultar estoque\n";
        std::cout << "[P] Consultar pedido\n";
        std::cout << "[S] Salvar e voltar ao menu\n";
        std::cin >> jogando;
        jogando = toupper(jogando);

        if (jogando == 'S') {
            if (salvarPartida(p, "partida.txt"))
                std::cout << "Partida salva. Voltando ao menu.\n";
            else
                std::cout << "Nao foi possivel salvar a partida.\n";
            break;
        }

        if (jogando == 'E') {
            printaEstoque(p.e);
            jogando = 'J';
            continue;
        }

        if (jogando == 'P') {
            std::cout << "Pedido atual: " << p.filaClientes.frente->dado.nome << '\n';
            std::cout << "Codigo da receita: " << p.filaClientes.frente->dado.cod << '\n';
            std::cout << "Paciencia: " << p.pacienciaAtual << "/15\n";
            jogando = 'J';
            continue;
        }

        if (jogando == 'I') {
            char ingrediente;
            std::cout << "Preparar: [S] queijo suico, [Q] cheddar, [C] cebola, [T] tomate, [N] carne, [O] ovo, [B] bacon: ";
            std::cin >> ingrediente;
            ingrediente = tolower(ingrediente);

            if (ingrediente == 's'){p.e.queijo_suico.qnt++;}
            else if (ingrediente == 'q') {p.e.cheddar.qnt++;}
            else if (ingrediente == 'c') {p.e.cebola.qnt++;}
            else if (ingrediente == 't') {p.e.tomate.qnt++;}
            else if (ingrediente == 'n' || ingrediente == 'o' || ingrediente == 'b') {
                int indice;
                if(ingrediente == 'n') indice = 0;
                if(ingrediente == 'o') indice = 1;
                if(ingrediente == 'b') indice = 2;
                Ingrediente novo = {ingrediente, 0, 1};
                if (!inserir(p.e.pereciveis[indice], novo, p.turnoAtual)) {
                    std::cout << "Estoque perecivel cheio ou nao inicializado. Acao cancelada.\n";
                    jogando = 'J';
                    continue;
                }
            } else {
                std::cout << "Ingrediente invalido. Acao cancelada.\n";
                jogando = 'J';
                continue;
            }
            std::cout << "Ingrediente preparado.\n";
        }
        else if (jogando == 'A') {
            char ingrediente;
            Ingrediente adicionar = {'\0', 0, 0};
            bool disponivel = false;
            std::cout << "Adicionar: [S] queijo suico, [Q] cheddar, [C] cebola, [T] tomate, [N] carne, [O] ovo, [B] bacon: ";
            std::cin >> ingrediente;
            ingrediente = tolower(ingrediente);

            if (ingrediente == 's' && p.e.queijo_suico.qnt > 0) { adicionar = p.e.queijo_suico; disponivel = true; }
            else if (ingrediente == 'q' && p.e.cheddar.qnt > 0) { adicionar = p.e.cheddar; disponivel = true; }
            else if (ingrediente == 'c' && p.e.cebola.qnt > 0) { adicionar = p.e.cebola; disponivel = true; }
            else if (ingrediente == 't' && p.e.tomate.qnt > 0) { adicionar = p.e.tomate; disponivel = true; }
            else if (ingrediente == 'n' || ingrediente == 'o' || ingrediente == 'b') {
                int indice;
                if(ingrediente == 'n') indice = 0;
                if(ingrediente == 'o') indice = 1;
                if(ingrediente == 'b') indice = 2;
                if (!estaVazio(p.e.pereciveis[indice])) {
                    adicionar = remover_min(p.e.pereciveis[indice]);
                    if(adicionar.comida != '\0')
                        disponivel = true;
                    else
                        disponivel = false;
                }
            }

            if (!disponivel) {
                std::cout << "Ingrediente invalido ou sem estoque. Acao cancelada.\n";
                jogando = 'J';
                continue;
            }

            if (!empilha(p.hamburguer, adicionar)) {
                std::cout << "Falha ao adicionar ingrediente. Acao cancelada.\n";
                jogando = 'J';
                continue;
            }

            if (ingrediente == 's') p.e.queijo_suico.qnt--;
            else if (ingrediente == 'q') p.e.cheddar.qnt--;
            else if (ingrediente == 'c') p.e.cebola.qnt--;
            else if (ingrediente == 't') p.e.tomate.qnt--;
            std::cout << "Ingrediente adicionado ao hamburguer.\n";
        }
        else if (jogando == 'R') {
            Ingrediente removido;
            if (!desempilha(p.hamburguer, removido)) {
                std::cout << "O hamburguer esta vazio. Acao cancelada.\n";
                jogando = 'J';
                continue;
            }
            std::cout << "Ingrediente " << removido.comida << " removido do topo.\n";
        }
        else if (jogando == 'M') {
            if (estaVazia(p.hamburguer)) {
                std::cout << "O hamburguer esta vazio. Adicione ingredientes antes de entregar.\n";
                jogando = 'J';
                continue;
            }

            // A composicao da receita ainda nao e validada nesta versao.
            Pedido atendido;
            desenfileira(p.filaClientes, atendido);
            Ingrediente removido;
            while (desempilha(p.hamburguer, removido)) {}
            p.pontuacao += 100;
            p.turnoAtual++;
            p.pacienciaAtual = 15;
            std::cout << "Pedido de " << atendido.nome << " entregue! +100 pontos.\n";

            if (estaVazia(p.filaClientes)) {
                std::cout << "VITORIA! Todos os clientes foram atendidos.\n";
                partidaFinalizada = true;
                break;
            }

            jogando = 'J';
            continue;
        }
        else {
            std::cout << "Opcao invalida. Tente novamente.\n";
            jogando = 'J';
            continue;
        }

        p.turnoAtual++;
        p.pacienciaAtual--;

        for (int i = 0; i < 3; ++i) {
            while (!estaVazio(p.e.pereciveis[i]) &&
                   p.e.pereciveis[i].vetor[0].turno_validade < p.turnoAtual) {
                remover_min(p.e.pereciveis[i]);
            }
        }

        if (p.pacienciaAtual <= 0) {
            std::cout << "\nO cliente perdeu a paciencia! Fim de jogo.\n";
            std::cout << "Pontuacao final: " << p.pontuacao << '\n';
            partidaFinalizada = true;
            break;
        }

        jogando = 'J';
    }

    return partidaFinalizada;
}
