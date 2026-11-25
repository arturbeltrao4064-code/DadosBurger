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
    DrawRectangleRoundedLines(area, 0.22f, 12, 2.0f, sobre ? CREME : MARROM);

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
            DrawText("COMO JOGAR", 90, 65, 42, MARROM);
            DrawText("OBJETIVO", 95, 145, 23, LARANJA);
            DrawText("Atenda os 10 clientes antes que a paciencia chegue a zero.", 95, 180, 20, MARROM);
            DrawText("ACOES", 95, 235, 23, LARANJA);
            DrawText("I - Preparar ingrediente (consome 1 turno)", 95, 270, 19, MARROM);
            DrawText("A - Adicionar ingrediente ao hamburguer (consome 1 turno)", 95, 302, 19, MARROM);
            DrawText("R - Remover ingrediente do topo (consome 1 turno)", 95, 334, 19, MARROM);
            DrawText("E - Consultar estoque (nao consome turno)", 95, 366, 19, MARROM);
            DrawText("P - Consultar pedido (nao consome turno)", 95, 398, 19, MARROM);
            DrawText("S - Encerrar a partida", 95, 430, 19, MARROM);
            DrawText("REGRAS IMPORTANTES", 95, 480, 23, LARANJA);
            DrawText("Prepare os ingredientes que exigem preparo antes de adiciona-los.", 95, 515, 18, MARROM);
            DrawText("O pao de cima fecha o hamburguer. Carne, ovo e bacon vencem em 3 turnos.", 95, 545, 18, MARROM);
            DrawText("Cada cliente comeca com 15 turnos de paciencia.", 95, 575, 18, MARROM);
            if (desenharBotao("VOLTAR", (Rectangle){ 95, 625, 210, 48 }, MARROM))
                tela = 0;
        }
        else if (tela == 2) {
            DrawText("HISTORICO", 90, 65, 42, MARROM);
            DrawText("As partidas salvas aparecerao nesta tela.", 95, 160, 22, MARROM);
            DrawText("A consulta e a ordenacao serao conectadas ao historico existente", 95, 198, 18, MARROM_CLARO);
            DrawText("na proxima etapa, preservando o Quick Sort do projeto.", 95, 226, 18, MARROM_CLARO);
            if (desenharBotao("VOLTAR", (Rectangle){ 95, 625, 210, 48 }, MARROM))
                tela = 0;
        }
        else if (tela == 3) {
            DrawText("NOVA PARTIDA", 90, 65, 42, MARROM);
            DrawText("A janela e o menu grafico estao funcionando.", 95, 160, 22, MARROM);
            DrawText("A partida ainda usa o fluxo atual do jogo no terminal.", 95, 200, 19, MARROM_CLARO);
            DrawText("A integracao das acoes com os controles graficos sera feita", 95, 240, 19, MARROM_CLARO);
            DrawText("sem reimplementar as estruturas ou os algoritmos avaliados.", 95, 270, 19, MARROM_CLARO);
            if (desenharBotao("VOLTAR", (Rectangle){ 95, 625, 210, 48 }, MARROM))
                tela = 0;
        }

        EndDrawing();
    }

    CloseWindow();
}
