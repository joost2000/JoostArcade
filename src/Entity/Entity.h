#pragma once

#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"

typedef enum DrawFrom
{
    DRAW_FROM_CENTER,
    DRAW_FROM_TOP_LEFT,
    DRAW_FROM_TOP_CENTER,
    DRAW_FROM_BOTTOM_LEFT,
    DRAW_FROM_BOTTOM_CENTER,
    DRAW_FROM_BOTTOM_RIGHT,
    DRAW_FROM_RIGHT_CENTER,
    DRAW_FROM_LEFT_CENTER
} DrawFrom;

typedef enum EntityState
{
    IDLE,
    WALK_FORWARD,
    WALK_FORWARD_STRAVE_RIGHT,
    WALK_FORWARD_STRAVE_LEFT,
    WALK_RIGHT,
    WALK_BACKWARD,
    WALK_BACKWARD_STRAVE_RIGHT,
    WALK_BACKWARD_STRAVE_LEFT,
    WALK_LEFT,
} EntityState;

static const Vector2 DEFAULT_DRAWING_ORIGINS[8] = {
    {0.5f, 0.5f}, // DRAW_FROM_CENTER
    {0.0f, 0.0f}, // DRAW_FROM_TOP_LEFT (do nothing this is default)
    {0.5f, 0.0f}, // DRAW_FROM_TOP_CENTER
    {0.0f, 1.0f}, // DRAW_FROM_BOTTOM_LEFT
    {0.5f, 1.0f}, // DRAW_FROM_BOTTOM_CENTER
    {1.0f, 1.0f}, // DRAW_FROM_BOTTOM_RIGHT
    {1.0f, 0.5f}, // DRAW_FROM_RIGHT_CENTER
    {0.0f, 0.5f}  // DRAW_FROM_LEFT_CENTER
};

typedef struct SpriteSheetAnimation
{
    EntityState AnimationType[10];
    int AnimationTypeToRow[10];
    Texture2D SpriteSheet;
    Vector2 SpriteDimensions;
    int SheetColumns;
    int SheetRows;
    int targetRow;
    Rectangle frameRectangle;
    float totalFrametime;
    int currentAnimationFrame;
    bool MirrorFromRightToLeft;
    bool MirrorFromLeftToRight;
} SpriteSheetAnimation;

typedef struct Entity
{
    char *EntityName;
    Texture2D EntityTexture;
    SpriteSheetAnimation *animation;
    Vector2 EntityPos;
    Vector2 EntityVelocity;
    Vector2 EntityFriction;
    float EntityRotation;
    float EntityScale;
    DrawFrom EntityOrigin;
    EntityState currentState;
    EntityState previousState;
} Entity;

void render_entity(Entity *entity);
void move_entity(Entity *entity);
void add_animation(Entity *entity, SpriteSheetAnimation *newAnimationSheet);
void animate_entity(Entity *entity, Vector2 drawingOffset);