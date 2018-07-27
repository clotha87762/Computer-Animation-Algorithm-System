/*
   display.h

   Display the skeleton, ground plane and other objects.			

   Revision 1 - Steve Lin, Jan. 14, 2002
   Revision 2 - Alla and Kiran, Jan 18, 2002
   */

#ifndef _GUI_DSIPLAY_H_
#define _GUI_DISPLAY_H_

#include <vector>
#include "boost/noncopyable.hpp"
#include "boost/optional.hpp"
#include "boost/ptr_container/ptr_container.hpp"
#include "math_type.h"
#include "acclaim_skeleton.h"
#include "acclaim_motion.h"

typedef boost::optional<acclaim::Skeleton &> SkeletonHdl_t;
typedef boost::optional<acclaim::Motion &> MotionHdl_t;

namespace motion {
class System;
class FootskateCleaner;
} // namespace motion {

namespace kinematics {
class Forward;
} // namespace kinematics {

namespace gui {

class Display : private boost::noncopyable
{

public:

    /**
     * \brief Default constructor
     */
    Display();
    /**
     * \brief Destructor
     */
    virtual ~Display();
    /**
     * \brief
     * \return
     */
    int32_t spot_joint_idx() const;
    /**
     * \brief
     * \param[in] skeleton_idx
     */
    SkeletonHdl_t SkeletonHdl(const int32_t skeleton_idx);
    /**
     * \brief
     * \param[in] motion_idx
     * \return
     */
    MotionHdl_t MotionHdl(const int32_t motion_idx);
    /**
     * \brief
     */
    Vector6d_t SkeletonRootOffset(const int32_t skeleton_idx) const;
    /**
     * \brief Get the number of actors
     * \return
     */
    int32_t SkeletonNum() const;
    /**
     * \brief Get the number of motions
     * \return
     */
    int32_t MotionNum() const;
    /**
     * \brief Get the frame number of the motion file indicated by a_klMotionIdx
     * \param[in] motion_idx
     * \return
     */
    int32_t FrameNum(const int32_t motion_idx) const;
    /**
     * \brief Get the frame offset in the motion file indicated by a_klMotionIdx
     * \param[in] motion_idx
     * \return
     */
    int32_t FrameOffset(const int32_t motion_idx) const;
    /**
     * \brief Get the remaining frame numbers after offset,
     * i.e. frameNum(i) - frameOffset(i), i is index of motion files
     * \param[in] motion_idx
     * \return
     */
    int32_t OffsetFrameNum(const int32_t motion_idx) const;
    /**
     * \brief
     * \param[in] motion_idx
     * \param[in] frame_idx
     */
    Posture GetPosture(const int32_t motion_idx, const int32_t frame_idx);
    /**
     * \brief
     * \param[in] spot_joint_idx
     */
    void set_spot_joint_idx(const int32_t spot_joint_idx);
    /**
     * \brief
     * \param[in] frame_idx
     */
    void set_frame_idx(const int32_t frame_idx);
    /**
     * \brief
     * \param[in] skeleton_idx
     * \param[in] posture
     */
    void SetPosture(const int32_t skeleton_idx, const Posture &posture);
    /**
     * \brief
     */
    void SetSkeletonRootOffset(const int32_t skeleton_idx, const Vector6d_t &root_offset);
    /**
     * \brief Set actor for display
     * \param[in] skeleton
     */
    void LoadSkeleton(acclaim::Skeleton *skeleton);
    /**
     * \brief Set motion for display
     * \param[in] motion
     */
    void LoadMotion(acclaim::Motion *motion);
    /**
     * \brief Display the scene (actor, ground plane ....)
     */
    void Show();
    /**
     * \brief
     */
    void Save();
    /**
     * \brief
     */
    void Reset();
    /**
     * \brief
     * \param[in] ik_target
     */
    void SolveIK(const Vector3d_t &ik_target);
    /**
     * \brief
     */
    void Slerp();
    /**
     * \brief
     */
    void CleanFootskate();
    /**
     * \brief
     * \param[in] motion_idx
     */
    bool IsEmptyMotion(const int32_t motion_idx) const;

private:

    typedef boost::ptr_vector<acclaim::Skeleton> SkeletonColl_t;
    typedef boost::ptr_vector<acclaim::Motion> MotionColl_t;
    typedef boost::ptr_vector<kinematics::Forward> FkColl_t;

    static const int32_t SkeletonNumHint()
    {return int32_t{16};}
    static const int32_t MotionNumHint()
    {return int32_t{16};}
    /**
     * \brief Compute FK and draw the specified character
     * \param[in] skeleton_idx
     */
    void ShowSkeleton(const int32_t skeleton_idx);

    int32_t frame_idx_;             //!< current playing frame
    int32_t spot_joint_idx_;		//!< joint whose local coordinate system is drawn
    std::unique_ptr<SkeletonColl_t> skeleton_coll_;
    std::unique_ptr<MotionColl_t> motion_coll_;
    std::unique_ptr<FkColl_t> fk_coll_;
    std::unique_ptr<Vector6dColl_t> skeleton_root_offset_coll_;
};

} // namespace gui {

#endif // #ifndef _GUI_DSIPLAY_H_
