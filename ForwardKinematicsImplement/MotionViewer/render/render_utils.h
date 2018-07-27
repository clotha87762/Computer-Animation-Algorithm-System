#ifndef _RENDER_API_H_
#define _RENDER_API_H_

#include "math_utils.h"
#include "render_camera.h"
#include "render_polar_rotation.h"

namespace geom {
class Arrow;
} // namespace geom {

namespace render {

enum
{
    Red = 0,
    Green,
    Blue,
    Yellow,
    Orange,
    TotalColor
};

/**
 * \brief Set color from list
 */
void SetColor(const int color);
/**
 * \brief Set color by directly assigning the RGB value
 */
void SetColor3b(const char red, const char green, const char blue);
void SetColor3ub(const unsigned char red, const unsigned char green, const unsigned char blue);
void SetColor3d(const double red, const double green, const double blue);
void SetColor4d(const double red, const double green, const double blue, const double alpha);

double CameraZoom();
render::PolarRotation CameraRotation();
Vector3d_t CameraTranslation();
Vector3d_t CameraLookAt();

void SetCameraView();
void SetCameraZoom(const double zoom);
/**
 * \brief Use polar coordinates to specify the viewing transformation.
 * Involves orbiting the camera around an object that's cnetered at the origin.
 * \param[in] azimuth   Describes the angle of rotation of the camera about
 * the object in the x-y plane, measured from the positive y-axis.
 * \param[in] elevation The angle of rotation of the camera in the y-z
 * plane, measured from the positive z-axis
 * \param[in] twist     Represents the rotation of the viewing volume around its
 * line of sight
 */
void SetCameraRotation(
        const double azimuth,
        const double elevation,
        const double twist
        );
/**
 * \brief Use polar coordinates to shift the viewing transformation
 * \param[in] azimuth   Increment of the azimuth
 * \param[in] elevation Increment of the elevation
 * \param[in] twist     Increment of the twist
 */
void ShiftCameraRotation(
        const double azimuth,
        const double elevation,
        const double twist
        );
/**
 * \brief
 * \param[in] xlt
 */
void SetCameraTranslation(const Vector3d_t &xlt);
/**
 * \brief
 * \param[in] shift_xlt
 */
void ShiftCameraTranslation(const Vector3d_t &shift_xlt);
/**
 * \brief
 * \param[in] pos
 */
void SetCameraLookAt(const Vector3d_t &pos);
/**
 * \brief
 * \param[in] shift_pos
 */
void ShiftCameraLookAt(const Vector3d_t &shift_pos);
/**
 * \brief Set translation and rotation of drawn objects
 * \param[in] xlt
 */
void SetTranslate(const Vector3d_t &xlt);
/**
 * \brief Set drawing rotation of drawn objects using angle-axis representation
 * \param[in] radian
 * \param[in] rotation_axis
 */
void SetRotation(const double radian, const Vector3d_t &rotation_axis);
/**
 * \brief Draw a thin line
 * \param[in] start_pt
 * \param[in] end_point
 */
void DrawLine(const Vector3d_t &start_pt, const Vector3d_t &end_point);
/**
 * \brief Draw a cylinder
 * \param[in] start_pt
 * \param[in] end_point
 * \param[in] radius
 */
void DrawCylinder(
        const Vector3d_t &start_pt, 
        const Vector3d_t &end_point, 
        double radius
        );
/**
 * \brief Draw a cylinder
 */
void DrawCylinder(
        const RotMat_t &rot_mat,
        const Vector3d_t &pos,
        const double height,
        const double radius
        );
/**
 * \brief
 * \param[in]
 */
void DrawArrow(const geom::Arrow &arrow);
/**
 * \brief Draw a sphere
 * \param[in] ball_pos
 * \param[in] radius
 */
void DrawSphere(const Vector3d_t &ball_pos, double radius);
/**
 * \brief Draw a triad in the world origin
 */
void DrawTriad();
/**
 * \brief Draw checker board ground plane
 */
void DrawGround();

} // namespace render {

#endif // #ifndef _RENDER_API_H_
