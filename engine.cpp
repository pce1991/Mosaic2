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

#include <math.h>


#include "raylib.h"

#include "src/mosaic.cpp"

// I want to treat this like the platform layer. That means the user should never be
// opening this file. Can I use a macro simply to supply the included my_game file 
// and pass that in from the compiler like
// #include MY_GAME_FILE
//
// Obviously going to need a custom compilation script that knows to compile engine
// but can a specific game file. 

struct PlatformMem {
    int32 screenWidth;
    int32 screenHeight;
};

PlatformMem Platform = {};

void MosaicInit();
void MosaicUpdate();


float32 Time = 0;
float32 DeltaTime = 0;

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

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0, 0};
    camera.offset = (Vector2){ Platform.screenWidth/2.0f, Platform.screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 16.0f;

    
    MosaicInternalInit();
    
    // this way its the same size as the default font. Kinda yucky but whatever.
    Mosaic->tileSize = 10;
    
    // @TODO: maybe I should just change the tile size to be 10 by default so that 
    

    SetMosaicGridSize(32, 32);
    Mosaic->padding = 8;
    
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
    

MosaicInit();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        Time = GetTime();
        DeltaTime = GetFrameTime();

        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        //DrawText("MOSAIC", 190, 200, 40, WHITE);
        
        
        MosaicUpdate();
        

        // @TODO: Mosaic should be accumulating a list of draw text commands 
        // which we iterate and call the correct stuff for
        
       BeginMode2D(camera);

       {
        MTile *tiles = Mosaic->tiles;
        for (int32 i = 0; i < Mosaic->tileCapacity; i++) {
            DrawTile(tiles[i].position, tiles[i].color);
        }
       }

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