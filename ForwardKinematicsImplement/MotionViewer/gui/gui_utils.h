#ifndef _GUI_UTILS_H_
#define _GUI_UTILS_H_

#include "math_type.h"

namespace gui {
class Marker;
}

namespace gui {

/**
 * \brief
 * \param[in]
 * \return
 */
Vector3d_t EvalCursorPos(const ScreenPos_t &mouse_pos);
/**
 * \brief
 * \param[in]
 * \param[out]
 */
void SelectColor(
        const ScreenPos_t &mouse_pos,
        Marker *marker
        );
/**
 * \brief
 * \param[in]
 * \param[in]
 * \param[in]
 * \param[out]
 */
void DragMarker(
        const ScreenPos_t &mouse_pos,
        const ScreenPos_t &previous_pos,
        const int mode,
        Marker *marker
        );
/**
 * \brief
 */
void TimeoutCallback(void *a_pGlWindow);

} // namespace gui {

#endif // #ifndef _GUI_UTILS_H_
