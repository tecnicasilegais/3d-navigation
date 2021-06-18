//
// Initial class given by professor Marcio Sarroglia Pinho (PUCRS)
// Modified by Eduardo Andrade and Marcelo Heredia
//

#ifndef Point_hpp
#define Point_hpp

#include <iostream>
#include <cmath>

#ifdef WIN32

#include <windows.h>
#include <GL/glut.h>

#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__

#include <GL/freeglut.h>
#include<bits/stdc++.h>

#endif

using namespace std;

class Point
{
public:
    GLfloat x, y, z;
    Point();
    Point(GLfloat x, GLfloat y, GLfloat z = 0);
    bool operator==(const Point &other) const;
    bool operator<=(const Point &other) const;
    bool operator>=(const Point &other) const;
    Point &operator+=(const Point &other);
    Point &operator-=(const Point &other);
    void print() const;
};

Point operator*(const Point &p2);
Point operator*(const Point &p, const double d);
Point operator+(const Point &p1, const Point &p2);
Point operator-(const Point &p1, const Point &p2);
Point get_min(Point p1, Point p2);
Point get_max(Point p1, Point p2);

#endif /* Ponto_hpp */
