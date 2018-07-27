#ifndef _GUI_MARKER_H_
#define _GUI_MARKER_H_

#include <vector>
#include "boost/noncopyable.hpp"
#include "math_col_vector_3d.h"
#include "math_type.h"

namespace gui {

class Marker : boost::noncopyable
{

public:

    Marker();
    ~Marker();
    /**
     * \brief
     * \return
     */
    Vector3d_t init_pos() const;
    /**
     * \brief
     * \return
     */
    Vector3d_t target_pos() const;
    /**
     * \brief
     * \return
     */
    std::vector<unsigned char> color_id() const;
    /**
     * \brief
     * \return
     */
    bool is_select() const;

    //set info of object
    void set_color_id(const unsigned char r, const unsigned char g, const unsigned char b);
    void set_init_pos(const Vector3d_t &init_pos);
    void set_target_pos(const Vector3d_t &target_pos);

    //reset object to initial position
    void ResetPos();

    //set condition of object when selected/released
    void Select();
    void Release();
    void Move(const Vector3d_t &shift_vector);

private:

    Vector3d_t target_pos_;
    Vector3d_t init_pos_;
    std::vector<unsigned char> color_id_;
    bool is_select_;
};

} // namespace gui {

#endif // #ifndef _GUI_MARKER_H_
