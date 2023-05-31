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


// Define camera position, direction, and up vectors
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  2.0f);
glm::vec3 lookAtPoint;


// Lighting
glm::vec3 lightPos = glm::vec3(0.0f, -3.0f,  -2.0f);
bool constantLight = true;

// normals

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

    std::vector<glm::vec3> faces_removed_back = Utils::removeBackFaces(scaledVertices, faces, cameraPos);
    std::vector<glm::vec3> normals = std::vector<glm::vec3>(scaledVertices.size());
    Utils::calculateNormals(scaledVertices, faces_removed_back, normals);

    for(int i=0; i< faces_removed_back.size(); i++){
        glBegin(GL_POLYGON);
        uint32_t v1_index = faces_removed_back[i].x-1;
        uint32_t v2_index = faces_removed_back[i].y-1;
        uint32_t v3_index = faces_removed_back[i].z-1;

        glm::vec3 v1 = glm::vec3(scaledVertices[v1_index].x , scaledVertices[v1_index ].y , scaledVertices[v1_index].z);
        glm::vec3 v2 = glm::vec3(scaledVertices[v2_index].x, scaledVertices[v2_index].y , scaledVertices[v2_index].z);
        glm::vec3 v3 = glm::vec3(scaledVertices[v3_index].x, scaledVertices[v3_index].y , scaledVertices[v3_index].z);

        uint32_t sourceIntensity = 200;
        uint32_t shadowIntensity = 10;
        uint32_t n = 1;

        // 0 - constant, 1 - Gouraud, 2 - Phong
        glm::vec3 light_g;        //glColor4f(norm_light, norm_light, norm_light, 1.0f);

        if (constantLight){
            float light = Utils::constantLighting(v1, v2, v3, lightPos, cameraPos, sourceIntensity, shadowIntensity, 0.5f, 0.5f, 0.5f, n);

            light_g.x = light;
            light_g.y = light;
            light_g.z = light;
        }
        else{
            light_g = Utils::gouraudoLighting(v1_index, v2_index, v3_index, scaledVertices, normals, lightPos, cameraPos, sourceIntensity, shadowIntensity, 0.5f, 0.5f, 0.5f, n);

        }
        glColor4f(light_g.x, light_g.x, light_g.x, 1.0f);
        glVertex3f(v1.x, v1.y, v1.z);
        glColor4f(light_g.y, light_g.y, light_g.y, 1.0f);
        glVertex3f(v2.x, v2.y, v2.z);
        glColor4f(light_g.z, light_g.z, light_g.z, 1.0f);
        glVertex3f(v3.x, v3.y, v3.z);

        glEnd();
    }


}


void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    myPolygon();

    glLoadIdentity();
    gluPerspective(45.0f, 1.f, 0.1f, 100.f);  // Perspective projection

    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, lookAtPoint.x, lookAtPoint.y, lookAtPoint.z, 0.0f, 1.0f, 0.0f);



    glFlush();
}

void myMouse(int button, int state, int x, int y){

}


//*********************************************************************************
//	Promjena velicine prozora.
//*********************************************************************************

void myReshape(int w, int h) {
// glMatrixMode(GL_MODELVIEW);



    glClearColor(0.5f, 1.0f, 1.0f, 0.0f); // boja pozadine
    glClear(GL_COLOR_BUFFER_BIT);        //	brisanje pozadine
    myDisplay();
}


//*********************************************************************************
//	Mis.
//*********************************************************************************



void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
    if (theKey == 'w' || theKey == 'W'){
        cameraPos.z -= 0.1f;
       // myReshape(width, height);
       myDisplay();
    }
    if (theKey == 's' || theKey == 'S'){
        cameraPos.z += 0.1f;
//        myReshape(width, height);
        myDisplay();

    }
    if (theKey == 'a' || theKey == 'A'){
        cameraPos.x -= 0.1f;
//        myReshape(width, height);
        myDisplay();

    }
    if (theKey == 'd' || theKey == 'D'){
        cameraPos.x += 0.1f;
//        myReshape(width, height);
        myDisplay();

    }

    if (theKey == 'g' || theKey == 'G') {
        constantLight = !constantLight;
        myDisplay();

    }
}
