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

static void desenharPartida(const Partida& p) {
    BeginDrawing();
    ClearBackground(FUNDO);
    desenharTopo(p);
    desenharHamburguer(p);
    desenharPedido(p);
    desenharEstoque(p.e);
    desenharControles();
    DrawRectangleRounded((Rectangle){ 55, 625, 1160, 48 }, 0.12f, 8, CREME);
    DrawText(mensagemAtual, 75, 640, 18, MARROM);
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
            DrawText("COMO JOGAR", 90, 400, 32, MARROM);
            DrawText("I preparar | A adicionar | R remover | E estoque | P pedido | S sair", 90, 455, 19, MARROM);
            if (botao("VOLTAR", (Rectangle){ 110, 570, 250, 55 }, MARROM)) tela = 0;
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
        desenharPartida(partida);
        if (IsKeyPressed(KEY_I)) return 'I';
        if (IsKeyPressed(KEY_A)) return 'A';
        if (IsKeyPressed(KEY_R)) return 'R';
        if (IsKeyPressed(KEY_E)) return 'E';
        if (IsKeyPressed(KEY_P)) return 'P';
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_ESCAPE)) return 'S';
        BeginDrawing();
        if (botao("PREPARAR", (Rectangle){ 55, 680, 170, 34 }, LARANJA)) { EndDrawing(); return 'I'; }
        if (botao("ADICIONAR", (Rectangle){ 235, 680, 170, 34 }, VERDE)) { EndDrawing(); return 'A'; }
        if (botao("REMOVER", (Rectangle){ 415, 680, 170, 34 }, MARROM)) { EndDrawing(); return 'R'; }
        if (botao("ESTOQUE", (Rectangle){ 595, 680, 170, 34 }, MARROM_CLARO)) { EndDrawing(); return 'E'; }
        if (botao("PEDIDO", (Rectangle){ 775, 680, 170, 34 }, MARROM_CLARO)) { EndDrawing(); return 'P'; }
        if (botao("SAIR", (Rectangle){ 955, 680, 170, 34 }, VERMELHO)) { EndDrawing(); return 'S'; }
        EndDrawing();
        PollInputEvents();
    }
    return 'S';
}

char interfaceLerIngrediente(const Partida& partida, bool preparar) {
    const char codigos[12] = {'k','m','p','a','z','s','q','c','t','n','o','b'};
    while (janelaAberta && !WindowShouldClose()) {
        desenharPartida(partida);
        BeginDrawing();
        DrawRectangleRounded((Rectangle){ 250, 115, 780, 480 }, 0.08f, 10, CREME);
        DrawText(preparar ? "ESCOLHA O INGREDIENTE PARA PREPARAR" : "ESCOLHA O INGREDIENTE PARA ADICIONAR",
                 285, 145, 21, MARROM);
        for (int i = 0; i < 12; i++) {
            int coluna = i % 3;
            int linha = i / 3;
            Rectangle area = { 285.0f + coluna * 235.0f, 205.0f + linha * 78.0f, 210, 55 };
            if (botao(TextFormat("%c - %s", (char)(codigos[i] - 32), nomeIngrediente(codigos[i])),
                      area, (Color){ 112, 70, 40, 255 }, 17))
                return codigos[i];
        }
        if (IsKeyPressed(KEY_ESCAPE)) return ' ';
        EndDrawing();
        PollInputEvents();
    }
    return ' ';
}

void interfaceMensagem(const char *mensagem) {
    mensagemAtual = mensagem;
}

void interfaceMostrarEstoque(const Estoque& estoque) {
    (void)estoque;
    mensagemAtual = "Estoque atualizado no painel lateral.";
}

void interfaceMostrarPedido(const Pedido& pedido) {
    (void)pedido;
    mensagemAtual = "Pedido atual exibido no painel superior.";
}
