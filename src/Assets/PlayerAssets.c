#include "PlayerAssets.h"
#include "../SpriteSheetLoader/SpriteSheetLoader.h"

static bool add_player_animation(Entity *entity, SpriteSheetInfo spriteSheet, EntityState state, int row, bool mirrorHorizontal)
{
    SpriteSheetAnimation animation = create_sprite_sheet_animation(state, spriteSheet, row, mirrorHorizontal, false);
    return add_animation(entity, animation);
}

bool load_player_animations(Entity *entity)
{
    if (entity == NULL)
    {
        TraceLog(LOG_ERROR, "Cannot load player animations for a null entity");
        return false;
    }

    SpriteSheetInfo walkSheet = init_spritesheet("bowling_outfit_sprite_sheet_fixed.png", 32, 32);
    if (walkSheet.SpriteSheet.id == 0)
    {
        return false;
    }

    SpriteSheetInfo idleSheet = walkSheet;
    idleSheet.SheetColumns = 1;

    if (!add_player_animation(entity, idleSheet, IDLE, 0, false) ||
        !add_player_animation(entity, walkSheet, WALK_FORWARD, 4, false) ||
        !add_player_animation(entity, walkSheet, WALK_FORWARD_STRAVE_LEFT, 3, true) ||
        !add_player_animation(entity, walkSheet, WALK_FORWARD_STRAVE_RIGHT, 3, false) ||
        !add_player_animation(entity, walkSheet, WALK_BACKWARD, 0, false) ||
        !add_player_animation(entity, walkSheet, WALK_BACKWARD_STRAVE_LEFT, 1, true) ||
        !add_player_animation(entity, walkSheet, WALK_BACKWARD_STRAVE_RIGHT, 1, false) ||
        !add_player_animation(entity, walkSheet, WALK_LEFT, 2, true) ||
        !add_player_animation(entity, walkSheet, WALK_RIGHT, 2, false))
    {
        release_entity(entity);
        return false;
    }

    return true;
}
