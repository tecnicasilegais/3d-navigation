//
// Created by marcel on 28/06/2021.
//

#include "headers/Bezier.h"

Curve3::Curve3(Point p1, Point p2, Point p3)
{
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
}
int Bezier::size() const
{
    return count;
}
void Bezier::addCurve(Curve3 c)
{
    curves.emplace_back(c);
    count++;
}
void Bezier::drawBezier()
{
    double DeltaT = 1.0/100;
    Point P;

    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    for(auto & curve : curves)
    {
        double t = 0.0;
        while(t<1.0)
        {
            P = calcBezier3(curve, t);
            glVertex3f(P.x, P.y, P.z);
            t += DeltaT;
        }
        P = calcBezier3(curve, 1.0); // faz o fechamento da curva
        glVertex3f(P.x, P.y, P.z);
    }
    glEnd();
}
Point calcBezier3(Curve3 &PC, double t)
{
    Point P;
    double UmMenosT = 1-t;

    P =  PC.p1 * UmMenosT * UmMenosT + PC.p2 * 2 * UmMenosT * t + PC.p3 * t*t;
    return P;
}
void drawBezier3Points(Curve3 &curve)
{
    double t=0.0;
    double DeltaT = 1.0/100;
    Point P;

    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    while(t<1.0)
    {
        P = calcBezier3(curve, t);
        glVertex3f(P.x, P.y, P.z);
        t += DeltaT;
    }
    P = calcBezier3(curve, 1.0); // faz o fechamento da curva
    glVertex3f(P.x, P.y, P.z);
    glEnd();
}
