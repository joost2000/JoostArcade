#pragma once

#include "../Entity/Entity.h"
#include "raylib.h"

typedef struct PlayerController
{
    Entity *controllingEntity;
    float movementSpeed;
} PlayerController;

void controller_assign_entity(PlayerController *controller, Entity *entity);
void controller_apply_input(PlayerController *controller);
