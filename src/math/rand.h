
#define LCG_MULT 1664525u
#define LCG_INC 1013904223u

typedef struct {
    uint32 seed;
    uint32 state;
} LCGState;

LCGState defaultLCGState = {};

void SeedRand(uint32 seed) {
    defaultLCGState.seed = seed;
    defaultLCGState.state = seed;
}

uint32 Randi() {
    defaultLCGState.state = defaultLCGState.state * LCG_MULT + LCG_INC;
    return defaultLCGState.state;
}

uint32 Randi(uint32 upperLimit) {
  return Randi() % upperLimit;
}

int32 RandiRange(int32 lowerLimit, int32 upperLimit) {
    return lowerLimit + (int32)Randi((uint32)(upperLimit - lowerLimit));
}

float32 Randf() {
    return (float32)Randi() / (float32)UINT32_MAX;
}

float32 RandfUpper(float32 upperLimit) {
    return Randf() * upperLimit;
}

float32 RandfRange(float32 lowerLimit, float32 upperLimit) {
    return lowerLimit + RandfUpper(upperLimit - lowerLimit);
}
