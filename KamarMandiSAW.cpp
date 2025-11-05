///////////////////////////////////
// movingCameraCube.cpp
//
// OpenGL program to draw a rectangular prism with a moving camera.
// Arrow keys rotate and move the camera.
//
// Based on square.cpp and spaceTravel.cpp by Sumanta Guha.
//
///////////////////////////////////

#include <stdlib.h>
#include <math.h>
#include <iostream>

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#pragma comment(lib, "glew32.lib")

#define PI 3.14159265

using namespace std;

// Globals
static float camX = -340.0;
static float camY = 100.0;
static float camZ = 400.0;
static float angle = 0.0;

// Light control globals
static float lightIntensity = 1.5;
static float ambientIntensity = 0.1;
static float linearAttenuation = 0.0005;
static float quadraticAttenuation = 0.00005;
static bool lightsEnabled[5] = { true, true, true, true, true };  // Toggle for each light

// Function prototypes
void drawScene(void);
void setup(void);
void resize(int w, int h);
void keyInput(unsigned char key, int x, int y);
void specialKeyInput(int key, int x, int y);

// Moving camera (from spaceTravel.cpp)
void setMovingCamera(float x, float y, float z, float angle)
{
    gluLookAt(
        x - 10 * sin((PI / 180.0) * angle),  // Camera position (behind)
        y,
        z - 10 * cos((PI / 180.0) * angle),

        x - 11 * sin((PI / 180.0) * angle),  // Look-at point (ahead)
        y,
        z - 11 * cos((PI / 180.0) * angle),

        0.0, 1.0, 0.0                        // Up direction
    );
}

// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply moving camera
    setMovingCamera(camX, camY, camZ, angle);

    // Flip horizontally (mirror across YZ plane)
    glScalef(-1.0, 1.0, 1.0);

    // Setup lighting AFTER transformations so light is in world space
    glEnable(GL_LIGHTING);

    // Common light properties
    GLfloat light_ambient[] = { ambientIntensity, ambientIntensity, ambientIntensity, 1.0 };
    GLfloat light_diffuse[] = { lightIntensity * 1.0f, lightIntensity * 1.0f, lightIntensity * 0.9f, 1.0 };
    GLfloat light_specular[] = { lightIntensity * 1.0f, lightIntensity * 1.0f, lightIntensity * 0.9f, 1.0 };

    // Light 0 - Right stall area (x=185, aligned with right stall)
    if (lightsEnabled[0]) {
        glEnable(GL_LIGHT0);
        GLfloat light0_position[] = { 185.0, 195.0, 290.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAttenuation);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
    }
    else {
        glDisable(GL_LIGHT0);
    }

    // Light 1 - Middle stall area (x=62, left stall)
    if (lightsEnabled[1]) {
        glEnable(GL_LIGHT1);
        GLfloat light1_position[] = { 62.0, 195.0, 290.0, 1.0 };
        glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
        glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, linearAttenuation);
        glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
    }
    else {
        glDisable(GL_LIGHT1);
    }

    // Light 2 - Player area, aligned with stalls (x=185, in front)
    if (lightsEnabled[2]) {
        glEnable(GL_LIGHT2);
        GLfloat light2_position[] = { 185.0, 195.0, 100.0, 1.0 };
        glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
        glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
        glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0);
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, linearAttenuation);
        glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
    }
    else {
        glDisable(GL_LIGHT2);
    }

    // Light 3 - Sink area, aligned with right stall light (x=185, near sink)
    if (lightsEnabled[3]) {
        glEnable(GL_LIGHT3);
        GLfloat light3_position[] = { 90.0, 195.0, 30.0, 1.0 };
        glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
        glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
        glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0);
        glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, linearAttenuation);
        glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
    }
    else {
        glDisable(GL_LIGHT3);
    }

    // Light 4 - Aligned with player and sink lights (x=62, in between)
    if (lightsEnabled[4]) {
        glEnable(GL_LIGHT4);
        GLfloat light4_position[] = { 300.0, 195.0, 100.0, 1.0 };
        glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
        glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular);
        glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1.0);
        glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, linearAttenuation);
        glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
    }
    else {
        glDisable(GL_LIGHT4);
    }

    // Enable color material so glColor works with lighting
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // wall
    // Face 1 (front wall - should face INTO the room, positive Z)
    glColor3f(1.0f, 0.98f, 0.82f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(30.0, 0.0, 0.0);
    glVertex3f(30.0, 80.0, 0.0);
    glVertex3f(430.0, 80.0, 0.0);
    glVertex3f(430.0, 0.0, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(30.0, 110.0, 0.0);
    glVertex3f(30.0, 200.0, 0.0);
    glVertex3f(430.0, 200.0, 0.0);
    glVertex3f(430.0, 110.0, 0.0);
    glEnd();

    // Face 2 (right wall - should face INTO the room, negative X)
    glColor3f(0.93f, 0.82f, 0.60f);
    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(430.0, 0.0, 0.0);
    glVertex3f(430.0, 80.0, 0.0);
    glVertex3f(430.0, 80.0, 420.0);
    glVertex3f(430.0, 0.0, 420.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(430.0, 110.0, 0.0);
    glVertex3f(430.0, 200.0, 0.0);
    glVertex3f(430.0, 200.0, 420.0);
    glVertex3f(430.0, 110.0, 420.0);
    glEnd();

    // Face 3 (back wall right section - should face INTO the room, negative Z)
    glColor3f(1.0f, 0.98f, 0.82f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(430.0, 0.0, 420.0);
    glVertex3f(250.0, 0.0, 420.0);
    glVertex3f(250.0, 80.0, 420.0);
    glVertex3f(430.0, 80.0, 420.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(430.0, 110.0, 420.0);
    glVertex3f(250.0, 110.0, 420.0);
    glVertex3f(250.0, 200.0, 420.0);
    glVertex3f(430.0, 200.0, 420.0);
    glEnd();

    // Face 4 (left wall - should face INTO the room, positive X)
    glColor3f(0.93f, 0.82f, 0.60f);
    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 420.0);
    glVertex3f(0.0, 0.0, 60.0);
    glVertex3f(0.0, 80.0, 60.0);
    glVertex3f(0.0, 80.0, 420.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 110.0, 420.0);
    glVertex3f(0.0, 110.0, 60.0);
    glVertex3f(0.0, 200.0, 60.0);
    glVertex3f(0.0, 200.0, 420.0);
    glEnd();

    // Face 5 (left front section - should face INTO the room, positive Z)
    glColor3f(1.0f, 0.98f, 0.82f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 60.0);
    glVertex3f(30.0, 0.0, 60.0);
    glVertex3f(30.0, 80.0, 60.0);
    glVertex3f(0.0, 80.0, 60.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 110.0, 60.0);
    glVertex3f(30.0, 110.0, 60.0);
    glVertex3f(30.0, 200.0, 60.0);
    glVertex3f(0.0, 200.0, 60.0);
    glEnd();

    // Face 6 (corner wall - at x=30, should face INTO the room, POSITIVE X to the right)
    glColor3f(0.93f, 0.82f, 0.60f);
    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);  // Changed from -1.0 to 1.0
    glVertex3f(30.0, 0.0, 60.0);
    glVertex3f(30.0, 0.0, 0.0);
    glVertex3f(30.0, 80.0, 0.0);
    glVertex3f(30.0, 80.0, 60.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);  // Changed from -1.0 to 1.0
    glVertex3f(30.0, 110.0, 60.0);
    glVertex3f(30.0, 110.0, 0.0);
    glVertex3f(30.0, 200.0, 0.0);
    glVertex3f(30.0, 200.0, 60.0);
    glEnd();


    // Indent
    glColor3f(1.0f, 0.98f, 0.82f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(250.0, 0.0, 420.0);
    glVertex3f(0.0, 0.0, 420.0);
    glVertex3f(0.0, 80.0, 420.0);
    glVertex3f(250.0, 80.0, 420.0);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(250.0, 110.0, 420.0);
    glVertex3f(250.0, 110.0, 440.0);
    glVertex3f(0.0, 110.0, 440.0);
    glVertex3f(0.0, 110.0, 420.0);
    glEnd();

    glColor3f(1.0f, 0.98f, 0.82f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(250.0, 110.0, 440.0);
    glVertex3f(250.0, 200.0, 440.0);
    glVertex3f(0.0, 200.0, 440.0);
    glVertex3f(0.0, 110.0, 440.0);
    glEnd();

    glColor3f(0.93f, 0.82f, 0.60f);
    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(250.0, 110.0, 420.0);
    glVertex3f(250.0, 110.0, 440.0);
    glVertex3f(250.0, 200.0, 440.0);
    glVertex3f(250.0, 200.0, 420.0);
    glEnd();

    glColor3f(0.93f, 0.82f, 0.60f);
    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 110.0, 420.0);
    glVertex3f(0.0, 110.0, 440.0);
    glVertex3f(0.0, 200.0, 440.0);
    glVertex3f(0.0, 200.0, 420.0);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);

    // Meja
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(150.0, 40.0, 50.0);
    glVertex3f(30.0, 40.0, 50.0);
    glVertex3f(30.0, 50.0, 50.0);
    glVertex3f(150.0, 50.0, 50.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(150.0, 40.0, 50.0);
    glVertex3f(30.0, 40.0, 50.0);
    glVertex3f(30.0, 40.0, 0.0);
    glVertex3f(150.0, 40.0, 0.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(150.0, 50.0, 50.0);
    glVertex3f(30.0, 50.0, 50.0);
    glVertex3f(30.0, 50.0, 0.0);
    glVertex3f(150.0, 50.0, 0.0);
    glEnd();


    // Samping Meja
    glColor3f(0.93f, 0.82f, 0.60f);
    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0.0, 0.0);  // Changed from 1.0 - left side faces LEFT
    glVertex3f(150.0, 0.0, 0.0);
    glVertex3f(150.0, 0.0, 60.0);
    glVertex3f(150.0, 110.0, 60.0);
    glVertex3f(150.0, 110.0, 0.0);
    glEnd();

    glColor3f(1.0f, 0.98f, 0.82f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(150.0, 0.0, 60.0);
    glVertex3f(160.0, 0.0, 60.0);
    glVertex3f(160.0, 110.0, 60.0);
    glVertex3f(150.0, 110.0, 60.0);
    glEnd();

    glColor3f(0.93f, 0.82f, 0.60f);
    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);  // Right side faces RIGHT
    glVertex3f(160.0, 0.0, 60.0);
    glVertex3f(160.0, 0.0, 0.0);
    glVertex3f(160.0, 110.0, 0.0);
    glVertex3f(160.0, 110.0, 60.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0, -1.0, 0.0);
    glVertex3f(160.0, 110.0, 60.0);
    glVertex3f(150.0, 110.0, 60.0);
    glVertex3f(150.0, 110.0, 0.0);
    glVertex3f(160.0, 110.0, 0.0);
    glEnd();


    // Stall 1
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);  // Front face should point backward (into stall area)
    glVertex3f(120.0, 160.0, 232.0);
    glVertex3f(0.0, 160.0, 232.0);
    glVertex3f(0.0, 10.0, 232.0);
    glVertex3f(120.0, 10.0, 232.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);  // Back face should point forward (into main room)
    glVertex3f(120.0, 160.0, 235.0);
    glVertex3f(0.0, 160.0, 235.0);
    glVertex3f(0.0, 10.0, 235.0);
    glVertex3f(120.0, 10.0, 235.0);
    glEnd();


    // Middle Wall
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(120.0, 0.0, 230.0);
    glVertex3f(125.0, 0.0, 230.0);
    glVertex3f(125.0, 160.0, 230.0);
    glVertex3f(120.0, 160.0, 230.0);
    glEnd();

    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);  // Changed from -1.0 - LEFT wall faces RIGHT
    glVertex3f(120.0, 0.0, 230.0);
    glVertex3f(120.0, 0.0, 440.0);
    glVertex3f(120.0, 160.0, 440.0);
    glVertex3f(120.0, 160.0, 230.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0.0, 0.0);  // Changed from 1.0 - RIGHT wall faces LEFT
    glVertex3f(125.0, 0.0, 230.0);
    glVertex3f(125.0, 0.0, 440.0);
    glVertex3f(125.0, 160.0, 440.0);
    glVertex3f(125.0, 160.0, 230.0);
    glEnd();


    // 2nd Stall
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);  // Front face should point backward
    glVertex3f(125.0, 10.0, 232.0);
    glVertex3f(250.0, 10.0, 232.0);
    glVertex3f(250.0, 160.0, 232.0);
    glVertex3f(125.0, 160.0, 232.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);  // Back face should point forward
    glVertex3f(125.0, 10.0, 235.0);
    glVertex3f(250.0, 10.0, 235.0);
    glVertex3f(250.0, 160.0, 235.0);
    glVertex3f(125.0, 160.0, 235.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);  // Front face
    glVertex3f(245.0, 0.0, 230.0);
    glVertex3f(245.0, 160.0, 230.0);
    glVertex3f(250.0, 160.0, 230.0);
    glVertex3f(250.0, 0.0, 230.0);
    glEnd();

    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);  // Changed from -1.0 - LEFT wall faces RIGHT
    glVertex3f(245.0, 0.0, 230.0);
    glVertex3f(245.0, 0.0, 440.0);
    glVertex3f(245.0, 160.0, 440.0);
    glVertex3f(245.0, 160.0, 230.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0.0, 0.0);  // Changed from 1.0 - RIGHT wall faces LEFT
    glVertex3f(250.0, 0.0, 230.0);
    glVertex3f(250.0, 0.0, 440.0);
    glVertex3f(250.0, 160.0, 440.0);
    glVertex3f(250.0, 160.0, 230.0);
    glEnd();


    //floor & roof
    glColor3f(0.72f, 0.65f, 0.53f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 1.0, 0.0);  // Floor faces up
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 420.0);
    glVertex3f(430.0, 0.0, 420.0);
    glVertex3f(430.0, 0.0, 0.0);
    glEnd();

    glColor3f(0.76f, 0.69f, 0.57f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, -1.0, 0.0);  // Ceiling faces down (into room)
    glVertex3f(0.0, 200.0, 440.0);
    glVertex3f(430.0, 200.0, 440.0);
    glVertex3f(430.0, 200.0, 0.0);
    glVertex3f(0.0, 200.0, 0.0);
    glEnd();


    //black strip
    // Face 1
    glColor3f(1.0f, 0.98f, 0.82f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(30.0, 110.0, 0.0);
    glVertex3f(30.0, 80.0, 0.0);
    glVertex3f(150.0, 80.0, 0.0);
    glVertex3f(150.0, 110.0, 0.0);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(150.0, 110.0, 0.0);
    glVertex3f(150.0, 80.0, 0.0);
    glVertex3f(430.0, 80.0, 0.0);
    glVertex3f(430.0, 110.0, 0.0);
    glEnd();

    // Face 2
    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(430.0, 110.0, 0.0);
    glVertex3f(430.0, 80.0, 0.0);
    glVertex3f(430.0, 80.0, 420.0);
    glVertex3f(430.0, 110.0, 420.0);
    glEnd();

    // Face 3
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(430.0, 110.0, 420.0);
    glVertex3f(250.0, 110.0, 420.0);
    glVertex3f(250.0, 80.0, 420.0);
    glVertex3f(430.0, 80.0, 420.0);
    glEnd();

    // Face 4
    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 110.0, 420.0);
    glVertex3f(0.0, 110.0, 60.0);
    glVertex3f(0.0, 80.0, 60.0);
    glVertex3f(0.0, 80.0, 420.0);
    glEnd();

    // Face 5
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 110.0, 60.0);
    glVertex3f(30.0, 110.0, 60.0);
    glVertex3f(30.0, 80.0, 60.0);
    glVertex3f(0.0, 80.0, 60.0);
    glEnd();

    // Face 6
    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(30.0, 110.0, 60.0);
    glVertex3f(30.0, 110.0, 0.0);
    glVertex3f(30.0, 80.0, 0.0);
    glVertex3f(30.0, 80.0, 60.0);
    glEnd();

    // Indent
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(250.0, 110.0, 420.0);
    glVertex3f(0.0, 110.0, 420.0);
    glVertex3f(0.0, 80.0, 420.0);
    glVertex3f(250.0, 80.0, 420.0);
    glEnd();


    //kaca
    glColor3f(0.80f, 0.90f, 1.0f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, 1.0);
    glVertex3f(32.0, 130.0, 0.2);
    glVertex3f(32.0, 70.0, 0.2);
    glVertex3f(148.0, 70.0, 0.2);
    glVertex3f(148.0, 130.0, 0.2);
    glEnd();


    // Draw light fixtures on the ceiling
    glDisable(GL_LIGHTING);

    glColor3f(1.0, 1.0, 0.9);

    // Light fixture 0 - Right stall
    if (lightsEnabled[0]) {
        glPushMatrix();
        glTranslatef(185.0, 195.0, 290.0);
        glutSolidSphere(5.0, 20, 20);
        glPopMatrix();
    }

    // Light fixture 1 - Left stall
    if (lightsEnabled[1]) {
        glPushMatrix();
        glTranslatef(62.0, 195.0, 290.0);
        glutSolidSphere(5.0, 20, 20);
        glPopMatrix();
    }

    // Light fixture 2 - Player area
    if (lightsEnabled[2]) {
        glPushMatrix();
        glTranslatef(185.0, 195.0, 100.0);
        glutSolidSphere(5.0, 20, 20);
        glPopMatrix();
    }

    // Light fixture 3 - Sink area
    if (lightsEnabled[3]) {
        glPushMatrix();
        glTranslatef(90.0, 195.0, 30.0);
        glutSolidSphere(5.0, 20, 20);
        glPopMatrix();
    }

    // Light fixture 4 - Aligned position
    if (lightsEnabled[4]) {
        glPushMatrix();
        glTranslatef(300.0, 195.0, 100.0);
        glutSolidSphere(5.0, 20, 20);
        glPopMatrix();
    }

    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 0.0);

    // Enable smooth shading for better lighting
    glShadeModel(GL_SMOOTH);

    // Set up material properties
    GLfloat mat_specular[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat mat_shininess[] = { 20.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / (float)h, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;

        // Light intensity controls
    case 'i':
    case 'I':
        lightIntensity += 0.1;
        cout << "Light Intensity: " << lightIntensity << endl;
        break;

    case 'k':
    case 'K':
        lightIntensity -= 0.1;
        if (lightIntensity < 0.0) lightIntensity = 0.0;
        cout << "Light Intensity: " << lightIntensity << endl;
        break;

        // Ambient light controls
    case 'a':
    case 'A':
        ambientIntensity += 0.05;
        cout << "Ambient Intensity: " << ambientIntensity << endl;
        break;

    case 's':
    case 'S':
        ambientIntensity -= 0.05;
        if (ambientIntensity < 0.0) ambientIntensity = 0.0;
        cout << "Ambient Intensity: " << ambientIntensity << endl;
        break;

        // Linear attenuation controls (how quickly light fades)
    case 'l':
    case 'L':
        linearAttenuation += 0.0005;
        cout << "Linear Attenuation: " << linearAttenuation << endl;
        break;

    case 'j':
    case 'J':
        linearAttenuation -= 0.0005;
        if (linearAttenuation < 0.0) linearAttenuation = 0.0;
        cout << "Linear Attenuation: " << linearAttenuation << endl;
        break;

        // Quadratic attenuation controls (realistic distance falloff)
    case 'q':
    case 'Q':
        quadraticAttenuation += 0.00005;
        cout << "Quadratic Attenuation: " << quadraticAttenuation << endl;
        break;

    case 'w':
    case 'W':
        quadraticAttenuation -= 0.00005;
        if (quadraticAttenuation < 0.0) quadraticAttenuation = 0.0;
        cout << "Quadratic Attenuation: " << quadraticAttenuation << endl;
        break;

        // Reset to defaults
    case 'r':
    case 'R':
        lightIntensity = 1.5;
        ambientIntensity = 0.1;
        linearAttenuation = 0.0005;
        quadraticAttenuation = 0.00005;
        cout << "Light settings reset to defaults" << endl;
        break;

        // Display help
    case 'h':
    case 'H':
        cout << "\n=== LIGHT CONTROLS ===" << endl;
        cout << "I/K - Increase/Decrease light intensity" << endl;
        cout << "A/S - Increase/Decrease ambient light" << endl;
        cout << "L/J - Increase/Decrease linear attenuation (fade speed)" << endl;
        cout << "Q/W - Increase/Decrease quadratic attenuation (distance falloff)" << endl;
        cout << "1-5 - Toggle individual lights (Light 1-5)" << endl;
        cout << "R - Reset all light settings" << endl;
        cout << "H - Show this help" << endl;
        cout << "ESC - Exit program" << endl;
        cout << "=====================\n" << endl;
        break;

        // Toggle individual lights
    case '1':
        lightsEnabled[0] = !lightsEnabled[0];
        cout << "Light 1 (Right Stall): " << (lightsEnabled[0] ? "ON" : "OFF") << endl;
        break;
    case '2':
        lightsEnabled[1] = !lightsEnabled[1];
        cout << "Light 2 (Left Stall): " << (lightsEnabled[1] ? "ON" : "OFF") << endl;
        break;
    case '3':
        lightsEnabled[2] = !lightsEnabled[2];
        cout << "Light 3 (Player Area): " << (lightsEnabled[2] ? "ON" : "OFF") << endl;
        break;
    case '4':
        lightsEnabled[3] = !lightsEnabled[3];
        cout << "Light 4 (Sink): " << (lightsEnabled[3] ? "ON" : "OFF") << endl;
        break;
    case '5':
        lightsEnabled[4] = !lightsEnabled[4];
        cout << "Light 5 (Aligned): " << (lightsEnabled[4] ? "ON" : "OFF") << endl;
        break;

    default:
        break;
    }

    glutPostRedisplay();
}

// Special key input (arrow keys)
void specialKeyInput(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        angle += 5.0;
        break;
    case GLUT_KEY_RIGHT:
        angle -= 5.0;
        break;
    case GLUT_KEY_UP:
        camX -= sin(angle * PI / 180.0) * 10;
        camZ -= cos(angle * PI / 180.0) * 10;
        break;
    case GLUT_KEY_DOWN:
        camX += sin(angle * PI / 180.0) * 10;
        camZ += cos(angle * PI / 180.0) * 10;
        break;
    }

    glutPostRedisplay();
}

// Main routine.
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

#ifdef _WIN32
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
#endif

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("movingCameraCube.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

#ifdef _WIN32
    glewExperimental = GL_TRUE;
    glewInit();
#endif

    setup();

    // Display initial help
    cout << "\n=== LIGHT CONTROLS ===" << endl;
    cout << "I/K - Increase/Decrease light intensity" << endl;
    cout << "A/S - Increase/Decrease ambient light" << endl;
    cout << "L/J - Increase/Decrease linear attenuation (fade speed)" << endl;
    cout << "Q/W - Increase/Decrease quadratic attenuation (distance falloff)" << endl;
    cout << "1-5 - Toggle individual lights (Light 1-5)" << endl;
    cout << "R - Reset all light settings" << endl;
    cout << "H - Show this help" << endl;
    cout << "Arrow Keys - Move camera" << endl;
    cout << "ESC - Exit program" << endl;
    cout << "=====================\n" << endl;

    glutMainLoop();
}