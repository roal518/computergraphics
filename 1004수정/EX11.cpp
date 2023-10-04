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
//어차피 다음주 수요일까지 
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

coord rect1st[2][3] = { 0 };
coord trian2nd[3] = { 0 };
coord rect3rd[3][3] = { 0 };
coord penta4th[3][3] = { 0 };

coord midline[2][2] = { 0 };
GLuint vao;
GLuint vbo;
GLuint ebo;

GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;
int indices[3] = { 0,1,2 };
void makeshape();
void draw();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
GLvoid Reshape(int w, int h);
GLvoid drawScene();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
void firstquad(int val);
void secondquad(int val);
void thirdquad(int val);
void fourthquad(int val);
void draw1Q();
void draw2Q();
void draw3Q();
void draw4Q();
bool mode[5] = { true,true,true,true,true };
void main(int argc, char** argv) {
	srand(unsigned(time(NULL)));
	makeshape();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 10");
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
void keyboard(unsigned char key, int x, int y) {
	makeshape();
	if (key == 'i') {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 3; j++) {
				rect1st[i][j].x = rect1st[i][j].x * 1.5 - 0.8;
				rect1st[i][j].y = rect1st[i][j].y * 1.5 - 0.8;

			}
		}
		mode[0] = true;
		mode[1] = false;
		mode[2] = false;
		mode[3] = false;
		mode[4] = false;
		glutPostRedisplay();
		glutTimerFunc(30, firstquad, 0);
	}
	if (key == 't') {
		for (int j = 0; j < 3; j++) {
			trian2nd[j].x = trian2nd[j].x * 1.5 + 0.8;
			trian2nd[j].y = trian2nd[j].y * 1.5 - 0.8;
		}
		
		mode[0] = false;
		mode[1] = true;
		mode[2] = false;
		mode[3] = false;
		mode[4] = false;
		glutPostRedisplay();
		glutTimerFunc(30, secondquad, 0);
	}
	if (key == 'r') {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				rect3rd[i][j].x= rect3rd[i][j].x * 1.5 + 0.8;
				rect3rd[i][j].y = rect3rd[i][j].y* 1.5 + 0.8;
			}
		}
		mode[0] = false;
		mode[1] = false;
		mode[2] = true;
		mode[3] = false;
		mode[4] = false;
		glutTimerFunc(30, thirdquad, 0);

		glutPostRedisplay();
	}
	if (key == 'p') {
		mode[0] = false;
		mode[1] = false;
		mode[2] = false;
		mode[3] = true;
		mode[4] = false;
		glutPostRedisplay();
		glutTimerFunc(30, fourthquad, 0);
	}
	if (key == 'a') {
		for (int i = 0; i < 5; i++) {
			mode[i] = true;
		}
		glutPostRedisplay();
	}
	if (key == 27) {
		exit(0);
	}
}
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float normalX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
		float normalY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
		for (int i = 0; i < 5; i++) {
			mode[i] = true;
		}
		makeshape();

		glutTimerFunc(30, thirdquad, 0);
		glutTimerFunc(30, fourthquad, 0);
		glutTimerFunc(30, firstquad, 0);
		glutTimerFunc(30, secondquad, 0);

	}
}
void firstquad(int val) {
	if (mode[0]) {
		if (rect1st[1][2].x <= rect1st[1][1].x) {
			rect1st[1][2].x += 0.001;
			if (rect1st[0][0].x <= rect1st[0][2].x) {
				rect1st[0][2].x -= 0.001;
				rect1st[1][0].x -= 0.001;
			}
			glutPostRedisplay();
			glutTimerFunc(30, firstquad, 0);
		}
	}
}
void secondquad(int val) {
	if (mode[1]) {
		if (trian2nd[2].y <= 0.7 * 1.5 - 0.8) {
			trian2nd[2].y += 0.001;
			if (trian2nd[2].x >= -0.5 * 1.5 + 0.8) {
				trian2nd[2].x -= 0.001;
			}
			glutPostRedisplay();
			glutTimerFunc(30, secondquad, 0);
		}
	}
}
void thirdquad(int val) {
	if (mode[2]) {
		if (rect3rd[0][0].x < -0.7) {
			rect3rd[0][0].x += 0.001;
		}
		if (rect3rd[0][0].y > -0.8) {
			rect3rd[0][0].y -= 0.001;
		}
		if (rect3rd[0][1].y > -0.8) {
			rect3rd[0][1].y -= 0.001;
			rect3rd[1][1].y -= 0.001;
			rect3rd[2][1].y -= 0.001;
		}
		if (rect3rd[0][2].y > -0.4) {
			rect3rd[0][2].y -= 0.001;
			rect3rd[1][0].y -= 0.001;
		}
		if (rect3rd[0][2].x > -0.3) {
			rect3rd[1][0].x -= 0.001;
			rect3rd[2][1].x -= 0.001;
		}
		if (rect3rd[1][2].x > -0.45) {
			rect3rd[1][2].x -= 0.001;
			rect3rd[2][0].x -= 0.001;
		}
		if (rect3rd[2][2].x < -0.2) {
			rect3rd[2][2].x += 0.001;
		}
		if (rect3rd[2][2].y > -0.4) {
			rect3rd[2][2].y -= 0.001;
		}
		glutPostRedisplay();
		glutTimerFunc(30, thirdquad, 0);
	}
}
void fourthquad(int val) {
	if (mode[3]) {
		if (penta4th[0][0].x < 0.68f) {
			penta4th[0][0].x += 0.001;
			penta4th[1][0].x += 0.001;
		}
		if (penta4th[0][0].y > -0.79f) {
			penta4th[0][0].y -= 0.001;
			penta4th[1][0].y -= 0.001;
		}
		if (penta4th[0][1].x < 0.685f) {
			penta4th[0][1].x += 0.001;
		}
		if (penta4th[1][2].x < 0.69) {
			penta4th[2][0].x += 0.001;
			penta4th[1][2].x += 0.001;
		}
		if (penta4th[1][2].y > -0.78) {
			penta4th[1][2].y -= 0.001;
			penta4th[2][0].y -= 0.001;
		}
		if (penta4th[2][2].x > 0.703) {
			penta4th[2][2].x -= 0.001;
		}
		if (penta4th[2][2].y > -0.79) {
			penta4th[2][2].y -= 0.001;
		}
		glutPostRedisplay();
		glutTimerFunc(30, fourthquad, 0);
	}
}
bool firstmake[4] = { true,true,true,true };
void makeshape() {
	midline[0][1].x = 1.0f;
	midline[0][0].x = -1.0f;
	midline[1][1].y = 1.0f;
	midline[1][0].y = -1.0f;
	{

		for (int i = 0; i < 3; i++) {
			if (i > 0) {
				trian2nd[i].x = -0.25;
				trian2nd[i].y = 0.2;
			}
			else {
				trian2nd[i].x = -0.75;
				trian2nd[i].y = 0.2;
			}
		}
		if (firstmake[0]) {
			for (int i = 0; i < 3; i++) {
				trian2nd[i].r = static_cast<float>(rand()) / RAND_MAX;
				trian2nd[i].b = static_cast<float>(rand()) / RAND_MAX;
				trian2nd[i].g = static_cast<float>(rand()) / RAND_MAX;
			}
			firstmake[0] = false;
		}

	}
	{
		if (firstmake[1]) {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 3; j++) {
					rect1st[i][j].r = static_cast<float>(rand()) / RAND_MAX;
					rect1st[i][j].g = static_cast<float>(rand()) / RAND_MAX;
					rect1st[i][j].b = static_cast<float>(rand()) / RAND_MAX;
				}
			}
			firstmake[1] = false;
		}

		rect1st[0][0].x = 0.25f;
		rect1st[0][0].y = 0.2f;

		rect1st[0][1].x = 0.75f;
		rect1st[0][1].y = 0.2f;

		rect1st[0][2].x = 0.5f;
		rect1st[0][2].y = 0.7f;

		rect1st[1][0].x = 0.5f;
		rect1st[1][0].y = 0.7f;

		rect1st[1][1].x = 0.75f;
		rect1st[1][1].y = 0.2f;

		rect1st[1][2].x = 0.5f;
		rect1st[1][2].y = 0.7f;
	}
	/////////////////////
	{
		rect3rd[0][0].x = -0.75;
		rect3rd[0][0].y = -0.7;

		rect3rd[0][1].x = -0.25;
		rect3rd[0][1].y = -0.7;

		rect3rd[0][2].x = -0.75;
		rect3rd[0][2].y = -0.2;

		rect3rd[1][0].x = -0.75;
		rect3rd[1][0].y = -0.2;

		rect3rd[1][1].x = -0.25;
		rect3rd[1][1].y = -0.7;

		rect3rd[1][2].x = -0.25;
		rect3rd[1][2].y = -0.2;

		rect3rd[2][0].x = -0.25;
		rect3rd[2][0].y = -0.2;

		rect3rd[2][1].x = -0.25;
		rect3rd[2][1].y = -0.7;

		rect3rd[2][2].x = -0.25;
		rect3rd[2][2].y = -0.2;
		if (firstmake[2]) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					rect3rd[i][j].r = static_cast<float>(rand()) / RAND_MAX;
					rect3rd[i][j].g = static_cast<float>(rand()) / RAND_MAX;
					rect3rd[i][j].b = static_cast<float>(rand()) / RAND_MAX;
				}
			}
			firstmake[2] = false;
		}
	}
	////////////////////////
	for (int i = 0; i < 3; i++) {
		if (i == 0) {
			penta4th[i][0].x = 0.2;
			penta4th[i][0].y = -0.4;

			penta4th[i][1].x = 0.3;
			penta4th[i][1].y = -0.8;

			penta4th[i][2].x = 0.7;
			penta4th[i][2].y = -0.8;
		}
		if (i == 1) {
			penta4th[i][0].x = 0.2;
			penta4th[i][0].y = -0.4;

			penta4th[i][1].x = 0.7;
			penta4th[i][1].y = -0.8;

			penta4th[i][2].x = 0.5;
			penta4th[i][2].y = -0.2;

		}
		if (i == 2) {
			penta4th[i][0].x = 0.5;
			penta4th[i][0].y = -0.2;

			penta4th[i][1].x = 0.7;
			penta4th[i][1].y = -0.8;

			penta4th[i][2].x = 0.8;
			penta4th[i][2].y = -0.4;

		}
	}
	if (firstmake[3]) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				penta4th[i][j].r = static_cast<float>(rand()) / RAND_MAX;
				penta4th[i][j].g = static_cast<float>(rand()) / RAND_MAX;
				penta4th[i][j].b = static_cast<float>(rand()) / RAND_MAX;
			}
		}
		firstmake[3] = false;
	}
}

GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (mode[4]) {
		draw();
	}
	if (mode[0]) {
		draw1Q();
	}
	if (mode[1]) {
		draw2Q();
	}
	if (mode[2]) {
		draw3Q();
	}
	if (mode[3]) {
		draw4Q();
	}
	glutSwapBuffers();
}
void draw1Q() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	for (int i = 0; i < 2; i++) {
		glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), rect1st[i], GL_STATIC_DRAW);
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDeleteBuffers(1, &ebo);
	}
}
void draw2Q() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), trian2nd, GL_STATIC_DRAW);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	if (trian2nd[2].y != trian2nd[1].y) {
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}
	else {
		glLineWidth(1.0f);
		glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
	}
	glDeleteBuffers(1, &ebo);
}
void draw3Q() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	for (int i = 0; i < 3; i++) {
		glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), rect3rd[i], GL_STATIC_DRAW);
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}
	glDeleteBuffers(1, &ebo);
}
void draw4Q() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	for (int i = 0; i < 3; i++) {
		glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), penta4th[i], GL_STATIC_DRAW);
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}
	glDeleteBuffers(1, &ebo);
}
void draw() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float), midline, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position");//0
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");


	glEnableVertexAttribArray(PosLocation);
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), 0);
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));
	glLineWidth(2.0);
	glDrawArrays(GL_LINES, 0, 4);

	glDisableVertexAttribArray(PosLocation);
	glDisableVertexAttribArray(ColorLocation);
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

// 리턴값이 계산할 좌표로 들어간다.
// 한시간정도면 그냥 할것?
//내일 끝내놓으면 될듯 
int returnintiger(int setmode,float coordinate) {
	if (setmode == 1) {
		if(coordinate>0){
			return coordinate = coordinate * 1.5 - 0.8;
		}
		if (coordinate < 0) {
			return coordinate = coordinate * 1.5 + 0.8;
		}
	}
	else if (setmode == 2) {
		return coordinate;
	}
}