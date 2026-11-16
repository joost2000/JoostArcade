/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "math.h"

#include "resource_dir.h" // utility header for SearchAndSetResourceDir

#include "Assets/PlayerAssets.h"
#include "Entity/Entity.h"
#include "PlayerController/PlayerController.h"
#include "SpriteSheetLoader/SpriteSheetLoader.h"

int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_FULLSCREEN_MODE);

	// Create the window and OpenGL context
	InitWindow(720, 1080, "Hello Raylib");

	if (!SearchAndSetResourceDir("resources"))
	{
		TraceLog(LOG_ERROR, "Could not locate the resources directory");
		CloseWindow();
		return 1;
	}

	if (!init_spritesheet_loader(1))
	{
		CloseWindow();
		return 1;
	}

	Entity PlayerEntity = {
		.EntityName = "myEntity",
		.EntityPos = {.x = 400, .y = 200},
		.EntityFriction = {.x = .988f, .y = .988f},
		.EntityRotation = 0.0f,
		.EntityScale = 5.0f,
		.EntityOrigin = DRAW_FROM_CENTER,
		.currentState = WALK_FORWARD,
		.previousState = WALK_FORWARD,
	};

	if (!load_player_animations(&PlayerEntity))
	{
		release_entity(&PlayerEntity);
		release_spritesheet_loader();
		CloseWindow();
		return 1;
	}

	PlayerController LocalController = {
		.movementSpeed = 100,
	};

	controller_assign_entity(&LocalController, &PlayerEntity);

	SetTargetFPS(1000);

	Camera2D camera = {0};
	camera.zoom = 1.0f;
	// camera.offset = (Vector2){GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f};

	// game loop
	while (!WindowShouldClose()) // run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		controller_apply_input(&LocalController);

		// float followSpeed = 5.0f;
		// float amount = 1.0f - expf(-followSpeed * GetFrameTime());
		// camera.target = Vector2Lerp(camera.target, PlayerEntity.EntityPos, amount);

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		rlPushMatrix();
		rlTranslatef(0, 25 * 50, 0);
		rlRotatef(90, 1, 0, 0);
		DrawGrid(250, 50);
		rlPopMatrix();

		move_entity(&PlayerEntity);
		render_entity(&PlayerEntity);
		DrawFPS(0, 0);

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	release_entity(&PlayerEntity);
	release_spritesheet_loader();

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
