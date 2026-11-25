#include "partida.hpp"
#include "receita.hpp"
#include "timer.hpp"
#include "interface.hpp"
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

static bool receitaCorreta(const ListaReceita& receita, const PilhaEnc& pilha) {
    PilhaEnc auxiliar;
    inicializa(auxiliar);

    const NoPilha *atual = pilha.topo;
    while (atual != nullptr) {
        if (!empilha(auxiliar, atual->dado)) {
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
        unsigned long long sorteio = (unsigned long long)std::rand();
        sorteio = sorteio * ((unsigned long long)RAND_MAX + 1) + std::rand();
        int intervalo = maior - menor + 1;
        int codigo = menor + (int)(sorteio % (unsigned long long)intervalo);
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
        inicializar(pedido.receita);

        bool receitaCarregada = true;
        for (int i = 0; i < (int)nome.size(); i++) {
            if (!inserirFim(pedido.receita, nome[i])) {
                receitaCarregada = false;
                break;
            }
        }

        if (!receitaCarregada) {
            destruir(pedido.receita);
            continue;
        }

        if (enfileira(p.filaClientes, pedido)) clientes++;
        else destruir(pedido.receita);
    }
}

bool executarPartida(Partida& p) {
    bool finalizada = false;

    while (!estaVazia(p.filaClientes)) {
        limparVencidos(p);
        interfaceAtualizar(p);

        
        
        
        
        
        
        if (estaVazia(p.hamburguer)) 
        const NoPilha *parte = p.hamburguer.topo;
        while (parte != nullptr) {
            
            parte = parte->prox;
        }
        
        
        
        
        
        
        
        

        char acao;
        acao = interfaceLerAcao(p);
        if (acao >= 'a' && acao <= 'z') acao = acao - 'a' + 'A';

        if (acao == 'S') {
            finalizada = true;
            break;
        }

        if (acao == 'E') {
            interfaceMostrarEstoque(p.e);
            continue;
        }

        if (acao == 'P') {
            interfaceMostrarPedido(p.filaClientes.frente->dado);
            continue;
        }

        if (acao == 'I') {
            char ingrediente;
            
            ingrediente = interfaceLerIngrediente(p, true);
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
                    
                    continue;
                }
            } else {
                
                continue;
            }
            
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
                
                continue;
            }

            
            ingrediente = interfaceLerIngrediente(p, false);
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
                
                continue;
            }

            if (!empilha(p.hamburguer, novo)) {
                
                continue;
            }

            if (ingrediente == 's') p.e.queijo_suico.qnt--;
            if (ingrediente == 'q') p.e.cheddar.qnt--;
            if (ingrediente == 'c') p.e.cebola.qnt--;
            if (ingrediente == 't') p.e.tomate.qnt--;

            avancarTurno(p);

            if (ingrediente == 'z') {
                if (receitaCorreta(p.filaClientes.frente->dado.receita, p.hamburguer)) {
                    Pedido atendido;
                    desenfileira(p.filaClientes, atendido);
                    destruir(atendido.receita);
                    p.pontuacao++;
                    limparHamburguer(p.hamburguer);
                    p.pacienciaAtual = 15;
                    
                    if (estaVazia(p.filaClientes)) {
                        
                        finalizada = true;
                        break;
                    }
                } else {
                    
                }
            }
        }
        else if (acao == 'R') {
            Ingrediente removido;
            if (!desempilha(p.hamburguer, removido)) {
                
                continue;
            }
            
            avancarTurno(p);
        }
        else {
            
            continue;
        }

        if (p.pacienciaAtual <= 0) {
            
            finalizada = true;
            break;
        }
    }

    return finalizada;
}
