#include "raylib.h"
#include "stdlib.h"

// // For using raygui
#include "raygui.h"

// Allow the toggling of display debug information
#define ALLOW_DEBUG_OVERLAY true

// The game version
#define MAJOR_VERSION 0
#define MINOR_VERSION 1
#define PATCH_VERSION 0
#define STR_VERSION TextFormat("%d.%d.%d", MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION)

// Min window size
#define MIN_WINDOW_WIDTH 800
#define MIN_WINDOW_HEIGHT 450

/** Signal that caller should also close window. */
#define WINDOW_CLOSED 11

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
Vector2 offsetPosByPinLocation(Vector2 currentPos, Vector2 offsetVal, PinLocation offsetFrom, bool negativeOffset);

void DrawTextRelativeEx(Font font, const char *text, Vector2 position, PinLocation relativeToWindow, PinLocation relativeToText, float fontSize, float spacing, Color tint);

/**
 * This is an extension of the raygui GuiButton.
 * @returns Returns true when clicked.
*/
int GuiButtonRelative(char *text, Vector2 position, Vector2 buttonDimensions, PinLocation relativeToWindow, PinLocation relativeToText, int fontSize);

int GuiDropdownBoxRelative(char *text, int *active, bool editMode, Vector2 position, Vector2 buttonDimensions, PinLocation relativeToWindow, PinLocation relativeToText, int fontSize);