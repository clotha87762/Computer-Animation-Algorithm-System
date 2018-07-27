void *font = GLUT_BITMAP_8_BY_13;
void *fonts[] = {GLUT_BITMAP_9_BY_15,
				 GLUT_BITMAP_TIMES_ROMAN_10,
				 GLUT_BITMAP_TIMES_ROMAN_24};

void drawText2D ( int x, int y,const char* msg);
void drawText3D ( int x, int y , int z ,const char* msg);
void mouse(int a_iButton, int a_iState, int a_iPosX, int a_iPosY);
void motion(int a_iPosX, int a_iPosY);
void OpenGLInit(int argc,char** argv);
void reshape(int iScreenWidth, int iScreenHeight);
void keyboard(unsigned char ucPressedKey, int iX, int iY);
void display();
void TextureInit();

void drawText2D ( int x, int y, const char* msg)
{
    int len, i;
    glRasterPos2f(x, y);
    len = (int) strlen(msg);
    for (i = 0; i < len; i++) 
        glutBitmapCharacter(font, msg[i]);  
}

void drawText3D ( int x, int y , int z ,const char* msg)
{
    int len, i;
    glRasterPos3f(x, y , z);
    len = (int) strlen(msg);
    for (i = 0; i < len; i++) 
        glutBitmapCharacter(font, msg[i]);  
}

void TextureInit()
{    
    CBmp TextureImage[g_ciTexNum];
    TextureImage[0].load("Texture/skybox0.bmp");
    TextureImage[1].load("Texture/skybox1.bmp");
    TextureImage[2].load("Texture/skybox2.bmp");
    TextureImage[3].load("Texture/skybox3.bmp");
    TextureImage[4].load("Texture/skybox4.bmp");
    TextureImage[5].load("Texture/skybox5.bmp");
    TextureImage[6].load("Texture/dice0.bmp");
    TextureImage[7].load("Texture/dice1.bmp");
    TextureImage[8].load("Texture/dice2.bmp");
    TextureImage[9].load("Texture/dice3.bmp");
    TextureImage[10].load("Texture/dice4.bmp");
    TextureImage[11].load("Texture/dice5.bmp");
    TextureImage[12].load("Texture/ground.bmp");
    TextureImage[13].load("Texture/grass.bmp");

    for(int iI = 0 ; iI<g_ciTexNum ; iI++)
    {
        glEnable(GL_TEXTURE_2D);
            glPixelStorei(GL_UNPACK_ALIGNMENT,1);
            glGenTextures(1,&g_uiTextureId[iI]);
            glBindTexture(GL_TEXTURE_2D,g_uiTextureId[iI]);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB , TextureImage[iI].w, TextureImage[iI].h,0,GL_RGB ,GL_UNSIGNED_BYTE, TextureImage[iI].rgb);
            //gluBuild2DMipmaps(GL_TEXTURE_2D, 3 , TextureImage[iI].w, TextureImage[iI].h,GL_RGB ,GL_UNSIGNED_BYTE, TextureImage[iI].rgb);
            glBindTexture(GL_TEXTURE_2D,0);
        glDisable(GL_TEXTURE_2D);
    }
}

void mouse(int a_iButton, int a_iState, int a_iPosX, int a_iPosY)
{
    if(a_iState == GLUT_DOWN)
    {
        g_iMouseLastPressX      = a_iPosX;
        g_iMouseLastPressY      = a_iPosY;
        g_iMouseLastPressButton = a_iButton;
        if(a_iButton == GLUT_RIGHT_BUTTON)
        {
            g_dLastCameraRotatePitchDeg = g_dCameraRotatePitchDeg;
            g_dLastCameraRotateYawDeg = g_dCameraRotateYawDeg;
        }
        if(a_iButton == GLUT_LEFT_BUTTON)
        {
            g_dLastCameraZoom = g_dCameraZoom;
        }
    }
}
void motion(int a_iPosX, int a_iPosY)
{
    if(g_iMouseLastPressButton == GLUT_RIGHT_BUTTON)
    {
        double dDeltaX = (double)( g_iMouseLastPressX - a_iPosX);
        double dDeltaY = (double)( g_iMouseLastPressY - a_iPosY);
        dDeltaX = 360.0f*dDeltaX/300.0f;
        dDeltaY = 360.0f*dDeltaY/300.0f;

        g_dCameraRotateYawDeg = g_dLastCameraRotateYawDeg + dDeltaX;
        g_dCameraRotatePitchDeg = g_dLastCameraRotatePitchDeg + dDeltaY;
        if(g_dCameraRotateYawDeg>179.0)
            g_dCameraRotateYawDeg = 179.0;
        if(g_dCameraRotateYawDeg<-179.0)
            g_dCameraRotateYawDeg = -179.0;
        if(g_dCameraRotatePitchDeg>89.0)
            g_dCameraRotatePitchDeg = 89.0;
        if(g_dCameraRotatePitchDeg<-89.0)
            g_dCameraRotatePitchDeg = -89.0;
    }
    if(g_iMouseLastPressButton == GLUT_LEFT_BUTTON)
    {
        double dDelta = (double)( a_iPosY - g_iMouseLastPressY);
        dDelta/=100.0;
        g_dCameraZoom = g_dLastCameraZoom + dDelta;
        if(g_dCameraZoom<0.5)
            g_dCameraZoom = 0.5;
        if(g_dCameraZoom>2.4)
            g_dCameraZoom = 2.4;
    }
}
void OpenGLInit(int argc,char** argv)
{
	glutInit(&argc, argv);                                    //glut initialization
	glutInitWindowSize(g_iScreenWidth, g_iScreenHeight);      //define window size
	glutInitWindowPosition(0, 0);                             //define window initial position
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH |
                        GLUT_STENCIL | GLUT_ACCUM);           //define display mode

    g_iMainWindow = glutCreateWindow("CA_Assignment1_Main");  //define window name

	glutDisplayFunc(display);                                 //define call back function
    GLUI_Master.set_glutReshapeFunc(reshape);
    GLUI_Master.set_glutKeyboardFunc(keyboard);    
    GLUI_Master.set_glutMouseFunc(mouse);  
    glutMotionFunc(motion);

    //std::cout << "info: " << "GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;
    //std::cout << "info: " << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
    //std::cout << "info: " << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;

    GUIConfigInit();
    GLUIInit();
    TextureInit();
}
void reshape(int iScreenWidth, int iScreenHeight)
{
	g_iScreenWidth  = iScreenWidth;
	g_iScreenHeight = iScreenHeight;

    g_Camera.SetViewPort(0.0,0.0,(double)g_iScreenWidth,(double)g_iScreenHeight);
    g_Camera.SetAspectRatio((double)g_iScreenWidth/(double)g_iScreenHeight);
}
void keyboard(unsigned char ucPressedKey, int iX, int iY)
{
    switch(ucPressedKey)
    {
        case 27:
            exit(0);
    }
    glutPostRedisplay();                          //call display
}