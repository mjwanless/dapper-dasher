#include "raylib.h"

struct AnimData
{
	Rectangle rec;
	Vector2 pos;
	int frame;
	float updateTime;
	float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
	return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
	// Update running time
	data.runningTime += deltaTime;
	if (data.runningTime >= data.updateTime)
	{
		data.runningTime = 0.0;
		// update animation frame

		data.rec.x = data.frame * data.rec.width;
		data.frame++;
		if (data.frame > maxFrame)
		{
			data.frame = 0;
		}
	}

	return data;
	// end
}

int main()
{
	// Initialize the window

	int windowDimensions[2]{512, 380};

	InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");

	// Acceleration fue to gravity (pixels/s)/s
	const int gravity{1'000};

	// Nebula variables
	Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

	const int sizeOfNebulae{3};

	AnimData nebulae[sizeOfNebulae]{};

	for (int i = 0; i < sizeOfNebulae; i++)
	{
		// pass
		nebulae[i].rec.x = 0.0;
		nebulae[i].rec.y = 0.0;
		nebulae[i].rec.width = nebula.width / 8;
		nebulae[i].rec.height = nebula.height / 8;
		nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
		nebulae[i].frame = 0;
		nebulae[i].runningTime = 0.0;
		nebulae[i].updateTime = 1.0 / 16.0;
		nebulae[i].pos.x = windowDimensions[0] + (i * 300);
		// pass
	}

	float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

	// nebula x velocity (pixels/second)
	int nebVel{-200};

	// Scarfy variables
	Texture2D scarfy = LoadTexture("textures/scarfy.png");
	AnimData scarfyData;
	scarfyData.rec.width = (scarfy.width / 6);
	scarfyData.rec.height = (scarfy.height);
	scarfyData.rec.x = 0;
	scarfyData.rec.y = 0;
	scarfyData.pos.x = (windowDimensions[0] / 2 - scarfyData.rec.width / 2);
	scarfyData.pos.y = (windowDimensions[1] - scarfyData.rec.height);
	scarfyData.frame = 0;
	scarfyData.updateTime = 1.0 / 12.0;
	scarfyData.runningTime = 0.0;

	// Pixels per second
	bool isInAir{false};

	const int jumpVel{-600};

	int velocity{0};

	Texture2D background = LoadTexture("textures/far-buildings.png");
	Texture2D midground = LoadTexture("textures/back-buildings.png");
	Texture2D foreground = LoadTexture("textures/foreground.png");
	float bgX{0};
	float mgX{0};
	float fgX{0};

	bool collision{false};
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		// delta time (time since last frame)
		const float dT{GetFrameTime()};

		// Start Drawing
		BeginDrawing();
		ClearBackground(WHITE);

		bgX -= 20 * dT;
		mgX -= 40 * dT;
		fgX -= 80 * dT;

		if (bgX <= (-background.width * 2))
		{
			bgX = 0.0;
		}

		if (mgX <= (-midground.width * 2))
		{
			mgX = 0.0;
		}

		if (fgX <= (-foreground.width * 2))
		{
			fgX = 0.0;
		}

		// draw the backgroud
		Vector2 bg1Pos{bgX, 0.0};
		Vector2 bg2Pos{bgX + background.width * 2, 0.0};
		Vector2 mg1Pos{mgX, 0.0};
		Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
		Vector2 fg1Pos{fgX, 0.0};
		Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
		DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
		DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);
		DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
		DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);
		DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
		DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

		// Ground check
		if (isOnGround(scarfyData, windowDimensions[1]))
		{
			// Rectangle is on the ground
			velocity = 0;
			isInAir = false;
		}
		else
		{
			// rectangle is in the air
			velocity += gravity * dT;
			isInAir = true;
		}

		// Check for jumping
		if (IsKeyPressed(KEY_SPACE) && !isInAir)
		{
			velocity += jumpVel;
		}

		// update nebula position
		for (int i = 0; i < sizeOfNebulae; i++)
		{
			nebulae[i].pos.x += nebVel * dT;
		}

		finishLine += nebVel * dT;

		// Update scarfy position
		scarfyData.pos.y += velocity * dT;

		// Update scarfy animation frame
		if (!isInAir)
		{
			scarfyData = updateAnimData(scarfyData, dT, 5);
		}

		// loop for animation
		for (int i = 0; i < sizeOfNebulae; i++)
		{
			nebulae[i] = updateAnimData(nebulae[i], dT, 7);
		}

		for (AnimData nebula : nebulae)
		{
			float pad{50};
			Rectangle nebRec{
				 nebula.pos.x + pad,
				 nebula.pos.y + pad,
				 nebula.rec.width - (2 * pad),
				 nebula.rec.height - (2 * pad)
				 // padding
			};
			Rectangle scarfyRec{
				 scarfyData.pos.x,
				 scarfyData.pos.y,
				 scarfyData.rec.width,
				 scarfyData.rec.height
				 // Space
			};
			if (CheckCollisionRecs(nebRec, scarfyRec))
			{
				collision = true;
				// space
			}
		}

		if (collision)
		{
			// lose the game
			DrawText("Game over; Please insert 40 quarters.", (windowDimensions[0] / 4), (windowDimensions[1] / 2), 40, RED);
		}
		else if (scarfyData.pos.x >= finishLine)
		{
			// Win the game
			DrawText("You win!; Please insert 50 quarters!", (windowDimensions[0] / 4), (windowDimensions[1] / 2), 40, RED);
		}
		else
		{
			for (int i = 0; i < sizeOfNebulae; i++)
			{
				DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
			}

			// Draw scarfy
			DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
		}

		// Stop Drawing
		EndDrawing();
	}

	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	UnloadTexture(background);
	UnloadTexture(midground);
	UnloadTexture(foreground);
	CloseWindow();
}