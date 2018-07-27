#include "render_utils.h"
#include "FL/gl.h"
#include "FL/glut.H"
#include "FL/glu.h"
#include "geom_arrow.h"
#include "render_camera.h"
#include "render_polar_rotation.h"

namespace render {

namespace {

render::Camera g_Camera;

} // namespace {

void SetColor(const int color)
{
    switch (color)
    {
        case 0:
            glColor3d(1., .2, .2);
            break;
        case 1:
            glColor3d(.2, 1., .2);
            break;
        case 2:
            glColor3d(.2, .2, 1.);
            break;
        case 3:
            glColor4d(1., 1., 0.1, 0.9);
            break;
        case 4:
            glColor4d(1.0, 0.54, .1, 0.9);
            break;
        default:
            break;
    }
}

void SetColor3b(
        const char red, 
        const char green, 
        const char blue
        )
{
    glColor3b(red, green, blue);
}

void SetColor3ub(
        const unsigned char red, 
        const unsigned char green, 
        const unsigned char blue
        )
{
    glColor3ub(red, green, blue);    
}

void SetColor3d(
        const double red, 
        const double green, 
        const double blue
        )
{
    glColor3d(red, green, blue);
}

void SetColor4d(const double red, const double green, const double blue, const double alpha)
{
    glEnable(GL_BLEND);
    glColor4d(red, green, blue, alpha);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

double CameraZoom()
{
    return g_Camera.zoom();
}

render::PolarRotation CameraRotation()
{
    return g_Camera.polar_rotation();
}

Vector3d_t CameraTranslation()
{
    return g_Camera.translation();
}

Vector3d_t CameraLookAt()
{
    return g_Camera.look_at_pos();
}

void SetCameraView()
{
    glTranslated(g_Camera.translation().x(), g_Camera.translation().y(), g_Camera.translation().z());
    glTranslated(g_Camera.look_at_pos().x(), g_Camera.look_at_pos().y(), g_Camera.look_at_pos().z());

    glRotated(-g_Camera.twist(), 0.0, 1.0, 0.0);
    glRotated(-g_Camera.elevation(), 1.0, 0.0, 0.0);
    glRotated(g_Camera.azimuth(), 0.0, 1.0, 0.0);

    glTranslated(-g_Camera.look_at_pos().x(), -g_Camera.look_at_pos().y(), -g_Camera.look_at_pos().z());
    glScaled(g_Camera.zoom(), g_Camera.zoom(), g_Camera.zoom());
}

void SetCameraZoom(const double zoom)
{
    g_Camera.set_zoom(zoom);
}

void SetCameraRotation(
        const double azimuth,
        const double elevation,
        const double twist
        )
{
    g_Camera.set_polar_rotation(
            PolarRotation(azimuth, elevation, twist)
            );
}

void ShiftCameraRotation(
        const double azimuth,
        const double elevation,
        const double twist
        )
{
    g_Camera.ShiftRotation(
            azimuth,
            elevation,
            twist
            );
}

void SetCameraTranslation(const Vector3d_t &xlt)
{
    g_Camera.set_translation(xlt);
}

void ShiftCameraTranslation(const Vector3d_t &shift_xlt)
{
    g_Camera.ShiftTranslation(shift_xlt);
}

void SetCameraLookAt(const Vector3d_t &pos)
{
    g_Camera.set_look_at_pos(pos);
}

void ShiftCameraLookAt(const Vector3d_t &shift_pos)
{
    g_Camera.ShiftLookAt(shift_pos);
}

void SetTranslate(const Vector3d_t &xlt)
{
    glTranslated(xlt.x(), xlt.y(), xlt.z());
}

void SetRotation(const double radian, const Vector3d_t &rotation_axis)
{
    glRotated(radian, rotation_axis.x(), rotation_axis.y(), rotation_axis.z());
}

void DrawLine(const Vector3d_t &start_pt, const Vector3d_t &end_point)
{
    glBegin(GL_LINES);
    glVertex3d(start_pt.x(), start_pt.y(), start_pt.z());		// start
    glVertex3d(end_point.x(), end_point.y(), end_point.z());		// end
    glEnd();
}

void DrawCylinder(
        const Vector3d_t &start_pt, 
        const Vector3d_t &end_point, 
        double radius
        )
{
    //the same quadric can be re-used for drawing many cylinders
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    static const int kSubdivisions = 18;

    Vector3d_t cyl_vec;
    cyl_vec = end_point - start_pt;
    double vx = cyl_vec.x();
    double vy = cyl_vec.y();
    double vz = cyl_vec.z();
    double ax,rx,ry,rz;
    double cyl_len = cyl_vec.norm();

    glPushMatrix();

    glTranslated(start_pt.x(), start_pt.y(), start_pt.z());
    if (fabs(vz) < 0.0001)
    {
        glRotated(90, 0, 1, 0);
        ax = 57.2957795*-atan( vy / vx );
        if (vx < 0){
            ax = ax + 180;
        }
        rx = 1;
        ry = 0;
        rz = 0;
    }
    else
    {
        ax = 57.2957795 * acos(vz/cyl_len);
        if (vz < 0.0)
        {
            ax = -ax;
        }
        rx = -vy*vz;
        ry = vx*vz;
        rz = 0;
    }
    glRotated(ax, rx, ry, rz);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, cyl_len, kSubdivisions, 1);

    glPopMatrix();

    gluDeleteQuadric(quadric);
}

void DrawCylinder(
        const RotMat_t &rot_mat,
        const Vector3d_t &pos,
        const double height,
        const double radius
        )
{
    GLUquadricObj *pGluQuadric = gluNewQuadric();
    //gluQuadricNormals(pGluQuadric, GLU_SMOOTH);
    glPushMatrix();

    //gluQuadricOrientation(pGluQuadric, GLU_OUTSIDE);
    static const int s_lSubdivisions = 18;
 
    double aeHomoXfmMat[glmat4::Size] = {0.0};
    math::ToGlMatrix4(rot_mat, pos, aeHomoXfmMat);
    glMultMatrixd(aeHomoXfmMat);
    glTranslated(0.0, 0.0, -0.5 * height);
    gluCylinder(
            pGluQuadric,     // quad, specifies the quadrics object
            radius,       // base, specifies the radius of the cylinder at z = 0
            radius,       // top, specifies the radius of the cylinder at z = height
            height,       // height, specifies the height of the cylinder
            s_lSubdivisions, // slices, specifies the number of subdivisions around the z axis
            1                // stacks, specifies the number of subdivisions along the z axis
            );

    glPopMatrix();
}

void DrawArrow(const geom::Arrow &arrow)
{
    static const double s_eEpsilon = 1.0e-4;
    Vector3d_t dirVec = arrow.end_pos() - arrow.start_pos();
    if (dirVec.norm() < s_eEpsilon)
    {
        return;
    }

    GLUquadricObj *pGluQuadric = gluNewQuadric();
    Vector3d_t unitDirVec = dirVec.normalized();
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
    //RotMat_t rotMatFromAxisZ = math::relativeRotMat(
    //        unitDirVec,
    //        math::UnitZ()
    //        );
    RotMat_t rotMatFromAxisZ = math::ComputeRelativeRotMat(
            math::UnitZ(),
            unitDirVec
            );
    double aeToStartPosHomoXfm[glmat4::Size] = {0.0};
    math::ToGlMatrix4(
            rotMatFromAxisZ,
            arrow.start_pos(),
            aeToStartPosHomoXfm
            );
    glMultMatrixd(aeToStartPosHomoXfm);
    gluCylinder(
            pGluQuadric,
            arrow.cylinder_radius(),
            arrow.cylinder_radius(),
            arrow.cylinder_height(),
            geom::Arrow::DefaultSlices(),
            geom::Arrow::DefaultStacks()
            );
    glTranslated(0.0, 0.0, arrow.cylinder_height());
    gluCylinder(
            pGluQuadric,
            arrow.cone_radius(),
            0.0,
            arrow.cone_height(),
            geom::Arrow::DefaultSlices(),
            geom::Arrow::DefaultStacks()
            );

    //RotMat_t negZRotMat = math::relativeRotMat(
    //        -math::UnitZ(),
    //        math::UnitZ()
    //        );
    RotMat_t negZRotMat = math::ComputeRelativeRotMat(
            math::UnitZ(),
            -math::UnitZ()
            );
    double aeDiskNormalHomoXfm[glmat4::Size] = {0.0};
    math::ToGlMatrix4(
            negZRotMat,
            Vector3d_t::Zero(),
            aeDiskNormalHomoXfm
            );
    glMultMatrixd(aeDiskNormalHomoXfm);
    gluDisk(
            pGluQuadric,                  // qobj, specifies the quadrics object
            0.0,                          // innerRadius, specifies the inner radius (may be 0)
            arrow.cone_radius(),       // outerRadius, specifies the outer radius of the disk
            geom::Arrow::DefaultSlices(), // slices, specifies the number of subdivisions around the z-axis
            geom::Arrow::DefaultRings()   // loops, specifies the number of concentric rings about the origin into which the disk is subdivided
            );
    glTranslated(0.0, 0.0, arrow.cylinder_height());
    gluDisk(
            pGluQuadric,
            0.0,
            arrow.cylinder_radius(),
            geom::Arrow::DefaultSlices(),
            geom::Arrow::DefaultRings()
            );

    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
}

void DrawSphere(const Vector3d_t &ball_pos, double radius)
{
    glPushMatrix();
    glTranslated(ball_pos.x(), ball_pos.y(), ball_pos.z());
    GLUquadricObj *qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, (GLenum)GLU_FILL);
    gluQuadricNormals(qobj, (GLenum)GLU_SMOOTH);
    glScaled(.3, .3, .3);
    gluSphere(qobj, radius, 25, 25);
    glPopMatrix();
}

void DrawTriad()
{
    static const double s_eCylinderHeight = 0.7;
    static const double s_eCylinderRadius = 0.02;
    static const double s_eConeHeight = 0.3;
    static const double s_eConeRadius = 0.04;

    // draw x axis in red, y axis in green, z axis in blue
    glColor3d(1., .2, .2);
    geom::Arrow arrowX(Vector3d_t::Zero(), Vector3d_t(1.0, 0.0, 0.0));
    arrowX.set_cylinder_height(s_eCylinderHeight);
    arrowX.set_cylinder_radius(s_eCylinderRadius);
    arrowX.set_cone_height(s_eConeHeight);
    arrowX.set_cone_radius(s_eConeRadius);
    render::DrawArrow(arrowX);

    glColor3d(.2, 1., .2);
    geom::Arrow arrowY(Vector3d_t::Zero(), Vector3d_t(0.0, 1.0, 0.0));
    arrowY.set_cylinder_height(s_eCylinderHeight);
    arrowY.set_cylinder_radius(s_eCylinderRadius);
    arrowY.set_cone_height(s_eConeHeight);
    arrowY.set_cone_radius(s_eConeRadius);
    render::DrawArrow(arrowY);

    glColor3d(.2, .2, 1.);
    geom::Arrow arrowZ(Vector3d_t::Zero(), Vector3d_t(0.0, 0.0, 1.0));
    arrowZ.set_cylinder_height(s_eCylinderHeight);
    arrowZ.set_cylinder_radius(s_eCylinderRadius);
    arrowZ.set_cone_height(s_eConeHeight);
    arrowZ.set_cone_radius(s_eConeRadius);
    render::DrawArrow(arrowZ);

    //glEnd();
}

void DrawGround()
{
    float i = 0.0, j = 0.0;
    int count = 0;
    
    GLfloat white4[] = {.4f, .4f, .4f, 1.f};
    //GLfloat white1[] = {.1, .1, .1, 1.};
    GLfloat green5[] = {0.f, .5f, 0.f, 1.f};
    //GLfloat green2[] = {0., .2, 0., 1.};
    GLfloat black[] = {0.f, 0.f, 0.f, 1.f};
    //GLfloat mat_shininess[] = {7.};		/* Phong exponent */

    glBegin(GL_QUADS);

    for(i=-15.;i<=15.;i+=1)
    {
        for(j=-15.;j<=15.;j+=1)
        {
            if((count%2) == 0)
            {
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white4);
                //			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white1);
                //			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
                glColor3d(.6, .6, .6);
            }
            else
            {
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, black);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green5);
                //			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, green2);
                //			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
                glColor3d(.9, .9, .9);
            }

            glNormal3d(0.,0.,1.);

            glVertex3d(j,  0, i);
            glVertex3d(j,  0, i+1);
            glVertex3d(j+1,0, i+1);
            glVertex3d(j+1,0, i);
            count++;
        }
    }

    glEnd();
}

} // namespace render {
