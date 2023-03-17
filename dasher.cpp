#include "raylib.h"

struct AnimData
{
   Rectange rec;
   Vector2 pos;
   int frame;
   float updateTime;
   float runningTime;
};

int main()
{
   // Window dimensions
   const int windowWidth{512};
   const int windowHeight{380};

   // Initialize the window

   InitWindow(windowWidth, windowHeight, "Dapper Dasher!");

   // Acceleration fue to gravity (pixels/s)/s
   const int gravity{1'000};

   // Nebula variables
   Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
   Rectangle nebRec{0.0, 0.0, (nebula.width / 8), (nebula.height / 8)};
   Vector2 nebPos{windowWidth, (windowHeight - nebRec.height)};

   Rectangle neb2Rec{0.0, 0.0, (nebula.width / 8), (nebula.height / 8)};
   Vector2 neb2Pos{windowWidth + 300, (windowHeight - nebRec.height)};

   // Nebula animation variables
   int nebFrame{0};
   const float nebUpdateTime{1.0 / 12.0};
   float nebRunningTime{0};

   int neb2Frame{};
   const float neb2UpdateTime{1.0 / 16.0};
   float neb2RunningTime{0};

   // nebula x velocity (pixels/second)
   int nebVel{-200};

   // Scarfy variables
   Texture2D scarfy = LoadTexture("textures/scarfy.png");
   AnimData scarfyData;
   scarfyData.rec.width = (scarfy.width / 6);
   scarfyData.rec.height = (scarfy.height);
   scarfyData.rec.x = 0;
   scarfyData.rec.y = 0;
   scarfyData.pos.x = (windowWidth / 2 - scarfyData.rec.width / 2);
   scarfyData.pos.y = (windowHeight - scarfyData.rec.height);
   scarfyData.frame = 0;
   scarfyData.updateTime = 1.0 / 12.0;
   scarfyData.runningTime = 0.0;

   Rectangle scarfyRec;
   scarfyRec.width = (scarfy.width / 6);
   scarfyRec.height = scarfy.height;
   scarfyRec.x = 0;
   scarfyRec.y = 0;
   Vector2 scarfyPos;
   scarfyPos.x = (windowWidth / 2 - scarfyRec.width / 2);
   scarfyPos.y = (windowHeight - scarfyRec.height);

   int frame{0};

   // Pixels per second
   bool isInAir{false};

   const int jumpVel{-600};

   const float updateTime{1.0 / 12.0};
   float runningTime{0};

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

      // update nebula position
      nebPos.x += nebVel * dT;

      // update nebula 2 position
      neb2Pos.x += nebVel * dT;

      // Update scarfy position
      scarfyPos.y += velocity * dT;

      // Update scarfy animation frame
      if (!isInAir)
      {
         runningTime += dT;

         // Update animation frame

         if (runningTime >= updateTime)
         {
            runningTime = 0.0;
            scarfyRec.x = frame * scarfyRec.width;
            frame++;
            if (frame > 5)
            {
               frame = 0;
            }
         }
      }

      // Update nebula animation frame
      nebRunningTime += dT;
      if (nebRunningTime >= nebUpdateTime)
      {
         nebRunningTime = 0.0;
         nebRec.x = nebFrame * nebRec.width;
         nebFrame++;
         if (nebFrame > 7)
         {
            nebFrame = 0;
         }
      }

      neb2RunningTime += dT;
      if (neb2RunningTime >= neb2UpdateTime)
      {
         neb2RunningTime = 0.0;
         neb2Rec.x = neb2Frame * neb2Rec.width;
         neb2Frame++;
         if (neb2Frame > 7)
         {
            neb2Frame = 0;
         }
      }

      // Draw nebula
      DrawTextureRec(nebula, nebRec, nebPos, WHITE);

      // Draw the 2nd nebula
      DrawTextureRec(nebula, neb2Rec, neb2Pos, RED);

      // Draw scarfy
      DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

      // Stop Drawing
      EndDrawing();
   }

   UnloadTexture(scarfy);
   UnloadTexture(nebula);
   CloseWindow();
}