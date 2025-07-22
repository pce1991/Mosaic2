inline int32 Powi(int32 base, int32 exp) {
    if (exp < 0) {
        return 0;
    }

    int32 result = 1;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }

        exp >>= 1;
        base *= base;
    }

    return result;
}

inline float32 Snap(float32 value, float32 snapInterval, float32 baseValue = 0.0f) {
    float32 result;
    if (snapInterval > 0.0f) {
        result = baseValue + roundf((value - baseValue) / snapInterval) * snapInterval;
    }
    else {
        result = value;
    }

    return result;
}

inline float32 SnapDown(float32 value, float32 snapInterval, float32 baseValue = 0.0f) {
    float32 result;
    if (snapInterval > 0.0f) {
        result = baseValue + floorf((value - baseValue) / snapInterval) * snapInterval;
    }
    else {
        result = value;
    }

    return result;
}

inline float32 SnapUp(float32 value, float32 snapInterval, float32 baseValue = 0.0f) {
    float32 result;
    if (snapInterval > 0.0f) {
        result = baseValue + ceilf((value - baseValue) / snapInterval) * snapInterval;
    }
    else {
        result = value;
    }

    return result;
}


// @NOTE: this only works with positive ints
inline int32 RoundUpToNearestMultiple(int32 x, int32 mult) {
    return ((x + mult - 1) / mult) * mult;
}


// Returns (-1, 0, 1)
inline int32 Signum(float32 x) {
    int32 result = (0 < x) - (x < 0);
    return result;
}

// Returns (-1, 1)
inline int32 Sign(float32 x) {
    int32 result = (0 <= x) - (x < 0);
    return result;
}

inline float32 Lerp(float32 a, float32 b, float32 t) {
    float32 result = (1 - t) * a + t * b;
    return result;
}

inline float32 InverseLerp(float32 a, float32 b, float32 x) {
    float32 result = 0;

    if (a != b) {
        result = (a - x) / (a - b);
    }

    return result;
}

inline float32 LinearRemap(float32 a, float32 b, float32 x, float32 aPrime, float32 bPrime) {
    float32 t = InverseLerp(a, b, x);
    float32 result = Lerp(aPrime, bPrime, t);

    return result;
}

inline float32 Min(float32 a, float32 b) {
    float32 result = a;

    if (a > b) {
        result = b;
    }

    return result;
}

inline float64 Min(float64 a, float64 b) {
    float64 result = a;

    if (a > b) {
        result = b;
    }

    return result;
}


inline int32 Min(int32 a, int32 b) {
    int32 result = a;

    if (a > b) {
        result = b;
    }

    return result;
}

inline uint32 Min(uint32 a, uint32 b) {
    int32 result = a;

    if (a > b) {
        result = b;
    }

    return result;
}

inline uint64 Min(uint64 a, uint64 b) {
    int64 result = a;

    if (a > b) {
        result = b;
    }

    return result;
}

inline float32 Max(float32 a, float32 b) {
    float32 result = a;

    if (a < b) {
        result = b;
    }

    return result;
}

inline float32 Max(int32 a, float32 b) {
    float32 result = a;

    if (a < b) {
        result = b;
    }

    return result;
}

inline float32 Max(float32 a, int32 b) {
    float32 result = a;

    if (a < b) {
        result = b;
    }

    return result;
}


inline int32 Max(int32 a, int32 b) {
    int32 result = a;

    if (a < b) {
        result = b;
    }

    return result;
}

inline uint32 Max(uint32 a, uint32 b) {
    uint32 result = a;

    if (a < b) {
        result = b;
    }

    return result;
}

inline uint64 Max(uint64 a, uint64 b) {
    uint64 result = a;

    if (a < b) {
        result = b;
    }

    return result;
}

inline float32 Clamp(float32 x, float32 a, float32 b) {
    float32 result = x;

    if (x < a) {
        result = a;
    }

    if (x > b) {
        result = b;
    }

    return result;
}

inline int32 Clamp(int32 x, int32 a, int32 b) {
    int32 result = x;

    if (x < a) {
        result = a;
    }

    if (x > b) {
        result = b;
    }

    return result;
}

inline float32 Clamp01(float32 x) {
    return Clamp(x, 0.0f, 1.0f);
}

inline float32 SafeRatio(float32 a, float32 b, float32 n) {
    float32 result = n;

    if (b != 0.0f) {
        result = a / b;
    }

    return result;
}

inline float32 SafeRatio(float32 a, float32 b, float32 n, float32 epsilon) {
    float32 result = n;

    if (b < 0.0f - epsilon && b > 0.0f + epsilon) {
        result = a / b;
    }

    return result;
}


inline float32 SafeInvert(float32 n) {
    return n != 0.0f ? 1.0f / n : 0.0f;
}

inline float32 NormalizeAngleDeg(float32 a) {
    float32 result = a;

    while (result >= 360.0f) {
        result -= 360.0f;
    }

    while (result < 0) {
        result += 360.0f;
    }

    return result;
}

inline float32 NormalizeAngleRad(float32 a) {
    float32 result = a;

    while (result >= _2PI) {
        result -= _2PI;
    }

    while (result < 0) {
        result += _2PI;
    }

    return result;
}

inline float32 DeltaAngleRad(float32 from, float32 to) {

    float32 result = to - from;
    if (result > _PI) {
        result -= _2PI;
    }

    if (result < -_PI) {
        result += _2PI;
    }

    return result;
}

inline float32 Ceilf(float32 x) {
    return ceilf(x);
}

inline float32 Floorf(float32 x) {
    return floorf(x);
}

inline float32 Roundf(float32 x) {
    return roundf(x);
}

inline float32 Fractf(float32 x) {
    return x - Floorf(x);
}

inline float32 Modf(float32 x, float32 m) {
    float32 result = x - m * Floorf(x / m);
    return result;
}

inline float32 Square(float32 x) {
    float32 result = x * x;
    return result;
}

inline float32 Cube(float32 x) {
    float32 result = x * x * x;
    return result;
}

inline float32 Abs(float32 x) {
    return fabsf(x);
}

inline float32 Absf(float32 x) {
    return fabsf(x);
}

inline int32 Abs(int32 x) {
    return abs(x);
}

inline int32 Modi(int32 n, int32 m) {
    if (n == -1) { return m - 1; }
    return n % m;
}

inline float32 SmoothStep(float32 t) {
    float32 result = t * t * (3.0f - 2.0f * t);
    return result;
}

inline float32 SmootherStep(float32 t) {
    float32 result = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    return result;
}

inline float32 DegToRad(float32 deg) {
    float32 result = deg * _PI / 180.0f;
    return result;
}

inline float32 RadToDeg(float32 rad) {
    float32 result = rad * 180.0f / _PI;
    return result;
}

inline float32 Distance(float32 x0, float32 y0, float32 x1, float32 y1) {
    float32 result = sqrtf(Square(x1 - x0) + Square(y1 - y0));
    return result;
}

inline float32 Log(float32 base, float32 x) {
    float32 result = log(x) / log(base);
    return result;
}

// [0] http://realtimecollisiondetection.net/blog/?p=89
inline bool NearlyEquals(float32 a, float32 b, float32 absTolerance, float32 relTolerance) {
    float32 diff = Abs(b - a);
    bool result = diff <= Max(absTolerance, relTolerance * Max(Abs(a), Abs(b)));
    return result;
}

inline bool NearlyEquals(float32 a, float32 b, float32 absTolerance = FLT_EPSILON) {
    float32 relTolerance = absTolerance;
    float32 diff = Abs(b - a);
    bool result = diff <= Max(absTolerance, relTolerance * Max(Abs(a), Abs(b)));
    return result;
}

inline float32 PingPong(float32 t, float32 max) {
    float32 L = 2 * max;
    float32 T = Modf(t, L);

    float32 result;

    if (T >= 0 && T <= max) {
        result = T;
    }
    else {
        result = L - T;
    }

    return result;
}

inline bool InRange(float32 n, float32 min, float32 max) {
    return n >= min && n <= max;
}


inline float32 Dampen(float32 n, float32 t) {
    float32 m = n - n * t;

    if (Signum(m) != Signum(n)) {
        return 0;
    }

    return m;
}

inline float32 MoveTowards(float32 n, float32 target, float32 delta) {
  if (n == target) { return n; }
    
  n = n + delta;

  if (delta < 0) {
    if (n < target) {
      n = target;
    }
  }
  else {
    if (n > target) {
      n = target;
    }
  }
    
  return n;
}
