#ifndef CSPRING_H
#define CSPRING_H

#include "Vector3d.h"
#include "CParticle.h"

class CSpring
{
    public:
        typedef enum
        {
            Type_nStruct,
            Type_nShear,
            Type_nBending,
        } enType_t;
    private:
        int   m_iSpringStartID;
        int   m_iSpringEndID;
        double m_dRestLength;
        double m_dSpringCoef;
        double m_dDamperCoef;
        Vector3d m_Color;
        enType_t m_nType;
        
    public:
        CSpring(
            const int a_ciSpringStartID,
            const int a_ciSpringEndID,
            const double a_cdRestLength,
            const double a_cdSpringCoef,
            const double a_cdDamperCoef,
            const Vector3d &a_rcColor,
            const enType_t a_cType
            );
        CSpring(const CSpring &a_rSpring);
        ~CSpring();
        inline void SetSpringCoef(const double a_cdSpringCoef){m_dSpringCoef = a_cdSpringCoef;}
        inline void SetDamperCoef(const double a_cdDamperCoef){m_dDamperCoef = a_cdDamperCoef;}
        inline int      GetSpringStartID()   {return m_iSpringStartID;}
        inline int      GetSpringEndID()     {return m_iSpringEndID;}
        inline double   GetSpringRestLength(){return m_dRestLength;}
        inline double   GetSpringCoef()      {return m_dSpringCoef;}
        inline double   GetDamperCoef()      {return m_dDamperCoef;}
        inline Vector3d GetSpringColor()     {return m_Color;}
        inline enType_t GetSpringType()      {return m_nType;}

};

#endif