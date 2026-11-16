#pragma once

#include "stdlib.h"
#include "../Animation/SpriteSheetAnimation.h"

typedef struct SpriteSheetAssetContainer SpriteSheetAssetContainer;

bool init_spritesheet_loader(size_t capacity);
void release_spritesheet_loader();

SpriteSheetInfo init_spritesheet(const char *spriteSheetPath, int spriteWidth, int spriteHeight);
SpriteSheetAnimation create_sprite_sheet_animation(EntityState animationType, SpriteSheetInfo spriteSheetInfo, int targetRow, bool mirrorHorizontal, bool mirrorVertical);