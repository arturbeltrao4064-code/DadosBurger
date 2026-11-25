#include "interface.hpp"
#include "raylib.h"

static const Color FUNDO = { 247, 235, 211, 255 };
static const Color MARROM = { 70, 40, 20, 255 };
static const Color MARROM_CLARO = { 112, 70, 40, 255 };
static const Color LARANJA = { 202, 112, 50, 255 };
static const Color CREME = { 255, 247, 229, 255 };
static const Color VERDE = { 70, 137, 80, 255 };
static const Color VERMELHO = { 174, 65, 48, 255 };

static bool janelaAberta = false;
static const char *mensagemAtual = "";

static bool botao(const char *texto, Rectangle area, Color cor, int fonte = 22) {
    Vector2 mouse = GetMousePosition();
    bool sobre = CheckCollisionPointRec(mouse, area);
    DrawRectangleRounded(area, 0.18f, 10, sobre ? MARROM_CLARO : cor);
    DrawRectangleRoundedLinesEx(area, 0.18f, 10, 2, MARROM);
    int largura = MeasureText(texto, fonte);
    DrawText(texto, (int)(area.x + (area.width - largura) / 2),
             (int)(area.y + (area.height - fonte) / 2), fonte, CREME);
    return sobre && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static const char *nomeIngrediente(char codigo) {
    if (codigo == 'k') return "Ketchup";
    if (codigo == 'm') return "Mostarda";
    if (codigo == 'p') return "Picles";
    if (codigo == 'a') return "Alface";
    if (codigo == 'z') return "Pao superior";
    if (codigo == 's') return "Queijo suico";
    if (codigo == 'q') return "Cheddar";
    if (codigo == 'c') return "Cebola";
    if (codigo == 't') return "Tomate";
    if (codigo == 'n') return "Carne";
    if (codigo == 'o') return "Ovo";
    if (codigo == 'b') return "Bacon";
    return "?";
}

static void desenharTopo(const Partida& p) {
    DrawRectangle(0, 0, 1280, 76, MARROM);
    DrawText("DADO BURGER", 34, 18, 34, CREME);
    DrawText(TextFormat("TURNO %d", p.turnoAtual), 720, 24, 22, CREME);
    DrawText(TextFormat("PONTOS %d", p.pontuacao), 900, 24, 22, CREME);
    DrawText(TextFormat("PACIENCIA %d", p.pacienciaAtual), 1060, 24, 22,
             p.pacienciaAtual <= 5 ? VERMELHO : CREME);
}

static void desenharHamburguer(const Partida& p) {
    DrawText("SEU HAMBURGUER", 55, 135, 25, MARROM);
    DrawRectangleRounded((Rectangle){ 55, 175, 485, 390 }, 0.04f, 8, CREME);
    DrawEllipse(295, 520, 175, 18, Fade(MARROM, 0.18f));

    int y = 475;
    DrawRectangleRounded((Rectangle){ 125, (float)y, 340, 38 }, 0.45f, 12,
                         (Color){ 231, 169, 70, 255 });
    y -= 42;

    const NoPilha *parte = p.hamburguer.topo;
    while (parte != nullptr && y > 205) {
        Color cor = (Color){ 183, 80, 40, 255 };
        if (parte->dado.comida == 'a') cor = VERDE;
        else if (parte->dado.comida == 's' || parte->dado.comida == 'q') cor = (Color){ 245, 195, 70, 255 };
        else if (parte->dado.comida == 'n' || parte->dado.comida == 'b') cor = (Color){ 112, 55, 35, 255 };
        else if (parte->dado.comida == 't') cor = VERMELHO;
        else if (parte->dado.comida == 'z') cor = (Color){ 231, 169, 70, 255 };
        DrawRectangleRounded((Rectangle){ 135, (float)y, 320, 25 }, 0.3f, 8, cor);
        DrawText(nomeIngrediente(parte->dado.comida), 470, y + 3, 15, MARROM);
        y -= 30;
        parte = parte->prox;
    }

    DrawText("BASE", 250, 530, 14, MARROM_CLARO);
    DrawText("Ingredientes adicionados aparecem acima da base.", 75, 580, 15, MARROM_CLARO);
}

static void desenharPedido(const Partida& p) {
    DrawText("PEDIDO ATUAL", 590, 135, 25, MARROM);
    DrawRectangleRounded((Rectangle){ 590, 175, 625, 120 }, 0.06f, 8, CREME);
    if (!estaVazia(p.filaClientes)) {
        const Pedido& pedido = p.filaClientes.frente->dado;
        DrawText(pedido.nome.c_str(), 620, 200, 30, MARROM);
        DrawText(TextFormat("Codigo: %d", pedido.cod), 620, 245, 18, MARROM_CLARO);
    } else {
        DrawText("Todos os clientes atendidos!", 620, 215, 25, VERDE);
    }
}

static void desenharEstoque(const Estoque& e) {
    DrawText("ESTOQUE", 590, 325, 25, MARROM);
    DrawRectangleRounded((Rectangle){ 590, 365, 625, 105 }, 0.06f, 8, CREME);
    DrawText(TextFormat("Suico: %d   Cheddar: %d   Cebola: %d   Tomate: %d",
        e.queijo_suico.qnt, e.cheddar.qnt, e.cebola.qnt, e.tomate.qnt),
        610, 385, 18, MARROM);
    DrawText(TextFormat("Carne: %d   Ovo: %d   Bacon: %d",
        e.pereciveis[0].tamanho, e.pereciveis[1].tamanho, e.pereciveis[2].tamanho),
        610, 420, 18, MARROM);
    DrawText("Ketchup, mostarda, picles, alface e pao: ilimitados",
        610, 448, 15, MARROM_CLARO);
}

static void desenharControles() {
    DrawText("ACOES", 590, 495, 23, MARROM);
    DrawText("I Preparar    A Adicionar    R Remover    E Estoque    P Pedido    S Sair",
        590, 528, 17, MARROM);
    DrawText("Use os botoes abaixo ou as teclas correspondentes.",
        590, 558, 15, MARROM_CLARO);
}

static void desenharPartidaConteudo(const Partida& p) {
    ClearBackground(FUNDO);
    desenharTopo(p);
    desenharHamburguer(p);
    desenharPedido(p);
    desenharEstoque(p.e);
    desenharControles();
    DrawRectangleRounded((Rectangle){ 55, 625, 1160, 48 }, 0.12f, 8, CREME);
    DrawText(mensagemAtual, 75, 640, 18, MARROM);
}

static void desenharPartida(const Partida& p) {
    BeginDrawing();
    desenharPartidaConteudo(p);
    EndDrawing();
}

static void abrirSeNecessario() {
    if (!janelaAberta) {
        InitWindow(1280, 720, "Dado Burger");
        SetTargetFPS(60);
        janelaAberta = true;
    }
}

int iniciarInterface() {
    abrirSeNecessario();
    int tela = 0;
    int opcao = 0;
    bool executando = true;

    while (!WindowShouldClose() && executando) {
        BeginDrawing();
        ClearBackground(FUNDO);
        DrawRectangle(0, 0, 1280, 18, MARROM);
        DrawText("DADO", 105, 115, 76, MARROM);
        DrawText("BURGER", 105, 188, 76, LARANJA);
        DrawText("SUA COZINHA, SUAS REGRAS!", 110, 280, 22, MARROM_CLARO);
        DrawText("Prepare, monte e atenda seus clientes.", 110, 315, 19, MARROM_CLARO);
        DrawEllipse(900, 270, 105, 16, Fade(MARROM, 0.18f));
        DrawEllipse(900, 205, 101, 31, (Color){ 235, 176, 80, 255 });
        DrawRectangleRounded((Rectangle){ 800, 210, 200, 28 }, 0.3f, 10, VERDE);
        DrawRectangleRounded((Rectangle){ 800, 235, 200, 30 }, 0.3f, 10, (Color){ 183, 80, 40, 255 });
        DrawRectangleRounded((Rectangle){ 800, 265, 200, 30 }, 0.3f, 10, (Color){ 231, 169, 70, 255 });

        if (tela == 0) {
            if (botao("NOVO JOGO", (Rectangle){ 110, 405, 330, 58 }, LARANJA)) { opcao = 1; executando = false; }
            if (botao("COMO JOGAR", (Rectangle){ 110, 478, 330, 58 }, MARROM)) tela = 1;
            if (botao("HISTORICO", (Rectangle){ 110, 551, 330, 58 }, MARROM)) { opcao = 2; executando = false; }
            if (botao("SAIR", (Rectangle){ 110, 624, 330, 48 }, MARROM_CLARO)) { opcao = 0; executando = false; }
        } else {
            DrawText("COMO JOGAR", 90, 390, 34, MARROM);
            DrawText("OBJETIVO: atender os 10 clientes antes que a paciencia acabe.", 90, 440, 20, MARROM);
            DrawText("I - Preparar ingrediente (1 turno)     A - Adicionar ingrediente (1 turno)", 90, 475, 18, MARROM);
            DrawText("R - Remover do topo (1 turno)           E - Consultar estoque (gratuito)", 90, 505, 18, MARROM);
            DrawText("P - Consultar pedido (gratuito)         S - Encerrar partida", 90, 535, 18, MARROM);
            DrawText("Selecione ingredientes na tela seguinte. Prepare antes de adicionar", 90, 565, 17, MARROM_CLARO);
            DrawText("carne, ovo, bacon, queijos, cebola e tomate. O pao superior fecha o lanche.", 90, 590, 17, MARROM_CLARO);
            if (botao("VOLTAR", (Rectangle){ 110, 630, 250, 48 }, MARROM)) tela = 0;
        }
        EndDrawing();
    }
    if (WindowShouldClose()) opcao = 0;
    if (opcao != 1 && opcao != 2) {
        CloseWindow();
        janelaAberta = false;
    }
    return opcao;
}

void interfaceAbrirJogo() {
    abrirSeNecessario();
    mensagemAtual = "Escolha uma acao para atender o pedido.";
}

void interfaceFecharJogo() {
    if (janelaAberta) CloseWindow();
    janelaAberta = false;
}

void interfaceAtualizar(const Partida& partida) {
    if (janelaAberta && !WindowShouldClose()) desenharPartida(partida);
}

char interfaceLerAcao(const Partida& partida) {
    while (janelaAberta && !WindowShouldClose()) {
        BeginDrawing();
        desenharPartidaConteudo(partida);
        if (IsKeyPressed(KEY_I)) { EndDrawing(); return 'I'; }
        if (IsKeyPressed(KEY_A)) { EndDrawing(); return 'A'; }
        if (IsKeyPressed(KEY_R)) { EndDrawing(); return 'R'; }
        if (IsKeyPressed(KEY_E)) { EndDrawing(); return 'E'; }
        if (IsKeyPressed(KEY_P)) { EndDrawing(); return 'P'; }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_ESCAPE)) { EndDrawing(); return 'S'; }
        char acao = '\0';
        if (botao("PREPARAR", (Rectangle){ 55, 680, 170, 34 }, LARANJA)) acao = 'I';
        else if (botao("ADICIONAR", (Rectangle){ 235, 680, 170, 34 }, VERDE)) acao = 'A';
        else if (botao("REMOVER", (Rectangle){ 415, 680, 170, 34 }, MARROM)) acao = 'R';
        else if (botao("ESTOQUE", (Rectangle){ 595, 680, 170, 34 }, MARROM_CLARO)) acao = 'E';
        else if (botao("PEDIDO", (Rectangle){ 775, 680, 170, 34 }, MARROM_CLARO)) acao = 'P';
        else if (botao("SAIR", (Rectangle){ 955, 680, 170, 34 }, VERMELHO)) acao = 'S';
        EndDrawing();
        if (acao != '\0') return acao;
    }
    return 'S';
}

char interfaceLerIngrediente(const Partida& partida, bool preparar) {
    const char codigos[12] = {'k','m','p','a','z','s','q','c','t','n','o','b'};
    while (janelaAberta && !WindowShouldClose()) {
        BeginDrawing();
        desenharPartidaConteudo(partida);
        DrawRectangleRounded((Rectangle){ 250, 115, 780, 480 }, 0.08f, 10, CREME);
        DrawText(preparar ? "ESCOLHA O INGREDIENTE PARA PREPARAR" : "ESCOLHA O INGREDIENTE PARA ADICIONAR",
                 285, 145, 21, MARROM);
        char escolhido = '\0';
        for (int i = 0; i < 12; i++) {
            int coluna = i % 3;
            int linha = i / 3;
            Rectangle area = { 285.0f + coluna * 235.0f, 205.0f + linha * 78.0f, 210, 55 };
            bool permitido = !preparar || codigos[i] == 's' || codigos[i] == 'q' ||
                             codigos[i] == 'c' || codigos[i] == 't' ||
                             codigos[i] == 'n' || codigos[i] == 'o' || codigos[i] == 'b';
            if (permitido && botao(TextFormat("%c - %s", (char)(codigos[i] - 32), nomeIngrediente(codigos[i])),
                                   area, (Color){ 112, 70, 40, 255 }, 17)) {
                escolhido = codigos[i];
                break;
            }
        }
        bool cancelar = IsKeyPressed(KEY_ESCAPE);
        EndDrawing();
        if (escolhido != '\0') return escolhido;
        if (cancelar) return '\0';
    }
    return '\0';
}

void interfaceMensagem(const char *mensagem) {
    mensagemAtual = mensagem;
}

void interfaceMostrarEstoque(const Estoque& estoque) {
    while (janelaAberta && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(FUNDO);
        DrawText("ESTOQUE DA COZINHA", 390, 90, 38, MARROM);
        DrawRectangleRounded((Rectangle){ 250, 165, 780, 350 }, 0.05f, 8, CREME);
        DrawText("INGREDIENTES PREPARADOS", 300, 195, 24, LARANJA);
        DrawText(TextFormat("Queijo suico: %d", estoque.queijo_suico.qnt), 310, 245, 22, MARROM);
        DrawText(TextFormat("Cheddar: %d", estoque.cheddar.qnt), 680, 245, 22, MARROM);
        DrawText(TextFormat("Cebola: %d", estoque.cebola.qnt), 310, 295, 22, MARROM);
        DrawText(TextFormat("Tomate: %d", estoque.tomate.qnt), 680, 295, 22, MARROM);
        DrawText("INGREDIENTES PERECIVEIS", 300, 355, 24, LARANJA);
        DrawText(TextFormat("Carne: %d", estoque.pereciveis[0].tamanho), 310, 405, 22, MARROM);
        DrawText(TextFormat("Ovo: %d", estoque.pereciveis[1].tamanho), 550, 405, 22, MARROM);
        DrawText(TextFormat("Bacon: %d", estoque.pereciveis[2].tamanho), 790, 405, 22, MARROM);
        DrawText("Ketchup, mostarda, picles, alface e pao: ilimitados", 310, 465, 18, MARROM_CLARO);
        bool voltar = botao("VOLTAR A PARTIDA", (Rectangle){ 455, 565, 370, 60 }, VERDE);
        bool cancelar = IsKeyPressed(KEY_ESCAPE);
        EndDrawing();
        if (voltar || cancelar) break;
    }
}

void interfaceMostrarPedido(const Pedido& pedido) {
    while (janelaAberta && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(FUNDO);
        DrawText("PEDIDO DO CLIENTE", 390, 90, 38, MARROM);
        DrawRectangleRounded((Rectangle){ 250, 165, 780, 350 }, 0.05f, 8, CREME);
        DrawText(pedido.nome.c_str(), 300, 205, 30, LARANJA);
        DrawText(TextFormat("Codigo: %d", pedido.cod), 300, 255, 21, MARROM_CLARO);
        DrawText("INGREDIENTES DA RECEITA", 300, 315, 23, MARROM);
        int x = 310;
        int y = 365;
        NoReceita *item = pedido.receita.ini;
        while (item != nullptr) {
            DrawText(TextFormat("%c", (char)(item->ingrediente - 'a' + 'A')), x, y, 23, MARROM);
            x += 42;
            if (x > 900) {
                x = 310;
                y += 38;
            }
            item = item->prox;
        }
        DrawText("Monte os ingredientes na ordem apresentada.", 300, 465, 18, MARROM_CLARO);
        bool voltar = botao("VOLTAR A PARTIDA", (Rectangle){ 455, 565, 370, 60 }, VERDE);
        bool cancelar = IsKeyPressed(KEY_ESCAPE);
        EndDrawing();
        if (voltar || cancelar) break;
    }
}

std::string interfaceLerNome() {
    char nome[21] = {};
    int quantidade = 0;

    while (janelaAberta && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(FUNDO);
        DrawText("PARTIDA FINALIZADA", 360, 170, 42, MARROM);
        DrawText("Digite seu nome para salvar a pontuacao:", 330, 270, 24, MARROM_CLARO);
        DrawRectangleRounded((Rectangle){ 330, 330, 620, 65 }, 0.12f, 8, CREME);
        DrawText(nome, 355, 347, 27, MARROM);
        DrawText("Ate 20 caracteres. ENTER para confirmar.", 355, 415, 18, MARROM_CLARO);
        EndDrawing();

        int tecla = GetCharPressed();
        while (tecla > 0) {
            if (tecla >= 32 && tecla <= 125 && quantidade < 20) {
                nome[quantidade] = (char)tecla;
                quantidade++;
                nome[quantidade] = '\0';
            }
            tecla = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && quantidade > 0) {
            quantidade--;
            nome[quantidade] = '\0';
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (quantidade == 0) return "Jogador";
            return std::string(nome);
        }
    }
    return "Jogador";
}

void interfaceMostrarResultado(const std::string& nome, int pontuacao) {
    while (janelaAberta && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(FUNDO);
        DrawText("RESULTADO DA PARTIDA", 330, 170, 42, MARROM);
        DrawText(nome.c_str(), 430, 275, 36, LARANJA);
        DrawText(TextFormat("%d clientes atendidos", pontuacao), 420, 345, 28, MARROM);
        if (botao("CONTINUAR", (Rectangle){ 480, 475, 320, 65 }, VERDE)) {
            EndDrawing();
            break;
        }
        EndDrawing();
    }
}

int interfaceEscolherCriterioHistorico() {
    while (janelaAberta && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(FUNDO);
        DrawText("HISTORICO DE PARTIDAS", 330, 150, 40, MARROM);
        DrawText("Escolha como deseja ordenar os resultados:", 350, 235, 22, MARROM_CLARO);
        int criterio = 0;
        if (botao("ORDEM ALFABETICA", (Rectangle){ 350, 315, 580, 65 }, LARANJA))
            criterio = 1;
        if (botao("PONTUACAO DECRESCENTE", (Rectangle){ 350, 405, 580, 65 }, MARROM))
            criterio = 2;
        bool voltar = botao("VOLTAR", (Rectangle){ 500, 510, 280, 55 }, MARROM_CLARO);
        EndDrawing();
        if (voltar) return 0;
        if (criterio != 0) return criterio;
        if (WindowShouldClose()) return 0;
    }
    return 0;
}

void interfaceMostrarHistorico(const PARTIDA vetor[], int quantidade, double tempo) {
    int inicio = 0;
    while (janelaAberta && !WindowShouldClose()) {
        float rolagem = GetMouseWheelMove();
        if (rolagem > 0 && inicio > 0) inicio--;
        if (rolagem < 0 && inicio + 8 < quantidade) inicio++;

        BeginDrawing();
        ClearBackground(FUNDO);
        DrawText("HISTORICO DE PARTIDAS", 55, 45, 38, MARROM);
        DrawText(TextFormat("%d registros", quantidade), 60, 100, 18, MARROM_CLARO);
        DrawText(TextFormat("Tempo do Quick Sort: %.8f s", tempo), 800, 100, 18, MARROM);
        DrawRectangleRounded((Rectangle){ 55, 145, 1160, 475 }, 0.04f, 8, CREME);
        int fim = inicio + 8;
        if (fim > quantidade) fim = quantidade;
        for (int i = inicio; i < fim; i++) {
            int y = 170 + (i - inicio) * 52;
            DrawText(TextFormat("%d.", i + 1), 85, y, 20, LARANJA);
            DrawText(vetor[i].nome_jogador.c_str(), 145, y, 21, MARROM);
            DrawText(TextFormat("%d pontos", vetor[i].pontuacao), 900, y, 21, MARROM);
            DrawLine(80, y + 34, 1185, y + 34, (Color){ 225, 210, 190, 255 });
        }
        DrawText("Use a roda do mouse para percorrer a lista.", 65, 635, 16, MARROM_CLARO);
        bool voltar = botao("VOLTAR", (Rectangle){ 1000, 640, 190, 48 }, MARROM);
        EndDrawing();
        if (voltar) break;
    }
}

void interfaceAviso(const char *mensagem) {
    while (janelaAberta && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(FUNDO);
        DrawText("DADO BURGER", 430, 180, 42, MARROM);
        DrawText(mensagem, 170, 300, 22, MARROM_CLARO);
        bool voltar = botao("VOLTAR", (Rectangle){ 500, 430, 280, 60 }, MARROM);
        EndDrawing();
        if (voltar) break;
    }
}

void interfaceMostrarRelatorio(int nos, int altura, int rotacoes, int historico,
                               int buscas, int comparacoes, double media,
                               double tempoBuscas, int entradas, int hits,
                               int misses, double taxa, double fator, int colisoes) {
    while (janelaAberta && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(FUNDO);
        DrawText("RELATORIO DE DESEMPENHO", 250, 45, 38, MARROM);
        DrawRectangleRounded((Rectangle){ 55, 115, 560, 485 }, 0.05f, 8, CREME);
        DrawRectangleRounded((Rectangle){ 640, 115, 575, 485 }, 0.05f, 8, CREME);
        DrawText("ARVORE AVL", 85, 140, 25, LARANJA);
        DrawText(TextFormat("Nos inseridos: %d", nos), 85, 195, 19, MARROM);
        DrawText(TextFormat("Altura: %d", altura), 85, 235, 19, MARROM);
        DrawText(TextFormat("Rotacoes: %d", rotacoes), 85, 275, 19, MARROM);
        DrawText(TextFormat("Historico carregado: %d", historico), 85, 315, 19, MARROM);
        DrawText(TextFormat("Buscas: %d", buscas), 85, 355, 19, MARROM);
        DrawText(TextFormat("Comparacoes: %d", comparacoes), 85, 395, 19, MARROM);
        DrawText(TextFormat("Media por busca: %.4f", media), 85, 435, 19, MARROM);
        DrawText(TextFormat("Tempo de buscas: %.8f s", tempoBuscas), 85, 475, 19, MARROM);
        DrawText("TABELA HASH", 670, 140, 25, LARANJA);
        DrawText("Tamanho da tabela: 101", 670, 195, 19, MARROM);
        DrawText(TextFormat("Entradas ocupadas: %d", entradas), 670, 235, 19, MARROM);
        DrawText(TextFormat("Hits: %d", hits), 670, 275, 19, MARROM);
        DrawText(TextFormat("Misses: %d", misses), 670, 315, 19, MARROM);
        DrawText(TextFormat("Taxa de acerto: %.2f%%", taxa), 670, 355, 19, MARROM);
        DrawText(TextFormat("Fator de carga: %.4f", fator), 670, 395, 19, MARROM);
        DrawText(TextFormat("Colisoes: %d", colisoes), 670, 435, 19, MARROM);
        bool continuar = botao("VOLTAR AO MENU", (Rectangle){ 470, 630, 340, 55 }, VERDE);
        EndDrawing();
        if (continuar) break;
    }
}
