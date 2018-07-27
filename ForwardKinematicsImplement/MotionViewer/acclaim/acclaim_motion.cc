#include "acclaim_motion.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <sstream>
#include <fstream>
#ifdef WIN32
#include <FL/gl.h>
#endif
#include "console_log.h"
#include "math_utils.h"
#include "math_spatial_temporal_data.h"
#include "acclaim_skeleton.h"

namespace acclaim {

// a default skeleton that defines each bone's degree of freedom and the order of the data stored in the AMC file
//static Skeleton actor("Skeleton.ASF", MOCAP_SCALE);

Motion::Motion(const Skeleton *skeleton, const int32_t frame_num)
    :frame_num_(frame_num),
    frame_offset_(0),
    skeleton_(const_cast<Skeleton *>(skeleton)),
    posture_coll_(new PostureColl_t(frame_num, Posture(Body::num))),
    st_joint_pos6d_(new SpatialTemporalVector6d_t(skeleton_->bone_num(), frame_num_)),
    amc_header_(new std::string)
{
}

Motion::Motion(const std::string &amc_file_name, const double skeleton_scale, const Skeleton *skeleton)
    :frame_num_(0),
    frame_offset_(0),
    skeleton_(const_cast<Skeleton *>(skeleton)),
    posture_coll_(new PostureColl_t),
    st_joint_pos6d_(new SpatialTemporalVector6d_t),
    amc_header_(new std::string)
{
    this->ReadAmcFile(amc_file_name, skeleton_scale);
}

Motion::~Motion()
{
}

std::string Motion::file_name() const
{
    return file_name_;
}

double Motion::SkeletonScale() const
{
    return skeleton_->scale();
}

std::string Motion::SkeletonFileName() const
{
    return skeleton_->file_name();
}

int32_t Motion::frame_num() const
{
    return frame_num_;
}

int32_t Motion::frame_offset() const
{
    return frame_offset_;
}

int32_t Motion::PostureIdx(const int32_t frame_idx)
{
    int32_t posture_idx = frame_idx +  frame_offset_;

    if (posture_idx < 0)
    {
        return 0;
    }
    else if (posture_idx >= frame_num_)
    {
        return frame_num_ - 1;
    }
    else
    {
        return posture_idx;
    }

    return 0;
}

Posture *Motion::PosturePtr(const int32_t frame_idx) const
{
    if (!posture_coll_->empty())
    {
        return &(*posture_coll_)[frame_idx];
    }
    else
    {
        return nullptr;
    }
}

Vector6dColl_t Motion::JointSpatialPos(const int32_t frame_idx) const
{
    return st_joint_pos6d_->TemporalElements(frame_idx);
}

void Motion::set_frame_offset(const int32_t offset)
{
    frame_offset_ = offset;
}

void Motion::SetPosturesToDefault()
{
    for (int32_t frame_idx = 0; frame_idx < frame_num_; ++frame_idx)
    {
        posture_coll_->at(frame_idx).setRootPos(Vector3d_t::Zero());

        for (int32_t bone_idx = 0; bone_idx < skeleton_->bone_num(); ++bone_idx)
        {
            posture_coll_->at(frame_idx).setBoneRotation(bone_idx, Vector3d_t::Zero());
        }
    }
}

void Motion::SetDefaultJointSpatialPos()
{
    for (int32_t frame_idx = 0; frame_idx < frame_num_; ++frame_idx)
    {
        for (int32_t bone_idx = 0; bone_idx < skeleton_->bone_num(); ++bone_idx)
        {
            st_joint_pos6d_->SetElement(
                    bone_idx,
                    frame_idx,
                    Vector6d_t::Zero()
                    );
        }
    }
}

void Motion::SetPosture(const int32_t frame_idx, const Posture &posture)
{
    posture_coll_->at(frame_idx) = posture;
}

void Motion::SetJointSpatialPos(const int32_t frame_idx, const Vector6dColl_t &pos6d_coll)
{
    st_joint_pos6d_->SetTemporalElements(frame_idx, pos6d_coll);
}

void Motion::SetBoneRotation(
        const int32_t frame_idx,
        const Vector3d_t &degree_angle,
        const int32_t bone_idx
        )
{
    posture_coll_->at(frame_idx).setBoneRotation(bone_idx, degree_angle);
}

void Motion::SetRootPos(
        const int32_t frame_idx,
        const Vector3d_t &pos
        )
{
    posture_coll_->at(frame_idx).setRootPos(pos);
}

bool Motion::ReadAmcFile(const std::string &file_name, const double skeleton_scale)
{
    LOGMSG << "Skeleton scale: " << skeleton_scale << std::endl;

    if (nullptr == skeleton_)
    {
        LOGERR << "No skeleton instance for Motion" << std::endl;
        assert(FALSE);
    }

    std::ifstream input_stream(file_name, std::ios::in);
    if (!input_stream)
    {
        LOGWARN << "Failed to open " << file_name << std::endl;
        assert(FALSE);
        return FALSE;
    }
    else
    {
        input_stream.close();
        input_stream.open(file_name, std::ios::out);
    }

    file_name_ = file_name;

    int32_t frame_count = 0;
    char str[2048];

    // count the number of lines
    while (!input_stream.eof())  
    {
        input_stream.getline(str, 2048);
        if (input_stream.eof())
        {
            break;
        }
        //We do not want to count empty lines
        if (strcmp(str, "") != 0)
        {
            ++frame_count;
        }
    }

    input_stream.close();
    input_stream.clear();

    Bone *bone = skeleton_->root_bone();

    //Compute number of frames. 
    //Subtract 3 to  ignore the header
    //There are (NUM_BONES_IN_ASF_FILE - 2) moving bones and 2 dummy bones (lhipjoint and rhipjoint)
    //const int32_t numbones = numBonesInSkel(bone[0]);
    const int32_t movbones = movBonesInSkel(bone[0]);

    //LOGMSG << "numbones: " << numbones << '\n'
    //    << "movbones: " << movbones << std::endl;
    //n = (n-3)/((movbones) + 1);
    frame_count = (frame_count - 3) / ((movbones) + 1);

    frame_num_ = frame_count;

    *posture_coll_ = PostureColl_t(frame_num_, Posture(Body::num));
    st_joint_pos6d_->Resize(skeleton_->bone_num(), frame_num_);
    input_stream.open(file_name);
    std::getline(input_stream, *amc_header_);

    while (1) 
    {
        input_stream >> str;
        if (strncmp(str, ":DEGREES", strlen(":DEGREES")) == 0)
        {
            break;
        }
    }

    int32_t pose_idx = 0;

    for (int32_t frame_idx = 0; frame_idx < frame_num_; ++frame_idx)
    {
        input_stream >> pose_idx;

        for (int32_t move_bone_counter = 0; move_bone_counter < movbones; ++move_bone_counter)
        {
            std::string bone_name;
            input_stream >> bone_name;
            
            int32_t bone_idx = skeleton_->BoneIdx(bone_name);
            Vector6d_t joint_pos6d;

            if (skeleton_->bone_num() == bone_idx)
            {
                LOGERR << "Invalid bone_idx: " << bone_idx << '\n'
                    << "skeleton bone num is " << skeleton_->bone_num() << '\n'
                    << std::endl;
                assert(FALSE);
            }

            if (std::string("root") == bone_name)
            {
                Vector3d_t root_pos;
                input_stream >> root_pos.x() >> root_pos.y() >> root_pos.z();
                root_pos *= skeleton_scale;
                posture_coll_->at(frame_idx).setBoneTranslation(bone_idx, root_pos);
                posture_coll_->at(frame_idx).setRootPos(root_pos);

                joint_pos6d.SetLinearVector(root_pos);
            }

            Vector3d_t bone_rotation;
            if (1 == bone[bone_idx].dofx)
            {
                input_stream >> bone_rotation.x();
            }

            if (1 == bone[bone_idx].dofy)
            {
                input_stream >> bone_rotation.y();
            }

            if (1 == bone[bone_idx].dofz)
            {
                input_stream >> bone_rotation.z();
            }

            posture_coll_->at(frame_idx).setBoneRotation(bone_idx, bone_rotation);
            joint_pos6d.SetAngularVector(bone_rotation);
            st_joint_pos6d_->SetElement(
                    bone_idx,
                    frame_idx,
                    joint_pos6d
                    );
        }
    } // for (int32_t frame_idx = 0; frame_idx < frame_num_; ++frame_idx)

    input_stream.close();
    LOGMSG << frame_count << " samples in " << file_name << " are read" << std::endl;

    //return frame_count;
    return TRUE;
}

int32_t Motion::WriteAmcFile(const std::string &file_name, double skeleton_scale)
{
    if (nullptr == skeleton_)
    {
        LOGERR << "No skeleton instance for Motion" << std::endl;
        assert(FALSE);
    }

    Bone *bone = skeleton_->root_bone();

    std::ofstream outputStream(file_name);
    if(outputStream.fail())
    {
        LOGERR << "Failed to open " << file_name << std::endl;
        return -1;
    }

    // header lines
    outputStream << std::setprecision(8) << std::fixed;
    outputStream << amc_header_->substr(0, amc_header_->size() - 1) << std::endl;
    outputStream << ":FULLY-SPECIFIED" << std::endl;
    outputStream << ":DEGREES" << std::endl;
    int32_t numbones = numBonesInSkel(bone[0]);

    for (int32_t f=0; f < frame_num_; f++)
    {
        outputStream << f+1 << std::endl;
        outputStream << "root "
            << posture_coll_->at(f).rootPos().x() / skeleton_scale << " " 
            << posture_coll_->at(f).rootPos().y() / skeleton_scale << " " 
            << posture_coll_->at(f).rootPos().z() / skeleton_scale << " " 
            << posture_coll_->at(f).boneRotation(Skeleton::RootIdx()).x() << " " 
            << posture_coll_->at(f).boneRotation(Skeleton::RootIdx()).y() << " " 
            << posture_coll_->at(f).boneRotation(Skeleton::RootIdx()).z();

        for(int32_t j = 2; j < numbones; j++) 
        {
            //output bone name
            if (bone[j].dof != 0)
            {
                //outputStream << std::endl << skeleton_->idx2name(j);
                outputStream << std::endl << skeleton_->BoneName(j);
            }

            //output bone rotation angles
            if (1 == bone[j].dofx)
            {
                outputStream << " " << posture_coll_->at(f).boneRotation(j).x();
            }

            if (1 == bone[j].dofy)
            {
                outputStream << " " << posture_coll_->at(f).boneRotation(j).y();
            }

            if (1 == bone[j].dofz)
            {
                outputStream << " " << posture_coll_->at(f).boneRotation(j).z();
            }
        }
        outputStream << std::endl;
    }

    outputStream.close();

    LOGMSG << "Write " << frame_num_ << " samples to " << file_name << std::endl;

    return frame_num_;
}

} // namespace acclaim {
