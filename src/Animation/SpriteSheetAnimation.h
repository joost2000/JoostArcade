#pragma once

#include "raylib.h"
#include "../Entity/EntityState.h"

typedef struct SpriteSheetInfo
{
    Texture2D SpriteSheet;
    Vector2 SpriteDimensions;
    int SheetColumns;
    int SheetRows;
} SpriteSheetInfo;

typedef struct SpriteSheetAnimation
{
    EntityState AnimationType;
    SpriteSheetInfo spriteSheetInfo;
    int targetRow;
    Rectangle frameRectangle;
    float totalFrametime;
    int currentAnimationFrame;
    bool mirrorHorizontal;
    bool mirrorVertical;
} SpriteSheetAnimation;
