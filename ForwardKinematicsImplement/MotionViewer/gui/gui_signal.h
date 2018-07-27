#ifndef _GUI_SIGNAL_H_
#define _GUI_SIGNAL_H_

#include <memory>
#include <functional>
#include "gui_type.h"
#include "boost/noncopyable.hpp"
#include "boost/ptr_container/ptr_container.hpp"

namespace gui {

class Signal final : private boost::noncopyable
{

public:

    virtual ~Signal();
    /**
     * \brief Enrolled Fl_Widget callback signal specified by id
     * \param[in] id
     */
    static void EnrollWidget(const ID_t &id);
    /**
     * \brief
     * \param[in] id
     * \param[in] slot
     */
    static void ConnectWidgetSlot(
            const ID_t &id,
            const WidgetSignal_t::slot_type &slot
            );
    /**
     * \brief
     * \param[in] fl_widget
     */
    static void Send(Fl_Widget *fl_widget);
    /**
     * \brief
     */
    static void DisplayShow();
    /**
     * \brief
     */
    static void ConnectDisplayShow(const DisplayShowSignal_t::slot_type &slot);
    /**
     * \brief
     */
    static void MainWindowTimeout();
    /**
     * \brief
     */
    static void ConnectMainWindowTimeout(const MainWindowTimeoutSignal_t::slot_type &slot);

protected:

private:

    typedef void (*func)(Fl_Widget *);
    typedef boost::ptr_map<const ID_t, WidgetSignal_t> WidgetSignalMap_t;

    Signal();
    static Signal &instance();

    std::unique_ptr<WidgetSignalMap_t> widget_signal_map_;
    std::unique_ptr<DisplayShowSignal_t> display_show_signal_;
    std::unique_ptr<MainWindowTimeoutSignal_t> main_window_timeout_signal_;
};

} // namespace gui {

#endif // #ifndef _GUI_SIGNAL_H_
