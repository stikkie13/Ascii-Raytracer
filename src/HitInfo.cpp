#include "../headers/HitInfo.h"


HitInfo hitInfo(bool h){
    HitInfo hi;
    hi.didHit = h;
    return hi;
}

HitInfo hitInfo(bool h, float t, Vec3D n, int object){
    HitInfo hi;
    hi.didHit = h;
    hi.distance = t;
    hi.normal = n;
    hi.object = object;
    return hi;
}