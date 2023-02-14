#include "primitives.hpp"

struct CollisionResult {
    bool colliding;
    vec2 normal;
    vec2 point;
    float depth;
};
typedef struct CollisionResult CollisionResult;

CollisionResult getCollision(Circle a, Circle b);
CollisionResult getCollision(Triangle a, Triangle b);

CollisionResult getCollision(Circle c, vec2 p);
CollisionResult getCollision(Circle c, Triangle t);
CollisionResult getCollision(Triangle t, Circle c);