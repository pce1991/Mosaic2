
#include "../engine.cpp"

MemoryArena Arena = {};

struct Ball {
  vec2 position;
  vec2 velocity;

  float32 radius;

  vec3 color;

  int32 health;
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
  int32 score;
  
  float32 spawnTimer;

  int32 lastScoreSpawned;
  
  Paddle paddles[2];

  DynamicArray<Ball> balls;
};

GameMem Game = {};

Texture2D testTexture = {};

Texture2D bokeh = {};

struct TileData {
  vec3 color;
  float32 rotation;
  float32 scale;
};

DynamicArray<TileData> backgroundTileData = {};

void SpawnBall(vec2 position, vec2 velocity) {
  Ball ball = {};
  ball.color = V3(0.2f, 0.8f, 0.5f);

  ball.position = position;
  ball.velocity = velocity;

  ball.radius = 2.5;

  ball.health = 3;

  PushBack(&Game.balls, ball);
}

void MosaicInit() {
  testTexture = LoadTexture("data/glube.png");
  bokeh = LoadTexture("data/textures/bokeh/waves_alpha2.png");
  
  SetMosaicGridSize(80, 120);
  //SetMosaicGridSize(8, 8);
  //Mosaic->padding = 2;

  // @WARNING: we dont have that much memory for web builds so
  // allolcating 64MB is no bueno.
  AllocateMemoryArena(&Arena, Megabytes(8));

  backgroundTileData = MakeDynamicArray<TileData>(&Arena, Mosaic->tileCapacity);

  for (int y = 0; y < Mosaic->gridHeight; y++) {
    for (int x = 0; x < Mosaic->gridWidth; x++) {
      TileData data = {};
      data.scale = RandfRange(0.0f, 0.25f);
      data.rotation = RandfRange(0, 360);

      vec3 hsv = {
        .x = 32,
        .y = RandfRange(0.5f, 0.7f),
        .z = RandfRange(0.1f, 0.4f),
      };
      data.color = HSVToRGB(hsv);

      PushBack(&backgroundTileData, data);
    }
  }

  Game.balls = MakeDynamicArray<Ball>(&Arena, 16);

  SpawnBall(V2(40, 80), V2(0, -30));

  {
    Game.paddles[0] = {
      .width = 11,
      .height = 3,
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

Color Vec4ToColor(vec4 v) {
  Color c = {};
  c.r = v.r * 255;
  c.b = v.b * 255;
  c.g = v.g * 255;
  c.a = v.a * 255;

  return c;
}

void DrawTextf(vec2 pos, float32 size, vec4 color, const char *fmt, ...) {
  va_list args;
  va_start (args, fmt);

  char str[1024];
  vsnprintf(str, 1024, fmt, args);

  vec2 floorPos = V2(floorf(pos.x), -floorf(pos.y));
    
  vec2 position = Mosaic->gridOrigin + floorPos + V2(0.0f, -1.0f);

  Color c = Vec4ToColor(color);
          
  DrawText(str, pos.x, pos.y, size, c);

  va_end(args);
}


void MosaicUpdate() {

  Game.spawnTimer += DeltaTime;

//   float32 ballSpawnRate = 8.0f;
// if (Game.spawnTimer >= ballSpawnRate && Game.balls.count < 10) {
//     SpawnBall(V2(40, 120), V2(RandfRange(-10, 10), -40));
    
//     Game.spawnTimer -= ballSpawnRate;
//   }

  if (Game.balls.count == 0) {
    SpawnBall(V2(40, 120), V2(RandfRange(-10, 10), -40));
  }

  {
    int32 scoreDelta = Game.score - Game.lastScoreSpawned;

    if (scoreDelta >= 5) {
      SpawnBall(V2(40, 120), V2(RandfRange(-10, 10), -40));
      Game.lastScoreSpawned = Game.score;
    }
  }

  
  if (InputHeld(Keyboard, Input_Space)) {
    SpawnBall(V2(40, 120), V2(RandfRange(-10, 10), -40));
      
    //DynamicArrayClear(&Game.balls);
  }

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

    // HACK
    if (IsKeyDown(KEY_LEFT)) {
      moveDirection = -1;
    }
    else if (IsKeyDown(KEY_RIGHT)) {
      moveDirection = 1;
    }


    float32 maxSpeed = 60;
    float32 accel = 500;
    
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
    float32 maxSpeed = 65;
      
    for (int i = 0; i < Game.balls.count; i++) {
      Ball *ball = &Game.balls[i];

      ball->velocity = ClampLength(ball->velocity, maxSpeed);

      if (Absf(ball->velocity.y) < 1.0f) {
        ball->velocity.y = 10;
      }

      ball->position = ball->position + ball->velocity * DeltaTime;
    }
  }

  // collision against paddles
  {
    Paddle *paddle = &Game.paddles[0];
    vec2 paddleMin = paddle->position;
    vec2 paddleMax = paddle->position + V2(paddle->width, paddle->height);
    
    for (int i = 0; i < Game.balls.count; i++) {
      Ball *ball = &Game.balls[i];

      // vec2 ballMin = ball->position;
      // vec2 ballMax = ball->position + V2(1, 1);

      vec2 ballMin = ball->position - V2(ball->radius);
      vec2 ballMax = ball->position + V2(ball->radius);

      vec2 ballCenter = Lerp(ballMin, ballMax, 0.5f);


      // @TODO: we need to check the position of the ball at the start of this frame!
      // Ray2D velRay;
      // velRay.origin = ballCenter;
      // velRay.direction = ball->velocity;

      // float32 tHit;
      // if (RaycastAABB(paddleMin, paddleMax, velRay, &tHit)) {
        
      // }

      vec2 dir = V2(0);
      if (TestAABBAABB(ballMin, ballMax, paddleMin, paddleMax, &dir)) {
        ball->position = ball->position + dir;

        ball->velocity.y *= -1;

        float32 swipeScale = 1.1f;
        ball->velocity.x += paddle->velocity.x * swipeScale;

        //ball->health--;

        Game.score++;

        // if (ball->health == 2) {
        //   vec3 hsv = {
        //     .x = 32,
        //     .y = RandfRange(0.5f, 0.7f),
        //     .z = RandfRange(0.3f, 0.6f),
        //   };
          
        //   ball->color = HSVToRGB(hsv);
        // }
        // else if (ball->health == 1) {
        //   ball->color = V3(0.7f, 0.3f, 0.3f);
        // }
      }
    }
  }

  // collision against walls
  {
    for (int i = 0; i < Game.balls.count; i++) {
      Ball *ball = &Game.balls[i];

      vec2 ballMin = ball->position;
      vec2 ballMax = ball->position + V2(1, 1);

      if (ballMin.x < 0) {
        ball->position.x = 0;
        ball->velocity.x *= -1;
      }
      if (ballMax.x >= Mosaic->gridWidth) {
        ball->position.x = Mosaic->gridWidth - 1;;
        ball->velocity.x *= -1;
      }
      
      if (ballMin.y < 0) {
        ball->position.y = 0;
        ball->velocity.y *= -1;

        Game.score -= 5;
        Game.score = Max(Game.score, 0);

        ball->health = 0;
      }
      if (ballMax.y >= Mosaic->gridHeight) {
        ball->position.y = Mosaic->gridHeight - 1;;
        ball->velocity.y *= -1;
      }
    }
  }

  

  // @TODO: make an easy function for this for clamping an AABB to a bounds
  {
    Paddle *paddle = &Game.paddles[0];
    vec2 paddleMin = paddle->position;
    vec2 paddleMax = paddle->position + V2(paddle->width, paddle->height);

    if (paddleMin.x < 0) {
      paddle->position.x = 0;
    }
    if (paddleMax.x >= Mosaic->gridWidth) {
      paddle->position.x = Mosaic->gridWidth - paddle->width;
    }
  }

  {
    for (int i = Game.balls.count - 1; i >= 0; i--) {
      Ball *ball = &Game.balls[i];
      if (ball->health <= 0) {
        RemoveAtIndex(&Game.balls, i);
      }
    }
  }

  // RENDER
  {
    int32 index = 0;
    for (int y = 0; y < Mosaic->gridHeight; y++) {
      for (int x = 0; x < Mosaic->gridWidth; x++) {
        TileData data = backgroundTileData[index];

        float32 scale = (1.0f + (((1 + sinf(Time + (index * 0.5f))) / 2) * 1.25f)) + data.scale;
        float32 rotation = 10 * (Time + index) + data.rotation;

        //SetTileTint(x, y, 0.2f, 0.2f, 0.2f);
        SetTileTint(x, y, data.color.r, data.color.g, data.color.b);
        SetTileSprite(x, y, &bokeh);

        SetTileScale(x, y, scale);

        index++;
      }
    }
  }

  DrawPaddle(Game.paddles[0]);

  {
    for (int i = 0; i < Game.balls.count; i++) {
      Ball *ball = &Game.balls[i];

      float32 radius = ball->radius;
      float32 radiusSq = ball->radius * ball->radius;

      int32 index = 0;
      
      for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {

        vec2 position = ball->position + V2(x, y);

        float32 distSq = DistanceSq(position, ball->position);

        if (distSq > radiusSq) {
          continue;
        }

        float32 scale = (2.0 + (((1 + sinf(4 * Time + (index * 0.5f))) / 2) * 1.0f));

        SetTileTint(position.x, position.y, ball->color.r, ball->color.g, ball->color.b);
        SetTileSprite(position.x, position.y, &bokeh);
        SetTileScale(position.x, position.y, scale);
        SetTileRotation(position.x, position.y, (Time + i) * 30);

        index++;
        }
      }
    }
  }

  DrawTextf(V2((Mosaic->tileSize * Mosaic->gridWidth * 0.5f) + 20, 0), 80, V4(1, 1, 1, 1),
            "SCORE %d", Game.score);
  //DrawText("MOSAIC", 0, 0, 40, WHITE);
  //DrawText("MOSAIC", Mosaic->tileSize * Mosaic->gridWidth, 0, 40, WHITE);
  //DrawText("MOSAIC", Mosaic->tileSize * Mosaic->gridWidth, 0, 40, WHITE);
}
