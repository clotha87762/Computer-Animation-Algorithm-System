#ifndef _KINEMATICS_TYPE_H_
#define _KINEMATICS_TYPE_H_

#include "global_type.h"
#include <vector>

namespace kinematics {

class ArticIdx;
class Pose;
typedef std::vector<ArticIdx> ArticIdxColl_t;
typedef std::vector<Pose> PoseColl_t;

} // namespace kinematics {

#endif // #ifndef _KINEMATICS_TYPE_H_
