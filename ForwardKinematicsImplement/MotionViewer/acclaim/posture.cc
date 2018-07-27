#include "posture.h"
#include <algorithm>
#include "boost/numeric/conversion/cast.hpp"
#include "console_log.h"
#include "math_utils.h"
#include "math_col_vector_3d.h"

// public func.

Posture Posture::toRadianBoneRotation(const Posture &degree_posture)
{
    Vector3dColl_t radian_bone_rotations = math::ToRadian(
            degree_posture.boneRotations()
            );

    Posture radian_posture = degree_posture;
    radian_posture.setBoneRotations(radian_bone_rotations);

    return radian_posture;
}

Posture Posture::toDegreeBoneRotation(const Posture &radian_posture)
{
    Vector3dColl_t degree_bone_rotations = math::ToDegree(
            radian_posture.boneRotations()
            );

    Posture degree_posture = radian_posture;
    degree_posture.setBoneRotations(degree_bone_rotations);

    return degree_posture;
}

Posture::Posture()
    :size_(0),
    root_pos_(new Vector3d_t),
    bone_rotations_(new Vector3dColl_t),
    bone_translations_(new Vector3dColl_t)
{
}

Posture::Posture(const int size)
    :size_(size),
    root_pos_(new Vector3d_t),
    bone_rotations_(new Vector3dColl_t(size, Vector3d_t::Zero())),
    bone_translations_(new Vector3dColl_t(size, Vector3d_t::Zero()))
{
}

Posture::Posture(const Posture &other)
    :size_(other.size()),
    root_pos_(new Vector3d_t(other.rootPos())),
    bone_rotations_(new Vector3dColl_t(other.boneRotations())),
    bone_translations_(new Vector3dColl_t(other.boneTranslations()))
{
}

Posture::~Posture()
{
}

Posture &Posture::operator=(const Posture &other)
{
    if (&other == this)
    {
        return *this;
    }

    size_ = other.size();
    *root_pos_ = other.rootPos();
    *bone_rotations_ = other.boneRotations();
    *bone_translations_ = other.boneTranslations();

    return *this;
}

int Posture::size() const
{
    return size_;
}

Vector3d_t Posture::rootPos() const
{
    return *root_pos_;
}

Vector3dColl_t Posture::boneRotations() const
{
    return *bone_rotations_;
}

Vector3d_t Posture::boneRotation(const int bone_idx) const
{
    return (*bone_rotations_)[bone_idx];
}

Vector3dColl_t Posture::boneTranslations() const
{
    return *bone_translations_;
}

Vector3d_t Posture::boneTranslation(const int bone_idx) const
{
    return (*bone_translations_)[bone_idx];
}

void Posture::setRootPos(const Vector3d_t &pos)
{
    *root_pos_ = pos;
}

void Posture::setBoneRotations(const Vector3dColl_t &angle3d_coll)
{
    if (boost::numeric_cast<int>(angle3d_coll.size()) != size_)
    {
        LOGERR << "Unmatched size of angle3d_coll: " << angle3d_coll.size() << '\n'
            << "correct size : " << size_ << std::endl;
        assert(FALSE);
    }

    *bone_rotations_ = angle3d_coll;
}

void Posture::setBoneRotation(const int bone_idx, const Vector3d_t &angle3d)
{
    (*bone_rotations_)[bone_idx] = angle3d;
}

void Posture::setBoneTranslations(const Vector3dColl_t &xlt_coll)
{
    *bone_translations_ = xlt_coll;
}

void Posture::setBoneTranslation(const int bone_idx, const Vector3d_t &xlt)
{
    (*bone_translations_)[bone_idx] = xlt;
}

void Posture::reset(const int size)
{
    size_ = size;
    *root_pos_ = Vector3d_t::Zero();
    *bone_rotations_ = Vector3dColl_t(size_, Vector3d_t::Zero());
    *bone_translations_ = Vector3dColl_t(size_, Vector3d_t::Zero());
}

bool Posture::isEmpty() const
{
    return 0 == size_ ? TRUE : FALSE;
}

bool Posture::isApprox(
        const Posture &other,
        const double epsilon
        ) const
{
    if (other.size() != this->size())
    {
        LOGMSG << "Unequal size" << std::endl;
        return FALSE;
    }

    Vector3d_t root_pos_diff = (*root_pos_) - other.rootPos();
    bool is_approx_root_pos = root_pos_diff.isZero(epsilon);

    if (!is_approx_root_pos)
    {
        LOGMSG << "Unapproximated root position" << std::endl;
        return FALSE;
    }

    bool is_approx_rotation = std::equal(
            bone_rotations_->begin(),
            bone_rotations_->end(),
            other.boneRotations().begin(),
            [&](const Vector3d_t &ref_angle, const Vector3d_t &trg_angle)
            {return (ref_angle - trg_angle).isZero(epsilon);}
            );

    if (!is_approx_rotation)
    {
        return FALSE;
    }

    bool is_approx_xlt = std::equal(
            bone_translations_->begin(),
            bone_translations_->end(),
            other.boneTranslations().begin(),
            [&](const Vector3d_t &ref_xlt, const Vector3d_t &trg_xlt)
            {return (ref_xlt - trg_xlt).isZero(epsilon);}
            );

    return is_approx_xlt;
}

// protected func.

// private func.
