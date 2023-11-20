#include "raylib.h"

// For using raygui
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_STATIC
#include "raygui.h"

typedef enum {
    TopLeft,
    TopCenter,
    TopRight,
    MiddleLeft, 
    Center,
    MiddleRight,
    BottomLeft, 
    BottomCenter,
    BottomRight
} PinLocation;

/**
 * Returns an offset defined by the offsetVal and offsetFrom parameters.
 * @param currentPos The desired location to be offset from using the offsetVal and offsetFrom.
 * @param offsetVal Used in conjunction with the offsetFrom to correctly offset the currentPos.
 * @param offsetFrom Where to offset from.
 * @returns Returns the pos correctly offset using the offsetVal and offsetFrom parameters.
*/
Vector2 offsetPosByPinLocation(Vector2 currentPos, Vector2 offsetVal, PinLocation offsetFrom, bool negativeOffset) {
    if (negativeOffset) {
        offsetVal.x = -1 * offsetVal.x;
        offsetVal.y = -1 * offsetVal.y;
    }
    switch (offsetFrom)
    {
    case 0: // TopLeft
        // Do nothing. Default operation is to draw from left
        break;

    case 1: // TopCenter
        currentPos.x = (offsetVal.x / 2) + currentPos.x;
        currentPos.y = 0 + currentPos.y;
        break;

    case 2: // TopRight
        currentPos.x = offsetVal.x + currentPos.x;
        currentPos.y = 0 + currentPos.y;
        break;

    case 3: // MiddleLeft
        currentPos.x = 0 + currentPos.x;
        currentPos.y = (offsetVal.y / 2) + currentPos.y;
        break;

    case 5: // MiddleRight
        currentPos.x = offsetVal.x + currentPos.x;
        currentPos.y = (offsetVal.y / 2) + currentPos.y;
        break;

    case 6: // BottomLeft
        currentPos.x = 0 + currentPos.x;
        currentPos.y = offsetVal.y + currentPos.y;
        break;

    case 7: // BottomCenter
        currentPos.x = (offsetVal.x / 2) + currentPos.x;
        currentPos.y = offsetVal.y + currentPos.y;
        break;

    case 8: // BottomRight
        currentPos.x = offsetVal.x + currentPos.x;
        currentPos.y = offsetVal.y + currentPos.y;
        break;
    
    default: // center
        currentPos.x = offsetVal.x / 2 + currentPos.x;
        currentPos.y = offsetVal.y / 2 + currentPos.y;
        break;
    }

    return currentPos;
}

void DrawTextRelativeEx(Font font, const char *text, Vector2 position, PinLocation relativeToWindow, PinLocation relativeToText, float fontSize, float spacing, Color tint) {
    Vector2 textRenderSize = MeasureTextEx(font, text, fontSize, spacing);

    Vector2 pos = {0, 0};
    int width = GetScreenWidth();
    int height = GetScreenHeight();

    // printf("Width & Height: %d, %d\n", width, height);

    pos = offsetPosByPinLocation(position, (Vector2){.x=width, .y=height}, relativeToWindow, false);
    pos = offsetPosByPinLocation(pos, textRenderSize, relativeToText, true);

    DrawTextEx(font, text, pos, fontSize, spacing, tint);
}

/**
 * This is an extension of the raygui GuiButton.
 * @returns Returns true when clicked.
*/
int GuiButtonRelative(char *text, Vector2 position, Vector2 buttonDimensions, PinLocation relativeToWindow, PinLocation relativeToText, int fontSize) {
    position = offsetPosByPinLocation(position, (Vector2){GetScreenWidth(), GetScreenHeight()}, relativeToWindow, false);
    position = offsetPosByPinLocation(position, buttonDimensions, relativeToText, true);
    Rectangle bounds = {.x=position.x, .y=position.y, .width=buttonDimensions.x, .height=buttonDimensions.y};
    GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
    return GuiButton(bounds, text);
}

int GuiDropdownBoxRelative(char *text, int *active, bool editMode, Vector2 position, Vector2 buttonDimensions, PinLocation relativeToWindow, PinLocation relativeToText, int fontSize) {
    position = offsetPosByPinLocation(position, (Vector2){GetScreenWidth(), GetScreenHeight()}, relativeToWindow, false);
    position = offsetPosByPinLocation(position, buttonDimensions, relativeToText, true);
    Rectangle bounds = {.x=position.x, .y=position.y, .width=buttonDimensions.x, .height=buttonDimensions.y};
    GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
    return GuiDropdownBox(bounds, text, active, editMode);
}