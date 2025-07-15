
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

void SpawnBall(vec2 pos, float32 radius) {
  // vec2 pos = V2(RandfRange(30, 60),
  //               RandfRange(30, 40));

  // float32 radius = RandfRange(6.0f, 12.0f);
  float32 radiusSq = radius * radius;

  for (int y = -radius; y <= radius; y++) {
    for (int x = -radius; x <= radius; x++) {
      Ball ball = {};
        
      ball.position.x = pos.x + x;
      ball.position.y = pos.y + y;

      float32 distSq = DistanceSq(pos, ball.position);

      if (distSq > radiusSq) {
        continue;
      }
      
      vec3 hsv = {
        .x = 32,
        .y = RandfRange(0.5f, 0.7f),
        .z = RandfRange(0.3f, 0.6f),
      };

      ball.color = HSVToRGB(hsv);

      PushBack(&balls, ball);
    }
  }
}

void MosaicInit() {
  testTexture = LoadTexture("data/glube.png");
  bokeh = LoadTexture("data/textures/bokeh/waves_alpha2.png");
  
  SetMosaicGridSize(80, 45);
  //SetMosaicGridSize(8, 8);
  //Mosaic->padding = 2;

  // @WARNING: we dont have that much memory for web builds so
  // allolcating 64MB is no bueno.
  AllocateMemoryArena(&Arena, Megabytes(8));

  balls = MakeDynamicArray<Ball>(&Arena, 256);

  for (int y = 0; y < Mosaic->gridHeight; y++) {
    for (int x = 0; x < Mosaic->gridWidth; x++) {
      Ball ball = {};

      ball.position.x = x;
      ball.position.y = y;
      
      vec3 hsv = {
        //.h = RandfRange(0.0f, 360.0f),
        .x = 206,
        .y = RandfRange(0.3f, 0.8f),
        .z = RandfRange(0.4f, 0.5f),
      };

      ball.color = HSVToRGB(hsv);

      PushBack(&balls, ball);
    }
  }

  {
    vec2 pos = V2(RandfRange(30, 60),
                  RandfRange(30, 40));

    float32 radius = RandfRange(6.0f, 8.0f);

    SpawnBall(pos, radius);

    SpawnBall(pos + V2(radius * 0.5f, -radius * 0.25f), radius);
    SpawnBall(pos + V2(radius * 1.5f, radius * 0.7f), radius);
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

      ball.velocity.x = RandfRange(1, 8);
      ball.velocity.y = RandfRange(1, 8);


      vec3 hsv = {
        //.h = RandfRange(0.0f, 360.0f),
        //.x = RandfRange(128.0f, 206.0f),
        //.x = RandfRange(0.0f, 60.0f),
        .x = RandfRange(200, 280),
        .y = RandfRange(0.4f, 1.0f),
        .z = RandfRange(0.5f, 1.0f),
      };

      ball.color = HSVToRGB(hsv);
#endif

      Print("%f %f", ball.position.x, ball.position.y);

      PushBack(&balls, ball);
    }
  }
  
  for (int i = 0; i < balls.count; i++) {
    Ball *ball = &balls[i];
    ball->position = ball->position + ball->velocity * DeltaTime;
  }

  // RENDER
  {
    int32 index = 0;
    
    for (int y = 0; y < Mosaic->gridHeight; y++) {
      for (int x = 0; x < Mosaic->gridWidth; x++) {
        float32 r = x / (Mosaic->gridWidth * 1.0f);
        float32 g = (1 + sinf(Time)) * 0.5f;
        float32 b = y / (Mosaic->gridHeight * 1.0f);

        // @TODO: use perlin noise!

        float32 scale = 1.0f + (((1 + sinf(Time + (index * 0.5f))) / 2) * 1.5f);
        float32 rotation = 10 * Time + index;

        SetTileRotation(x, y, rotation);
        SetTileScale(x, y, scale);

        index++;
      }
    }
  }

  for (int i = 0; i < balls.count; i++) {
    Ball ball = balls[i];
    //SetTileColor(ball.position.x, ball.position.y, 0.8f, 0.4f, 0.6f);
    
    // SetTileSprite(ball.position.x, ball.position.y, &testTexture);

    //SetTileColor(ball.position.x, ball.position.y, ball.color.r, ball.color.g, ball.color.b);
    
    SetTileTint(ball.position.x, ball.position.y, ball.color.r, ball.color.g, ball.color.b);
    SetTileSprite(ball.position.x, ball.position.y, &bokeh);
  }
        
  //SetTileColor(10, 10, 0.8f, 0.4f, 0.6f);
}
