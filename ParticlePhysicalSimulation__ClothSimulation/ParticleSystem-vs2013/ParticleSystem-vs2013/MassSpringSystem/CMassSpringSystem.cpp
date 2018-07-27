#include <cmath>
#include <iostream>
#include "configFile.h"
#include "CMassSpringSystem.h"
#include "glut.h"
#include "Render_API.h"

#pragma comment( lib, "glut32.lib" )

const double g_cdDeltaT = 0.001f;
const double g_cdK	   = 2500.0f;
const double g_cdD	   = 50.0f;
const double eps = 0.01;
const Vector3d  normal = Vector3d(0,1.0,0);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Constructor & Destructor
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMassSpringSystem::CMassSpringSystem()
   :m_bDrawParticle(true),
    m_bDrawStruct(false),
    m_bDrawShear(false),
    m_bDrawBending(false),
    m_bDrawGoalpost(true),
    m_bSimulation(false),

    m_iIntegratorType(EXPLICIT_EULER),

    m_dDeltaT(g_cdDeltaT),
    m_dSpringCoefStruct(g_cdK),
    m_dSpringCoefShear(g_cdK),
    m_dSpringCoefBending(g_cdK),
    m_dDamperCoefStruct(g_cdD),
    m_dDamperCoefShear(g_cdD),
    m_dDamperCoefBending(g_cdD),

    m_ForceField(Vector3d(0.0,-9.8,0.0)),

    m_GoalNet(),
    m_Balls()
{
}

CMassSpringSystem::CMassSpringSystem(const std::string &a_rcsConfigFilename)
:m_GoalNet(a_rcsConfigFilename)
{
    int iIntegratorType;
    double dSpringCoef,dDamperCoef;

    ConfigFile configFile;
    configFile.suppressWarnings(1);

    configFile.addOption("DrawParticle"        ,&m_bDrawParticle);
    configFile.addOption("DrawSpringStructural",&m_bDrawStruct);
    configFile.addOption("DrawSpringShear"     ,&m_bDrawShear);
    configFile.addOption("DrawSpringBending"   ,&m_bDrawBending);
    configFile.addOption("DrawGoalpost"        ,&m_bDrawGoalpost);
    configFile.addOption("SimulationStart"     ,&m_bSimulation);

    configFile.addOption("IntegratorType",&iIntegratorType);
    configFile.addOption("DeltaT"    ,&m_dDeltaT);
    configFile.addOption("SpringCoef",&dSpringCoef);
    configFile.addOption("DamperCoef",&dDamperCoef);

    int code = configFile.parseOptions((char *)a_rcsConfigFilename.c_str());
    if(code == 1)
    {
        std::cout<<"Error in CMassSpringSystem constructor."<<std::endl;
        system("pause");
        exit(0);
    }
    m_iIntegratorType = CMassSpringSystem::EXPLICIT_EULER;
    if(iIntegratorType == 1)
    {
        m_iIntegratorType = CMassSpringSystem::RUNGE_KUTTA;
    }

    m_dSpringCoefStruct  = dSpringCoef;
    m_dSpringCoefShear   = dSpringCoef;
    m_dSpringCoefBending = dSpringCoef;
    m_dDamperCoefStruct  = dDamperCoef;
    m_dDamperCoefShear   = dDamperCoef;
    m_dDamperCoefBending = dDamperCoef;

    m_ForceField   = Vector3d(0.0,-9.8,0.0);

    Reset();
}

CMassSpringSystem::CMassSpringSystem(const CMassSpringSystem &a_rcMassSpringSystem)
    :m_bDrawParticle(a_rcMassSpringSystem.m_bDrawParticle),
    m_bDrawStruct(a_rcMassSpringSystem.m_bDrawStruct),
    m_bDrawShear(a_rcMassSpringSystem.m_bDrawShear),
    m_bDrawBending(a_rcMassSpringSystem.m_bDrawBending),
    m_bDrawGoalpost(a_rcMassSpringSystem.m_bDrawGoalpost),
    m_bSimulation(a_rcMassSpringSystem.m_bSimulation),

    m_iIntegratorType(a_rcMassSpringSystem.m_iIntegratorType),

    m_dDeltaT(a_rcMassSpringSystem.m_dDeltaT),
    m_dSpringCoefStruct(a_rcMassSpringSystem.m_dSpringCoefStruct),
    m_dSpringCoefShear(a_rcMassSpringSystem.m_dSpringCoefShear),
    m_dSpringCoefBending(a_rcMassSpringSystem.m_dSpringCoefBending),
    m_dDamperCoefStruct(a_rcMassSpringSystem.m_dDamperCoefStruct),
    m_dDamperCoefShear(a_rcMassSpringSystem.m_dDamperCoefShear),
    m_dDamperCoefBending(a_rcMassSpringSystem.m_dDamperCoefBending),

    m_ForceField(a_rcMassSpringSystem.m_ForceField)
{
}
CMassSpringSystem::~CMassSpringSystem()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Draw
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMassSpringSystem::Draw()
{
    DrawGoalNet();
    DrawBall();
}

void CMassSpringSystem::DrawGoalNet()
{    
    // draw particle
    if (m_bDrawParticle)
    {
        glPushAttrib(GL_CURRENT_BIT);
        for (int uiI = 0; uiI < m_GoalNet.ParticleNum(); uiI++)
        {
            setColor3f(1.0, 0.0, 0.0);
            drawPoint(m_GoalNet.GetParticle(uiI).GetPosition(), 3.0);
        }
        glPopAttrib();
    }

    // draw spring
    glPushAttrib(GL_CURRENT_BIT);
    for (int uiI = 0; uiI < m_GoalNet.SpringNum(); uiI++)
    {
        if ((m_GoalNet.GetSpring(uiI).GetSpringType() == CSpring::Type_nStruct && m_bDrawStruct) ||
            (m_GoalNet.GetSpring(uiI).GetSpringType() == CSpring::Type_nShear && m_bDrawShear) ||
            (m_GoalNet.GetSpring(uiI).GetSpringType() == CSpring::Type_nBending && m_bDrawBending))
        {
            int iSpringStartID = m_GoalNet.GetSpring(uiI).GetSpringStartID();
            int iSpringEndID = m_GoalNet.GetSpring(uiI).GetSpringEndID();
            Vector3d springColor = m_GoalNet.GetSpring(uiI).GetSpringColor();
            Vector3d startPos = m_GoalNet.GetParticle(iSpringStartID).GetPosition();
            Vector3d endPos = m_GoalNet.GetParticle(iSpringEndID).GetPosition();

            setColor3fv(springColor);
            drawLine(startPos, endPos);                   
        }
    }
    glPopAttrib();

    if (m_bDrawGoalpost)
    {
        DrawGoalpost();
    }
}

void CMassSpringSystem::DrawGoalpost()
{
    // draw cylinder
    int widthNum = m_GoalNet.GetWidthNum();
    int heightNum = m_GoalNet.GetHeightNum();
    int lengthNum = m_GoalNet.GetLengthNum();

    int backBottomRightId = m_GoalNet.GetParticleID(0, 0, 0);
    int backBottomLeftId = m_GoalNet.GetParticleID(0, 0, lengthNum - 1);
    int frontBottomRightId = m_GoalNet.GetParticleID(widthNum - 1, 0, 0);
    int frontBottomLeftId = m_GoalNet.GetParticleID(widthNum - 1, 0, lengthNum - 1);
    int backTopRightId = m_GoalNet.GetParticleID(0, heightNum - 1, 0);
    int backTopLeftId = m_GoalNet.GetParticleID(0, heightNum - 1, lengthNum - 1);
    int frontTopRightId = m_GoalNet.GetParticleID(widthNum - 1, heightNum - 1, 0);
    int frontTopLeftId = m_GoalNet.GetParticleID(widthNum - 1, heightNum - 1, lengthNum - 1);
    drawCylinder(
        m_GoalNet.GetParticle(backBottomLeftId).GetPosition(),
        m_GoalNet.GetParticle(backTopLeftId).GetPosition(),
        0.05);
    drawCylinder(
        m_GoalNet.GetParticle(backBottomRightId).GetPosition(),
        m_GoalNet.GetParticle(backTopRightId).GetPosition(),
        0.05);
    drawCylinder(
        m_GoalNet.GetParticle(backTopRightId).GetPosition(),
        m_GoalNet.GetParticle(backTopLeftId).GetPosition(),
        0.05);
    drawCylinder(
        m_GoalNet.GetParticle(backTopRightId).GetPosition(),
        m_GoalNet.GetParticle(frontTopRightId).GetPosition(),
        0.05);
    drawCylinder(
        m_GoalNet.GetParticle(backTopLeftId).GetPosition(),
        m_GoalNet.GetParticle(frontTopLeftId).GetPosition(),
        0.05);
    drawCylinder(
        m_GoalNet.GetParticle(frontTopRightId).GetPosition(),
        m_GoalNet.GetParticle(frontTopLeftId).GetPosition(),
        0.05);
    drawCylinder(
        m_GoalNet.GetParticle(frontBottomRightId).GetPosition(),
        m_GoalNet.GetParticle(frontTopRightId).GetPosition(),
        0.05);
    drawCylinder(
        m_GoalNet.GetParticle(frontBottomLeftId).GetPosition(),
        m_GoalNet.GetParticle(frontTopLeftId).GetPosition(),
        0.05);
}

void CMassSpringSystem::DrawBall()
{
    for (int ballIdx = 0; ballIdx < BallNum(); ++ballIdx)
    {
        drawSolidBall(m_Balls[ballIdx].GetPosition(), m_Balls[ballIdx].GetRadius());
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Set and Update
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMassSpringSystem::Reset()
{ 
    m_GoalNet.Reset();
    m_Balls.clear();
}

void CMassSpringSystem::SetSpringCoef(const double a_cdSpringCoef, const CSpring::enType_t a_cSpringType)
{
    if (a_cSpringType == CSpring::Type_nStruct)
    {
        m_dSpringCoefStruct = a_cdSpringCoef;
        m_GoalNet.SetSpringCoef(a_cdSpringCoef, CSpring::Type_nStruct);
    }
    else if (a_cSpringType == CSpring::Type_nShear)
    {
        m_dSpringCoefShear = a_cdSpringCoef;
        m_GoalNet.SetSpringCoef(a_cdSpringCoef, CSpring::Type_nShear);
    }
    else if (a_cSpringType == CSpring::Type_nBending)
    {
        m_dSpringCoefBending = a_cdSpringCoef;
        m_GoalNet.SetSpringCoef(a_cdSpringCoef, CSpring::Type_nBending);
    }
    else
    {
        std::cout << "Error spring type in CMassSpringSystme SetSpringCoef" << std::endl;
    }

}

void CMassSpringSystem::SetDamperCoef(const double a_cdDamperCoef, const CSpring::enType_t a_cSpringType)
{
    if (a_cSpringType == CSpring::Type_nStruct)
    {
        m_dDamperCoefStruct = a_cdDamperCoef;
        m_GoalNet.SetDamperCoef(a_cdDamperCoef, CSpring::Type_nStruct);
    }
    else if (a_cSpringType == CSpring::Type_nShear)
    {
        m_dDamperCoefShear = a_cdDamperCoef;
        m_GoalNet.SetDamperCoef(a_cdDamperCoef, CSpring::Type_nShear);
    }
    else if (a_cSpringType == CSpring::Type_nBending)
    {
        m_dDamperCoefBending = a_cdDamperCoef;
        m_GoalNet.SetDamperCoef(a_cdDamperCoef, CSpring::Type_nBending);
    }
    else
    {
        std::cout << "Error spring type in CMassSpringSystme SetDamperCoef" << std::endl;
    }
}

double CMassSpringSystem::GetSpringCoef(const CSpring::enType_t a_cSpringType)
{
    if(a_cSpringType == CSpring::Type_nStruct)
    {
        return m_dSpringCoefStruct;
    }
    else if(a_cSpringType == CSpring::Type_nShear)
    {
        return m_dSpringCoefShear;
    }
    else if(a_cSpringType == CSpring::Type_nBending)
    {
        return m_dSpringCoefBending;
    }
    else
    {
        std::cout<<"Error spring type in CMassSpringSystme GetSpringCoef"<<std::endl;
        return -1.0;
    }
}
double CMassSpringSystem::GetDamperCoef(const CSpring::enType_t a_cSpringType)
{
    if(a_cSpringType == CSpring::Type_nStruct)
    {
        return m_dDamperCoefStruct;
    }
    else if(a_cSpringType == CSpring::Type_nShear)
    {
        return m_dDamperCoefShear;
    }
    else if(a_cSpringType == CSpring::Type_nBending)
    {
        return m_dDamperCoefBending;
    }
    else
    {
        std::cout<<"Error spring type in CMassSpringSystme GetDamperCoef"<<std::endl;
        return -1.0;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Simulation Part
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CMassSpringSystem::CheckStable()
{
    double threshold = 1e6;
    for(int pIdx = 0; pIdx < m_GoalNet.ParticleNum(); pIdx++)
    {
        Vector3d particleVel = m_GoalNet.GetParticle(pIdx).GetVelocity();
        if (particleVel.Magnitude() > threshold)
        {
            return false;
        }  
    }
    return true;
}
void CMassSpringSystem::SimulationOneTimeStep()
{
    if(m_bSimulation)
    {
        Integrate();
    }
    
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMassSpringSystem::CreateBall()
{
    // randomly assign initial velocity and position to a ball
    Ball newBall;
    Vector3d randomOffset((double)(rand() % 5 + 5.0), (double)(rand() % 5), (double)(rand() % 5));
    Vector3d randomVelOffset(0.0, 0.0, (double)(rand() % 3));
    Vector3d initBallPos = m_GoalNet.GetInitPos() + randomOffset;
    Vector3d initBallVel = (m_GoalNet.GetInitPos() + randomVelOffset - initBallPos)*7.0 +Vector3d(0,0,0) ;
	cout << initBallVel << endl;
    //Vector3d initBallVel = Vector3d::ZERO;
    newBall.SetPosition(initBallPos);
    newBall.SetVelocity(initBallVel);
    m_Balls.push_back(newBall);
}

int CMassSpringSystem::BallNum()
{
    return m_Balls.size();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Compute Force
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMassSpringSystem::ResetAllForce()
{
    for (int pIdx = 0; pIdx < m_GoalNet.ParticleNum(); ++pIdx)
    {
        m_GoalNet.GetParticle(pIdx).SetAcceleration(Vector3d::ZERO);
    }

    for (int ballIdx = 0; ballIdx < BallNum(); ++ballIdx)
    {
        m_Balls[ballIdx].SetAcceleration(Vector3d::ZERO);
    }
}

void CMassSpringSystem::ComputeAllForce()
{
    ComputeParticleForce();
    ComputeBallForce();
}

void CMassSpringSystem::ComputeParticleForce()
{
    m_GoalNet.AddForceField(m_ForceField);
    m_GoalNet.ComputeInternalForce();
}

void CMassSpringSystem::ComputeBallForce()
{
    for (int ballIdx = 0; ballIdx < BallNum(); ++ballIdx)
    {
        m_Balls[ballIdx].AddForce(m_ForceField * m_Balls[ballIdx].GetMass());
    }
}

void CMassSpringSystem::HandleCollision()
{
    ParticlePlaneCollision();
    BallPlaneCollision();
    BallToBallCollision();
    BallParticleCollision();
}

void CMassSpringSystem::ParticlePlaneCollision()
{
    //TO DO 
	for (int pIdx = 0; pIdx < m_GoalNet.ParticleNum(); pIdx++){
		CParticle p = m_GoalNet.GetParticle(pIdx);
		double kr = 0.5;
		double kf = 25;
		if (p.GetPosition().DotProduct(normal) < (-1.0+eps) && p.GetVelocity().DotProduct(normal) < 0){
			Vector3d temp = p.GetVelocity();
			temp.y = temp.y * kr * (-1);
			p.SetVelocity(temp);
		
		}


			if (p.GetPosition().DotProduct(normal) <(-1.0+eps)){

				if (abs(p.GetVelocity().DotProduct(normal)) < eps && p.GetForce().DotProduct(normal) < 0){   // Friction
					Vector3d temp = p.GetVelocity();
					temp.y = 0;
					temp.Normalize();
					p.AddForce(p.GetForce().DotProduct(normal)*(-1)*kf*(-1)*temp);
				}
				

				if (p.GetForce().DotProduct(normal) < 0){
					p.AddForce(p.GetForce().DotProduct(normal) * normal * (-1));
				}

			}

		m_GoalNet.setParticle(p, pIdx);
	}
}

void CMassSpringSystem::BallPlaneCollision()
{
    //TO DO
	  for (int ballIdx = 0; ballIdx < BallNum(); ++ballIdx)
    {
		Ball b = m_Balls[ballIdx];
		double kr = 0.3;
		double kf = 10;
		if((b.GetPosition()).DotProduct(normal)<(eps-1.0+b.GetRadius())&&b.GetVelocity().DotProduct(normal)<0){
			Vector3d temp = b.GetVelocity();
			temp.y = temp.y * kr * (-1);
			b.SetVelocity(temp);
			//cout << "YOOO" << endl;
		}

		if ((b.GetPosition()).DotProduct(normal)<(eps -1.0 +b.GetRadius())){

			if (b.GetVelocity().DotProduct(normal) <= 0.01 && b.GetForce().DotProduct(normal)<0){   // Friction
				Vector3d temp = b.GetVelocity();
				temp.y = 0;
				temp.Normalize();
				b.AddForce(b.GetForce().DotProduct(normal)*(-1)*kf*(-1)*temp);
				//cout << "bbb " << b.GetForce().DotProduct(normal)*(-1)*kf*(-1)*temp << endl;
			}
			
			
			if(b.GetForce().DotProduct(normal)<0){   
				b.AddForce(b.GetForce().DotProduct(normal) * normal * (-1));
				//cout << "aaa Position:" <<b.GetPosition()<< endl;
			}
		
		}
	  
		m_Balls[ballIdx] = b;
	}
	
}

void CMassSpringSystem::BallToBallCollision()
{
    //TO DO
	for (int ballIdx = 0; ballIdx < BallNum(); ++ballIdx)
    {
		Ball b1 = m_Balls[ballIdx];
		for (int ballIdx2 = ballIdx + 1; ballIdx2 <BallNum(); ++ballIdx2)
		{	
			Ball b2 = m_Balls[ballIdx2];
			Vector3d l = (b1.GetPosition() - b2.GetPosition()).NormalizedCopy();
			
			if( (b1.GetPosition()-b2.GetPosition()).Magnitude()<(b1.GetRadius()*2)+0.01&&(b1.GetVelocity()-b2.GetVelocity()).DotProduct(l)<=0){
				Vector3d b1v = b1.GetVelocity();
				Vector3d b2v = b2.GetVelocity();
				Vector3d b1Tob2 = b2.GetPosition() - b1.GetPosition();
				Vector3d b2Tob1 = b1.GetPosition() - b2.GetPosition();
				Vector3d b1vn = b1v.DotProduct(b1Tob2)*b1Tob2.NormalizedCopy();
				Vector3d b1vt = b1v - b1vn;
				Vector3d b2vn = b2v.DotProduct(b2Tob1)*b2Tob1.NormalizedCopy();
				Vector3d b2vt = b2v - b2vn;
				b1.SetVelocity((b1vn*(b1.GetMass()-b2.GetMass())+2*b2.GetMass()*b2vn) / (b1.GetMass()+b2.GetMass()) + b1vt) ;
				b2.SetVelocity((b2vn*(b2.GetMass()-b1.GetMass())+2*b1.GetMass()*b1vn) / (b1.GetMass()+b2.GetMass()) + b2vt) ;

			}
			m_Balls[ballIdx2] = b2;
			m_Balls[ballIdx] = b1;
		}
	}

}

void CMassSpringSystem::BallParticleCollision()
{
    //TO DO
	for (int ballIdx = 0; ballIdx < BallNum(); ++ballIdx)
    {
		Ball b = m_Balls[ballIdx];
		int count = 0;
		for (int pIdx = 0; pIdx < m_GoalNet.ParticleNum(); ++pIdx)
		{	
			CParticle p = m_GoalNet.GetParticle(pIdx);
			Vector3d l = b.GetPosition() - p.GetPosition();
		if (l.Length()<b.GetRadius()+0.1 && (b.GetVelocity()-p.GetVelocity()).DotProduct(l.NormalizedCopy())<=0 ){
				
			
				Vector3d pv = p.GetVelocity();
				Vector3d bv = b.GetVelocity();
				Vector3d bTop = p.GetPosition() - b.GetPosition();
				Vector3d pTob = b.GetPosition() - p.GetPosition();
				Vector3d pvn = pv.DotProduct(pTob)*pTob.NormalizedCopy();
				Vector3d pvt = pv - pvn;
				Vector3d bvn = bv.DotProduct(bTop)*bTop.NormalizedCopy();
				Vector3d bvt = bv - bvn;
				p.SetVelocity((pvn*(p.GetMass() - b.GetMass()) + 2 * b.GetMass()*bvn) / (p.GetMass() + b.GetMass()) + pvt);
				b.SetVelocity((bvn*(b.GetMass() - p.GetMass()) + 2 * p.GetMass()*pvn) / (b.GetMass() + p.GetMass()) + bvt);
				count++;
				m_GoalNet.setParticle(p, pIdx);
			

			}
			
			
		}

		//cout <<"count "<<count<< " ddd " << b.GetVelocity() << endl;
		m_Balls[ballIdx] = b;
		
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Integrator
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMassSpringSystem::Integrate()
{
    if(m_iIntegratorType == CMassSpringSystem::EXPLICIT_EULER)
    {
        ComputeAllForce();
        HandleCollision();
	    ExplicitEuler();
        ResetAllForce();
    }
    else if(m_iIntegratorType == CMassSpringSystem::RUNGE_KUTTA)
    {	
		//ComputeAllForce();
       // HandleCollision();            // 這邊可能不需要?
        RungeKutta();
		//ResetAllForce(); 
    }
    else
    {
        std::cout<<"Error integrator type, use explicit Euler instead!!"<<std::endl;
        ComputeAllForce();
        HandleCollision();
        ExplicitEuler();
        ResetAllForce();
    }
}

void CMassSpringSystem::ExplicitEuler()
{
    //TO DO
	//cout << "YOOO" << endl;
	for (int pIdx = 0; pIdx < m_GoalNet.ParticleNum(); ++pIdx)
    {	
		
		CParticle p =m_GoalNet.GetParticle(pIdx);
		p.SetPosition(p.GetVelocity()*g_cdDeltaT + p.GetPosition());
		p.SetVelocity(p.GetAcceleration()*g_cdDeltaT + p.GetVelocity());

		m_GoalNet.setParticle(p, pIdx);
			
	}
    for (int ballIdx = 0; ballIdx < BallNum(); ++ballIdx)
    {
        Ball b = m_Balls[ballIdx];

		b.SetPosition(b.GetVelocity()*g_cdDeltaT + b.GetPosition());
		b.SetVelocity(b.GetAcceleration()*g_cdDeltaT + b.GetVelocity());
		
		m_Balls[ballIdx] = b;
    }

}

void CMassSpringSystem::RungeKutta()
{
    //TO DO
	
	Vector3d k1p[2000],k1v[2000],k1a,f1;
	Vector3d k2p[2000],k2v[2000],k2a,f2;
	Vector3d k3p[2000],k3v[2000],k3a,f3;
	Vector3d k4p[2000],k4v[2000],k4a,f4;
	Vector3d t0p[2000],t0v[2000],t0a[2000];
	int num = m_GoalNet.ParticleNum();
	int bnum = BallNum();
	//int pIdx = 0;
	
	
	ComputeAllForce();
	HandleCollision();

	
	for (int pIdx = 0; pIdx < num ; ++pIdx)
    {	
		CParticle p = m_GoalNet.GetParticle(pIdx);
		t0a[pIdx] = p.GetForce()/p.GetMass();
		t0v[pIdx] = p.GetVelocity();
		t0p[pIdx] = p.GetPosition();
	}
	for (int pIdx = num; pIdx < bnum +num ; pIdx++){

		Ball b = m_Balls[pIdx-num];
		t0a[pIdx] = b.GetForce() / b.GetMass();
		t0v[pIdx] = b.GetVelocity();
		t0p[pIdx] = b.GetPosition();

	}
	
	
	
	for (int  pIdx = 0; pIdx < m_GoalNet.ParticleNum(); ++pIdx)
    {	
		CParticle p = m_GoalNet.GetParticle(pIdx);
		
		k1p[pIdx] = p.GetVelocity()*g_cdDeltaT;
		k1v[pIdx] = p.GetAcceleration() * g_cdDeltaT;
		p.SetPosition(p.GetVelocity()*g_cdDeltaT*0.5 + p.GetPosition());
		p.SetVelocity(p.GetAcceleration()*g_cdDeltaT*0.5 + p.GetVelocity());
		m_GoalNet.setParticle(p, pIdx);

	}
	
	for (int pIdx = num; pIdx < bnum + num; pIdx++){

		Ball b = m_Balls[pIdx - num];  // 恩亨?
		k1p[pIdx] = b.GetVelocity()*g_cdDeltaT;
		k1v[pIdx] = b.GetAcceleration() * g_cdDeltaT;
		b.SetPosition(b.GetVelocity()*g_cdDeltaT*0.5 + b.GetPosition());
		b.SetVelocity(b.GetAcceleration()*g_cdDeltaT*0.5 + b.GetVelocity());
		m_Balls[pIdx - num] = b;

	}
	
	ResetAllForce();
	
	
	ComputeAllForce();
    HandleCollision();
	for ( int pIdx = 0; pIdx < m_GoalNet.ParticleNum(); ++pIdx)
    {	
		CParticle p = m_GoalNet.GetParticle(pIdx);
		k2p[pIdx] = p.GetVelocity()*g_cdDeltaT;
		k2v[pIdx] = p.GetAcceleration() * g_cdDeltaT;
		p.SetPosition(t0p[pIdx]+0.5*k2p[pIdx]);
		p.SetVelocity(t0v[pIdx]+0.5*k2v[pIdx]);
		
		m_GoalNet.setParticle(p, pIdx);
	}
	
	for (int pIdx = num; pIdx < bnum + num; pIdx++){

		Ball b = m_Balls[pIdx - num];  // 恩亨?
		k2p[pIdx] = b.GetVelocity()*g_cdDeltaT;
		k2v[pIdx] = b.GetAcceleration() * g_cdDeltaT;
		b.SetPosition(t0p[pIdx] + 0.5 *k2p[pIdx]);
		b.SetVelocity(t0v[pIdx] + 0.5*k2v[pIdx]);
		m_Balls[pIdx - num] = b;

	}
	
	ResetAllForce();
	
	
	ComputeAllForce();
	HandleCollision();
	for (int pIdx = 0; pIdx < m_GoalNet.ParticleNum(); ++pIdx)
	{
		CParticle p = m_GoalNet.GetParticle(pIdx);
		k3p[pIdx] = p.GetVelocity()*g_cdDeltaT;
		k3v[pIdx] = p.GetAcceleration() * g_cdDeltaT;
		p.SetPosition(t0p[pIdx]+k3p[pIdx]);
		p.SetVelocity(t0v[pIdx]+k3v[pIdx]);

		m_GoalNet.setParticle(p, pIdx);
	}
	
	for (int pIdx = num; pIdx < bnum + num; pIdx++){

		Ball b = m_Balls[pIdx - num];  // 恩亨?
		k3p[pIdx] = b.GetVelocity()*g_cdDeltaT;
		k3v[pIdx] = b.GetAcceleration() * g_cdDeltaT;
		b.SetPosition(t0p[pIdx] + k3p[pIdx]);
		b.SetVelocity(t0v[pIdx] + k3v[pIdx]);
		m_Balls[pIdx - num] = b;

	}
	
	ResetAllForce();

	
	ComputeAllForce();
	HandleCollision();
	double t = 1 ;
	t /= 6;
	for (int pIdx = 0; pIdx < m_GoalNet.ParticleNum(); ++pIdx)
	{
		CParticle p = m_GoalNet.GetParticle(pIdx);
		k4p[pIdx] = p.GetVelocity()*g_cdDeltaT;
		k4v[pIdx] = p.GetAcceleration() * g_cdDeltaT;
		p.SetPosition(t0p[pIdx] + (t*k1p[pIdx] +2*t*k2p[pIdx]+2*t*k3p[pIdx]+t*k4p[pIdx]));
		p.SetVelocity(t0v[pIdx] + (t*k1v[pIdx] + 2*t*k2v[pIdx] + 2*t*k3v[pIdx] + t*k4v[pIdx]));
		//cout << " qq " << ((1 / 6)*k1v[pIdx] + (2 / 6)*k2v[pIdx] + (2 / 6)*k3v[pIdx] + (1/6)*k4v[pIdx]) << endl;
		//p.SetPosition(p.GetPosition()+k4p[pIdx]);
		//p.SetVelocity(p.GetVelocity()+k4v[pIdx]);
		m_GoalNet.setParticle(p, pIdx);
	}
	
	for (int pIdx = num; pIdx < bnum + num; pIdx++){

		Ball b = m_Balls[pIdx - num];  // 恩亨?
		k4p[pIdx] = b.GetVelocity()*g_cdDeltaT;
		k4v[pIdx] = b.GetAcceleration() * g_cdDeltaT;
		b.SetPosition(t0p[pIdx] + (t*k1p[pIdx] + 2 * t*k2p[pIdx] + 2 * t*k3p[pIdx] + t*k4p[pIdx]));
		b.SetVelocity(t0v[pIdx] + t*k1v[pIdx] + 2 * t*k2v[pIdx] + 2 * t*k3v[pIdx] + t*k4v[pIdx]);
		//b.SetPosition(b.GetPosition() + k4p[pIdx]);
		//b.SetVelocity(b.GetVelocity() + k4v[pIdx]);
		//cout << "qaq " << t*k1v[pIdx] + 2 * t*k2v[pIdx] + 2 * t*k3v[pIdx] + t*k4v[pIdx] << endl;
		m_Balls[pIdx - num] = b;

	}
	ResetAllForce();
	
	
}
