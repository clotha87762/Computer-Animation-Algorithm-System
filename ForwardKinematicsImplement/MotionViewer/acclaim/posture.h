#ifndef _POSTURE_H_
#define _POSTURE_H_

#include <memory>
#include <vector>
#include "acclaim_body_idx.h"
#include "math_type.h"

class Posture;
typedef std::vector<Posture> PostureColl_t;

/**
 * Root position and all bone, including root, rotatin angles
 */
class Posture
{

public:

    static Posture toRadianBoneRotation(const Posture &degree_posture);
    static Posture toDegreeBoneRotation(const Posture &radian_posture);
    /**
     * \brief
     */
    Posture();
    /**
     * \brief
     * \param[in] size
     */
    Posture(const int size);
    /**
     * \brief
     * \param[in] other
     */
    Posture(const Posture &other);
    /**
     * \brief
     */
    ~Posture();
    /**
     * \brief
     * \param[in] other
     */
    Posture &operator=(const Posture &other);
    /**
     * \brief
     * \return
     */
    int size() const;
    /**
     * \brief
     * \return
     */
    Vector3d_t rootPos() const;
    /**
     * \brief
     * \return
     */
    Vector3dColl_t boneRotations() const;
    /**
     * \brief
     * \param[in] bone_idx
     * \return
     */
    Vector3d_t boneRotation(const int bone_idx) const;
    /**
     * \brief
     * \return
     */
    Vector3dColl_t boneTranslations() const;
    /**
     * \brief
     * \param[in] bone_idx
     * \return
     */
    Vector3d_t boneTranslation(const int bone_idx) const;
    /**
     * \brief
     * \param[in]
     */
    void setRootPos(const Vector3d_t &pos);
    /**
     * \brief
     * \param[in]
     */
    void setBoneRotations(const Vector3dColl_t &angle3d_coll);
    /**
     * \brief
     * \param[in]
     */
    void setBoneRotation(const int bone_idx, const Vector3d_t &angle3d);
    /**
     * \brief
     * \param[in]
     */
    void setBoneTranslations(const Vector3dColl_t &xlt_coll);
    /**
     * \brief
     * \param[in]
     */
    void setBoneTranslation(const int bone_idx, const Vector3d_t &xlt);
    /**
     * \brief
     * \param[in]
     */
    void reset(const int size);
    /**
     * \brief
     * \return
     */
    bool isEmpty() const;
    /**
     * \brief
     * \param[in] a_kPoseture
     * \param[in] epsilon
     */
    bool isApprox(
            const Posture &other,
            const double epsilon
            ) const;

protected:

private:

    /**
     * Rotation(x, y, z) of all bones at a particular time frame in their local coordinate system.
     * If a particular bone does not have a certain Dof,
     * the corresponding rotation is 0.
     * The order of the bones in the container corresponding to their IDs in ASF file: root,
     * lhipjoint, lfemur....
     */
    int32_t size_;
    std::unique_ptr<Vector3d_t> root_pos_;
    std::unique_ptr<Vector3dColl_t> bone_rotations_;
    std::unique_ptr<Vector3dColl_t> bone_translations_;
};

#endif // #ifndef _POSTURE_H_
