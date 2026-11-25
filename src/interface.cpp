#include "interface.hpp"
#include "raylib.h"

void iniciarInterface() {
    InitWindow(1280, 720, "Dado Burger");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("DADO BURGER", 450, 280, 50, DARKBROWN);
        DrawText("Nossa cozinha esta aberta!", 465, 350, 25, GRAY);

        EndDrawing();
    }

    CloseWindow();
}