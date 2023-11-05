#include <stdio.h>
#include <GL/freeglut.h>
#include <glm/vec2.hpp>
#include <algorithm>

#include "utils.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

GLint Ix;
std::vector<glm::vec2> polygonPoints;
glm::vec2 minPoint;
glm::vec2 maxPoint;
bool polygonDrawn = false;

std::vector<Utils::Edge> leftEdges;
std::vector<Utils::Edge> rightEdges;

//*********************************************************************************
//	Pokazivac na glavni prozor i pocetna velicina.
//*********************************************************************************

GLuint window;
GLuint width = 300, height = 300;

//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay();
void myReshape(int width, int height);
void myMouse(int button, int state, int x, int y);


//*********************************************************************************
//	Glavni program.
//*********************************************************************************

int main(int argc, char ** argv)
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutInit(&argc, argv);

    window = glutCreateWindow("Glut OpenGL Polygon");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(myDisplay);
    glutMouseFunc(myMouse);
    printf("Lijevom tipkom misa zadaj tocke polyigona\n");
    printf("Desnom tipkom misa crtaj poligon\n");

    glutMainLoop();
    return 0;
}


//*********************************************************************************
//	Osvjezavanje prikaza. (nakon preklapanja prozora)
//*********************************************************************************

void myDisplay()
{
    glFlush();
}

//*********************************************************************************
//	Promjena velicine prozora.
//*********************************************************************************

void myReshape(int w, int h)
{
    width = w; height = h;               //promjena sirine i visine prozora
    Ix = 0;								//	indeks tocke 0-prva 1-druga tocka
    glViewport(0, 0, width, height);	//  otvor u prozoru

    glMatrixMode(GL_PROJECTION);		//	matrica projekcije
    glLoadIdentity();					//	jedinicna matrica
    gluOrtho2D(0, width, 0, height); 	//	okomita projekcija
    glMatrixMode(GL_MODELVIEW);			//	matrica pogleda
    glLoadIdentity();					//	jedinicna matrica

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // boja pozadine
    glClear(GL_COLOR_BUFFER_BIT);		//	brisanje pozadine
    glPointSize(1.0);					//	postavi velicinu tocke za liniju
    glColor3f(0.0f, 0.0f, 0.0f);		//	postavi boju linije
}

//*********************************************************************************
//	Crtaj moju liniju.
//*********************************************************************************

void myPolygon()
{
        for(int i=minPoint.y; i<maxPoint.y; i++){
            std::vector<glm::vec2> leftIntersections;
            std::vector<glm::vec2> rightIntersections;
            for(auto edge : leftEdges){
                leftIntersections.push_back(Utils::calculateIntersection(edge, i));
            }
            for(auto edge : rightEdges){
                rightIntersections.push_back(Utils::calculateIntersection(edge, i));
            }
            auto LMin = std::min_element(leftIntersections.begin(), leftIntersections.end(), [](glm::vec2 a, glm::vec2 b){
                return a.x < b.x;
            });

            auto BMax = std::max_element(rightIntersections.begin(), rightIntersections.end(), [](glm::vec2 a, glm::vec2 b){
                return a.x < b.x;
            });
            glBegin(GL_LINES);
            {
            glVertex2i(LMin->x, i);
            glVertex2i(BMax->x,i );

            }
            glEnd();
        }

}

void myPoint(GLint x, GLint y, float size, bool isRed = false) {
    glPointSize(size);  // Set the point size
    glBegin(GL_POINTS);
    if (isRed) {
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else{
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    }
    glVertex2f(x, y);
    glEnd();

}

//*********************************************************************************
//	Mis.
//*********************************************************************************

void myMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(!polygonDrawn) {
            polygonPoints.push_back(glm::vec2(x, height - y));
        }
        else{
            std::vector<Utils::LineCoefficients> coefficients = Utils::calculateLineCoefficients(polygonPoints);
            if (Utils::checkPointInPolygon(glm::vec2(x, height - y), coefficients)) {
                myPoint(x, height - y, 5.0f, true);
            }
            else{
                myPoint(x, height - y, 5.0f, false);
            }
            glFlush();
        };
    }

    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {

        Utils::sortClockWise(polygonPoints);


        auto [min_it, max_it] = minmax_element(polygonPoints.begin(), polygonPoints.end(), Utils::compareY);
        minPoint = *min_it;
        maxPoint = *max_it;

        Utils::sortLeftRightEdges(polygonPoints, leftEdges, rightEdges);
        myPolygon();
        glFlush();
        polygonDrawn = true;

    }
}
