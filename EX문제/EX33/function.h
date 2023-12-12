#pragma once
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

/*세이더 함수*/
void make_vertexShader();
void make_fragmentShader();
void make_shaderProgram();
char* filetobuf(const char* file);
bool Load_Object(const char* path);
/*텍스쳐 함수*/
void InitTexture();
GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info);
/*카메라 함수*/
void CameraTransform();
/*조명 함수*/
void LightTransform();
/*초기화 함수*/
void InitObject(); //객체 초기화
void InitBuffer(); // vao, vbo 연결
void Draw(); // 그리기
/*상태 변화 함수*/
/*콜백 함수*/
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char button, int x, int y);
GLvoid TimerFunction(int value);