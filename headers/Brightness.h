#pragma once
#include <math.h>

class Brightness{
public:
    wchar_t values[10] = {' ','.',':','-','=','+','*','#','%','@'};
    wchar_t Uvalues[5] = {' ', L'\u2591', L'\u2592', L'\u2593', L'\u2588'};

    wchar_t getBrightness(float n);
    char getBrightness(float y, float x);
};