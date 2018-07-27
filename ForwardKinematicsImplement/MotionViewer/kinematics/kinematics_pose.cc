#include "kinematics_pose.h"
#include "math_utils.h"

namespace kinematics {

// public func.

Pose::Pose()
    :start_pos_(new Vector3d_t),
    end_pos_(new Vector3d_t),
    rotation_(new RotMat_t)
{
}

Pose::Pose(
        const Vector3d_t &start_pos,
        const Vector3d_t &end_pos,
        const RotMat_t &rotation
        )
    :start_pos_(new Vector3d_t(start_pos)),
    end_pos_(new Vector3d_t(end_pos)),
    rotation_(new RotMat_t(rotation))
{
}

Pose::Pose(const Pose &other)
    :start_pos_(new Vector3d_t(other.start_pos())),
    end_pos_(new Vector3d_t(other.end_pos())),
    rotation_(new RotMat_t(other.rotation()))
{
}

Pose::~Pose()
{
}

Pose &Pose::operator=(const Pose &other)
{
    if (&other == this)
    {
        return *this;
    }

    *start_pos_ = other.start_pos();
    *end_pos_ = other.end_pos();
    *rotation_ = other.rotation();

    return *this;
}

Vector3d_t Pose::start_pos() const
{
    return *start_pos_;
}

Vector3d_t Pose::end_pos() const
{
    return *end_pos_;
}

RotMat_t Pose::rotation() const
{
    return *rotation_;
}

void Pose::set_start_pos(const Vector3d_t &start_pos)
{
    *start_pos_ = start_pos;
}

void Pose::set_end_pos(const Vector3d_t &end_pos)
{
    *end_pos_ = end_pos;
}

void Pose::set_rotation(const RotMat_t &rotation)
{
    *rotation_ = rotation;
}

bool Pose::IsApprox(
        const Pose &other,
        const Vector3d_t::RealScalar &epsilon
        ) const
{
    bool is_approx_start_pos = math::IsApprox(
            *start_pos_,
            other.start_pos(),
            epsilon
            );
    if (!is_approx_start_pos)
    {
        return is_approx_start_pos;
    }

    bool is_approx_end_pos = math::IsApprox(
            *end_pos_,
            other.end_pos(),
            epsilon
            );
    if (!is_approx_end_pos)
    {
        return is_approx_end_pos;
    }

    bool is_approx_rotation = math::IsApprox(
            *rotation_,
            other.rotation(),
            epsilon
            );
    if (!is_approx_rotation)
    {
        return is_approx_rotation;
    }

    return TRUE;
}

// protected func.

// private func.

} // namespace kinematics {
