//
// Created by Eduardo Andrade e Marcelo Heredia on 20/05/2021.
//

#ifndef PINE_SHOOTER_ENGINE_H
#define PINE_SHOOTER_ENGINE_H

#ifdef WIN32

#include <windows.h>
#include <GL/freeglut.h>

#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__

#include <GL/freeglut.h>

#endif
using namespace std;

#include <iostream>
#include <image_libs/TextureClass.h>
#include <map>
#include <tuple>
#include <cmath>
#include "Point.h"
#include "BoundingBox.h"

//Ortho

#define ORTHO_X 240
#define ORTHO_Y 130
#define FLOOR_H 19.6
#define BB_GL_COLOR glColor3f(1,0,0)
#define O_TIME 15
#define GRAVITY 40
#define ENEMY_SPEED_STEP 1
#define MIN_STRENGTH 7
#define MAX_STRENGTH 10

#define MIN_ESHOOT 3
#define MAX_ESHOOT 10

#define HORIZONTAL_SPRITE 0
#define VERTICAL_SPRITE 1

#define LEFT -1
#define RIGHT 1

#define BG_FILE "img/background.png"
#define EXPLOSION_T "img/Explosion.png"
#define BUILD1_T "img/build1.png"
#define BUILD2_T "img/build2.png"
#define BUILD3_T "img/build3.png"
#define BUILD4_T "img/build4.png"
#define PW_STICK_T "img/stick.png"
#define PW_SPIRAL_T "img/pinwheel.png"
#define PLAYER_FILE "img/squirrel.png"
#define PLAYER_AMMO_T "img/pine.png"
#define EAGLE_T "img/eagle.png"
#define RAVEN_T "img/raven.png"
#define OWL_T "img/owl.png"
#define ENEMY_AMMO_T "img/bird_poop.png"
#define HEALTH_T "img/life.png"
#define MSG_WIN_T "img/win.png"
#define MSG_LOSE_T "img/lose.png"

#define PLAYER 1
#define PLAYER_AMMO 2
#define EAGLE 3
#define RAVEN 4
#define OWL 5
#define ENEMY_AMMO 6
#define EXPLOSION 7
#define BUILD1 8
#define BUILD2 9
#define BUILD3 10
#define BUILD4 11
#define PW_STICK 12
#define PW_SPIRAL 13
#define HEALTH 14
#define MSG_WIN 15
#define MSG_LOSE 16

struct TexData
{
    GLuint texture;
    Point proportion;
};

TexData store_tex(GLuint tex, Point p);

struct enemy_data
{
    Point position;
    float speed;
    int direction;
};

enemy_data store_enemy(Point pos, float speed, int dir);

class GameTextures
{
private:
    map<int, TexData> textures;
public:
    GameTextures();
    void draw_texture(int n);
    void draw_sprite(int n, int orientation, GLfloat sprite, GLfloat total_sprites);
    GLuint get(int n);
    Point get_proportion(int n);
    Point get_scaled(int n, GLfloat scale);

};

class GameObject
{
public:
    BoundingBox bb, root;
    GLfloat rotation = 0;
    GLfloat rotation_incr = 0;
    Point position, speed;
    Point scale = Point(1, 1);
    bool active = true;
    bool moving = false;
    int model, sprite = 0, n_sprites = 0, s_orientation;
    virtual bool collided(GameObject &other, Point &coll_pos);
    void handle_rotation() const;
    void walk_mru(double dt, Point &direction);
    virtual void draw(GameTextures &gt, bool debug);
};

class Message : public GameObject
{
public:
    explicit Message(int model, Point position, Point scale);
};

class Explosion : public GameObject
{
public:
    explicit Explosion(Point position);
    int slowness = 3;
    int repetitions = 0;
    bool ended = false;
    void draw(GameTextures &gt);
};

class Projectile : public GameObject
{
private:
    Point direction, origin;
    GLfloat animation = 0;
public:
    Projectile();
    explicit Projectile(int type, Point pos, Point scale, Point direction, Point speed);
    void oblique_throw(double dt);
};

class Building : public GameObject
{
public:
    int health;
    explicit Building(int model, Point pos, Point scale, int n_sprites = 3, int s_orientation = VERTICAL_SPRITE);
    void draw(GameTextures &gt, bool debug) override;
    bool collided(GameObject &other, Point &coll_pos) override;
};

class Player : public GameObject
{
private:
    GLfloat str = MIN_STRENGTH;
public:
    explicit Player(int model, Point pos, Point scale);
    Point move_dir;
    Point aim = Point(0, 1);
    int health = 3;
    GLfloat max_rotation = 80.0f;
    bool aiming = false;
    void display_health(GameTextures &gt) const;
    void rotate_l();
    void rotate_r();
    void walk_mru(double dt);
    void walk_l();
    void walk_r();
    void increase_str();
    void decrease_str();
    Projectile shoot(GameTextures &gt);
    void draw_aim();
    void draw(GameTextures &gt, bool debug) override;
    bool collided(GameObject &other);
};

class Enemy : public GameObject
{
public:
    GLfloat str = 2;
    Point dir;
    Point aim;
    GLfloat acum = 0;
    GLfloat shoot_time;
    explicit Enemy(int model, Point pos, Point scale, float speed = 1, int direction = LEFT);
    void walk_mru(double dt);
    bool shoot(double dt, GameTextures &gt, Projectile &p);
};

void display_background(ImageClass &bg);
void draw_floor();
void draw_square(const Point &min, const Point &max);
vector<enemy_data> enemy_positions();
Point calc_ob_throw(Point &p, double t, Point &speed, Point &aim);
Point adjust_aim(Point &p, GLfloat rotation);
#endif //PINE_SHOOTER_ENGINE_H
