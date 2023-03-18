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

int main()
{
   // Initialize the window

   int windowDimensions[2]{512, 380};

   InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");

   // Acceleration fue to gravity (pixels/s)/s
   const int gravity{1'000};

   // Nebula variables
   Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

   const int sizeOfNebulae{6};

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

      // Update scarfy position
      scarfyData.pos.y += velocity * dT;

      // Update scarfy animation frame
      if (!isInAir)
      {
         scarfyData.runningTime += dT;

         // Update animation frame

         if (scarfyData.runningTime >= scarfyData.updateTime)
         {
            scarfyData.runningTime = 0.0;
            scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
            scarfyData.frame++;
            if (scarfyData.frame > 5)
            {
               scarfyData.frame = 0;
            }
         }
      }

      // loop for animation
      for (int i = 0; i < sizeOfNebulae; i++)
      {
         // pass
         nebulae[i].runningTime += dT;
         if (nebulae[i].runningTime >= nebulae[i].updateTime)
         {
            nebulae[i].runningTime = 0.0;
            nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
            nebulae[i].frame++;
            if (nebulae[i].frame > 7)
            {
               nebulae[i].frame = 0;
            }
         }
         // pass
      }

      // Draw nebula

      for (int i = 0; i < sizeOfNebulae; i++)
      {
         DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
      }

      // Draw scarfy
      DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

      // Stop Drawing
      EndDrawing();
   }

   UnloadTexture(scarfy);
   UnloadTexture(nebula);
   CloseWindow();
}