//
// Created by marcel on 27/06/2021.
//

#include "headers/Camera.h"

Camera::Camera()
{
    observer = Point(0.0f, 0.7f, -0.7f); //obs
    front = Point(0.0f,0.0f,1.0f); //car direction
    target = Point(0.0f, 0.0f, 15.0f);//target
    rotation=0;
}

Camera::Camera(Point pos)
{
    observer = Point(pos.x, pos.y + 0.7f, pos.z - 0.7f); //obs
    front = Point(0.0f,0.0f,1.0f); //car direction
    target = Point(pos.x, pos.y + 0.7f, pos.z + 15.0f);//target
    rotation = 0;
}

void Camera::update(Camera &origin, Point &position, GLfloat car_rotation)
{
    front = copy(origin.front);
    front.rotateY(car_rotation);

    observer = copy(origin.observer);
    if(car_rotation != 0)
        observer.rotateY(car_rotation);
    observer += position;

    target = copy(origin.target);
    if((bool)car_rotation || (bool)rotation)
        target.rotateY(car_rotation+rotation);
    target += position;
}

void calc_point(Point &p, Point &out)
{

    GLfloat ponto_novo[4];
    GLfloat matriz_gl[4][4];
    int i;

    glGetFloatv(GL_MODELVIEW_MATRIX, &matriz_gl[0][0]);

    for (i = 0; i < 4; i++)
    {
        ponto_novo[i] = matriz_gl[0][i] * p.x +
                        matriz_gl[1][i] * p.y +
                        matriz_gl[2][i] * p.z +
                        matriz_gl[3][i];
    }
    out.x = ponto_novo[0];
    out.y = ponto_novo[1];
    out.z = ponto_novo[2];
}