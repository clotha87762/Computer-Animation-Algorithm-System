#ifndef _GUI_MAIN_WINDOW_H_
#define _GUI_MAIN_WINDOW_H_

#pragma warning(disable:4800)
#include "global_type.h"
#include <utility>
#include <unordered_map>
#include <functional>
#include "boost/noncopyable.hpp"
#include "boost/ptr_container/ptr_container.hpp"
#include "FL/Fl_Window.H"
#include "FL/Fl_Widget.H"
#include "math_type.h"
#include "gui_type.h"

class Fl_Button;
class Fl_Light_Button;
class Fl_Value_Slider;
class Fl_Value_Input;

namespace acclaim {
class Skeleton;
class Motion;
} // namespace acclaim {

namespace gui {
class Mouse;
class Display;
class GlWindow;
class Marker;
}

namespace gui {

class MainWindow final : public Fl_Window, private boost::noncopyable
{

public:

    /**
     * \brief
     */
    MainWindow(int32_t width, int32_t height, const char *title);
    /**
     * \brief
     */
    virtual ~MainWindow();
    /**
     * \brief
     */
    virtual int32_t handle(int32_t event) override;

protected:

private:

    typedef boost::ptr_map<const std::string, Fl_Button> ButtonMap_t;
    typedef boost::ptr_map<const std::string, Fl_Light_Button> LightButtonMap_t;
    typedef boost::ptr_map<const std::string, Fl_Value_Input> ValueInputMap_t;
    typedef boost::ptr_map<const std::string, Fl_Value_Slider> ValueSliderMap_t;

    typedef boost::function<void (MainWindow *, Fl_Widget *)> SlotFunc_t;
    typedef std::unordered_map<std::string, SlotFunc_t> SlotFuncMap_t;

    /**
     * \brief Initialize the button (Fl_Button) on the main window
     */
    void InitButton();
    /**
     * \brief Initialize the light button (Fl_Light_Button) on the main window
     */
    void InitLightButton();
    /**
     * \brief Initialize the value input (Fl_Value_Input) on the main window
     */
    void InitValueInput();
    /**
     * \brief Initialize the value slider (Fl_Value_Slider) on the main window
     */
    void InitValueSlider();
    /**
     * \brief Initialized the GL window for rendering
     */
    void InitGlWindow();
    /**
     * \brief Enable the corresponding switch specified by id
     */
    void EnableSwitch(const ID_t &id);
    /**
     * \brief Disable the corresponding switch specified by id
     */
    void DisableSwitch(const ID_t &id);
    /**
     * \brief Set the correspdongin switch, specified by id, value to is_on
     */
    void SetSwitch(const ID_t &id, const bool is_on);
    /**
     * \brief Get the value of the switch, specified by id
     */
    bool IsSwitchOn(const ID_t &id);
    /**
     * \brief Callback (slot) function of the "Exit" button (signal)
     */
    void ExitSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "Load motion" button (signal)
     */
    void LoadMotionSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "Load skeleton" button (signal)
     */
    void LoadSkeletonSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "Reset skeleton" button (signal)
     */
    void ResetSkeletonSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "Reset marker" button (signal)
     */
    void ResetMarkerSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "Slerp" button (signal)
     */
    void SlerpSlot(Fl_Widget *widget);

    void CleanFootskateSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "Save" button (signal)
     */
    void SaveSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "@|<" (rewind) button (signal)
     */
    void RewindSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "@||" (pause) button (signal)
     */
    void PauseSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "@>" (play) button (signal)
     */
    void PlaySlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "@<->" (repeat) button (signal)
     */
    void RepeatSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "R" (record) light button (signal)
     */
    void RecordSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "Light" light button (singal)
     */
    void LightSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "Ground" light button (signal)
     */
    void GroundSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "IK marker" light button (signal)
     */
    void IkMarkerSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "IK mode" light button (signal)
     */
    void IkModeSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "frameSlider" Fl_Value_Slider (signal)
     */
    void FrameSlideSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "Frame steps" Fl_Value_Input (signal)
     */
    void FrameStepsSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "dt" Fl_Value_Input (signal)
     */
    void FrameOffsetSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "sub" Fl_Value_Input (signal)
     */
    void IndicateSkeletonSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "tx" Fl_Value_Input (signal)
     */
    void SkeletonTranslationXSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "ty" Fl_Value_Input (signal)
     */
    void SkeletonTranslationYSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "tz" Fl_Value_Input (signal)
     */
    void SkeletonTranslationZSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "rx" Fl_Value_Input (signal)
     */
    void SkeletonRotationXSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "ry" Fl_Value_Input (signal)
     */
    void SkeletonRotationYSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "rz" Fl_Value_Input (signal)
     */
    void SkeletonRotationZSlot(Fl_Widget *widget);
    /**
     * \brief Callback (slot) function of the "Joint" Fl_Value_Input (signal)
     */
    void JointIdxSlot(Fl_Widget *widget);
    /**
     * \brief Callback function when timeout at each forward step
     */
    void TimeoutSlot();
    /**
     * \brief
     */
    Vector6d_t SkeletonRootOffset() const;

    std::unique_ptr<ButtonMap_t> button_map_;
    std::unique_ptr<LightButtonMap_t> light_button_map_;
    std::unique_ptr<ValueInputMap_t> value_input_map_;
    std::unique_ptr<ValueSliderMap_t> value_slider_map_;
    std::unique_ptr<GlWindow> gl_window_;

    std::unique_ptr<Display> display_;

    std::unique_ptr<SlotFuncMap_t> button_slot_map_;
    std::unique_ptr<SlotFuncMap_t> light_button_slot_map_;
    std::unique_ptr<SlotFuncMap_t> value_input_slot_map_;
    std::unique_ptr<SwitchMap_t> switch_map_;
    std::unique_ptr<Mouse> mouse_;

    acclaim::Skeleton *skeleton_;

    std::unique_ptr<Marker> ik_marker_;

    int32_t max_frame_num_;
    int32_t frame_idx_;
    int32_t frame_increment_;
    double time_step_;
};

} // namespace gui {

#endif // #ifndef _GUI_MAIN_WINDOW_H_
