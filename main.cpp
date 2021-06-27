// **********************************************************************
// PUCRS/Escola Polit�cnica
// COMPUTA��O GR�FICA
//
// Simulador de Cidade
//
// Marcio Sarroglia Pinho
// pinho@pucrs.br
// **********************************************************************

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;


#include <GL/glew.h>
#ifdef WIN32
#include <windows.h>
#include <glut.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/freeglut.h>
#endif

#include <iostream>
#include "headers/Point.h"
#include "BoundingBox.h"
#include "headers/Temporizador.h"
#include "headers/ListaDeCoresRGB.h"
#include "headers/Engine3d.h"
#include <headers/shader.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

Temporizador T;
double AccumDeltaT=0;
double nFrames=0;
double TempoTotal=0;

GLfloat AspectRatio;
GLfloat messages_panel = 0.2; //msg panel h %

GameTextures* gt;
GameObject gameFloor[30][30]; //fixed at map size
Shader* skyboxShader;
int sizeX = 30, sizeZ = 30;
int curr_view = View::ThirdPerson;
float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
};


/*
// Controle do modo de projecao
// 0: Projecao Paralela Ortografica; 1: Projecao Perspectiva
// A funcao "PosicUser" utiliza esta variavel. O valor dela eh alterado
// pela tecla 'p'*/


Point Curva1[3];

// Representa o conteudo de uma celula do piso
class Elemento{
public:
    int tipo;
    int cor;
    float altura;
    int texID;
};

// codigos que definem o o tipo do elemento que est� em uma c�lula
#define VAZIO 0
#define PREDIO 10
#define RUA 20
#define COMBUSTIVEL 30

void init_city()
{
    ifstream input;
    input.open("data/map.txt", ios::in);
    if(!input)
    {
        cout << "Error opening map file ." << endl;
        exit(1);
    }
    cout << "Loading map configs ... " << endl;

    input >> sizeX >> sizeZ;

    Point playerpos; //todo put inside object
    input >> playerpos.x >> playerpos.y >> playerpos.z;

    int count_fuel;
    input >> count_fuel;

    for(int i=0; i<count_fuel; i++)
    {
        input >> playerpos.x >> playerpos.z; //todo create fuels
    }

    for (int i=0;i<sizeX;i++)
        for (int j=0; j < sizeZ; j++)
            input >> gameFloor[j][i].model;

}
unsigned int skyboxVAO, skyboxVBO;

void init_skybox()
{

    glewExperimental = GL_TRUE;
    glewInit();

    skyboxShader = new Shader("skybox.vs", "skybox.fs");
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

void init(void)
{
    defineCorBg(MediumAquamarine);
    /*//transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_NORMALIZE);

    Curva1[0] = Point (-6,0,1);
    Curva1[1] = Point (0,10,1);
    Curva1[2] = Point (6,0,1);
    
    srand((unsigned int)time(NULL));

    init_skybox();

    gt = new GameTextures();

    skyboxShader->use();
    skyboxShader->setInt("skybox", 0);


    init_city();


}

// **********************************************************************
//
// **********************************************************************
void animate()
{
    double dt;
    dt = T.get_delta_t();
    AccumDeltaT += dt;
    TempoTotal += dt;
    nFrames++;

    if (AccumDeltaT > 1.0/30) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        glutPostRedisplay();
    }
    if (TempoTotal > 5.0)
    {
        cout << "Tempo Acumulado: "  << TempoTotal << " segundos. " ;
        cout << "Nros de Frames sem desenho: " << nFrames << endl;
        cout << "FPS(sem desenho): " << nFrames/TempoTotal << endl;
        TempoTotal = 0;
        nFrames = 0;
    }
}

// **********************************************************************
Point CalculaBezier3(Point PC[], double t)
{
    Point P;
    double UmMenosT = 1-t;
    
    P =  PC[0] * UmMenosT * UmMenosT + PC[1] * 2 * UmMenosT * t + PC[2] * t*t;
    //P.z = 5;
    return P;
}
// **********************************************************************
void TracaBezier3Pontos()
{
    double t=0.0;
    double DeltaT = 1.0/10;
    Point P;
    //cout << "DeltaT: " << DeltaT << endl;
    glBegin(GL_LINE_STRIP);
    
    while(t<1.0)
    {
        P = CalculaBezier3(Curva1, t);
        glVertex3f(P.x, P.y, P.z);
        t += DeltaT;
       // P.imprime(); cout << endl;
    }
    P = CalculaBezier3(Curva1, 1.0); // faz o fechamento da curva
    glVertex3f(P.x, P.y, P.z);
    glEnd();
}
// **********************************************************************
//  Desenha um pr�dio no meio de uam c�lula
// **********************************************************************
void DesenhaPredio(float altura)
{
    glPushMatrix();
        glScalef(0.2, altura, 0.2);
        glTranslatef(0, 1, 0);
        draw_cube();
    glPopMatrix();
    
}

// **********************************************************************
// DesenhaCidade(int nLinhas, int nColunas)
// QtdX: nro de c�lulas em X
// QtdZ: nro de c�lulas em Z
// Desenha elementos que compiem a cidade
// **********************************************************************
void DesenhaCidade()
{
    for(int i=0; i<sizeX; i++)
    {
        for(int j=0; j < sizeZ; j++)
        {
            glPushMatrix();
                auto gf = &gameFloor[i][j];
                glTranslatef(i, 0, j);
                gt->draw_tex_floor(gf->model);
            glPopMatrix();
        }
    }
}
void PosicUser()
{

    // Define os par�metros da proje��o Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90,AspectRatio,0.01,1500); // Projecao perspectiva
    //mat4 pMatrix = perspective(90/180.0f, AspectRatio, 0.01f, 1500.0f);
    //glLoadMatrixf(&pMatrix[0][0]);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    if(curr_view == View::ThirdPerson)
    {
        gluLookAt(sizeX/2, 10, 0,
               sizeX/2, 0, sizeZ/2,
               0,1,0);
    }
    else
    {
        /*auto cam = lookAt(vec3(sizeX/2, 15, 0),
               vec3(sizeX/2, 0, sizeZ/2),
               vec3(0,1,0));*/

        gluLookAt(sizeX/2, 10, 0,
                  sizeX/2, 0, sizeZ/2,
                  0,1,0);
    }

}
void reshape( int w, int h )
{

	// Evita divis�o por zero, no caso de uam janela com largura 0.
	if(h == 0) h = 1;
    // Ajusta a rela��o entre largura e altura para evitar distor��o na imagem.
    // Veja fun��o "PosicUser".
	AspectRatio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Seta a viewport para ocupar toda a janela
    //glViewport(0, 0, w, h);
    glViewport(0, h * messages_panel, w, h - h * messages_panel);

    //cout << "Largura" << w << endl;

	PosicUser();

}

void printString(string s, int posX, int posY, int cor)
{
    defineCor(cor);
    
    glRasterPos3i(posX, posY, 0); //define posicao na tela
    for (int i = 0; i < s.length(); i++)
    {
//GLUT_BITMAP_HELVETICA_10,
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
    
}

void DesenhaEm2D()
{
    int ativarLuz = false;
    if (glIsEnabled(GL_LIGHTING))
    {
        glDisable(GL_LIGHTING);
        ativarLuz = true;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Salva o tamanho da janela
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    
    // Define a area a ser ocupada pela area OpenGL dentro da Janela
    glViewport(0, 0, w, h * messages_panel); // a janela de mensagens fica na parte de baixo da janela

    // Define os limites logicos da area OpenGL dentro da Janela
    glOrtho(0,10, 0,10, 0,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Desenha linha que Divide as �reas 2D e 3D
    defineCor(Yellow);
    glLineWidth(5);
    glBegin(GL_LINES);
        glVertex2f(0,10);
        glVertex2f(10,10);
    glEnd();
    
    printString("Amarelo", 0, 0, Yellow);
    printString("Vermelho", 4, 2, Red);
    printString("Verde", 8, 4, Green);

    // Resataura os par�metro que foram alterados
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, h * messages_panel, w, h - h * messages_panel);

    if (ativarLuz)
        glEnable(GL_LIGHTING);

}

void drawSkyBox()
{
    // draw skybox as last

    glEnable(GL_TEXTURE_CUBE_MAP);
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    // skybox cube
    skyboxShader->use();
    mat4 view = mat4(glm::mat3(lookAt(vec3(0, 0.5, 2), vec3(0,0,0), vec3(0,1,0)))); // remove translation from the view matrix
    skyboxShader->setMat4("view", view);
    mat4 projection = perspective(90/180.0f, AspectRatio, 0.01f, 1500.0f);
    skyboxShader->setMat4("projection", projection);
    glPushMatrix();
    glTranslatef(sizeX/2, 0, sizeZ/2);
    glScalef(20,20,20);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, gt->cubeMap);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glPopMatrix();
    glDepthFunc(GL_LESS); // set depth function back to default
    glDisable(GL_TEXTURE_CUBE_MAP);
}

void display( void )
{


    glDisable(GL_TEXTURE_2D);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	DefineLuz();

	PosicUser();
    glLineWidth(2);
	
	glMatrixMode(GL_MODELVIEW);

    glColor3f(1,1,1);
    TracaBezier3Pontos();

    DesenhaCidade();
    drawSkyBox();

    DesenhaEm2D();

	glutSwapBuffers();
}


void keyboard ( unsigned char key, int x, int y )
{
	switch ( key ) 
	{
        case 27:        // Termina o programa qdo
            exit ( 0 );   // a tecla ESC for pressionada
            break;
	    case 49:
	        curr_view = View::ThirdPerson;
	        glutPostRedisplay();
	        break;
        case 50:
            curr_view = View::Floating;
            glutPostRedisplay();
            break;
        case 'p':
            break;
        default:
            cout << key;
    break;
  }
}

void arrow_keys ( int a_keys, int x, int y )  
{
	switch ( a_keys ) 
	{
		case GLUT_KEY_UP:       // When Up Arrow Is Pressed...
			glutFullScreen ( ); // Go Into Full Screen Mode
			break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
			glutInitWindowSize  ( 700, 500 ); 
			break;
		default:
			break;
	}
}

int main ( int argc, char** argv )   
{
	glutInit            ( &argc, argv ); 
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
	glutInitWindowPosition (0,0);
	glutInitWindowSize  ( 700, 700 );
	glutCreateWindow    ( "Computacao Grafica - Exemplo Basico 3D" ); 
		
	init ();
    //system("pwd");
	
	glutDisplayFunc ( display );  
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );

	glutMainLoop ( );          
	return 0; 
}



