
#include "src/engine.cpp"


void MosaicInit() {  
    SetMosaicGridSize(9, 9);
    Mosaic->padding = 2;
}

void MosaicUpdate() {

    MTile*tiles = Mosaic->tiles;
        for (int y = 0; y < Mosaic->gridHeight; y++) {
            for (int x = 0; x < Mosaic->gridWidth; x++) {
                float32 r = x / (Mosaic->gridWidth * 1.0f);
                float32 g = (1 + sinf(Time)) * 0.5f;
                float32 b = y / (Mosaic->gridHeight * 1.0f);
               SetTileColor(x, y, r, g, b);
            }
    }      
        
    SetTileColor(10, 10, 0.8f, 0.4f, 0.6f);
}
