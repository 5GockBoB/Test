#pragma once

class Manifold;
class Collider;

typedef bool(*CollisionCallback)(Collider *a, Collider *b);

extern CollisionCallback Dispatch[Shape::eCount][Shape::eCount];

bool RaytoRay(Collider *a, Collider *b);
bool RaytoSphere(Collider *a, Collider *b);
bool RaytoCube(Collider *a, Collider *b);
bool RaytoCapsule(Collider *a, Collider *b);
bool SpheretoRay(Collider *a, Collider *b);
bool SpheretoSphere(Collider *a, Collider *b);
bool SpheretoCube(Collider *a, Collider *b);
bool SpheretoCapsule(Collider *a, Collider *b);
bool CubetoRay(Collider *a, Collider *b);
bool CubetoSphere(Collider *a, Collider *b);
bool CubetoCube(Collider *a, Collider *b);
bool CubetoCapsule(Collider *a, Collider *b);
bool CapsuletoRay(Collider *a, Collider *b);
bool CapsuletoSphere(Collider *a, Collider *b);
bool CapsuletoCube(Collider *a, Collider *b);
bool CapsuletoCapsule(Collider *a, Collider *b);
