#include "stdafx.h"
#include "Vector3d.h"
#include <math.h>

const Vector3d Vector3d::ZERO  ( 0.0, 0.0, 0.0 );
const Vector3d Vector3d::UNIT_X( 1.0, 0.0, 0.0 );
const Vector3d Vector3d::UNIT_Y( 0.0, 1.0, 0.0 );
const Vector3d Vector3d::UNIT_Z( 0.0, 0.0, 1.0 );

std::ostream &operator<<(
    std::ostream &a_kOstream,
    const Vector3d &a_kInput
    )
{
    a_kOstream << a_kInput.x << ", "
        << a_kInput.y << ", "
        << a_kInput.z << std::endl;

    return a_kOstream;
}
