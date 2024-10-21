#pragma once
#include "Vec3D.h"

#define NOTHING -1
#define FLOOR 0
#define SPHERE 1

struct HitInfo{
    bool didHit = false;
    float distance = 0;
    Vec3D normal = Vec3D(0,0,0);
    int object = NOTHING;
};

HitInfo hitInfo(bool h);
HitInfo hitInfo(bool h, float t, Vec3D n, int object);