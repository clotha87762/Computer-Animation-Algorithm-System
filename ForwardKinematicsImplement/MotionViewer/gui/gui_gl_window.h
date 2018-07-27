#ifndef _GUI_GL_WINDOW_H_
#define _GUI_GL_WINDOW_H_

#include <memory>
#include "boost/noncopyable.hpp"
#include "boost/optional.hpp"
#include "FL/Fl_Gl_Window.H"
#include "math_type.h"
#include "gui_type.h"
#include "gui_screenshot.h"
#include "gui_mouse.h"

namespace acclaim {
class Skeleton;
class Motion;
}

namespace gui {
class Screenshot;
class Display;
class Marker;
}

namespace gui {

class GlWindow : public Fl_Gl_Window, private boost::noncopyable
{

public:

    GlWindow(int x, int y, int w, int h, const char *l = 0);
    /**
     * \brief Destructor
     */
    virtual ~GlWindow();
    /**
     * \brief
     */
    void set_ik_marker(const Marker &ik_marker);
    /**
     * \brief
     */
    void SetSwitch(const std::string &id, const bool is_on);
    /**
     * \brief Save screenshot to PNG
     */
    void SaveScreenshot();
    /**
     * \breif
     */
    void EnableSwitch(const std::string &id);
    /**
     * \brief
     */
    void DisableSwitch(const std::string &id);
    /**
     * \brief
     */
    bool IsSwitchOn(const std::string &id) const;

protected:

    /* This is an overloading of a Fl_Gl_Window call.  It is called
       whenever a window needs refreshing. */
    virtual void draw() override;
    
    /* This is an overloading of a Window call. It is 
       called whenever a event happens inside the space 
       taken up by the Fl_Gl_Window */
    virtual int32_t handle(int32_t event);

private:

    /**
     * \brief
     */
    void Redisplay();
    /**
     * \brief
     */
    void InitGL();
    /**
     * \brief
     */
    void InitLight();

    std::unique_ptr<gui::SwitchMap_t> switch_map_;
    std::unique_ptr<gui::Screenshot> screenshot_;
    std::unique_ptr<Mouse> mouse_;
    boost::optional<Marker &> ik_marker_;
};

} // namespace gui {

#endif // #ifndef _GUI_GL_WINDOW_H_
