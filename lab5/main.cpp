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
std::vector<glm::vec3> scaledVertices;
std::vector<glm::vec3> transformed_vertices;
std::vector<glm::vec3> faces;
std::string filename = "resources/teddy.obj";
glm::vec3 point = {0.5f, 0.5f, 0.f};

glm::vec3 O = {0.f, 0.f, 20.f};
glm::vec3 G;

glm::mat4 projectionMatrix = glm::identity<glm::mat4>();
glm::mat4 viewMatrix = glm::identity<glm::mat4>();
glm::vec3 view_up = glm::vec3(0.f, 1.f, 0.f);

//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay();
void myReshape(int width, int height);
void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);


int main(int argc, char ** argv) {

    std::vector<glm::vec3> vertices;

    Utils::readVerticesAndFaces(filename, vertices, faces);

    scaledVertices = Utils::scaleToOne(vertices);
    //scaledVertices = vertices;
    transformed_vertices = scaledVertices;
    Utils::minMax minMax = Utils::getMinMax(scaledVertices);
     O = glm::vec3(minMax.XMax + 1.f,minMax.YMax + 1.f, minMax.ZMax+ 1.f);
    G = Utils::getCenter(scaledVertices);
//    G = point;
    std::cout << "O: " << O.x << " " << O.y << " " << O.z << std::endl;
    std::cout << "G: " << G.x << " " << G.y << " " << G.z << std::endl;



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

    viewMatrix = Utils::getViewMatrix(O, G, view_up);
    transformed_vertices = Utils::transformVertices(scaledVertices, viewMatrix);
    projectionMatrix = Utils::getProjectionMatrix(O, G);
    std::vector<glm::vec3> projected_vertices = Utils::projectVertices(transformed_vertices, projectionMatrix);
    for(int i=0; i< faces.size(); i++){
        glBegin(GL_POLYGON);

        glColor4f( (projected_vertices[faces[i].x-1].z + 1)*0.5f, 0.0f, 0.f, 0.0f);
        glVertex2f(projected_vertices[faces[i].x-1].x , projected_vertices[faces[i].x -1 ].y);

        glColor4f((projected_vertices[faces[i].y-1].z + 1)*0.5f,0.f,  0.f, 0.0f);
        glVertex2f(projected_vertices[faces[i].y-1].x, projected_vertices[faces[i].y -1].y);

        glColor4f((projected_vertices[faces[i].z-1].z + 1)*0.5f, 0.f, 0.f, 0.0f);
        glVertex2f(projected_vertices[faces[i].z -1 ].x , projected_vertices[faces[i].z -1 ].y );

        glEnd();
    }


}


void myDisplay()
{
    myPolygon();
    glFlush();
}

void myMouse(int button, int state, int x, int y){

}

//*********************************************************************************
//	Promjena velicine prozora.
//*********************************************************************************

void myReshape(int w, int h) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -100, 100);

    glClearColor(0.5f, 1.0f, 1.0f, 0.0f); // boja pozadine
    glClear(GL_COLOR_BUFFER_BIT);        //	brisanje pozadine
    myDisplay();
}

//*********************************************************************************
//	Mis.
//*********************************************************************************



void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    float translation_speed = 0.1f;
    switch (theKey)
    {
        case 'w':
            O = O + glm::vec3(0, translation_speed, 0);
            break;

        case 's':
            O = O + glm::vec3(0, -translation_speed, 0);
            break;

        case 'q':
            O = O + glm::vec3(translation_speed, 0, 0);
            break;

        case 'a':
            O = O + glm::vec3(-translation_speed, 0, 0);
            break;
        case 'e':
            O = O + glm::vec3(0, 0, translation_speed);
            break;

        case 'd':
            O = O + glm::vec3(0, 0, -translation_speed);
            break;

        case 't':
            G = G + glm::vec3(0, translation_speed, 0);
            break;

        case 'g':
            G = G + glm::vec3(0, -translation_speed, 0);
            break;

        case 'r':
            G = G + glm::vec3(translation_speed, 0, 0);
            break;

        case 'f':
            G = G + glm::vec3(-translation_speed, 0, 0);
            break;
        case 'y':
            G =G + glm::vec3(0, 0, translation_speed);
            break;

        case 'h':
            G = G + glm::vec3(0, 0, -translation_speed);
            break;

    }


    std::cout << "O: " << O.x << " " << O.y << " " << O.z << std::endl;
    std::cout << "G: " << G.x << " " << G.y << " " << G.z << std::endl;
    std::cout << "\n\n";

    myReshape(width, height);

    glFlush();
}
