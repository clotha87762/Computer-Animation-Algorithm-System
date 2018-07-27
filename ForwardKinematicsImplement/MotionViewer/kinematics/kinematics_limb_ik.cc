#include "kinematics_limb_ik.h"
#include "acclaim_skeleton.h"

namespace kinematics {

// public func.

LimbIk::LimbIk()
{
}

LimbIk::~LimbIk()
{
}

void LimbIk::set_skeleton(const acclaim::Skeleton &skeleton)
{
    skeleton_ = boost::optional<acclaim::Skeleton &>(const_cast<acclaim::Skeleton &>(skeleton));
}

// protected func.

// private func.

} // namespace kinematics {
