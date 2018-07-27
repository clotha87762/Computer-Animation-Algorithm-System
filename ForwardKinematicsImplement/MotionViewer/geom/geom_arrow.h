#ifndef _GEOM_ARROW_H_
#define _GEOM_ARROW_H_

#include <memory>
#include "math_type.h"

namespace geom {

class Arrow
{

public:

    static const int DefaultSlices()
    {return int{18};}
    static const int DefaultStacks()
    {return int{1};}
    static const int DefaultRings()
    {return int{2};}

    Arrow();
    explicit Arrow(const Vector3d_t &start_pos, const Vector3d_t &end_pos);
    Arrow(const Arrow &other);
    ~Arrow();

    Arrow &operator=(const Arrow &other);

    Vector3d_t start_pos() const;
    Vector3d_t end_pos() const;
    double cylinder_height() const;
    double cone_height() const;
    double cylinder_radius() const;
    double cone_radius() const;

    void set_start_pos(const Vector3d_t &start_pos);
    void set_end_pos(const Vector3d_t &end_pos);
    void set_cylinder_height(const double cylinder_height);
    void set_cone_height(const double cone_height);
    void set_cylinder_radius(const double cylinder_radius);
    void set_cone_radius(const double cone_radius);

private:

    std::unique_ptr<Vector3d_t> start_pos_;
    std::unique_ptr<Vector3d_t> end_pos_;
    double cylinder_height_;
    double cone_height_;
    double cylinder_radius_;
    double cone_radius_;
};

} // namespace geom {

#endif // #ifndef _GEOM_ARROW_H_
