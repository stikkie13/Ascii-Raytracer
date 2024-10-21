#include "../headers/Ray.h"

Ray::Ray (float xSup, float ySup, float zSup, float xDir, float yDir, float zDir, vector<Object*> &objects){
    origin = Vec3D(xSup, ySup, zSup);
    direction = Vec3D(xDir, yDir, zDir);
    Ray::objects = &objects;
}
Ray::Ray (Vec3D sup, Vec3D dir, vector<Object*> &objects){
    origin = sup;
    direction = dir;
    Ray::objects = &objects;
}
Ray::Ray (float xStart, float yStart, vector<Object*> &objects){
    Ray::objects = &objects;
}


Vec3D Ray::pointOnRay(float t) const {
    return origin.add( direction.mul(t) );
}

Ray Ray::getReflected(float t, Vec3D normal){
    Vec3D orig = pointOnRay(t);
    Vec3D dir = direction + (normal * direction.dot(normal) * - 2); // direction of the relfected ray
    return Ray(orig, dir, *objects); // reflected ray
}


float Ray::trace(){
    brightness = 1; // reset brightness because the same ray is constantly reused
    if(depth > 1){
        return 0;
    }
    for (int i = 0; i < objects->size(); i++){
        HitInfo hitinfo = objects->at(i)->hit(*this);
        if (hitinfo.didHit == false){
            continue;
        }
        if (hitinfo.object == SPHERE){
            if (pointOnRay(hitinfo.distance).y < 0){ // allow sphere clipping in the floor
                continue;
            }
            brightness = 0.3;
            Ray reflected = getReflected(hitinfo.distance, hitinfo.normal);

            reflected.depth = depth +1;
            float br = reflected.trace();
            if (br != 0){
                brightness += br;
            }
            return brightness;
        }
        if (hitinfo.object == FLOOR){
            return brightness;
        }
    }
    return 0; // hit nothing
}


void Ray::show(){
    printf(
    "\nRay:\n"
    " |Origin|Direction\n"
    "X|%g\t|%g\n"
    "Y|%g\t|%g\n"
    "Z|%g\t|%g\n", 
    origin.x, direction.x,
    origin.y, direction.y,
    origin.z, direction.z);
}
