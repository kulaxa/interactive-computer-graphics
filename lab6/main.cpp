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
std::vector<glm::vec3> control_points;
std::vector<glm::vec3> faces;
std::string filename = "resources/teddy.obj";
std::string control_points_filename = "resources/control_points2.txt";

std::vector<glm::vec3> bazier_curve;
int current_index = 0;
int n = 100;

// Define camera position, direction, and up vectors
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lookAtPoint;

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
    control_points = Utils::readControlPoints(control_points_filename);
    bazier_curve = Utils::calculateBazierCurve(control_points, n);
    glm::vec3 lookAtPoint = Utils::getCenter(scaledVertices);


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
    glBegin(GL_LINES);
    // Draw polygon from control points
    for (int i = 0; i < control_points.size()-1; i++) {


        std::cout << control_points[i].x << " " << control_points[i].y << " " << control_points[i].z << std::endl;
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glVertex3f(control_points[i].x, control_points[i].y, control_points[i].z);
        glVertex3f(control_points[i+1].x, control_points[i+1].y, control_points[i+1].z);


    }
    glEnd();


    glBegin(GL_POINTS);
    glPointSize(5.0f);
    // Draw bazier curve

    for (int i = 0; i < bazier_curve.size(); i++) {
        glm::vec3 point = bazier_curve[i];
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glVertex3f(point.x, point.y, point.z);
    }
    glEnd();
    std::vector<glm::vec3> faces_removed_back = Utils::removeBackFaces(scaledVertices, faces, cameraPos);
   // faces_removed_back = faces;
    std::cout << "Faces before removal: " << faces.size() << std::endl;
    std::cout << "Faces removed: " << faces.size() - faces_removed_back.size() << std::endl;
    for(int i=0; i< faces_removed_back.size(); i++){
        glBegin(GL_POLYGON);

        glColor4f( (scaledVertices[faces_removed_back[i].x-1].z + 1)*0.5f, 0.0f, 0.f, 0.0f);
        glVertex3f(scaledVertices[faces_removed_back[i].x-1].x , scaledVertices[faces_removed_back[i].x -1 ].y , scaledVertices[faces_removed_back[i].x - 1].z);
        //glVertex3f(scaledVertices[faces[i].x-1].x , scaledVertices[faces[i].x -1 ].y , 0.f);


        glColor4f((scaledVertices[faces_removed_back[i].y-1].z + 1)*0.5f,0.f,  0.f, 0.0f);
        glVertex3f(scaledVertices[faces_removed_back[i].y-1].x, scaledVertices[faces_removed_back[i].y -1].y , scaledVertices[faces_removed_back[i].y -1].z);
        //glVertex3f(scaledVertices[faces[i].y-1].x, scaledVertices[faces[i].y -1].y ,0.f);

        glColor4f((scaledVertices[faces_removed_back[i].z-1].z + 1)*0.5f, 0.f, 0.f, 0.0f);
        glVertex3f(scaledVertices[faces_removed_back[i].z -1 ].x , scaledVertices[faces_removed_back[i].z -1 ].y , scaledVertices[faces_removed_back[i].z -1].z);
        //glVertex3f(scaledVertices[faces[i].z -1 ].x , scaledVertices[faces[i].z -1 ].y ,0.0f);

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
    //cameraPos = bazier_curve[0];
    //cameraPos = glm::vec3(0.0f, 0.5f, -0.5f);

    // Calculate new camera direction vector
    glm::vec3 cameraDirection = glm::normalize(lookAtPoint - cameraPos);

// Calculate new camera right vector
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));

// Calculate new camera up vector
    glm::vec3 cameraUpNew = glm::normalize(glm::cross(cameraDirection, cameraRight));

// Define the new view matrix using the updated camera vectors
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUpNew);

 // Set the view matrix for the current scene
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&view[0][0]);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, lookAtPoint.x, lookAtPoint.y, lookAtPoint.z, 0.0f, 1.0f, 0.0f);


    // Set up an orthographic projection matrix
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
// w key

    if (theKey == 'w' || theKey == 'W') {
        if (current_index < bazier_curve.size() - 1) {
            current_index++;
            cameraPos = bazier_curve[current_index];
            std::cout << "Camera position: " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
            std::cout << "Camera direction: " << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << std::endl;
            std::cout << "Camera up: " << cameraUp.x << " " << cameraUp.y << " " << cameraUp.z << std::endl;
            std::cout << "Look at point: " << lookAtPoint.x << " " << lookAtPoint.y << " " << lookAtPoint.z << std::endl;
            std::cout << "Current index: " << current_index << std::endl;
            std::cout << "-----------------------------------" << std::endl;
            myReshape(width, height);
        }
    }

    if (theKey == 's' || theKey == 'S') {
        if (current_index > 0) {
            current_index--;
            cameraPos = bazier_curve[current_index];
            std::cout << "Camera position: " << cameraPos.x << " " << cameraPos.y << " " << cameraPos.z << std::endl;
            std::cout << "Camera direction: " << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << std::endl;
            std::cout << "Camera up: " << cameraUp.x << " " << cameraUp.y << " " << cameraUp.z << std::endl;
            std::cout << "Look at point: " << lookAtPoint.x << " " << lookAtPoint.y << " " << lookAtPoint.z << std::endl;
            std::cout << "Current index: " << current_index << std::endl;
            std::cout << "-----------------------------------" << std::endl;
            myReshape(width, height);
        }
    }

}
