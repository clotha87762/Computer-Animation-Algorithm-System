#ifndef _MATH_TYPE_H_
#define _MATH_TYPE_H_

#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>
#undef Success ///< Avoid the conflicts with X11.h
#include "Eigen/Dense"
#include "Eigen/Geometry"
#include <Eigen/StdVector>
#include "math_col_vector_3d.h"
#include "math_col_vector_6d.h"
#include "math_homogeneous_transform.h"
#include "math_spatial_temporal_data.h"

typedef math::ColVector3d<double> Vector3d_t;
typedef math::ColVector6d<double> Vector6d_t;
typedef Eigen::Matrix<double, 2, 1> Vector2d_t;
typedef Eigen::Matrix<double, 4, 1> Vector4d_t;
typedef Eigen::Matrix<int, 2, 1> ScreenPos_t;

typedef Eigen::Matrix<double, 3, 3> RotMat_t;
typedef Eigen::Matrix<double, 4, 4> HomoTransfMat_t;
typedef math::HomogeneousTransform<double> HomoXfm_t;
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> MatrixN_t;
typedef Eigen::ParametrizedLine<double, 3> ParametrizedLine_t;
typedef Eigen::Hyperplane<double, 3> Hyperplane_t;

typedef Eigen::Quaternion<double, Eigen::DontAlign> Quaternion_t;
typedef Eigen::AngleAxis<double> AngleAxis_t;

typedef std::vector<Vector3d_t> Vector3dColl_t;
typedef std::vector<Vector6d_t, Eigen::aligned_allocator<Vector6d_t>> Vector6dColl_t;
typedef std::vector<RotMat_t> RotMatColl_t;
typedef std::vector<HomoXfm_t> HomoXfmColl_t;
typedef std::vector<Quaternion_t> QuaternionColl_t;
typedef std::pair<Vector3d_t, Vector3d_t> Vector3dPair_t;
//typedef math::SpatialTemporalData<Vector6d_t> SpatialTemporalVector6d_t;
typedef math::SpatialTemporalData<Vector6d_t, Eigen::aligned_allocator<Vector6d_t>> SpatialTemporalVector6d_t;

namespace Eigen {

template<typename charT, typename traits, typename Scalar_t, int Option>
std::basic_ostream<charT, traits> &operator<<(
        std::basic_ostream<charT, traits> &ostream,
        const Quaternion<Scalar_t, Option> &quat
        );

}

namespace Eigen {

template<typename charT, typename traits, typename Scalar_t, int Option>
std::basic_ostream<charT, traits> &operator<<(
        std::basic_ostream<charT, traits> &ostream,
        const Quaternion<Scalar_t, Option> &quat
        )
{
    ostream << quat.w() << ", "
        << quat.x() << ", "
        << quat.y() << ", "
        << quat.z();

    return ostream;
}

}

#endif // #ifndef _MATH_TYPE_H_
