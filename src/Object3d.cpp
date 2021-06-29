//
// Created by marcel on 27/06/2021.
//

#include "headers/Object3d.h"

Color::Color(int rgb)
{
    b = rgb % 256;
    g = (int)((rgb-b)/256) % 256;
    r = ((rgb-b)/ pow(256,2))-(g/256);

    r /= 255;
    g /= 255;
    b /= 255;

}

Object3d::Object3d()
{
    nFaces = 0;
    faces = NULL;
}
unsigned int Object3d::count()
{
    return nFaces;
}
void Object3d::readObject(const string& name)
{
    ifstream f;
    f.open(name, ios::in);
    if(!f)
    {
        cout << "Error trying to open f " << name << endl;
        exit(1);
    }

    f >> nFaces;
    faces = new Triangle[nFaces];

    for(int i=0; i<nFaces; i++)
    {
        f >> faces[i].p1.x >>  faces[i].p1.y >> faces[i].p1.z;
        f >> faces[i].p2.x >>  faces[i].p2.y >> faces[i].p2.z;
        f >> faces[i].p3.x >>  faces[i].p3.y >> faces[i].p3.z;
        int rgb;
        f >> hex >> rgb;
        faces[i].color = Color(rgb);
    }
    cout << "object loaded " << name << endl;
}

void Object3d::drawObject()
{
    for(int i=0; i<nFaces; i++)
    {
        Point n;
        Point va = faces[i].p2 - faces[i].p1;
        Point vb = faces[i].p3 - faces[i].p2;
        prod_vetorial(va, vb, n);
        vet_unit(n);

        glColor3f(faces[i].color.r, faces[i].color.g, faces[i].color.b);
        glBegin(GL_TRIANGLES);
        glNormal3f(n.x, n.y, n.z);
        glVertex3f(faces[i].p1.x, faces[i].p1.y, faces[i].p1.z);
        glVertex3f(faces[i].p2.x, faces[i].p2.y, faces[i].p2.z);
        glVertex3f(faces[i].p3.x, faces[i].p3.y, faces[i].p3.z);
        glEnd();
    }

}