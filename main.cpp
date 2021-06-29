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
#include <headers/Bezier.h>
#include "headers/Point.h"
#include "headers/Temporizador.h"
#include "headers/ListaDeCoresRGB.h"
#include "headers/Engine3d.h"

Temporizador T;
double AccumDeltaT=0;
double nFrames=0;
double TempoTotal=0;

GLfloat AspectRatio;
GLfloat messages_panel = 0.2; //msg panel h %

GameTextures* gt;
GameObject gameFloor[30][30]; //fixed at map size
int sizeX = 30, sizeZ = 30;
int curr_view = View::Floating1;
bool debug = false;

Player* player;
vector<Building> buildings;
vector<Fuel> fuels;
vector<Plane> planes;



// codigos que definem o o tipo do elemento que est� em uma c�lula
#define VAZIO 0
#define PREDIO 10
#define RUA 20
#define COMBUSTIVEL 30

void switch_debug()
{
    debug = !debug;
    player->speed = debug ? ((GLfloat) S_SIZE)/DB_SPEED : ((GLfloat)S_SIZE) / P_SPEED;
}

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

    int count_fuel;
    input >> count_fuel;
    Object3d fuel_model;
    fuel_model.readObject(FUEL);
    for(int i=0; i<count_fuel; i++)
    {
        Point pos;
        input >> pos.x >> pos.z;
        fuels.emplace_back(pos, fuel_model);
    }

    for (int i=0;i<sizeX;i++)
        for (int j=0; j < sizeZ; j++)
            input >> gameFloor[j][i].tex;

}

void init_3d_structures()
{
    auto bd = Building(Point(27.0f,0.1f,4.0f), "models/gas_station.tri",
                      0.005, 90);
    buildings.emplace_back(bd);

    auto bd1 = Building(Point(3.0f, 2.0f, 24.0f), "models/p51mustg.tri",
                        0.1, 0);
    buildings.emplace_back(bd1);
    auto bd2 = Building(Point(5.0f, 0.0f, 7.0f), "models/house_1.tri",
                        0.5, 0);
    buildings.emplace_back(bd2);
    auto bd3 = Building(Point(5.0f, 0.0f, 15.0f), "models/build_1.tri",
                        0.5, 0);
    buildings.emplace_back(bd3);
    auto bd4 = Building(Point(29.0f, 0.0f, 19.0f), "models/build_3.tri",
                        0.001, 90);
    buildings.emplace_back(bd4);
    auto bd5 = Building(Point(18.0f, 0.0f, 4.0f), "models/build_2.tri",
                        0.01, -90);
    buildings.emplace_back(bd5);
}

void init_player()
{
    player = new Player(Point((GLfloat)sizeX/2,0,(GLfloat)sizeZ/2));
}

void init_curves()
{
    Bezier bz1, bz2;
    auto c1 = Curve3(Point(8,0,5),Point(8,0,7),Point(10,0,7));
    bz1.addCurve(c1);
    auto c2 = Curve3(Point(10,0,7),Point(12,0,7),Point(12,0,5));
    bz1.addCurve(c2);
    auto c3 = Curve3(Point(12,0,5),Point(12,0,3),Point(10,0,3));
    bz1.addCurve(c3);
    auto c4 = Curve3(Point(10,0,3),Point(8,0,3),Point(8,0,5));
    bz1.addCurve(c4);
    auto p1 = Plane(bz1, "models/ufo_1.tri", 0.006f);
    p1.rotation = 90;
    p1.rot = Point(1,0,0);
    planes.emplace_back(p1);


    bz2.addCurve(Curve3(Point(16,0,10),Point(16,0,12),Point(18,0,12)));
    bz2.addCurve(Curve3(Point(18,0,12),Point(20,0,12),Point(22,0,10)));
    bz2.addCurve(Curve3(Point(22,0,10),Point(24,0,8),Point(27,0,8)));
    bz2.addCurve(Curve3(Point(27,0,8),Point(30,0,8),Point(28.5,0,10.5)));
    bz2.addCurve(Curve3(Point(28.5,0,10.5),Point(27,0,13),Point(25,0,12)));
    bz2.addCurve(Curve3(Point(25,0,12),Point(20,0,8),Point(18,0,8)));
    bz2.addCurve(Curve3(Point(18,0,8),Point(16,0,8),Point(16,0,10)));
    auto p2 = Plane(bz2, "models/ufo_2.tri", 0.003f);
    p2.rotation = 0.0f;
    //p2.rot = Point(1,0,0);
    planes.emplace_back(p2);



}

void init(void)
{
    defineCorBg(Black);
    /*//transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/

    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
    
    srand((unsigned int)time(NULL));

    gt = new GameTextures();

    init_city();
    init_curves();
    init_3d_structures();
    init_player();


}

void clean()
{
    auto end = remove_if(
            fuels.begin(),
            fuels.end(),
            [](Fuel const &f) {
                return !f.active;
            });
    fuels.erase(end, fuels.end());
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

    if (AccumDeltaT > 1.0/60) // fixa a atualiza��o da tela em 30
    {
        AccumDeltaT = 0;
        for(auto &p : planes)
        {
            p.incr_time();
        }
        if(player->moving)
        {
            Point old = player->walk_mru(1.0/60);

            if(!debug)
            {
                if(handle_ambient_collision((*player), &gameFloor) || player->fuel <= 0)
                {
                    player->pos = old;
                    cout << "collided" << endl;
                }
                else
                {
                    player->fuel -= CONSUMPTION;
                    if(player->fuel <= 0)
                        player->fuel = 0;
                }
            }

            handle_fuel_collision((*player), fuels);
            clean();
        }
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

void DesenhaCidade()
{
    for(int i=0; i<sizeX; i++)
    {
        for(int j=0; j < sizeZ; j++)
        {
            glPushMatrix();
                auto gf = &gameFloor[i][j];
                glTranslatef((GLfloat)i, 0, (GLfloat)j);
                gt->draw_tex_floor(gf->tex);
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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    switch(curr_view)
    {
        case View::Floating1:
            gluLookAt((GLfloat)sizeX/2, 10, 0.0f,
                      (GLfloat)sizeX/2, 0, (GLfloat)sizeZ/2,
                      0,1,0);
            break;
        case View::Floating2:
            gluLookAt(0, 10, (GLfloat)sizeZ/2,
                      (GLfloat)sizeX/2, 0, (GLfloat)sizeZ/2,
                      0,1,0);
            break;
        case View::Floating3:
            gluLookAt((GLfloat)sizeX/2, 5, (GLfloat)sizeZ,
                      (GLfloat)sizeX/2, 0, (GLfloat)sizeZ/2,
                      0,1,0);
            break;
        case View::Floating4:
            gluLookAt((GLfloat)sizeX, 10, (GLfloat)sizeZ/2,
                      (GLfloat)sizeX/2, 0, (GLfloat)sizeZ/2,
                      0,1,0);
            break;
        case View::ThirdPerson:
            gluLookAt(player->cam.observer.x, player->cam.observer.y, player->cam.observer.z,
                      player->cam.target.x, player->cam.target.y, player->cam.target.z,
                      0, 1, 0);
        default:
            break;

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

void printString(const string& s, GLfloat posX, GLfloat posY)
{
    
    glRasterPos3f(posX, posY, 0); //define posicao na tela
    for (char i : s)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, i);
    }
    
}

void display_2d()
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

    defineCor(Gray);

    //fst half
    printString("KEYBINDINGS:", 0.1f, 8.5f);
    printString("Movements: W A S D", 0.1f, 7.0f);
    printString("Camera: arrow keys", 0.1f, 5.5f);
    printString("Camera point of view: 1 2 3 4", 0.1f, 4.0f);
    printString("Reset camera: x", 0.1f, 2.5f);
    printString("Exit: ESC", 0.1f, 1.0f);

    printString(player->pos.get(), 3.0f, 8.5f);

    //snd half
    printString("REMAINING RECHARGES: "+to_string(fuels.size()), 5.0f, 8.5f);

    char ret [30];
    auto f_lvl = player->fuel_level();
    snprintf(ret, 30, "FUEL: %.2f%%", f_lvl);
    auto s = string(ret);
    if(f_lvl > 70) {defineCor(Green);}
    else if (f_lvl > 35) {defineCor(Yellow);}
    else {defineCor(Red); }
    printString(s, 5.0f, 7.0f);



    // Resataura os par�metro que foram alterados
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, h * messages_panel, w, h - h * messages_panel);

    if (ativarLuz)
        glEnable(GL_LIGHTING);

}

void display( void )
{

    glDisable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DefineLuz();

    PosicUser();
    glLineWidth(2);

    glMatrixMode(GL_MODELVIEW);

    glColor3f(1, 1, 1);

    DesenhaCidade();

    for (auto &bd : buildings)
    {
        bd.draw();
    }

    for (auto &f : fuels)
    {
        if(f.active)
            f.draw();
    }

    player->draw();

    for (auto &p : planes)
    {
        p.route.drawBezier();
        p.draw();
    }

    drawCubeSk(player->pos, (*gt));

    display_2d();

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
            curr_view = View::Floating1;
            glutPostRedisplay();
            break;
        case 51:
            curr_view = View::Floating2;
            glutPostRedisplay();
            break;
        case 52:
            curr_view = View::Floating3;
            glutPostRedisplay();
            break;
        case 53:
            curr_view = View::Floating4;
            glutPostRedisplay();
            break;
	    case 'w':
	        player->walk_forward();
	        break;
	    case 's':
	        player->walk_backward();
	        break;
	    case 'a':
	        player->rotate_l();
	        break;
	    case 'd':
	        player->rotate_r();
	        break;
	    case 'q':
            player->rotate_camera_l();
	        break;
	    case 'e':
	        player->rotate_camera_r();
	        break;
        case 'x':
            player->reset_camera();
            break;
	    case 'c':
            glutFullScreen();
            break;
	    case 'z':
	        switch_debug();
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
            player->rotate_camera_u();
			break;
	    case GLUT_KEY_DOWN:     // When Down Arrow Is Pressed...
            player->rotate_camera_d();
			break;
        case GLUT_KEY_LEFT:
            player->rotate_camera_l();
            break;
        case GLUT_KEY_RIGHT:
            player->rotate_camera_r();
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
    //glutFullScreen();
		
	init ();

	glutDisplayFunc ( display );  
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( keyboard );
	glutSpecialFunc ( arrow_keys );
	glutIdleFunc ( animate );


	glutMainLoop ( );          
	return 0; 
}



