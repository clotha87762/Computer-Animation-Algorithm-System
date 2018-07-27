#ifdef WIN32
#include <FL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "Render_API.h"
#include "glut.h"
#include "glui.h" 


void setColor(const int color)
{
    switch(color){
    case 0:
        glColor3f(1., .2, .2);
        break;
    case 1:
        glColor3f(.2, 1., .2);
        break;
    case 2:
        glColor3f(.2, .2, 1.);
        break;
    case 3:
        glColor4f(1., 1., 0.1, 0.9);
        break;
    case 4:
        glColor4f(1.0, 0.54, .1, 0.9);
        break;
    }
}

void setColor3d(const unsigned char red, const unsigned char green, const unsigned char blue)
{
    double fRed = static_cast<double>(red)/255.0;
    double fGreen = static_cast<double>(green)/255.0;
    double fBlue = static_cast<double>(blue)/255.0;
    glColor3f(fRed, fGreen, fBlue);
}
    
void setColor3f(const double red, const double green, const double blue)
{
    glColor3f(red, green, blue);
}

void setColor3fv(const Vector3d color)
{
    glColor3dv(color.val);
}

void setColor4f(const double red, const double green, const double blue, const double alpha)
{
    glEnable(GL_BLEND);
    glColor4f(red, green, blue, alpha);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}


void setTranslate(const Vector3d &transVec)
{
    glTranslatef(transVec.x, transVec.y, transVec.z);
}

void setRotation(const double rotAngle, const Vector3d &rotAxis)
{
    glRotatef(rotAngle, rotAxis.x, rotAxis.y, rotAxis.z);
}

void drawPoint(const Vector3d &pos, double size)
{
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex3dv(pos.val);
    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawLine(const Vector3d &startPoint, const Vector3d &endPoint)
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glVertex3f(startPoint.x, startPoint.y, startPoint.z);		// start
    glVertex3f(endPoint.x, endPoint.y, endPoint.z);		        // end
    glEnd();
    glEnable(GL_LIGHTING);
}


void drawCylinder(const Vector3d &startPoint, const Vector3d &endPoint, double radius)
{
    //the same quadric can be re-used for drawing many cylinders
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    int subdivisions = 18;

    Vector3d cyl_vec;
    cyl_vec = endPoint - startPoint;
    double vx = cyl_vec.x;
    double vy = cyl_vec.y;
    double vz = cyl_vec.z;
    double ax,rx,ry,rz;
    double cyl_len = cyl_vec.Length();

    glPushMatrix();

    glTranslatef(startPoint.x, startPoint.y, startPoint.z);
    if (fabs(vz) < 0.0001){
        glRotatef(90, 0, 1, 0);
        ax = 57.2957795*-atan( vy / vx );
        if (vx < 0){
            ax = ax + 180;
        }
        rx = 1;
        ry = 0;
        rz = 0;
    }
    else{
        ax = 57.2957795*acos( vz/ cyl_len );
        if (vz < 0.0)
            ax = -ax;
        rx = -vy*vz;
        ry = vx*vz;
        rz = 0;
    }
    glRotatef(ax, rx, ry, rz);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, cyl_len, subdivisions, 1);
    
    glPopMatrix();

    gluDeleteQuadric(quadric);
}

void drawBall(const Vector3d &ballPos, double radius)
{
	glPushMatrix();
    glTranslatef(ballPos.x, ballPos.y, ballPos.z);    // ²yªº¦ì¸m
	GLUquadricObj *qobj;
	qobj=gluNewQuadric();
	gluQuadricDrawStyle(qobj, (GLenum) GLU_FILL);
    gluQuadricNormals(qobj, (GLenum) GLU_SMOOTH);
	glScalef(.3, .3, .3);
	gluSphere(qobj,radius,25,25); 
	glPopMatrix(); 
}

void drawSolidBall(const Vector3d &ballPos, double radius)
{
    glPushMatrix();
    glTranslatef(ballPos.x, ballPos.y, ballPos.z);
    glutSolidSphere(radius, 50, 50);
    glPopMatrix();
}