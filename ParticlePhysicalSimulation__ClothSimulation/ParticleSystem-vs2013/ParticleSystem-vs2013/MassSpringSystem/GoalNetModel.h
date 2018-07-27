#ifndef GOALNETMODEL_H
#define GOALNETMODEL_H

#include <vector>
#include <map>
#include "CParticle.h"
#include "CSpring.h"
using namespace std;

class GoalNet
{
public:

    GoalNet();
    GoalNet(const GoalNet &a_rcGoalNet);
    GoalNet(const std::string &a_rcsConfigFilename);
    ~GoalNet();

    CParticle& GetParticle(int particleIdx);    // get particle in the container with index
    CSpring& GetSpring(int springIdx);          // get spring in the container with index

    int ParticleNum() const;  // return number of particles in the net
    int SpringNum() const;    // return number of springs in the net
	void setParticle(CParticle p, int x){ m_Particles[x] = p; };
    double GetWidth() const;
    double GetHeight() const;
    double GetLength() const;
    int GetWidthNum() const;
    int GetHeightNum() const;
    int GetLengthNum() const;
    int GetParticleID(        // get index to access particles in the container
        int xId,
        int yId,
        int zId
        );
    Vector3d GetInitPos() const;

    void SetSpringCoef(
        const double a_cdSpringCoef,
        const CSpring::enType_t a_cSpringType
        );
    void SetDamperCoef(
        const double a_cdDamperCoef,
        const CSpring::enType_t a_cSpringType
        );

    void Reset();
    void AddForceField(const Vector3d &a_kForce);    //add gravity
    void ComputeInternalForce();


private:

    void Initialize();
    void InitializeParticle();
    void InitializeSpring();

    Vector3d ComputeSpringForce(
        const Vector3d &a_crPos1,
        const Vector3d &a_crPos2,
        const double a_cdSpringCoef,
        const double a_cdRestLength
        );
    Vector3d ComputeDamperForce(
        const Vector3d &a_crPos1,
        const Vector3d &a_crPos2,
        const Vector3d &a_crVel1,
        const Vector3d &a_crVel2,
        const double a_cdDamperCoef
        );

    void UpdateSpringCoef(
        const double a_cdSpringCoef,
        const CSpring::enType_t a_cSpringType
        );
    void UpdateDamperCoef(
        const double a_cdDamperCoef,
        const CSpring::enType_t a_cSpringType
        );

    bool isAtFace(
        const int xId,
        const int yId,
        const int zId
        );
    bool isAtEdge(
        const int xId,
        const int yId,
        const int zId
        );

    vector<CParticle> m_Particles;
    vector<CSpring> m_Springs;
    map<int, int> m_ParticleIdMap;

    Vector3d m_InitPos;   
    double m_NetWidth;
    double m_NetHeight;
    double m_NetLength;
    /*
     * number of particles at a edge in the net 
     */
    int m_NumAtWidth;    
    int m_NumAtHeight;    
    int m_NumAtLength;  
    /*
     * spring parameter
     */
    double m_dSpringCoefStruct;    
    double m_dSpringCoefShear;
    double m_dSpringCoefBending;
    double m_dDamperCoefStruct;
    double m_dDamperCoefShear;
    double m_dDamperCoefBending;
    Vector3d m_ColorStruct;     
    Vector3d m_ColorShear;
    Vector3d m_ColorBending;
};

#endif