#ifndef _KINEMATICS_LIMB_IK_H_
#define _KINEMATICS_LIMB_IK_H_

#include "kinematics_type.h"
#include "boost/noncopyable.hpp"
#include "boost/optional.hpp"

namespace acclaim {
class Skeleton;
class Motion;
} // namespace acclaim {

namespace kinematics {

class LimbIk : private boost::noncopyable
{

public:

    LimbIk();
    virtual ~LimbIk();

    void set_skeleton(const acclaim::Skeleton &skeleton);

protected:

private:

    boost::optional<acclaim::Skeleton &> skeleton_;
};

} // namespace kinematics {

#endif // #ifndef _KINEMATICS_LIMB_IK_H_
