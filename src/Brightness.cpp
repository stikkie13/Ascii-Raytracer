#include "../headers/Brightness.h"

wchar_t Brightness::getBrightness(float n){
    // int b = (int)round(n * ( ( sizeof(Uvalues) / sizeof(Uvalues[1]) ) -1 ));
    int b = (int)round(n*9);
    return values[b];
}
    
// unused and broken, maybe used in the future for multi-sampling
char Brightness::getBrightness(float y, float x){
    // n-round(n) = removing integers -> n*10 = first number behind seperator becomes the integer -> round(n) = get closest integer for index
    int indexX = (int)round((x - floor(x))*10.f);
    int indexY = (int)round((y - floor(y))*10.f);
    // get index based on the coverage in pixel
    int index = round((indexX*indexY)/10);
    char c = values[index];
    // printf("x: %f, rounded x: %f,  Xi: %d, y: %f, Yi: %d, I: %d, char: %c\n", x, floor(x), indexX, y, indexY, index, c);
    return c;
};