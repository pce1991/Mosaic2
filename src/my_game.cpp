
#include "engine.cpp"

MemoryArena Arena = {};

struct Ball {
  vec2 position;
  vec2 velocity;

  vec3 color;
};

DynamicArray<Ball> balls = {};

void MosaicInit() {  
  SetMosaicGridSize(192, 128);
  //Mosaic->padding = 2;

  // @WARNING: we dont have that much memory for web builds so
  // allolcating 64MB is no bueno.
  AllocateMemoryArena(&Arena, Megabytes(8));

  balls = MakeDynamicArray<Ball>(&Arena, 256);

  
}

void MosaicUpdate() {

  if (IsKeyPressed(KEY_SPACE)) {
    for (int i = 0; i < 8; i++) {
      Ball ball = {};

      // ball.position.x = GetRandomValue(0, 9);
      // ball.position.y = GetRandomValue(0, 9);
    
#if 1
      ball.position.x = RandfRange(0, 9);
      ball.position.y = RandfRange(0, 9);

      ball.velocity.x = RandfRange(1, 15);
      ball.velocity.y = RandfRange(1, 15);


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
  }
        
  //SetTileColor(10, 10, 0.8f, 0.4f, 0.6f);
}
