#include "../headers/Vec3D.h"

#define PI 3.14159265358979323846264338327950288
#define deg2rad PI/180

using namespace std;

Vec3D::Vec3D(float x, float y, float z){
    Vec3D::x = x;
    Vec3D::y = y;
    Vec3D::z = z;
}

void Vec3D::show(string label){
    printf("\n%s\nx: %g\ny: %g\nz: %g\n", label.c_str(), x, y, z);
}
void Vec3D::show(string label, float n){
    printf("\nscalar %s:\n%f\n", label.c_str(), n);
}
void Vec3D::show(){
    cout<<endl;
}

// -1 times every term
Vec3D Vec3D::Minus() const {
    return Vec3D(x*-1,
                 y*-1,
                 z*-1
    );
}

// combine every element to element of the same type
Vec3D Vec3D::add(Vec3D const &other) const {
    return Vec3D(x+other.x,
                 y+other.y,
                 z+other.z);
}
Vec3D Vec3D::operator+ (Vec3D const &other) const {
    return Vec3D(x+other.x,
                 y+other.y,
                 z+other.z);
}

// subtract every element from element of the same type
Vec3D Vec3D::sub(Vec3D const &other) const {
    return Vec3D(x-other.x,
                 y-other.y,
                 z-other.z);
}
Vec3D Vec3D::operator- (Vec3D const &other) const {
    return Vec3D(x-other.x,
                 y-other.y,
                 z-other.z);
}

// multiply every element with scalar
Vec3D Vec3D::mul(float l) const {
    return Vec3D(x*l,
                 y*l,
                 z*l);
}
Vec3D Vec3D::operator* (float l) const {
    return Vec3D(x*l,
                 y*l,
                 z*l);
}

// divide every element with scalar
Vec3D Vec3D::div(float l) const {
    return Vec3D(x/l,
                 y/l,
                 z/l);
}

// get the length of the vector
float Vec3D::norm() const {
    return sqrt(pow(x, 2)+pow(y, 2)+pow(z, 2));
}

// divide every element with the length of the vector
Vec3D Vec3D::unit() const {
    return Vec3D(x/norm(),
                 y/norm(),
                 z/norm());
}

// get dot product of two vectors
float Vec3D::dot(Vec3D const &other) const {
    return x*other.x + y*other.y + z*other.z;
}

// get cross product of two vectors AyBz - AzBy, AzBx - AxBz, AxBy - AyBx
Vec3D Vec3D::cross(Vec3D const &other) const {
    return Vec3D((y*other.z - z*other.y),
                 (z*other.x - x*other.z),
                 (x*other.y - y*other.x));
}

// rotate vector by theta around axis x = 1, y = 2, z = 3
Vec3D Vec3D::rotateBy(float theta, int axis) const {
    theta = -theta * deg2rad;
    Vec3D rotated(0,0,0);
    switch (axis){
        // rotate around x
        case 1:
            rotated.x = x;
            rotated.y = y*cosf(theta) - z*sinf(theta);
            rotated.z = y*sinf(theta) + z*cosf(theta);
            break;
        // rotate around y
        case 2:
            rotated.x = x*cosf(theta) + z*sinf(theta);
            rotated.y = y;
            rotated.z = -x*sinf(theta) + z*cosf(theta);
            break;
        // rotate around z
        case 3:
            rotated.x = x*cosf(theta) - y*sinf(theta);
            rotated.y = x*sinf(theta) + y*cosf(theta);
            rotated.z = z;
            break;
        default:
            break;
    }
    return rotated;
}