#include "Entity.h"

void render_entity(Entity *entity)
{
    Vector2 drawingOrigin = DEFAULT_DRAWING_ORIGINS[entity->EntityOrigin];
    Vector2 textureDimensions = {.x = entity->EntityTexture.width, .y = entity->EntityTexture.height};
    textureDimensions = Vector2Multiply(textureDimensions, (Vector2){.x = entity->EntityScale, .y = entity->EntityScale});
    Vector2 drawingOffset = Vector2Multiply(drawingOrigin, textureDimensions);
    // DrawTextureEx(entity->EntityTexture, Vector2Subtract(entity->EntityPos, drawingOffset), entity->EntityRotation, entity->EntityScale, WHITE);
    animate_entity(entity, drawingOffset);
}

void move_entity(Entity *entity)
{
    entity->EntityPos = Vector2Add(entity->EntityPos, entity->EntityVelocity);
    entity->EntityVelocity = Vector2Multiply(entity->EntityVelocity, entity->EntityFriction);

    entity->previousState = entity->currentState;

    EntityState newState = IDLE;

    if (entity->EntityVelocity.y < -0.01f)
    {
        newState = WALK_FORWARD;
    }

    if (entity->EntityVelocity.y > 0.01f)
    {
        newState = WALK_BACKWARD;
    }

    if (newState == WALK_FORWARD)
    {
        if (entity->EntityVelocity.x < -0.01f)
        {
            newState = WALK_FORWARD_STRAVE_LEFT;
        }

        if (entity->EntityVelocity.x > 0.01f)
        {
            newState = WALK_FORWARD_STRAVE_RIGHT;
        }
    }

    if (newState == WALK_BACKWARD)
    {
        if (entity->EntityVelocity.x < -0.01f)
        {
            newState = WALK_BACKWARD_STRAVE_LEFT;
        }

        if (entity->EntityVelocity.x > 0.01f)
        {
            newState = WALK_BACKWARD_STRAVE_RIGHT;
        }
    }

    if (newState == IDLE)
    {
        if (entity->EntityVelocity.x < -0.01f)
        {
            newState = WALK_LEFT;
        }

        if (entity->EntityVelocity.x > 0.01f)
        {
            newState = WALK_RIGHT;
        }
    }

    entity->currentState = newState;
}

bool add_animation(Entity *entity, SpriteSheetAnimation newAnimationSheet)
{
    if (entity == NULL || newAnimationSheet.AnimationType < 0 || newAnimationSheet.AnimationType >= ENTITY_STATE_COUNT)
    {
        TraceLog(LOG_ERROR, "Cannot add animation: invalid entity or animation state");
        return false;
    }

    if (entity->AssignedAnimations == NULL)
    {
        entity->AssignedAnimations = (SpriteSheetAnimation *)calloc(ENTITY_STATE_COUNT, sizeof(*entity->AssignedAnimations));
        if (entity->AssignedAnimations == NULL)
        {
            TraceLog(LOG_ERROR, "Cannot allocate entity animation slots");
            return false;
        }
    }

    entity->AssignedAnimations[newAnimationSheet.AnimationType] = newAnimationSheet;
    return true;
}

void animate_entity(Entity *entity, Vector2 drawingOffset)
{
    int AnimationIndex = (int)entity->currentState;
    if (entity->AssignedAnimations == NULL || AnimationIndex < 0 || AnimationIndex >= ENTITY_STATE_COUNT)
    {
        return;
    }

    SpriteSheetAnimation *currentAnimationData = &entity->AssignedAnimations[AnimationIndex];

    if (currentAnimationData->spriteSheetInfo.SpriteSheet.id == 0 ||
        currentAnimationData->spriteSheetInfo.SheetColumns <= 0 ||
        currentAnimationData->spriteSheetInfo.SpriteDimensions.x <= 0 ||
        currentAnimationData->spriteSheetInfo.SpriteDimensions.y <= 0)
    {
        return;
    }

    if (entity->previousState != entity->currentState)
    {
        currentAnimationData->currentAnimationFrame = 0;
        currentAnimationData->totalFrametime = 0;
    }

    if (entity->currentState != IDLE)
    {
        currentAnimationData->totalFrametime += GetFrameTime();
        currentAnimationData->currentAnimationFrame = currentAnimationData->totalFrametime / (1.0f / 8.0f);
    }

    currentAnimationData->frameRectangle.x = (currentAnimationData->currentAnimationFrame % currentAnimationData->spriteSheetInfo.SheetColumns) * currentAnimationData->spriteSheetInfo.SpriteDimensions.x;

    Rectangle destinatinRect = {.x = Vector2Subtract(entity->EntityPos, drawingOffset).x, .y = Vector2Subtract(entity->EntityPos, drawingOffset).y, .width = currentAnimationData->spriteSheetInfo.SpriteDimensions.x * 4.0f, .height = currentAnimationData->spriteSheetInfo.SpriteDimensions.y * 4.0f};
    Vector2 origin = {(float)destinatinRect.width / 2, (float)destinatinRect.height / 2};
    Rectangle sourceRectangle = currentAnimationData->frameRectangle;

    if (currentAnimationData->mirrorHorizontal)
    {
        sourceRectangle.width *= -1.0f;
    }

    if (currentAnimationData->mirrorVertical)
    {
        sourceRectangle.height *= -1.0f;
    }

    DrawTexturePro(currentAnimationData->spriteSheetInfo.SpriteSheet, sourceRectangle, destinatinRect, origin, entity->EntityRotation, WHITE);
}

void release_entity(Entity *entity)
{
    if (entity == NULL)
    {
        return;
    }

    free(entity->AssignedAnimations);
    entity->AssignedAnimations = NULL;
}
