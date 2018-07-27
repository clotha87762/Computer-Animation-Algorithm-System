#ifndef _KINEMATICS_POSE_H_
#define _KINEMATICS_POSE_H_

#include "kinematics_type.h"
#include <memory>
#include "math_type.h"

namespace kinematics {

class Pose
{

public:

    Pose();
    explicit Pose(
            const Vector3d_t &start_pos,
            const Vector3d_t &end_pos,
            const RotMat_t &rotation
            );
    Pose(const Pose &other);
    virtual ~Pose();
    
    Pose &operator=(const Pose &other);

    Vector3d_t start_pos() const;
    Vector3d_t end_pos() const;
    RotMat_t rotation() const;

    void set_start_pos(const Vector3d_t &start_pos);
    void set_end_pos(const Vector3d_t &end_pos);
    void set_rotation(const RotMat_t &rotation);

    bool IsApprox(const Pose &other, const Vector3d_t::RealScalar &epsilon) const;

protected:

private:

    std::unique_ptr<Vector3d_t> start_pos_;
    std::unique_ptr<Vector3d_t> end_pos_;
    std::unique_ptr<RotMat_t> rotation_;
};

} // namespace kinematics {

#endif // #ifndef _KINEMATICS_POSE_H_
