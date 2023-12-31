﻿#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
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
struct coord {
	float x = 0;
	float y = 0;
	float z = 0;
	float r = 0;
	float g = 0;
	float b = 0;
	float xn = 0;
	float yn = 0;
	float zn = 0;
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
float Light_X = -0.8f;
float Light_Y = 0.0f;
float Light_Z = -0.8f;
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
unsigned int tetradices[] = {
	0,2,1,0,3,2,
	0,1,4,
	1,2,4,
	2,3,4,
	3,0,4
};
unsigned int tetralinedice[] = {
	0,2,2,1,1,0,
	0,3,3,2,2,0,
	0,1,1,4,4,0,
	1,2,2,4,4,1,
	2,3,3,4,4,2,
	3,0,0,4,4,3
};
void specialKeys(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void rotation(int val);
void light_rot(int val);
void drawmidline();
void initbuffer();
void drawcube();
void drawtetra();
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
int drawpoly = 0;
int rotating_mode = 0;
bool already_run = false;
bool cullface = true;
int drawmode = 0;
float Xmove = 0;
float Ymove = 0;
float Zmove = 0;
float light_theta = 0;
int light_on = 0;
void main(int argc, char** argv) {
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 15");
	glewExperimental = GL_TRUE;
	glewInit();
	initbuffer();
	makeshape();
	make_shaderProgram();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutMainLoop();
}
void specialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		Xmove -= 0.01;
	}
	if (key == GLUT_KEY_RIGHT) {
		Xmove += 0.01;
	}
	if (key == GLUT_KEY_UP) {
		Ymove += 0.01;
	}
	if (key == GLUT_KEY_DOWN) {
		Ymove -= 0.01;
	}
	glutPostRedisplay();
}
bool light_spin = false;
void keyboard(unsigned char key, int x, int y) {
	if (key == 'X') {
		if (!already_run) {
			rotating_mode = 1;
			glutTimerFunc(30, rotation, 0);
			already_run = true;
		}
		else if (already_run) {
			rotating_mode = 1;
		}
	}
	if (key == 'x') {
		if (!already_run) {
			rotating_mode = 0;
			glutTimerFunc(30, rotation, 0);
			already_run = true;
		}
		else if (already_run) {
			rotating_mode = 0;
		}
	}
	if (key == 'Y') {
		if (!already_run) {
			rotating_mode = 2;
			glutTimerFunc(30, rotation, 0);
			already_run = true;
		}
		else if (already_run) {
			rotating_mode = 2;
		}
	}
	if (key == 'y') {
		if (!already_run) {
			rotating_mode = 3;
			glutTimerFunc(30, rotation, 0);
			already_run = true;
		}
		else if (already_run) {
			rotating_mode = 3;
		}
	}
	if (key == 'h') {
		if (cullface) {
			cullface = false;
		}
		else {
			cullface = true;
		}
	}
	if (key == 'w') {
		drawmode = 1;
	}
	if (key == 'W') {
		drawmode = 0;
	}
	if (key == 's') {
		rotating_mode = -1;
		already_run = false;
		Xmove = 0;
		Ymove = 0;
		Zmove = 0;
	}
	if (key == 'r') {
		if (!light_spin) {
			light_spin = true;
			glutTimerFunc(30, light_rot, 0);
		}
		else {
			light_spin = false;
		}
	}
	if (key == 'z') {
		Light_Z += 0.01f;
	}
	if (key == 'Z') {
		Light_Z -= 0.01f;
	}
	if (key == 'm') {
		if (light_on == 0) {
			light_on = 1;
		}
		else {
			light_on = 0;
		}
	}
	if (key == 'n') {
		if (!drawpoly) {
			drawpoly = 1;
		}
		else {
			drawpoly = 0;
		}
	}
	if (key == 'q' || key == 'Q'||key == 27) {
		exit(0);
	}
	glutPostRedisplay();
}
void rotation(int val) {
	if (rotating_mode == 0) {
		cube_Xtheta -= 1;
	}
	else if (rotating_mode == 1) {
		cube_Xtheta += 1;
	}
	else if (rotating_mode == 2) {
		cube_Ytheta += 1;
	}
	else if (rotating_mode == 3) {
		cube_Ytheta -= 1;
	}

	glutTimerFunc(30, rotation, 0);
	glutPostRedisplay();
}
void light_rot(int val) {
	if (light_spin) {
		light_theta++;
		glutTimerFunc(30, light_rot, 0);
	}
	glutPostRedisplay();
}
void light_effect() {
	//조명효과 적용여부
	GLuint BoolUniformLocation = glGetUniformLocation(shaderProgramID, "light_Mode");
	glUniform1i(BoolUniformLocation, light_on);
	//디퓨즈 조명 위치
	glm::vec3 lightPosition(Light_X, Light_Y, Light_Z);
	GLuint lightPositionUniformLocation = glGetUniformLocation(shaderProgramID, "lightPosition");
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);
	//디퓨즈 조명 회전 결정
	GLuint lightMatrixLocation = glGetUniformLocation(shaderProgramID, "lightMatrix");
	glm::mat4 lightMatrix = glm::mat4(1.0f);
	lightMatrix = glm::rotate(lightMatrix, glm::radians(light_theta), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(lightMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightMatrix));
}
GLvoid drawScene() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	light_effect();
	if (cullface) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
	drawmidline();
	if (!drawpoly) {
		drawcube();
	}
	else {
		drawtetra();
	}
	glutSwapBuffers();
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
		glBufferData(GL_ARRAY_BUFFER, 2 * 9 * sizeof(float), midline[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glDrawArrays(GL_LINES, 0, 2);
	}
}
void drawcube() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	modelMatrix *= scaleMatrix;

	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix *= rotationMatrix;

	translationMatrix = glm::translate(translationMatrix, glm::vec3(Xmove, Ymove, Zmove));
	modelMatrix *= translationMatrix;

	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Xtheta), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix *= rotationMatrix;

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	if (drawmode == 0) {
		glBindBuffer(GL_ARRAY_BUFFER, cubevbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(float), cube, GL_STATIC_DRAW);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubedices), cubedices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
	else if (drawmode == 1) {
		glLineWidth(2.0f);
		glBindBuffer(GL_ARRAY_BUFFER, cubevbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(float), cube, GL_STATIC_DRAW);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubelinedice), cubelinedice, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glDrawElements(GL_LINES, 72, GL_UNSIGNED_INT, 0);

	}
}
void drawtetra() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 translationMatrix = glm::mat4(1.0f);

	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix *=
		modelMatrix *= rotationMatrix;
	translationMatrix = glm::translate(translationMatrix, glm::vec3(Xmove, Ymove, Zmove));
	modelMatrix *= translationMatrix;
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Xtheta), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix *= rotationMatrix;
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.04f, 0.04f, 0.04f));

	modelMatrix *= scaleMatrix;
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	if (drawmode == 0) {
		glBindBuffer(GL_ARRAY_BUFFER, tetravbo);
		glBufferData(GL_ARRAY_BUFFER, 5 * 9 * sizeof(float), tetra, GL_STATIC_DRAW);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetradices), tetradices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	}
	else if (drawmode == 1) {
		glLineWidth(2.0f);
		glBindBuffer(GL_ARRAY_BUFFER, tetravbo);
		glBufferData(GL_ARRAY_BUFFER, 5 * 9 * sizeof(float), tetra, GL_STATIC_DRAW);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetralinedice), tetralinedice, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, 0);

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
			midline[i][j].xn = 0;
			midline[i][j].yn = 0;
			midline[i][j].zn = 0;
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
	tetra[0].xn = -0.334;
	tetra[0].yn = -0.333;
	tetra[0].zn = 0.333;

	tetra[1].x = -3;
	tetra[1].y = -3;
	tetra[1].z = -3;
	tetra[1].xn = -0.334;
	tetra[1].yn = -0.333;
	tetra[1].zn = -0.333;

	tetra[2].x = 3;
	tetra[2].y = -3;
	tetra[2].z = -3;
	tetra[2].xn = 0.334;
	tetra[2].yn = -0.333;
	tetra[2].zn = -0.333;

	tetra[3].x = 3;
	tetra[3].y = -3;
	tetra[3].z = 3;
	tetra[3].xn = 0.334;
	tetra[3].yn = -0.333;
	tetra[3].zn = 0.333;

	tetra[4].x = 0;
	tetra[4].y = 6;
	tetra[4].z = 0;
	tetra[4].xn = 0;
	tetra[4].yn = 1;
	tetra[4].zn = 0;

	float red = static_cast<float>(rand()) / RAND_MAX;
	float green = static_cast<float>(rand()) / RAND_MAX;
	float blue = static_cast<float>(rand()) / RAND_MAX;
	for (int i = 0; i < 5; i++) {
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
	cube[0].xn = -0.334f;
	cube[0].yn = 0.333f;
	cube[0].zn = 0.333f;

	//정면 좌하단
	cube[1].x = -0.5f;
	cube[1].y = -0.5f;
	cube[1].z = 0.5f;
	cube[1].xn = -0.334f;
	cube[1].yn = -0.333f;
	cube[1].zn = 0.333f;

	//정면 우하단
	cube[2].x = 0.5f;
	cube[2].y = -0.5f;
	cube[2].z = 0.5f;
	cube[2].xn = 0.334f;
	cube[2].yn = -0.333f;
	cube[2].zn = 0.333f;
	//정면 우상단
	cube[3].x = 0.5f;
	cube[3].y = 0.5f;
	cube[3].z = 0.5f;
	cube[3].xn = 0.334f;
	cube[3].yn = 0.333f;
	cube[3].zn = 0.333f;
	//후면 우상단
	cube[4].x = 0.5f;
	cube[4].y = 0.5f;
	cube[4].z = -0.5f;
	cube[4].xn = 0.334f;
	cube[4].yn = 0.333f;
	cube[4].zn = -0.333f;

	//후면 우하단
	cube[5].x = 0.5f;
	cube[5].y = -0.5f;
	cube[5].z = -0.5f;
	cube[5].xn = 0.334f;
	cube[5].yn = -0.333f;
	cube[5].zn = -0.333f;

	//후면 좌하단
	cube[6].x = -0.5f;
	cube[6].y = -0.5f;
	cube[6].z = -0.5f;
	cube[6].xn = -0.334f;
	cube[6].yn = -0.333f;
	cube[6].zn = -0.333f;


	//후면 좌상단
	cube[7].x = -0.5f;
	cube[7].y = 0.5f;
	cube[7].z = -0.5f;
	cube[7].xn = -0.334f;
	cube[7].yn = 0.333f;
	cube[7].zn = -0.333f;
	float red = static_cast<float>(rand()) / RAND_MAX;
	float green = static_cast<float>(rand()) / RAND_MAX;
	float blue = static_cast<float>(rand()) / RAND_MAX;
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