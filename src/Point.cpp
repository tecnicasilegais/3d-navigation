//
// Initial class given by professor Marcio Sarroglia Pinho (PUCRS)
// Modified by Eduardo Andrade and Marcelo Heredia
//

#include "../headers/Point.h"

Point::Point()
{
    x = y = z = 0;
}

Point::Point(GLfloat x, GLfloat y, GLfloat z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Point::print() const
{
    cout << "(" << x << ", " << y << ", " << z << ")" << endl << flush;
}

string Point::get() const
{
    char ret [30];
    snprintf(ret, 30, "POSITION: (%.2f,%.2f,%.2f)",x, y, z);
    return string(ret);
}

bool Point::operator==(const Point &other) const
{
    return (x == other.x) && (y == other.y) && (z == other.z);
}

bool Point::operator<=(const Point &other) const
{
    return (x <= other.x) && (y <= other.y) && (z <= other.z);
}

bool Point::operator>=(const Point &other) const
{
    return (x >= other.x) && (y >= other.y) && (z >= other.z);
}

Point &Point::operator+=(const Point &other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}

Point &Point::operator-=(const Point &other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

void Point::rotateZ(GLfloat angle)
{
    float xr, yr;
    double anguloRad = angle * 3.14159265359/180.0;
    xr = x*cos(anguloRad) - y*sin(anguloRad);
    yr = x*sin(anguloRad) + y*cos(anguloRad);
    x = xr;
    y = yr;
}

void Point::rotateY(GLfloat angle)
{
    GLfloat xr, zr;
    double anguloRad = angle * 3.14159265359/180.0;
    xr =  x * cos(anguloRad) + z * sin(anguloRad);
    zr = -x * sin(anguloRad) + z * cos(anguloRad);
    x = xr;
    z = zr;
}

void Point::rotateX(GLfloat angle)
{
    float yr, zr;
    double anguloRad = angle * 3.14159265359/180.0;
    yr =  y*cos(anguloRad) - z*sin(anguloRad);
    zr =  y*sin(anguloRad) + z*cos(anguloRad);
    y = yr;
    z = zr;
}

Point operator*(const Point &p1, const Point &p2)
{
    Point res;
    res.x = p1.x * p2.x;
    res.y = p1.y * p2.y;
    res.z = p1.z * p2.z;
    return res;
}

Point operator*(const Point &p, const double d)
{
    Point res;
    res.x = p.x * (GLfloat) d;
    res.y = p.y * (GLfloat) d;
    res.z = p.z * (GLfloat) d;
    return res;
}

Point operator+(const Point &p1, const Point &p2)
{
    Point res;
    res.x = p1.x + p2.x;
    res.y = p1.y + p2.y;
    res.z = p1.z + p2.z;
    return res;
}

Point operator-(const Point &p1, const Point &p2)
{
    Point res;
    res.x = p1.x - p2.x;
    res.y = p1.y - p2.y;
    res.z = p1.z - p2.z;
    return res;
}

Point get_max(Point p1, Point p2)
{
    Point max;

    max.x = (p2.x > p1.x) ? p2.x : p1.x;
    max.y = (p2.y > p1.y) ? p2.y : p1.y;
    max.z = (p2.z > p1.x) ? p2.z : p1.z;
    return max;
}

Point get_min(Point p1, Point p2)
{
    Point min;

    min.x = (p2.x < p1.x) ? p2.x : p1.x;
    min.y = (p2.y < p1.y) ? p2.y : p1.y;
    min.z = (p2.z < p1.x) ? p2.z : p1.z;

    return min;
}

void rotatedY(Point &origin, GLfloat angle, Point &translate, Point &out)
{
    GLfloat xr, zr;
    double anguloRad = angle * 3.14159265359/180.0;
    xr = (origin.x) * cos(anguloRad) + (origin.z) * sin(anguloRad);
    zr = -(origin.x) * sin(anguloRad) + (origin.z) * cos(anguloRad);

    out.x = xr + translate.x;
    out.z = zr + translate.z;
}
Point copy(Point &other)
{
    return Point(other.x, other.y, other.z);
}

/**
 * Calcula o produto vetorial entre os vetores V1 e V2 e guarda em x,y,z de vResult
 */
void prod_vetorial(Point v1, Point v2, Point &vResult)
{
    vResult.x = v1.y * v2.z - (v1.z * v2.y);
    vResult.y = v1.z * v2.x - (v1.x * v2.z);
    vResult.z = v1.x * v2.y - (v1.y * v2.x);
}

void vet_unit(Point &p)
{
    float mod;
    mod = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);

    if(mod == 0)  return;

    p.x /=mod;
    p.y /=mod;
    p.z /=mod;
}
int ceil_int(GLfloat x)
{
    return (int)(ceil(x));
}
int floor_int(GLfloat x)
{
    return (int)(floor(x));
}