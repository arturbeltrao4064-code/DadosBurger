#include "partida.hpp"
#include <iostream>
#include <cstdlib>

void inicializarPartida(Partida& p, const NoAVL* pedidos, int maiorId) {
    inicializa(p.filaClientes);
    inicializa(p.hamburguer);
    p.e = {};
    p.cache = {};
    p.id = maiorId + 1;
    p.turnoAtual = 0;
    p.pontuacao = 0;

    for (int i = 0; i < CLIENTES; i++) {
        int x = std::rand() % 87000;
        const NoAVL* tmp = Buscar(pedidos, x);
        if (tmp != nullptr)
            enfileira(p.filaClientes, tmp->prod);
    }
}

void executarPartida(Partida& p){
    char jogando = 'J';
    while(jogando == 'J'){
        std::cout << "Turno atual:" << p.turnoAtual << '\n' <<
                        "Acoes disponiveis:\n"
                        "As seguintes acoes consomem 1 turno cada:\n"
                        "[I]: Preparar ingrediente\n[A]: Adicionar ingrediente\n[R]: Remover ingrediente do topo do hamburguer.\n"
                        "Acoes de consulta (nao consomem turnos):\n"
                        "[E]: Ver estoque\n[P]: Ver pedido.\n"
                        "Pressione [S] para salvar a partida.\n";
        std::cin >> jogando;
        toupper(jogando);
        if(jogando != 'I' && jogando != 'A' && jogando != 'R' && jogando != 'E' && jogando != 'P' && jogando != 'M'){
            jogando = 'J';
            std::cout << "Erro! Digite um caracter valido";            
        }
        if(jogando == 'I'){
            char ingrediente;
            std::cout << "Qual ingrediente deseja preparar?\nDigite [J] para realizar outra acao.";
            std::cin >> ingrediente;
            tolower(ingrediente);
            while(ingrediente != 's' && ingrediente != 'q' && ingrediente != 'c' && ingrediente != 't' && ingrediente != 'n' && ingrediente != 'o' && ingrediente != 'b'){
                std::cout << "Codigo de ingrediente invalido. Tente novamente!";
            }
            p.turnoAtual++;
            if(ingrediente == 's'){
                p.e.queijo_suico.qnt++;
                std::cout << "Queijo suico preparado!";
            }
            if(ingrediente == 'q'){
                p.e.cheddar.qnt++;
                std::cout << "Queijo cheddar preparado!";
            }
            if(ingrediente == 'c'){
                p.e.cebola.qnt++;
                std::cout << "Cebola preparada!";
            }
            if(ingrediente == 't'){
                p.e.tomate.qnt++;
                std::cout << "Tomate preparado!";
            }
            if(ingrediente == 'n'){
                Ingrediente tmp = {'n', 3, 1};
                inserir(p.e.pereciveis[0], tmp, p.turnoAtual);
                std::cout << "Carne preparada!";
            }
            if(ingrediente == 'o'){
                Ingrediente tmp = {'o', 3, 1};
                inserir(p.e.pereciveis[1], tmp, p.turnoAtual);
                std::cout << "Ovo frito preparado!";
            }
            if(ingrediente == 'b'){
                Ingrediente tmp = {'b', 3, 1};
                inserir(p.e.pereciveis[2], tmp, p.turnoAtual);
                std::cout << "Bacon preparado!";
            }
            jogando = 'J';
        }
        if(jogando == 'A'){
            char ingrediente;
            std::cout << "Qual ingrediente deseja adicionar ao hamburguer?";
            std::cin >> ingrediente;
            tolower(ingrediente);
            while(ingrediente != 's' && ingrediente != 'q' && ingrediente != 'c' && ingrediente != 't' && ingrediente != 'n' && ingrediente != 'o' && ingrediente != 'b' && ingrediente != 'z' && ingrediente != 'k' && ingrediente != 'm' && ingrediente != 'p' && ingrediente != 'a'){
                std::cout << "Codigo de ingrediente invalido. Tente novamente!";
            }
            p.turnoAtual++;
            if(ingrediente == 's'){
                p.e.queijo_suico.qnt--;
                empilha(p.hamburguer, p.e.queijo_suico);
                std::cout << "Queijo suico adicionado ao hamburguer!";
            }
            if(ingrediente == 'q'){
                p.e.cheddar.qnt--;
                empilha(p.hamburguer, p.e.cheddar);
                std::cout << "Queijo cheddar adicionado ao hamburguer!";
            }
            if(ingrediente == 'c'){
                p.e.cebola.qnt--;
                empilha(p.hamburguer, p.e.cebola);
                std::cout << "Cebola adicionada ao hamburguer!";
            }
            if(ingrediente == 't'){
                p.e.tomate.qnt--;
                empilha(p.hamburguer, p.e.tomate);
                std::cout << "Tomate adicionado ao hamburguer!";
            }
            if(ingrediente == 'n'){
                Ingrediente tmp = remover_min(p.e.pereciveis[0]);
                empilha(p.hamburguer, tmp);
                std::cout << "Carne adicionada ao hamburguer!";
            }
            if(ingrediente == 'o'){
                Ingrediente tmp = remover_min(p.e.pereciveis[1]);
                empilha(p.hamburguer, tmp);
                std::cout << "Ovo frito adicionado ao hamburguer!";
            }
            if(ingrediente == 'b'){
                Ingrediente tmp = remover_min(p.e.pereciveis[2]);
                empilha(p.hamburguer, tmp);
                std::cout << "Bacon adicionado ao hamburguer!";
            }
            jogando = 'J';
        }
        if(jogando == 'R'){
            char ingrediente;
            std::cout << "Qual ingrediente deseja remover do hamburguer?";
            std::cin >> ingrediente;
            tolower(ingrediente);
            while(ingrediente != 's' && ingrediente != 'q' && ingrediente != 'c' && ingrediente != 't' && ingrediente != 'n' && ingrediente != 'o' && ingrediente != 'b' && ingrediente != 'z' && ingrediente != 'k' && ingrediente != 'm' && ingrediente != 'p' && ingrediente != 'a'){
                std::cout << "Codigo de ingrediente invalido. Tente novamente!";
            }
            p.turnoAtual++;
            if(ingrediente == 's'){
                Ingrediente tmp;
                desempilha(p.hamburguer, tmp);
                std::cout << "Queijo suico removido do hamburguer!";
            }
            if(ingrediente == 'q'){
                Ingrediente tmp;
                desempilha(p.hamburguer, tmp);
                std::cout << "Queijo cheddar removido do hamburguer!";
            }
            if(ingrediente == 'c'){
                Ingrediente tmp;
                desempilha(p.hamburguer, tmp);
                std::cout << "Cebola removida do hamburguer!";
            }
            if(ingrediente == 't'){
                Ingrediente tmp;
                desempilha(p.hamburguer, tmp);
                std::cout << "Tomate removido do hamburguer!";
            }
            if(ingrediente == 'n'){
                Ingrediente tmp;
                desempilha(p.hamburguer, tmp);
                std::cout << "Carne removida do hamburguer!";
            }
            if(ingrediente == 'o'){
                Ingrediente tmp;
                desempilha(p.hamburguer, tmp);
                std::cout << "Ovo frito removido do hamburguer!";
            }
            if(ingrediente == 'b'){
                Ingrediente tmp;
                desempilha(p.hamburguer, tmp);
                std::cout << "Bacon removido do hamburguer!";
            }
            jogando = 'J';
        }
        if(jogando == 'E'){
            printaEstoque(p.e);
            jogando = 'J';
        }
        if(jogando == 'P'){
            std::cout << "Pedido atual:\nNome: " << p.filaClientes.frente->dado.nome << '\n';
            std::cout << "Receita: " << p.filaClientes.frente->dado.cod << '\n';
            jogando = 'J';
        }
    }
}