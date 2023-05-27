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
GLuint width = 800, height = 800;

// Fractals
float eps;
int m;

float uMin, uMax, vMin, vMax;

float cReal, cImag;

bool isMandelbrot = false;
//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay();
void myReshape(int width, int height);
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);



int main(int argc, char ** argv) {

std::cout << "Upisite 1 za mandelbrot, 2 za julia: ";
    int input = -1;
    std::cin >> input;
    if (input == 1){
        isMandelbrot = true;
    }else if (input == 2){
        isMandelbrot = false;
    }

    std::cout << "Upisite espilon i maksimalni broj iteracija: ";
     std::cin >> eps >> m;


    std::cout << "Upisite uMin, uMax, vMin, vMax: ";
 std::cin >> uMin >> uMax >> vMin >> vMax;

    if ( ! isMandelbrot){
        std::cout << "Upisite cReal i cImag: ";
        std::cin >> cReal >> cImag;
    }


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



void myDisplay()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // mandelbrot
    if (isMandelbrot){
    for (int i =0; i<  height; i++) {
        for (int j = 0; j < width; j++) {
            float u0 = uMin + (uMax - uMin) * i / (height - 1);
            float v0 = vMin + (vMax - vMin) * j / (width - 1);
            int k = -1;
            float cReal = u0;
            float cImag = v0;
            float zReal = 0;
            float zImag = 0;
            float r = 0;
            while (r < eps && k < m) {
                k++;
                float zR = zReal * zReal - zImag * zImag + cReal;
                float zI = 2 * zReal * zImag + cImag;
                zReal = zR;
                zImag = zI;
                r = glm::sqrt(zReal * zReal + zImag * zImag);

            }
            glBegin(GL_POINTS);
            float color = 1 - k / (float) m;
            glColor4f(color, color, color, 1.0f);
            float x = 2 * (i / (float) width - 0.5f);
            float y = 2 * (j / (float) height - 0.5f);
            glVertex2f(x, y);
            glEnd();
        }
    } }else{
    for (int i =0; i<  height; i++) {
        for (int j = 0; j < width; j++) {
            float u0 = uMin + (uMax - uMin) * i / (height - 1);
            float v0 = vMin + (vMax - vMin) * j / (width - 1);
            int k = -1;

            float zReal = u0;
            float zImag = v0;
            float r = 0;
            while (r < eps && k < m) {
                k++;
                float zR = zReal * zReal - zImag * zImag + cReal;
                float zI = 2 * zReal * zImag + cImag;
                zReal = zR;
                zImag = zI;
                r = glm::sqrt(zReal * zReal + zImag * zImag);

            }
            glBegin(GL_POINTS);
            float color = 1 - k / (float) m;
            glColor4f(color, color, color, 1.0f);
            float x = 2 * (i / (float) width - 0.5f);
            float y = 2 * (j / (float) height - 0.5f);
            glVertex2f(x, y);
            glEnd();
        }
    }
    }


//    glLoadIdentity();
//    gluPerspective(45.0f, 1.f, 0.1f, 100.f);  // Perspective projection
//
//    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, lookAtPoint.x, lookAtPoint.y, lookAtPoint.z, 0.0f, 1.0f, 0.0f);
//


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
    glClearColor(0.5f, 1.0f, 1.0f, 0.0f); // boja pozadine
    glClear(GL_COLOR_BUFFER_BIT);        //	brisanje pozadine
    myDisplay();
}


//*********************************************************************************
//	Mis.
//*********************************************************************************



void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {

}
