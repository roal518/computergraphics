#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include<time.h>
#include<Windows.h>
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm/glm.hpp>
#include <gl/glm/glm/ext.hpp>
#include <gl/glm/glm/gtc/matrix_transform.hpp>

#define WINDOWS_HEIGHT 800
#define WINDOWS_WIDTH 600

struct coord {
	float x = 0;
	float y = 0;
	float z = 0;
	float r = 0;
	float g = 0;
	float b = 0;
};
coord Dot[5][2][75];
unsigned int indices[5][150];
GLuint vao;
GLuint vbo;
GLuint ebo1;
float red = static_cast<float>(rand()) / (RAND_MAX);
float green = static_cast<float>(rand()) / (RAND_MAX);
float blue = static_cast<float>(rand()) / (RAND_MAX);
float normalY, normalX2, normalX1;

int index = 1;
GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;
void Mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);
GLvoid Reshape(int w, int h);
GLvoid drawScene();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

void BezierLineDot();
void InitPointBuffer();
void InitLineBuffer();
void Pointdraw(int val);
void Linedraw(int val);
void Buffer();
int Pointnum = 0;
int Linenum = 0;

bool isgoing = false;
bool ongoing = false;
void main(int argc, char** argv) {
	srand(unsigned(time(NULL)));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 10");
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	glutMouseFunc(Mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
int drawingmode = 0;
void keyboard(unsigned char key, int x, int y) {
	if (key == 'p') {
		drawingmode = 0;
	}
	else if (key == 'l') {
		drawingmode = 1;
	}
	if (key == '1') {
		index = 1;
	}
	else if (key == '2') {
		index = 2;
	}
	else if (key == '3') {
		index = 3;
	}
	else if (key == '4') {
		index = 4;
	}
	else if (key == '5') {
		index = 5;
	}
	else if (key == 27) {
		exit(0);
	}
}
bool isclick = false;
void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		normalX1 = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f - 0.225f;
		normalX2 = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f + 0.225f;
		normalY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
		isclick = true;
		//red = static_cast<float>(rand()) / (RAND_MAX);
		//green = static_cast<float>(rand()) / (RAND_MAX);
		//blue = static_cast<float>(rand()) / (RAND_MAX);
		BezierLineDot();
	}
}
GLvoid drawScene() {
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (isclick) {
		if (drawingmode == 0) {
			InitPointBuffer();
			Buffer();
			if (ongoing == false) {
				ongoing = true;
				glutTimerFunc(30, Pointdraw, 0);
			}
		}
		else if (drawingmode == 1) {
			Buffer();
			if (isgoing == false) {
				isgoing = true;
				glutTimerFunc(30, Linedraw, 0);

			}
		}
	}
	glutSwapBuffers();
}
void Linedraw(int val) {
	if (isgoing) {
		if (Linenum < index * 150) {
			ongoing = false;
			InitLineBuffer();
			glutPostRedisplay();
			glutTimerFunc(30, Linedraw, 0);
			Linenum++;
		}
		else {
			Linenum = 0;
			isgoing = false;
		}
	}
}
void InitPointBuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);

	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position");//0
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");
	GLint uniformLocation = glGetUniformLocation(shaderProgramID, "n");
	int value = 2; // 변수에 설정할 값
	glUniform1i(uniformLocation, value);
	glEnableVertexAttribArray(PosLocation);
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), 0);
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));

	glPointSize(2.0);
	glDrawArrays(GL_POINTS, 0, Pointnum);
	Pointnum++;
}
void InitLineBuffer() {
		glDrawElements(GL_LINE_STRIP, index*Linenum, GL_UNSIGNED_INT, 0);
		
		glDeleteBuffers(1, &ebo1);
}
void Buffer() {
	if (drawingmode == 0) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, index * 150 * 6 * sizeof(float), Dot, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	else if (drawingmode == 1) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, index*150 * 6 * sizeof(float), Dot, GL_STATIC_DRAW);

		for (int i = 0; i < index; i++) {
			glBufferData(GL_ARRAY_BUFFER, 150 * 6 * sizeof(float), Dot[i], GL_STATIC_DRAW);


			glGenBuffers(1, &ebo1);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			InitLineBuffer();

		}
	}
}
void Pointdraw(int val) {
	if (ongoing) {
		if (Pointnum < index * 150) {
			isgoing = false;
			InitPointBuffer();
			glutPostRedisplay();
			glutTimerFunc(30, Pointdraw, 0);
		}
		else {
			Pointnum = 0;
			ongoing = false; 
		}
	}
}
void BezierLineDot() {
	int j = 0;
	float a = 0.01;
	float b = 0.02;
	float theta = 0.0f;
	for (int i = 0; i < index; i++) {
		float x = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
		float x1 = x - 0.225f;
		float x2 = x + 0.225f;
		float y = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
		if (i == 0) {
			for (int n = 0; n < 75; n++) {
				float r = a + b * theta;
				Dot[i][0][n].x = normalX1 + 0.01 * r * (glm::cos(glm::radians(theta)));
				Dot[i][0][n].y = normalY + 0.01 * r * (glm::sin(glm::radians(theta)));
				Dot[i][0][n].z = 0.0f;
				Dot[i][0][n].r = 0.5f;
				Dot[i][0][n].g = 0.0f;
				Dot[i][0][n].b = 0.5f;
				theta += 14.5;
			}
			a = 0.01;
			b = 0.02;
			theta = 0.0f;
			for (int n = 0; n < 75; n++) {
				float r = a + b * theta;
				Dot[i][1][n].x = normalX2 + 0.01 * r * (glm::cos(glm::radians(theta)));
				Dot[i][1][n].y = normalY - 0.01 * r * (glm::sin(glm::radians(theta)));
				Dot[i][1][n].z = 0.0f;
				Dot[i][1][n].r = 0.5f;
				Dot[i][1][n].g = 0.0f;
				Dot[i][1][n].b = 0.5f;
				theta -= 14.5;
			}
		}
		else {
			a = 0.01; 
			b = 0.02;
			theta = 0.0f;
			for (int n = 0; n < 75; n++) {
				float r = a + b * theta;
				Dot[i][0][n].x = x1 + 0.01 * r * (glm::cos(glm::radians(theta)));
				Dot[i][0][n].y = y + 0.01 * r * (glm::sin(glm::radians(theta)));
				Dot[i][0][n].z = 0.0f;
				Dot[i][0][n].r = 0.5f;
				Dot[i][0][n].g = 0.0f;
				Dot[i][0][n].b = 0.5f;
				theta += 14.5;
			}
			a = 0.01;
			b = 0.02;
			theta = 0.0f;
			for (int n = 0; n < 75; n++) {
				float r = a + b * theta;
				Dot[i][1][n].x = x2 + 0.01 * r * (glm::cos(glm::radians(theta)));
				Dot[i][1][n].y = y - 0.01 * r * (glm::sin(glm::radians(theta)));
				Dot[i][1][n].z = 0.0f;
				Dot[i][1][n].r = 0.5f;
				Dot[i][1][n].g = 0.0f;
				Dot[i][1][n].b = 0.5f;
				theta -= 14.5;
			}
		}
		int noj = 149;
		for (int j = 0; j < 150; j++) {
			if (j < 75) {
				indices[i][j] = j;
			}
			else if (j >= 75) {
				indices[i][j] = noj--;
			}
		}
	}
}
GLvoid Reshape(int w, int h) {
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