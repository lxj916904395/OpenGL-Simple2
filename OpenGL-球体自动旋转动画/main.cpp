//
//  main.m
//  OpenGL-球体自动旋转动画
//
//  Created by zhongding on 2018/12/11.
//

#include "GLTools.h"
#include "GLFrustum.h"
#include "StopWatch.h"
#include "GLTriangleBatch.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <GL/glut.h>
#endif

GLTriangleBatch triangleBatch;
GLFrustum viewFraustum;

GLShaderManager shaderManager;


void changeSize(int w, int h){
    glViewport(0, 0, w, h);
    //投影矩阵
    viewFraustum.SetPerspective(35.f, float(w)/float(h), 1, 1000);
}

void renderScene(void){
    
    
    static CStopWatch rotateTime ;
    
    //当前时间
    float yRot = rotateTime.GetElapsedSeconds() * 60;
    
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    
    M3DMatrix44f mTranslation,mRotate,mModelView,mModelViewProjection;
   
    
    //往z轴负方向移动
    m3dTranslationMatrix44(mTranslation, 0, 0, -2.5f);
    
    //绕Y轴旋转
    m3dRotationMatrix44(mRotate, m3dDegToRad(yRot), 0.0f, 1.0f, 0.0f);
    
    //平移、旋转矩阵相乘，结果存储在mModelView
    m3dMatrixMultiply44(mModelView, mTranslation, mRotate);
    
    //模型矩阵、投影矩阵相乘，结果存储在mModelViewProjection
    m3dMatrixMultiply44(mModelViewProjection, viewFraustum.GetProjectionMatrix(), mModelView);

    GLfloat vblack[] = {0.0f,0.0f,0.0f,1.0f};
    
    //平面着色器绘制
    shaderManager.UseStockShader(GLT_SHADER_FLAT,mModelViewProjection,vblack);
    
    triangleBatch.Draw();
    
    glutSwapBuffers();
    
    glutPostRedisplay();
}


void setup(){
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    
    shaderManager.InitializeStockShaders();
    
    glEnable(GL_DEPTH_TEST);
    
    
    gltMakeSphere(triangleBatch, 0.4f, 10, 20);
    
    //填充模式：
    //正背面都填充
    //以线的方式填充
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char * argv[]) {
    
    gltSetWorkingDirectory(argv[0]);
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

    glutInitWindowSize(800, 600);
    
    glutCreateWindow("球体自动旋转");
    
    glutReshapeFunc(changeSize);
    glutDisplayFunc(renderScene);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    setup();
    
    glutMainLoop();
    
    return 0;
}
