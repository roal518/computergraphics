#define _CRT_SECURE_NO_WARNINGS 
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

GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;
GLuint vao, vbo, ebo;
GLuint vao1, vbo1, ebo1;
struct coord {
	float x = 0;
	float y = 0;
	float z = 0;
	float r = 0;
	float g = 0;
	float b = 0;
};
//기본 정보
coord line[4];
coord midline[2][2];
int indices[4] = {0,1,2,3}; // ebo 써서 인덱스 어쩌고 쓰자 
void ray_casting(int x, int y);
void makeLine();
void initbuffer();
void initQuadBuffer();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mousemove(int x, int y);
GLvoid Reshape(int w, int h);
GLvoid drawScene();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
bool isclicked =  false ;
bool ismoving = false;
void main(int argc, char** argv) {
	srand(unsigned(time(NULL)));
	makeLine();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 13");
	glewExperimental = GL_TRUE;
	glewInit();

	make_shaderProgram();

	glutMouseFunc(mouse);
	glutMotionFunc(mousemove);
	glutKeyboardFunc(keyboard);

	glutDisplayFunc(drawScene);	
	glutReshapeFunc(Reshape);

	glutMainLoop();
}
void makeLine() {
	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			line[i].x = 0.5f;
			line[i].y = 0.5f;
		}
		else if (i == 1) {
			line[i].x = -0.5f;
			line[i].y = 0.5f;
		}
		else if (i == 2) {
			line[i].x = -0.5f;
			line[i].y = -0.5f;
		}
		else if (i == 3) {
			line[i].x = 0.5f;
			line[i].y = -0.5f;
		}
		line[i].r = static_cast<float>(rand()) / (RAND_MAX);
		line[i].g = static_cast<float>(rand()) / (RAND_MAX);
		line[i].b = static_cast<float>(rand()) / (RAND_MAX);
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
void initbuffer() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float), line, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}
void rectdraw() {
	initbuffer();
	glLineWidth(2.0f);
	glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_INT, 0);
}
void drawQuad() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);

	glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float), midline, GL_STATIC_DRAW);

	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position");//0
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");

	glEnableVertexAttribArray(PosLocation);
	glEnableVertexAttribArray(ColorLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), 0);
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));
	glLineWidth(2.0f);
	glDrawArrays(GL_LINES, 0, 4);

	glDisableVertexAttribArray(PosLocation);
	glDisableVertexAttribArray(ColorLocation);
}
GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	rectdraw();
	drawQuad();
	glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}
}
float mouseX;
float mouseY;
int nowline;
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float normalX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
		float normalY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
		for (int i = 0; i < 4; i++) {
			if ((normalX > line[i].x - 0.01 && normalX < line[i].x + 0.01) &&
				(normalY > line[i].y - 0.01 && normalY < line[i].y + 0.01)) {
				nowline = i;
				isclicked = true;
			}
		}
		ray_casting(x, y);
		mouseX = x;
		mouseY = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		isclicked = false;
		ismoving = false;
	}
}
bool change_sign(float a, float b) {
	if ((a > 0 && b <= 0) || (a <= 0 && b > 0)) {
		return true;
	}
	else {
		return false;
	}
}
void ray_casting(int x, int y) {
	float normalX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
	float normalY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);

	float maxHeight = -1;
	float minHeight = 1;
	float maxlength = -1;
	float minlength = 1;
	bool checkonce[4];
	float MP[4];

	for (int i = 0; i < 4; i++) {
		if (maxHeight < line[i].y) {
			maxHeight = line[i].y;
		}
		if (minHeight > line[i].y) {
			minHeight = line[i].y;
		}
		if (maxlength < line[i].x) {
			maxlength = line[i].x;
		}
		if (minlength > line[i].x) {
			minlength = line[i].x;
		}
		checkonce[i] = false;
		MP[i] = 0;
	}

	int count = 0;
	float temp = 0;
	float t = 0;

	if ((minHeight < normalY && normalY < maxHeight) && (minlength < normalX && normalX < maxlength)) {
		for (int n = 0; n < 4; n++) {
			if (!checkonce[n]) {
				for (int i = 1; i < 99; i++) {
					float sbX = (1 - t) * line[n].x + t * line[(n + 1) % 4].x;
					if (sbX < normalX) {
						MP[n] = (1 - t) * line[n].y + t * line[(n + 1) % 4].y - normalY;
						temp = (1 - (i + 1) / 100.0f) * line[n].y + (i + 1) / 100.0f * line[(n + 1) % 4].y - normalY;
						if (change_sign(MP[n], temp)) {
							checkonce[n] = true;
							count++;
							break;
						}
					}
					t += 0.01;
				}
			}
		}
		if (count % 2 == 1) {
			ismoving = true;
		}
	}
	else {
		ismoving = false;
	}
}

void mousemove(int x, int y) {
	if (isclicked) {
		float deltaX = (x - mouseX) / 400.0f;
		float deltaY = (y - mouseY) / 300.0f;
		line[nowline].x += deltaX;
		line[nowline].y -= deltaY;
		mouseX = x;
		mouseY = y;
	}
	if (ismoving) {
		float deltaX = (x - mouseX) / 400.0f;
		float deltaY = (y - mouseY) / 300.0f;
		for (int i = 0; i < 4; i++) {
			line[i].x += deltaX;
			line[i].y -= deltaY;
		}
		mouseX = x;
		mouseY = y;
	}

	glutPostRedisplay();
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