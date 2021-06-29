//
// Created by marcel on 28/06/2021.
//

#ifndef INC_3D_NAV_BEZIER_H
#define INC_3D_NAV_BEZIER_H


#include "Point.h"
#include "ListaDeCoresRGB.h"

class Curve3
{
public:
    Curve3(Point p1, Point p2, Point p3);
    Point p1, p2, p3;
};

class Bezier
{
    int count = 0;
public:
    vector<Curve3> curves;
    int size() const;
    void addCurve(Curve3 c);
    Point step(double t);
    void drawBezier();
};

Point calcBezier3(Curve3 &PC, double t);
void drawBezier3Points(Curve3 &curve);

#endif //INC_3D_NAV_BEZIER_H
