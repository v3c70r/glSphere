//Build a framwork for gl4 which can load shaders
//TODO: 
//[DONE] 1. add archball 
//[DONE] 2. Encapsulate a mesh class
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h> 
#include "tools/include/vsShaderLib.h" 
#include "tools/include/vsMathLib.h"
#include <IL/ilu.h>
#include <IL/il.h>
#include <iostream>
#include <cmath>
#include "Mesh.h"
#include "coordinate.h"
#include "Atmosphere.h"
#define PI 3.141592653f

using namespace std;

//VAO
GLuint vao;

//Variables for archball
GLfloat preMouesPos[2] = {0.0, 0.0};
GLfloat zoom = 0.0f;
bool archBallOn = false;
GLfloat archBallMat[16];
Atmosphere atmosphere;

float ang = 0.0f;
float verticalAng = 0.0f;
float lightR = 1.0f;
float lightDir[4];



//vsml
VSMathLib *vsml = NULL;
VSShaderLib shader;
VSShaderLib groundFromSpace;
VSShaderLib skyFromSpace;
Mesh mesh;
Mesh outerSphere;
Line line;


GLint faceIndex[] = { 0, 1, 3, 1, 2, 3};

void initOpenGL()     //Initiate Function
{
    //set camera position based on its spherical coordinate
    //TODO: Camera location here
    //OpenGL Paramters
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);
    glFrontFace(GL_CW);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    //Light Values
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glBindVertexArray(0);

    //set archBall to identity
    VSMathLib::setIdentityMatrix(archBallMat, 4);
}

void initVSL()
{
    
    vsml = VSMathLib::getInstance();

    vsml->setUniformBlockName("Matrices");
    vsml->setUniformName(VSMathLib::PROJ_VIEW_MODEL, "m_pvm");
    vsml->setUniformName(VSMathLib::NORMAL, "m_normal");
    vsml->setUniformName(VSMathLib::VIEW_MODEL, "m_viewModel");
    vsml->setUniformName(VSMathLib::VIEW, "m_view");
    //Normalize all vectors
    vsml->loadIdentity(VSMathLib::VIEW);
    vsml->loadIdentity(VSMathLib::MODEL);
    vsml->loadIdentity(VSMathLib::PROJECTION);

    vsml->loadIdentity(VSMathLib::NORMAL);    


}

GLuint setupShaders() {
    // Shader for drawing the cube
    float *n = new float[16];
    shader.init();
    shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/helloWorld.vert");
    shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/helloWorld.frag");
 
    // set semantics for the shader variables
    shader.setProgramOutput(0,"outputF");
    shader.setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "position");
    shader.setVertexAttribName(VSShaderLib::NORMAL_ATTRIB, "normal");
 
    shader.prepareProgram();
 
    printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());

    //Init shader for drawing earth
    
    groundFromSpace.init();
    groundFromSpace.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/GroundFromSpace.vert");
    groundFromSpace.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/GroundFromSpace.frag");
 
    // set semantics for the shader variables
    groundFromSpace.setProgramOutput(0,"outputF");
    groundFromSpace.setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "position");
    shader.setVertexAttribName(VSShaderLib::NORMAL_ATTRIB, "normal");
 
    groundFromSpace.prepareProgram();
 
    printf("InfoLog for Ground from Space Shader\n%s\n\n", groundFromSpace.getAllInfoLogs().c_str());

    //Sky from space
    skyFromSpace.init();
    skyFromSpace.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/SkyFromSpace.vert");
    skyFromSpace.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/SkyFromSpace.frag");
 
    // set semantics for the shader variables
    skyFromSpace.setProgramOutput(0,"outputF");
    skyFromSpace.setVertexAttribName(VSShaderLib::VERTEX_COORD_ATTRIB, "position");
    shader.setVertexAttribName(VSShaderLib::NORMAL_ATTRIB, "normal");
 
    skyFromSpace.prepareProgram();
 
    printf("InfoLog for Sky from Space Shader\n%s\n\n", skyFromSpace.getAllInfoLogs().c_str());

    return((shader.isProgramValid() && groundFromSpace.isProgramValid()) && skyFromSpace.isProgramValid());
}

void setupAtmosphere()
{

		atmosphere.m_nSamples = 50;		// Number of sample rays to use in integral equation
		atmosphere.m_fSamples = 50.0;		// Number of sample rays to use in integral equation
		atmosphere.m_Kr = 0.0025f;		// Rayleigh scattering constant
		atmosphere.m_Kr4PI = atmosphere.m_Kr*4.0f*PI;
		atmosphere.m_Km = 0.0015f;		// Mie scattering constant
		atmosphere.m_Km4PI = atmosphere.m_Km*4.0f*PI;
		atmosphere.m_ESun = 15.0f;		// Sun brightness constant
		atmosphere.m_g = -0.95f;		// The Mie phase asymmetry factor

		atmosphere.m_fInnerRadius = 10.0f;
        float m_fInnerRadius2 = atmosphere.m_fInnerRadius *atmosphere.m_fInnerRadius;
		atmosphere.m_fOuterRadius = 10.25f;
        float m_fOuterRadius2 = atmosphere.m_fOuterRadius * atmosphere.m_fOuterRadius;
		atmosphere.m_fScale = 1 / (atmosphere.m_fOuterRadius - atmosphere.m_fInnerRadius);

		atmosphere.m_fWavelength[0] = 0.650f;		// 650 nm for red
		atmosphere.m_fWavelength[1] = 0.570f;		// 570 nm for green
		atmosphere.m_fWavelength[2] = 0.475f;		// 475 nm for blue
		atmosphere.m_fWavelength4[0] = 1/powf(atmosphere.m_fWavelength[0], 4.0f);
		atmosphere.m_fWavelength4[1] = 1/powf(atmosphere.m_fWavelength[1], 4.0f);
		atmosphere.m_fWavelength4[2] = 1/powf(atmosphere.m_fWavelength[2], 4.0f);


		atmosphere.m_fRayleighScaleDepth = 0.25f;
		atmosphere.m_fMieScaleDepth = 0.1f;

        float fKrESun = atmosphere.m_Kr * atmosphere.m_ESun;
        float fKmESun = atmosphere.m_Km * atmosphere.m_ESun;
        float fScaleOverScaleDepth = 1.0/(atmosphere.m_fOuterRadius - atmosphere.m_fInnerRadius) / atmosphere.m_fRayleighScaleDepth;

        float g2 = atmosphere.m_g * atmosphere.m_g;
        skyFromSpace.setBlockUniform("Stmosphere", "fg", &(atmosphere.m_g));
        skyFromSpace.setBlockUniform("Stmosphere", "fg2", &(g2));

        groundFromSpace.setBlockUniform("Atmosphere", "fOuterRadius", &(atmosphere.m_fOuterRadius));
        groundFromSpace.setBlockUniform("Atmosphere", "fOuterRadius2", &m_fInnerRadius2);
        groundFromSpace.setBlockUniform("Atmosphere", "fInnerRadius", &(atmosphere.m_fInnerRadius));
        groundFromSpace.setBlockUniform("Atmosphere", "fInnerRadius2", &m_fOuterRadius2);
        groundFromSpace.setBlockUniform("Atmosphere", "fKrESun", &fKrESun);
        groundFromSpace.setBlockUniform("Atmosphere", "fKmESun", &fKmESun);
        groundFromSpace.setBlockUniform("Atmosphere", "fKr4PI", &(atmosphere.m_Kr4PI));
        groundFromSpace.setBlockUniform("Atmosphere", "fKm4PI", &(atmosphere.m_Km4PI));
        groundFromSpace.setBlockUniform("Atmosphere", "fScale", &(atmosphere.m_fScale));
        groundFromSpace.setBlockUniform("Atmosphere", "fScaleDepth", &(atmosphere.m_fRayleighScaleDepth));
        groundFromSpace.setBlockUniform("Atmosphere", "fScaleOverScaleDepth", &(fScaleOverScaleDepth));
        groundFromSpace.setBlockUniform("Atmosphere", "nSamples", &(atmosphere.m_nSamples));
        groundFromSpace.setBlockUniform("Atmosphere", "fSamples", &(atmosphere.m_fSamples));

        float fCameraHeight = 50.0f;
        float fCameraHeight2 = fCameraHeight * fCameraHeight;

        groundFromSpace.setBlockUniform("Atmosphere", "fCameraHeight", &(fCameraHeight));
        groundFromSpace.setBlockUniform("Atmosphere", "fCameraHeight2", &(fCameraHeight2));
        groundFromSpace.setBlockUniform("Atmosphere", "v3InvWavelength", atmosphere.m_fWavelength4);


        skyFromSpace.setBlockUniform("Stmosphere", "fOuterRadius", &(atmosphere.m_fOuterRadius));
        skyFromSpace.setBlockUniform("Stmosphere", "fOuterRadius2", &m_fInnerRadius2);
        skyFromSpace.setBlockUniform("Stmosphere", "fInnerRadius", &(atmosphere.m_fInnerRadius));
        skyFromSpace.setBlockUniform("Stmosphere", "fInnerRadius2", &m_fOuterRadius2);
        skyFromSpace.setBlockUniform("Stmosphere", "fKrESun", &fKrESun);
        skyFromSpace.setBlockUniform("Stmosphere", "fKmESun", &fKmESun);
        skyFromSpace.setBlockUniform("Stmosphere", "fKr4PI", &(atmosphere.m_Kr4PI));
        skyFromSpace.setBlockUniform("Stmosphere", "fKm4PI", &(atmosphere.m_Km4PI));
        skyFromSpace.setBlockUniform("Stmosphere", "fScale", &(atmosphere.m_fScale));
        skyFromSpace.setBlockUniform("Stmosphere", "fScaleDepth", &(atmosphere.m_fRayleighScaleDepth));
        skyFromSpace.setBlockUniform("Stmosphere", "fScaleOverScaleDepth", &(fScaleOverScaleDepth));
        skyFromSpace.setBlockUniform("Stmosphere", "nSamples", &(atmosphere.m_nSamples));
        skyFromSpace.setBlockUniform("Stmosphere", "fSamples", &(atmosphere.m_fSamples));
        skyFromSpace.setBlockUniform("Stmosphere", "fCameraHeight", &(fCameraHeight));
        skyFromSpace.setBlockUniform("Stmosphere", "fCameraHeight2", &(fCameraHeight2));
        skyFromSpace.setBlockUniform("Stmosphere", "v3InvWavelength", atmosphere.m_fWavelength4);



}

void setupLights()
{
    float specular[4] = {1.0, 1.0, 1.0, 1.0};
    float ambient[4] = {0.5, 0.5, 0.5, 1.0};
    float diffuse[4] = {1.0, 1.0, 1.0, 1.0};
    float res[4];
    lightDir[0] = lightR * sin(ang);
    lightDir[1] = lightR * sin(verticalAng);
    lightDir[2] = lightR * cos(ang);
    lightDir[3] = 0.0f;

    


    groundFromSpace.setBlockUniform("Atmosphere", "v3LightPos", lightDir);
    skyFromSpace.setBlockUniform("Stmosphere", "v3LightPos", lightDir);
    /*
    shader.setBlockUniform("DirectionalLight", "l_specular", specular);
    shader.setBlockUniform("DirectionalLight", "l_dir", lightDir);
    shader.setBlockUniform("DirectionalLight", "l_ambient", ambient);
    shader.setBlockUniform("DirectionalLight", "l_diffuse", diffuse);
    */
}

void setupMatierials()
{
    GLfloat diffuse[4] = {0.756, 0.8421, 0.9632, 0.5};
    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat shininess = 128.0;
    groundFromSpace.setBlockUniform("Materials", "diffuse", diffuse);    //Checked
    shader.setBlockUniform("Materials", "specular", specular);    //Checked
    shader.setBlockUniform("Materials", "ambient", ambient);    //Checked
    shader.setBlockUniform("Materials", "shininess", &shininess);    //Checked
}


void display()
    //Function for rendering
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    vsml->loadIdentity(VSMathLib::VIEW);
    vsml->loadIdentity(VSMathLib::MODEL);

    vsml->multMatrix(VSMathLib::MODEL, archBallMat);
    //vsml->multMatrix(VSMathLib::MODEL, archBallMat);
    
    //set camera
    vsml->lookAt(0.0, 0.0, 50.0, 0, 0, 1, 0, 1, 0);

    float Ntemp[9]={0};


    vsml->matricesToGL();

    glUseProgram(groundFromSpace.getProgramIndex());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glFrontFace(GL_CCW);
    mesh.bind();
    mesh.draw();
    mesh.unbind();
    
    glUseProgram(skyFromSpace.getProgramIndex());
    //glPolygonMode(GL_FRONT_AND_BACK, GL_);
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    outerSphere.bind();
    outerSphere.draw();
    outerSphere.unbind();
    glDisable(GL_BLEND);

    /*
    line.bind();
    line.draw();
    line.unbind();
    */
    glUseProgram(0);

    glutSwapBuffers();

}

void mouseMoveEvent(int x, int y)
    //Function for tracking mouse movement
{
    if(archBallOn)
    {
        if(x!=preMouesPos[0]||y!=preMouesPos[1])
        {
            GLfloat accuMat[16];
            int w=glutGet(GLUT_WINDOW_WIDTH);
            int h=glutGet(GLUT_WINDOW_HEIGHT);
            GLfloat lastVec[3],currentVec[3],axis[3];
            GLfloat rotAngleRad;

            VSMathLib::mapScreen2Sphere(preMouesPos[0],h-preMouesPos[1],w,h, lastVec);
            VSMathLib::mapScreen2Sphere(x,h-y,w,h, currentVec);

            VSMathLib::crossProduct(lastVec, currentVec, axis);
            VSMathLib::normalize(axis);
            VSMathLib::normalize(lastVec);
            VSMathLib::normalize(currentVec);

            rotAngleRad=acos(VSMathLib::dotProduct(lastVec, currentVec));


            preMouesPos[0]=x;preMouesPos[1]=y;

            VSMathLib::createRotationMatrix(rotAngleRad, axis[0], axis[1], axis[2], accuMat);
            vsml->multMatrix(archBallMat, accuMat);

            //Debug rotation
            /*
            cout<<"DO ROTATION: "<< rotAngleRad<<endl;
            cout<<"Current Vector: "<<currentVec[0]<<"\t"<<currentVec[1]<<"\t"<<currentVec[2]<<endl;
            cout<<"Last Vector: "<<lastVec[0]<<"\t"<<lastVec[1]<<"\t"<<lastVec[2]<<endl;
            cout<<"Dot production: "<<VSMathLib::dotProduct(lastVec, currentVec)<<endl;
            */

            //glutPostRedisplay();
        }

    }
}        
    

void mouseEvent(int button, int state, int x, int y)
    //Function for tracking mouse event
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            preMouesPos[0] = x;
            preMouesPos[1] = y;
            archBallOn = true;
        }
        else if(state == GLUT_UP)
        {    
            archBallOn = false;
        }            
        //glutPostRedisplay();
    }
    //process wheel to zoom in and zoom out
    //if ( zoom <= -100 && zoom >= -1000)
    {
        /*
        if (state == GLUT_UP && button == GLUT_WHEEL_UP)
        {
            zoom += 3.0f;
        //    glutPostRedisplay();
        }
        if (state == GLUT_UP && button == GLUT_WHEEL_DOWN)
        {
            zoom -= 3.0f;
            //glutPostRedisplay();
        }
        */
    }
}

void changeSize(int w, int h)
    //function to response to size changing of window
{
    float ratio;

    if( h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    ratio = (1.0f * w)/h;
    vsml->loadIdentity(VSMathLib::PROJECTION);
    vsml->perspective(30.00f, ratio, 0.1f, 500.0f);
    //vsml->ortho(-15*ratio, 15*ratio, -15, 15, 0.1f, 500.0f);

}

void timerFunction(int value)       
    //force it to refresh, no need of postRedisplay function any more
{

    //change values here
    
    
    glutPostRedisplay();
    glutTimerFunc(10,timerFunction,1);
}

void handleSpecialKeypress(int key, int x, int y) {
 switch (key) {
 case GLUT_KEY_LEFT:
        ang += 0.1;
  break;
 case GLUT_KEY_RIGHT:
       ang -= 0.1;
  break;
 case GLUT_KEY_UP:
       verticalAng += 0.1;
  break;
 case GLUT_KEY_DOWN:
       verticalAng -= 0.1;
  break;
 }
    setupLights();
}

int main( int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);

    glutInitContextVersion(4, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_DEBUG);



    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("gl4Demo");

            
    //Print contex info
    cout<<"Vendor: "<<glGetString(GL_VENDOR)<<endl;
    cout<<"Version: "<<glGetString(GL_VERSION)<<endl;
    cout<<"GLSL: "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<endl;

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        exit(1);        //Handle the glew error
    }

    if (!GLEW_VERSION_4_2)
        exit(1);

    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutTimerFunc(10, timerFunction, 1);
    

    if (!setupShaders())
        return(1);

    initVSL();
    mesh.init(10.0f);
    outerSphere.init(10.25f);
    line.init();
    initOpenGL();
    setupAtmosphere();
    setupLights();
    setupMatierials();
    glutSpecialFunc(handleSpecialKeypress);

    glutMainLoop();

    return 0;
}


