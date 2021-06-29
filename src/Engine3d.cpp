//
// Created by m on 26/06/2021.
//

#include <headers/Point.h>

#include <utility>
#include "headers/Engine3d.h"

/*
 * Game Texture class and methods
 */
GameTextures::GameTextures()
{
    textures[GRASS] = LoadTexture(GRASS_T);
    textures[CROSS] = LoadTexture(CROSS_T);
    textures[DL] = LoadTexture(DL_T);
    textures[DLR] = LoadTexture(DLR_T);
    textures[DR] = LoadTexture(DR_T);
    textures[LR] = LoadTexture(LR_T);
    textures[UD] = LoadTexture(UD_T);
    textures[UDL] = LoadTexture(UDL_T);
    textures[UDR] = LoadTexture(UDR_T);
    textures[UL] = LoadTexture(UL_T);
    textures[ULR] = LoadTexture(ULR_T);
    textures[UR] = LoadTexture(UR_T);
    front = LoadTexture("img/front.jpg");
    back = LoadTexture("img/back.jpg");
    top = LoadTexture("img/top.jpg");
    bottom = LoadTexture("img/bottom.jpg");
    right = LoadTexture("img/right.jpg");
    left = LoadTexture("img/left.jpg");
}
/*
 * Returns the texture
 * @param n -> texture number
 */
GLuint GameTextures::get(int n)
{
    return textures[n];
}

void GameTextures::draw_building(int n)
{
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->get(n));
    draw_cube();
    glDisable(GL_TEXTURE_2D);

}
void GameTextures::draw_tex_floor(int n)
{
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->get(n));

    draw_floor();

    glDisable(GL_TEXTURE_2D);
}

void GO3d::draw()
{
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    if(rotation != 0)
    {
        glRotatef(rotation, 0,1,0);
    }
    glScalef(scale,scale,scale);
    model.drawObject();
    glPopMatrix();
}

Building::Building(Point pos, const string& model3d, GLfloat scale, GLfloat rotation=0.0f)
{
    this->pos = pos;
    this->scale = scale;
    this->rotation = rotation;
    model.readObject(model3d);
}

Plane::Plane(Bezier route, const string& model3d, GLfloat scale)
{
    this->route = std::move(route);
    this->time = 0.0;
    this->scale = scale;
    model.readObject(model3d);

    this->pos = this->route.step(time);
}
Plane::Plane(Bezier route, Object3d &model3d, GLfloat scale)
{
    this->route = std::move(route);
    this->time = 0.0;
    this->scale = scale;
    model = model3d;

    this->pos = this->route.step(time);
}
void Plane::incr_time()
{
    time += (double)1/30;
    if(time >= route.size())
        time = 0;
    pos = route.step(time);
}
void Plane::draw()
{
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    if(rotation != 0)
    {
        glRotatef(rotation, floor(rot.x),floor(rot.y),floor(rot.z));
    }
    glScalef(scale,scale,scale);
    model.drawObject();
    glPopMatrix();
}

Fuel::Fuel(Point pos, Object3d &md)
{
    this->pos = pos;
    pos.y = 0.3;
    scale = 0.015;
    model = md;
}


Player::Player(Point pos)
{
    this->pos = pos;
    this->scale = 0.3f; //maintain object proportion
    this->rotation = 0;
    this->fuel = MAX_FUEL;
    cam = Camera(pos);
    model.readObject(CAR);
    speed = (GLfloat) (S_SIZE) / P_SPEED;
    moving = false;
    this->dir = 1;
}
/*
 * returns old position
 */
Point Player::walk_mru(double dt)
{
    // Position = Position0 + speed * time (* direction)
    Point old = copy(pos);
    Point S;
    S.x = (GLfloat)dt * this->speed * ((GLfloat)dir * cam.front.x);
    S.z = (GLfloat)dt * this->speed * ((GLfloat)dir * cam.front.z);

    this->pos.x += S.x;
    this->pos.z += S.z;

    moving = false;

    return old;
}
/*
 * Rotates player in counter-clockwise
 * Uses the predefined rotation_incr
 * Limits to max_rotation
 */
void Player::rotate_l()
{
    rotation += rotation_incr;
}
/*
 * Rotates player in clockwise
 * Uses the predefined rotation_incr
 * Limits to max_rotation
 */
void Player::rotate_r()
{
    rotation -= rotation_incr;
}
void Player::rotate_camera_l()
{
    cam.rotation_h += rotation_incr;
}
void Player::rotate_camera_r()
{
    cam.rotation_h -= rotation_incr;
}
void Player::rotate_camera_u()
{
    cam.rotation_v += 2;
}
void Player::rotate_camera_d()
{
    cam.rotation_v -= 2;
}
void Player::reset_camera()
{
    cam.rotation_h = 0;
    cam.rotation_v = 0;
}
void Player::walk_forward()
{
    moving = dir = true;
}
void Player::walk_backward()
{
    moving = true;
    dir = -1;
}
void Player::fill()
{
    fuel = (fuel + S_GALLON > MAX_FUEL) ? MAX_FUEL : fuel + 20;
}
GLfloat Player::fuel_level() const
{
    return (fuel / MAX_FUEL) * 100;
}
void Player::draw()
{
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    if(rotation != 0)
    {
        glRotatef(rotation, 0,1,0);
    }
    glScalef(scale,scale,scale);
    cam.update(origin, pos, rotation);
    model.drawObject();

    glPopMatrix();
}


bool handle_ambient_collision(Player &p, GameObject (*ambient)[S_SIZE][S_SIZE])
{
    auto x = p.pos.x;
    auto z = p.pos.z;

    if(x < 0 ||  x > 30 || z < 0 || z > 30)
    {
        return true;
    }

    if((*ambient)[ceil_int(x)][ceil_int(z)].tex == GRASS /*||
            (*ambient)[floor_int(x)][floor_int(z)].tex == GRASS ||
            (*ambient)[ceil_int(x)][floor_int(z)].tex == GRASS ||
            (*ambient)[floor_int(x)][ceil_int(z)].tex == GRASS*/)
    {
        return true;
    }

    return false;
}

void handle_fuel_collision(Player &p, vector<Fuel> &fs)
{
    for(auto &f:fs)
    {
        if(f.active && collided(p.pos, f.pos))
        {
            p.fill();
            f.active = false;
        }
    }
}

bool collided(Point p1, Point p2)
{
    if(abs(p1.x - p2.x) <= 0.5 &&
        abs(p1.z - p2.z) <= 0.5)
    {
        return true;
    }
    return false;
}
void draw_floor()
{

    glBegin ( GL_QUADS );
    glNormal3f(0,1,0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.0f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.0f,  0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.0f,  0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

    glBegin ( GL_LINE_STRIP ); // desenha borda do ladrilho
    glNormal3f(0,1,0);
    glVertex3f(-0.5f,  0.0f, -0.5f);
    glVertex3f(-0.5f,  0.0f,  0.5f);
    glVertex3f( 0.5f,  0.0f,  0.5f);
    glVertex3f( 0.5f,  0.0f, -0.5f);
    glEnd();

}
void draw_cube()
{
    glBegin ( GL_QUADS );
    // Front Face
    glNormal3f(0,0,1);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f,  1.0f,  1.0f);

    // Back Face
    glNormal3f(0,0,-1);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

    // Top Face
    glNormal3f(0,1,0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

    // Bottom Face
    glNormal3f(0,-1,0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

    // Right face
    glNormal3f(1,0,0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

    // Left Face
    glNormal3f(-1,0,0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);  glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
}

void DefineLuz(void)
{
    // Define cores para um objeto dourado
    GLfloat LuzAmbiente[]   = {0.4, 0.4, 0.4f } ;
    GLfloat LuzDifusa[]   = {0.7, 0.7, 0.7};
    GLfloat LuzEspecular[] = {0.9f, 0.9f, 0.9 };
    GLfloat PosicaoLuz0[]  = {0.0f, 3.0f, 5.0f };  // Posi��o da Luz
    GLfloat Especularidade[] = {1.0f, 1.0f, 1.0f};

    // ****************  Fonte de Luz 0

    glEnable ( GL_COLOR_MATERIAL );

    // Habilita o uso de ilumina��o
    glEnable(GL_LIGHTING);

    // Ativa o uso da luz ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LuzAmbiente);
    // Define os parametros da luz n�mero Zero
    glLightfv(GL_LIGHT0, GL_AMBIENT, LuzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, LuzDifusa  );
    glLightfv(GL_LIGHT0, GL_SPECULAR, LuzEspecular  );
    glLightfv(GL_LIGHT0, GL_POSITION, PosicaoLuz0 );
    glEnable(GL_LIGHT0);

    // Ativa o "Color Tracking"
    glEnable(GL_COLOR_MATERIAL);

    // Define a reflectancia do material
    glMaterialfv(GL_FRONT,GL_SPECULAR, Especularidade);

    // Define a concentra��oo do brilho.
    // Quanto maior o valor do Segundo parametro, mais
    // concentrado ser� o brilho. (Valores v�lidos: de 0 a 128)
    glMateriali(GL_FRONT,GL_SHININESS,51);

}

void drawCubeSk (Point &position, GameTextures &gt)
{
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glColor3f(1,1,1);
    glTranslatef(position.x, 0, position.z);
    glScalef(33,33,33);

    glBindTexture (GL_TEXTURE_2D, gt.front);
    glBegin ( GL_QUADS );
    // Front Face
    glNormal3f(0,0,1);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f,  1.0f,  1.0f);
    glEnd();

    glBindTexture (GL_TEXTURE_2D, gt.back);
    glBegin ( GL_QUADS );
    // Back Face
    glNormal3f(0,0,-1);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();

    glBindTexture (GL_TEXTURE_2D, gt.top);
    glBegin ( GL_QUADS );
    // Top Face
    glNormal3f(0,1,0);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f( 1.0f,  1.0f, -1.0f);
    glEnd();

    glBindTexture (GL_TEXTURE_2D, gt.bottom);
    glBegin ( GL_QUADS );
    // Bottom Face
    glNormal3f(0,-1,0);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f);glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-1.0f, -1.0f,  1.0f);
    glEnd();

    glBindTexture (GL_TEXTURE_2D, gt.right);
    glBegin ( GL_QUADS );
    // Right face
    glNormal3f(1,0,0);
    glTexCoord2f(1.0f, 0.0f);glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f);glVertex3f( 1.0f, -1.0f,  1.0f);
    glEnd();

    glBindTexture (GL_TEXTURE_2D, gt.left);
    glBegin ( GL_QUADS );
    // Left Face
    glNormal3f(-1,0,0);
    glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f);glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}