
struct MTile{
  // This is kinda redundant because the tile can't move, but it's easier to have a tile
  // and know where it is than pass it's position around. 
  vec2i position;
  vec4 color;
  
  Texture2D *sprite;
  vec4 tint; // used with sprite
};


struct MosaicMem {
  vec2 gridSize;
  vec2 gridOrigin;
  float32 padding;
  
  float32 tileSize;

  uint8 gridWidth;
  uint8 gridHeight;

  uint32 tileCapacity;
  MTile* tiles;
    
};

const int32 GlyphBufferCapacity = 128;

MosaicMem *Mosaic = NULL;
