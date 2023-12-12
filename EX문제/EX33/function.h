#pragma once
#include <gl/glew.h> 
#include <gl/freeglut.h> 
#include <gl/freeglut_ext.h>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

/*���̴� �Լ�*/
void make_vertexShader();
void make_fragmentShader();
void make_shaderProgram();
char* filetobuf(const char* file);
bool Load_Object(const char* path);
/*�ؽ��� �Լ�*/
void InitTexture();
GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info);
/*ī�޶� �Լ�*/
void CameraTransform();
/*���� �Լ�*/
void LightTransform();
/*�ʱ�ȭ �Լ�*/
void InitObject(); //��ü �ʱ�ȭ
void InitBuffer(); // vao, vbo ����
void Draw(); // �׸���
/*���� ��ȭ �Լ�*/
/*�ݹ� �Լ�*/
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char button, int x, int y);
GLvoid TimerFunction(int value);