#include "primitives.hpp"

struct CollisionResult {
    bool colliding;
    vec2 point;
    float depth;
};
typedef struct CollisionResult CollisionResult;

CollisionResult getCollision(Circle c1, Circle c2);
//CollisionResult getCollision(Triangle t1, Triangle t2);

CollisionResult getCollision(Circle c, Triangle t);
//CollisionResult getCollision(Triangle t, Circle c);