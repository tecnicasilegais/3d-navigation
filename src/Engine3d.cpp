//
// Created by m on 26/06/2021.
//

#include <headers/Point.h>
#include "headers/Engine3d.h"

/*
 * Game Texture class and methods
 */
GameTextures::GameTextures()
{
    vector<string> faces
            {
                    "img/right.jpg",
                    "img/left.jpg",
                    "img/top.jpg",
                    "img/bottom.jpg",
                    "img/front.jpg",
                    "img/back.jpg",
            };
    cubeMap = loadCubemap(faces);
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

// **********************************************************************
// void draw_floor(int corBorda, int corDentro)
// Desenha uma c�lula do piso.
// Eh possivel definir a cor da borda e do interior do piso
// O ladrilho tem largula 1, centro no (0,0,0) e est� sobre o plano XZ
// **********************************************************************
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
// **********************************************************************
//  void draw_cube()
//
//
// **********************************************************************
void draw_cube()
{
    glBegin ( GL_QUADS );
    // Front Face
    glNormal3f(0,0,1);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.5f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.5f, 0.5); glVertex3f(-1.0f,  1.0f,  1.0f);

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

// **********************************************************************
//  void DefineLuz(void)
// **********************************************************************
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
unsigned int loadCubemap(vector<std::string> faces)
{
    glEnable(GL_TEXTURE_CUBE_MAP);
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        ImageClass img;
        int data = img.Load(faces[i].c_str());
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, img.SizeX(), img.SizeY(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.GetImagePtr());
            img.Delete();
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            img.Delete();
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glDisable(GL_TEXTURE_CUBE_MAP);
    return textureID;
}