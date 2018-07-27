#ifndef KINEMATICS_NUMERICAL_IK_H
#define KINEMATICS_NUMERICAL_IK_H

#include <memory>
#include "kinematics_type.h"
#include "boost/noncopyable.hpp"
#include "boost/optional.hpp"
#include "math_type.h"
#include "kinematics_forward.h"

namespace acclaim {
class Skeleton;
class Motion;
} // namespace acclaim {

namespace kinematics
{

class NumericalIk
{
public:
    NumericalIk();
    ~NumericalIk();
    
	bool isRealTime = FALSE;
	

    void set_skeleton(acclaim::Skeleton &skeleton);
    
    void ComputeNumericalIK(
        const Vector3d_t &target_pos,
        const int start_bone_idx,
        const int end_bone_idx,
        const int frame_idx,
        acclaim::Motion *ik_motion
        );
    
    
private:

    boost::optional<acclaim::Skeleton &> skeleton_;
    std::unique_ptr<Forward> fk_solver_;
	
};

}

#endif // KINEMATICS_NUMERICAL_IK_H
