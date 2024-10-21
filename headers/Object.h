#pragma once
#include "Vec3D.h"
#include "HitInfo.h"
#include "Ray.h"

class Ray;

class Object{
public:
    Vec3D center = Vec3D(0,0,0);

    Object (float x, float y, float z): center (x, y, z) {}
    
    virtual HitInfo hit(Ray &ray) = 0;
};