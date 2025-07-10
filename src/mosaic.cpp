
#include "mosaic.h"

void SetMosaicGridSize(uint32 newWidth, uint32 newHeight);

void MosaicInternalInit() {
    Mosaic = (MosaicMem *)malloc(sizeof(MosaicMem));
    memset(Mosaic, 0, sizeof(MosaicMem));
    
    Mosaic->tileSize = 10;

    
    SetMosaicGridSize(16, 16);
}
    
    
inline MTile* GetTile(int32 x, int32 y) {
  if (x < 0 || x >= Mosaic->gridWidth) {
    return NULL;
  }
  if (y < 0 || y >= Mosaic->gridHeight) {
    return NULL;
  }
    
  // TODO: clamp these in case they're out of bounds, so we don't crash
  int32 index = (y * Mosaic->gridWidth) + x;
  return &Mosaic->tiles[index];
}


inline MTile*GetTile(vec2i pos) {
  return GetTile(pos.x, pos.y);
}

inline MTile*GetTile(vec2 pos) {
  return GetTile(pos.x, pos.y);
}

inline void SetTileColor(int32 x, int32 y, vec4 color) {
  MTile*t = GetTile(x, y);
  if (t) {
    t->color = color;
  }
}

void SetMosaicGridSize(uint32 newWidth, uint32 newHeight) {
    // @TODO: clamp
    Mosaic->gridWidth = newWidth;
    Mosaic->gridHeight = newHeight;

    if (Mosaic->tiles != NULL) {
        free(Mosaic->tiles);
    }
    
    Mosaic->tileCapacity = Mosaic->gridWidth * Mosaic->gridHeight;
    Mosaic->tiles = (MTile*)malloc(sizeof(MTile) * Mosaic->tileCapacity);

    memset(Mosaic->tiles, 0, Mosaic->tileCapacity * sizeof(MTile));

    float32 levelAspect = Mosaic->gridWidth / (Mosaic->gridHeight * 1.0f);
    
    Mosaic->padding = 2;
    
    float32 screenAspect = 16.0 / 9.0f;
    // @HARDCODED


    // @TODO: add the line sizes
    Mosaic->gridSize.x = Mosaic->tileSize * Mosaic->gridWidth;
    Mosaic->gridSize.y = Mosaic->tileSize * Mosaic->gridHeight;
    
    // @TODO: because we're not in 3D our grid origin needs to get shifted from the top left.
    // OR maybe we should just put ourselves in 3D, idk? 
    // Or we can begin a 2D camera pass, that's probably better, and we don't have to care about
    // screen dimensions. 
    Mosaic->gridOrigin = V2(0) + V2(-Mosaic->gridSize.x * 0.5f, Mosaic->gridSize.y * 0.5f);

    //AllocateRectBuffer(Mosaic->gridWidth * Mosaic->gridHeight, &Mosaic->rectBuffer);

    MTile*tiles = Mosaic->tiles;
    for (int y = 0; y < Mosaic->gridHeight; y++) {
        for (int x = 0; x < Mosaic->gridWidth; x++) {
            MTile*tile = GetTile(x, y);
            tile->position = V2i(x, y);
        }
    }
}




inline void SetTileColor(int32 x, int32 y, float32 r, float32 g, float32 b) {
  MTile*t = GetTile(x, y);
  if (t) {
    t->color = V4(r, g, b, 1.0f);
  }
}

inline void SetTileColor(vec2 position, float32 r, float32 g, float32 b) {
  MTile*t = GetTile(position);
  if (t) {
    t->color = V4(r, g, b, 1.0f);
  }
}

inline void SetTileColor(vec2 position, vec4 color) {
  MTile*t = GetTile(position);
  if (t) {
    t->color = color;
  }
}
/*

*/

vec2 GridPositionToWorldPosition(vec2i gridPosition) {
  vec2 worldPos = Mosaic->gridOrigin;
  float32 size = Mosaic->tileSize;
  worldPos.x += (size * gridPosition.x);// + (size * 0.5f);
  worldPos.y += (-size * gridPosition.y) + (-size);
  
  // @HACK: unsure about this calculation but it works
  
  return worldPos;
}

void DrawTile(vec2i position, vec4 color) {
  vec2 worldPos = GridPositionToWorldPosition(position);
  
 Color c = {};
 c.r = color.r * 255;
 c.b = color.b * 255;
 c.g = color.g * 255;
 c.a = color.a * 255;

  // this is a Raylib function. I think it should get implemented there.
  // @TODO: need to convert the vec4 color to raylib's Color struct
  // We need to convert tileSize into pixel size for raylib
  //DrawRectangle(worldPos.x, worldPos.y, Mosaic->tileSize * 0.5f, Mosaic->tileSize * 0.5f, MAROON);
  
  DrawRectangle(worldPos.x, worldPos.y, Mosaic->tileSize, Mosaic->tileSize, c);
  
  //DrawRectangle(0, 0, 50, 50, MAROON);
}

void DrawTile(vec2i position, vec3 color) {
   DrawTile(position, V4(color.r, color.g, color.b, 1.0f));
}

/*

void DrawTextTop(vec4 color, const char *fmt, ...) {
  va_list args;
  va_start (args, fmt);

  char str[GlyphBufferCapacity];
  vsnprintf(str, PRINT_MAX_BUFFER_LEN, fmt, args);

  vec2 position = Mosaic->gridOrigin + V2(Mosaic->gridSize.x * 0.5f, 0.1f);
  DrawText(&Game->monoFont, position, 0.35f, color, true, str);

  va_end(args);
}

void DrawTextTop(vec4 color, float32 scale, const char *fmt, ...) {
  va_list args;
  va_start (args, fmt);

  char str[GlyphBufferCapacity];
  vsnprintf(str, PRINT_MAX_BUFFER_LEN, fmt, args);

  vec2 position = Mosaic->gridOrigin + V2(Mosaic->gridSize.x * 0.5f, 0.1f);
  DrawText(&Game->monoFont, position, 0.35f * scale, color, true, str);

  va_end(args);
}

void DrawTextTile(vec2 pos, float32 size, vec4 color, const char *fmt, ...) {
  va_list args;
  va_start (args, fmt);

  char str[GlyphBufferCapacity];
  vsnprintf(str, PRINT_MAX_BUFFER_LEN, fmt, args);

  vec2 floorPos = V2(floorf(pos.x), -floorf(pos.y));
    
  vec2 position = Mosaic->gridOrigin + floorPos + V2(0.0f, -1.0f);
  DrawText(&Game->monoFont, position, size, color, false, str);

  va_end(args);
}

void DrawTextTile(vec2 pos, float32 size, vec4 color, bool center, const char *fmt, ...) {
  va_list args;
  va_start (args, fmt);

  char str[GlyphBufferCapacity];
  vsnprintf(str, PRINT_MAX_BUFFER_LEN, fmt, args);

  vec2 floorPos = V2(floorf(pos.x), -floorf(pos.y));
    
  vec2 position = Mosaic->gridOrigin + floorPos + V2(0.0f, -1.0f);
  DrawText(&Game->monoFont, position, size, color, center, str);

  va_end(args);
}

*/

