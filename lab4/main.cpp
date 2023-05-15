#include <iostream>
#include <string>
#include <vector>
#include "utils.h"
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>


GLint Ix;


//*********************************************************************************
//	Pokazivac na glavni prozor i pocetna velicina.
//*********************************************************************************

GLuint window;
GLuint width = 300, height = 300;
glm::mat4 projectionMatrix = glm::identity<glm::mat4>();
std::vector<glm::vec3> scaledVertices;
std::vector<glm::vec3> faces;
std::vector<Utils::planeCoefficients> planesCoefficients;


glm::vec3 testPoint4 = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 testPoint1 = glm::vec3(0.1, 0.1, 0.0);

glm::vec3 testPoint2 = glm::vec3(0.5, 0.5, 0.0);
glm::vec3 testPoint3 = glm::vec3(0.5, 0.5, -0.5);

std::string filename = "resources/temple.obj";
bool convex = true;

std::vector<glm::vec3> points = {testPoint1, testPoint2, testPoint3, testPoint4, {0.0, 0.7, 0.8}, {0.7, 0.7, 0.8}, {-0.45, -0.05, -0.25}};
//std::vector<glm::vec3> points = {};
//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay();
void myReshape(int width, int height);
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);

void myPoint(float x, float y, float z, bool isGreen);


int main(int argc, char ** argv) {

    std::vector<glm::vec3> vertices;

    Utils::readVerticesAndFaces(filename, vertices, faces);

    scaledVertices = Utils::scaleToOne(vertices);

    planesCoefficients = Utils::getAllPlanesCoefficients(faces, scaledVertices);


    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutInit(&argc, argv);

    window = glutCreateWindow("Glut OpenGL Polygon");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(myDisplay);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);


    glutMainLoop();
    return 0;
}
void myPolygon()
{

    glm::vec3 center = Utils::getCenter(scaledVertices);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for(int i=0; i< faces.size(); i++){
        glBegin(GL_POLYGON);

        glColor4f( (scaledVertices[faces[i].x-1].z + 1)*0.5f, 0.0f, 0.f, 0.0f);
        glVertex3f(scaledVertices[faces[i].x-1].x , scaledVertices[faces[i].x -1 ].y , scaledVertices[faces[i].x - 1].z);
        //glVertex3f(scaledVertices[faces[i].x-1].x , scaledVertices[faces[i].x -1 ].y , 0.f);


        glColor4f((scaledVertices[faces[i].y-1].z + 1)*0.5f,0.f,  0.f, 0.0f);
        glVertex3f(scaledVertices[faces[i].y-1].x, scaledVertices[faces[i].y -1].y , scaledVertices[faces[i].y -1].z);
        //glVertex3f(scaledVertices[faces[i].y-1].x, scaledVertices[faces[i].y -1].y ,0.f);

        glColor4f((scaledVertices[faces[i].z-1].z + 1)*0.5f, 0.f, 0.f, 0.0f);
        glVertex3f(scaledVertices[faces[i].z -1 ].x , scaledVertices[faces[i].z -1 ].y , scaledVertices[faces[i].z -1].z);
        //glVertex3f(scaledVertices[faces[i].z -1 ].x , scaledVertices[faces[i].z -1 ].y ,0.0f);

        glEnd();
    }


}


void myDisplay()
{
    myPolygon();

    for (auto point : points) {
        if(Utils::isPointInside(point, planesCoefficients, convex)) {
            myPoint(point.x, point.y, point.z, true);
        }else {
            myPoint(point.x, point.y, point.z, false);
        }
    }


    glFlush();
}

void myMouse(int button, int state, int x, int y){

}

//*********************************************************************************
//	Promjena velicine prozora.
//*********************************************************************************

void myReshape(int w, int h) {
    width = w;
    height = h;

    //promjena sirine i visine prozora
    Ix = 0;
    float aspectRatio = (float)w / (float)h;
//	indeks tocke 0-prva 1-druga tocka
    glViewport(0, 0, w, h);    //  otvor u prozoru
    glMatrixMode(GL_PROJECTION);        //	matrica projekcije
    glLoadMatrixf(&projectionMatrix[0][0]);

    //glLoadIdentity();  //	jedinicna matrica
    glOrtho(-1, 1, -1, 1, -1 , 1);    //	okomita projekcija
    glMatrixMode(GL_MODELVIEW);            //	matrica pogleda


   // glLoadMatrixf(&projectionMatrix[0][0]);
    glLoadIdentity();
    glClearColor(0.5f, 1.0f, 1.0f, 0.0f); // boja pozadine
    glClear(GL_COLOR_BUFFER_BIT);        //	brisanje pozadine
    myDisplay();
}

void myPoint(float x, float y, float z, bool isGreen = false) {

    // Set the modelview matrix to move the sphere to the custom location
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, z);

    // Draw the sphere

    if (isGreen) {
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    }
    else{
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    }

    glutSolidSphere(0.01, 50, 50);

}

//*********************************************************************************
//	Mis.
//*********************************************************************************



void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    float rotation_speed = 5.f;
    switch (theKey)
    {
        case 'w':
            // rotation up (x axis)
            projectionMatrix = glm::rotate(projectionMatrix, glm::radians(rotation_speed), glm::vec3(1.0f, 0.0f, 0.0f));
            break;

        case 's':

            // rotation down (x axis)
            projectionMatrix = glm::rotate(projectionMatrix, glm::radians(-rotation_speed), glm::vec3(1.0f, 0.0f, 0.0f));
            break;

        case 'a':
            // rotation left (y axis)
            projectionMatrix = glm::rotate(projectionMatrix, glm::radians(rotation_speed), glm::vec3(0.0f, 1.0f, 0.0f));
            break;

        case 'd':
            // rotation right (y axis)
            projectionMatrix = glm::rotate(projectionMatrix, glm::radians(-rotation_speed), glm::vec3(0.0f, 1.0f, 0.0f));
            break;

    }
    myReshape(width, height);
    glFlush();
}
