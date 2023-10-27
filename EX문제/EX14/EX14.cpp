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
bool iscubedraw[6];
coord tetra[4];
bool istetradraw[4];
GLuint vao; // 삼각형 개수 저장
GLuint cubevbo; // 색상,정점 위치 저장
GLuint tetravbo;
GLuint midvbo;
GLuint ebo;

unsigned int cubedices[] = {
	0, 1, 2, 0, 2, 3,   // 정면 삼각형
	4, 5, 6, 6, 7, 4,   // 후면 삼각형
	0, 3, 4, 0, 4, 7,   // 상면 삼각형
	1, 6, 5, 5, 2, 1,   // 하면 삼각형
	0, 7, 6, 0, 6, 1,   // 좌측 삼각형
	2, 5, 4, 4, 3, 2    // 우측 삼각형
};
unsigned int tetradices[] = {
	0,1,2,
	0,2,3,
	0,3,1,
	1,3,2
};
void keyboard(unsigned char key, int x, int y);
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

void main(int argc, char** argv) {
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 14");
	glewExperimental = GL_TRUE;
	glewInit();
	makeshape();
	make_shaderProgram();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}
int drawmode = 1;
void keyboard(unsigned char key, int x, int y) {
	if (key == '1') {
		drawmode = 1;
		if (!iscubedraw[0]) {
			iscubedraw[0] = true;
		}
		else {
			iscubedraw[0] = false;
		}
	}
	if (key == '2') {
		drawmode = 1;
		if (!iscubedraw[1]) {
			iscubedraw[1] = true;
		}
		else {
			iscubedraw[1] = false;
		}
	}
	if (key == '3') {
		drawmode = 1;
		if (!iscubedraw[2]) {
			iscubedraw[2] = true;
		}
		else {
			iscubedraw[2] = false;
		}
	}
	if (key == '4') {
		drawmode = 1;
		if (!iscubedraw[3]) {
			iscubedraw[3] = true;
		}
		else {
			iscubedraw[3] = false;
		}
	}
	if (key == '5') {
		drawmode = 1;
		if (!iscubedraw[4]) {
			iscubedraw[4] = true;
		}
		else {
			iscubedraw[4] = false;
		}
	}
	if (key == '6') {
		drawmode = 1;
		if (!iscubedraw[5]) {
			iscubedraw[5] = true;
		}
		else {
			iscubedraw[5] = false;
		}
	}
	if (key == 'c') {
		drawmode = 1;
		int index1 = rand() % 6;
		int index2 = rand() % 6;
		for (int i = 0; i < 6; i++) {
			iscubedraw[i] = false;
		}
		while (true) {
			if (index1 == index2) {
				index2 = rand() % 6;
			}
			else {
				iscubedraw[index1]=true;
				iscubedraw[index2]=true;
				break;
			}
		}
	}
	if (key == '7') {
		drawmode = 0;
		if (!istetradraw[0]) {
			istetradraw[0] = true;
		}
		else {
			istetradraw[0] = false;
		}
	}
	if (key == '8') {
		drawmode = 0;

		if (!istetradraw[1]) {
			istetradraw[1] = true;
		}
		else {
			istetradraw[1] = false;
		}
	}
	if (key == '9') {
		drawmode = 0;
		if (!istetradraw[2]) {
			istetradraw[2] = true;
		}
		else {
			istetradraw[2] = false;
		}
	}
	if (key == '0') {
		drawmode = 0;
		if (!istetradraw[3]) {
			istetradraw[3] = true;
		}
		else {
			istetradraw[3] = false;
		}
	}
	if (key == 't') {
		drawmode = 0;
		int index1 = rand() % 4;
		int index2 = rand() % 4;
		for (int i = 0; i < 4; i++) {
			istetradraw[i] = false;
		}
		while (true) {
			if (index1 == index2) {
				index2 = rand() % 4;
			}
			else {
				istetradraw[index1] = true;
				istetradraw[index2] = true;
				break;
			}
		}
	}

	glutPostRedisplay();
}
GLvoid drawScene() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	drawmidline();
	if (drawmode == 1) {
		for (int i = 0; i < 4; i++) {
			istetradraw[i] = false;
		}
		drawcube();
	}
	else if (drawmode == 0) {
		for (int i = 0; i < 6; i++) {
			iscubedraw[i] = false;
		}
		drawtetra();
	}	
	glutSwapBuffers();
}
void drawtetra() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);

	glGenBuffers(1, &tetravbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.2f, 1.2f, 1.2f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, tetravbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float), tetra, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetradices), tetradices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	if (istetradraw[0]) {
		int i = 0;
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * i * sizeof(float)));
	}
	if (istetradraw[1]) {
		int i = 1;
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * i * sizeof(float)));
	}
	if (istetradraw[2]) {
		int i = 2;
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * i * sizeof(float)));
	}
	if (istetradraw[3]) {
		int i = 3;
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * i * sizeof(float)));
	}
}
void drawmidline() {
	glLineWidth(2.0f);
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &midvbo);
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
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
void drawcube() {
	initbuffer();
}
void initbuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);

	glGenBuffers(1, &cubevbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, cubevbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), cube, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubedices), cubedices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	if (iscubedraw[0]) {
		int i = 0;
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * i * sizeof(float)));
	}
	if (iscubedraw[1]) {
		int i = 1;
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * i * sizeof(float)));
	}
	if (iscubedraw[2]) {
		int i = 2;
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * i * sizeof(float)));
	}	
	if (iscubedraw[3]) {
		int i = 3;
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * i * sizeof(float)));
	}
	if (iscubedraw[4]) {
		int i = 4;
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * i * sizeof(float)));
	}
	if (iscubedraw[5]) {
		int i = 5;
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * i * sizeof(float)));
	}
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
	make_tetra();
	//**정사면체
	//정면
}
void make_cube() {
	for (int i = 0; i < 6; i++) {
		iscubedraw[i] = false;
	}
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
void make_tetra() {
	for (int i = 0; i < 4; i++) {
		istetradraw[i] = false;
	}
	tetra[0].x = 0.0f;
	tetra[0].y = 0.0f;
	tetra[0].z = 2 * sqrt(3) / 12.0f;


	tetra[1].x = 0.0f;
	tetra[1].y = 2 * sqrt(3) / 12.0f;
	tetra[1].z = -sqrt(3) / 12.0f;

	tetra[2].x = -3.0f/12.0f;
	tetra[2].y = -sqrt(3) / 12.0f;
	tetra[2].z = -sqrt(3) / 12.0f;

	tetra[3].x = 3.0f / 12.0f;
	tetra[3].y = -sqrt(3) / 12.0f;
	tetra[3].z = -sqrt(3) / 12.0f;
	for (int i = 0; i < 3; i++) {
		tetra[i].r = static_cast<float>(rand()) / RAND_MAX;
		tetra[i].g = static_cast<float>(rand()) / RAND_MAX;
		tetra[i].b = static_cast<float>(rand()) / RAND_MAX;
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