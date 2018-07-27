#include "gui_display.h"
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream> 
#include <iomanip>
#include "FL/glut.H"
#include "FL/gl.h"
#include "boost/numeric/conversion/cast.hpp"
#include "console_log.h"
#include "param_holder.h"
#include "acclaim_skeleton.h"
#include "acclaim_motion.h"
#include "math_utils.h"
#include "render_utils.h"
#include "kinematics_forward.h"
#include "kinematics_pose.h"

namespace gui {

Display::Display()
    :spot_joint_idx_(-1),
    skeleton_coll_(new SkeletonColl_t),
    motion_coll_(new MotionColl_t),
    fk_coll_(new FkColl_t),
    skeleton_root_offset_coll_(new Vector6dColl_t)
{
    skeleton_coll_->reserve(Display::SkeletonNumHint());
    motion_coll_->reserve(Display::MotionNumHint());
    fk_coll_->reserve(Display::SkeletonNumHint());
    skeleton_root_offset_coll_->reserve(Display::SkeletonNumHint());
}

Display::~Display()
{
}

int32_t Display::spot_joint_idx() const
{
    return spot_joint_idx_;
}

SkeletonHdl_t Display::SkeletonHdl(const int32_t skeleton_idx)
{
    return skeleton_coll_->at(skeleton_idx);
}

MotionHdl_t Display::MotionHdl(const int32_t motion_idx)
{
    return motion_coll_->at(motion_idx);
}

Vector6d_t Display::SkeletonRootOffset(const int32_t skeleton_idx) const
{
    return skeleton_root_offset_coll_->at(skeleton_idx);
}

int32_t Display::SkeletonNum() const
{
    return boost::numeric_cast<int32_t>(skeleton_coll_->size());
}

int32_t Display::MotionNum() const
{
    return boost::numeric_cast<int32_t>(motion_coll_->size());
}

int32_t Display::FrameNum(const int32_t motion_idx) const
{
    return motion_coll_->at(motion_idx).frame_num();
}

int32_t Display::FrameOffset(const int32_t motion_idx) const
{
    return motion_coll_->at(motion_idx).frame_offset();
}

int32_t Display::OffsetFrameNum(const int32_t motion_idx) const
{
    return this->FrameNum(motion_idx) - this->FrameOffset(motion_idx);
}

Posture Display::GetPosture(const int32_t motion_idx, const int32_t frame_idx)
{
    acclaim::Motion &current_motion = motion_coll_->at(motion_idx);
    return *current_motion.PosturePtr(
            current_motion.PostureIdx(frame_idx)
            );
}

void Display::set_spot_joint_idx(const int32_t spot_joint_idx)
{
    spot_joint_idx_ = spot_joint_idx;
}


void Display::set_frame_idx(const int32_t frame_idx)
{
    frame_idx_ = frame_idx;
}

void Display::SetPosture(const int32_t skeleton_idx, const Posture &posture)
{
    /*
    if (param::ConfigValue<bool>("enable_motion_sys"))
    {
        Posture radian_posture = Posture::toRadianBoneRotation(posture);
    }
    */
}

void Display::SetSkeletonRootOffset(const int32_t skeleton_idx, const Vector6d_t &root_offset)
{
    (*skeleton_root_offset_coll_)[skeleton_idx] = root_offset;
}

void Display::LoadSkeleton(acclaim::Skeleton *skeleton)
{
    skeleton_coll_->push_back(skeleton);
    fk_coll_->push_back(new kinematics::Forward);
    fk_coll_->back().set_skeleton(*skeleton);
    fk_coll_->back().ConstructArticPath();

    skeleton_root_offset_coll_->push_back(Vector6d_t::Zero());
}

void Display::LoadMotion(acclaim::Motion *motion)
{
    motion_coll_->push_back(motion);
}

void Display::Show()
{
    glPushMatrix();

    for (int32_t skeleton_idx = 0; skeleton_idx < this->SkeletonNum(); ++skeleton_idx)
    {
        render::SetTranslate(skeleton_root_offset_coll_->at(skeleton_idx).LinearVector() * acclaim::MoCapScale());
        render::SetRotation(skeleton_root_offset_coll_->at(skeleton_idx).AngularVector().x(), math::UnitX());
        render::SetRotation(skeleton_root_offset_coll_->at(skeleton_idx).AngularVector().y(), math::UnitY());
        render::SetRotation(skeleton_root_offset_coll_->at(skeleton_idx).AngularVector().z(), math::UnitZ());

        this->ShowSkeleton(skeleton_idx);
    } // for (int32_t skeleton_idx = 0; skeleton_idx < this->actorNum(); ++skeleton_idx)

    glPopMatrix();
}

void Display::Save()
{
    motion_coll_->at(0).WriteAmcFile("save.amc", acclaim::MoCapScale());
}

void Display::Reset()
{
    skeleton_coll_->clear();
    motion_coll_->clear();
    skeleton_root_offset_coll_->clear();
    fk_coll_->clear();
    skeleton_coll_->reserve(Display::SkeletonNumHint());
    motion_coll_->reserve(Display::MotionNumHint());
    skeleton_root_offset_coll_->reserve(Display::SkeletonNumHint());
}

void Display::SolveIK(const Vector3d_t &ik_target)
{
}

void Display::Slerp()
{
}

void Display::CleanFootskate()
{
}

bool Display::IsEmptyMotion(const int32_t motion_idx) const
{
    return motion_idx < boost::numeric_cast<int32_t>(motion_coll_->size()) ? FALSE : TRUE;
}

// private func.

void Display::ShowSkeleton(const int32_t skeleton_idx)
{
    int32_t fk_frame_idx = 0;
    kinematics::PoseColl_t fk_pose_coll;

    if (skeleton_idx == this->MotionNum()) // the actor has no corresponding AMC file -> draw T-pose
    {
        //fk_frame_idx = 0;
        acclaim::Motion default_motion(
                &(*skeleton_coll_)[skeleton_idx],
                fk_frame_idx + 1
                );
        fk_coll_->at(skeleton_idx).set_motion(default_motion);
        fk_pose_coll = fk_coll_->at(skeleton_idx).ComputeSkeletonPose(fk_frame_idx);
    }
    else
    {
        const int32_t max_frame_idx = motion_coll_->at(skeleton_idx).frame_num() - 1;
        if (frame_idx_ > max_frame_idx)
        {
            //current frameIdx > maxFrame of the motion file
            fk_frame_idx = max_frame_idx;
        }
        else
        {
            fk_frame_idx = frame_idx_;
        }
        fk_coll_->at(skeleton_idx).set_motion(motion_coll_->at(skeleton_idx));
        fk_pose_coll = fk_coll_->at(skeleton_idx).ComputeSkeletonPose(fk_frame_idx);
    }

        //bool is_approx_rotation = math::IsApprox(
        //        ref_pose->coord(),
        //        trg_pose->rotation(),
        //        kEpsilon
        //        );
        //if (!is_approx_rotation)
        //{
        //    LOGERR << std::setprecision(5) << std::fixed
        //        << "bone name: " << skeleton_coll_->at(skeleton_idx).BoneName(bone_idx) << '\n'
        //        << "ref: " << '\n'
        //        << ref_pose->coord() << '\n'
        //        << "trg: " << '\n'
        //        << trg_pose->rotation() << '\n'
        //        << std::endl;
        //    //assert(FALSE);
        //    std::cin.get();
        //}

    //draw all bones
    Vector3d_t bone_start_pos, bone_end_pos;
    for (int32_t bone_idx = 0; bone_idx < skeleton_coll_->at(skeleton_idx).bone_num(); ++bone_idx)
    {
        bone_start_pos = fk_pose_coll[bone_idx].start_pos();
        bone_end_pos = fk_pose_coll[bone_idx].end_pos();
        
        //draw bone 
        render::SetColor((int32_t)(render::Yellow + skeleton_idx) % (int32_t)render::TotalColor);
        double bone_radius = param::ConfigValue<double>("bone_radius");
        render::DrawCylinder(fk_pose_coll[bone_idx].start_pos(), fk_pose_coll[bone_idx].end_pos(), bone_radius);
        Vector3d_t axis_x = fk_pose_coll[bone_idx].rotation().col(math::X_id);
        Vector3d_t axis_y = fk_pose_coll[bone_idx].rotation().col(math::Y_id);
        Vector3d_t axis_z = fk_pose_coll[bone_idx].rotation().col(math::Z_id);		
        
        
        
        double axis_scale = param::ConfigValue<double>("axis_scale");
        if (bone_idx == spot_joint_idx_)
        {
            render::SetColor(render::Red);
            render::DrawLine(bone_start_pos, Vector3d_t(bone_start_pos + axis_scale * axis_x));
            render::SetColor(render::Green);
            render::DrawLine(bone_start_pos, Vector3d_t(bone_start_pos + axis_scale * axis_y));
            render::SetColor(render::Blue);
            render::DrawLine(bone_start_pos, Vector3d_t(bone_start_pos + axis_scale * axis_z));
        }
    } // for bone_idx
}

} // namespace gui {
