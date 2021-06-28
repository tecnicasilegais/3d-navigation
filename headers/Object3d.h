//
// Created by marcel on 27/06/2021.
//

#ifndef INC_3D_NAV_OBJECT3D_H
#define INC_3D_NAV_OBJECT3D_H

#include "Point.h"

class Color {
public:
    float r, g, b;
    Color() = default;
    Color(int rgb);
};

typedef struct
{
    Point p1, p2, p3;
    Color color;
} Triangle;

class Object3d {
private:
    Triangle *faces;
    unsigned int nFaces;
public:
    Object3d();
    unsigned int count();
    void readObject(const string& name);
    void drawObject();
};


#endif //INC_3D_NAV_OBJECT3D_H
