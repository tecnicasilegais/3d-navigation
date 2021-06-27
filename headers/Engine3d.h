//
// Created by m on 26/06/2021.
//

#ifndef INC_3D_NAV_ENGINE3D_H
#define INC_3D_NAV_ENGINE3D_H

#include <GL/freeglut.h>
#include <image_libs/TextureClass.h>
#include <map>


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
    vector<string> faces;
    GLuint cubeMap,front,back,top,bottom,right,left;
    GameTextures();
    GLuint get(int n);
    void draw_building(int n);
    void draw_tex_floor(int n);
};

class GameObject
{
public:
    int model;
    bool step;
};


void draw_floor();
void draw_cube();
void DefineLuz(void);
void calc_point(Point &p, Point &out);
unsigned int loadCubemap(vector<std::string> faces);
#endif //INC_3D_NAV_ENGINE3D_H
