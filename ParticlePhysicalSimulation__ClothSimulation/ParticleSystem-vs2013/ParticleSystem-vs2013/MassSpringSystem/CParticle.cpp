#include "CParticle.h"

CParticle::CParticle()
   :m_dMass(1.0f),
    m_Position(Vector3d::ZERO),
    m_Velocity(Vector3d::ZERO),
    //m_Acceleration(Vector3d::ZERO),
    m_Force(Vector3d::ZERO),
    m_AccumulateNormal(Vector3d::ZERO),
    m_IsMovable(true)
{
}

CParticle::CParticle(const CParticle &a_rcParticle)
   :m_dMass(a_rcParticle.m_dMass),
    m_Position(a_rcParticle.m_Position),
    m_Velocity(a_rcParticle.m_Velocity),
    //m_Acceleration(a_rcParticle.m_Acceleration),
    m_Force(a_rcParticle.m_Force),
    m_AccumulateNormal(a_rcParticle.m_AccumulateNormal),
    m_IsMovable(a_rcParticle.m_IsMovable)
{
}

CParticle::~CParticle()
{
}