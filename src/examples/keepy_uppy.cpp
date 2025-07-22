
#include "../engine.cpp"

MemoryArena Arena = {};

struct Ball {
  vec2 position;
  vec2 velocity;

  float32 radius;

  vec3 color;

  int32 health;
};


struct Bumper {
  vec2 position;
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

  bool grounded;

  vec3 color;
};

struct GameMem {
  int32 score;
  
  float32 spawnTimer;

  int32 lastScoreSpawned;
  bool spawnedAtScore;
  
  Paddle paddles[2];

  DynamicArray<Ball> balls;
  DynamicArray<Bumper> bumpers;
};

GameMem Game = {};

Texture2D testTexture = {};

Texture2D bokeh = {};

struct TileData {
  vec3 color;
  float32 rotation;
  float32 scale;

  // since these are randomized values I want to cache them
  // so we can always set our targetColor to animate
  // towards the original value. 
  vec3 originalColor;
  float32 originalScale;

  vec3 targetColor;
  float32 targetScale;
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

void SpawnBumper(vec2 position) {
  Bumper bumper = {};
  bumper.color = V3(0.2f, 0.3f, 0.8f);

  bumper.position = position;

  bumper.radius = 4.0;

  bumper.health = 3;

  PushBack(&Game.bumpers, bumper);
}

float32 PaddleFloor = 12;

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
      data.scale = RandfRange(0.15f, 0.25f);
      //data.scale = 1.0f;
      data.rotation = RandfRange(0, 360);

      vec3 hsv = {
        .x = 32,
        .y = RandfRange(0.5f, 0.7f),
        .z = RandfRange(0.1f, 0.4f),
      };
      data.color = HSVToRGB(hsv);

      data.originalColor = data.color;
      data.originalScale = data.scale;

      PushBack(&backgroundTileData, data);
    }
  }

  Game.balls = MakeDynamicArray<Ball>(&Arena, 16);
  Game.bumpers = MakeDynamicArray<Bumper>(&Arena, 16);

  SpawnBall(V2(40, 80), V2(0, -30));

  SpawnBumper(V2(25, 65));
  SpawnBumper(V2(60, 50));

  {
    Game.paddles[0] = {
      .width = 13,
      .height = 3,
      .position = V2(22, PaddleFloor),
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
    SpawnBall(V2(40, 120), V2(RandfRange(-10, 10), -20));
  }

  {
    int32 nextBallScore = 5;
    bool canSpawn = true;

    if (Game.score < 5) {
      nextBallScore = 3;
    }
    else if (Game.score < 10) {
      nextBallScore = 10;
    }
    else if (Game.score < 15) {
      nextBallScore = 15;
    }
    else {
      //nextBallScore = 20;
      canSpawn = false;
    }

    // we check >= because we may have score multiple points on one
    // frame and don't want to miss our spawn.
    // @BUG: technically if we score enough points to spawn two balls
    // on one frame we'll only spawn one. 
    if (Game.score >= nextBallScore && canSpawn &&
        !Game.spawnedAtScore) {
      SpawnBall(V2(40, 120), V2(RandfRange(-10, 10), -20));
      Game.lastScoreSpawned = Game.score;
      Game.spawnedAtScore = true;
    }
  }

  
  
  // update paddle movement
  {
    Paddle *paddle = &Game.paddles[0];
    
    float32 moveDirection = 0;
    
    if (InputHeld(Keyboard, Input_A) ||
        InputHeld(Keyboard, Input_LeftArrow)) {
      moveDirection = -1;
    }
    else if (InputHeld(Keyboard, Input_D) ||
             InputHeld(Keyboard, Input_RightArrow)) {
      moveDirection = 1;
    }

    {
      float32 trigger = InputAnalogue(Gamepad, Input_RightTrigger);
      //Print("%f", trigger);
    }
    
    {
      float32 stick = InputAnalogue(Gamepad, Input_LeftStickX);

      //Print("stick value %f", stick);

      if (stick != 0) {
        moveDirection = stick;
      }
    }


    float32 maxVertSpeed = 60;
    float32 maxHorzSpeed = 80;
    float32 accel = 700;
    
    if (moveDirection != 0) {
      paddle->velocity.x += accel * moveDirection * DeltaTime;
      //paddle->velocity.y += accel * moveDirection * DeltaTime;
    }
    else {
      float32 decel = 5;
      //paddle->velocity = V2(0);
      paddle->velocity.x = Dampen(paddle->velocity.x, decel * DeltaTime);
    }

    if (InputPressed(Keyboard, Input_Space) ||
        InputPressed(Gamepad, Input_FaceA)) {

      // @TODO: maybe do this as different forces summing to
      // a velocity? 
      if (paddle->grounded) {
        paddle->velocity.y = 240;
        paddle->grounded = false;
      }
    }

    if (!paddle->grounded) {
      float32 gravity = 100;
      paddle->velocity.y += -gravity * DeltaTime;
    }

    paddle->velocity = Clamp(paddle->velocity, V2(-maxHorzSpeed, -maxVertSpeed), V2(maxHorzSpeed, maxVertSpeed));

    paddle->position = paddle->position + (paddle->velocity * DeltaTime);
    
    if (paddle->position.y <= PaddleFloor) {
      paddle->position.y = PaddleFloor;
      paddle->grounded = true;
    }
  }

  {
    float32 maxSpeed = 90;
    float32 gravity = 60;
      
    for (int i = 0; i < Game.balls.count; i++) {
      Ball *ball = &Game.balls[i];
      
      ball->velocity = ClampLength(ball->velocity, maxSpeed);

      ball->velocity.y += -gravity * DeltaTime;

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

      // @BUG: need to make sure that we get repeat score by
      // overlapping on multiple frames with the same ball. 
      vec2 dir = V2(0);
      if (TestAABBAABB(ballMin, ballMax, paddleMin, paddleMax, &dir)) {
        ball->position = ball->position + dir;

        ball->velocity.y *= -0.8f;

        float32 swipeScale = 1.5f;
        ball->velocity.x += paddle->velocity.x * swipeScale;

        // dont add the velocity if the paddle is moving down in same
        // direction as the ball.
        if (paddle->velocity.y > 0) {
          float32 bounceScale = 40.5f;
          ball->velocity.y += paddle->velocity.y * bounceScale;
        }

        Game.score++;
        Game.spawnedAtScore = false;
      }
    }
  }

  // collisions against bumpers
  {
    for (int i = 0; i < Game.balls.count; i++) {
      Ball *ball = &Game.balls[i];

      float32 ballSpeed = Length(ball->velocity);

      for (int j = 0; j < Game.bumpers.count; j++) {
        Bumper *bumper = &Game.bumpers[j];

        if (TestCircleCircle(ball->position, ball->radius,
                             bumper->position, bumper->radius)) {
          vec2 diff = ball->position - bumper->position;
          float32 dist = Length(diff);
          vec2 dir = diff / dist;
          
          ball->position = ball->position + (dir * bumper->radius);

          ball->velocity = Reflect(ball->velocity, dir);
        }
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

  {
    
    // set the target values based on distance from the balls
    float32 radius = 4;
    float32 innerRadius = 2;
    float32 radiusSq = radius * radius;
    float32 innerRadiusSq = innerRadius * innerRadius;

    for (int i = 0; i < backgroundTileData.count; i++) {
      TileData *data = &backgroundTileData[i];

      data->targetColor = data->originalColor;
      data->targetScale = data->originalScale;
    }

    // @PERF: instead of looping over tile, loop over the balls
    // and then only grab the tiles around them in a radius
#if 1

    for (int i = 0; i < Game.balls.count; i++) {
      Ball *ball = &Game.balls[i];

      for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
          int32 x_ = x + ball->position.x;
          int32 y_ = y + ball->position.y;
          int32 index = GetTileIndex(x_, y_);

          if (index < 0) {
            continue;
          }

          TileData *data = &backgroundTileData[index];
          
          float32 distSq = DistanceSq(V2(x_, y_), ball->position);

          if (distSq >= radiusSq || distSq < innerRadiusSq) {
            continue;
          }

          //data->targetColor = V3(0.5f, 0.05f, 0.3f);
          data->targetColor = V3(0.6f, 0.55f, 0.1f);
          //data->targetColor = V3(0.7f, 0.05f, 0.2f);
          data->targetScale = 1.0;
        }
      }
    }
#else
    int32 index = 0;
    for (int y = 0; y < Mosaic->gridHeight; y++) {
      for (int x = 0; x < Mosaic->gridWidth; x++) {
        TileData *data = &backgroundTileData[index];

        vec2 position = V2(x, y);

        for (int i = 0; i < Game.balls.count; i++) {
          Ball *ball = &Game.balls[i];

          float32 distSq = DistanceSq(position, ball->position);

          if (distSq >= radiusSq) {
            continue;
          }

          data->targetColor = V3(0.4f, 0.05f, 0.2f);
          //data->targetColor = V3(0.7f, 1.0f, 1.0f);
          data->targetScale = 1.0;
        }

        index++;
      }
    }
#endif

    for (int i = 0; i < backgroundTileData.count; i++) {
      TileData *data = &backgroundTileData[i];

      float32 rate = 3.0f;

      // @BUG: should actually animate towards it and clamp
      data->color = Lerp(data->color, data->targetColor, rate * DeltaTime);
      data->scale = Lerp(data->scale, data->targetScale, rate * DeltaTime);
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

  for (int i = 0; i < Game.bumpers.count; i++) {
    Bumper *bumper = &Game.bumpers[i];

    float32 radius = bumper->radius;
    float32 radiusSq = bumper->radius * bumper->radius;

    int32 index = 0;
      
    for (int y = -radius; y <= radius; y++) {
      for (int x = -radius; x <= radius; x++) {

        vec2 position = bumper->position + V2(x, y);

        float32 distSq = DistanceSq(position, bumper->position);

        if (distSq > radiusSq) {
          continue;
        }

        float32 scale = (2.0 + (((1 + sinf(4 * Time + (index * 0.5f))) / 2) * 1.0f));

        SetTileTint(position.x, position.y, bumper->color.r, bumper->color.g, bumper->color.b);
        SetTileSprite(position.x, position.y, &bokeh);
        SetTileScale(position.x, position.y, scale);
        SetTileRotation(position.x, position.y, (Time + i) * 30);

        index++;
      }
    }
  }

  DrawTextf(V2((Mosaic->tileSize * Mosaic->gridWidth * 0.5f) + 20, 0), 80, V4(1, 1, 1, 1),
            "SCORE %d", Game.score);
  //DrawText("MOSAIC", 0, 0, 40, WHITE);
  //DrawText("MOSAIC", Mosaic->tileSize * Mosaic->gridWidth, 0, 40, WHITE);
  //DrawText("MOSAIC", Mosaic->tileSize * Mosaic->gridWidth, 0, 40, WHITE);
}
