#ifndef CPARTICLE_H
#define CPARTICLE_H

#include "Vector3d.h"

class CParticle
{
private:
        double   m_dMass;
        Vector3d m_Position;
        Vector3d m_Velocity;
        //Vector3d m_Acceleration;
        Vector3d m_Force;       
        Vector3d m_AccumulateNormal;   // an accumulated normal (i.e. non normalized), used for OpenGL soft shading  
        bool m_IsMovable;            

public:
        CParticle();
        CParticle(const CParticle &a_rcParticle);
        ~CParticle();

        inline void SetMass(const double a_cdMass){m_dMass = a_cdMass;}
        inline void SetPosition(const Vector3d &a_rcPosition){ if (m_IsMovable) m_Position = a_rcPosition;}
        inline void SetVelocity(const Vector3d &a_rcVelocity){ if (m_IsMovable) m_Velocity = a_rcVelocity;}
        inline void SetAcceleration(const Vector3d &a_rcAcceleration){ if (m_IsMovable) m_Force = a_rcAcceleration*m_dMass;}
        inline void SetForce(const Vector3d &a_rcForce){m_Force = a_rcForce;}
        inline void SetMovable(const bool isMovable){m_IsMovable = isMovable;}
        inline void ResetNormal(){m_AccumulateNormal = Vector3d(0, 0, 0);}

        inline double GetMass(){return m_dMass;}
        inline Vector3d GetPosition(){return m_Position;}
        inline Vector3d GetVelocity(){return m_Velocity;}
        inline Vector3d GetAcceleration(){return m_Force/m_dMass;}
        inline Vector3d GetForce(){return m_Force;}
        inline Vector3d GetNormal(){return m_AccumulateNormal;} // notice, the normal is not unit length

        inline void AddPosition(const Vector3d &a_rcPosition){ if (m_IsMovable) m_Position += a_rcPosition;}
        inline void AddVelocity(const Vector3d &a_rcVelocity){ if (m_IsMovable) m_Velocity += a_rcVelocity;}
        //inline void AddAcceleration(const Vector3d &a_rcAcceleration){m_Force += a_rcAcceleration*m_dMass;}
        inline void AddForce(const Vector3d &a_rcForce){m_Force += a_rcForce;}
        inline void AddToNormal(const Vector3d a_NormalVec){m_AccumulateNormal += a_NormalVec.NormalizedCopy();}
};

#endif