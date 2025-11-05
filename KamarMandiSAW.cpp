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

// Door animation globals
static float doorAngle = 0.0;
static bool doorsOpen = false;
static const float maxDoorAngle = 90.0;
static const float doorSpeed = 5.0;

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
void animate(int value);

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

    // Light 0 - Back row, left position
    if (lightsEnabled[0]) {
        glEnable(GL_LIGHT0);
        GLfloat light0_position[] = { 107.5, 195.0, 315.0, 1.0 };  // Back left
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

    // Light 1 - Back row, center position
    if (lightsEnabled[1]) {
        glEnable(GL_LIGHT1);
        GLfloat light1_position[] = { 215.0, 195.0, 315.0, 1.0 };  // Back center
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

    // Light 2 - Back row, right position
    if (lightsEnabled[2]) {
        glEnable(GL_LIGHT2);
        GLfloat light2_position[] = { 322.5, 195.0, 315.0, 1.0 };  // Back right
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

    // Light 3 - Front row, left position
    if (lightsEnabled[3]) {
        glEnable(GL_LIGHT3);
        GLfloat light3_position[] = { 107.5, 195.0, 140.0, 1.0 };  // Front left
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

    // Light 4 - Front row, right position
    if (lightsEnabled[4]) {
        glEnable(GL_LIGHT4);
        GLfloat light4_position[] = { 322.5, 195.0, 140.0, 1.0 };  // Front right
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


    // Meja
    glColor3f(0.0, 0.0, 0.0);
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


    // Urinal 1 (left of divider)
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Urinal back panel
    glPushMatrix();
    glTranslatef(250.0, 60.0, 5.0);
    glScalef(20.0, 50.0, 4.0);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Urinal bowl
    glPushMatrix();
    glTranslatef(250.0, 45.0, 12.0);
    glScalef(16.0, 35.0, 10.0);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
    
    // Urinal drain pipe
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(250.0, 30.0, 12.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(2.5, 30.0, 15, 15);
    glPopMatrix();
    
    // Flush pipe
    glPushMatrix();
    glTranslatef(250.0, 85.0, 5.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(2.0, 20.0, 15, 15);
    glPopMatrix();


    // Urinal 2 (right of divider)
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Urinal back panel
    glPushMatrix();
    glTranslatef(370.0, 60.0, 5.0);
    glScalef(20.0, 50.0, 4.0);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Urinal bowl
    glPushMatrix();
    glTranslatef(370.0, 45.0, 12.0);
    glScalef(16.0, 35.0, 10.0);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
    
    // Urinal drain pipe
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(370.0, 30.0, 12.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(2.5, 30.0, 15, 15);
    glPopMatrix();
    
    // Flush pipe
    glPushMatrix();
    glTranslatef(370.0, 85.0, 5.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidCylinder(2.0, 20.0, 15, 15);
    glPopMatrix();


    // Stall 1
    glColor3f(0.7f, 0.7f, 0.7f);

    glPushMatrix();
    // Rotate around the left edge (x=0) of the door
    glTranslatef(0.0, 0.0, 233.5);  // Move to door center position
    glRotatef(-doorAngle, 0.0, 1.0, 0.0);  // Rotate around Y axis
    glTranslatef(0.0, 0.0, -233.5);  // Move back

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

    glPopMatrix();


    // Middle Wall (separator between stall 1 and stall 2)
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(120.0, 0.0, 230.0);
    glVertex3f(125.0, 0.0, 230.0);
    glVertex3f(125.0, 160.0, 230.0);
    glVertex3f(120.0, 160.0, 230.0);
    glEnd();

    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(120.0, 0.0, 230.0);
    glVertex3f(120.0, 0.0, 440.0);
    glVertex3f(120.0, 160.0, 440.0);
    glVertex3f(120.0, 160.0, 230.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(125.0, 0.0, 230.0);
    glVertex3f(125.0, 0.0, 440.0);
    glVertex3f(125.0, 160.0, 440.0);
    glVertex3f(125.0, 160.0, 230.0);
    glEnd();


    // 2nd Stall
    glColor3f(0.7f, 0.7f, 0.7f);

    glPushMatrix();
    // Rotate around the left edge (x=125) of the door
    glTranslatef(125.0, 0.0, 233.5);  // Move to door hinge position
    glRotatef(-doorAngle, 0.0, 1.0, 0.0);  // Rotate around Y axis
    glTranslatef(-125.0, 0.0, -233.5);  // Move back

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

    glPopMatrix();

    // Right side wall of stall 2
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(245.0, 0.0, 230.0);
    glVertex3f(245.0, 160.0, 230.0);
    glVertex3f(250.0, 160.0, 230.0);
    glVertex3f(250.0, 0.0, 230.0);
    glEnd();

    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_POLYGON);
    glNormal3f(1.0, 0.0, 0.0);
    glVertex3f(245.0, 0.0, 230.0);
    glVertex3f(245.0, 0.0, 440.0);
    glVertex3f(245.0, 160.0, 440.0);
    glVertex3f(245.0, 160.0, 230.0);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(-1.0, 0.0, 0.0);
    glVertex3f(250.0, 0.0, 230.0);
    glVertex3f(250.0, 0.0, 440.0);
    glVertex3f(250.0, 160.0, 440.0);
    glVertex3f(250.0, 160.0, 230.0);
    glEnd();


    // Toilet 1 (in first stall)
    // Toilet bowl base
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(60.0, 5.0, 390.0);
    glScalef(20.0, 8.0, 25.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // Toilet bowl - curved part
    glPushMatrix();
    glTranslatef(60.0, 15.0, 385.0);
    glScalef(18.0, 12.0, 20.0);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    // Toilet seat rim
    glColor3f(0.85f, 0.85f, 0.85f);
    glPushMatrix();
    glTranslatef(60.0, 22.0, 385.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidTorus(2.0, 12.0, 15, 25);
    glPopMatrix();

    // Toilet tank
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(60.0, 35.0, 410.0);
    glScalef(22.0, 25.0, 12.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // Toilet tank lid
    glColor3f(0.95f, 0.95f, 0.95f);
    glPushMatrix();
    glTranslatef(60.0, 48.0, 410.0);
    glScalef(23.0, 2.0, 13.0);
    glutSolidCube(1.0);
    glPopMatrix();


    // Toilet 2 (in second stall)
    // Toilet bowl base
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(185.0, 5.0, 390.0);
    glScalef(20.0, 8.0, 25.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // Toilet bowl - curved part
    glPushMatrix();
    glTranslatef(185.0, 15.0, 385.0);
    glScalef(18.0, 12.0, 20.0);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    // Toilet seat rim
    glColor3f(0.85f, 0.85f, 0.85f);
    glPushMatrix();
    glTranslatef(185.0, 22.0, 385.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidTorus(2.0, 12.0, 15, 25);
    glPopMatrix();

    // Toilet tank
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(185.0, 35.0, 410.0);
    glScalef(22.0, 25.0, 12.0);
    glutSolidCube(1.0);
    glPopMatrix();

    // Toilet tank lid
    glColor3f(0.95f, 0.95f, 0.95f);
    glPushMatrix();
    glTranslatef(185.0, 48.0, 410.0);
    glScalef(23.0, 2.0, 13.0);
    glutSolidCube(1.0);
    glPopMatrix();


    // Indent section (back wall recess)
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


    //floor & roof
    glColor3f(0.72f, 0.65f, 0.53f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 420.0);
    glVertex3f(430.0, 0.0, 420.0);
    glVertex3f(430.0, 0.0, 0.0);
    glEnd();

    glColor3f(0.76f, 0.69f, 0.57f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, -1.0, 0.0);
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

    // Indent section for black strip (y=80 to y=110 at z=420)
    glColor3f(1.0f, 0.98f, 0.82f);
    glBegin(GL_POLYGON);
    glNormal3f(0.0, 0.0, -1.0);
    glVertex3f(250.0, 110.0, 420.0);
    glVertex3f(0.0, 110.0, 420.0);
    glVertex3f(0.0, 80.0, 420.0);
    glVertex3f(250.0, 80.0, 420.0);
    glEnd();

    // Face 4
    glColor3f(0.0f, 0.0f, 0.0f);
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

    // Light fixture 0 - Back left
    if (lightsEnabled[0]) {
        glPushMatrix();
        glTranslatef(107.5, 195.0, 315.0);
        glutSolidSphere(5.0, 20, 20);
        glPopMatrix();
    }

    // Light fixture 1 - Back center
    if (lightsEnabled[1]) {
        glPushMatrix();
        glTranslatef(215.0, 195.0, 315.0);
        glutSolidSphere(5.0, 20, 20);
        glPopMatrix();
    }

    // Light fixture 2 - Back right
    if (lightsEnabled[2]) {
        glPushMatrix();
        glTranslatef(322.5, 195.0, 315.0);
        glutSolidSphere(5.0, 20, 20);
        glPopMatrix();
    }

    // Light fixture 3 - Front left
    if (lightsEnabled[3]) {
        glPushMatrix();
        glTranslatef(107.5, 195.0, 140.0);
        glutSolidSphere(5.0, 20, 20);
        glPopMatrix();
    }

    // Light fixture 4 - Front right
    if (lightsEnabled[4]) {
        glPushMatrix();
        glTranslatef(322.5, 195.0, 140.0);
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

    case ' ':  // Space bar to toggle doors
        doorsOpen = !doorsOpen;
        glutTimerFunc(16, animate, 0);  // Start animation (60 FPS)
        cout << "Doors " << (doorsOpen ? "opening..." : "closing...") << endl;
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
        cout << "SPACE - Open/Close stall doors" << endl;
        cout << "ESC - Exit program" << endl;
        cout << "=====================\n" << endl;
        break;

        // Toggle individual lights
    case '1':
        lightsEnabled[0] = !lightsEnabled[0];
        cout << "Light 1 (Back Left): " << (lightsEnabled[0] ? "ON" : "OFF") << endl;
        break;
    case '2':
        lightsEnabled[1] = !lightsEnabled[1];
        cout << "Light 2 (Back Center): " << (lightsEnabled[1] ? "ON" : "OFF") << endl;
        break;
    case '3':
        lightsEnabled[2] = !lightsEnabled[2];
        cout << "Light 3 (Back Right): " << (lightsEnabled[2] ? "ON" : "OFF") << endl;
        break;
    case '4':
        lightsEnabled[3] = !lightsEnabled[3];
        cout << "Light 4 (Front Left): " << (lightsEnabled[3] ? "ON" : "OFF") << endl;
        break;
    case '5':
        lightsEnabled[4] = !lightsEnabled[4];
        cout << "Light 5 (Front Right): " << (lightsEnabled[4] ? "ON" : "OFF") << endl;
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

// Animation routine for door opening/closing
void animate(int value)
{
    bool needsUpdate = false;

    if (doorsOpen && doorAngle < maxDoorAngle) {
        doorAngle += doorSpeed;
        if (doorAngle > maxDoorAngle) doorAngle = maxDoorAngle;
        needsUpdate = true;
    }
    else if (!doorsOpen && doorAngle > 0.0) {
        doorAngle -= doorSpeed;
        if (doorAngle < 0.0) doorAngle = 0.0;
        needsUpdate = true;
    }

    if (needsUpdate) {
        glutPostRedisplay();
        glutTimerFunc(16, animate, 0);  // Continue animation
    }
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
    cout << "SPACE - Open/Close stall doors" << endl;
    cout << "ESC - Exit program" << endl;
    cout << "=====================\n" << endl;

    glutMainLoop();
}