#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Timer.H>
#include "param_holder.h"
#include "acclaim_skeleton.h"
#include "acclaim_motion.h"
#include "gui_display.h"
#include "gui_signal.h"
#include "gui_main_window.h"

int main(int argc, char **argv)
{
    gui::MainWindow *main_window = new gui::MainWindow(750, 610, "Acclaim file player");
    main_window->show();

    return Fl::run();
}
