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
#include "common.h"

#include "math/math.h"

#include "dynamic_array.h"

#include "string.h"

#include "raylib.h"

#include "input.h"
#include "input.cpp"

#include "mosaic.cpp"

#include "rlgl.h"    // Required for rlBegin, rlSetTexture, rlVertex2f, etc.

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
  InputDevice *gamepads[4];

  vec2 mousePosition;
  vec2 mousePositionNorm;

  MemoryArena arena;
  MemoryArena frameMem;

  InputManager input;

  Image perlinNoise;
};

InputDevice *Keyboard = NULL;
InputDevice *Mouse = NULL;
InputDevice *Gamepads[4];
InputDevice *Gamepad = NULL;

EngineMem Engine = {};

MemoryArena *FrameMem = NULL;

struct SpriteBatchData {
  vec2 position;
  float32 rotation;
  float32 scale;

  Color tint;

  Rectangle srcRect;
};

void TextureBatchRender(SpriteBatchData *sprites, int32 spriteCount, Texture texture) {
  rlSetTexture(0);
  rlSetTexture(texture.id);
  rlBegin(RL_QUADS);

  for (int i = 0; i < spriteCount; i++) {
    vec2 pos = sprites[i].position;
    float rotation = sprites[i].rotation;
    float scale = sprites[i].scale;
    Color tint = sprites[i].tint;

    float size = 10.0f * scale;  // <- desired quad width in world units
    float halfSize = size / 2.0f;

    // Maintain texture aspect ratio
    float aspect = (float)texture.height / (float)texture.width;
    float halfWidth = halfSize;
    float halfHeight = halfSize * aspect;

    float cosr = cosf(rotation);
    float sinr = sinf(rotation);

    // ccw winding
    Vector2 corners[4] = {
      { -halfWidth,  halfHeight },  // Bottom-left
      {  halfWidth,  halfHeight },  // Bottom-right
      {  halfWidth, -halfHeight },  // Top-right
      { -halfWidth, -halfHeight }   // Top-left
    };

    Vector2 uvs[4] = {
      { 0.0f, 1.0f },  // Bottom-left
      { 1.0f, 1.0f },  // Bottom-right
      { 1.0f, 0.0f },  // Top-right
      { 0.0f, 0.0f }   // Top-left
    };

    for (int j = 0; j < 4; j++) {
      float x = corners[j].x;
      float y = corners[j].y;

      float tx = pos.x + x * cosr - y * sinr;
      float ty = pos.y + x * sinr + y * cosr;

      rlColor4ub(tint.r, tint.g, tint.b, tint.a);
      rlTexCoord2f(uvs[j].x, uvs[j].y);
      rlVertex2f(tx, ty);
    }
  }

  rlEnd();
  rlSetTexture(0);
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{

  // Initialization
  //--------------------------------------------------------------------------------------
  Platform.screenWidth = 3200;
  Platform.screenHeight = 1800;

  InitWindow(Platform.screenWidth, Platform.screenHeight, "raylib [core] example - basic window");

  AllocateMemoryArena(&Engine.arena, Megabytes(1));
  AllocateMemoryArena(&Engine.frameMem, Megabytes(1));

  FrameMem = &Engine.frameMem;

  char *workingDirectory = (char *)GetWorkingDirectory();

  if (StringEquals(workingDirectory, "build")) {
    ChangeDirectory("..");
  }

  Print("WORKING DIRECTORY %s", workingDirectory);
  
  {
    InputManager *input = &Engine.input;

    // 4 controllers + mouse & keyboard
    AllocateInputManager(input, &Engine.arena, 32, 6);

    AllocateInputDevice(&input->devices[0], InputDeviceType_Keyboard, Input_KeyCount, 0, -1);
    AllocateInputDevice(&input->devices[1], InputDeviceType_Mouse,
                        Input_MouseDiscreteCount, Input_MouseAnalogueCount, -1);

    AllocateInputDevice(&input->devices[2], InputDeviceType_Gamepad,
                        Input_GamepadButtonCount, Input_GamepadAnalogueCount, 0);

    AllocateInputDevice(&input->devices[3], InputDeviceType_Gamepad,
                        Input_GamepadButtonCount, Input_GamepadAnalogueCount, 1);

    AllocateInputDevice(&input->devices[4], InputDeviceType_Gamepad,
                        Input_GamepadButtonCount, Input_GamepadAnalogueCount, 2);

    AllocateInputDevice(&input->devices[5], InputDeviceType_Gamepad,
                        Input_GamepadButtonCount, Input_GamepadAnalogueCount, 3);

    Engine.keyboard = &input->devices[0];
    Engine.mouse = &input->devices[1];

    Engine.gamepads[0] = &input->devices[2];
    Engine.gamepads[1] = &input->devices[3];
    Engine.gamepads[2] = &input->devices[4];
    Engine.gamepads[3] = &input->devices[5];

    Keyboard = Engine.keyboard;
    Mouse = Engine.mouse;

    Gamepads[0] = Engine.gamepads[0];
    Gamepads[1] = Engine.gamepads[1];
    Gamepads[2] = Engine.gamepads[2];
    Gamepads[3] = Engine.gamepads[3];

    Gamepad = Gamepads[0];
    
    SetMousePosition(Platform.screenWidth / 2,
                     Platform.screenHeight / 2);
  }

  Engine.perlinNoise= GenImagePerlinNoise(Platform.screenWidth, Platform.screenHeight, 50, 50, 4.0f);

  //SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
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

    if (DeltaTime >= 1 / 60.0f) {
      DeltaTime = 1.0f / 60.0f;
    }

    {
      InputManager *input = &Engine.input;
      DynamicArrayClear(&input->events);
    
      RaylibPushKeyboardEvents(input, Engine.keyboard);
      RaylibPushMouseEvents(input, Engine.mouse);

      for (int i = 0; i < 4; i++) {
        RaylibPushGamepadEvents(input, Engine.gamepads[i]);
      }
      

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
        
    BeginMode2D(camera);

    BeginBlendMode(1);

    MosaicInternalUpdate();
    MosaicUpdate();
        

    // @TODO: Mosaic should be accumulating a list of draw text commands 
    // which we iterate and call the correct stuff for

    
    for (int32 i = 0; i < Mosaic->tileCapacity; i++) {
      MTile *tile = &Mosaic->tiles[i];
      DrawTile(tile->position, tile->color);
    }

#if 0
    {
      MTile *tiles = Mosaic->tiles;
      vec2 pos;
        
      for (int32 i = 0; i < Mosaic->tileCapacity; i++) {
        MTile *tile = &Mosaic->tiles[i];

        if (tile->sprite != NULL) {
          Texture2D *sprite = tile->sprite;
          pos = GridPositionToWorldPosition(tile->position);

          float32 scale = tile->scale; 

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
                
          //DrawTexturePro(*sprite, src, dest, Vector2{origin.x, origin.y}, 10 * Time + i, BLUE);

          Color c = {};
          c.r = tile->tint.r * 255;
          c.b = tile->tint.b * 255;
          c.g = tile->tint.g * 255;
          c.a = tile->tint.a * 255;

          //DrawTexturePro(*sprite, src, dest, Vector2{origin.x, origin.y}, 10 * Time + i, BLUE);
          DrawTexturePro(*sprite, src, dest, Vector2{origin.x, origin.y}, tile->rotation, c);

          //DrawCircle(pos.x, pos.y, 1, WHITE);

          // @BUG: 
          //DrawCircle(dest.x, dest.y, 1, WHITE);
        }
      }
    }
#else

#if 1
    // @TODO: sort thru all the sprites used and batch accordingly.
    // @TODO: support the ability to sample from the textures, which means we've got to
    // remap our uv coordinates!
    SpriteBatchData *sprites = PushArray(FrameMem, SpriteBatchData, Mosaic->tileCapacity);

    Texture *texture = NULL;
    for (int32 i = 0; i < Mosaic->tileCapacity; i++) {
      MTile *tile = &Mosaic->tiles[i];

      if (texture == NULL) {
        // @HACK: really this should probably all be baked into one big atlas
        texture = tile->sprite;
      }

      vec2 position = GridPositionToWorldPosition(tile->position);

      SpriteBatchData *sprite = &sprites[i];
      sprite->position = position;
      sprite->rotation = tile->rotation;
      sprite->scale = tile->scale;

      Color c = {};
      c.r = tile->tint.r * 255;
      c.b = tile->tint.b * 255;
      c.g = tile->tint.g * 255;
      c.a = tile->tint.a * 255;

      sprite->tint = c;

      Rectangle src = {};
      src.x = 0;
      src.y = 0;
      src.width = texture->width;
      src.height = texture->height;

      sprite->srcRect = src;
    }

    if (texture != NULL) {
      TextureBatchRender(sprites, Mosaic->tileCapacity, *texture);
    }
#endif
    
#endif
    

    EndBlendMode();

    //DrawText("MOSAIC", Mosaic->tileSize * Mosaic->gridWidth, 0, 40, WHITE);
        
    EndMode2D();
        
                
    DrawFPS(10, 10);


    EndDrawing();
    //----------------------------------------------------------------------------------

    ClearMemoryArena(&Engine.frameMem);
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();        // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
