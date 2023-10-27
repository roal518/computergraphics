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

int rendermode = 0;
coord Trian[4][3] = { 0 };
coord Line[2][2] = { 0 };

unsigned int indices[] = { 0,1,2,0,2,1};
GLuint vao; // 데이터
GLuint vbo; // 색상,정점 위치 저장
GLuint vbo1;
GLuint vbo2;
GLuint vbo3;
GLuint ebo;
GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;
GLvoid drawScene();
GLvoid Reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
void draw_line();
void draw_face();
void maketriangle();
void draw_mid();

void main(int argc, char** argv) {
	srand(unsigned(time(NULL)));
	maketriangle();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 8");
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
void maketriangle() {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			Trian[i][j].r = static_cast<float>(rand()) / RAND_MAX;
			Trian[i][j].g = static_cast<float>(rand()) / RAND_MAX;
			Trian[i][j].b = static_cast<float>(rand()) / RAND_MAX;
			if (i == 0) {
				if (j == 0) {
					Trian[i][j].x = 0.5f;
					Trian[i][j].y = 0.5f;
					Trian[i][j].z = 0.0f;
				}
				else if (j == 1) {

					Trian[i][j].x = 0.25f;
					Trian[i][j].y = 0.25f;
					Trian[i][j].z = 0.0f;
				}
				else if (j == 2) {

					Trian[i][j].x = 0.75f;
					Trian[i][j].y = 0.25f;
					Trian[i][j].z = 0.0f;
				}
			}
			else if (i == 1) {
				if (j == 0) {
					Trian[i][j].x = -0.5f;
					Trian[i][j].y = 0.5f;
					Trian[i][j].z = 0.0f;
				}
				else if (j == 1) {

					Trian[i][j].x = -0.25f;
					Trian[i][j].y = 0.25f;
					Trian[i][j].z = 0.0f;
				}
				else if (j == 2) {

					Trian[i][j].x = -0.75f;
					Trian[i][j].y = 0.25f;
					Trian[i][j].z = 0.0f;
				}
			}
			else if (i == 2) {
				if (j == 0) {
					Trian[i][j].x = -0.5f;
					Trian[i][j].y = -0.25f;
					Trian[i][j].z = 0.0f;
				}
				else if (j == 1) {
					Trian[i][j].x = -0.25f;
					Trian[i][j].y = -0.5f;
					Trian[i][j].z = 0.0f;
				}
				else if (j == 2) {
					Trian[i][j].x = -0.75f;
					Trian[i][j].y = -0.5f;
					Trian[i][j].z = 0.0f;
				}
			}
			else if (i == 3) {
				if (j == 0) {
					Trian[i][j].x = 0.5f;
					Trian[i][j].y = -0.25f;
					Trian[i][j].z = 0.0f;
				}
				else if (j == 1) {

					Trian[i][j].x = 0.25f;
					Trian[i][j].y = -0.5f;
					Trian[i][j].z = 0.0f;
				}
				else if (j == 2) {

					Trian[i][j].x = 0.75f;
					Trian[i][j].y = -0.5f;
					Trian[i][j].z = 0.0f;
				}
			}
		}
	}
	Line[0][0].x = -1.0f;
	Line[0][1].x = 1.0f;

	Line[1][0].y = -1.0f;
	Line[1][1].y = 1.0f;
}
GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_mid();
	if (rendermode == 0) {
		draw_face();
	}
	else if (rendermode == 1) {
		draw_line();
	}
	glutSwapBuffers();
}
void InitfaceBuffer(int state) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (state == 12) {
		glBufferData(GL_ARRAY_BUFFER, state * 6 * sizeof(float), Trian, GL_STATIC_DRAW);
	}
	if (state == 4) {
		glBufferData(GL_ARRAY_BUFFER, state * 6 * sizeof(float), Line, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
void InitLineBuffer(int i) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	if (i == 0) {
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
	}
	if (i == 1) {
		glGenBuffers(1, &vbo1);
		glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	}
	if (i == 2) {
		glGenBuffers(1, &vbo2);
		glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	}
	if (i == 3) {
		glGenBuffers(1, &vbo3);
		glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	}
	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), Trian[i], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}
void draw_line() {
	for (int i = 0; i < 4; i++) {
		InitLineBuffer(i);
		glUseProgram(shaderProgramID);
		glBindVertexArray(vao);
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
	}
}
void draw_mid() {
	InitfaceBuffer(4);
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);


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
void draw_face() {
	InitfaceBuffer(12);
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);


	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position");//0
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");


	glEnableVertexAttribArray(PosLocation);
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), 0);
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));
	glLineWidth(2.0);
	glDrawArrays(GL_TRIANGLES, 0, 12);

	glDisableVertexAttribArray(PosLocation);
	glDisableVertexAttribArray(ColorLocation);
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 'a') {
		rendermode = 0;
		glutPostRedisplay();
	}
	else if (key == 'b') {
		rendermode = 1;
		glutPostRedisplay();
	}
	else if (key == 27) {
		exit(0);
	}
}
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float normalX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
		float normalY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
		float randomX = (static_cast<float>(rand()) / RAND_MAX)/4;
		float randomY = (static_cast<float>(rand()) / RAND_MAX)/4;
		if (normalX > 0 && normalY > 0) {
			Trian[0][0].x = normalX;
			Trian[0][0].y = normalY + randomY;

			Trian[0][1].x = normalX - randomX;
			Trian[0][1].y = normalY - randomY;

			Trian[0][2].x = normalX + randomX;
			Trian[0][2].y = normalY - randomY;
			for (int j = 0; j < 3; j++) {
				Trian[0][j].r = static_cast<float>(rand()) / RAND_MAX;
				Trian[0][j].g = static_cast<float>(rand()) / RAND_MAX;
				Trian[0][j].b = static_cast<float>(rand()) / RAND_MAX;
			}
		}
		else if (normalX < 0 && normalY>0) {
			Trian[1][0].x = normalX;
			Trian[1][0].y = normalY + randomY;

			Trian[1][1].x = normalX - randomX;
			Trian[1][1].y = normalY - randomY;

			Trian[1][2].x = normalX + randomX;
			Trian[1][2].y = normalY - randomY;
			for (int j = 0; j < 3; j++) {
				Trian[1][j].r = static_cast<float>(rand()) / RAND_MAX;
				Trian[1][j].g = static_cast<float>(rand()) / RAND_MAX;
				Trian[1][j].b = static_cast<float>(rand()) / RAND_MAX;
			}
		}
		else if (normalX < 0 && normalY<0) {
			Trian[2][0].x = normalX;
			Trian[2][0].y = normalY + randomY;

			Trian[2][1].x = normalX - randomX;
			Trian[2][1].y = normalY - randomY;

			Trian[2][2].x = normalX + randomX;
			Trian[2][2].y = normalY - randomY;
			for (int j = 0; j < 3; j++) {
				Trian[2][j].r = static_cast<float>(rand()) / RAND_MAX;
				Trian[2][j].g = static_cast<float>(rand()) / RAND_MAX;
				Trian[2][j].b = static_cast<float>(rand()) / RAND_MAX;
			}

		}
		else if (normalX > 0 && normalY < 0) {
			Trian[3][0].x = normalX;
			Trian[3][0].y = normalY + randomY;

			Trian[3][1].x = normalX - randomX;
			Trian[3][1].y = normalY - randomY;

			Trian[3][2].x = normalX + randomX;
			Trian[3][2].y = normalY - randomY;
			for (int j = 0; j < 3; j++) {
				Trian[3][j].r = static_cast<float>(rand()) / RAND_MAX;
				Trian[3][j].g = static_cast<float>(rand()) / RAND_MAX;
				Trian[3][j].b = static_cast<float>(rand()) / RAND_MAX;
			}
		}
		glutPostRedisplay();
	}
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