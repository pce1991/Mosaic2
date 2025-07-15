
#include "../engine.cpp"

MemoryArena Arena = {};

struct Ball {
  vec2 position;
  vec2 velocity;

  vec3 color;
};

struct Paddle {
  int32 width;
  int32 height;

  // top left of paddle
  vec2 position;
  vec2 velocity;

  vec3 color;
};

struct GameMem {
  Paddle paddles[2];
};

GameMem Game = {};

DynamicArray<Ball> balls = {};


Texture2D testTexture = {};

Texture2D bokeh = {};

void SpawnBall(vec2 pos, float32 radius) {
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
  
  SetMosaicGridSize(60, 80);
  //SetMosaicGridSize(8, 8);
  //Mosaic->padding = 2;

  // @WARNING: we dont have that much memory for web builds so
  // allolcating 64MB is no bueno.
  AllocateMemoryArena(&Arena, Megabytes(8));

  balls = MakeDynamicArray<Ball>(&Arena, 256);

  {
    Game.paddles[0] = {
      .width = 7,
      .height = 2,
      .position = V2(22, 12),
      .color = V3(1, 1, 1)
    };
  }
}

void DrawPaddle(Paddle paddle) {
  vec2 pos = paddle.position;
  
  for (int y = 0; y < paddle.height; y++) {
    for (int x = 0; x < paddle.width; x++) {

      float32 scale = 1.25f + (((1 + sinf(Time + (x + y * 0.5f))) / 2) * 0.5f);

      SetTileTint(pos.x + x, pos.y + y, paddle.color.r, paddle.color.g, paddle.color.b);
      SetTileSprite(pos.x + x, pos.y + y, &bokeh);

      SetTileScale(pos.x + x, pos.y + y, scale);
    }
  }
}

void MosaicUpdate() {

  // update paddle movement
  {
    Paddle *paddle = &Game.paddles[0];
    
    int32 moveDirection = 0;
    if (InputHeld(Keyboard, Input_A)) {
      moveDirection = -1;
    }
    else if (InputHeld(Keyboard, Input_D)) {
      moveDirection = 1;
    }

    float32 maxSpeed = 40;
    float32 accel = 300;
    
    if (moveDirection != 0) {
      paddle->velocity.x += accel * moveDirection * DeltaTime;
      //paddle->velocity.y += accel * moveDirection * DeltaTime;
    }
    else {
      float32 decel = 5;
      //paddle->velocity = V2(0);
      paddle->velocity.x = Dampen(paddle->velocity.x, decel * DeltaTime);
    }

    paddle->velocity = Clamp(paddle->velocity, V2(-maxSpeed), V2(maxSpeed));

    paddle->position = paddle->position + (paddle->velocity * DeltaTime);
  }

  {
    int32 index = 0;
    for (int y = 0; y < Mosaic->gridHeight; y++) {
      for (int x = 0; x < Mosaic->gridWidth; x++) {
        // float32 r = x / (Mosaic->gridWidth * 1.0f);
        // float32 g = (1 + sinf(Time)) * 0.5f;
        // float32 b = y / (Mosaic->gridHeight * 1.0f);

        float32 scale = 1.0f + (((1 + sinf(Time + (index * 0.5f))) / 2) * 1.25f);

        float32 rotation = 10 * (Time + x * y);

        SetTileTint(x, y, 0.2f, 0.2f, 0.2f);
        SetTileSprite(x, y, &bokeh);

        SetTileScale(x, y, scale);

        index++;
      }
    }
  }

  DrawPaddle(Game.paddles[0]);
}
