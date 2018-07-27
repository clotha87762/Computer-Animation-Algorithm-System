#ifndef CCAMERA_H
#define CCAMERA_H

#include <string>
#include "glut.h"
#include "Vector3d.h"

#pragma comment( lib, "glut32.lib" )

class CCamera
{	
    private:
        double m_dViewAngle;
        double m_dAspectRatio;
        double m_dNearPlane;
        double m_dFarPlane;

        double m_dViewPort[4];

        Vector3d m_EyePos;
        Vector3d m_EyeUpDir;
        Vector3d m_EyeAtPos;

		void CameraInit();
        void UpdateAspectRatio();
		int GetMemberVarDim(const std::string &a_rsMemberVarName);
    public:
        CCamera();
        CCamera(const std::string &a_rcsCameraInfoFileName);
        CCamera(const CCamera &a_rcCamera);
        ~CCamera();
        void ZoomAndRotateToOrigin( const double a_cdZoom,
                                    const double a_cdPhiDeg,
                                    const double a_cdThetaDeg);
		void UseCamera();
        void UseCameraWithViewport();
        void UseCameraWithViewportAndPick(const double a_cdX,const double a_cdY,
                                          const double a_cdWidth, const double a_cdHeight);

        inline void SetAspectRatio(const double a_cdAspectRatio){m_dAspectRatio = a_cdAspectRatio;}
        inline void SetEyeAtPos(const Vector3d &a_rEyeAtPos){m_EyeAtPos = a_rEyeAtPos;}
        inline void SetEyePos(const Vector3d &a_rEyePos){m_EyePos = a_rEyePos;}
        inline void SetEyeUpDir(const Vector3d &a_rEyeUpDir){m_EyeUpDir = a_rEyeUpDir;}
        inline void SetFarPlane(const double a_cdFarPlane){m_dFarPlane = a_cdFarPlane;}
        inline void SetNearPlane(const double a_cdNearPlane){m_dNearPlane = a_cdNearPlane;}
        inline void SetPerspectiveInfo(const double a_cdViewAngle,
                                       const double a_cdAspectRatio,
                                       const double a_cdNearPlane,
                                       const double a_cdFarPlane){m_dViewAngle = a_cdViewAngle;
                                                                   m_dAspectRatio = a_cdAspectRatio;
                                                                   m_dNearPlane = a_cdNearPlane;
                                                                   m_dFarPlane = a_cdFarPlane;}
        inline void SetViewAngle(const double a_cdViewAngle){m_dViewAngle = a_cdViewAngle;}
        inline void SetViewPort(const double a_cdViewPortUpLeftX,
                                const double a_cdViewPortUpLeftY,
                                const double a_cdViewPortDownRightX,
                                const double a_cdViewPortDownRightY){m_dViewPort[0] = a_cdViewPortUpLeftX;
                                                                      m_dViewPort[1] = a_cdViewPortUpLeftY;
                                                                      m_dViewPort[2] = a_cdViewPortDownRightX;
                                                                      m_dViewPort[3] = a_cdViewPortDownRightY;}
};

#endif