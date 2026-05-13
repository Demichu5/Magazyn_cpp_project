#include "Raylib/include/raylib.h"
#include "Settings/settings.h"
#include "GuiHandler/renderHandler.h"

int main() {
    InitWindow(G_SCREEN_WIDTH, G_SCREEN_HEIGHT, "System Magazynowy");
    wczytajMagazyn();
    SetTargetFPS(30);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GetColor(C_COLOR_GRAY1));
        drawMainWindow();
        EndDrawing();
    }
    zapiszMagazyn();
    CloseWindow();
    return 0;
}
