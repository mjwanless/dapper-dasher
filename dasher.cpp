#include "raylib.h"

int main()
{
   // Window dimensions
   const int windowWidth{512};
   const int windowHeight{380};

   // Initialize the window

   InitWindow(windowWidth, windowHeight, "Dapper Dasher!");

   // Acceleration fue to gravity (pixels/s)/s
   const int gravity{1'000};

   Texture2D scarfy = LoadTexture("textures/scarfy.png");
   Rectangle scarfyRec;
   scarfyRec.width = (scarfy.width / 6);
   scarfyRec.height = scarfy.height;
   scarfyRec.x = 0;
   scarfyRec.y = 0;
   Vector2 scarfyPos;
   scarfyPos.x = (windowWidth / 2 - scarfyRec.width / 2);
   scarfyPos.y = (windowHeight / 2 - scarfyRec.height / 2);

   // Pixels per second
   const int jumpVel{-600};
   bool isInAir{false};

   int velocity{0};

   SetTargetFPS(60);

   while (!WindowShouldClose())
   {
      // delta time (time since last frame)
      const float dT{GetFrameTime()};

      // Start Drawing
      BeginDrawing();
      ClearBackground(WHITE);

      // Movement

      // Ground check
      if (scarfyPos.y >= windowHeight - scarfyRec.height)
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

      // Update position
      scarfyPos.y += velocity * dT;

      DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

      // Stop Drawing
      EndDrawing();
   }

   UnloadTexture(scarfy);
   CloseWindow();
}