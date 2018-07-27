#include "gui_signal.h"
#include <utility>
#include <boost/assign/ptr_map_inserter.hpp>
#include <boost/assign/list_of.hpp> 
#include "FL/Fl_Widget.H"

namespace gui {

// public func.

Signal::~Signal()
{
}

void Signal::EnrollWidget(const ID_t &id)
{
    Signal::instance().widget_signal_map_->insert(id, new WidgetSignal_t);
}

void Signal::ConnectWidgetSlot(
        const ID_t &id,
        const WidgetSignal_t::slot_type &slot
        )
{
    Signal::instance().widget_signal_map_->at(id).connect(slot);
}

void Signal::Send(Fl_Widget *fl_widget)
{
    std::string name = std::string(static_cast<char *>(fl_widget->user_data()));
    Signal::instance().widget_signal_map_->at(name)(fl_widget);
}

void Signal::DisplayShow()
{
    (*Signal::instance().display_show_signal_)();
}

void Signal::ConnectDisplayShow(const DisplayShowSignal_t::slot_type &slot)
{
    Signal::instance().display_show_signal_->connect(slot);
}

void Signal::MainWindowTimeout()
{
    (*Signal::instance().main_window_timeout_signal_)();
}

void Signal::ConnectMainWindowTimeout(const MainWindowTimeoutSignal_t::slot_type &slot)
{
    Signal::instance().main_window_timeout_signal_->connect(slot);
}

// protected func.

// private func.

Signal::Signal()
    :widget_signal_map_(new WidgetSignalMap_t),
    //gl_redraw_signal_(new GlRedrawSignal_t),
    //timeout_signal_(new TimeoutSignal_t),
    display_show_signal_(new DisplayShowSignal_t),
    main_window_timeout_signal_(new MainWindowTimeoutSignal_t)
{
}

Signal &Signal::instance()
{
    static Signal signal;
    return signal;
}

} // namespace gui {
