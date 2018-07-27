#ifndef BALLMODEL_H
#define BALLMODEL_H

#include "Vector3d.h"

class Ball
{
public:

    Ball();
    Ball(const Ball &a_rcBall);
    ~Ball();

    inline void SetMass(const double a_cdMass){ m_dMass = a_cdMass; }
    inline void SetPosition(const Vector3d &a_rcPosition){ m_Position = a_rcPosition; }
    inline void SetVelocity(const Vector3d &a_rcVelocity){ m_Velocity = a_rcVelocity; }
    inline void SetAcceleration(const Vector3d &a_rcAcceleration){ m_Force = a_rcAcceleration*m_dMass; }
    inline void SetForce(const Vector3d &a_rcForce){ m_Force = a_rcForce; }

    inline double GetMass(){ return m_dMass; }
    inline double GetRadius(){ return m_dRadius; }
    inline Vector3d GetPosition(){ return m_Position; }
    inline Vector3d GetVelocity(){ return m_Velocity; }
    inline Vector3d GetAcceleration(){ return m_Force / m_dMass; }
    inline Vector3d GetForce(){ return m_Force; }

    inline void AddPosition(const Vector3d &a_rcPosition){ m_Position += a_rcPosition; }
    inline void AddVelocity(const Vector3d &a_rcVelocity){ m_Velocity += a_rcVelocity; }
    inline void AddForce(const Vector3d &a_rcForce){ m_Force += a_rcForce; }

private:

    double m_dMass;
    double m_dRadius;
    Vector3d m_Position;
    Vector3d m_Velocity;
    Vector3d m_Force;

};

#endif