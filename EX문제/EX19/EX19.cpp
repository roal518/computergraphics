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
GLUquadricObj* sun;
GLUquadricObj* earth;
GLUquadricObj* moon;


GLuint vao; // 삼각형 개수 저장
GLuint earvbo; // 색상,정점 위치 저장
GLuint setvbo;
GLuint midvbo;
GLuint ebo;
coord planet_dot[3][50];
coord satelite_dot[3][50];
int line_index[51];
void planet_road_make();
void Y_spin(int val);
void keyboard(unsigned char key, int x, int y);
void initbuffer();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
float theta_mother = 0.0f;
float theta_son = 0.0f;
float moving_mother_X = 0.0f;
float moving_mother_Y = 0.0f;
float moving_son_X = 0.0f;
float moving_son_Y = 0.0f;
void menu() {
	printf("p/P: 직각 투영/원근 투영\n");
	printf("m/M: 솔리드 모델/와이어 모델\n");
	printf("w/a/s/d: 위의 도형들을 좌/우/상/하로 이동 (x축과 y축 값 이동-> 객체 이동\n");
	printf("+/-: 위의 도형들을 앞/뒤로 이동 (z축 값 이동 - 객체 이동)\n");
	printf("y/Y: 전체 객체들을 y축으로 양/음 방향으로 회전 (중앙의 구의 y축에 대하여 회전)\n");
	printf("z/Z: 중심의 구를 제외하고 행성, 달, 궤도가 z축에 대하여 양/음 방향으로 일제히 회전\n");
}
void main(int argc, char** argv) {
	menu();
	srand(unsigned(time(NULL)));
	planet_road_make();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 18");
	glewExperimental = GL_TRUE;
	glewInit();
	initbuffer();
	make_shaderProgram();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
bool proj_mode = false;
bool yspin = false;
bool y_backspin = false;
void keyboard(unsigned char key, int x, int y) {
	if (key == 'p') {
		if (!proj_mode) {
			proj_mode = true;
		}
		else {
			proj_mode = false;
		}
	}
	if (key == 'y') {
		if (!yspin) {
			yspin = true;
			y_backspin = false;
			glutTimerFunc(30, Y_spin, 0);
		}
		else {
			yspin = false;
			glutTimerFunc(30, Y_spin, 0);
		}
	}
	if (key == 'Y') {
		if (!y_backspin) {
			y_backspin = true;
			yspin = false;
			glutTimerFunc(30, Y_spin, 0);
		}
		else {
			y_backspin = false;
			glutTimerFunc(30, Y_spin, 0);
		}
	}
	glutPostRedisplay();
}
void Y_spin(int val) {
	if (yspin) {
		theta_mother+=3;
		glutTimerFunc(30, Y_spin, 0);
	}
	if (y_backspin) {
		theta_mother-=2;
		glutTimerFunc(30, Y_spin, 0);
	}
	glutPostRedisplay();
}
void initbuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &earvbo);
	glGenBuffers(1, &setvbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	sun = gluNewQuadric();
	gluQuadricDrawStyle(sun, GLU_LINE);
	earth = gluNewQuadric();
	gluQuadricDrawStyle(earth, GLU_LINE);
	moon = gluNewQuadric();
	gluQuadricDrawStyle(moon, GLU_LINE);
}
/// <summary>
/// 궤도를 구 스케치 함수에 종속시키지 말고 독립하자 일단 이동까지만 구현 할 듯:DAY- 10/20
/// </summary>

void viewproj() {
	GLuint projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	GLuint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	if (!proj_mode) {
		projection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	}
	else {
		projection = glm::perspective(glm::radians(45.0f), 0.8f, 0.1f, 150.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -75.0));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	}
}
void sun_road1() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, earvbo);
	glBufferData(GL_ARRAY_BUFFER, 50 * 6 * sizeof(float), planet_dot[0], GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line_index), line_index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_LINE_STRIP, 51, GL_UNSIGNED_INT, 0);
}
void Sun() {
	sun_road1();
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(0.0f ), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(0.0f+theta_mother), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	gluSphere(sun, 5, 20, 20);
}
void Earth() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(theta_mother), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(10.0f, 0.0f, 10.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(theta_mother), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	gluSphere(earth, 3, 20, 20);
	glBindBuffer(GL_ARRAY_BUFFER, setvbo);
	glBufferData(GL_ARRAY_BUFFER, 50 * 6 * sizeof(float), satelite_dot[0], GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line_index), line_index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_LINE_STRIP, 51, GL_UNSIGNED_INT, 0);
}
void Moon() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	
	modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(theta_mother), glm::vec3(0.0f, 1.0f, 0.0f));
	//modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(10.0f, 0.0f, 15.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(theta_mother), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	gluSphere(moon, 1, 20, 20);
}
void Mars() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::rotate(modelMatrix, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-theta_mother), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(10.0f, 0.0f, 10.0f));

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	gluSphere(earth, 1, 20, 20);
	glBindBuffer(GL_ARRAY_BUFFER, setvbo);
	glBufferData(GL_ARRAY_BUFFER, 50 * 6 * sizeof(float), satelite_dot[0], GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line_index), line_index, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_LINE_STRIP, 51, GL_UNSIGNED_INT, 0);

}
GLvoid drawScene() {
	viewproj();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	Sun();
	Earth();
//	Mars();
	Moon();
	glutSwapBuffers();
}
void planet_road_make() {
	for (int i = 0; i < 3; i++) {
		float info_theta = 0.0f;
		float rad = glm::distance(glm::vec3(10, 10, 0), glm::vec3(0, 0, 0));
		for (int j = 0; j < 50; j++) {
			planet_dot[i][j].x = rad * glm::cos(glm::radians(info_theta));
			planet_dot[i][j].z = rad * glm::sin(glm::radians(info_theta));

			planet_dot[i][j].r = 1.0f;
			planet_dot[i][j].g = 1.0f;
			planet_dot[i][j].b = 0.0f;
			info_theta += 7.2f;
		}
		for (int j = 0; j < 50; j++) {
			satelite_dot[i][j].x = 5 * glm::cos(glm::radians(info_theta));
			satelite_dot[i][j].z = 5 * glm::sin(glm::radians(info_theta));

			satelite_dot[i][j].r = 0.0f;
			satelite_dot[i][j].g = 1.0f;
			satelite_dot[i][j].b = 0.0f;
			info_theta += 7.2f;
		}
		printf("in\t%f\n", rad);
	}
	for (int i = 0; i < 50; i++) {
		line_index[i] = i;
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