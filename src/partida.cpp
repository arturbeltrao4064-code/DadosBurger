#include "partida.hpp"
#include "receita.hpp"
#include "timer.hpp"
#include "interface.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <cstdio>

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

bool executarPartida(Partida& p, bool& salvar) {
    bool finalizada = false;
    salvar = false;

    while (!estaVazia(p.filaClientes)) {
        limparVencidos(p);

        interfaceAtualizar(p);

        char acao;
        acao = interfaceLerAcao(p);
        if (acao >= 'a' && acao <= 'z') acao = acao - 'a' + 'A';

        if (acao == 'G') {
            salvar = true;
            break;
        }

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
                    interfaceMensagem("Estoque cheio: nao foi possivel preparar.");
                    continue;
                }
            } else {
                
                continue;
            }
            
            interfaceMensagem("Ingrediente preparado.");
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
                interfaceMensagem("O pao superior ja foi colocado. Remova-o primeiro.");
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
                interfaceMensagem("Ingrediente invalido ou sem estoque.");
                continue;
            }

            if (!empilha(p.hamburguer, novo)) {
                interfaceMensagem("Nao foi possivel adicionar o ingrediente.");
                continue;
            }

            if (ingrediente == 's') p.e.queijo_suico.qnt--;
            if (ingrediente == 'q') p.e.cheddar.qnt--;
            if (ingrediente == 'c') p.e.cebola.qnt--;
            if (ingrediente == 't') p.e.tomate.qnt--;

            interfaceMensagem("Ingrediente adicionado ao hamburguer.");
            avancarTurno(p);

            if (ingrediente == 'z') {
                if (receitaCorreta(p.filaClientes.frente->dado.receita, p.hamburguer)) {
                    Pedido atendido;
                    desenfileira(p.filaClientes, atendido);
                    destruir(atendido.receita);
                    p.pontuacao++;
                    limparHamburguer(p.hamburguer);
                    p.pacienciaAtual = 15;
                    interfaceMensagem("Pedido correto! Cliente atendido.");
                    
                    if (estaVazia(p.filaClientes)) {
                        
                        finalizada = true;
                        break;
                    }
                } else {
                    interfaceMensagem("Receita incorreta. Remova ingredientes e tente novamente.");
                }
            }
        }
        else if (acao == 'R') {
            Ingrediente removido;
            if (!desempilha(p.hamburguer, removido)) {
                interfaceMensagem("O hamburguer esta vazio.");
                continue;
            }
            
            interfaceMensagem("Ingrediente removido do topo.");
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


static void escreverReceita(std::ostream& arquivo, const ListaReceita& receita) {
    std::string texto;
    const NoReceita *item = receita.ini;
    while (item != nullptr) {
        texto += item->ingrediente;
        item = item->prox;
    }
    arquivo << std::quoted(texto) << '\n';
}

bool salvarPartida(const Partida& p, const char *caminho) {
    std::ofstream arq(caminho);
    if (!arq.is_open()) return false;

    arq << "DADOBURGER 1\n";
    arq << p.id << ' ' << p.turnoAtual << ' ' << p.pontuacao << ' '
        << p.pacienciaAtual << '\n';

    arq << tamanho(p.filaClientes) << '\n';
    const NoFila *cliente = p.filaClientes.frente;
    while (cliente != nullptr) {
        arq << cliente->dado.cod << ' ' << std::quoted(cliente->dado.nome) << ' ';
        escreverReceita(arq, cliente->dado.receita);
        cliente = cliente->prox;
    }

    int qtdPilha = tamanho(p.hamburguer);
    arq << qtdPilha << '\n';
    const NoPilha *parte = p.hamburguer.topo;
    while (parte != nullptr) {
        arq << parte->dado.comida << ' ' << parte->dado.turno_validade
            << ' ' << parte->dado.qnt << '\n';
        parte = parte->prox;
    }

    const Ingrediente simples[4] = {
        p.e.queijo_suico, p.e.cheddar, p.e.cebola, p.e.tomate
    };
    for (int i = 0; i < 4; i++)
        arq << simples[i].comida << ' ' << simples[i].turno_validade
            << ' ' << simples[i].qnt << '\n';

    for (int i = 0; i < 3; i++) {
        const heap& h = p.e.pereciveis[i];
        arq << h.comida << ' ' << h.validade_config << ' ' << h.tamanho << '\n';
        for (int j = 0; j < h.tamanho; j++)
            arq << h.vetor[j].comida << ' ' << h.vetor[j].turno_validade
                << ' ' << h.vetor[j].qnt << '\n';
    }

    arq << totalBuscas << ' ' << totalComparacoesBusca << ' '
        << tempoTotalBuscas << ' ' << totalHits << ' ' << totalMisses << ' '
        << totalColisoes << ' ' << totalEntradasCache << '\n';
    arq.close();
    return arq.good();
}

bool carregarPartida(Partida& p, const char *caminho) {
    std::ifstream arq(caminho);
    if (!arq.is_open()) return false;

    std::string marca;
    int versao;
    if (!(arq >> marca >> versao) || marca != "DADOBURGER" || versao != 1)
        return false;

    Partida nova = {};
    inicializa(nova.filaClientes);
    inicializa(nova.hamburguer);

    if (!(arq >> nova.id >> nova.turnoAtual >> nova.pontuacao
              >> nova.pacienciaAtual)) return false;

    int clientes;
    if (!(arq >> clientes) || clientes < 1 || clientes > CLIENTES) return false;
    for (int i = 0; i < clientes; i++) {
        Pedido pedido;
        std::string receita;
        if (!(arq >> pedido.cod >> std::quoted(pedido.nome)
                  >> std::quoted(receita))) return false;
        inicializar(pedido.receita);
        bool ok = true;
        for (int j = 0; j < (int)receita.size(); j++)
            if (!inserirFim(pedido.receita, receita[j])) { ok = false; break; }
        if (!ok || !enfileira(nova.filaClientes, pedido)) {
            destruir(pedido.receita);
            return false;
        }
    }

    int qtdPilha;
    if (!(arq >> qtdPilha) || qtdPilha < 0 || qtdPilha > 10000) return false;
    Ingrediente itens[10000];
    for (int i = 0; i < qtdPilha; i++)
        if (!(arq >> itens[i].comida >> itens[i].turno_validade >> itens[i].qnt))
            return false;
    for (int i = qtdPilha - 1; i >= 0; i--)
        if (!empilha(nova.hamburguer, itens[i])) return false;

    Ingrediente *simples[4] = {
        &nova.e.queijo_suico, &nova.e.cheddar, &nova.e.cebola, &nova.e.tomate
    };
    for (int i = 0; i < 4; i++)
        if (!(arq >> simples[i]->comida >> simples[i]->turno_validade
                  >> simples[i]->qnt)) return false;

    const char codigos[3] = {'n', 'o', 'b'};
    for (int i = 0; i < 3; i++) {
        char comida;
        int validade, quantidade;
        if (!(arq >> comida >> validade >> quantidade) ||
            quantidade < 0 || quantidade > 1000) return false;
        inicializarHeap(nova.e.pereciveis[i], codigos[i], 1000, validade);
        if (nova.e.pereciveis[i].vetor == nullptr) return false;
        if (comida != codigos[i]) return false;
        nova.e.pereciveis[i].tamanho = quantidade;
        for (int j = 0; j < quantidade; j++)
            if (!(arq >> nova.e.pereciveis[i].vetor[j].comida
                      >> nova.e.pereciveis[i].vetor[j].turno_validade
                      >> nova.e.pereciveis[i].vetor[j].qnt)) return false;
        buildHeap(nova.e.pereciveis[i]);
    }

    int buscas, comparacoes, hits, misses, colisoes, entradas;
    double tempo;
    if (!(arq >> buscas >> comparacoes >> tempo >> hits >> misses
              >> colisoes >> entradas)) return false;

    encInicializar(nova.cache, 101);
    const NoFila *c = nova.filaClientes.frente;
    while (c != nullptr) {
        encInserirReceita(nova.cache, c->dado.cod, c->dado.nome);
        c = c->prox;
    }
    totalBuscas = buscas;
    totalComparacoesBusca = comparacoes;
    tempoTotalBuscas = tempo;
    totalHits = hits;
    totalMisses = misses;
    totalColisoes = colisoes;
    totalEntradasCache = entradas;

    p = nova;
    return true;
}
