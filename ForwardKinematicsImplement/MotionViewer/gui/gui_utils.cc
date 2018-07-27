#include "gui_utils.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "boost/numeric/conversion/cast.hpp"
#include "FL/Enumerations.H"
#include "FL/Fl.H"
#include "param_holder.h"
#include "gui_marker.h"
#include "gui_signal.h"
#include "gui_gl_window.h"
#include "gl_utils.h"

namespace gui {

Vector3d_t EvalCursorPos(const ScreenPos_t &mouse_pos)
{
    GLdouble model_view_mat[glmat4::Size] = {0.0};
    GLdouble projection_mat[glmat4::Size] = {0.0};
    GLint viewport[4] = {0};
    glGetDoublev(GL_MODELVIEW_MATRIX, model_view_mat);
    glGetDoublev(GL_PROJECTION_MATRIX, projection_mat);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLfloat win_x = boost::numeric_cast<GLfloat>(mouse_pos.x());
    GLfloat win_y = boost::numeric_cast<GLfloat>(viewport[3]) - boost::numeric_cast<GLfloat>(mouse_pos.y());
    GLfloat win_z = GLfloat{0.0};
    glReadPixels(
            mouse_pos.x(),               // window coordinate x
            viewport[3] - mouse_pos.y(), // window coordinate y
            1,                           // width
            1,                           // height
            GL_DEPTH_COMPONENT,          // format
            GL_FLOAT,                    // type
            &win_z                       // data, the pixel data
            );

    //get world position of cursor
    GLdouble pos_x = GLdouble{0.0};
    GLdouble pos_y = GLdouble{0.0};
    GLdouble pos_z = GLdouble{0.0};
    gluUnProject(
            win_x,
            win_y,
            0.98,
            model_view_mat,
            projection_mat,
            viewport,
            &pos_x,
            &pos_y,
            &pos_z
            );

    return Vector3d_t(pos_x, pos_y, pos_z);
}

void SelectColor(
        const ScreenPos_t &mouse_pos,
        Marker *marker
        )
{
    GLint viewport[4] = {0};
    glGetIntegerv(GL_VIEWPORT, viewport);

    const ScreenPos_t screen_pos(
            mouse_pos.x(),
            viewport[3] - mouse_pos.y()
            );

    unsigned char pixel[3] = {0};
    glReadPixels(
            screen_pos.x(),
            screen_pos.y(),
            1,
            1,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            pixel
            );

    std::vector<unsigned char> marker_color = marker->color_id();
    if (pixel[0] == marker_color[0] && pixel[1] == marker_color[1] && pixel[2] == marker_color[2])
    {
        marker->Select();
    }
}

void DragMarker(
        const ScreenPos_t &mouse_pos,
        const ScreenPos_t &previous_pos,
        const int mode,
        Marker *marker
        )
{
    static const Vector3d_t::RealScalar kZero = Vector3d_t::RealScalar{0.0};
    const Vector3d_t cur_cursor_pos = EvalCursorPos(mouse_pos);
    const Vector3d_t previous_cursor_pos = EvalCursorPos(previous_pos);
    const Vector3d_t diff_pos = cur_cursor_pos - previous_cursor_pos;

    if (FL_LEFT_MOUSE == mode)
    {
        marker->Move(Vector3d_t(diff_pos.x(), diff_pos.y(), kZero));
        //marker->move_x(diff_pos.x());
        //marker->move_y(diff_pos.y());
    }
    else
    {
        marker->Move(Vector3d_t(diff_pos.x(), kZero, diff_pos.y()));
        //marker->move_x(diff_pos.x());
        //marker->move_z(diff_pos.y());
    }
}

void TimeoutCallback(void *gl_window)
{
    GlWindow *window_handle = (GlWindow *)gl_window;
    gui::Signal::MainWindowTimeout();
    window_handle->redraw();

    //if (param::ConfigValue<bool>("enable_footskate_cleaner"))
    //{
    //    Fl::add_timeout(
    //            param::ConfigValue<double>("footskate_clean_time_step"),
    //            gui::TimeoutCallback,
    //            gl_window
    //            );
    //}
    //else
    //{
    //    Fl::add_timeout(
    //            param::ConfigValue<double>("time_step"),
    //            gui::TimeoutCallback,
    //            gl_window
    //            );
    //}
    Fl::add_timeout(
            param::ConfigValue<double>("time_step"),
            gui::TimeoutCallback,
            gl_window
            );
}

} // namespace gui {
