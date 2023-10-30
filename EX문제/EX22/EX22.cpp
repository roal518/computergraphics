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
};
GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;
coord mainbox[10];
int box_indices[] = {
	0,2,1,0,3,2,//밑면
	4,5,6,4,6,7,//윗면
	0,1,5,0,5,4,//좌측면
//	1,2,5,2,6,5,//정면

	2,9,8,2,8,6,//정면(R)
	1,5,8,1,8,9,//정면(L)
	2,3,7,2,7,6,//우측면
	0,7,3,0,4,7 //후면
};
GLuint vao;
GLuint main_Box_vbo;
GLuint main_Box_ebo;

void initbuffer();
GLvoid drawScene();
void makeshape();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);


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
	glutDisplayFunc(drawScene);
	glutMainLoop();
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

	view = glm::translate(view, glm::vec3(10, 0, 0));
	view = glm::rotate(view, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0, -20.0, -sqrt(101)));
	view = glm::rotate(view, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 400.0f);
	projection = glm::translate(projection, glm::vec3(0.0, -1.0, -100.0));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}
void draw_head() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, main_Box_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), mainbox, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(box_indices), box_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 42, GL_UNSIGNED_INT, 0);
}
GLvoid drawScene() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	viewproj();
	draw_head();
	glViewport(0, 0, 800, 600);

	glutSwapBuffers();
}


void make_main_box() {
	mainbox[0].x = -10.f;
	mainbox[0].z = 5.f;
	mainbox[0].y = 10.f;

	mainbox[1].x = -10.f;
	mainbox[1].z = -5.f;
	mainbox[1].y = 10.f;

	mainbox[2].x = 10.f;
	mainbox[2].z = -5.f;
	mainbox[2].y = 10.f;

	mainbox[3].x = 10.f;
	mainbox[3].z = 5.f;
	mainbox[3].y = 10.f;

	mainbox[4].x = -10.f;
	mainbox[4].z = 5.f;
	mainbox[4].y = 20.f;

	mainbox[5].x = -10.f;
	mainbox[5].z = -5.f;
	mainbox[5].y = 20.f;

	mainbox[6].x = 10.f;
	mainbox[6].z = -5.f;
	mainbox[6].y = 20.f;

	mainbox[7].x = 10.f;
	mainbox[7].z = 5.f;
	mainbox[7].y = 20.f;
	
	mainbox[8].x = 10.f;
	mainbox[8].z = 5.f;
	mainbox[8].y = 20.f;
	
	mainbox[9].x = 10.f;
	mainbox[9].z = 5.f;
	mainbox[9].y = 20.f;
	
	for (int i = 0; i < 10; i++) {
		mainbox[i].r = static_cast<float>(rand()) / RAND_MAX;
		mainbox[i].g = static_cast<float>(rand()) / RAND_MAX;
		mainbox[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
}
void makeshape() {
	make_main_box();
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