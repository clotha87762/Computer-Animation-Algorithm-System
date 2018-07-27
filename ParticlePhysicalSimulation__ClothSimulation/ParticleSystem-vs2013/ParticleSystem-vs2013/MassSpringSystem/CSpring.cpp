#include "CSpring.h"

CSpring::CSpring(const int a_ciSpringStartID,
                 const int a_ciSpringEndID,
                 const double a_cdRestLength,
                 const double a_cdSpringCoef,
                 const double a_cdDamperCoef,
                 const Vector3d &a_rcColor,
                 const enType_t a_cType)
   :m_iSpringStartID(a_ciSpringStartID),
    m_iSpringEndID(a_ciSpringEndID),
    m_dRestLength(a_cdRestLength),
    m_dSpringCoef(a_cdSpringCoef),
    m_dDamperCoef(a_cdDamperCoef),
    m_Color(a_rcColor),
    m_nType(a_cType)
{
}

CSpring::CSpring(const CSpring &a_rcSpring)
   :m_iSpringStartID(a_rcSpring.m_iSpringStartID),
    m_iSpringEndID(a_rcSpring.m_iSpringEndID),
    m_dRestLength(a_rcSpring.m_dRestLength),
    m_dSpringCoef(a_rcSpring.m_dSpringCoef),
    m_dDamperCoef(a_rcSpring.m_dDamperCoef),
    m_Color(a_rcSpring.m_Color),
    m_nType(a_rcSpring.m_nType)
{
}

CSpring::~CSpring()
{
}