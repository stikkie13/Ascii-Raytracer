#include <iostream>
#include <cmath>
#include <vector>
#include <windows.h>
#include <chrono>
#include <thread>
#include <mutex>
#include "headers/Brightness.h"
#include "headers/Vec3D.h"
#include "headers/Object.h"
#include "headers/HitInfo.h"
#include "headers/Ray.h"

#define PI 3.14159265358979323846264338327950288
#define deg2rad (PI/180)
using namespace std;

// for readable access to height and width, overwritten in main
struct Resolution{
    int x = 80;
    int y = 40;
} res;

Brightness br;

class Sphere: public Object{
public:
    float radius = 0;

    Sphere (float x, float y, float z, float radius): Object(x, y, z){
        Sphere::radius = radius;
    }

    float getDistanceToIntersect(Ray &ray, float dist){
        float tc = center.sub(ray.origin).dot(ray.direction); // length of ray parallel to center
        float ti = sqrtf((radius*radius) - (dist*dist)); // length between intersection and tc
        return tc-ti; // distance to point on circle where ray intersects
    }

    HitInfo hit (Ray &ray) override {
        float dist = ray.origin.sub(center).cross(ray.direction).norm(); // smallest distance between ray and center
        if( dist > radius || getDistanceToIntersect(ray, dist) < 0){
            return hitInfo(false);
        }else{
            HitInfo HI;
            HI.didHit = true;
            HI.object = SPHERE;
            HI.distance = getDistanceToIntersect(ray, dist);
            HI.normal = (ray.pointOnRay(HI.distance)-center).unit();
            return HI;
        }

    }
};

class Floor: public Object{
public:
    // default height
    float Height = 0.f;

    Floor(float height): Object(0, 0, 0){
        Height = height;
    }
    Floor(): Object(0, 0, 0){
    }

    HitInfo hit(Ray &ray) override {
        float t = ( Height - ray.origin.y ) / ray.direction.y;
        if(t <= 0){
            return hitInfo(false);
        }
        Vec3D floorCoord = ray.pointOnRay(t);
        if(t > 15){
            return hitInfo(false);
        }
        
        int Z = floor(floorCoord.z *2);
        int X = floor(floorCoord.x *2);
        if( abs(Z%2) != abs(X%2) ){
            return hitInfo(true, t, Vec3D(0,1,0), FLOOR);
        }else{
            return hitInfo(false);
        }
    }
};

class RayScanner{
public:
    // default values
    float fov = 40;
    float aspectRatio = 1;
    float degreesPerPixel = 0;
    Vec3D origin    = Vec3D(0,1,0);
    Vec3D direction = Vec3D(1,0,0);
    Vec3D topleft = Vec3D(0,0,0);
    Vec3D forward = Vec3D(0,0,0);
    Vec3D left = Vec3D(0,0,0);
    Vec3D up = Vec3D(0,0,0);

    mutex frame_mutex;

    RayScanner(){
        aspectRatio = res.x/res.y;
        degreesPerPixel = fov/(float)res.x;
        direction = direction.unit();
    }


    vector<Object *> objects{
        new Sphere(10, 1, 1, 0.5),
        new Sphere(4.4, 0.4, -1.2, 0.4),
        new Sphere(4.4, 0, 1.2, 0.4),
        new Sphere(6.7, 1.15, -1.5, 0.2),
        new Floor(0)
    };

    void scanRow(vector<vector<float>>& frame, int y, float ScreenHeight, float ScreenWidth){
        float Yn = ( (float)y / (res.y-1) );
        Ray ray = Ray(origin, direction, objects);
        for (int x = 0; x < res.x; x++){
            float Xn = (float)x / (res.x-1);

            Vec3D pointlocal = topleft.add(Vec3D(0, -ScreenHeight*Yn, ScreenWidth*Xn)); // pixel location in camera space

            Vec3D PixelCoord = origin + (forward*pointlocal.x) + (up * (pointlocal.y*2)) + (left * pointlocal.z);

            ray.direction = (PixelCoord - origin).unit();

            float b = ray.trace();

            lock_guard<mutex> lock(frame_mutex); // no race conditions

            frame.at(y).at(x) = b;
        }
    }

    void scanCam(vector<vector<float>>& frame){
        float ScreenHeight = 1.f * tanf( (fov/2.f) * deg2rad ) * 2.f; // height of screen
        float ScreenWidth = ScreenHeight * aspectRatio; // width of screen
        topleft = Vec3D(1, ScreenHeight/2, -ScreenWidth/2); // top left location of screen compared to camera origin
        forward = direction.unit(); // vector facing forwards in local space, positive x-axis
        left = Vec3D(forward.z, 0, -forward.x).unit(); // vector facing to the left in local space, negative z-axis
        up = forward.cross(left); // vector facing up in local space, positive y-axis

        std::vector<std::thread> threads;
        threads.reserve(res.y);

        for (int y = 0; y < res.y; y++) {
            threads.emplace_back(&RayScanner::scanRow, std::ref(*this), ref(frame), y, ScreenHeight, ScreenWidth);

        }

        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        return;
    }

};

// converts brightness value to character and prints the frame to the terminal
void print_frame(vector<vector<float>>& frame){
    vector<wchar_t> frame_buffer;
    frame_buffer.reserve((res.x+1)*res.y);
    for (int y = 0; y < res.y; ++y) {
        for (int x = 0; x < res.x; ++x) {
            frame_buffer.push_back(br.getBrightness(frame.at(y).at(x)));
        }
        frame_buffer.push_back('\n');
    }
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &frame_buffer[0], frame_buffer.size(), nullptr, nullptr);
}

main(){
    chrono::time_point start = chrono::high_resolution_clock::now(); // timing check
    // init terminal ======
    if (true){
        CONSOLE_SCREEN_BUFFER_INFO csbi; // holds window size info
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi); // gets the terminal info
        res.x = csbi.dwMaximumWindowSize.X;
        res.y = csbi.dwMaximumWindowSize.Y -1;
    }
    // ====================

    RayScanner RayScanner;

    vector<vector<float>> emptyFrame(res.y, vector<float>(res.x, 0.f));
    // create empty frame filled with nothing
    vector<vector<float>> frame(res.y, vector<float>(res.x, 0.f));

    enum KeyInput{
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    RayScanner.scanCam(frame); // fill frame
    print_frame(frame);

    KeyInput prevInput = UP;
    KeyInput input = NONE;
    while (true){
        if(GetAsyncKeyState(VK_UP)){
            input = UP;
        }else if(GetAsyncKeyState(VK_DOWN)){
            input = DOWN;
        }else if(GetAsyncKeyState(VK_LEFT)){
            input = LEFT;
        }else if(GetAsyncKeyState(VK_RIGHT)){
            input = RIGHT;
        }else {
            input = NONE;
        }


        if (input == NONE){
            Sleep(100);
            prevInput = input;
            continue;
        }else if (input == LEFT){
            RayScanner.direction = RayScanner.direction.rotateBy(5, 2);
            RayScanner.scanCam(frame); // fill frame
            print_frame(emptyFrame);
            print_frame(frame);
            prevInput = input;
            Sleep(10);
            continue;
        }else if (input == RIGHT){
            RayScanner.direction = RayScanner.direction.rotateBy(-5, 2);
            RayScanner.scanCam(frame); // fill frame
            print_frame(emptyFrame);
            print_frame(frame);
            prevInput = input;
            Sleep(10);
            continue;
        }else if (input == UP){
            RayScanner.origin = RayScanner.origin + (RayScanner.direction * 0.1);
            RayScanner.scanCam(frame); // fill frame
            print_frame(emptyFrame);
            print_frame(frame);
            prevInput = input;
            Sleep(10);
            continue;
        }else if (input == DOWN){
            RayScanner.origin = RayScanner.origin + (RayScanner.direction * -0.1);
            RayScanner.scanCam(frame); // fill frame
            print_frame(emptyFrame);
            print_frame(frame);
            prevInput = input;
            Sleep(10);
            continue;
        }



    }

    chrono::time_point FillFrame = chrono::high_resolution_clock::now(); // timing check

    chrono::time_point PrintFrame = chrono::high_resolution_clock::now();

    if(false){
        int full_time = chrono::duration_cast<chrono::milliseconds>(PrintFrame - start).count();
        int FF = (float)chrono::duration_cast<chrono::milliseconds>(FillFrame - start).count();
        int PF = (float)chrono::duration_cast<chrono::milliseconds>(PrintFrame - FillFrame).count();

        printf("time from start (0):\nfill frame: %d", FF);
        printf("\nprint frame: %d", PF);
        printf("\ntotal time: %d", chrono::duration_cast<chrono::milliseconds>(PrintFrame - start).count());
    }
}

/*
https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays.html
https://www.youtube.com/watch?v=Qz0KTGYJtUk
https://github.com/SebLague/Ray-Tracing/blob/Episode01/Assets/Scripts/RayTracingManager.cs#L16
*/
