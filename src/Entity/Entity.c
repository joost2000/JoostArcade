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

void add_animation(Entity *entity, SpriteSheetAnimation *newAnimationSheet)
{
    newAnimationSheet->frameRectangle = (Rectangle){
        0,
        newAnimationSheet->targetRow * newAnimationSheet->SpriteDimensions.y,
        newAnimationSheet->SpriteDimensions.x,
        newAnimationSheet->SpriteDimensions.y};

    entity->animation = newAnimationSheet;
}

void animate_entity(Entity *entity, Vector2 drawingOffset)
{
    int AnimationIndex = (int)entity->currentState;
    SpriteSheetAnimation *currentAnimationData = entity->animation;

    if (currentAnimationData == NULL)
    {
        return;
    }

    if (entity->previousState != entity->currentState)
    {
        currentAnimationData->currentAnimationFrame = 0;
        currentAnimationData->totalFrametime = 0;
        currentAnimationData->frameRectangle.y = currentAnimationData->AnimationTypeToRow[AnimationIndex] * currentAnimationData->SpriteDimensions.y;
    }

    if (entity->currentState != IDLE)
    {
        currentAnimationData->totalFrametime += GetFrameTime();
        currentAnimationData->currentAnimationFrame = currentAnimationData->totalFrametime / (1.0f / 8.0f);
    }

    currentAnimationData->frameRectangle.x = (currentAnimationData->currentAnimationFrame % currentAnimationData->SheetColumns) * currentAnimationData->SpriteDimensions.x;

    DrawText(TextFormat(
                 "framrect.x: %f\nframerect.y: %f\ncurrent state: %d",
                 currentAnimationData->frameRectangle.x,
                 currentAnimationData->frameRectangle.y,
                 entity->currentState),
             0,
             50,
             20,
             RED);

    Rectangle destinatinRect = {.x = Vector2Subtract(entity->EntityPos, drawingOffset).x, .y = Vector2Subtract(entity->EntityPos, drawingOffset).y, .width = currentAnimationData->SpriteDimensions.x * 4.0f, .height = currentAnimationData->SpriteDimensions.y * 4.0f};
    Vector2 origin = {(float)destinatinRect.width / 2, (float)destinatinRect.height / 2};
    Rectangle sourceRectangle = currentAnimationData->frameRectangle;

    if (currentAnimationData->MirrorFromRightToLeft)
    {
        if (entity->currentState == WALK_LEFT ||
            entity->currentState == WALK_FORWARD_STRAVE_LEFT ||
            entity->currentState == WALK_BACKWARD_STRAVE_LEFT)
        {
            sourceRectangle.width *= -1.0f;
        }
    }

    if (currentAnimationData->MirrorFromLeftToRight)
    {
        if (entity->currentState == WALK_RIGHT ||
            entity->currentState == WALK_FORWARD_STRAVE_RIGHT ||
            entity->currentState == WALK_BACKWARD_STRAVE_RIGHT)
        {
            sourceRectangle.width *= -1.0f;
        }
    }

    DrawTexturePro(currentAnimationData->SpriteSheet, sourceRectangle, destinatinRect, origin, entity->EntityRotation, WHITE);
}
