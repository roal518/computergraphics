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
coord mainbox[10];
coord robot_L_arm[8];
coord robot_R_arm[8];
coord robot_L_leg[8];
coord robot_R_leg[8];
coord robot_chest[8];
coord robot_head[8];
coord robot_nose[8];
int box_indices[] = {
	0,1,9,0,9,8,//LF
	2,8,9,2,3,8,//RF
	4,5,6,4,6,7,//BACK
	2,3,4,4,5,2,//R
	0,1,6,0,6,7,//L
	0,3,4,0,4,7,//TOP
	1,6,5,2,1,5//BOT
};
int robot_indices[] = {
	0,1,2,0,2,3,//F
	4,5,6,4,6,7,//BACK
	2,3,4,4,5,2,//R
	0,1,6,0,6,7,//L
	0,3,4,0,4,7,//TOP
	1,6,5,2,1,5//BOT
};
GLuint vao;
GLuint main_Box_vbo;
GLuint main_Box_ebo;

GLuint robot_vbo;
GLuint robot_ebo;

void keyboard(unsigned char key, int x, int y);

void initbuffer();
GLvoid drawScene();
void makeshape();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
float theta_open = 0.0f;
float theta_ex = 0.0f;
void main(int argc, char** argv) {
	//menu();
	makeshape();
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 20");
	glewExperimental = GL_TRUE;
	glewInit();
	initbuffer();
	make_shaderProgram();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(drawScene);
	glutMainLoop();
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 'o') {
		theta_open++;
	}
	if (key == 'y') {
		theta_ex++;
	}
	glutPostRedisplay();
}
void initbuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &main_Box_vbo);
	glGenBuffers(1, &main_Box_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, main_Box_ebo);
}

void viewproj() {
	GLuint projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	GLuint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");

	glm::mat4 view = glm::mat4(1.0f);

	view = glm::rotate(view, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0, -50, -100));

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 400.0f);
	projection = glm::translate(projection, glm::vec3(0.0, -1.0, -200.0));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}
void LF_box() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f + theta_ex), glm::vec3(0.0f, 1.0f, 0.0f));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(-50, 0.0f, 50.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-theta_open), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(50, 0.0f, -50.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}
void RF_box() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f + theta_ex), glm::vec3(0.0f, 1.0f, 0.0f));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(50, 0.0f, 50.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(theta_open), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-50, 0.0f, -50.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(float)));

}
void front_box() {
	LF_box();
	RF_box();
}
void other_box() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f + theta_ex), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void*)(12 * sizeof(float)));

}
void draw_box() {
	glBindBuffer(GL_ARRAY_BUFFER, main_Box_vbo);
	glBufferData(GL_ARRAY_BUFFER, 10 * 6 * sizeof(float), mainbox, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_indices), box_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	front_box();
	other_box();
}
GLvoid drawScene() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	viewproj();
	draw_box();
	glViewport(0, 0, 800, 600);

	glutSwapBuffers();
}
void make_robot_nose() {
	robot_nose[0].x = -0.5f;
	robot_nose[0].y = 0.5f;
	robot_nose[0].z = 0.5f;

	robot_nose[1].x = -0.5f;
	robot_nose[1].y = -0.5f;
	robot_nose[1].z = 0.5f;

	robot_nose[2].x = 0.5f;
	robot_nose[2].y = -0.5f;
	robot_nose[2].z = 0.5f;

	robot_nose[3].x = 0.5f;
	robot_nose[3].y = 0.5f;
	robot_nose[3].z = 0.5f;

	robot_nose[4].x = 0.5f;
	robot_nose[4].y = 0.5f;
	robot_nose[4].z = -0.5f;

	robot_nose[5].x = 0.5f;
	robot_nose[5].y = -0.5f;
	robot_nose[5].z = -0.5f;

	robot_nose[6].x = -0.5f;
	robot_nose[6].y = -0.5f;
	robot_nose[6].z = -0.5f;

	robot_nose[7].x = -3.5f;
	robot_nose[7].y = 3.5f;
	robot_nose[7].z = -3.5f;
	for (int i = 0; i < 8; i++) {
		robot_nose[i].r = 0.7;
		robot_nose[i].g = 0.0;
		robot_nose[i].b = 0.3;
	}
}
void make_robot_head() {
	robot_head[0].x = -3.5f;
	robot_head[0].y = 3.5f;
	robot_head[0].z = 3.5f;

	robot_head[1].x = -3.5f;
	robot_head[1].y = -3.5f;
	robot_head[1].z = 3.5f;

	robot_head[2].x = 3.5f;
	robot_head[2].y = -3.5f;
	robot_head[2].z = 3.5f;

	robot_head[3].x = 3.5f;
	robot_head[3].y = 3.5f;
	robot_head[3].z = 3.5f;

	robot_head[4].x = 3.5f;
	robot_head[4].y = 3.5f;
	robot_head[4].z = -3.5f;

	robot_head[5].x = 3.5f;
	robot_head[5].y = -3.5f;
	robot_head[5].z = -3.5f;

	robot_head[6].x = -3.5f;
	robot_head[6].y = -3.5f;
	robot_head[6].z = -3.5f;

	robot_head[7].x = -3.5f;
	robot_head[7].y = 3.5f;
	robot_head[7].z = -3.5f;
	for (int i = 0; i < 8; i++) {
		robot_head[i].r = 0.3;
		robot_head[i].g = 0.7;
		robot_head[i].b = 0.0;
	}
}
void make_robot_L_arm() {


	robot_L_arm[0].x = -2.5f;
	robot_L_arm[0].y = 5.f;
	robot_L_arm[0].z = 2.5f;

	robot_L_arm[1].x = -2.5f;
	robot_L_arm[1].y = -5.f;
	robot_L_arm[1].z = 2.5f;

	robot_L_arm[2].x = 2.5f;
	robot_L_arm[2].y = -5.f;
	robot_L_arm[2].z = 2.5f;

	robot_L_arm[3].x = 2.5f;
	robot_L_arm[3].y = 5.f;
	robot_L_arm[3].z = 2.5f;

	robot_L_arm[4].x = 2.5f;
	robot_L_arm[4].y = 5.f;
	robot_L_arm[4].z = -2.5f;

	robot_L_arm[5].x = 2.5f;
	robot_L_arm[5].y = -5.f;
	robot_L_arm[5].z = -2.5f;

	robot_L_arm[6].x = -2.5f;
	robot_L_arm[6].y = -5.f;
	robot_L_arm[6].z = -2.5f;

	robot_L_arm[7].x = -2.5f;
	robot_L_arm[7].y = 5.f;
	robot_L_arm[7].z = -2.5f;
	for (int i = 0; i < 8; i++) {
		robot_L_arm[i].r = 0;
		robot_L_arm[i].g = 1;
		robot_L_arm[i].b = 0;
	}
}
void make_robot_R_arm() {

	robot_R_arm[0].x = -2.5f;
	robot_R_arm[0].y = 5.f;
	robot_R_arm[0].z = 2.5f;

	robot_R_arm[1].x = -2.5f;
	robot_R_arm[1].y = -5.f;
	robot_R_arm[1].z = 2.5f;

	robot_R_arm[2].x = 2.5f;
	robot_R_arm[2].y = -5.f;
	robot_R_arm[2].z = 2.5f;

	robot_R_arm[3].x = 2.5f;
	robot_R_arm[3].y = 5.f;
	robot_R_arm[3].z = 2.5f;

	robot_R_arm[4].x = 2.5f;
	robot_R_arm[4].y = 5.f;
	robot_R_arm[4].z = -2.5f;

	robot_R_arm[5].x = 2.5f;
	robot_R_arm[5].y = -5.f;
	robot_R_arm[5].z = -2.5f;

	robot_R_arm[6].x = -2.5f;
	robot_R_arm[6].y = -5.f;
	robot_R_arm[6].z = -2.5f;

	robot_R_arm[7].x = -2.5f;
	robot_R_arm[7].y = 5.f;
	robot_R_arm[7].z = -2.5f;
	for (int i = 0; i < 8; i++) {
		robot_R_arm[i].r = 0;
		robot_R_arm[i].g = 1;
		robot_R_arm[i].b = 0;
	}
}
void make_robot_chest() {

	robot_chest[0].x = -5.f;
	robot_chest[0].y = 7.5f;
	robot_chest[0].z = 5.f;

	robot_chest[1].x = -5.f;
	robot_chest[1].y = -7.5f;
	robot_chest[1].z = 5.f;

	robot_chest[2].x = 5.f;
	robot_chest[2].y = -7.5f;
	robot_chest[2].z = 5.f;

	robot_chest[3].x = 5.f;
	robot_chest[3].y = 7.5f;
	robot_chest[3].z = 5.f;

	robot_chest[4].x = 5.f;
	robot_chest[4].y = 7.5f;
	robot_chest[4].z = -5.f;

	robot_chest[5].x = 5.f;
	robot_chest[5].y = -7.5f;
	robot_chest[5].z = -5.f;

	robot_chest[6].x = -5.f;
	robot_chest[6].y = -7.5f;
	robot_chest[6].z = -5.f;

	robot_chest[7].x = -5.f;
	robot_chest[7].y = 7.5f;
	robot_chest[7].z = -5.f;
	for (int i = 0; i < 8; i++) {
		robot_chest[i].r = 0;
		robot_chest[i].g = 0;
		robot_chest[i].b = 1;
	}
}
void make_robot_R_leg() {

	robot_R_leg[0].x = -2.5f;
	robot_R_leg[0].y = 5.f;
	robot_R_leg[0].z = 2.5f;

	robot_R_leg[1].x = -2.5f;
	robot_R_leg[1].y = -5.f;
	robot_R_leg[1].z = 2.5f;

	robot_R_leg[2].x = 2.5f;
	robot_R_leg[2].y = -5.f;
	robot_R_leg[2].z = 2.5f;

	robot_R_leg[3].x = 2.5f;
	robot_R_leg[3].y = 5.f;
	robot_R_leg[3].z = 2.5f;

	robot_R_leg[4].x = 2.5f;
	robot_R_leg[4].y = 5.f;
	robot_R_leg[4].z = -2.5f;

	robot_R_leg[5].x = 2.5f;
	robot_R_leg[5].y = -5.f;
	robot_R_leg[5].z = -2.5f;

	robot_R_leg[6].x = -2.5f;
	robot_R_leg[6].y = -5.f;
	robot_R_leg[6].z = -2.5f;

	robot_R_leg[7].x = -2.5f;
	robot_R_leg[7].y = 5.f;
	robot_R_leg[7].z = -2.5f;
	for (int i = 0; i < 8; i++) {
		robot_R_leg[i].r = 1;
		robot_R_leg[i].g = 0;
		robot_R_leg[i].b = 0;
	}
}
void make_robot_L_leg() {

	robot_L_leg[0].x = -2.5f;
	robot_L_leg[0].y = 5.f;
	robot_L_leg[0].z = 2.5f;

	robot_L_leg[1].x = -2.5f;
	robot_L_leg[1].y = -5.f;
	robot_L_leg[1].z = 2.5f;

	robot_L_leg[2].x = 2.5f;
	robot_L_leg[2].y = -5.f;
	robot_L_leg[2].z = 2.5f;

	robot_L_leg[3].x = 2.5f;
	robot_L_leg[3].y = 5.f;
	robot_L_leg[3].z = 2.5f;

	robot_L_leg[4].x = 2.5f;
	robot_L_leg[4].y = 5.f;
	robot_L_leg[4].z = -2.5f;

	robot_L_leg[5].x = 2.5f;
	robot_L_leg[5].y = -5.f;
	robot_L_leg[5].z = -2.5f;

	robot_L_leg[6].x = -2.5f;
	robot_L_leg[6].y = -5.f;
	robot_L_leg[6].z = -2.5f;

	robot_L_leg[7].x = -2.5f;
	robot_L_leg[7].y = 5.f;
	robot_L_leg[7].z = -2.5f;
	for (int i = 0; i < 8; i++) {
		robot_L_leg[i].r = 1;
		robot_L_leg[i].g = 0;
		robot_L_leg[i].b = 0;
	}
}
void make_main_box() {
	mainbox[0].x = -50.f;
	mainbox[0].y = 50.f;
	mainbox[0].z = 50.f;
	
	mainbox[1].x = -50.f;
	mainbox[1].y = -50.f;
	mainbox[1].z = 50.f;

	mainbox[2].x = 50.f;
	mainbox[2].y = -50.f;
	mainbox[2].z = 50.f;

	mainbox[3].x = 50.f;
	mainbox[3].y = 50.f;
	mainbox[3].z = 50.f;

	mainbox[4].x = 50.f;
	mainbox[4].y = 50.f;
	mainbox[4].z = -50.f;

	mainbox[5].x = 50.f;
	mainbox[5].y = -50.f;
	mainbox[5].z = -50.f;

	mainbox[6].x = -50.f;
	mainbox[6].y = -50.f;
	mainbox[6].z = -50.f;

	mainbox[7].x = -50.f;
	mainbox[7].y = 50.f;
	mainbox[7].z = -50.f;


	mainbox[8].x = 0.f;
	mainbox[8].z = 50.f;
	mainbox[8].y = 50.f;

	mainbox[9].x = 0.f;
	mainbox[9].z = 50.f;
	mainbox[9].y = -50.f;

	for (int i = 0; i < 10; i++) {
		mainbox[i].r = static_cast<float>(rand()) / RAND_MAX;
		mainbox[i].g = static_cast<float>(rand()) / RAND_MAX;
		mainbox[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
}
void makeshape() {
	make_main_box();
	make_robot_L_leg();
	make_robot_R_leg();
	make_robot_chest();
	make_robot_R_arm();
	make_robot_L_arm();
	make_robot_head();
	make_robot_nose();
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