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

#include "Entity/Entity.h"
#include "PlayerController/PlayerController.h"

int main()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_FULLSCREEN_MODE);

	// Create the window and OpenGL context
	InitWindow(720, 1080, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");
	Texture walkingsprite = LoadTexture("16x32 Walk-Sheet.png");

	Entity PlayerEntity = {
		.EntityName = "myEntity",
		.EntityOrigin = DRAW_FROM_CENTER,
		.EntityPos = {.x = 400, .y = 200},
		.EntityFriction = {.x = .988f, .y = .988f},
		.EntityRotation = 0.0f,
		.EntityScale = 5.0f,
		.EntityTexture = wabbit,
		.currentState = WALK_FORWARD,
		.previousState = WALK_FORWARD,
	};

	SpriteSheetAnimation playerWalkAnim = {
		.AnimationType = {
			IDLE,
			WALK_FORWARD,
			WALK_FORWARD_STRAVE_RIGHT,
			WALK_FORWARD_STRAVE_LEFT,
			WALK_RIGHT,
			WALK_BACKWARD,
			WALK_BACKWARD_STRAVE_RIGHT,
			WALK_BACKWARD_STRAVE_LEFT,
			WALK_LEFT,
		},
		.AnimationTypeToRow = {
			0,
			4,
			3,
			3,
			2,
			0,
			1,
			1,
			2,
		},
		.SheetColumns = 4,
		.SheetRows = 5,
		.targetRow = 4,
		.SpriteDimensions = (Vector2){.x = 32, .y = 32},
		.SpriteSheet = walkingsprite,
		.MirrorFromRightToLeft = true,
	};

	PlayerController LocalController = {
		.movementSpeed = 100,
	};

	add_animation(&PlayerEntity, &playerWalkAnim);

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

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
