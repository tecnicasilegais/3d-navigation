#include "headers/Engine.h"

/*
 * creates a TexData instance
 */
TexData store_tex(GLuint tex, Point p)
{
    TexData t;
    t.texture = tex;
    t.proportion = p;
    return t;
}

enemy_data store_enemy(Point pos, float speed, int dir)
{
    enemy_data e;
    e.position = pos;
    e.speed = speed;
    e.direction = dir;
    return e;
}

/*
 * Game Texture class and methods
 */
GameTextures::GameTextures()
{
    textures[PLAYER] = store_tex(LoadTexture(PLAYER_FILE), Point(1, 1.2));
    textures[PLAYER_AMMO] = store_tex(LoadTexture(PLAYER_AMMO_T), Point(1, 1.407));
    textures[RAVEN] = store_tex(LoadTexture(RAVEN_T), Point(1.47727, 1));
    textures[EAGLE] = store_tex(LoadTexture(EAGLE_T), Point(1.47727, 1));
    textures[OWL] = store_tex(LoadTexture(OWL_T), Point(1.47727, 1));
    textures[ENEMY_AMMO] = store_tex(LoadTexture(ENEMY_AMMO_T), Point(1, 3.3));
    textures[BUILD1] = store_tex(LoadTexture(BUILD1_T), Point(1, 1.42857));
    textures[BUILD2] = store_tex(LoadTexture(BUILD2_T), Point(1, 1.42857));
    textures[BUILD3] = store_tex(LoadTexture(BUILD3_T), Point(1, 1.42857));
    textures[BUILD4] = store_tex(LoadTexture(BUILD4_T), Point(1, 1.42857));
    textures[PW_STICK] = store_tex(LoadTexture(PW_STICK_T), Point(1, 7));
    textures[PW_SPIRAL] = store_tex(LoadTexture(PW_SPIRAL_T), Point(1, 1));
    textures[EXPLOSION] = store_tex(LoadTexture(EXPLOSION_T), Point(1, 1));
    textures[HEALTH] = store_tex(LoadTexture(HEALTH_T), Point(5, 1));
    textures[MSG_WIN] = store_tex(LoadTexture(MSG_WIN_T), Point(7.8, 1));
    textures[MSG_LOSE] = store_tex(LoadTexture(MSG_LOSE_T), Point(8.7, 1));
}

/*
 * Returns the texture
 * @param n -> texture number
 */
GLuint GameTextures::get(int n)
{
    return textures[n].texture;
}

/*
 * returns the proportion of a texture
 * @param n -> texture number
 */
Point GameTextures::get_proportion(int n)
{
    return textures[n].proportion;
}

/*
 * Returns proportional scale of a texture
 * @param n -> number of initialized texture
 * @param scale -> scale to multiply both proportions
 */
Point GameTextures::get_scaled(int n, GLfloat scale)
{
    return textures[n].proportion * scale;
}

void GameTextures::draw_texture(int n)
{
    Point min = Point(0, 0);
    Point max = Point(1, 1);

    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->get(n));

    draw_square(min, max);

    glDisable(GL_TEXTURE_2D);
}

void GameTextures::draw_sprite(int n, int orientation, GLfloat sprite, GLfloat total_sprites)
{
    Point min = Point(0, 0);
    Point max = Point(1, 1);
    if (orientation == VERTICAL_SPRITE)
    {
        min.y = sprite / total_sprites;
        max.y = (sprite + 1) / total_sprites;
    }
    else //HORIZONTAL_SPRITE
    {
        min.x = sprite / total_sprites;
        max.x = (sprite + 1) / total_sprites;
    }

    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->get(n));

    draw_square(min, max);

    glDisable(GL_TEXTURE_2D);
}

bool GameObject::collided(GameObject &other, Point &coll_pos)
{
    return bb.collision_detect(other.bb, coll_pos);
}

/*
 * Handles a GameObject sequential rotation
 * Increments the rotation everytime it gets displayed
 * Subtracts 360 to keep it low
 */
void GameObject::handle_rotation() const
{
    if (rotation != 0)
    {
        glRotatef(rotation, 0, 0, 1);
    }
}

void GameObject::draw(GameTextures &gt, bool debug)
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);

    this->handle_rotation();
    glScalef(scale.x, scale.y, 1);

    if (n_sprites > 0)
    {
        gt.draw_sprite(model, s_orientation, (GLfloat) sprite, (GLfloat) n_sprites);
    }
    else
    {
        gt.draw_texture(model);
    }
    bb.update(root);

    glPopMatrix();
    if (debug && active)
    {
        BB_GL_COLOR;
        bb.draw();
    }

}

/*
 * Calculates movement MRU of given object
 * If object is outside the GL Ortho, it will not be updated
 */
void GameObject::walk_mru(double dt, Point &direction)
{
    // Position = Position0 + speed * time (* direction)
    Point S;
    S.x = dt * this->speed.x * direction.x;
    this->position.x += S.x;
}

Message::Message(int model, Point position, Point scale)
{
    this->model = model;
    this->position = position;
    this->scale = scale;
}

/*
 * Building Constructor
 * @param model Number of the model stored in GameTextures
 * @param n_sprites Number of sprites in the texture (default = 3)
 * @param s_orientation VERTICAL_SPRITE or HORIZONTAL_SPRITE (default = Vertical)
 */
Building::Building(int model, Point pos, Point scale, int n_sprites, int s_orientation)
{
    this->model = model;
    this->n_sprites = n_sprites;
    this->s_orientation = s_orientation;
    this->position = pos;
    this->scale = scale;
    health = 2;
    sprite = health;
    rotation = 0;
    rotation_incr = 0;
}

bool Building::collided(GameObject &other, Point &coll_pos)
{
    auto c = GameObject::collided(other, coll_pos);
    if (c)
    {
        health -= 1;
        if (health == 0)
        {
            active = false;
        }
    }
    return c;
}

void Building::draw(GameTextures &gt, bool debug)
{
    if (rotation_incr > 0)
    {
        rotation += rotation_incr;
        if (rotation > 360)
        {
            rotation = rotation - 360;
        }
    }
    sprite = health;
    GameObject::draw(gt, debug);
}

Explosion::Explosion(Point position)
{
    this->position = position;
    model = EXPLOSION;
    s_orientation = HORIZONTAL_SPRITE;
    n_sprites = 12;
    sprite = 0;
}

/*
 * Explosion drawing method
 * Does a single explosion animation
 * stops by setting ended to true.
 */
void Explosion::draw(GameTextures &gt)
{
    if (!ended)
    {
        GameObject::draw(gt, false);
        repetitions++;
        if (repetitions >= slowness)
        {
            sprite++;
            repetitions = 0;
        }
        if (sprite == 12)
        {
            ended = true;
        }
    }
}

Player::Player(int model, Point pos, Point scale)
{
    this->model = model;
    this->position = pos;
    this->scale = scale;
    //default speed
    this->speed.x = (GLfloat) (ORTHO_X) / O_TIME;
    this->speed.y = (GLfloat) (ORTHO_Y) / O_TIME;
}

void Player::display_health(GameTextures &gt) const
{
    glPushMatrix();

    glTranslatef(30, 10, -1);
    auto hs = gt.get_scaled(HEALTH, 2);
    glScalef(hs.x, hs.y, 1);
    gt.draw_sprite(HEALTH, VERTICAL_SPRITE, (GLfloat) health, 4);

    glPopMatrix();
}

/*
 * Rotates player in counter-clockwise
 * Uses the predefined rotation_incr
 * Limits to max_rotation
 */
void Player::rotate_l()
{
    rotation += rotation_incr;
    if (rotation > max_rotation)
    {
        rotation = max_rotation;
    }
}

/*
 * Rotates player in clockwise
 * Uses the predefined rotation_incr
 * Limits to max_rotation
 */
void Player::rotate_r()
{
    rotation -= rotation_incr;
    if (rotation < -max_rotation)
    {
        rotation = -max_rotation;
    }
}

void Player::walk_mru(double dt)
{
    if (move_dir.x == -1 && bb.get_min().x <= 0)
    {
        cout << "Player is touching left limit" << endl;
        moving = false;
        return;
    }
    if (move_dir.x == 1 && bb.get_max().x >= ORTHO_X)
    {
        cout << "Player is touching right limit" << endl;
        moving = false;
        return;
    }
    GameObject::walk_mru(dt, move_dir);
    moving = false;
}

void Player::walk_l()
{
    this->move_dir = Point(-1, 0);
    this->moving = true;
}

void Player::walk_r()
{
    this->move_dir = Point(1, 0);
    this->moving = true;
}

void Player::increase_str()
{
    if (str < MAX_STRENGTH)
    {
        str += 0.5f;
    }
}

void Player::decrease_str()
{
    if (str > MIN_STRENGTH)
    {
        str -= 0.5f;
    }
}

void Player::draw_aim()
{
    auto aim_direction = adjust_aim(aim, rotation);
    glPushAttrib(GL_ENABLE_BIT);
    double t = 0.0;
    double DeltaT = 1.0 / (300);
    //cout << "DeltaT: " << DeltaT << endl;
    BB_GL_COLOR;
    glLineStipple(1, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINE_STRIP);
    Point P = bb.projectile_origin;
    Point spd = speed * str;
    while (P.y > FLOOR_H && P.x >= 0 && P.x <= ORTHO_X)
    {
        P = calc_ob_throw(bb.projectile_origin, t, spd, aim_direction);
        glVertex2f(P.x, P.y);
        t += DeltaT;
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glPopAttrib();
}

bool Player::collided(GameObject &other)
{
    Point p;
    auto c = //bb.collision_detect(other.bb, p);
            bb.rotated_collision_detect(other.bb);
    if (c)
    {
        health -= 1;
        if (health <= 0)
        {
            active = false;
        }
    }
    return c;
}

Projectile Player::shoot(GameTextures &gt)
{
    auto p = Projectile(PLAYER_AMMO,
                        bb.projectile_origin,
                        gt.get_scaled(PLAYER_AMMO, 2),
                        adjust_aim(aim, rotation),
                        speed * str);
    return p;
}

void Player::draw(GameTextures &gt, bool debug)
{
    GameObject::draw(gt, debug);
    if (aiming)
    {
        draw_aim();
    }
}

Enemy::Enemy(int model, Point pos, Point scale, float speed, int direction)
{
    this->model = model;
    this->position = pos;
    this->scale = scale;
    if (direction == RIGHT)
    { this->scale.x = -this->scale.x; }
    this->bb.projectile_origin = Point(1, 0);
    this->aim = Point(direction, 0);
    this->dir = Point(direction, 0);
    //default speed
    this->speed.x = ((GLfloat) (ORTHO_X) / O_TIME) * speed;
    this->speed.y = (GLfloat) (ORTHO_Y) / O_TIME;
    shoot_time = MIN_ESHOOT + rand() % (MAX_ESHOOT - MIN_ESHOOT);

}

void Enemy::walk_mru(double dt)
{
    //only walks left
    if (bb.get_max().x <= 0 && dir.x == LEFT)
    {
        //cout << "outside limits object" << endl;
        position.x = ORTHO_X + (bb.get_max().x - bb.get_min().x);
        return;
    }
    else if (bb.get_min().x >= ORTHO_X && dir.x == RIGHT)
    {
        position.x = -(bb.get_max().x - bb.get_min().x);
    }
    GameObject::walk_mru(dt, dir);
}

bool Enemy::shoot(double dt, GameTextures &gt, Projectile &p)
{
    acum += (GLfloat) dt;
    if (acum >= shoot_time)
    {
        p = Projectile(ENEMY_AMMO,
                       bb.projectile_origin,
                       gt.get_scaled(ENEMY_AMMO, 1.5),
                       adjust_aim(aim, rotation),
                       speed * str);
        acum = 0;
        shoot_time = MIN_ESHOOT + rand() % (MAX_ESHOOT - MIN_ESHOOT);
        return true;
    }
    return false;
}

Projectile::Projectile(int type, Point pos, Point scale, Point direction, Point speed)
{
    this->position = pos;
    this->origin = pos;
    this->scale = scale;
    this->model = type;
    this->moving = true;
    this->direction = direction;
    this->speed = speed;
}

void Projectile::oblique_throw(double dt)
{
    if (position.y < FLOOR_H ||
        position.x < 0 || position.x > ORTHO_X)
    {
        moving = false;
        active = false;
    }
    animation += dt;
    // Position = Position0 + speed * time (* direction)
    this->position = calc_ob_throw(origin, animation, speed, direction);
}

Projectile::Projectile() = default;

/*
 * draw background using given ImageClass img
 */
void display_background(ImageClass &bg)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, ORTHO_X, 0, ORTHO_Y, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float zoomH = (glutGet(GLUT_WINDOW_WIDTH)) / (float) bg.SizeX();
    float zoomV = (glutGet(GLUT_WINDOW_HEIGHT)) / (float) bg.SizeY();
    bg.SetZoomH(zoomH);
    bg.SetZoomV(zoomV);
    bg.SetPos(0, 0);
    bg.Display();
}

/*
 * Draw a line on the floor
 * Should only be used on debug mode
 */
void draw_floor()
{
    glBegin(GL_LINES);
    BB_GL_COLOR;
    glVertex3f(0, FLOOR_H, -1);
    glVertex3f(ORTHO_X, FLOOR_H, -1);
    glEnd();
}

void draw_square(const Point &min, const Point &max)
{
    //min.print();
    //max.print();
    glBegin(GL_QUADS);
    // Front Face
    glTexCoord2f(min.x, min.y); //image coords
    glVertex3f(-1.0f, -1.0f, 0.0f); //gl coords
    glTexCoord2f(max.x, min.y);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glTexCoord2f(max.x, max.y);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(min.x, max.y);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();
}

vector<enemy_data> enemy_positions()
{
    GLfloat hb = 90;

    vector<enemy_data> positions;
    srand(time(nullptr));
    int dir = LEFT;
    int speed = 1;
    for (int i = 0; i < 3; i++)
    {
        GLfloat base_x = 20.0f;
        for (int j = 0; j < 4; j++)
        {
            auto x = base_x + (GLfloat) (19 + rand() % 45);
            base_x = x;
            auto p = store_enemy(Point(x, hb + (GLfloat) (i * 15)), speed, dir);
            positions.emplace_back(p);
        }
        dir = -dir;
        speed += ENEMY_SPEED_STEP;
    }
    return positions;
}

Point calc_ob_throw(Point &p, double t, Point &speed, Point &aim)
{
    Point Pr;
    Point S;
    S.x = t * speed.x * aim.x;
    S.y = (speed.y * aim.y) * (GLfloat) t - 0.5f * (GRAVITY * pow(t, 2));
    Pr = p + S;
    return Pr;
}

Point adjust_aim(Point &p, GLfloat rotation)
{
    Point rotated;
    auto rad = rotation * (M_PI / 180);
    rotated.x = p.x * cos(rad) - p.y * sin(rad);
    rotated.y = p.y * cos(rad) + p.x * sin(rad);

    return rotated;
}
