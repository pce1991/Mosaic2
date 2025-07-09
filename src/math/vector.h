
union vec2 {
    float32 data[2];
    struct { float32 x, y; };

    float32& operator [](const int index) {
        return data[index];
    }
};

union vec3 {
    float32 data[3];
    struct { float32 x, y, z; };
    struct { float32 r, g, b; };
    struct { float32 h, s, v; };
    vec2 xy;

    float32& operator [](const int index) {
        return data[index];
    }
};

union vec4 {
    float32 data[4];
    struct { float32 x, y, z, w; };
    struct { float32 r, g, b, a; };
    struct { float32 h, s, v, _a; };
    vec2 xy;
    vec3 xyz;
    vec3 rgb;
    vec3 hsv;

    float32& operator [](const int index) {
        return data[index];
    }
};

union vec2i {
    int32 data[2];
    struct { int32 x, y; };

    int32& operator [](const int index) {
        return data[index];
    }
};

union vec3i {
    int32 data[3];
    struct { int32 x, y, z; };
    struct { int32 r, g, b; };
    vec2i xy;

    int32& operator [](const int index) {
        return data[index];
    }
};

union vec4i {
    int32 data[4];
    struct { int32 x, y, z, w; };
    struct { int32 r, g, b, a; };
    vec2i xy;
    vec3i xyz;
    vec3i rgb;

    int32& operator [](const int index) {
        return data[index];
    }
};

#define UP V3(0, 1, 0)
#define LEFT V3(1, 0, 0)
#define FORWARD V3(0, 0, 1)

#define XZ(a) V2(a.x, a.z)


// Constructors
inline vec2 V2(float32 x, float32 y) {
    vec2 result = {x, y};

    return result;
}

inline vec2 V2(int32 x, int32 y) {
    vec2 result = {(float32)x, (float32)y};

    return result;
}

inline vec2 V2(uint32 x, uint32 y) {
    vec2 result = {(float32)x, (float32)y};

    return result;
}

inline vec2 V2(float32 x, int32 y) {
    vec2 result = {x, (float32)y};

    return result;
}

inline vec2 V2(int32 x, float32 y) {
    vec2 result = {(float32)x, y};

    return result;
}

inline vec2 V2(uint32 x, float32 y) {
    vec2 result = {(float32)x, y};

    return result;
}

inline vec2 V2(float32 x, uint32 y) {
    vec2 result = {x, (float32)y};

    return result;
}


inline vec2 V2(float64 x, float64 y) {
    vec2 result = {(float32)x, (float32)y};

    return result;
}

inline vec2 V2(vec2i a) {
    vec2 result = {(float32)a.x, (float32)a.y};

    return result;
}

inline vec2i V2i(float32 x, float32 y) {
    vec2i result = {(int32)x, (int32)y};

    return result;
}

inline vec2i V2i(int32 x, int32 y) {
    vec2i result = {x, y};

    return result;
}

inline vec2i V2i(uint32 x, uint32 y) {
    vec2i result = {(int32)x, (int32)y};

    return result;
}

inline vec2i V2i(vec2 a) {
    vec2i result = {(int32)a.x, (int32)a.y};

    return result;
}

inline vec2 V2(float32 n) {
    vec2 result = {n, n};
    return result;
}

inline vec3 V3(float32 x, float32 y, float32 z) {
    vec3 result = {x, y, z};

    return result;
}

inline vec3 V3(float32 x, int32 y, int32 z) {
    vec3 result = {x, (float32)y, (float32)z};

    return result;
}

inline vec3 V3(float32 x, float32 y, int32 z) {
    vec3 result = {x, y, (float32)z};

    return result;
}

inline vec3 V3(int32 x, float32 y, float32 z) {
    vec3 result = {(float32)x, y, z};

    return result;
}

inline vec3 V3(int32 x, float32 y, int32 z) {
    vec3 result = {(float32)x, y, (float32)z};

    return result;
}

inline vec3 V3(int32 x, int32 y, float32 z) {
    vec3 result = {(float32)x, (float32)y, z};

    return result;
}

inline vec3 V3(int32 x, int32 y, int32 z) {
    vec3 result = {(float32)x, (float32)y, (float32)z};

    return result;
}

inline vec3 V3(uint32 x, uint32 y, uint32 z) {
    vec3 result = {(float32)x, (float32)y, (float32)z};

    return result;
}

inline vec3 V3(float32 x, int32 y, float32 z) {
    vec3 result = {x, (float32)y, z};

    return result;
}

inline vec3 V3(vec2 xy, float32 z) {
    vec3 result = {(float32)xy.x, (float32)xy.y, (float32)z};

    return result;
}

inline vec3 V3(vec2 xy, int32 z) {
    vec3 result = {(float32)xy.x, (float32)xy.y, (float32)z};

    return result;
}

inline vec3 V3(vec2 xy, uint32 z) {
    vec3 result = {(float32)xy.x, (float32)xy.y, (float32)z};

    return result;
}

inline vec3 V3(float32 n) {
    vec3 result = {n, n, n};
    return result;
}

inline vec3 V3(int32 n) {
    vec3 result = {(float32)n, (float32)n, (float32)n};
    return result;
}

inline vec3 V3(uint32 n) {
    vec3 result = {(float32)n, (float32)n, (float32)n};
    return result;
}

inline vec3 V3(vec3i a) {
    vec3 result = {(float32)a.x, (float32)a.y, (float32)a.z};

    return result;
}

inline vec3i V3i(float32 x, float32 y, float32 z) {
    vec3i result = {(int32)x, (int32)y, (int32)z};

    return result;
}

inline vec3i V3i(int32 x, int32 y, int32 z) {
    vec3i result = {x, y, z};

    return result;
}

inline vec3i V3i(uint32 x, uint32 y, uint32 z) {
    vec3i result = {(int32)x, (int32)y, (int32)z};

    return result;
}

inline vec3i V3i(vec3 a) {
    vec3i result = {(int32)a.x, (int32)a.y, (int32)a.z};

    return result;
}

inline vec4 V4(float32 a) {
    vec4 result = {a, a, a, a};
    return result;
}

inline vec4 V4(float32 x, float32 y, float32 z, float32 w) {
    vec4 result = {x, y, z, w};

    return result;
}

inline vec4 V4(int32 x, int32 y, int32 z, int32 w) {
    vec4 result = {(float32)x, (float32)y, (float32)z, (float32)w};

    return result;
}

inline vec4 V4(uint32 x, uint32 y, uint32 z, uint32 w) {
    vec4 result = {(float32)x, (float32)y, (float32)z, (float32)w};

    return result;
}

inline vec4 V4(int32 x, int32 y, int32 z, float32 w) {
    vec4 result = {(float32)x, (float32)y, (float32)z, (float32)w};

    return result;
}

inline vec4 V4(vec3 xyz, float32 w) {
    vec4 result = {(float32)xyz.x, (float32)xyz.y, (float32)xyz.z, (float32)w};

    return result;
}

inline vec4 V4(vec3 xyz, int32 w) {
    vec4 result = {(float32)xyz.x, (float32)xyz.y, (float32)xyz.z, (float32)w};

    return result;
}

inline vec4 V4(vec3 xyz, uint32 w) {
    vec4 result = {(float32)xyz.x, (float32)xyz.y, (float32)xyz.z, (float32)w};

    return result;
}

inline vec4 V4(vec4i a) {
    vec4 result = {(float32)a.x, (float32)a.y, (float32)a.z, (float32)a.w};

    return result;
}

inline vec4i V4i(float32 x, float32 y, float32 z, float32 w) {
    vec4i result = {(int32)x, (int32)y, (int32)z, (int32)w};

    return result;
}

inline vec4i V4i(int32 x, int32 y, int32 z, int32 w) {
    vec4i result = {x, y, z, w};

    return result;
}

inline vec4i V4i(uint32 x, uint32 y, uint32 z, uint32 w) {
    vec4i result = {(int32)x, (int32)y, (int32)z, (int32)w};

    return result;
}

inline vec4i V4i(vec4 a) {
    vec4i result = {(int32)a.x, (int32)a.y, (int32)a.z, (int32)a.w};

    return result;
}


// vec2 operations
inline vec2 operator+(vec2 a, vec2 b) {
    vec2 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

inline vec3 operator+(vec3 a, vec3 b) {
    vec3 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;
}

inline vec4 operator+(vec4 a, vec4 b) {
    vec4 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;

    return result;
}

inline vec2i operator+(vec2i a, vec2i b) {
    vec2i result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

inline vec3i operator+(vec3i a, vec3i b) {
    vec3i result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;
}

inline vec4i operator+(vec4i a, vec4i b) {
    vec4i result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;

    return result;
}

inline vec2 operator-(vec2 a) {
    vec2 result;

    result.x = -a.x;
    result.y = -a.y;

    return result;
}

inline vec3 operator-(vec3 a) {
    vec3 result;

    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;

    return result;
}

inline vec4 operator-(vec4 a) {
    vec4 result;

    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    result.w = -a.w;

    return result;
}

inline vec2i operator-(vec2i a) {
    vec2i result;

    result.x = -a.x;
    result.y = -a.y;

    return result;
}

inline vec3i operator-(vec3i a) {
    vec3i result;

    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;

    return result;
}

inline vec4i operator-(vec4i a) {
    vec4i result;

    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    result.w = -a.w;

    return result;
}

inline vec2 operator-(vec2 a, vec2 b) {
    vec2 result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;

    return result;
}

inline vec3 operator-(vec3 a, vec3 b) {
    vec3 result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}

inline vec4 operator-(vec4 a, vec4 b) {
    vec4 result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;

    return result;
}

inline vec2i operator-(vec2i a, vec2i b) {
    vec2i result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;

    return result;
}

inline vec3i operator-(vec3i a, vec3i b) {
    vec3i result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}

inline vec4i operator-(vec4i a, vec4i b) {
    vec4i result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;

    return result;
}

inline vec2 operator*(float32 a, vec2 b) {
    vec2 result;

    result.x = a * b.x;
    result.y = a * b.y;

    return result;
}

inline vec2 operator*(vec2 a, float32 b) {
    vec2 result;

    result.x = a.x * b;
    result.y = a.y * b;

    return result;
}

inline vec3 operator*(float32 a, vec3 b) {
    vec3 result;

    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;

    return result;
}

inline vec3 operator*(vec3 a, float32 b) {
    vec3 result;

    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;

    return result;
}

inline vec4 operator*(float32 a, vec4 b) {
    vec4 result;

    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;
    result.w = a * b.w;

    return result;
}

inline vec4 operator*(vec4 a, float32 b) {
    vec4 result;

    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    result.w = a.w * b;

    return result;
}

inline vec2i operator*(int32 a, vec2i b) {
    vec2i result;

    result.x = a * b.x;
    result.y = a * b.y;

    return result;
}

inline vec2i operator*(vec2i a, int32 b) {
    vec2i result;

    result.x = a.x * b;
    result.y = a.y * b;

    return result;
}

inline vec3i operator*(int32 a, vec3i b) {
    vec3i result;

    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;

    return result;
}

inline vec3i operator*(vec3i a, int32 b) {
    vec3i result;

    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;

    return result;
}

inline vec4i operator*(int32 a, vec4i b) {
    vec4i result;

    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;
    result.w = a * b.w;

    return result;
}

inline vec4i operator*(vec4i a, int32 b) {
    vec4i result;

    result.x = a.x * b;
    result.y = a.y * b;
    result.z = a.z * b;
    result.w = a.w * b;

    return result;
}

inline vec2 operator/(float32 a, vec2 b) {
    vec2 result;

    result.x = a / b.x;
    result.y = a / b.y;

    return result;
}

inline vec2 operator/(vec2 a, float32 b) {
    vec2 result;

    result.x = a.x / b;
    result.y = a.y / b;

    return result;
}

inline vec3 operator/(float32 a, vec3 b) {
    vec3 result;

    result.x = a / b.x;
    result.y = a / b.y;
    result.z = a / b.z;

    return result;
}

inline vec3 operator/(vec3 a, float32 b) {
    vec3 result;

    result.x = a.x / b;
    result.y = a.y / b;
    result.z = a.z / b;

    return result;
}

inline vec4 operator/(float32 a, vec4 b) {
    vec4 result;

    result.x = a / b.x;
    result.y = a / b.y;
    result.z = a / b.z;
    result.w = a / b.w;

    return result;
}

inline vec4 operator/(vec4 a, float32 b) {
    vec4 result;

    result.x = a.x / b;
    result.y = a.y / b;
    result.z = a.z / b;
    result.w = a.w / b;

    return result;
}

inline vec2i operator/(int32 a, vec2i b) {
    vec2i result;

    result.x = a / b.x;
    result.y = a / b.y;

    return result;
}

inline vec2i operator/(vec2i a, int32 b) {
    vec2i result;

    result.x = a.x / b;
    result.y = a.y / b;

    return result;
}

inline vec3i operator/(int32 a, vec3i b) {
    vec3i result;

    result.x = a / b.x;
    result.y = a / b.y;
    result.z = a / b.z;

    return result;
}

inline vec3i operator/(vec3i a, int32 b) {
    vec3i result;

    result.x = a.x / b;
    result.y = a.y / b;
    result.z = a.z / b;

    return result;
}

inline vec4i operator/(int32 a, vec4i b) {
    vec4i result;

    result.x = a / b.x;
    result.y = a / b.y;
    result.z = a / b.z;
    result.w = a / b.w;

    return result;
}

inline vec4i operator/(vec4i a, int32 b) {
    vec4i result;

    result.x = a.x / b;
    result.y = a.y / b;
    result.z = a.z / b;
    result.w = a.w / b;

    return result;
}

inline bool operator==(vec2 a, vec2 b) {
    bool result = (a.x == b.x &&
                  a.y == b.y);

    return result;
}

inline bool operator==(vec3 a, vec3 b) {
    bool result = (a.x == b.x &&
                  a.y == b.y &&
                  a.z == b.z);

    return result;
}

inline bool operator==(vec4 a, vec4 b) {
    bool result = (a.x == b.x &&
                  a.y == b.y &&
                  a.z == b.z &&
                  a.w == b.w);

    return result;
}

inline bool operator==(vec2i a, vec2i b) {
    bool result = (a.x == b.x &&
                  a.y == b.y);

    return result;
}

inline bool operator==(vec3i a, vec3i b) {
    bool result = (a.x == b.x &&
                  a.y == b.y &&
                  a.z == b.z);

    return result;
}

inline bool operator==(vec4i a, vec4i b) {
    bool result = (a.x == b.x &&
                  a.y == b.y &&
                  a.z == b.z &&
                  a.w == b.w);

    return result;
}

inline bool operator!=(vec2 a, vec2 b) {
    bool result = (a.x != b.x ||
                  a.y != b.y);

    return result;
}

inline bool operator!=(vec3 a, vec3 b) {
    bool result = (a.x != b.x ||
                  a.y != b.y ||
                  a.z != b.z);

    return result;
}

inline bool operator!=(vec4 a, vec4 b) {
    bool result = (a.x != b.x ||
                  a.y != b.y ||
                  a.z != b.z ||
                  a.w != b.w);

    return result;
}

inline bool operator!=(vec2i a, vec2i b) {
    bool result = (a.x != b.x ||
                  a.y != b.y);

    return result;
}

inline bool operator!=(vec3i a, vec3i b) {
    bool result = (a.x != b.x ||
                  a.y != b.y ||
                  a.z != b.z);

    return result;
}

inline bool operator!=(vec4i a, vec4i b) {
    bool result = (a.x != b.x ||
                  a.y != b.y ||
                  a.z != b.z ||
                  a.w != b.w);

    return result;
}
