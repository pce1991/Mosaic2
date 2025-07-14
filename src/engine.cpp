/*******************************************************************************************
 *
 *   raylib [core] example - Basic window
 *
 *   Welcome to raylib!
 *
 *   To test examples, just press F6 and execute 'raylib_compile_execute' script
 *   Note that compiled executable is placed in the same folder as .c file
 *
 *   To test the examples on Web, press F6 and execute 'raylib_compile_execute_web' script
 *   Web version of the program is generated in the same folder as .c file
 *
 *   You can find all basic examples on C:\raylib\raylib\examples folder or
 *   raylib official webpage: www.raylib.com
 *
 *   Enjoy using raylib. :)
 *
 *   Example originally created with raylib 1.0, last time updated with raylib 1.0
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
 *   BSD-like license that allows static linking with closed source software
 *
 *   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h> 
#include <string.h>
#include <cassert>

#include <math.h>

#include <time.h>


#define ASSERT(...) assert(__VA_ARGS__)


#include "types.h"



float32 Time = 0;
float32 DeltaTime = 0;


#define PRINT_MAX_BUFFER_LEN 1024

void Print(const char *fmt, ...) {
  char buffer[PRINT_MAX_BUFFER_LEN];

  va_list args;
  va_start (args, fmt);


  vsnprintf(buffer, PRINT_MAX_BUFFER_LEN, fmt, args);

  printf("%s", buffer);
  printf("\n");

  va_end(args);
}


#include "memory.h"
#include "dynamic_array.h"

#include "math/math.h"

#include "common.h"

#include "raylib.h"

#include "input.h"
#include "input.cpp"

#include "mosaic.cpp"


struct PlatformMem {
  int32 screenWidth;
  int32 screenHeight;
};

PlatformMem Platform = {};

void MosaicInit();
void MosaicUpdate();


struct EngineMem {
  InputDevice *keyboard;
  InputDevice *mouse;

  vec2 mousePosition;
  vec2 mousePositionNorm;

  MemoryArena arena;

  InputManager input;
};

InputDevice *Keyboard = NULL;
InputDevice *Mouse = NULL;

EngineMem Engine = {};

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{

  // Initialization
  //--------------------------------------------------------------------------------------
  Platform.screenWidth = 1600;
  Platform.screenHeight = 900;

  InitWindow(Platform.screenWidth, Platform.screenHeight, "raylib [core] example - basic window");

  AllocateMemoryArena(&Engine.arena, Megabytes(1));
  
  {
    InputManager *input = &Engine.input;

    // 4 controllers + mouse & keyboard
    AllocateInputManager(input, &Engine.arena, 32, 6);

    AllocateInputDevice(&input->devices[0], InputDeviceType_Keyboard, Input_KeyCount, 0);
    AllocateInputDevice(&input->devices[1], InputDeviceType_Mouse,
                        Input_MouseDiscreteCount, Input_MouseAnalogueCount);

    Engine.keyboard = &input->devices[0];
    Engine.mouse = &input->devices[1];

    Keyboard = Engine.keyboard;
    Mouse = Engine.mouse;

    
    SetMousePosition(Platform.screenWidth / 2,
                     Platform.screenHeight / 2);
  }

  SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------
    
  Camera2D camera = { 0 };
  camera.target.x = 0;
  camera.target.y = 0;
  camera.offset.x = Platform.screenWidth/2.0f;
  camera.offset.y = Platform.screenHeight/2.0f;;
  camera.rotation = 0.0f;
  camera.zoom = 16.0f;

    
  MosaicInternalInit();
    
  // this way its the same size as the default font. Kinda yucky but whatever.
  Mosaic->tileSize = 10;
    
  // @TODO: maybe I should just change the tile size to be 10 by default so that 
    
  //testTexture = LoadTexture("data/test.png");        // Texture loading


  SetMosaicGridSize(9, 9);
  Mosaic->padding = 4;

  //SeedRand(1298743);
  SeedRand(time(NULL));

  MosaicInit();

  // Main game loop
  while (!WindowShouldClose()) {
    Time = GetTime();
    DeltaTime = GetFrameTime();

    {
      InputManager *input = &Engine.input;
      DynamicArrayClear(&input->events);
    
      RaylibPushKeyboardEvents(input, Engine.keyboard);
      RaylibPushMouseEvents(input, Engine.mouse);

      Engine.mousePosition.x = GetMouseX();
      Engine.mousePosition.y = GetMouseX();

      Engine.mousePositionNorm.x = Engine.mousePosition.x / (1.0f * Platform.screenWidth);
      Engine.mousePositionNorm.y = Engine.mousePosition.y / (1.0f * Platform.screenHeight);

      InputManagerUpdate(input);
    }
        
    // @HACK: just do this when the grid resizes in Mosaic     
    {
      float32 levelAspect = Mosaic->gridWidth / Mosaic->gridHeight;
      float32 screenAspect = Platform.screenWidth / (1.0f * Platform.screenHeight);
        
      float32 size = 1;
        
      if (levelAspect > screenAspect) {
        size = Mosaic->gridWidth / (16.0f - Mosaic->padding);
           
        size = Platform.screenWidth / (1.0f * (Mosaic->gridWidth + (2 * Mosaic->padding)));

      }
      else {
        size = Mosaic->gridHeight / (9.0f - Mosaic->padding);
           
        size = Platform.screenHeight / (1.0f * (Mosaic->gridHeight + (2 * Mosaic->padding)));

      }
        
      camera.zoom = size / Mosaic->tileSize;
    }
    


    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update your variables here
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(BLACK);

    //DrawText("MOSAIC", 190, 200, 40, WHITE);
        

    MosaicInternalUpdate();
    MosaicUpdate();
        

    // @TODO: Mosaic should be accumulating a list of draw text commands 
    // which we iterate and call the correct stuff for
        
    BeginMode2D(camera);

    BeginBlendMode(1);

    {
      MTile *tiles = Mosaic->tiles;
      vec2 pos;
        
      for (int32 i = 0; i < Mosaic->tileCapacity; i++) {
        MTile *tile = &Mosaic->tiles[i];
          
        DrawTile(tile->position, tile->color);

#if 1
        if (tile->sprite != NULL) {
          Texture2D *sprite = tile->sprite;
          pos = GridPositionToWorldPosition(tile->position);

          float32 scale = 1.0f + ((1 + sinf(Time + (i * 0.5f))) / 2);

          Rectangle src = {};
          src.x = 0;
          src.y = 0;
          src.width = sprite->width ;
          src.height = sprite->height;

          Rectangle dest = {};
          dest.width = Mosaic->tileSize * scale;
          dest.height = Mosaic->tileSize * scale;

          vec2 origin = V2(dest.width / 2, dest.height / 2);

          vec2 centerOffset = V2(Mosaic->tileSize / 2, Mosaic->tileSize / 2);
          
          dest.x = pos.x + centerOffset.x;
          dest.y = pos.y + centerOffset.y;

          //Print("origin %f %f", origin.x, origin.y);
                
          DrawTexturePro(*sprite, src, dest, Vector2{origin.x, origin.y}, 10 * Time + i, RED);

          //DrawCircle(pos.x, pos.y, 1, WHITE);

          // @BUG: 
          //DrawCircle(dest.x, dest.y, 1, WHITE);
        }
#endif
      }
    }

    EndBlendMode();

    //DrawTile(V2i(0, 0), V3(1, 1, 1));
    //DrawTile(V2i(Mosaic->gridWidth - 1, 0), V3(0, 0, 1));
    //DrawTile(V2i(Mosaic->gridWidth - 1, Mosaic->gridHeight - 1), V3(1, 0, 0));

        
    //printf("orig %d %d\n", Mosaic->gridOrigin.x, Mosaic->gridOrigin.y);

    //SetTileColor();
        
    // hmm frustrating that a text size of 1 isn't the same as a tile...
    // looks like the default size is 10, and since we just take an integer gonna need to 
    // generate a new font...
    //DrawText("MOSAIC", 0, 0, 40, WHITE);
       
    //DrawRectangle(0, 0, 50, 50, GRAY);
        
    //DrawRectangle(-300, -250, 50, 50, GRAY);

    // DrawCircle(Mosaic->gridOrigin.x, Mosaic->gridOrigin.y, 1, WHITE);
    // DrawCircle(0, 0, 1, BLACK);

#if 0
    {
      vec2 pos = GridPositionToWorldPosition(V2i(0, 0));
      //DrawTextureEx(testTexture, {pos.x, pos.y}, 0, 0.5f, WHITE);
                
      pos = GridPositionToWorldPosition(V2i(4, 4));

      Rectangle src = {};
      src.x = 0;
      src.y = 0;
      src.width = testTexture.width;
      src.height = testTexture.height;
                
      Rectangle dest = {};
      dest.x = pos.x;
      dest.y = pos.y;
      dest.width = Mosaic->tileSize;
      dest.height = Mosaic->tileSize;
                
      DrawTexturePro(testTexture, src, dest, Vector2{0, 0}, 0.0, WHITE);
    }
#endif
        

        
    EndMode2D();
        
                
    DrawFPS(10, 10);


    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();        // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
