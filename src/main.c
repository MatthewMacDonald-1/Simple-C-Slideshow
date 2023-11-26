#include "utils/common.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#define FRAMES_PER_SECOND 30
#define PANORAMA_MIN_ASPECT_RATIO 21 / 9

int main(void) {
    // raylib initialization start
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, "Simple C Slideshow");
    SetWindowMinSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    SetTargetFPS(FRAMES_PER_SECOND); 
    SetExitKey(0);
    // raylib initialization end

    const int32_t testImagesLen = 5;
    char* testImages[] = {
        "test3.jpg",
        "test2.jpg",
        "test1.jpg",
        "test4.jpg",
        "test5.png"
    };

    int32_t index = 0;
    uint32_t maxFrameCount = FRAMES_PER_SECOND * 1.5;
    uint32_t frameCount = 0;

    float panoramaXOffset = 0;
    float panoramaBreak = 0.25;

    bool panForPanorama = true;
    bool fullScreenMode = false;
    bool showIndex = false;
    bool blowUpImages = false;

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

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT)) {
            if (IsKeyPressed(KEY_LEFT)) {
                index --;
            } else {
                index ++;
            }
            if (index >= testImagesLen) index = 0;
            if (index < 0) index = testImagesLen - 1;
            frameCount = 0;
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

        BeginDrawing();
        ClearBackground(BLACK);
        float xScale = (float)GetScreenWidth() / (float)displayImageTexture.width;
        float yScale = (float)GetScreenHeight() / (float)displayImageTexture.height;
        float scale = min(xScale, yScale);
        float aspectRatio = displayImageTexture.width / displayImageTexture.height;

        if (scale > 1 && !blowUpImages) {
            scale = 1;
            yScale = 1;
        }

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

        if (showIndex) {
            char* text = TextFormat("%d / %d", index + 1, testImagesLen);
            DrawTextRelativeEx(GetFontDefault(), text, (Vector2){10, -5}, BottomLeft, BottomLeft, 20, 1, RAYWHITE);
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
            int yStart = -128;
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
                    fullScreenMode = true;
                }
            }

            if (!blowUpImages) {
                if (GuiButtonRelative("Enlarge Small Images", (Vector2){0, yStart + yIndex++ * yOffset}, (Vector2){MeasureText("Enlarge Small Images", 20) + padding, buttonHeight}, Center, TopCenter, 20)) {
                    blowUpImages = true;
                }
            } else {
                if (GuiButtonRelative("Don't enlarge Small Images", (Vector2){0, yStart + yIndex++ * yOffset}, (Vector2){MeasureText("Don't enlarge Small Images", 20) + padding, buttonHeight}, Center, TopCenter, 20)) {
                    blowUpImages = false;
                }
            }

            if (showIndex) {
                if (GuiButtonRelative("Hide Index", (Vector2){0, yStart + yIndex++ * yOffset}, (Vector2){MeasureText("Hide Index", 20) + padding, buttonHeight}, Center, TopCenter, 20)) {
                    showIndex = false;
                }
            } else {
                if (GuiButtonRelative("Show Index", (Vector2){0, yStart + yIndex++ * yOffset}, (Vector2){MeasureText("Show Index", 20) + padding, buttonHeight}, Center, TopCenter, 20)) {
                    showIndex = true;
                }
            }
            

            if (GuiButtonRelative("Exit to Desktop", (Vector2){0, yStart + yIndex++ * yOffset}, (Vector2){MeasureText("Exit to Desktop", 20) + padding, buttonHeight}, Center, TopCenter, 20)) break;
        }
        EndDrawing();
    }

    UnloadTexture(displayImageTexture);

    CloseWindow();

    return EXIT_SUCCESS;
}
