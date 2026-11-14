#include "PlayerController.h"

void controller_assign_entity(PlayerController *controller, Entity *entity)
{
    controller->controllingEntity = entity;
}

void controller_apply_input(PlayerController *controller)
{
    Vector2 inputDelta = {.x = 0, .y = 0};

    if (IsKeyDown(KEY_W))
    {
        inputDelta.y = -1.0f;
    }

    if (IsKeyDown(KEY_A))
    {

        inputDelta.x = -1.0f;
    }

    if (IsKeyDown(KEY_S))
    {

        inputDelta.y = 1.0f;
    }

    if (IsKeyDown(KEY_D))
    {

        inputDelta.x = 1.0f;
    }

    inputDelta = Vector2Multiply(
        inputDelta,
        (Vector2){.x = controller->movementSpeed, .y = controller->movementSpeed});

    inputDelta = Vector2Multiply(
        inputDelta,
        (Vector2){.x = GetFrameTime(), .y = GetFrameTime()});

    inputDelta.x = inputDelta.x == 0 ? controller->controllingEntity->EntityVelocity.x : inputDelta.x;
    inputDelta.y = inputDelta.y == 0 ? controller->controllingEntity->EntityVelocity.y : inputDelta.y;

    controller->controllingEntity->EntityVelocity = inputDelta;
}
