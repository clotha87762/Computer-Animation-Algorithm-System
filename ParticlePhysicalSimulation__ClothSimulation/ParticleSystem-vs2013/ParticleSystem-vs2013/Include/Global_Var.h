PerformanceCounter g_PerformanceCounter;
CMassSpringSystem g_MassSpringSystem("Configuration.txt");
CCamera g_Camera("camera.txt");

const int g_ciTexNum = 14;

GLuint g_uiTextureId[g_ciTexNum] = {0};

GLint g_iScreenWidth  = 800;
GLint g_iScreenHeight = 600;

int g_iPictureCounter = 0;

int g_iMouseLastPressX      = 0;
int g_iMouseLastPressY      = 0;
int g_iMouseLastPressButton = -1;

double g_dCameraZoom = 1.0;
double g_dLastCameraZoom = 1.0;
double g_dCameraRotatePitchDeg = 0.0;
double g_dCameraRotateYawDeg = 0.0;
double g_dLastCameraRotatePitchDeg = 0.0;
double g_dLastCameraRotateYawDeg = 0.0;
double g_dOrientRotatePitchDeg = 0.0;
double g_dOrientRotateYawDeg = 0.0;
double g_dLastOrientRotatePitchDeg = 0.0;
double g_dLastOrientRotateYawDeg = 0.0;
