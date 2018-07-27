namespace enControlID
{
    enum
    {
        DRAW_PARTICLE = 0,
        DRAW_STRUCT_SPRING,
        DRAW_SHEAR_SPRING,
        DRAW_BENDING_SPRING,
        DRAW_GOALPOST,
        DRAW_PLANE,
        DRAW_BACKGROUND,
        DRAW_AXIS,
        SPRINGCOEF,
        DAMPERCOEF,
        DELTAT,
        INTEGRATOR,
        OUTPUT_START,
        OUTPUT_PAUSE,
        PAUSE,
        START,
        RESET,
        PARAM_RESET,
        QUIT,
        THROW,
        SIM_PER_FRAME        
    };
}

bool g_bOutputStart = false;

int g_iMainWindow = -1;
int g_iCheckboxDrawAxis = 0;
int g_iCheckboxDrawBackground = 1;
int g_iCheckboxDrawParticle = 1;
int g_iCheckboxDrawPlane = 1;
int g_iCheckboxDrawGoalpost = 1;
int g_iCheckboxDrawSpringStruct = 0;
int g_iCheckboxDrawSpringShear = 0;
int g_iCheckboxDrawSpringBending = 0;

int g_iListboxCurrIntegrator = 0;

int g_iSpinnerSimPerFrame = 5;

float g_dSpinnerSpringCoef = 2500.0;
float g_dSpinnerDamperCoef = 50.0;
float g_dSpinnerDeltaT = 0.001;
float g_dEditboxFPS = 0.0;

std::string g_sStudentID;

GLUI *p_gGlui;

GLUI_Checkbox *g_pCheckboxDrawAxis;
GLUI_Checkbox *g_pCheckboxDrawBackground;
GLUI_Checkbox *g_pCheckboxDrawParticle;
GLUI_Checkbox *g_pCheckboxDrawPlane;
GLUI_Checkbox *g_pCheckboxDrawShadow;
GLUI_Checkbox *g_pCheckboxDrawSpringStruct;
GLUI_Checkbox *g_pCheckboxDrawSpringShear;
GLUI_Checkbox *g_pCheckboxDrawSpringBending;

GLUI_Spinner *g_pSpinnerStiffness;
GLUI_Spinner *g_pSpinnerDamper;
GLUI_Spinner *g_pSpinnerDeltaT;
GLUI_Spinner *g_pSpinnerHeight;
GLUI_Spinner *g_pSpinnerRotate;
GLUI_Spinner *g_pSpinnerSimPerFrame;

GLUI_Listbox *g_pListboxIntegrator;

GLUI_Button *g_pButtonStart;
GLUI_Button *g_pButtonPause;
GLUI_Button *g_pButtonReset;
GLUI_Button *g_pButtonThrow;
GLUI_Button *g_pButtonOutputStart;
GLUI_Button *g_pButtonOutputPause;
GLUI_Button *g_pButtonQuit;
GLUI_Button *g_pButtonParamReset;

void GUIConfigInit()
{
    bool bDrawAxis          = false;
    bool bDrawBackground    = false;
    bool bDrawPlane         = false;
    bool bDrawParticle      = false;
    bool bDrawGoalpost      = false;
    bool bDrawSpringStruct  = false;
    bool bDrawSpringShear   = false;
    bool bDrawSpringBending = false;

    char cStudentID[15]     = "\0";

    ConfigFile configFile;
    configFile.suppressWarnings(1);

    configFile.addOption("DrawAxis",&bDrawAxis);
    configFile.addOption("DrawBackground",&bDrawBackground);
    configFile.addOption("DrawParticle", &bDrawParticle);
    configFile.addOption("DrawPlane",&bDrawPlane);
    configFile.addOption("DrawGoalpost",&bDrawGoalpost);
    configFile.addOption("DrawSpringStructural",&bDrawSpringStruct);
    configFile.addOption("DrawSpringShear",&bDrawSpringShear);
    configFile.addOption("DrawSpringBending",&bDrawSpringBending);
      
    configFile.addOption("IntegratorType",&g_iListboxCurrIntegrator);
    configFile.addOption("SimulationPerFrame",&g_iSpinnerSimPerFrame);

    configFile.addOption("SpringCoef",&g_dSpinnerSpringCoef);
    configFile.addOption("DamperCoef",&g_dSpinnerDamperCoef);
    configFile.addOption("DeltaT",&g_dSpinnerDeltaT);

    configFile.addOption("StudentID",cStudentID);
    
    int code = configFile.parseOptions("Configuration.txt");
    if(code == 1)
    {
        std::cout<<"Error in GUIConfigInit."<<std::endl;
        system("pause");
        exit(0);
    }

    g_iCheckboxDrawParticle      = (bDrawParticle) ? 1 : 0;
    g_iCheckboxDrawSpringStruct  = (bDrawSpringStruct)?1:0;
    g_iCheckboxDrawSpringShear   = (bDrawSpringShear)?1:0;
    g_iCheckboxDrawSpringBending = (bDrawSpringBending)?1:0;
    g_iCheckboxDrawGoalpost      = (bDrawGoalpost)?1:0;
    g_iCheckboxDrawPlane         = (bDrawPlane)?1:0;
    g_iCheckboxDrawBackground    = (bDrawBackground)?1:0;
    g_iCheckboxDrawAxis          = (bDrawAxis)?1:0;
    
    g_sStudentID.assign(cStudentID);
}

void GLUI_Control_CallBack(int a_iControl)
{
    if(a_iControl == enControlID::START)
    {
        g_MassSpringSystem.SetStartSimulation();
        g_pButtonStart->disable();
        g_pButtonPause->enable();
        g_pButtonThrow->enable();
    }
    else if(a_iControl == enControlID::PAUSE)
    {
        g_MassSpringSystem.SetPauseSimulation();
        g_pButtonStart->enable();
        g_pButtonPause->disable();
        g_pButtonThrow->disable();
    }
    else if(a_iControl == enControlID::RESET)
    {
        g_MassSpringSystem.SetPauseSimulation();
        g_MassSpringSystem.Reset();
        g_pButtonStart->enable();
        g_pButtonPause->disable();
        g_pButtonThrow->disable();
    }
    else if(a_iControl == enControlID::DRAW_PARTICLE)
    {
        if (g_iCheckboxDrawParticle == 1)
            g_MassSpringSystem.SetDrawParticle(true);
        if (g_iCheckboxDrawParticle == 0)
            g_MassSpringSystem.SetDrawParticle(false);
    }
    else if (a_iControl == enControlID::DRAW_GOALPOST)
    {
        if (g_iCheckboxDrawGoalpost == 1)
            g_MassSpringSystem.SetDrawGoalpost(true);
        if (g_iCheckboxDrawGoalpost == 0)
            g_MassSpringSystem.SetDrawGoalpost(false);
    }
    else if(a_iControl == enControlID::DRAW_STRUCT_SPRING)
    {
        if(g_iCheckboxDrawSpringStruct == 1)
            g_MassSpringSystem.SetDrawStruct(true);
        if(g_iCheckboxDrawSpringStruct == 0)
            g_MassSpringSystem.SetDrawStruct(false);
    }
    else if(a_iControl == enControlID::DRAW_SHEAR_SPRING)
    {
        if(g_iCheckboxDrawSpringShear == 1)
            g_MassSpringSystem.SetDrawShear(true);
        if(g_iCheckboxDrawSpringShear == 0)
            g_MassSpringSystem.SetDrawShear(false);
    }
    else if(a_iControl == enControlID::DRAW_BENDING_SPRING)
    {
        if(g_iCheckboxDrawSpringBending == 1)
            g_MassSpringSystem.SetDrawBending(true);
        if(g_iCheckboxDrawSpringBending == 0)
            g_MassSpringSystem.SetDrawBending(false);
    }
    else if(a_iControl == enControlID::SPRINGCOEF)
    {
        g_MassSpringSystem.SetPauseSimulation();
        g_MassSpringSystem.Reset();
        g_pButtonStart->enable();
        g_pButtonPause->disable();
        g_pButtonThrow->disable();
        g_MassSpringSystem.SetSpringCoef(g_dSpinnerSpringCoef,CSpring::Type_nStruct);
        g_MassSpringSystem.SetSpringCoef(g_dSpinnerSpringCoef,CSpring::Type_nShear);
        g_MassSpringSystem.SetSpringCoef(g_dSpinnerSpringCoef,CSpring::Type_nBending);
    }
    else if(a_iControl == enControlID::DAMPERCOEF)
    {
        g_MassSpringSystem.SetPauseSimulation();
        g_MassSpringSystem.Reset();
        g_pButtonStart->enable();
        g_pButtonPause->disable();
        g_pButtonThrow->disable();
        g_MassSpringSystem.SetDamperCoef(g_dSpinnerDamperCoef,CSpring::Type_nStruct);
        g_MassSpringSystem.SetDamperCoef(g_dSpinnerDamperCoef,CSpring::Type_nShear);
        g_MassSpringSystem.SetDamperCoef(g_dSpinnerDamperCoef,CSpring::Type_nBending);
    }
    else if(a_iControl == enControlID::DELTAT)
    {
        g_MassSpringSystem.SetPauseSimulation();
        g_MassSpringSystem.Reset();
        g_pButtonStart->enable();
        g_pButtonPause->disable();
        g_pButtonThrow->disable();
        g_MassSpringSystem.SetDeltaT(g_dSpinnerDeltaT);
    }
    else if(a_iControl == enControlID::INTEGRATOR)
    {
        if(g_iListboxCurrIntegrator == 0)
        {
            g_MassSpringSystem.SetPauseSimulation();
            g_MassSpringSystem.Reset();
            g_pButtonStart->enable();
            g_pButtonPause->disable();
            g_pButtonThrow->disable();
            g_MassSpringSystem.SetIntegratorType(CMassSpringSystem::EXPLICIT_EULER);
        }
        else if(g_iListboxCurrIntegrator == 1)
        {
            g_MassSpringSystem.SetPauseSimulation();
            g_MassSpringSystem.Reset();
            g_pButtonStart->enable();
            g_pButtonPause->disable();
            g_pButtonThrow->disable();
            g_MassSpringSystem.SetIntegratorType(CMassSpringSystem::RUNGE_KUTTA);
        }
    }
    else if(a_iControl == enControlID::QUIT)
    {
        exit(0);
    }
    else if(a_iControl == enControlID::THROW)
    {
        g_MassSpringSystem.CreateBall();
    }
    else if(a_iControl == enControlID::SIM_PER_FRAME)
    {
        g_MassSpringSystem.SetPauseSimulation();
        g_MassSpringSystem.Reset();
        g_pButtonStart->enable();
        g_pButtonPause->disable();
        g_pButtonThrow->disable();
    }
    else if(a_iControl == enControlID::OUTPUT_START)
    {
        g_bOutputStart = true;
        g_pButtonOutputStart->disable();
        g_pButtonOutputPause->enable();
        g_pButtonThrow->enable();
    }
    else if(a_iControl == enControlID::OUTPUT_PAUSE)
    {
        g_bOutputStart = false;
        g_pButtonOutputStart->enable();
        g_pButtonOutputPause->disable();
        g_pButtonThrow->disable();
        std::string sCommand = "ImageToVideo ./Temp/ ";
        sCommand.append(g_sStudentID);
        glutSetWindow(g_iMainWindow);
        glutSetWindowTitle("CA_Assignment1_Main    Converting video, please wait a moment......");
        system(sCommand.c_str());
        glutSetWindowTitle("CA_Assignment1_Main    Deleting temp file");
        system("rmdir /S /Q Temp");
        glutSetWindowTitle("CA_Assignment1_Main");
        g_iPictureCounter = 0;
    }
    else if(a_iControl == enControlID::PARAM_RESET)
    {
        g_bOutputStart = false;

        GUIConfigInit();

        g_MassSpringSystem.SetDrawParticle(false);
        g_MassSpringSystem.SetDrawStruct(true);
        g_MassSpringSystem.SetDrawShear(false);
        g_MassSpringSystem.SetDrawBending(false);
        g_MassSpringSystem.SetDrawGoalpost(true);
        g_MassSpringSystem.SetPauseSimulation();
        g_MassSpringSystem.SetSpringCoef(g_dSpinnerSpringCoef,CSpring::Type_nStruct);
        g_MassSpringSystem.SetSpringCoef(g_dSpinnerSpringCoef,CSpring::Type_nShear);
        g_MassSpringSystem.SetSpringCoef(g_dSpinnerSpringCoef,CSpring::Type_nBending);
        g_MassSpringSystem.SetDamperCoef(g_dSpinnerDamperCoef,CSpring::Type_nStruct);
        g_MassSpringSystem.SetDamperCoef(g_dSpinnerDamperCoef,CSpring::Type_nShear);
        g_MassSpringSystem.SetDamperCoef(g_dSpinnerDamperCoef,CSpring::Type_nBending);
        g_MassSpringSystem.SetDeltaT(g_dSpinnerDeltaT);
        g_MassSpringSystem.SetIntegratorType(CMassSpringSystem::EXPLICIT_EULER);
        g_MassSpringSystem.Reset();
        g_pButtonStart->enable();
        g_pButtonPause->disable();
        g_pButtonThrow->disable();
        g_pButtonOutputPause->disable();

        g_iMouseLastPressX      = 0;
        g_iMouseLastPressY      = 0;
        g_iMouseLastPressButton = -1;

        g_dCameraZoom = 1.0;
        g_dLastCameraZoom = 1.0;
        g_dCameraRotatePitchDeg = 0.0;
        g_dCameraRotateYawDeg = 0.0;
        g_dLastCameraRotatePitchDeg = 0.0;
        g_dLastCameraRotateYawDeg = 0.0;
        g_dOrientRotatePitchDeg = 0.0;
        g_dOrientRotateYawDeg = 0.0;
        g_dLastOrientRotatePitchDeg = 0.0;
        g_dLastOrientRotateYawDeg = 0.0;
    }
}
void GLUIInit()
{
    p_gGlui = GLUI_Master.create_glui("CA_Assignment1_Control",0,g_iScreenWidth+25,0);

    GLUI_Panel *pPanel = new GLUI_Panel(p_gGlui, "", GLUI_PANEL_NONE );

    //Information Panel
    GLUI_Panel *pInformationPanel = new GLUI_Panel( pPanel, "Information" );
    pInformationPanel->set_alignment(GLUI_ALIGN_LEFT);
        GLUI_EditText *counter_edittext = new GLUI_EditText(pInformationPanel, "FPS", &g_dEditboxFPS );
        counter_edittext->disable();
    
    //Control Panel
    GLUI_Panel *pContorlPanel = new GLUI_Panel( pPanel, "Simulation Control" );
    pContorlPanel->set_alignment(GLUI_ALIGN_LEFT);
        g_pButtonStart = new GLUI_Button(pContorlPanel, "Start" ,
                                         enControlID::START,GLUI_Control_CallBack);
        g_pButtonPause = new GLUI_Button(pContorlPanel, "Pause" ,
                                         enControlID::PAUSE,GLUI_Control_CallBack);
        g_pButtonPause->disable();
        g_pButtonReset = new GLUI_Button(pContorlPanel, "Reset" ,
                                         enControlID::RESET,GLUI_Control_CallBack);
        g_pSpinnerSimPerFrame = new GLUI_Spinner(pContorlPanel,"Simualtion/Frame",&g_iSpinnerSimPerFrame,
                                                 enControlID::SIM_PER_FRAME,GLUI_Control_CallBack);
        g_pSpinnerSimPerFrame->set_int_limits(1,10);

    //Object Panel
    GLUI_Panel *pObjectPanel = new GLUI_Panel( pPanel, "Object" );
    pObjectPanel->set_alignment(GLUI_ALIGN_LEFT);
        g_pButtonThrow = new GLUI_Button(pObjectPanel, "Throw",
                                         enControlID::THROW, GLUI_Control_CallBack);
        g_pButtonThrow->disable();

    //Render Panel
    GLUI_Panel *pRenderPanel = new GLUI_Panel( pPanel, "Render" );
    pRenderPanel->set_alignment(GLUI_ALIGN_LEFT);
        g_pCheckboxDrawAxis          = new GLUI_Checkbox( pRenderPanel, "DrawAxis" ,&g_iCheckboxDrawAxis ,
                                                           enControlID::DRAW_AXIS,GLUI_Control_CallBack);
        g_pCheckboxDrawBackground    = new GLUI_Checkbox( pRenderPanel, "DrawBackground" ,&g_iCheckboxDrawBackground ,
                                                           enControlID::DRAW_BACKGROUND,GLUI_Control_CallBack);
        g_pCheckboxDrawParticle      = new GLUI_Checkbox( pRenderPanel, "DrawParticle" ,&g_iCheckboxDrawParticle ,
                                                           enControlID::DRAW_PARTICLE,GLUI_Control_CallBack);
        g_pCheckboxDrawPlane         = new GLUI_Checkbox( pRenderPanel, "DrawPlane" ,&g_iCheckboxDrawPlane ,
                                                           enControlID::DRAW_PLANE,GLUI_Control_CallBack);
        g_pCheckboxDrawShadow        = new GLUI_Checkbox( pRenderPanel, "DrawGoalpost" ,&g_iCheckboxDrawGoalpost ,
                                                           enControlID::DRAW_GOALPOST,GLUI_Control_CallBack);
        g_pCheckboxDrawSpringStruct  = new GLUI_Checkbox( pRenderPanel, "DrawSpringStructural" ,&g_iCheckboxDrawSpringStruct ,
                                                           enControlID::DRAW_STRUCT_SPRING,GLUI_Control_CallBack);
        g_pCheckboxDrawSpringShear   = new GLUI_Checkbox( pRenderPanel, "DrawSpringShear" ,&g_iCheckboxDrawSpringShear ,
                                                           enControlID::DRAW_SHEAR_SPRING,GLUI_Control_CallBack);
        g_pCheckboxDrawSpringBending = new GLUI_Checkbox( pRenderPanel, "DrawSpringBending" ,&g_iCheckboxDrawSpringBending ,
                                                           enControlID::DRAW_BENDING_SPRING,GLUI_Control_CallBack);


    //Spring Panel
    GLUI_Panel *pSpringPanel = new GLUI_Panel( pPanel, "Spring" );
    pSpringPanel->set_alignment(GLUI_ALIGN_LEFT);
        g_pSpinnerStiffness = new GLUI_Spinner( pSpringPanel, "Spring Coef." , &g_dSpinnerSpringCoef,
                                                enControlID::SPRINGCOEF,GLUI_Control_CallBack);
        g_pSpinnerStiffness->set_float_limits(0.0,100000.0);
        g_pSpinnerStiffness->set_speed(0.005f);
        g_pSpinnerDamper   = new GLUI_Spinner( pSpringPanel, "Damper Coef." , &g_dSpinnerDamperCoef,
                                                enControlID::DAMPERCOEF,GLUI_Control_CallBack);
        g_pSpinnerDamper->set_float_limits(0.0,10000.0);
        g_pSpinnerDamper->set_speed(0.005f);

    //Integrator Panel
    GLUI_Panel *pIntegratorPanel = new GLUI_Panel( pPanel, "Integrator" );
    pIntegratorPanel->set_alignment(GLUI_ALIGN_LEFT);
        g_pSpinnerDeltaT = new GLUI_Spinner( pIntegratorPanel, "DeltaT" , &g_dSpinnerDeltaT,
                                             enControlID::DELTAT,GLUI_Control_CallBack);
        g_pSpinnerDeltaT->set_float_limits(0.00001,1.0);
        g_pSpinnerDeltaT->set_speed(0.005f);
        char *pcIntegratorList[] = { "Explicit Euler", "Runge Kutta 4th"};
        g_pListboxIntegrator = new GLUI_Listbox( pIntegratorPanel, "Integrator", &g_iListboxCurrIntegrator ,
                                                 enControlID::INTEGRATOR,GLUI_Control_CallBack);
        for(int i=0; i<2; i++ )
            g_pListboxIntegrator->add_item( i, pcIntegratorList[i] );

    //Output Panel
    GLUI_Panel *pOutputPanel = new GLUI_Panel( pPanel, "Output" );
    pOutputPanel->set_alignment(GLUI_ALIGN_LEFT);
        g_pButtonOutputStart = new GLUI_Button(pOutputPanel, "Start Recording" ,
                                               enControlID::OUTPUT_START,GLUI_Control_CallBack);
        g_pButtonOutputPause = new GLUI_Button(pOutputPanel, "Stop Recording" ,
                                               enControlID::OUTPUT_PAUSE,GLUI_Control_CallBack);
        g_pButtonOutputPause->disable();

    //Program Panel
    GLUI_Panel *pProgramPanel = new GLUI_Panel( pPanel, "Program Control" );
    pProgramPanel->set_alignment(GLUI_ALIGN_LEFT);
        g_pButtonParamReset = new GLUI_Button(pProgramPanel, "Parameters Reset" ,
                                              enControlID::PARAM_RESET,GLUI_Control_CallBack);
        g_pButtonQuit = new GLUI_Button(pProgramPanel, "Quit" ,
                                         enControlID::QUIT,GLUI_Control_CallBack);
}