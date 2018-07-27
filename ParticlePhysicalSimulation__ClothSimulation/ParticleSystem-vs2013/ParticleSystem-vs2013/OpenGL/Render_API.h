#ifndef _RENDER_API_H_
#define _RENDER_API_H_

#include "Vector3d.h"

enum{
    Red = 0,   
    Green,
    Blue,
    Yellow,
    Orange,
    TotalColor
};

// set color from list
void setColor(const int color);

// set color by directly assign the RGB (and alpha) value
void setColor3d(const unsigned char red, const unsigned char green, const unsigned char blue);     //set integer value range from 0 to 255

void setColor3f(const double red, const double green, const double blue);

void setColor3fv(const Vector3d color);

void setColor4f(const double red, const double green, const double blue, const double alpha);


// set translation and rotation of an object
void setTranslate(const Vector3d &transVec);

void setRotation(const double rotAngle, const Vector3d &rotAxis);

// draw basic shape
void drawPoint(const Vector3d &pos, const double size);

void drawLine(const Vector3d &startPoint, const Vector3d &endPoint);

void drawCylinder(const Vector3d &startPoint, const Vector3d &endPoint, double radius);

void drawBall(const Vector3d &ballPos, double radius);

void drawSolidBall(const Vector3d &ballPos, double radius);

#endif
