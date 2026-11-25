#include "interface.hpp"
#include "raylib.h"

static const Color FUNDO = { 247, 235, 211, 255 };
static const Color MARROM = {  70,  40,  20, 255 };
static const Color MARROM_CLARO = { 112,  70,  40, 255 };
static const Color LARANJA = { 202, 112,  50, 255 };
static const Color CREME = { 255, 247, 229, 255 };

static bool desenharBotao(const char *texto, Rectangle area, Color cor) {
    Vector2 mouse = GetMousePosition();
    bool sobre = CheckCollisionPointRec(mouse, area);

    DrawRectangleRounded(area, 0.22f, 12, sobre ? MARROM_CLARO : cor);
    DrawRectangleRoundedLinesEx(area, 0.22f, 12, 2.0f, sobre ? CREME : MARROM);

    int largura = MeasureText(texto, 24);
    DrawText(texto, (int)(area.x + (area.width - largura) / 2),
             (int)(area.y + 16), 24, CREME);

    return sobre && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void desenharHamburguer(int x, int y) {
    DrawEllipse(x, y + 68, 105, 16, Fade(MARROM, 0.18f));
    DrawRectangleRounded((Rectangle){ (float)x - 100, (float)y + 28, 200, 32 },
                         0.35f, 12, (Color){ 111,  60,  30, 255 });
    DrawRectangleRounded((Rectangle){ (float)x - 94, (float)y + 10, 188, 24 },
                         0.35f, 12, (Color){ 231, 169,  70, 255 });
    DrawRectangleRounded((Rectangle){ (float)x - 96, (float)y - 5, 192, 22 },
                         0.35f, 12, (Color){  70, 137,  80, 255 });
    DrawRectangleRounded((Rectangle){ (float)x - 100, (float)y - 22, 200, 26 },
                         0.35f, 12, (Color){ 183,  80,  40, 255 });
    DrawEllipse(x, y - 26, 101, 31, (Color){ 235, 176,  80, 255 });
    DrawEllipse(x - 38, y - 42, 4, 2, CREME);
    DrawEllipse(x - 8, y - 48, 4, 2, CREME);
    DrawEllipse(x + 25, y - 40, 4, 2, CREME);
    DrawEllipse(x + 52, y - 30, 4, 2, CREME);
}

void iniciarInterface() {
    InitWindow(1280, 720, "Dado Burger");
    SetTargetFPS(60);

    int tela = 0;
    bool executando = true;

    while (!WindowShouldClose() && executando) {
        BeginDrawing();
        ClearBackground(FUNDO);

        DrawRectangle(0, 0, 1280, 18, MARROM);
        DrawRectangle(0, 702, 1280, 18, MARROM);

        if (tela == 0) {
            DrawText("DADO", 105, 115, 76, MARROM);
            DrawText("BURGER", 105, 188, 76, LARANJA);
            DrawText("SUA COZINHA, SUAS REGRAS!", 110, 280, 22, MARROM_CLARO);
            DrawText("Prepare, monte e atenda seus clientes.", 110, 315, 19, MARROM_CLARO);

            desenharHamburguer(900, 205);

            if (desenharBotao("NOVO JOGO", (Rectangle){ 110, 405, 330, 58 }, LARANJA))
                tela = 3;
            if (desenharBotao("COMO JOGAR", (Rectangle){ 110, 478, 330, 58 }, MARROM))
                tela = 1;
            if (desenharBotao("HISTORICO", (Rectangle){ 110, 551, 330, 58 }, MARROM))
                tela = 2;
            if (desenharBotao("SAIR", (Rectangle){ 110, 624, 330, 48 }, MARROM_CLARO))
                executando = false;

            DrawText("DADO BURGER", 930, 640, 18, MARROM_CLARO);
        }
        else if (tela == 1) {
            DrawText("COMO JOGAR", 70, 42, 42, MARROM);

            DrawText("OBJETIVO", 75, 112, 22, LARANJA);
            DrawText("Atenda os 10 clientes da fila antes que algum perca a paciencia.", 75, 143, 18, MARROM);
            DrawText("Cada cliente comeca com 15 turnos de paciencia. Se chegar a zero, voce perde.", 75, 170, 18, MARROM);
            DrawText("VITORIA: atender todos os 10 clientes. PONTUACAO: clientes atendidos.", 75, 197, 18, MARROM);

            DrawText("INGREDIENTES E SIMBOLOS", 75, 245, 22, LARANJA);
            DrawText("k - ketchup       m - mostarda       p - picles       a - alface", 75, 278, 18, MARROM);
            DrawText("z - pao de cima   s - queijo suico   q - cheddar     c - cebola", 75, 307, 18, MARROM);
            DrawText("t - tomate        n - carne          o - ovo frito    b - bacon", 75, 336, 18, MARROM);

            DrawText("ACOES E TURNOS", 75, 385, 22, LARANJA);
            DrawText("I - Preparar ingrediente: consome 1 turno.", 75, 418, 18, MARROM);
            DrawText("A - Adicionar ingrediente ao hamburguer: consome 1 turno.", 75, 445, 18, MARROM);
            DrawText("R - Remover ingrediente do topo: consome 1 turno.", 75, 472, 18, MARROM);
            DrawText("E - Consultar estoque; P - consultar pedido: nao consomem turno.", 75, 499, 18, MARROM);

            DrawText("REGRAS DE MONTAGEM", 75, 545, 22, LARANJA);
            DrawText("O pao de baixo ja esta no prato. O pao de cima fecha o hamburguer.", 75, 576, 17, MARROM);
            DrawText("Prepare antes os ingredientes que exigem preparo. Carne, ovo e bacon vencem", 75, 601, 17, MARROM);
            DrawText("apos 3 turnos. Nao e possivel adicionar ingredientes depois do pao de cima.", 75, 625, 17, MARROM);

            if (desenharBotao("VOLTAR", (Rectangle){ 1030, 650, 170, 42 }, MARROM))
                tela = 0;
        }
        else if (tela == 2) {
            DrawText("HISTORICO", 90, 65, 42, MARROM);
            DrawText("O projeto ja carrega e salva o historico em historico.txt.", 95, 160, 21, MARROM);
            DrawText("A visualizacao grafica e os controles de ordenacao ainda serao integrados", 95, 205, 18, MARROM_CLARO);
            DrawText("ao Quick Sort existente no programa principal.", 95, 233, 18, MARROM_CLARO);
            DrawText("As opcoes exigidas sao ordem alfabetica e pontuacao decrescente.", 95, 280, 18, MARROM);
            if (desenharBotao("VOLTAR", (Rectangle){ 95, 625, 210, 48 }, MARROM))
                tela = 0;
        }
        else if (tela == 3) {
            DrawText("NOVA PARTIDA", 90, 65, 42, MARROM);
            DrawText("A integracao da partida com os controles graficos ainda sera feita.", 95, 160, 21, MARROM);
            DrawText("O fluxo atual de jogo e as operacoes avaliadas permanecem no programa", 95, 200, 19, MARROM_CLARO);
            DrawText("principal. Esta tela ainda nao inicia uma partida.", 95, 230, 19, MARROM_CLARO);
            if (desenharBotao("VOLTAR", (Rectangle){ 95, 625, 210, 48 }, MARROM))
                tela = 0;
        }

        EndDrawing();
    }

    CloseWindow();
}
