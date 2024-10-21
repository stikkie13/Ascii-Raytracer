#pragma once
#include <iostream>
#include "Vec3D.h"
#include "HitInfo.h"
#include "Object.h"
#include <vector>

using namespace std;

class Object;

class Ray{
public:
    Vec3D origin = Vec3D(0, 0, 0);
    Vec3D direction = Vec3D(0, 0, 0);
    vector<Object *> *objects;
    float brightness = 1;
    int depth = 0;

    Ray (float xSup, float ySup, float zSup, float xDir, float yDir, float zDir, vector<Object*> &objects);
    Ray (Vec3D sup, Vec3D dir, vector<Object*> &objects);
    Ray (float xStart, float yStart, vector<Object*> &objects);

    Vec3D pointOnRay(float t) const;

    Ray getReflected(float t, Vec3D normal);

    float trace();

    void show();
};