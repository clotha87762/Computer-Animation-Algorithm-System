#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>
#ifdef _MSC_VER
    #include <intrin.h>
#endif
#include "glut.h"
#include "glui.h"
#include "performanceCounter.h"
#include "CCamera.h"
#include "CParticle.h"
#include "CSpring.h"
#include "CMassSpringSystem.h"
#include "CBmp.h"
#include "configFile.h"
#include "Global_Var.h"
#include "Lighting.h"
#include "GUI.h"
#include "glutFunction.h"

#pragma warning(disable:4996)
#pragma comment( lib, "glut32.lib" )

using std::cout;
using std::endl;

void DrawAxis();
void DrawBackground();
void DrawInformation();
void DrawPlane();
void DrawPlaneShadow();
void SavePicture();

int main(int argc,char** argv)
{
    OpenGLInit(argc,argv);
    srand(time(NULL));
	glutMainLoop();
	return 0;
}

void DrawAxis()
{
    glPushAttrib(GL_CURRENT_BIT);
        glPushMatrix();
            glDisable(GL_LIGHTING);
                glLineWidth(2.0f);
                glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
                drawText3D(0.0,0.0,0.0,"o");
                glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
                glBegin(GL_LINES);
                    glVertex3dv(Vector3d::ZERO.val);
                    glVertex3dv((Vector3d::UNIT_X*2.0).val);
                glEnd();
                drawText3D(2.1,0.0,0.0,"x");
                glColor4f( 0.0f, 0.0f, 1.0f, 1.0f );
                glBegin(GL_LINES);
                    glVertex3dv(Vector3d::ZERO.val);
                    glVertex3dv((Vector3d::UNIT_Y*2.0).val);
                glEnd();
                drawText3D(0.0,2.1,0.0,"y");
                glColor4f( 0.0f, 1.0f, 0.0f, 1.0f );
                glBegin(GL_LINES);
                    glVertex3dv(Vector3d::ZERO.val);
                    glVertex3dv((Vector3d::UNIT_Z*2.0).val);
                glEnd();
                drawText3D(0.0,0.0,2.1,"z");
            glEnable(GL_LIGHTING);
        glPopMatrix();  
    glPopAttrib();
}

void DrawInformation()
{
    glPushAttrib(GL_ENABLE_BIT);
    glPushMatrix();
        glDisable ( GL_LIGHTING );  
        glDisable ( GL_DEPTH_TEST );

        glMatrixMode ( GL_PROJECTION );
        glLoadIdentity ();  

        glMatrixMode ( GL_MODELVIEW );
        glLoadIdentity ();
        glScalef ( 2.0/g_iScreenWidth, -2.0/g_iScreenHeight, 1 );		// Setup view (0,0) to (800,600)
        glTranslatef ( -g_iScreenWidth/2.0, -g_iScreenHeight/2, 0.0);

        glColor4f ( 1.0, 1.0, 1.0, 1.0 );

        static const int s_ciTextStartX = 10;
        static const int s_ciTextStartY = 20;
        static const int s_ciTextRowRange = 15;
        static const int s_ciInfoNum = 20;
        std::string sInfo[s_ciInfoNum];
        char cInfoTemp[100] = "\0";

        sInfo[0] = "Information:";
        sInfo[1] = "StudentID:";
        sInfo[1].append(g_sStudentID);
#ifdef _MSC_VER
        // Get extended ids.
        int CPUInfo[4] = {-1};
        __cpuid(CPUInfo, 0x80000000);
        unsigned int nExIds = CPUInfo[0];

        // Get the information associated with each extended ID.
        char CPUBrandString[0x40] = { 0 };
        for( unsigned int i=0x80000000; i<=nExIds; ++i)
        {
            __cpuid(CPUInfo, i);

            // Interpret CPU brand string and cache information.
            if  (i == 0x80000002)
            {
                memcpy( CPUBrandString,
                CPUInfo,
                sizeof(CPUInfo));
            }
            else if( i == 0x80000003 )
            {
                memcpy( CPUBrandString + 16,
                CPUInfo,
                sizeof(CPUInfo));
            }
            else if( i == 0x80000004 )
            {
                memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
            }
        }
        sInfo[2] = "CPU:";
        sInfo[2].append(CPUBrandString);
#endif
        sInfo[3] = "GPU:";
        sInfo[3].append((char*)glGetString(GL_RENDERER));
        sInfo[4] = "OpenGL version:";
        sInfo[4].append((char*)glGetString(GL_VERSION) );
        /*
        sInfo[5] = "Height Offset:";
        sprintf(cInfoTemp, "%f", g_dSpinnerHeight);
        sInfo[5].append(cInfoTemp);
        sInfo[6] = "Rotate Angle :";
        sprintf(cInfoTemp, "%d", g_iSpinnerRotate); 
        sInfo[6].append(cInfoTemp);
        */
        sInfo[5] = "Spring Coef. :";
        sprintf(cInfoTemp, "%f", g_MassSpringSystem.GetSpringCoef(CSpring::Type_nStruct));
        sInfo[5].append(cInfoTemp);
        sInfo[6] = "Damper Coef. :";
        sprintf(cInfoTemp, "%f", g_MassSpringSystem.GetDamperCoef(CSpring::Type_nShear));
        sInfo[6].append(cInfoTemp);
        sInfo[7] = "DeltaT       :";
        sprintf(cInfoTemp, "%f", g_MassSpringSystem.GetDeltaT());
        sInfo[7].append(cInfoTemp);
        sInfo[8] = "Integrator   :Explicit Euler";
        if(g_MassSpringSystem.GetIntegratorType() == 1)
        {
            sInfo[8] = "Integrator   :Runge Kutta 4th";
        }
        if(!g_MassSpringSystem.CheckStable())
        {
            glColor4f ( 1.0f, 0.0f, 0.0f, 1.0f );
            sInfo[9] = "System is unstable!! Please press reset and modify your parameters!!";
        }
        for(int i=0 ; i<s_ciInfoNum ; i++)
        {
            drawText2D (s_ciTextStartX,s_ciTextStartY+i*s_ciTextRowRange,  sInfo[i].c_str() );
        }

    glPopMatrix();
    glPopAttrib();
}

void DrawBackground()
{
    glPushAttrib(GL_CURRENT_BIT);
        glPushMatrix();
            glDisable(GL_LIGHTING);
                glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D,g_uiTextureId[1]);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glBegin(GL_QUADS);
                        glTexCoord2d( 0.0 , 0.0);
                        glVertex3d(-30.0,-30.0,-30.0);
                        glTexCoord2d( 1.0 , 0.0);
                        glVertex3d( 30.0,-30.0,-30.0);
                        glTexCoord2d( 1.0 , 1.0);
                        glVertex3d( 30.0, 30.0,-30.0);
                        glTexCoord2d( 0.0 , 1.0);
                        glVertex3d(-30.0, 30.0,-30.0);
                    glEnd();
                    glBindTexture(GL_TEXTURE_2D,g_uiTextureId[3]);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glBegin(GL_QUADS);
                        glTexCoord2d( 0.0 , 0.0);
                        glVertex3d(-30.0,-30.0,  30.0);
                        glTexCoord2d( 1.0 , 0.0);
                        glVertex3d( 30.0,-30.0,  30.0);
                        glTexCoord2d( 1.0 , 1.0);
                        glVertex3d( 30.0, 30.0,  30.0);
                        glTexCoord2d( 0.0 , 1.0);
                        glVertex3d(-30.0, 30.0,  30.0);
                    glEnd();
                    glBindTexture(GL_TEXTURE_2D,g_uiTextureId[0]);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glBegin(GL_QUADS);
                        glTexCoord2d( 0.0 , 0.0);
                        glVertex3d(-30.0,-30.0, -30.0);
                        glTexCoord2d( 1.0 , 0.0);
                        glVertex3d(-30.0,-30.0,  30.0);
                        glTexCoord2d( 1.0 , 1.0);
                        glVertex3d(-30.0, 30.0,  30.0);
                        glTexCoord2d( 0.0 , 1.0);
                        glVertex3d(-30.0, 30.0, -30.0);
                    glEnd();
                    glBindTexture(GL_TEXTURE_2D,g_uiTextureId[2]);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glBegin(GL_QUADS);
                        glTexCoord2d( 0.0 , 0.0);
                        glVertex3d( 30.0,-30.0, -30.0);
                        glTexCoord2d( 0.0 , 1.0);
                        glVertex3d( 30.0, 30.0, -30.0);
                        glTexCoord2d( 1.0 , 1.0);
                        glVertex3d( 30.0, 30.0,  30.0);
                        glTexCoord2d( 1.0 , 0.0);
                        glVertex3d( 30.0,-30.0,  30.0);
                    glEnd();
                    glBindTexture(GL_TEXTURE_2D,g_uiTextureId[5]);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glBegin(GL_QUADS);
                        glTexCoord2d( 0.0 , 0.0);
                        glVertex3d(-30.0,-30.0, -30.0);
                        glTexCoord2d( 0.0 , 1.0);
                        glVertex3d(-30.0,-30.0,  30.0);
                        glTexCoord2d( 1.0 , 1.0);
                        glVertex3d( 30.0,-30.0,  30.0);
                        glTexCoord2d( 1.0 , 0.0);
                        glVertex3d( 30.0,-30.0, -30.0);
                    glEnd();
                    glBindTexture(GL_TEXTURE_2D,g_uiTextureId[4]);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glBegin(GL_QUADS);
                        glTexCoord2d( 0.0 , 0.0);
                        glVertex3d(-30.0, 30.0, -30.0);
                        glTexCoord2d( 0.0 , 1.0);
                        glVertex3d(-30.0, 30.0,  30.0);
                        glTexCoord2d( 1.0 , 1.0);
                        glVertex3d( 30.0, 30.0,  30.0);
                        glTexCoord2d( 1.0 , 0.0);
                        glVertex3d( 30.0, 30.0, -30.0);
                    glEnd();
                    glBindTexture(GL_TEXTURE_2D,0); 
                glDisable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
        glPopMatrix();  
    glPopAttrib();
}

void DrawPlane()
{
    glPushAttrib(GL_CURRENT_BIT);
        glPushMatrix();
            float fKa[] = { 0.5f, 0.5f, 0.5f, 1.0f };
            float fKd[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            float fKs[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            float fKe[] = { 0.0f, 0.0f, 0.0f, 1.0f };

            glMaterialfv(GL_FRONT, GL_AMBIENT, fKa);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, fKd);
            glMaterialfv(GL_FRONT, GL_SPECULAR, fKs);
            glMaterialfv(GL_FRONT, GL_EMISSION, fKe);
            glMaterialf(GL_FRONT, GL_SHININESS, 120.0f);
                glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, g_uiTextureId[13]);
                    //glBindTexture(GL_TEXTURE_2D, g_uiTextureId[12]);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                    glBegin(GL_QUADS);
                    glNormal3d(0.0, 1.0, 0.0);
                    glTexCoord2f(0.0f, 20.0f);
                    glVertex3f(-30.0, -1.0, 30.0);

                    glNormal3d(0.0, 1.0, 0.0);
                    glTexCoord2d(20.0, 20.0);
                    glVertex3d(-30.0, -1.0, -30.0);

                    glNormal3d(0.0, 1.0, 0.0);
                    glTexCoord2d(20.0, 0.0);
                    glVertex3d(30.0, -1.0, -30.0);

                    glNormal3d(0.0, 1.0, 0.0);
                    glTexCoord2d(0.0, 0.0);
                    glVertex3d(30.0, -1.0, 30.0);
                    glEnd();
                    /*
                    for(int i = 0 ; i<60 ; i++)
                    {
                        for(int j = 0; j<60 ; j++)
                        {
                            glBegin( GL_QUADS );
                                glNormal3d(0.0,1.0,0.0);
                                glTexCoord2d( (double)(i)/60.0 , (double)(j)/60.0);
                                glVertex3d( -30.0+(double)(i), -1.0, -30.0+(double)(j) );

                                glNormal3d(0.0,1.0,0.0);
                                glTexCoord2d( (double)(i+1)/60.0 , (double)(j)/60.0);
                                glVertex3d(  -30.0+(double)(i+1), -1.0, -30.0+(double)(j) );

                                glNormal3d(0.0,1.0,0.0);
                                glTexCoord2d( (double)(i+1)/60.0 , (double)(j+1)/60.0);
                                glVertex3d(  -30.0+(double)(i+1), -1.0,  -30.0+(double)(j+1) );
                                
                                glNormal3d(0.0,1.0,0.0);
                                glTexCoord2d( (double)(i)/60.0 , (double)(j+1)/60.0);
                                glVertex3d( -30.0+(double)(i), -1.0, -30.0+(double)(j+1) );
                            glEnd ();
                        }
                    }
                    */
                    glBindTexture(GL_TEXTURE_2D,0); 
                glDisable(GL_TEXTURE_2D);
        glPopMatrix();  
    glPopAttrib();
}

void DrawPlaneShadow()
{
    glPushAttrib(GL_CURRENT_BIT);
        glPushMatrix();
            float fKa[] = { 0.2f, 0.2f, 0.2f, 1.0f };

            glMaterialfv(GL_FRONT, GL_AMBIENT, fKa);
            for(int i = 0 ; i<60 ; i++)
            {
                for(int j = 0; j<60 ; j++)
                {
                    glBegin( GL_QUADS );
                        glNormal3d(0.0,1.0,0.0);
                        glVertex3d( -30.0+(double)(i), -1.0, -30.0+(double)(j) );

                        glNormal3d(0.0,1.0,0.0);
                        glVertex3d(  -30.0+(double)(i+1), -1.0, -30.0+(double)(j) );

                        glNormal3d(0.0,1.0,0.0);
                        glVertex3d(  -30.0+(double)(i+1), -1.0,  -30.0+(double)(j+1) );
                        
                        glNormal3d(0.0,1.0,0.0);
                        glVertex3d( -30.0+(double)(i), -1.0, -30.0+(double)(j+1) );
                    glEnd ();
                }
            }
        glPopMatrix();  
    glPopAttrib();
}

void SavePicture()
{
    if(g_iPictureCounter<1000)
    {
        if(g_iPictureCounter == 0)
        {
            system("mkdir Temp");
            system("del /Q Temp");
        }
        CBmp Picture;
        Picture.load(g_iScreenWidth,g_iScreenHeight);

        glReadPixels(0,0,Picture.w,Picture.h,GL_RGB,GL_UNSIGNED_BYTE,Picture.rgb);

        static const char *s_psFILE_NAME = "Image%03d.bmp";
        std::string PictureName;
        PictureName.assign("./Temp/");
        PictureName.append(s_psFILE_NAME);

        static const int g_lPICTURE_FILE_NAME_LENGTH = 64;
        char g_sPictureName[g_lPICTURE_FILE_NAME_LENGTH] = "\0";

        sprintf(g_sPictureName, PictureName.c_str(), g_iPictureCounter);

        Picture.save(g_sPictureName);
        ++g_iPictureCounter;
    }
    else
    {
        GLUI_Control_CallBack(enControlID::OUTPUT_PAUSE);
    }
}

void display()
{       
    g_PerformanceCounter.StartCounter();

    for(int i=0 ; i<g_iSpinnerSimPerFrame ; i++)
    {
        g_MassSpringSystem.SimulationOneTimeStep();
    }
    if(!g_MassSpringSystem.CheckStable())
    {
        GLUI_Control_CallBack(enControlID::PAUSE);
        if (g_bOutputStart == true)
        {
            GLUI_Control_CallBack(enControlID::OUTPUT_PAUSE);
        }
    }
    glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glClearStencil(0);
	glClearAccum(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glMatrixMode(GL_MODELVIEW);
	glPushAttrib(GL_ENABLE_BIT);

    g_Camera.UseCameraWithViewport();
    g_Camera.ZoomAndRotateToOrigin(g_dCameraZoom,g_dCameraRotateYawDeg,g_dCameraRotatePitchDeg);
    
    if(g_iCheckboxDrawPlane == 1)
    {
        lighting();
        g_MassSpringSystem.Draw();
        DrawPlane();
    }
    else
    {
        lighting();
        g_MassSpringSystem.Draw();
    }
    if(g_iCheckboxDrawBackground == 1)
    {
        DrawBackground();
    }

    if(g_iCheckboxDrawAxis == 1)
    {
        DrawAxis();
    }

    DrawInformation();

    if(g_bOutputStart && g_MassSpringSystem.IsSimulation())
    {
        SavePicture();
    }
    
    glutSwapBuffers();

    g_PerformanceCounter.StopCounter();
    g_dEditboxFPS = 1.0 / g_PerformanceCounter.GetElapsedTime();
    p_gGlui->sync_live();

    glPopAttrib();
    glutPostRedisplay();
}