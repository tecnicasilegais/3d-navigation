//
// Created by m on 26/06/2021.
//

#ifndef INC_3D_NAV_ENGINE3D_H
#define INC_3D_NAV_ENGINE3D_H

#include <GL/freeglut.h>
#include <image_libs/TextureClass.h>
#include <map>
#include "Object3d.h"
#include "Camera.h"


#define CROSS_T "textures/CROSS.png"
#define DL_T "textures/DL.png"
#define DLR_T "textures/DLR.png"
#define DR_T "textures/DR.png"
#define LR_T "textures/LR.png"
#define GRASS_T "textures/grass.png"
#define UD_T "textures/UD.png"
#define UDL_T "textures/UDL.png"
#define UDR_T "textures/UDR.png"
#define UL_T "textures/UL.png"
#define ULR_T "textures/ULR.png"
#define UR_T "textures/UR.png"

#define CAR "models/Car.tri"
#define FUEL "models/fuel.tri"
#define P_TIME 10
#define S_SIZE 30

#define GRASS 0
#define CROSS 1
#define DL 4
#define DLR 3
#define DR 2
#define LR 5
#define UD 7
#define UDL 8
#define UDR 9
#define UL 10
#define ULR 11
#define UR 12

enum View{
    ThirdPerson,
    Floating
};

class GameTextures
{
private:
    map<int, GLuint> textures;

public:
    GLuint front,back,top,bottom,right,left;
    GameTextures();
    GLuint get(int n);
    void draw_building(int n);
    void draw_tex_floor(int n);
};

class GameObject
{
public:
    int tex;
    bool step;
};

class GO3d:GameObject
{
public:
    Point pos;
    GLfloat scale{};
    Object3d model;
    virtual void draw();
};

class Fuel:public GO3d
{
public:
    explicit Fuel(Point pos);
};

class Player:public GO3d
{
public:
    explicit Player(Point pos);
    Camera cam, origin;
    GLfloat speed, rotation;
    GLfloat rotation_incr = 10.0f;
    bool moving;
    int dir;
    void walk_mru(double dt);
    void walk_forward();
    void walk_backward();
    void rotate_r();
    void rotate_l();
    void rotate_camera_r();
    void rotate_camera_l();
    void draw() override;
};

Point calcBezier3(Point *PC, double t);
void drawBezier3Points(vector<Point> &curve);

void draw_floor();
void draw_cube();
void DefineLuz(void);
unsigned int loadCubemap(vector<std::string> faces);
void drawCubeSk (Point &position, GameTextures &gt);
#endif //INC_3D_NAV_ENGINE3D_H
