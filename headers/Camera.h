//
// Created by marcel on 27/06/2021.
//

#ifndef INC_3D_NAV_CAMERA_H
#define INC_3D_NAV_CAMERA_H


#include "Point.h"

class Camera
{
public:
    Camera();
    explicit Camera(Point pos);
    Point front, target, observer;
    GLfloat rotation_h, rotation_v;
    void update(Camera &origin, Point &position, GLfloat car_rotation);
    void update2(Camera &origin, GLfloat car_rotation);
};

void calc_point(Point &p, Point &out);
#endif //INC_3D_NAV_CAMERA_H
