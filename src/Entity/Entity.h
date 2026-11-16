#pragma once

#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "EntityState.h"
#include "../Animation/SpriteSheetAnimation.h"

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

typedef struct Entity
{
    const char *EntityName;
    Texture2D EntityTexture;
    SpriteSheetAnimation *AssignedAnimations;
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
bool add_animation(Entity *entity, SpriteSheetAnimation newAnimationSheet);
void animate_entity(Entity *entity, Vector2 drawingOffset);
void release_entity(Entity *entity);