
bool PointsAreCollinear(vec2 a, vec2 b, vec2 c, float32 epsilon = 0.0f) {
    return NearlyEquals((b.y - a.y) * (c.x - b.x), (c.y - b.y) * (b.x - a.x), 0.0f, epsilon);
}

bool PointsAreCollinear(vec3 a, vec3 b, vec3 c, float32 epsilon = 0.0f) {
    return NearlyEquals(Length(Cross(c - a, b - a)), 0.0f, epsilon);
}

struct Plane {
    // @TODO: get rid of this so we save space, and conceptually we're mathier
    // I havent seen anyone else define a plane by using a point, so I think its not useful
    vec3 point;
    
    vec3 normal;
    float32 d; // d = Dot(point, normal)
};

inline Plane TransformPlane(mat4 transform, Plane plane) {
    mat4 inverse;
    Inverse(transform, &inverse);

    Plane result = {};
    result.point = MultiplyPoint(transform, plane.point);

    vec4 v = LeftMultiply(V4(plane.normal, plane.d), inverse);
    result.normal = v.xyz;
    result.d = v.w;

    return result;
}

inline Plane MakePlane(vec3 pt, vec3 normal) {
    Plane p = {};
    p.point = pt;
    p.normal = normal;
    p.d = Dot(pt, normal);
    return p;
}

inline Plane MakePlane(vec3 pt, quaternion rotation) {
    Plane p = {};
    p.point = pt;
    p.normal = Rotate(rotation, FORWARD);
    return p;
}

inline float32 PerpProduct(vec2 a, vec2 b) {
    return a.y * b.x - a.x * b.y;
}

// @NOTE: assumes triangle is wound counter clockwise
inline bool PointInTriangle(vec2 point, vec2 a, vec2 b, vec2 c) {
    if (Cross(point - a, b - a) < 0) return false;
    if (Cross(point - b, c - b) < 0) return false;
    if (Cross(point - c, a - c) < 0) return false;

    return true;
}

// [0]: Realtime Collision Detection, pg 152
// Returns 2 * the signed triangle area. The result is positive if abc
// is ccw, negative if abc is cw, and 0 if degenerate.
inline float32 SignedTriangleArea(vec2 a, vec2 b, vec2 c) {
    return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
}

// Planes
// @GACK: the order of arguments is not consistent here

inline vec3 ClosestPointOnPlane(vec3 point, vec3 planePoint, vec3 normal) {
    float32 d = Dot(normal, planePoint);
    float32 t = Dot(normal, point) - d;

    return point - (t * normal);
}


inline vec3 ClosestPointOnPlane(vec3 point, vec3 normal, float32 d) {
    float32 t = Dot(normal, point) - d;
    return point - (t * normal);
}

inline vec3 ClosestPointOnPlane(vec3 point, Plane plane) {
    return ClosestPointOnPlane(point, plane.point, plane.normal);
}

inline float32 PlaneTest(vec3 point, vec3 normal, vec3 testPoint) {
    float32 result = Dot(testPoint - point, normal);
    return result;
}

inline float32 PlaneTest(Plane plane, vec3 testPoint) {
    return PlaneTest(plane.point, plane.normal, testPoint);
}

inline float32 PlaneDistance(vec3 point, vec3 normal, vec3 planePoint) {
    float32 d = Dot(normal, planePoint);
    float32 d_ = Dot(normal, point) - d;
    return (d_ / Dot(normal, normal));
}

inline float32 PlaneDistance(vec3 point, Plane plane) {
    return PlaneDistance(point, plane.normal, plane.point);
}

inline bool LineLiesOnPlane(vec3 a, vec3 b, Plane plane, float32 epsilon = 0.0f) {
    float32 distA = PlaneDistance(a, plane);
    float32 distB = PlaneDistance(b, plane);

    return (distA > -epsilon && distA < epsilon &&
            distB > -epsilon && distB < epsilon);
}

inline bool TestPointFrustum(vec3 p, Plane *planes, int32 planeCount, float32 *results) {
    bool success = true;

    float32 r = 0;
    for (int i = 0; i < planeCount; i++) {
        r = PlaneTest(planes[i], p);
        if (results != NULL) {
            results[i] = r;
        }
        if (r < 0) {
            success = false;
        }
    }

    return success;
}

inline bool PlanePlaneIntersection(Plane a, Plane b, vec3 *pt, vec3 *dir, float32 eps = 0.0f) {
    vec3 u = Cross(a.normal, b.normal);

    float32 denom = Dot(u, u);
    if (denom <= eps) { return false; }

    *dir = u;
    *pt = Cross((a.d * b.normal) - (b.d * a.normal), u) / denom;

    return true;
}

inline bool PlanePlanePlaneIntersection(Plane a, Plane b, Plane c, vec3 *pt, float32 eps = 0.0f) {

    float32 d = Dot(a.normal, Cross(b.normal, c.normal));
    if (d < -eps || d > eps) {
        *pt = (-a.d * Cross(b.normal, c.normal) - (b.d * Cross(c.normal, a.normal)) - (c.d * Cross(a.normal, b.normal))) / d;
        return true;
    }
    return false;
}

inline bool PlaneSegmentIntersection(Plane p, vec3 a, vec3 b, vec3 *pt) {
    vec3 ab = b - a;

    float32 t = (p.d - Dot(p.normal, a)) / Dot(p.normal, ab);

    if (t >= 0 && t <= 1.0f) {
        *pt = a + t * ab;
        return true;
    }

    return false;
}

// Shamelessly stolen from: http://box2d.org/2014/02/computing-a-basis/
inline void ConstructBasis(vec3 normal, vec3 *X, vec3 *Y) {
    // Suppose normal vector has all equal components and is a unit vector: normal = (s, s, s)
    // Then 3*s*s = 1, s = sqrt(1/3) = 0.57735. This means that at least one component of a
    // unit vector must be greater or equal to 0.57735.
 
    if (Abs(normal.x) >= 0.57735f) {
        *Y = Normalize(V3(normal.y, -normal.x, 0.0f));
    }
    else {
        *Y = Normalize(V3(0.0f, normal.z, -normal.y));
    }
 
    *X = Cross(normal, *Y);
}

inline vec2 ProjectTo2D(vec3 origin, vec3 X, vec3 Y, vec3 pointToProject) {
    vec2 result;

    result.x = Dot(X, pointToProject - origin);
    result.y = Dot(Y, pointToProject - origin);

    return result;
}

inline vec2 ProjectTo2D(vec3 normal, vec3 point) {
    uint32 xIndex = 0;
    uint32 yIndex = 1;

    vec3 absNormal = V3(Abs(normal[0]), Abs(normal[1]), Abs(normal[2]));

    if (absNormal[0] >= absNormal[1] &&
        absNormal[0] >= absNormal[2]) {
        xIndex = 1;
        yIndex = 2;
    }
    else if (absNormal[1] >= absNormal[0] &&
             absNormal[1] >= absNormal[2]) {
        xIndex = 2;
        yIndex = 0;
    }
    else if (absNormal[2] >= absNormal[0] &&
             absNormal[2] >= absNormal[1]) {
        xIndex = 0;
        yIndex = 1;
    }

    return V2(point.data[xIndex], point.data[yIndex]);
}

inline vec3 ProjectTo3D(vec2 point, vec3 origin, vec3 X, vec3 Y) {
    vec3 result = {};
    result.x = origin.x + (point.x * X.x) + (point.y * Y.x);
    result.y = origin.y + (point.x * X.y) + (point.y * Y.y);
    result.z = origin.z + (point.x * X.z) + (point.y * Y.z);

    return result;
}


// Polygons
inline bool PointInPolygon(vec2 *points, uint32 pointCount, vec2 point) {
    vec2 scanline = V2(1, 0);
    vec2 scanlinePerp = V2(0, 1);

    int intersections = 0;

    for (int i = 0; i < pointCount; i++) {
        vec2 a = points[i];
        vec2 b = points[(i + 1) % pointCount];

        vec2 u = b - a;
        if (Dot(u, scanlinePerp) == 0) {
            continue;
        }

        vec2 uPerp = V2(-u.y, u.x);
        vec2 w = a - point;

        float32 s = -Dot(scanlinePerp, w) / Dot(scanlinePerp, u);
        float32 t = Dot(uPerp, w) / Dot(uPerp, scanline);

        if (0 < s && s <= 1 && t >= 0) {
            intersections++;
        }
    }

    bool pointInPolygon = (intersections % 2) == 1;
    return pointInPolygon;
}

inline vec2 Centroid(vec2 *points, uint32 pointCount) {
    vec2 centroid = V2(0, 0);

    for (int i = 0; i < pointCount; i++) {
        centroid = centroid + points[i];
    }

    if (pointCount > 0) {
        centroid = centroid / pointCount;
    }

    return centroid;
}

inline float32 SignedAreaOfPolygon(vec2 *points, uint32 pointCount) {
    float32 result = 0.0f;
    for (int i = 0; i < pointCount; i++) {
        vec2 p0 = points[i];
        vec2 p1 = points[(i + 1) % pointCount];        
        result += (p0.x * p1.y - p1.x * p0.y);
    }

    return 0.5 * result;
}

inline vec2 CentroidOfPolygon(vec2 *points, uint32 pointCount) {
    float32 area = SignedAreaOfPolygon(points, pointCount);

    vec2 result = {};

    for (int i = 0; i < pointCount; i++) {
        vec2 p0 = points[i];
        vec2 p1 = points[(i + 1) % pointCount];

        result.x += (p0.x + p1.x) * (p0.x * p1.y - p1.x * p0.y);
        result.y += (p0.y + p1.y) * (p0.x * p1.y - p1.x * p0.y);
    }

    result = result * (1 / (6 * area));

    return result;
}

// returns positive when points are clockwise
inline float32 ShoelaceFormula(vec2 *points, uint32 pointCount, uint32 stride = 0) {
    if (stride == 0) {
        stride = sizeof(vec2);
    }

    float32 sum = 0.0f;

    for (int i = 0; i < pointCount; i++) {
        vec2 a = STRIDED_READ(vec2, points, stride, i);
        vec2 b = STRIDED_READ(vec2, points, stride, (i + 1) % pointCount);
        vec2 c = STRIDED_READ(vec2, points, stride, (i + 2) % pointCount);

        sum += b.x * (c.y - a.y);
    }

    float32 result = 0.5f * sum;
    return result;
}


inline float32 ShoelaceFormulaXZ(vec3 *points, uint32 pointCount, uint32 stride = 0) {
    if (stride == 0) {
        stride = sizeof(vec3);
    }

    float32 sum = 0.0f;

    for (int i = 0; i < pointCount; i++) {
        vec3 a = STRIDED_READ(vec3, points, stride, i);
        vec3 b = STRIDED_READ(vec3, points, stride, (i + 1) % pointCount);
        vec3 c = STRIDED_READ(vec3, points, stride, (i + 2) % pointCount);

        sum += b.x * (c.z - a.z);
    }

    float32 result = 0.5f * sum;
    return result;
}


// Rays
struct Ray3D {
    vec3 origin;
    vec3 direction;
};

inline Ray3D MakeRay(vec3 origin, vec3 direction) {
    Ray3D result;
    result.origin = origin;
    result.direction = direction;

    return result;
}

inline Ray3D TransformRay(mat4 transform, Ray3D ray) {
    Ray3D result;
    result.origin = MultiplyPoint(transform, ray.origin);
    result.direction = Normalize(MultiplyDirection(transform, ray.direction));
    return result;
}


inline vec3 PointAt(Ray3D ray, float32 t) {
    vec3 result = ray.origin + t * ray.direction;
    return result;
}


struct Ray2D {
    vec2 origin;
    vec2 direction;
};

inline Ray2D MakeRay(vec2 origin, vec2 direction) {
    Ray2D result;
    result.origin = origin;
    result.direction = direction;

    return result;
}

inline vec2 PointAt(Ray2D ray, float32 t) {
    vec2 result = ray.origin + t * ray.direction;
    return result;
}


// Finds the parameter of Ray a closest to Ray b. Potentially finds
// points "behind" either ray.
// [0] http://geomalgorithms.com/a07-_distance.html
inline float32 ClosestToRayAt(Ray3D a, Ray3D b) {
    float32 s = 0;

    float32 a_ = Dot(a.direction, a.direction);
    float32 b_ = Dot(a.direction, b.direction);
    float32 c_ = Dot(b.direction, b.direction);
    float32 d_ = Dot(a.direction, a.origin - b.origin);
    float32 e_ = Dot(b.direction, a.origin - b.origin);

    float32 denom = a_ * c_ - Square(b_);
    if (denom != 0) {
        s = (b_ * e_ - c_ * d_) / denom;
    }

    return s;
}

// @TODO: I don't trust this at all -hz
inline float32 ClosestToPointAt(Ray3D ray, vec3 point) {
    float32 t = Dot(ray.direction, point - ray.origin);
    return t;
}

inline bool RaycastPlane(vec3 point, vec3 normal, Ray3D ray, float32 *t = NULL) {
    bool result = false;
    float32 intersection = 0.0f;

    float32 nd = Dot(ray.direction, normal);
    if (!NearlyEquals(nd, 0.0f)) {
        intersection = Dot(point - ray.origin, normal) / nd;
        result = intersection >= 0.0f;
    }

    if (t != NULL) {
        *t = intersection;
    }
    return result;
}

inline bool RaycastPlane(Plane plane, Ray3D ray, float32 *t = NULL) {
    return (bool)RaycastPlane(plane.point, plane.normal, ray, t);
}

inline bool IsTriangleDegenerate(vec3 a, vec3 b, vec3 c) {
    float32 parallelogramArea = Length(Cross(b - a, c - a));
    return NearlyEquals(parallelogramArea, 0.0f);
}


inline bool PointCircleTest(vec2 point, vec2 center, float32 radius) {
    bool result = Distance(center, point) <= radius;
    return result;
}


inline bool CircleCircleTest(vec2 centerA, float32 radiusA, vec2 centerB, float32 radiusB) {
    vec2 worldDisplacement = centerB - centerA;
    float32 distanceSquared = LengthSq(worldDisplacement);
    float32 radiusSum = radiusA + radiusB;

    if (distanceSquared <= (radiusSum * radiusSum)) {
        return true;
    }

    return false;
}

inline bool SphereSphereTest(vec3 a, float32 radiusA, vec3 b, float32 radiusB) {
    vec3 worldDisplacement = b - a;
    float32 distanceSquared = LengthSq(worldDisplacement);
    float32 radiusSum = radiusA + radiusB;

    if (distanceSquared <= (radiusSum * radiusSum)) {
        return true;
    }

    return false;
}

bool SegmentCircleIntersection(vec2 p0, vec2 p1, vec2 center, float32 radius, float32 *t) {
    vec2 seg = p1 - p0; // d 
    vec2 diff = p0 - center; // f

    float32 a = Dot(seg, seg);
    float32 b = 2 * Dot(diff, seg);
    float32 c = Dot(diff, diff) - (radius * radius);

    float32 discrim = b * b - 4 * a * c;
    if (discrim < 0) {
        return false;
    }
    else {
        discrim = sqrt(discrim);

        float32 t0 = (-b - discrim) / (2 * a);
        float32 t1 = (-b + discrim) / (2 * a);

        if (t0 >= 0 && t0 <= 1) {
            *t = t0;
            return true;
        }

        if (t1 >= 0 && t1 <= 1) {
            *t = t1;
            return true;
        }
    }

    return false;
}

struct DelaunayTriangleEdge {
    int32 indices[2];
    bool duplicated;

    int32& operator [](const int index) {
        return indices[index];
    }
};

inline bool TriangleEdgeEquals(DelaunayTriangleEdge a, DelaunayTriangleEdge b) {
    bool result = ((a[0] == b[0] && a[1] == b[1]) ||
                  (a[0] == b[1] && a[1] == b[0]));

    return result;
}

// @NOTE: assumes consistent winding
void TriangulateConvexPolygon(u32 vertCount, u32 *indices, u32 *indexCount, u32 vertOffset = 0, bool reverseWinding = false) {
    ASSERT(vertCount > 2);
    u32 support = 1 + vertOffset;
    
    while (support + 1 < vertCount + vertOffset) {
        u32 temp = support + 1;

        if (reverseWinding) {
            indices[*indexCount + 2] = vertOffset;
            indices[*indexCount + 1] = support;
            indices[*indexCount + 0] = temp;
        }
        else {
            indices[*indexCount + 0] = vertOffset;
            indices[*indexCount + 1] = support;
            indices[*indexCount + 2] = temp;
        }

        *indexCount += 3;
        
        support = temp;
    }
}

bool VectorsAreParallel(vec3 a, vec3 b) {
    return Dot(a, a) * Dot(b, b) - Dot(a, b) * Dot(a, b) == 0;
}

bool PointInFOV(vec3 apex, vec3 dir, float32 fov, vec3 p) {
    vec3 diff = Normalize(p - dir);
    float32 dot = Dot(dir, diff);
    return Dot(dir, diff) > fov;
}

// Add to the bottom of the geometry.h file

struct Rect {
    vec2 min;
    vec2 max;
};

Rect MakeRect(vec2 position, vec2 halfSize) {
    Rect r;
    r.min = position - halfSize;
    r.max = position + halfSize;
    return r;
}

// @NOTE: inclusive
bool PointRectTest(Rect r, vec2 p) {
    if (p.x < r.min.x || p.x > r.max.x) {
        return false;
    }

    if (p.y < r.min.y || p.y > r.max.y) {
        return false;
    }

    return (p.x >= r.min.x && p.x <= r.max.x) && (p.y >= r.min.y && p.y <= r.max.y);
}

Rect GlobalRect(vec2 position, Rect rect) {
    Rect result;

    result.min = rect.min + position;
    result.max = rect.max + position;

    return result;
}

inline bool TestPointAABB(vec2 p, vec2 min, vec2 max) {
    return (p.x > min.x && p.x < max.x) &&
        (p.y > min.y && p.y < max.y);
}

inline bool RaycastAABB(vec2 aabbMin, vec2 aabbMax, vec2 origin, vec2 direction, float32 *tMin, bool testInside = false, float32 epsilon = FLT_EPSILON) {
    *tMin = 0.0f;
    float32 tMax = INFINITY;

    for (int i = 0; i < 2; i++) {
        if (Abs(direction.data[i]) < epsilon) {
            if (origin.data[i] < aabbMin.data[i] || origin.data[i] > aabbMax.data[i]) { return false; }
        }
        else {
            float32 ood = 1.0f / direction.data[i];
            float32 t1 = (aabbMin.data[i] - origin.data[i]) * ood;
            float32 t2 = (aabbMax.data[i] - origin.data[i]) * ood;

            if (t1 > t2) {
                float32 temp = t1; t1 = t2; t2 = temp;
            }

            *tMin = Max(*tMin, t1);
            tMax = Min(tMax, t2);

            if (*tMin > tMax) { return false; }
        }
    }
    
    if (testInside && TestPointAABB(V2(origin.x, origin.y), aabbMin, aabbMax)) {
        *tMin = tMax;    
    }

    return true;
}

inline bool RaycastAABB(vec2 aabbMin, vec2 aabbMax, Ray2D ray, float32 *tMin, bool testInside = false, float32 epsilon = FLT_EPSILON) {
    *tMin = 0.0f;
    float32 tMax = INFINITY;

    for (int i = 0; i < 2; i++) {
        if (Abs(ray.direction.data[i]) < epsilon) {
            if (ray.origin.data[i] < aabbMin.data[i] || ray.origin.data[i] > aabbMax.data[i]) { return false; }
        }
        else {
            float32 ood = 1.0f / ray.direction.data[i];
            float32 t1 = (aabbMin.data[i] - ray.origin.data[i]) * ood;
            float32 t2 = (aabbMax.data[i] - ray.origin.data[i]) * ood;

            if (t1 > t2) {
                float32 temp = t1; t1 = t2; t2 = temp;
            }

            *tMin = Max(*tMin, t1);
            tMax = Min(tMax, t2);

            if (*tMin > tMax) { return false; }
        }
    }
    
    if (testInside && TestPointAABB(V2(ray.origin.x, ray.origin.y), aabbMin, aabbMax)) {
        *tMin = tMax;    
    }

    return true;
}


inline bool RaycastAABB(vec2 aabbMin, vec2 aabbMax, Ray3D ray, float32 *tMin, bool testInside = false, float32 epsilon = FLT_EPSILON) {
    *tMin = 0.0f;
    float32 tMax = INFINITY;

    for (int i = 0; i < 2; i++) {
        if (Abs(ray.direction.data[i]) < epsilon) {
            if (ray.origin.data[i] < aabbMin.data[i] || ray.origin.data[i] > aabbMax.data[i]) { return false; }
        }
        else {
            float32 ood = 1.0f / ray.direction.data[i];
            float32 t1 = (aabbMin.data[i] - ray.origin.data[i]) * ood;
            float32 t2 = (aabbMax.data[i] - ray.origin.data[i]) * ood;

            if (t1 > t2) {
                float32 temp = t1; t1 = t2; t2 = temp;
            }

            *tMin = Max(*tMin, t1);
            tMax = Min(tMax, t2);

            if (*tMin > tMax) { return false; }
        }
    }
    
    if (testInside && TestPointAABB(V2(ray.origin.x, ray.origin.y), aabbMin, aabbMax)) {
        *tMin = tMax;    
    }

    return true;
}

bool TestAABBAABB(vec2 minA, vec2 maxA, vec2 minB, vec2 maxB, vec2 *dir) {
    // Is the bug that the positions can be negative?
    float32 lengthX = Min(maxA.x, maxB.x) - Max(minA.x, minB.x);
    float32 lengthY = Min(maxA.y, maxB.y) - Max(minA.y, minB.y);

    // This tells us if there is separation on either axis
    if (lengthX < 0) { return false; }
    if (lengthY < 0) { return false; }
    // If we get here there is no separation, and we want to find the axis with the least length
    
    if (lengthX < lengthY) {
        if (maxB.x < maxA.x) {
            dir->x = Abs(maxB.x - minA.x);
        }
        else {
            dir->x = -Abs(minB.x - maxA.x);
        }
    }
    else {
        if (maxB.y < maxA.y) {
            dir->y = Abs(maxB.y - minA.y);
        }
        else {
            dir->y = -Abs(minB.y - maxA.y);
        }
    }

    return true;
}

bool TestAABBAABB(vec2 minA, vec2 maxA, vec2 minB, vec2 maxB) {
    // Is the bug that the positions can be negative?
    float32 lengthX = Min(maxA.x, maxB.x) - Max(minA.x, minB.x);
    float32 lengthY = Min(maxA.y, maxB.y) - Max(minA.y, minB.y);

    // This tells us if there is separation on either axis
    if (lengthX < 0) { return false; }
    if (lengthY < 0) { return false; }

    return true;
}

