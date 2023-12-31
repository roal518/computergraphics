﻿
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
GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;
coord midline[3][2];
coord cube[8];
coord tetra[5];
GLuint vao; // 삼각형 개수 저장
GLuint cubevbo; // 색상,정점 위치 저장
GLuint tetravbo;
GLuint midvbo;
GLuint ebo;
float cube_Xtheta = 30.0f;
float cube_Ytheta = 30.0f;
float cube_Ztheta = 0.0f;
unsigned int cubedices[] = {
	0, 1, 2, 0, 2, 3,   // 정면 삼각형
	4, 5, 6, 6, 7, 4,   // 후면 삼각형
	0, 3, 4, 0, 4, 7,   // 상면 삼각형
	1, 6, 5, 5, 2, 1,   // 하면 삼각형
	0, 7, 6, 0, 6, 1,   // 좌측 삼각형
	2, 5, 4, 4, 3, 2    // 우측 삼각형
};
unsigned int cubelinedice[] = {
	0,1,1,2,2,0,
	0,2,2,3,3,0,
	4,5,5,6,6,4,
	6,7,7,4,4,6,
	0,3,3,4,4,0,
	0,4,4,7,7,0,
	1,6,6,5,5,1,
	5,2,2,1,1,5,
	0,7,7,6,6,0,
	0,6,6,1,1,0,
	2,5,5,4,4,2,
	4,3,3,2,2,4
};
void keyboard(unsigned char key, int x, int y);
void rotation(int val);
void drawmidline();
void initbuffer();
void drawcube();
void drawsphere();
void spin(int val);
GLvoid drawScene();
void makeshape();
void make_cube();
void make_tetra();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
GLvoid Reshape(int w, int h);
GLUquadricObj* qobj;
int rotating_mode = 0;
int spin_mode = 0;
bool already_rotating = false;
bool already_spin = false;
int drawing_case = 0;
float Xmove = 2.5;
float Ymove = 0;
float Zmove = 0;
void main(int argc, char** argv) {
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 16");
	glewExperimental = GL_TRUE;
	glewInit();
	initbuffer();
	makeshape();
	make_shaderProgram();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 'X') {
		if (!already_rotating) {
			rotating_mode = 1;
			glutTimerFunc(30, rotation, 0);
			already_rotating = true;
		}
		else if (already_rotating) {
			rotating_mode = 1;
		}
	}
	if (key == 'x') {
		if (!already_rotating) {
			rotating_mode = 0;
			glutTimerFunc(30, rotation, 0);
			already_rotating = true;
		}
		else if (already_rotating) {
			rotating_mode = 0;
		}
	}
	if (key == 'Y') {
		if (!already_rotating) {
			rotating_mode = 2;
			glutTimerFunc(30, rotation, 0);
			already_rotating = true;
		}
		else if (already_rotating) {
			rotating_mode = 2;
		}
	}
	if (key == 'y') {
		if (!already_rotating) {
			rotating_mode = 3;
			glutTimerFunc(30, rotation, 0);
			already_rotating = true;
		}
		else if (already_rotating) {
			rotating_mode = 3;
		}
	}
	if (key == 'r') {
		if (!already_spin) {
			spin_mode = 0;
			glutTimerFunc(30, spin, 0);
			already_spin = true;
		}
		else if (already_spin) {
			spin_mode = 0;
		}
	}
	if (key == 'R') {
		if (!already_spin) {
			spin_mode = 1;
			glutTimerFunc(30, spin, 0);
			already_spin = true;
		}
		else if (already_spin) {
			spin_mode = 1;
		}
	}
	if (key == 'c') {
		if (drawing_case == 0) {
			drawing_case = 1;
		}
		else {
			drawing_case = 0;
		}
	}
	if (key == 's') {
		already_spin = false;
		already_rotating = false;
		rotating_mode = -1;
		spin_mode = -1;
	}
	glutPostRedisplay();
}
float theta = 0;
void spin(int val) {
	if (spin_mode == 0) {
		Xmove = 2.5* (glm::cos(glm::radians(theta)));
		Zmove = 2.5 * (glm::sin(glm::radians(theta)));
		theta += 1;
		glutTimerFunc(30, spin, 0);
	}
	else if (spin_mode == 1) {
		Xmove = 2.5 * (glm::cos(glm::radians(theta)));
		Zmove = 2.5 * (glm::sin(glm::radians(theta)));
		theta -= 1;
		glutTimerFunc(30, spin, 0);
	}
	else {
		return;
	}
	glutPostRedisplay();
}
void rotation(int val) {
	if (rotating_mode == 0) {
		cube_Xtheta -= 1;
		glutTimerFunc(30, rotation, 0);
	}
	else if (rotating_mode == 1) {
		cube_Xtheta += 1;
		glutTimerFunc(30, rotation, 0);
	}
	else if (rotating_mode == 2) {
		cube_Ytheta += 1;
		glutTimerFunc(30, rotation, 0);
	}
	else if (rotating_mode == 3) {
		cube_Ytheta -= 1;
		glutTimerFunc(30, rotation, 0);
	}
	else {
		return;
	}
	glutPostRedisplay();
}
GLvoid drawScene() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	drawmidline();
	drawcube();
	drawsphere();
	glutSwapBuffers();
}
void drawsphere() {
	qobj = gluNewQuadric();

	glLineWidth(2.0f);

	if (!drawing_case) {
		gluQuadricDrawStyle(qobj, GLU_LINE);
		GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		glm::mat4 scaleMatrix = glm::mat4(1.0f);
		glm::mat4 translationMatrix = glm::mat4(1.0f);
		scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
		modelMatrix *= scaleMatrix;

		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix *= rotationMatrix;


		translationMatrix = glm::translate(translationMatrix, glm::vec3(-Xmove, Ymove, -Zmove));
		modelMatrix *= translationMatrix;

		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Xtheta), glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix *= rotationMatrix;

		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		gluSphere(qobj, 0.5, 10, 10);

	}
	else {
		gluQuadricDrawStyle(qobj, GLU_LINE);
		GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		glm::mat4 scaleMatrix = glm::mat4(1.0f);
		glm::mat4 translationMatrix = glm::mat4(1.0f);
		scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
		modelMatrix *= scaleMatrix;

		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix *= rotationMatrix;
		translationMatrix = glm::translate(translationMatrix, glm::vec3(-Xmove, Ymove, -Zmove));
		modelMatrix *= translationMatrix;

		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Xtheta), glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix *= rotationMatrix;

		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		gluCylinder(qobj, 0.5, 0.0, 2.0, 10, 8);
	}
}
void drawcube() {

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_LINE);
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
	modelMatrix *= scaleMatrix;

	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix *= rotationMatrix;


	translationMatrix = glm::translate(translationMatrix, glm::vec3(Xmove, Ymove, Zmove));
	modelMatrix *= translationMatrix;

	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Xtheta), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix *= rotationMatrix;

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));


	if (!drawing_case) {
		glLineWidth(2.0f);
		glBindBuffer(GL_ARRAY_BUFFER, cubevbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), cube, GL_STATIC_DRAW);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubelinedice), cubelinedice, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glDrawElements(GL_LINES, 72, GL_UNSIGNED_INT, 0);
	}
	else {
		gluCylinder(qobj, 0.5,0.5, 2.0, 10, 8);
	}
}
void drawmidline() {
	glLineWidth(2.0f);
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &midvbo);
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix *= rotationMatrix;
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	for (int i = 0; i < 3; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, midvbo);
		glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), midline[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_LINES, 0, 2);
	}
}

void initbuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &cubevbo);
	glGenBuffers(1, &tetravbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}
void makeshape() {
	//0->X 1->Y 2->Z
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			midline[i][j].x = 0;
			midline[i][j].y = 0;
			midline[i][j].z = 0;
		}
		//init 좌표계
	}
	midline[0][0].x = -50.0f;
	midline[0][0].r = 1.0f;
	midline[0][0].g = 0.0f;
	midline[0][0].b = 0.0f;

	midline[0][1].x = 50.0f;
	midline[0][1].r = 1.0f;
	midline[0][1].g = 0.0f;
	midline[0][1].b = 0.0f;

	midline[1][0].y = -50.0f;
	midline[1][0].r = 0.0f;
	midline[1][0].g = 1.0f;
	midline[1][0].b = 0.0f;

	midline[1][1].y = 50.0f;
	midline[1][1].r = 0.0f;
	midline[1][1].g = 1.0f;
	midline[1][1].b = 0.0f;

	midline[2][0].z = -50.0f;
	midline[2][0].r = 0.0f;
	midline[2][0].g = 0.0f;
	midline[2][0].b = 1.0f;

	midline[2][1].z = 50.0f;
	midline[2][1].r = 0.0f;
	midline[2][1].g = 0.0f;
	midline[2][1].b = 1.0f;

	//**정육면체
	//**보는 시점기준**
	//정면
	make_cube();
	//**사각뿔
	//정면h
	make_tetra();
}
void make_tetra() {
	tetra[0].x = -3;
	tetra[0].y = -3;
	tetra[0].z = 3;

	tetra[1].x = -3;
	tetra[1].y = -3;
	tetra[1].z = -3;

	tetra[2].x = 3;
	tetra[2].y = -3;
	tetra[2].z = -3;

	tetra[3].x = 3;
	tetra[3].y = -3;
	tetra[3].z = 3;

	tetra[4].x = 0;
	tetra[4].y = 6;
	tetra[4].z = 0;
	for (int i = 0; i < 4; i++) {
		tetra[i].r = static_cast<float>(rand()) / RAND_MAX;
		tetra[i].g = static_cast<float>(rand()) / RAND_MAX;
		tetra[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
}
void make_cube() {
	//정면 좌상단
	cube[0].x = -0.5f;
	cube[0].y = 0.5f;
	cube[0].z = 0.5f;

	//정면 좌하단
	cube[1].x = -0.5f;
	cube[1].y = -0.5f;
	cube[1].z = 0.5f;

	//정면 우하단
	cube[2].x = 0.5f;
	cube[2].y = -0.5f;
	cube[2].z = 0.5f;

	//정면 우상단
	cube[3].x = 0.5f;
	cube[3].y = 0.5f;
	cube[3].z = 0.5f;

	//후면 우상단
	cube[4].x = 0.5f;
	cube[4].y = 0.5f;
	cube[4].z = -0.5f;

	//후면 우하단
	cube[5].x = 0.5f;
	cube[5].y = -0.5f;
	cube[5].z = -0.5f;

	//후면 좌하단
	cube[6].x = -0.5f;
	cube[6].y = -0.5f;
	cube[6].z = -0.5f;


	//후면 좌상단
	cube[7].x = -0.5f;
	cube[7].y = 0.5f;
	cube[7].z = -0.5f;
	for (int i = 0; i < 8; i++) {
		cube[i].r = static_cast<float>(rand()) / RAND_MAX;
		cube[i].g = static_cast<float>(rand()) / RAND_MAX;
		cube[i].b = static_cast<float>(rand()) / RAND_MAX;
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