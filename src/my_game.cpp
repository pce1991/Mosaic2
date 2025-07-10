
#include "engine.cpp"

MemoryArena Arena = {};

struct Ball {
  vec2 position;
  vec2 velocity;
};

DynamicArray<Ball> balls = {};

void MosaicInit() {  
  SetMosaicGridSize(9, 9);
  //Mosaic->padding = 2;

  AllocateMemoryArena(&Arena, Megabytes(64));

  balls = MakeDynamicArray<Ball>(&Arena, 16);

  for (int i = 0; i < 16; i++) {
    Ball ball = {};

    // ball.position.x = GetRandomValue(0, 9);
    // ball.position.y = GetRandomValue(0, 9);
    
#if 1
    ball.position.x = RandfRange(0, 9);
    ball.position.y = RandfRange(0, 9);

    ball.velocity.x = RandfRange(0, 9);
    ball.velocity.y = RandfRange(0, 9);
#endif

    Print("%f %f", ball.position.x, ball.position.y);

    PushBack(&balls, ball);
  }
}

void MosaicUpdate() {
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
    SetTileColor(ball.position.x, ball.position.y, 0.8f, 0.4f, 0.6f);
  }
        
  //SetTileColor(10, 10, 0.8f, 0.4f, 0.6f);
}
