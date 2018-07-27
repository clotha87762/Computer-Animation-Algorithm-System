#include "gui_main_window.h"
#include <algorithm>
#include "boost/numeric/conversion/cast.hpp"
#include "boost/bind.hpp"
// define the maximum parameter number in boost/ptr_map_inserter.hpp,
// since parameter number in the constructor Fl_Button exceeds the default value
#define BOOST_ASSIGN_MAX_PARAMS 7
#include <boost/assign/list_of.hpp>
#include <boost/assign/ptr_map_inserter.hpp>
#include "FL/gl.h" 
#include "FL/Fl_Group.H"
#include "FL/Fl_Widget.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Light_Button.H"
#include "FL/Fl_Value_Slider.H"
#include "FL/Fl_Value_Input.H"
#include "FL/Fl_File_Chooser.H" // Allow a file chooser for save.
#include "console_log.h"
#include "param_holder.h"
#include "render_utils.h"
#include "gui_display.h"
#include "acclaim_skeleton.h"
#include "gui_marker.h"
#include "gui_utils.h"
#include "gui_signal.h"
#include "gui_mouse.h"
#include "gui_gl_window.h"

namespace gui {

// public func.

MainWindow::MainWindow(int32_t width, int32_t height, const char *title)
    :Fl_Window(width, height, title),
    button_map_(new ButtonMap_t),
    light_button_map_(new LightButtonMap_t),
    value_input_map_(new ValueInputMap_t),
    value_slider_map_(new ValueSliderMap_t),
    gl_window_(nullptr),
    display_(new Display),
    button_slot_map_(new SlotFuncMap_t),
    light_button_slot_map_(new SlotFuncMap_t),
    value_input_slot_map_(new SlotFuncMap_t),
    switch_map_(new SwitchMap_t),
    mouse_(new Mouse),
    skeleton_(nullptr),
    ik_marker_(new Marker),
    max_frame_num_(0),
    frame_idx_(0),
    frame_increment_(4),
    time_step_(0.0)
{
    this->InitButton();
    this->InitLightButton();
    this->InitValueInput();
    this->InitValueSlider();
    this->InitGlWindow();

    *switch_map_ = SwitchMap_t(
        {
            {"play",    FALSE},
            {"rewind",  FALSE},
            {"repeat",  FALSE},
            {"record",  FALSE},
            {"id_mode", FALSE},
        }
    );

    light_button_map_->at("light").value(gl_window_->IsSwitchOn("light"));
    light_button_map_->at("ground").value(gl_window_->IsSwitchOn("ground"));
    light_button_map_->at("ik_marker").value(gl_window_->IsSwitchOn("ik_marker"));
    light_button_map_->at("id_mode").value(this->IsSwitchOn("id_mode"));
    light_button_map_->at("record").value(this->IsSwitchOn("record"));

    //if (param::ConfigValue<bool>("enable_footskate_cleaner"))
    //{
    //    time_step_ = param::ConfigValue<double>("footskate_clean_time_step");
    //}
    //else
    //{
    //    time_step_ = param::ConfigValue<double>("time_step");
    //}
    time_step_ = param::ConfigValue<double>("time_step");

    frame_increment_ = param::ConfigValue<int32_t>("frame_increment");

    ik_marker_->set_init_pos(
            Vector3d_t(
                param::ConfigValue<double>("ik_target_x"),
                param::ConfigValue<double>("ik_target_y"),
                param::ConfigValue<double>("ik_target_z")
                )
            );
    ik_marker_->set_target_pos(ik_marker_->init_pos());
    ik_marker_->set_color_id(
            param::ConfigValue<unsigned char>("ik_color_r"),
            param::ConfigValue<unsigned char>("ik_color_g"),
            param::ConfigValue<unsigned char>("ik_color_b")
            );
    gl_window_->set_ik_marker(*ik_marker_);

    Signal::ConnectDisplayShow(
            boost::bind(&Display::Show, display_.get())
            );
    Signal::ConnectMainWindowTimeout(
            boost::bind(&MainWindow::TimeoutSlot, this)
            );
    Fl::add_timeout(time_step_, gui::TimeoutCallback, (void *)gl_window_.get());
}

MainWindow::~MainWindow()
{
}

int32_t MainWindow::handle(int32_t event)
{
    // retval 0 if the event was not used or understood
    // retval 1 if the event was used and can be deleted
    int32_t used_event = 1;

    used_event = Fl_Window::handle(event);
    gl_window_->redraw();

    return used_event;  // Returning one acknowledges that we handled this event
}

// protected func.

// private func.

void MainWindow::InitButton()
{
    boost::assign::ptr_map_insert<Fl_Button>(*button_map_)
        ("load_motion", 315, 540, 120, 30, "Load motion")
        ("load_skeleton", 315, 500, 120, 30, "Load skeleton")
        ("reset_skeleton", 445, 500, 120, 30, "Reset skeleton")
        ("reset_marker", 445, 540, 120, 30, "Reset marker")
        ("clean_footskate", 575, 500, 120, 30, "Clean footskate")
        ("exit", 575, 540, 120, 30, "Exit")
        ("rewind", 115, 525, 35, 25, "@|<")
        ("pause", 155, 525, 35, 25, "@||")
        ("play", 195, 525, 35, 25, "@>")
        ("rewind", 75, 525, 35, 25, "@|<")
        ("repeat", 235, 525, 35, 25, "@<->");
    //("slerp", 535, 500, 100, 30,       "Slerp")
    //("save",   x,   y,   w,  h,        "Save")
    
    *button_slot_map_ = SlotFuncMap_t(
        {
            { "exit", &MainWindow::ExitSlot },
            { "load_motion", &MainWindow::LoadMotionSlot },
            { "load_skeleton", &MainWindow::LoadSkeletonSlot },
            { "reset_skeleton", &MainWindow::ResetSkeletonSlot },
            { "reset_marker", &MainWindow::ResetMarkerSlot },
            { "clean_footskate", &MainWindow::CleanFootskateSlot },
            { "rewind", &MainWindow::RewindSlot },
            { "pause", &MainWindow::PauseSlot },
            { "play", &MainWindow::PlaySlot },
            { "repeat", &MainWindow::RepeatSlot },
            //{"slerp", &MainWindow::slerpSlot},
        }
    );
    
    auto found_rewind = button_map_->find("rewind");
    found_rewind->second->labeltype(FL_SYMBOL_LABEL);
    found_rewind->second->labelsize(12);
    auto found_pause = button_map_->find("pause");
    found_pause->second->labeltype(FL_SYMBOL_LABEL);
    found_pause->second->labelsize(12);
    auto found_paly = button_map_->find("play");
    found_paly->second->labeltype(FL_SYMBOL_LABEL);
    found_paly->second->labelsize(12);
    auto found_repeat = button_map_->find("repeat");
    found_repeat->second->labeltype(FL_SYMBOL_LABEL);
    found_repeat->second->labelsize(12);
    found_repeat->second->align(200 | FL_ALIGN_INSIDE);

    // register the signal-slot
    std::for_each(
            button_map_->begin(),
            button_map_->end(),
            [&](const decltype(*(button_map_->begin())) &id_button)
            {
                id_button.second->user_data(const_cast<char *>(id_button.first.c_str()));
                id_button.second->callback(&Signal::Send);
                auto found_slot = button_slot_map_->find(id_button.first);
                Signal::EnrollWidget(found_slot->first);
                Signal::ConnectWidgetSlot(
                    found_slot->first,
                    boost::bind(found_slot->second, this, _1)
                    );
            }
            );
}

void MainWindow::InitLightButton()
{
    boost::assign::ptr_map_insert<Fl_Light_Button>(*light_button_map_)
        ("record",    70, 525, 40, 25,  "R")
        ("light",     655, 300, 90, 25, "Light")
        ("ground",    655, 330, 90, 25, "Ground")
        ("ik_marker", 655, 370, 90, 25, "IK marker")
        ("id_mode",   655, 400, 90, 25, "IK mode");

    *light_button_slot_map_ = SlotFuncMap_t(
        {
            {"record",    &MainWindow::RecordSlot},
            {"light",     &MainWindow::LightSlot},
            {"ground",    &MainWindow::GroundSlot},
            {"ik_marker", &MainWindow::IkMarkerSlot},
            {"id_mode",   &MainWindow::IkModeSlot},
        }
    );

    std::for_each(
            light_button_map_->begin(),
            light_button_map_->end(),
            [&](const decltype(*(light_button_map_->begin())) &id_button)
            {
                id_button.second->user_data(const_cast<char *>(id_button.first.c_str()));
                id_button.second->callback(&Signal::Send);
                auto found_slot = light_button_slot_map_->find(id_button.first);
                Signal::EnrollWidget(found_slot->first);
                Signal::ConnectWidgetSlot(
                    found_slot->first,
                    boost::bind(found_slot->second, this, _1)
                    );
            }
            );
}

void MainWindow::InitValueInput()
{
    boost::assign::ptr_map_insert<Fl_Value_Input>(*value_input_map_)
        ("frame_steps", 240, 560, 30, 20, "Frame steps")
        ("dt",          680, 660, 45, 25, "dt:")
        ("sub",         680,  20, 45, 25, "sub:")
        ("tx",          680,  95, 45, 25, "tx:")
        ("ty",          680, 125, 45, 25, "ty:")
        ("tz",          680, 155, 45, 25, "tz:")
        ("rx",          680, 190, 45, 25, "rx:")
        ("ry",          680, 220, 45, 25, "ry:")
        ("rz",          680, 250, 45, 25, "rz:")
        ("joint_idx",    65, 560, 30, 20, "Joint");

    *value_input_slot_map_ = SlotFuncMap_t(
        {
            {"frame_steps", &MainWindow::FrameStepsSlot},
            {"dt",          &MainWindow::FrameOffsetSlot},
            {"sub",         &MainWindow::IndicateSkeletonSlot},
            {"tx",          &MainWindow::SkeletonTranslationXSlot},
            {"ty",          &MainWindow::SkeletonTranslationYSlot},
            {"tz",          &MainWindow::SkeletonTranslationZSlot},
            {"rx",          &MainWindow::SkeletonRotationXSlot},
            {"ry",          &MainWindow::SkeletonRotationYSlot},
            {"rz",          &MainWindow::SkeletonRotationZSlot},
            {"joint_idx",   &MainWindow::JointIdxSlot},
        }
    );

    auto found_joint_idx = value_input_map_->find("joint_idx");
    found_joint_idx->second->maximum(30);
    found_joint_idx->second->step(1);
    found_joint_idx->second->value(-1);
    auto found_frame_steps = value_input_map_->find("frame_steps");
    found_frame_steps->second->minimum(1);
    found_frame_steps->second->maximum(30);
    found_frame_steps->second->step(1);
    found_frame_steps->second->value(1);

    std::for_each(
            value_input_map_->begin(),
            value_input_map_->end(),
            [&](const decltype(*value_input_map_->begin()) &id_value_input)
            {
                id_value_input.second->user_data(const_cast<char *>(id_value_input.first.c_str()));
                id_value_input.second->callback(&Signal::Send);
                auto found_slot = value_input_slot_map_->find(id_value_input.first);
                Signal::EnrollWidget(found_slot->first);
                Signal::ConnectWidgetSlot(
                    found_slot->first,
                    boost::bind(found_slot->second, this, _1)
                    );
            }
            );
}

void MainWindow::InitValueSlider()
{
    boost::assign::ptr_map_insert<Fl_Value_Slider>(*value_slider_map_)
        ("frame_slider", 30, 500, 240, 20);
    auto found_frame_slider = value_slider_map_->find("frame_slider");
    found_frame_slider->second->user_data(const_cast<char *>(found_frame_slider->first.c_str()));
    found_frame_slider->second->value(boost::numeric_cast<int32_t>(1));
    found_frame_slider->second->type(5);
    found_frame_slider->second->labelsize(18);
    found_frame_slider->second->minimum(1);
    found_frame_slider->second->maximum(60000);
    found_frame_slider->second->step(1);
    found_frame_slider->second->align(197);
    found_frame_slider->second->callback(&Signal::Send);

    Signal::EnrollWidget("frame_slider");
    Signal::ConnectWidgetSlot(
            "frame_slider",
            boost::bind(&MainWindow::FrameSlideSlot, this, _1)
            );
}

void MainWindow::InitGlWindow()
{
    gl_window_ = std::unique_ptr<GlWindow>(new GlWindow(5, 5, 640, 480, "label"));
    gl_window_->box(FL_DOWN_FRAME);
    gl_window_->labeltype(FL_NO_LABEL);
}

void MainWindow::EnableSwitch(const ID_t &id)
{
    switch_map_->at(id) = TRUE;
}

void MainWindow::DisableSwitch(const ID_t &id)
{
    switch_map_->at(id) = FALSE;
}

void MainWindow::SetSwitch(const ID_t &id, const bool is_on)
{
    switch_map_->at(id) = is_on;
}

bool MainWindow::IsSwitchOn(const ID_t &id)
{
    return switch_map_->at(id);
}

void MainWindow::ExitSlot(Fl_Widget *widget)
{
    exit(1);
}

void MainWindow::LoadMotionSlot(Fl_Widget *widget)
{
    if (gl_window_->IsSwitchOn("skeleton_exit"))
    {
        const char *amc_file_name = fl_file_chooser(
                "Select file name",
                "*.AMC",
                "../acclaim_file"
                );
        if (nullptr == amc_file_name)
        {
            return;
        }

        display_->LoadMotion(
                new acclaim::Motion(
                    std::string(amc_file_name),
                    acclaim::MoCapScale(),
                    skeleton_
                    )
                );
        int32_t motion_idx = display_->MotionNum() - 1;
        if ((display_->FrameNum(motion_idx) - 1) > max_frame_num_)
        {
            max_frame_num_ = display_->FrameNum(motion_idx) - 1;
            value_slider_map_->at("frame_slider").maximum(boost::numeric_cast<double>(max_frame_num_ + 1));
        }

        frame_idx_ = boost::numeric_cast<int32_t>(value_slider_map_->at("frame_slider").value() - 1);
        display_->set_frame_idx(frame_idx_);

        for (int32_t skeleton_idx = 0; skeleton_idx < display_->SkeletonNum(); ++skeleton_idx)
        {
            display_->SetPosture(
                    skeleton_idx,
                    display_->GetPosture(skeleton_idx, frame_idx_)
                    );
        }

        if (display_->SkeletonNum() == display_->MotionNum())
        {
            //display_->SkeletonHdl(display_->SkeletonNum() - 1)->ty -= param::ConfigValue<double>("drift_up");
            const int32_t skeleton_idx = display_->SkeletonNum() - 1;
            Vector6d_t skeleton_root_offset = display_->SkeletonRootOffset(skeleton_idx);
            Vector3d_t linear_offset(
                    Vector3d_t::RealScalar{0.0},
                    -param::ConfigValue<Vector3d_t::RealScalar>("drift_up"),
                    Vector3d_t::RealScalar{0.0}
                    );
            skeleton_root_offset.SetLinearVector(
                    skeleton_root_offset.LinearVector() + linear_offset
                    );
            display_->SetSkeletonRootOffset(skeleton_idx, skeleton_root_offset);
        }

        Fl::flush();
        gl_window_->redraw();
    } // if (gl_window_->IsSwitchOn("skeleton_exit"))
}

void MainWindow::LoadSkeletonSlot(Fl_Widget *widget)
{
    const char *asf_file_name = fl_file_chooser(
            "Select file name",
            "*.ASF",
            "../acclaim_file"
            );
    if (nullptr == asf_file_name)
    {
        return;
    }

    skeleton_ = new acclaim::Skeleton(std::string(asf_file_name), acclaim::MoCapScale());
    skeleton_->SetBasePosture();
    display_->LoadSkeleton(skeleton_);
    gl_window_->EnableSwitch("skeleton_exit");

    const int32_t skeleton_idx = display_->SkeletonNum() - 1;
    Vector6d_t skeleton_root_offset = display_->SkeletonRootOffset(skeleton_idx);
    Vector3d_t linear_offset(
            Vector3d_t::RealScalar{0.0},
            param::ConfigValue<Vector3d_t::RealScalar>("drift_up"),
            Vector3d_t::RealScalar{0.0}
            );
    skeleton_root_offset.SetLinearVector(
            skeleton_root_offset.LinearVector() + linear_offset
            );
    display_->SetSkeletonRootOffset(skeleton_idx, skeleton_root_offset);


    gl_window_->redraw();
}

void MainWindow::ResetSkeletonSlot(Fl_Widget *widget)
{
    this->EnableSwitch("rewind");
    this->DisableSwitch("play");
    this->DisableSwitch("repeat");

    gl_window_->DisableSwitch("skeleton_exit");
    display_->Reset();

    skeleton_ = nullptr;

    max_frame_num_ = 0;
    frame_idx_ = 0;

    gl_window_->redraw();
}

void MainWindow::ResetMarkerSlot(Fl_Widget *widget)
{
    light_button_map_->at("ik_marker").value(boost::numeric_cast<int32_t>(FALSE));
    this->DisableSwitch("id_mode");
    ik_marker_->ResetPos();
}

void MainWindow::SlerpSlot(Fl_Widget *widget)
{
    display_->Slerp();
}

void MainWindow::CleanFootskateSlot(Fl_Widget *widget)
{
    display_->CleanFootskate();
}

void MainWindow::SaveSlot(Fl_Widget *widget)
{
    display_->Save();
}

void MainWindow::RewindSlot(Fl_Widget *widget)
{
    this->PlaySlot(widget);
}

void MainWindow::PauseSlot(Fl_Widget *widget)
{
    this->PlaySlot(widget);
}

void MainWindow::PlaySlot(Fl_Widget *widget)
{
    if (!display_->IsEmptyMotion(display_->MotionNum() - 1)
            && display_->SkeletonNum() == display_->MotionNum())
    {
        if (widget == &(button_map_->at("play")))
        {
            this->EnableSwitch("play");
            this->DisableSwitch("rewind");
        }

        if (widget == &(button_map_->at("pause")))
        {
            this->DisableSwitch("play");
            this->DisableSwitch("repeat");
        }

        if (widget == &(button_map_->at("repeat")))
        {
            this->DisableSwitch("rewind");
            this->EnableSwitch("play");
            this->EnableSwitch("repeat");
        }

        if (widget == &(button_map_->at("rewind")))
        {
            this->EnableSwitch("rewind");
            this->DisableSwitch("play");
            this->DisableSwitch("repeat");
        }
    }
}

void MainWindow::RepeatSlot(Fl_Widget *widget)
{
    this->PlaySlot(widget);
}

void MainWindow::RecordSlot(Fl_Widget *widget)
{
    if (!this->IsSwitchOn("play"))
    {
        bool is_record_light_on = boost::numeric_cast<bool>(light_button_map_->at("record").value());
        if (!this->IsSwitchOn("record") && is_record_light_on)
        {
            this->EnableSwitch("record");
        }

        if (this->IsSwitchOn("record") && !is_record_light_on)
        {
            this->DisableSwitch("record");
        }
    }

    light_button_map_->at("record").value(
            boost::numeric_cast<char>(this->IsSwitchOn("record"))
            );
}

void MainWindow::LightSlot(Fl_Widget *widget)
{
    gl_window_->SetSwitch(
            "light",
            boost::numeric_cast<bool>(light_button_map_->at("light").value())
            );
    gl_window_->SetSwitch(
            "ground",
            boost::numeric_cast<bool>(light_button_map_->at("ground").value())
            );
    gl_window_->SetSwitch(
            "ik_marker",
            boost::numeric_cast<bool>(light_button_map_->at("ik_marker").value())
            );
    gl_window_->redraw();
}

void MainWindow::GroundSlot(Fl_Widget *widget)
{
    this->LightSlot(widget);
}

void MainWindow::IkMarkerSlot(Fl_Widget *widget)
{
    this->LightSlot(widget);
}

void MainWindow::IkModeSlot(Fl_Widget *widget)
{
    switch_map_->at("id_mode") = boost::numeric_cast<bool>(light_button_map_->at("id_mode").value());
    bool is_solve_ik = this->IsSwitchOn("id_mode")
        && gl_window_->IsSwitchOn("ik_marker")
        && !display_->IsEmptyMotion(0);

    if (is_solve_ik)
    {
        display_->SolveIK(ik_marker_->target_pos());
    }
}

void MainWindow::FrameSlideSlot(Fl_Widget *widget)
{
    if (gl_window_->IsSwitchOn("skeleton_exit") && !display_->IsEmptyMotion(0))
    {
        if (display_->FrameNum(0) > 0)
        {
            frame_idx_ = boost::numeric_cast<int32_t>(value_slider_map_->at("frame_slider").value() - 1);
            display_->set_frame_idx(frame_idx_);
            for (int32_t skeleton_idx = 0; skeleton_idx < display_->SkeletonNum(); ++skeleton_idx)
            {
                display_->SetPosture(
                        skeleton_idx,
                        display_->GetPosture(skeleton_idx, frame_idx_)
                        );
            }
            Fl::flush();
            gl_window_->redraw();
        }
    }
}

void MainWindow::FrameStepsSlot(Fl_Widget *widget)
{
    display_->set_spot_joint_idx(boost::numeric_cast<int32_t>(value_input_map_->at("joint").value()));
    frame_increment_ = boost::numeric_cast<int32_t>(value_input_map_->at("frame_steps").value());
    gl_window_->redraw();
}

void MainWindow::FrameOffsetSlot(Fl_Widget *widget)
{
    const int32_t skeleton_idx = boost::numeric_cast<int32_t>(button_map_->at("sub").value());
    if (skeleton_idx >= 0 && skeleton_idx < display_->SkeletonNum())
    {
        display_->MotionHdl(skeleton_idx)->set_frame_offset(
                boost::numeric_cast<int32_t>(value_input_map_->at("dt").value())
                );
        LOGMSG << "Shifting subject " << skeleton_idx
            << " by " << value_input_map_->at("dt").value()
            << std::endl;
        int32_t max_offset_frame_num = 0;
        for (int32_t i = 0; i < display_->SkeletonNum(); ++i)
        {
            int32_t offset_frame_num = display_->OffsetFrameNum(i) - 1;
            if (offset_frame_num > max_offset_frame_num)
            {
                max_offset_frame_num = offset_frame_num;
            }
        }
        max_frame_num_ = max_offset_frame_num;
        value_slider_map_->at("frame_slider").maximum(
                boost::numeric_cast<double>(max_frame_num_ + 1)
                );
        display_->SetPosture(
                skeleton_idx,
                display_->GetPosture(skeleton_idx, frame_idx_)
                );
        display_->set_frame_idx(frame_idx_);
    }

    gl_window_->redraw();
}

void MainWindow::IndicateSkeletonSlot(Fl_Widget *widget)
{
    const int32_t skeleton_idx = boost::numeric_cast<int32_t>(value_input_map_->at("sub").value());
    if (skeleton_idx < 0)
    {
        value_input_map_->at("sub").value(0);
    }
    else if (skeleton_idx > display_->SkeletonNum() - 1)
    {
        value_input_map_->at("sub").value(display_->SkeletonNum() - 1);
    }
    else
    {
        //value_input_map_->at("dt").value(display_->motionHdl(skeleton_idx)->offset);
        value_input_map_->at("dt").value(display_->MotionHdl(skeleton_idx)->frame_offset());
        Vector6d_t skeleton_root_offset = display_->SkeletonRootOffset(skeleton_idx);
        value_input_map_->at("tx").value(skeleton_root_offset.LinearVector().x());
        value_input_map_->at("ty").value(skeleton_root_offset.LinearVector().y());
        value_input_map_->at("tz").value(skeleton_root_offset.LinearVector().z());
        value_input_map_->at("rx").value(skeleton_root_offset.AngularVector().x());
        value_input_map_->at("ry").value(skeleton_root_offset.AngularVector().y());
        value_input_map_->at("rz").value(skeleton_root_offset.AngularVector().z());
    }
    gl_window_->redraw();
}

void MainWindow::SkeletonTranslationXSlot(Fl_Widget *widget)
{
    const int32_t skeleton_idx = boost::numeric_cast<int32_t>(value_input_map_->at("sub").value());
    if (skeleton_idx >= 0 && skeleton_idx < display_->SkeletonNum())
    {
        //display_->SkeletonHdl(skeleton_idx)->tx = boost::numeric_cast<int32_t>(value_input_map_->at("tx").value());
        display_->SetSkeletonRootOffset(
                skeleton_idx,
                this->SkeletonRootOffset()
                );
    }
    gl_window_->redraw();
}

void MainWindow::SkeletonTranslationYSlot(Fl_Widget *widget)
{
    const int32_t skeleton_idx = boost::numeric_cast<int32_t>(value_input_map_->at("sub").value());
    if (skeleton_idx >= 0 && skeleton_idx < display_->SkeletonNum())
    {
        //display_->SkeletonHdl(skeleton_idx)->ty = boost::numeric_cast<float>(value_input_map_->at("ty").value());
        display_->SetSkeletonRootOffset(
                skeleton_idx,
                this->SkeletonRootOffset()
                );
    }
    gl_window_->redraw();
}

void MainWindow::SkeletonTranslationZSlot(Fl_Widget *widget)
{
    const int32_t skeleton_idx = boost::numeric_cast<int32_t>(value_input_map_->at("sub").value());
    if (skeleton_idx >= 0 && skeleton_idx < display_->SkeletonNum())
    {
        //display_->SkeletonHdl(skeleton_idx)->tz = boost::numeric_cast<int32_t>(value_input_map_->at("tz").value());
        display_->SetSkeletonRootOffset(
                skeleton_idx,
                this->SkeletonRootOffset()
                );
    }
    gl_window_->redraw();
}

void MainWindow::SkeletonRotationXSlot(Fl_Widget *widget)
{
    const int32_t skeleton_idx = boost::numeric_cast<int32_t>(value_input_map_->at("sub").value());
    if (skeleton_idx >= 0 && skeleton_idx < display_->SkeletonNum())
    {
        //display_->SkeletonHdl(skeleton_idx)->rx = boost::numeric_cast<int32_t>(value_input_map_->at("rx").value());
        display_->SetSkeletonRootOffset(
                skeleton_idx,
                this->SkeletonRootOffset()
                );
    }
    gl_window_->redraw();
}

void MainWindow::SkeletonRotationYSlot(Fl_Widget *widget)
{
    const int32_t skeleton_idx = boost::numeric_cast<int32_t>(value_input_map_->at("sub").value());
    if (skeleton_idx >= 0 && skeleton_idx < display_->SkeletonNum())
    {
        //display_->SkeletonHdl(skeleton_idx)->ry = boost::numeric_cast<int32_t>(value_input_map_->at("ry").value());
        display_->SetSkeletonRootOffset(
                skeleton_idx,
                this->SkeletonRootOffset()
                );
    }
    gl_window_->redraw();
}

void MainWindow::SkeletonRotationZSlot(Fl_Widget *widget)
{
    const int32_t skeleton_idx = boost::numeric_cast<int32_t>(value_input_map_->at("sub").value());
    if (skeleton_idx >= 0 && skeleton_idx < display_->SkeletonNum())
    {
        //display_->SkeletonHdl(skeleton_idx)->rz = boost::numeric_cast<int32_t>(value_input_map_->at("rz").value());
        display_->SetSkeletonRootOffset(
                skeleton_idx,
                this->SkeletonRootOffset()
                );
    }
    gl_window_->redraw();
}

void MainWindow::JointIdxSlot(Fl_Widget *widget)
{
    display_->set_spot_joint_idx(boost::numeric_cast<int32_t>(value_input_map_->at("joint_idx").value()));
    frame_increment_ = boost::numeric_cast<int32_t>(value_input_map_->at("frame_steps").value());
    gl_window_->redraw();
}

void MainWindow::TimeoutSlot()
{
    if (gl_window_->IsSwitchOn("skeleton_exit") && !display_->IsEmptyMotion(0))
    {
        if (this->IsSwitchOn("rewind"))
        {
            frame_idx_ = 0;
            display_->set_frame_idx(frame_idx_);
            this->DisableSwitch("rewind");
            for (int32_t skeleton_idx = 0; skeleton_idx < display_->SkeletonNum(); ++skeleton_idx)
            {
                display_->SetPosture(
                        skeleton_idx,
                        display_->GetPosture(skeleton_idx, frame_idx_)
                        );
            }
        } // if (this->IsSwitchOn("rewind"))

        if (this->IsSwitchOn("play"))
        {
            if (frame_idx_ >= max_frame_num_)
            {
                if (this->IsSwitchOn("repeat"))
                {
                    frame_idx_ = 0;
                }
            }
            else
            {
                frame_idx_ += frame_increment_;
            }

            display_->set_frame_idx(frame_idx_);
            for (int32_t skeleton_idx = 0; skeleton_idx < display_->SkeletonNum(); ++skeleton_idx)
            {
                display_->SetPosture(
                        skeleton_idx,
                        display_->GetPosture(skeleton_idx, frame_idx_)
                        );
            }

            if (this->IsSwitchOn("record"))
            {
                gl_window_->SaveScreenshot();
            }
        } // if (this->IsSwitchOn("play"))

        if (this->IsSwitchOn("id_mode") && ik_marker_->is_select())
        {
            display_->SolveIK(ik_marker_->target_pos());
        }
    }

    value_slider_map_->at("frame_slider").value(boost::numeric_cast<double>(frame_idx_ + 1));
}

Vector6d_t MainWindow::SkeletonRootOffset() const
{
    const Vector3d_t rot_offset(
            boost::numeric_cast<double>(value_input_map_->at("rx").value()),
            boost::numeric_cast<double>(value_input_map_->at("ry").value()),
            boost::numeric_cast<double>(value_input_map_->at("rz").value())
            );
    const Vector3d_t xlt_offset(
            boost::numeric_cast<double>(value_input_map_->at("tx").value()),
            boost::numeric_cast<double>(value_input_map_->at("ty").value()),
            boost::numeric_cast<double>(value_input_map_->at("tz").value())
            );
    Vector6d_t root_offset;
    root_offset.SetAngularVector(rot_offset);
    root_offset.SetLinearVector(xlt_offset);

    return root_offset;
}

} // namespace gui {
