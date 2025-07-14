
#include "engine.cpp"

MemoryArena Arena = {};

struct Ball {
  vec2 position;
  vec2 velocity;

  vec3 color;
};

DynamicArray<Ball> balls = {};


Texture2D testTexture = {};

Texture2D bokeh = {};

void MosaicInit() {
  testTexture = LoadTexture("data/glube.png");
  bokeh = LoadTexture("data/textures/bokeh/waves_alpha.png");
  
  //SetMosaicGridSize(32, 32);
  SetMosaicGridSize(8, 8);
  //Mosaic->padding = 2;

  // @WARNING: we dont have that much memory for web builds so
  // allolcating 64MB is no bueno.
  AllocateMemoryArena(&Arena, Megabytes(8));

  balls = MakeDynamicArray<Ball>(&Arena, 256);

  for (int i = 0; i < 80; i++) {
      Ball ball = {};

      // ball.position.x = GetRandomValue(0, 9);
      // ball.position.y = GetRandomValue(0, 9);

      ball.position.x = RandfRange(0, 9);
      ball.position.y = RandfRange(0, 9);

      PushBack(&balls, ball);
  }
}

void MosaicUpdate() {

  if (InputPressed(Engine.keyboard, Input_Space) ||
      InputPressed(Engine.mouse, Input_MouseLeft)
      //Engine.mousePositionNorm.x > 0.5f
      ) {
    for (int i = 0; i < 8; i++) {
      Ball ball = {};

      // ball.position.x = GetRandomValue(0, 9);
      // ball.position.y = GetRandomValue(0, 9);
    
#if 1
      ball.position.x = RandfRange(0, 9);
      ball.position.y = RandfRange(0, 9);

      // ball.velocity.x = RandfRange(1, 3);
      // ball.velocity.y = RandfRange(1, 3);


      vec3 hsv = {
        //.h = RandfRange(0.0f, 360.0f),
        .x = 128,
        .y = RandfRange(0.5f, 1.0f),
        .z = RandfRange(0.5f, 1.0f),
      };

      ball.color = HSVToRGB(hsv);
#endif

      Print("%f %f", ball.position.x, ball.position.y);

      PushBack(&balls, ball);
    }
  }
  
  for (int y = 0; y < Mosaic->gridHeight; y++) {
    for (int x = 0; x < Mosaic->gridWidth; x++) {
      float32 r = x / (Mosaic->gridWidth * 1.0f);
      float32 g = (1 + sinf(Time)) * 0.5f;
      float32 b = y / (Mosaic->gridHeight * 1.0f);
      SetTileColor(x, y, r, g, b);
    }
  }

  for (int i = 0; i < balls.count; i++) {
    Ball *ball = &balls[i];
    ball->position = ball->position + ball->velocity * DeltaTime;
  }

  for (int i = 0; i < balls.count; i++) {
    Ball ball = balls[i];
    //SetTileColor(ball.position.x, ball.position.y, 0.8f, 0.4f, 0.6f);
    SetTileColor(ball.position.x, ball.position.y, ball.color.r, ball.color.g, ball.color.b);
    //SetTileSprite(ball.position.x, ball.position.y, &testTexture);
    SetTileSprite(ball.position.x, ball.position.y, &bokeh);
  }
        
  //SetTileColor(10, 10, 0.8f, 0.4f, 0.6f);
}
