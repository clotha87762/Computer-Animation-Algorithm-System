const int g_ciLightNum = 4;

float g_fLightKa[g_ciLightNum][4] = {0.0f, 0.0f, 0.0f, 1.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f};

float g_fLightKd[g_ciLightNum][4] = {1.0f, 1.0f, 1.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 1.0f};

float g_fLightKs[g_ciLightNum][4] = {1.0f, 1.0f, 1.0f, 1.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 1.0f};

float g_fLightPos[g_ciLightNum][4] = { 3.0f, 10.0f, -3.0f, 1.0f,
                                      -3.0f, 10.0f,  3.0f, 1.0f,
                                       3.0f, 10.0f,  3.0f, 1.0f,
                                      -3.0f, 10.0f, -3.0f, 1.0f};

float g_fShadowLightKa[4] = {0.5f, 0.5f, 0.5f, 0.5f};
float g_fShadowLightKd[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float g_fShadowLightKs[4] = {0.0f, 0.0f, 0.0f, 1.0f};

void lighting()
{
    #define LIGHTSETUP(i)\
    glLightfv(GL_LIGHT##i, GL_POSITION, g_fLightPos[##i]);\
    glLightfv(GL_LIGHT##i, GL_AMBIENT, g_fLightKa[##i]);\
    glLightfv(GL_LIGHT##i, GL_DIFFUSE, g_fLightKd[##i]);\
    glLightfv(GL_LIGHT##i, GL_SPECULAR, g_fLightKs[##i]);\
    glEnable(GL_LIGHT##i)

    LIGHTSETUP (0);
    LIGHTSETUP (1);
    //LIGHTSETUP (2);
    //LIGHTSETUP (3);

    // enable lighting
    glEnable(GL_LIGHTING);
}
void lightingShadow()
{
    #define LIGHTSETUPSHADOW(i)\
    glLightfv(GL_LIGHT##i, GL_POSITION, g_fLightPos[##i]);\
    glLightfv(GL_LIGHT##i, GL_AMBIENT, g_fShadowLightKa);\
    glLightfv(GL_LIGHT##i, GL_DIFFUSE, g_fShadowLightKd);\
    glLightfv(GL_LIGHT##i, GL_SPECULAR, g_fShadowLightKs);\
    glEnable(GL_LIGHT##i)

    LIGHTSETUPSHADOW (0);
    //LIGHTSETUPSHADOW (1);
    //LIGHTSETUPSHADOW (2);
    //LIGHTSETUPSHADOW (3);

    // enable lighting
    glEnable(GL_LIGHTING);
}