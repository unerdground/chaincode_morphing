#ifndef PIXEL_H
#define PIXEL_H

#include "custom_exceptions.h"

//representation of pixel position
class pixel{
private:
    int x; // x-coordinate
    int y; // y-coordinate
public:
    int getx(){
        return x;
    }
    int gety(){
        return y;
    }

    void setx(int new_x){
        x = new_x;
    }
    void sety(int new_y){
        y = new_y;
    }

    pixel(){}
    pixel(int init_x, int init_y): x(init_x), y(init_y){}

    bool operator!= (const pixel& second){
        return ((x != second.x) || (y != second.y));
    }
    bool operator== (const pixel& second){
        return ((x == second.x) && (y == second.y));
    }
};

#endif // PIXEL_H
