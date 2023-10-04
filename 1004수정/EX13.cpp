#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include<time.h>
#include<Windows.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#define WINDOWS_HEIGHT 800
#define WINDOWS_WIDTH 600

GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;

struct coord {
	float x = 0;
	float y = 0;
	float z = 0;
	float r = 0;
	float g = 0;
	float b = 0;
};
//기본 정보
coord line[4][2];
coord midline[2][2];
int indices[4]; // ebo 써서 인덱스 어쩌고 쓰자 

void makeLine();
GLvoid Reshape(int w, int h);
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
bool isclicked = false;
void main(int argc, char** argv) {
	srand(unsigned(time(NULL)));
	makeLine();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 10");
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	glutMouseFunc(mouse);
	glutMotionFunc(mousemove);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}
void makeLine() {
	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			line[i][0].x = 0.5f;
			line[i][0].y = 0.5f;
			line[i][1].x = -0.5f;
			line[i][1].y = 0.5f;
		}
		else if (i == 1) {
			line[i][0].x = -0.5f;
			line[i][0].y = 0.5f;
			line[i][1].x = -0.5f;
			line[i][1].y = -0.5f;
		}
		if (i == 3) {
			line[i][0].x = -0.5f;
			line[i][0].y = -0.5f;
			line[i][1].x = 0.5f;
			line[i][1].y = -0.5f;
		}
		if (i == 4) {
			line[i][0].x = 0.5f;
			line[i][0].y = -0.5f;
			line[i][1].x = 0.5f;
			line[i][1].y = 0.5f;
		}
	}
	midline[0][0].x = 1.0f;
	midline[0][0].y = 0.0f;
	midline[0][1].x = -1.0f;
	midline[0][1].y = 0.0f;

	midline[1][0].x = 0.0f;
	midline[1][0].y = 1.0f;
	midline[1][1].x = 0.0f;
	midline[1][1].y = -1.0f;
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}
}

float normalX;
float normalY;
void mouse(int button, int state, int x, int y) {
	if (GLUT_LEFT_BUTTON && GLUT_DOWN) {
		isclicked = true;
		float normalX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
		float normalY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
	//	if()//클릭 검사
	//	계획) 사분면을 넘어가지 않도록 하자
	//	넘어가도 별로 문제 없?지않나
	// 이것도 쉬울듯 하니 째끼겠다
	}
	else if(GLUT_LEFT_BUTTON && GLUT_UP) {
		isclicked = false;
	}
}
void mousemove(int x, int y) {
	if (isclicked) {
		float mouseX;
		float mouseY;
	}
}
void collapse() {

}

GLvoid drawScene() {

}GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}
void make_shaderProgram() {
	make_vertexShaders();
	make_fragmentShaders();

	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexshader);
	glAttachShader(shaderProgramID, fragmentshader);
	glLinkProgram(shaderProgramID);

	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	glUseProgram(shaderProgramID);
}
void make_vertexShaders() {
	vertexSource = filetobuf("vertex.glsl");
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexSource, 0);
	glCompileShader(vertexshader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		return;
	}
}
void make_fragmentShaders() {
	fragmentSource = filetobuf("fragment.glsl");
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentSource, 0);
	glCompileShader(fragmentshader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		return;
	}
}
char* filetobuf(const char* file) {
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb");
	if (!fptr) {
		return NULL;
	}
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);//끝까지 읽었으니까 다시 돌아가야겠지??
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;
	return buf;
}
