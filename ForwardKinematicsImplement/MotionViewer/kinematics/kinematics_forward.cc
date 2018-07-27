#include "kinematics_forward.h"
#include <algorithm>
#include "math_utils.h"
#include "acclaim_skeleton.h"
#include "acclaim_motion.h"
#include "kinematics_artic_idx.h"
#include "kinematics_pose.h"
#include "acclaim_bone.h"
#include <stack>
#include <iostream>
namespace kinematics {

// public func.

Forward::Forward()
    :skeleton_(boost::none),
    motion_(boost::none),
    artic_path_(new ArticIdxColl_t)
{
}

Forward::~Forward()
{
}

void Forward::set_skeleton(const acclaim::Skeleton &skeleton)
{
    skeleton_ = boost::optional<acclaim::Skeleton &>(const_cast<acclaim::Skeleton &>(skeleton));
}

void Forward::set_motion(const acclaim::Motion &motion)
{
    motion_ = boost::optional<acclaim::Motion &>(const_cast<acclaim::Motion &>(motion));
}

int table[1000];

void Forward::ConstructArticPath()
{
	//struct Bone* ptr;
	/*const acclaim::Bone* ptr;
	for(int32_t i = 0; i < (*skeleton_).bone_num(); i++){
		ptr = (*skeleton_).BonePtr(i);
		ArticIdx artic(int32_t(ptr->idx));
		artic.set_parent_idx(int32_t(ptr->parent->idx));
		(*artic_path_).push_back(artic);
	}
	*/
}


PoseColl_t ComputeChildPose(acclaim::Bone*bone,RotMat_t StartRot,Vector3d_t StartTrans,PoseColl_t skeleton_pose,Vector6dColl_t joint ){
	RotMat_t tempRot;
	Vector3d_t tempTrans;
	int id = bone->idx;
	//std::cout << "id" << id << std::endl;
	tempRot = StartRot* math::ToRotMat(bone->rot_parent_current).transpose()* math::ComputeRotMatXyz(math::ToRadian(joint[id].AngularVector()));
	tempTrans = tempRot * (bone->dir);
	tempTrans = tempTrans * bone->length;
	tempTrans += StartTrans;
	skeleton_pose.at(id).set_start_pos(StartTrans);
	skeleton_pose.at(id).set_end_pos(tempTrans);
	skeleton_pose.at(id).set_rotation(tempRot);
	acclaim::Bone* child;
	child = bone->child;
	while (child != nullptr){
		RotMat_t tempRot2(tempRot);
		Vector3d_t tempTrans2(tempTrans);
		skeleton_pose = ComputeChildPose(child,tempRot2,tempTrans2,skeleton_pose,joint);
		child = child->sibling;
		//std::cout << "id:" << id << " child:" << child->idx << std::endl;
	}
	return skeleton_pose;
}

PoseColl_t Forward::ComputeSkeletonPose(const int32_t frame_idx)
{
    
	//ConstructArticPath();
	PoseColl_t skeleton_pose;
    skeleton_pose.resize(skeleton_->bone_num());
	Vector6dColl_t joint = (*motion_).JointSpatialPos(frame_idx);
	const acclaim::Bone* bone;
	
	/*
	for (int32_t i = 0; i < (*skeleton_).bone_num; i++){
		Vector6d_t t = temp[i];
		Vector3d_t trans = t.LinearVector();
		Vector3d_t rot = t.AngularVector();
		bone = (*skeleton_).BonePtr(i);
		RotMat_t tempRot = math::ToRotMat(bone->rot_parent_current);
		Vector3d_t translation
		bone->rot_parent_current
		//Pose* tempPose = new Pose();
			
	}
	*/
	
	bone = (*skeleton_).root_bone();
	int id = bone->idx;
	
	RotMat_t tempRot = math::ToRotMat(bone->rot_parent_current).transpose()* math::ComputeRotMatXyz(math::ToRadian(joint.at(id).AngularVector()));
	Vector3d_t tempStart = (*skeleton_).root_pos() + joint[id].LinearVector();
	Vector3d_t tempTrans = tempStart + (tempRot*bone->dir) *bone->length;
	skeleton_pose.at(id).set_start_pos(tempStart);
	skeleton_pose.at(id).set_end_pos(tempTrans);
	skeleton_pose.at(id).set_rotation(tempRot);
	
	
	acclaim::Bone* child = bone->child;
	while (child != nullptr){
		skeleton_pose = ComputeChildPose(child, tempRot, tempTrans, skeleton_pose, joint);
		child = child->sibling;
		
		
	}
	
	

	// TO DO..

    return skeleton_pose;
}

// protected func.

// private func.

} // namespace kinematics {
