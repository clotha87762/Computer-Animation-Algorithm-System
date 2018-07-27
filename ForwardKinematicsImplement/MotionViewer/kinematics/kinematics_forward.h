#ifndef _KINEMATICS_FORWARD_H_
#define _KINEMATICS_FORWARD_H_

#include <memory>
#include "kinematics_type.h"
#include "boost/noncopyable.hpp"
#include "boost/optional.hpp"
#include "math_type.h"

namespace acclaim {
class Skeleton;
class Motion;

} // namespace acclaim {

namespace kinematics {

class Forward : private boost::noncopyable
{

public:

    Forward();
    virtual ~Forward();

    void set_skeleton(const acclaim::Skeleton &skeleton);
    void set_motion(const acclaim::Motion &motion);

    void ConstructArticPath();

    PoseColl_t ComputeSkeletonPose(const int32_t frame_idx);

protected:

private:

    boost::optional<acclaim::Skeleton &> skeleton_;
    boost::optional<acclaim::Motion &> motion_;
    std::unique_ptr<ArticIdxColl_t> artic_path_;
};

} // namespace kinematics {

#endif // #ifndef _KINEMATICS_FORWARD_H_
