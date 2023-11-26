#include "utils/common.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#define FRAMES_PER_SECOND 30
#define UP_SCALE_IMAGES false
#define PANORAMA_MIN_ASPECT_RATIO 21 / 9

int main(void) {
    // raylib initialization start
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, "Simple C Slideshow");
    SetWindowMinSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    SetTargetFPS(FRAMES_PER_SECOND); 
    SetExitKey(0);
    // raylib initialization end

    const uint32_t testImagesLen = 3;
    char* testImages[] = {
        "test1.png",
        "test2.jpg",
        "test3.jpg"
    };

    uint32_t index = 0;
    uint32_t maxFrameCount = FRAMES_PER_SECOND * 1.5;
    uint32_t frameCount = 0;

    float panoramaXOffset = 0;
    float panoramaBreak = 0.25;
    bool panForPanorama = true;

    bool fullScreenMode = false;

    uint32_t standardMaxFrameCount = FRAMES_PER_SECOND * 1.5;
    uint32_t panoramaMaxFrameCount = FRAMES_PER_SECOND * 5;

    Image displayImage = LoadImage(testImages[0]);
    Texture displayImageTexture = LoadTextureFromImage(displayImage);
    UnloadImage(displayImage);

    bool pauseMenu = false;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyPressed(KEY_ESCAPE)) pauseMenu = !pauseMenu;

        BeginDrawing();
        ClearBackground(BLACK);
        float xScale = (float)GetScreenWidth() / (float)displayImageTexture.width;
        float yScale = (float)GetScreenHeight() / (float)displayImageTexture.height;
        float scale = min(xScale, yScale);
        float aspectRatio = displayImageTexture.width / displayImageTexture.height;

        if (aspectRatio > PANORAMA_MIN_ASPECT_RATIO && panForPanorama) {
            if (frameCount > FRAMES_PER_SECOND * panoramaBreak && frameCount < (maxFrameCount - FRAMES_PER_SECOND * panoramaBreak)) {
                int maxFrameCountMod = maxFrameCount - FRAMES_PER_SECOND * panoramaBreak * 2;
                int frameCountMod = frameCount - FRAMES_PER_SECOND * panoramaBreak;
                panoramaXOffset = ((float)frameCountMod / (float)maxFrameCountMod) * -((float)displayImageTexture.width * yScale - (float)GetScreenWidth());
            }
            DrawTextureRelativeEx(displayImageTexture, (Vector2){panoramaXOffset,0}, 0, yScale, MiddleLeft, MiddleLeft, WHITE);
        } else {
            DrawTextureRelativeEx(displayImageTexture, (Vector2){0,0}, 0, scale, Center, Center, WHITE);
        }


        if (!pauseMenu) {
            frameCount++;
            if (frameCount >= maxFrameCount) {
                frameCount = 0;
                index++;
                if (index >= testImagesLen)
                    index = 0;
                displayImage = LoadImage(testImages[index]);
                displayImageTexture = LoadTextureFromImage(displayImage);
                float aspectRatio = displayImageTexture.width / displayImageTexture.height;
                if (aspectRatio > PANORAMA_MIN_ASPECT_RATIO && panForPanorama) {
                    maxFrameCount = panoramaMaxFrameCount;
                    panoramaXOffset = 0;
                } else {
                    maxFrameCount = standardMaxFrameCount;
                }
                UnloadImage(displayImage);
            }
        } else {
            DrawRectangleRelative((Vector2){0,0}, (Vector2){GetScreenWidth(), GetScreenHeight()}, Center, Center, (Color){0,0,0,100});
            int buttonHeight = 32;
            int yStart = -64;
            int yOffset = buttonHeight * 1.25;
            int yIndex = 0;
            int padding = 20;
            DrawTextRelativeEx(GetFontDefault(), "Pause", (Vector2){0, yStart + yIndex++ * yOffset}, Center, TopCenter, 32, 1, RAYWHITE);
            if (panForPanorama) {
                if (GuiButtonRelative("Disable pan for panorama", (Vector2){0, yStart + yIndex++ * yOffset}, (Vector2){MeasureText("Disable pan for panorama", 20) + padding, buttonHeight}, Center, TopCenter, 20)) {
                    panForPanorama = false;
                    if (aspectRatio > PANORAMA_MIN_ASPECT_RATIO)
                        maxFrameCount = standardMaxFrameCount;
                }
            } else {
                if (GuiButtonRelative("Enable pan for panorama", (Vector2){0, yStart + yIndex++ * yOffset}, (Vector2){MeasureText("Enable pan for panorama", 20) + padding, buttonHeight}, Center, TopCenter, 20)) {
                    panForPanorama = true;
                    if (aspectRatio > PANORAMA_MIN_ASPECT_RATIO)
                        maxFrameCount = panoramaMaxFrameCount;  
                }
            }

            if (GuiButtonRelative("Toggle Full Screen", (Vector2){0, yStart + yIndex++ * yOffset}, (Vector2){MeasureText("Toggle Full Screen", 20) + padding, buttonHeight}, Center, TopCenter, 20)) {
                if (fullScreenMode) {
                    SetWindowState(FLAG_WINDOW_MAXIMIZED);
                    fullScreenMode = false;
                } else {
                    SetWindowState(FLAG_WINDOW_MAXIMIZED);
                    SetWindowState(FLAG_FULLSCREEN_MODE);
                    // ToggleFullscreen();
                    fullScreenMode = true;
                }
                // ToggleFullscreen();
            }

            if (GuiButtonRelative("Exit to Desktop", (Vector2){0, yStart + yIndex++ * yOffset}, (Vector2){MeasureText("Exit to Desktop", 20) + padding, buttonHeight}, Center, TopCenter, 20)) break;
        }
        EndDrawing();
    }

    UnloadTexture(displayImageTexture);

    CloseWindow();

    return EXIT_SUCCESS;
}
