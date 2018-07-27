#include <iostream>
#include <fstream>
#include <cmath>
#include "CCamera.h"

using std::fstream;
using std::cout;
using std::endl;
using std::string;

const double g_cdPI = 3.14159265358979323846;

CCamera::CCamera()
{
    CameraInit();
}
CCamera::CCamera(const string &r_csCameraInfoFileName)
{
    fstream CameraInfoFile;
    CameraInfoFile.open(r_csCameraInfoFileName.c_str());

    if(CameraInfoFile.is_open())
	{
        //cout<<crsCameraInfoFileName<<" open successfully"<<endl;
        while(!CameraInfoFile.eof())
        {
            string sVarName;
            CameraInfoFile>>sVarName;

            int iVarDim = GetMemberVarDim(sVarName);
            if(iVarDim == 4)
			{
                double rgdValue[4] = {0.0,0.0,0.0,0.0};
                CameraInfoFile>>rgdValue[0]>>rgdValue[1]>>rgdValue[2]>>rgdValue[3];
                //if(CameraInfoFile>>rgdValue[0]>>rgdValue[1]>>rgdValue[2]>>rgdValue[3])
                //{
                //    cout<<"Error!! Check "<<crsCameraInfoFileName<<" "<<sVarName<<endl;
                //    system("pause");
                //    exit(0);
                //}
                
                if(sVarName.compare("ViewPort") == 0)
                {
                    SetViewPort(rgdValue[0],rgdValue[1],rgdValue[2],rgdValue[3]);
                }
                if(sVarName.compare("PerspectiveInfo") == 0)
                {
                    SetPerspectiveInfo(rgdValue[0],rgdValue[1],rgdValue[2],rgdValue[3]);
                }
			}
            else if(iVarDim == 3)
            {
                double rgdValue[3] = {0.0,0.0,0.0};
                CameraInfoFile>>rgdValue[0]>>rgdValue[1]>>rgdValue[2];
                //if(CameraInfoFile>>rgdValue[0]>>rgdValue[1]>>rgdValue[2])
                //{
                //    cout<<"Error!! Check "<<crsCameraInfoFileName<<" "<<sVarName<<endl;
                //    system("pause");
                //    exit(0);
                //}
                
                if(sVarName.compare("EyePos") == 0)
                {
                    SetEyePos(Vector3d(rgdValue[0],rgdValue[1],rgdValue[2]));
                }
                if(sVarName.compare("EyeUpDir") == 0)
                {
                    SetEyeUpDir(Vector3d(rgdValue[0],rgdValue[1],rgdValue[2]));
                }
                if(sVarName.compare("EyeAtPos") == 0)
                {
                    SetEyeAtPos(Vector3d(rgdValue[0],rgdValue[1],rgdValue[2]));
                }
            }
            else
            {
                cout<<sVarName<<" is not a legal variable name!!"<<endl;
                cout<<"Use default value instead."<<endl;
                CameraInit();
                break;
            }
		}
	}
    else
    {
		cout<<r_csCameraInfoFileName<<" does not open correctly"<<endl;
    }

	CameraInfoFile.close();
}
CCamera::CCamera(const CCamera &a_rcCamera)
{
    SetPerspectiveInfo(a_rcCamera.m_dViewAngle,
                       a_rcCamera.m_dAspectRatio,
                       a_rcCamera.m_dNearPlane,
                       a_rcCamera.m_dFarPlane);				
    SetEyePos(a_rcCamera.m_EyePos);
    SetEyeUpDir(a_rcCamera.m_EyeUpDir);
    SetEyeAtPos(a_rcCamera.m_EyePos);
    SetViewPort(a_rcCamera.m_dViewPort[0],
                a_rcCamera.m_dViewPort[1],
                a_rcCamera.m_dViewPort[2],
                a_rcCamera.m_dViewPort[3]);
}
CCamera::~CCamera()
{

}
void CCamera::CameraInit()
{
    SetPerspectiveInfo(45.0f,1.0f,0.0f,1.0f);
    SetEyePos(Vector3d(0.0f,0.0f,1.0f));
    SetEyeUpDir(Vector3d(0.0f,1.0f,0.0f));
    SetEyeAtPos(Vector3d(0.0f,0.0f,0.0f));
    SetViewPort(0.0f,0.0f,100.0f,100.0f);
}
void CCamera::UpdateAspectRatio()
{
    m_dAspectRatio = (double) glutGet(GLUT_WINDOW_WIDTH) / (double) glutGet(GLUT_WINDOW_HEIGHT);
}

int CCamera::GetMemberVarDim(const string &r_csMemberVarName)
{
    if(r_csMemberVarName.compare("EyePos") == 0||
       r_csMemberVarName.compare("EyeUpDir") == 0||
       r_csMemberVarName.compare("EyeAtPos") == 0)
    {
        return 3;
    }
	else if(r_csMemberVarName.compare("ViewPort") == 0||
            r_csMemberVarName.compare("PerspectiveInfo") == 0)
    {
        return 4;
    }
	else
    {
		return 0;
	}
}
void CCamera::ZoomAndRotateToOrigin(const double a_cdZoom,
                                    const double a_cdPhiDeg,
                                    const double a_cdThetaDeg)
{
    //sphere coordinate
    //phi is the angle between the vector projected to XY-plain and X-axis
    //theta is the angle between the vector and Z-axis
    float fPhiRad   = a_cdPhiDeg   * g_cdPI / 180;
    float fThetaRad = a_cdThetaDeg * g_cdPI / 180;

    Vector3d EyePosVector = m_EyePos;
    Vector3d EyeAtPosVector = m_EyeAtPos;
    Vector3d EyeVector = (EyePosVector - EyeAtPosVector)*a_cdZoom;
    Vector3d EyeUpVector = m_EyeUpDir;
    EyeVector.Rotate(fPhiRad,EyeUpVector);
    Vector3d EyeSideVector = EyeUpVector.CrossProduct(EyeVector);
    EyeVector.Rotate(fThetaRad,EyeSideVector);
    EyeUpVector = EyeVector.CrossProduct(EyeSideVector);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(EyeVector.x+EyeAtPosVector.x,EyeVector.y+EyeAtPosVector.y,EyeVector.z+EyeAtPosVector.z,
              m_EyeAtPos.x,m_EyeAtPos.y,m_EyeAtPos.z,
              EyeUpVector.x,EyeUpVector.y,EyeUpVector.z);
    glMatrixMode(GL_MODELVIEW);
}
void CCamera::UseCamera()
{
    // viewing and modeling transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(m_EyePos.x  ,m_EyePos.y  ,m_EyePos.z,
              m_EyeAtPos.x,m_EyeAtPos.y,m_EyeAtPos.z,
              m_EyeUpDir.x,m_EyeUpDir.y,m_EyeUpDir.z);
	
	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluPerspective(m_dViewAngle,
                   m_dAspectRatio,
                   m_dNearPlane,
                   m_dFarPlane);

    glMatrixMode(GL_MODELVIEW);
}

void CCamera::UseCameraWithViewport()
{
    UpdateAspectRatio();
    // viewing and modeling transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(m_EyePos.x  ,m_EyePos.y  ,m_EyePos.z,
              m_EyeAtPos.x,m_EyeAtPos.y,m_EyeAtPos.z,
              m_EyeUpDir.x,m_EyeUpDir.y,m_EyeUpDir.z);
	
	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluPerspective(m_dViewAngle,
                   m_dAspectRatio,
                   m_dNearPlane,
                   m_dFarPlane);

	// viewport transformation
	glViewport(m_dViewPort[0],
               m_dViewPort[1],
               m_dViewPort[2],
               m_dViewPort[3]);

    glMatrixMode(GL_MODELVIEW);
}
void CCamera::UseCameraWithViewportAndPick(const double a_cdX,const double a_cdY,
                                           const double a_cdWidth, const double a_cdHeight)
{
    GLint viewport[4];
    glGetIntegerv (GL_VIEWPORT, viewport);
    // viewing and modeling transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(m_EyePos.x  ,m_EyePos.y  ,m_EyePos.z,
              m_EyeAtPos.x,m_EyeAtPos.y,m_EyeAtPos.z,
              m_EyeUpDir.x,m_EyeUpDir.y,m_EyeUpDir.z);
	
	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPickMatrix ((double) a_cdX, (double) (viewport[3] - a_cdY), a_cdWidth, a_cdHeight, viewport);
    gluPerspective(m_dViewAngle,
                   m_dAspectRatio,
                   m_dNearPlane,
                   m_dFarPlane);
   
    glMatrixMode(GL_MODELVIEW);
    
}