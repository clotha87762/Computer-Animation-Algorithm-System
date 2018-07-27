#include "kinematics_numerical_ik.h"
#include "console_log.h"
#include "param_holder.h"
#include "math_utils.h"
#include "acclaim_skeleton.h"
#include "acclaim_motion.h"
#include "kinematics_pose.h"
#include "math_utils.h"
#include <cmath>

using namespace Eigen;



namespace kinematics
{

	NumericalIk::NumericalIk()
		:skeleton_(boost::none),
		fk_solver_(new Forward)
	{
		
	}

	NumericalIk::~NumericalIk()
	{
	}

	void NumericalIk::set_skeleton(acclaim::Skeleton &skeleton)
	{
		skeleton_ = skeleton;
		fk_solver_->set_skeleton(skeleton_.get());
		fk_solver_->ConstructArticPath();
	}

	Vector3d_t Cross(Vector3d_t v1, Vector3d_t v2){

		return Vector3d_t(v1.y()*v2.z() - v1.z()*v2.y(), v1.z()*v2.x() - v1.x()*v2.z(), v1.x()*v2.y() - v1.y()*v2.x());
	}

	void NumericalIk::ComputeNumericalIK(
		const Vector3d_t &target_pos,
		const int start_bone_idx,
		const int end_bone_idx,
		const int frame_idx,
		acclaim::Motion *ik_motion
		)
	{
		// TO DO
		// sample code for computing fullbody pose

		int boneIdx = end_bone_idx;
		int count = 0;
		Vector3d_t xAxis = Vector3d_t(1, 0, 0);
		Vector3d_t yAxis = Vector3d_t(0, 1, 0);
		Vector3d_t zAxis = Vector3d_t(0, 0, 1);
		Vector3dColl_t V;
		Vector3dColl_t J;
		double len;
		double deltalen;
		int iterate = 0;
		double totallen=0, totaldistance=0;
		Vector3d disVec;
		do{
			fk_solver_->set_motion(*ik_motion);
			PoseColl_t bone_pose_coll = fk_solver_->ComputeSkeletonPose(frame_idx);
			Vector6dColl_t oriMotion = (*ik_motion).JointSpatialPos(frame_idx);
			disVec = target_pos - bone_pose_coll[start_bone_idx].start_pos();
			totaldistance = sqrt(disVec.x()*disVec.x() + disVec.y()*disVec.y() + disVec.z()*disVec.z());
			while (1){
				const acclaim::Bone* tempBone = (*skeleton_).BonePtr(boneIdx);
				Vector3d_t tempJointPos = bone_pose_coll[boneIdx].start_pos();
				Vector3d_t distance = target_pos - tempJointPos;
				Vector3d_t tempAxis;
				
				totallen += tempBone->length;
				if (tempBone->dofx){
					//V.push_back(Vector3d_t(1, 0, 0));
					tempAxis = bone_pose_coll[boneIdx].rotation() * xAxis;
					J.push_back(Cross(tempAxis, distance));
				}
				if (tempBone->dofy){
					//V.push_back(Vector3d_t(0, 1, 0));
					tempAxis = bone_pose_coll[boneIdx].rotation() * yAxis;
					J.push_back(Cross(tempAxis, distance));
				}
				if (tempBone->dofz){
					//V.push_back(Vector3d_t(0, 0, 1));
					tempAxis = bone_pose_coll[boneIdx].rotation() * zAxis;
					J.push_back(Cross(tempAxis, distance));
				}
				if (boneIdx == start_bone_idx || count > 1000)
					break;
				boneIdx = tempBone->parent->idx;

			}
			if (totallen < totaldistance)
				break;

			MatrixXd jacobian(3, J.size());
			Eigen::VectorXd theta(J.size());
			for (int i = 0; i < J.size(); i++){
				jacobian(0, i) = J[i].x();
				jacobian(1, i) = J[i].y();
				jacobian(2, i) = J[i].z();
			}
			Eigen::JacobiSVD<Eigen::MatrixXd> jacobiansvd(
				jacobian,
				Eigen::ComputeThinU | Eigen::ComputeThinV
				);
			Vector3d_t deltaPos = target_pos - bone_pose_coll[end_bone_idx].end_pos();
			theta = jacobiansvd.solve(deltaPos);
			deltalen = sqrt(pow(deltaPos.x(), 2) + pow(deltaPos.y(), 2) + pow(deltaPos.z(), 2));
			Vector6dColl_t tempMotion = (*ik_motion).JointSpatialPos(frame_idx);



			double scale = param::ConfigValue<double>("ik_step");

			Vector3d_t end1 = bone_pose_coll[end_bone_idx].end_pos();
			//	double endlen1 = sqrt(end1.x()*end1.x() + end1.y()*end1.y() + end1.z()*end1.z());


			boneIdx = end_bone_idx;
			count = 0;
			int x;
			if (isRealTime==TRUE)
				x = 1;
			else
				x = 5;

			while (1){
				const acclaim::Bone* tempBone = (*skeleton_).BonePtr(boneIdx);
				Vector3d_t tempVec = (tempMotion[boneIdx].AngularVector());
				if (tempBone->dofx){
					tempVec.SetX(tempVec.x() + x*scale * theta(count++));
				}
				if (tempBone->dofy){
					tempVec.SetY(tempVec.y() + x*scale* theta(count++));
				}
				if (tempBone->dofz){
					tempVec.SetZ(tempVec.z() + x*scale * theta(count++));
				}
				tempMotion[boneIdx].SetAngularVector(tempVec);   // 啊啊啊啊啊單位問題

				if (boneIdx == start_bone_idx)
					break;
				boneIdx = tempBone->parent->idx;
			}

			(*ik_motion).SetJointSpatialPos(frame_idx, tempMotion);
			fk_solver_->set_motion(*ik_motion);
			bone_pose_coll = fk_solver_->ComputeSkeletonPose(frame_idx);
		
			if (isRealTime == TRUE){

				Vector3d_t end2 = bone_pose_coll[end_bone_idx].end_pos();
				Vector3d_t end3 = end2 - end1;
				double endlen = sqrt(end3.x()*end3.x() + end3.y()*end3.y() + end3.z()*end3.z());
				deltaPos = target_pos - bone_pose_coll[end_bone_idx].end_pos();
				deltalen = sqrt(pow(deltaPos.x(), 2) + pow(deltaPos.y(), 2) + pow(deltaPos.z(), 2));


				//(*ik_motion).SetJointSpatialPos(frame_idx, oriMotion);

				double ratio = (deltalen) / (endlen);
				boneIdx = end_bone_idx;
				count = 0;
				tempMotion = (*ik_motion).JointSpatialPos(frame_idx);
				while (1){
					const acclaim::Bone* tempBone = (*skeleton_).BonePtr(boneIdx);
					Vector3d_t tempVec = (tempMotion[boneIdx].AngularVector());
					if (tempBone->dofx){
						tempVec.SetX(tempVec.x() + ratio * scale * theta(count++));
					}
					if (tempBone->dofy){
						tempVec.SetY(tempVec.y() + ratio * scale* theta(count++));
					}
					if (tempBone->dofz){
						tempVec.SetZ(tempVec.z() + ratio * scale * theta(count++));
					}
					tempMotion[boneIdx].SetAngularVector(tempVec);   // 啊啊啊啊啊單位問題

					if (boneIdx == start_bone_idx)
						break;
					boneIdx = tempBone->parent->idx;

				}

				(*ik_motion).SetJointSpatialPos(frame_idx, tempMotion);

			}

			
			boneIdx = end_bone_idx;
			deltaPos = target_pos - bone_pose_coll[end_bone_idx].end_pos();
			deltalen = sqrt(pow(deltaPos.x(), 2) + pow(deltaPos.y(), 2) + pow(deltaPos.z(), 2));
			iterate++;

		}while (iterate<1);
			
		}
		
	
}

