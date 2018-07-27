#include "BallModel.h"

Ball::Ball()
:m_dMass(1.0f),
m_dRadius(0.3f),
m_Position(Vector3d::ZERO),
m_Velocity(Vector3d::ZERO),
m_Force(Vector3d::ZERO)
{
}

Ball::Ball(const Ball &a_rcBall)
:m_dMass(a_rcBall.m_dMass),
m_dRadius(a_rcBall.m_dRadius),
m_Position(a_rcBall.m_Position),
m_Velocity(a_rcBall.m_Velocity),
m_Force(a_rcBall.m_Force)
{
}

Ball::~Ball()
{
}
