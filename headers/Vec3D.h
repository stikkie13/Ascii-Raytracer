#pragma once
#include <math.h>
#include <iostream>

using namespace std;

class Vec3D{
public:
    float x,y,z;

    Vec3D(float x, float y, float z);

    void show(string label);

    void show(string label, float n);

    void show();

// -1 times every term
    Vec3D Minus() const;

// combine every element to element of the same type
    Vec3D add(Vec3D const &other) const;
    Vec3D operator+ (Vec3D const &other) const;

// subtract every element from element of the same type
    Vec3D sub(Vec3D const &other) const;
    Vec3D operator- (Vec3D const &other) const;

// multiply every element with scalar
    Vec3D mul(float l) const;
    Vec3D operator* (float l) const;

// divide every element with scalar
    Vec3D div(float l) const;

// get the length of the vector
    float norm() const;

// divide every element with the length of the vector
    Vec3D unit() const;

// get dot product of two vectors
    float dot(Vec3D const &other) const;

// get cross product of two vectors AyBz - AzBy, AzBx - AxBz, AxBy - AyBx
    Vec3D cross(Vec3D const &other) const;

// rotate vector by theta around axis x = 1, y = 2, z = 3
    Vec3D rotateBy(float theta, int axis) const;

};