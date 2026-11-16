#include "SpriteSheetLoader.h"

struct SpriteSheetAssetContainer
{
    SpriteSheetInfo *SpriteSheetInfoCollection;
    size_t capacity;
    size_t count;
};

static SpriteSheetAssetContainer *_spritesheet_container;

bool init_spritesheet_loader(size_t capacity)
{
    if (capacity == 0)
    {
        TraceLog(LOG_ERROR, "Spritesheet loader capacity must be greater than zero");
        return false;
    }

    release_spritesheet_loader();

    SpriteSheetAssetContainer *newContainer = (SpriteSheetAssetContainer *)malloc(sizeof(*newContainer));

    if (newContainer == NULL)
    {
        TraceLog(LOG_ERROR, "Cannot allocate spritesheet loader");
        return false;
    }

    newContainer->SpriteSheetInfoCollection = (SpriteSheetInfo *)calloc(capacity, sizeof(*newContainer->SpriteSheetInfoCollection));

    if (newContainer->SpriteSheetInfoCollection == NULL)
    {
        free(newContainer);
        TraceLog(LOG_ERROR, "Cannot allocate spritesheet loader asset collection");
        return false;
    }

    newContainer->capacity = capacity;
    newContainer->count = 0;

    _spritesheet_container = newContainer;
    return true;
}

void release_spritesheet_loader()
{
    if (_spritesheet_container == NULL)
    {
        return;
    }

    for (size_t i = 0; i < _spritesheet_container->count; i++)
    {
        UnloadTexture(_spritesheet_container->SpriteSheetInfoCollection[i].SpriteSheet);
    }

    free(_spritesheet_container->SpriteSheetInfoCollection);
    free(_spritesheet_container);
    _spritesheet_container = NULL;
}

SpriteSheetInfo init_spritesheet(const char *spriteSheetPath, int spriteWidth, int spriteHeight)
{
    if (_spritesheet_container == NULL || spriteSheetPath == NULL || spriteWidth <= 0 || spriteHeight <= 0)
    {
        TraceLog(LOG_ERROR, "Cannot load spritesheet: invalid loader state, path, or frame dimensions");
        return (SpriteSheetInfo){};
    }

    if (_spritesheet_container->count >= _spritesheet_container->capacity)
    {
        TraceLog(LOG_ERROR, "Spritesheet loader asset capacity exceeded");
        return (SpriteSheetInfo){};
    }

    SpriteSheetInfo newSpriteSheetInfo = {
        .SpriteSheet = LoadTexture(spriteSheetPath),
        .SpriteDimensions = (Vector2){.x = spriteWidth, .y = spriteHeight},
    };

    if (newSpriteSheetInfo.SpriteSheet.id == 0)
    {
        TraceLog(LOG_ERROR, "Failed to load spritesheet: %s", spriteSheetPath);
        return (SpriteSheetInfo){};
    }

    newSpriteSheetInfo.SheetRows = newSpriteSheetInfo.SpriteSheet.height / spriteHeight;
    newSpriteSheetInfo.SheetColumns = newSpriteSheetInfo.SpriteSheet.width / spriteWidth;

    if (newSpriteSheetInfo.SheetRows <= 0 || newSpriteSheetInfo.SheetColumns <= 0)
    {
        TraceLog(LOG_ERROR, "Spritesheet dimensions are smaller than the requested frame size: %s", spriteSheetPath);
        UnloadTexture(newSpriteSheetInfo.SpriteSheet);
        return (SpriteSheetInfo){};
    }

    _spritesheet_container->SpriteSheetInfoCollection[_spritesheet_container->count++] = newSpriteSheetInfo;

    return newSpriteSheetInfo;
}

SpriteSheetAnimation create_sprite_sheet_animation(EntityState animationType, SpriteSheetInfo spriteSheetInfo, int targetRow, bool mirrorHorizontal, bool mirrorVertical)
{
    if (_spritesheet_container == NULL)
    {
        return (SpriteSheetAnimation){};
    }

    Rectangle frameRectangle = {
        .x = 0,
        .y = targetRow * spriteSheetInfo.SpriteDimensions.y,
        .width = spriteSheetInfo.SpriteDimensions.x,
        .height = spriteSheetInfo.SpriteDimensions.y};

    SpriteSheetAnimation newAnimation = {
        .AnimationType = animationType,
        .spriteSheetInfo = spriteSheetInfo,
        .targetRow = targetRow,
        .frameRectangle = frameRectangle,
        .mirrorHorizontal = mirrorHorizontal,
        .mirrorVertical = mirrorVertical};

    return newAnimation;
}
