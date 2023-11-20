#include "utils/common.h"
#include "stdio.h"

int main(void) {
    // raylib initialization start
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, "Simple C Slideshow");
    SetWindowMinSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    SetTargetFPS(60); 
    // raylib initialization end


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextRelativeEx(GetFontDefault(), "Simple C Slideshow", (Vector2){0,0}, Center, Center, 24, 1, GRAY);
        EndDrawing();
    }

    CloseWindow();

    return EXIT_SUCCESS;
}
