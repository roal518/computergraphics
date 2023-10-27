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

coord Points[10]= { 0 };
coord Lines[10][2] = { 0 };
coord Trian[10][3] = { 0 };
coord Rect1[10][3] = { 0 };
coord Rect2[10][3] = { 0 };
int Points_count = 0;
int Lines_count = 0;
int Trian_count = 0;
int Rect_count = 0;


void draw_point();
void draw_line();
void draw_trian();
void draw_rect();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
void InitPointBuffer();
void InitLineBuffer();
void InitTrianBuffer();
void InitRectBuffer();
void under();
void up();
GLuint vao; // 삼각형 개수 저장
GLuint vbo; // 색상,정점 위치 저장
GLuint ebo;
GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;



int drawing_mode = 0;

void main(int argc, char ** argv){
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 7");
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_point();
	draw_line();
	draw_trian();
	draw_rect();
	glutSwapBuffers();
}
void InitPointBuffer() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, Points_count * 6 * sizeof(float), Points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}
void draw_point() {
	if (Points_count != 0) {
		InitPointBuffer();
		glUseProgram(shaderProgramID);
		glBindVertexArray(vao);


		int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position");//0
		int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");
		

		glEnableVertexAttribArray(PosLocation);
		glEnableVertexAttribArray(ColorLocation);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), 0);
		glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));
		glPointSize(10.0);
		glDrawArrays(GL_POINTS, 0, Points_count);

		glDisableVertexAttribArray(PosLocation);
		glDisableVertexAttribArray(ColorLocation);
	}
}
void InitLineBuffer() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, Lines_count * 12 * sizeof(float), Lines, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
void draw_line() {
	if (Lines_count != 0) {
		InitLineBuffer();
		glLineWidth(2.0);
		glUseProgram(shaderProgramID);
		glBindVertexArray(vao);

		int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position");//0
		int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");


		glEnableVertexAttribArray(PosLocation);
		glEnableVertexAttribArray(ColorLocation);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), 0);
		glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));

		glDrawArrays(GL_LINES, 0, Lines_count * 2);

		glDisableVertexAttribArray(PosLocation);
		glDisableVertexAttribArray(ColorLocation);
	}
}

void InitTrianBuffer() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, Trian_count * 18 * sizeof(float), Trian, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
void draw_trian() {
	if (Trian_count != 0) {
		InitTrianBuffer();
		glUseProgram(shaderProgramID);
		glBindVertexArray(vao);


		int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position");//0
		int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");


		glEnableVertexAttribArray(PosLocation);
		glEnableVertexAttribArray(ColorLocation);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), 0);
		glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));

		glDrawArrays(GL_TRIANGLES, 0, Trian_count * 3);

		glDisableVertexAttribArray(PosLocation);
		glDisableVertexAttribArray(ColorLocation);
	}
}
void draw_rect() {
	if (Rect_count != 0) {
		under();
		up();
	}
}
int where = 0;
void under() {
	where = 0;
	InitRectBuffer();
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);


	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position");//0
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");


	glEnableVertexAttribArray(PosLocation);
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), 0);
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, Rect_count * 3);

	glDisableVertexAttribArray(PosLocation);
	glDisableVertexAttribArray(ColorLocation);
}
void up() {
	where = 1;
	InitRectBuffer();
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);


	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position");//0
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");


	glEnableVertexAttribArray(PosLocation);
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), 0);
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, Rect_count * 3);

	glDisableVertexAttribArray(PosLocation);
	glDisableVertexAttribArray(ColorLocation);
}
void InitRectBuffer() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if (where == 0) {
		glBufferData(GL_ARRAY_BUFFER, Rect_count * 18 * sizeof(float), Rect1, GL_STATIC_DRAW);
	}
	else if (where == 1) {
		glBufferData(GL_ARRAY_BUFFER, Rect_count * 18 * sizeof(float), Rect2, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}


GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 'p') {
		drawing_mode = 1;

	}
	else if (key == 'l') {
		drawing_mode = 2;

	}
	else if (key == 't') {
		drawing_mode = 3;

	}
	else if (key == 'r') {
		drawing_mode = 4;

	}
	else {
		float translationX = 0.0f;
		float translationY = 0.0f;
		if (key == 'w') {
			for (int i = 0; i < Points_count; i++) {
				Points[i].y += 0.005f;

			}
			for (int i = 0; i < Lines_count; i++) {
				Lines[i][0].y += 0.005f;
				Lines[i][1].y += 0.005f;
			}
			for (int i = 0; i < Trian_count; i++) {
				Trian[i][0].y += 0.005f;
				Trian[i][1].y += 0.005f;
				Trian[i][2].y += 0.005f;
			}
			for (int i = 0; i < Rect_count; i++) {
				Rect1[i][0].y += 0.005f;
				Rect1[i][1].y += 0.005f;
				Rect1[i][2].y += 0.005f;
			
				Rect2[i][0].y += 0.005f;
				Rect2[i][1].y += 0.005f;
				Rect2[i][2].y += 0.005f;

			}
		}
		if (key == 'a') {
			for (int i = 0; i < Points_count; i++) {
				Points[i].x -= 0.005f;
			}			
			for (int i = 0; i < Lines_count; i++) {
				Lines[i][0].x -= 0.005f;
				Lines[i][1].x -= 0.005f;
			}			
			for (int i = 0; i < Trian_count; i++) {
				Trian[i][0].x -= 0.005f;
				Trian[i][1].x -= 0.005f;
				Trian[i][2].x -= 0.005f;
			}
			for (int i = 0; i < Rect_count; i++) {
				Rect1[i][0].x -= 0.005f;
				Rect1[i][1].x -= 0.005f;
				Rect1[i][2].x -= 0.005f;

				Rect2[i][0].x -= 0.005f;
				Rect2[i][1].x -= 0.005f;
				Rect2[i][2].x -= 0.005f;

			}
		}
		if (key == 's') {
			for (int i = 0; i < Points_count; i++) {
				Points[i].y -= 0.005f;
			}
			for (int i = 0; i < Lines_count; i++) {
				Lines[i][0].y -= 0.005f;
				Lines[i][1].y -= 0.005f;
			}
			for (int i = 0; i < Trian_count; i++) {
				Trian[i][0].y -= 0.005f;
				Trian[i][1].y -= 0.005f;
				Trian[i][2].y -= 0.005f;
			}
			for (int i = 0; i < Rect_count; i++) {
				Rect1[i][0].y -= 0.005f;
				Rect1[i][1].y -= 0.005f;
				Rect1[i][2].y -= 0.005f;

				Rect2[i][0].y -= 0.005f;
				Rect2[i][1].y -= 0.005f;
				Rect2[i][2].y -= 0.005f;

			}
		}
		if (key == 'd') {
			for (int i = 0; i < Points_count; i++) {
				Points[i].x += 0.005f;
			}
			for (int i = 0; i < Lines_count; i++) {
				Lines[i][0].x += 0.005f;
				Lines[i][1].x += 0.005f;
			}
			for (int i = 0; i < Trian_count; i++) {
				Trian[i][0].x += 0.005f;
				Trian[i][1].x += 0.005f;
				Trian[i][2].x += 0.005f;
			}
			for (int i = 0; i < Rect_count; i++) {
				Rect1[i][0].x += 0.005f;
				Rect1[i][1].x += 0.005f;
				Rect1[i][2].x += 0.005f;

				Rect2[i][0].x += 0.005f;
				Rect2[i][1].x += 0.005f;
				Rect2[i][2].x += 0.005f;

			}
		}
		else if (key == 'c') {
			Points_count = 0;
			Rect_count = 0;
			Trian_count = 0;
			Lines_count = 0;

		}
		glutPostRedisplay();
	}
}
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float normalX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
		float normalY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
		if (drawing_mode != 0) {
			if (drawing_mode == 1) {
				if (Points_count < 10) {
					Points[Points_count].x = normalX;
					Points[Points_count].y = normalY;
					Points[Points_count].z = 0.0f;
					Points[Points_count].r = static_cast<float>(rand()) / RAND_MAX;
					Points[Points_count].g = static_cast<float>(rand()) / RAND_MAX;
					Points[Points_count].b = static_cast<float>(rand()) / RAND_MAX;
					Points_count++;
				}
				else {
					printf("overflow\n");
				}
			}
			if (drawing_mode == 2) {
				if (Lines_count < 10) {
					for (int i = 0; i < 2; i++) {
						Lines[Lines_count][i].r = static_cast<float>(rand()) / RAND_MAX;
						Lines[Lines_count][i].g = static_cast<float>(rand()) / RAND_MAX;
						Lines[Lines_count][i].b = static_cast<float>(rand()) / RAND_MAX;
						if (i == 0) {
							Lines[Lines_count][i].x = normalX-0.05f;
							Lines[Lines_count][i].y = normalY+0.05f;
							Lines[Lines_count][i].z = 0.0f;
						}
						else {
							Lines[Lines_count][i].x = normalX+0.05f;
							Lines[Lines_count][i].y = normalY-0.05f;
							Lines[Lines_count][i].z = 0.0f;
						}
					}
					Lines_count++;
				}
				else {
					printf("overflow\n");
				}
			}
			if (drawing_mode == 3) {
				if (Trian_count < 10) {
					for (int i = 0; i < 3; i++) {
						Trian[Trian_count][i].r = static_cast<float>(rand()) / RAND_MAX;
						Trian[Trian_count][i].g = static_cast<float>(rand()) / RAND_MAX;
						Trian[Trian_count][i].b = static_cast<float>(rand()) / RAND_MAX;
						if (i == 0) {
							Trian[Trian_count][i].x = normalX - 0.02f;
							Trian[Trian_count][i].y = normalY - 0.03f;
							Trian[Trian_count][i].z = 0.0f;
						}
						else if (i == 1) {
							Trian[Trian_count][i].x = normalX + 0.02f;
							Trian[Trian_count][i].y = normalY - 0.03f;
							Trian[Trian_count][i].z = 0.0f;
						}
						else if (i == 2) {
							Trian[Trian_count][i].x = normalX;
							Trian[Trian_count][i].y = normalY + 0.03f;
							Trian[Trian_count][i].z = 0.0f;
						}
					}
					Trian_count++;
				}
				else {
					printf("overflow\n");
				}
			}
			if (drawing_mode == 4) {
				if (Rect_count < 10) {
					for (int i = 0; i < 3; i++) {
						Rect1[Rect_count][i].r = static_cast<float>(rand()) / RAND_MAX;
						Rect1[Rect_count][i].g = static_cast<float>(rand()) / RAND_MAX;
						Rect1[Rect_count][i].b = static_cast<float>(rand()) / RAND_MAX;
						Rect2[Rect_count][i].r = static_cast<float>(rand()) / RAND_MAX;
						Rect2[Rect_count][i].g = static_cast<float>(rand()) / RAND_MAX;
						Rect2[Rect_count][i].b = static_cast<float>(rand()) / RAND_MAX;

						if (i == 0) {
							Rect1[Rect_count][i].x = normalX - 0.03f;
							Rect1[Rect_count][i].y = normalY - 0.03f;
							Rect1[Rect_count][i].z = 0.0f;

							Rect2[Rect_count][i].x = normalX + 0.03f;
							Rect2[Rect_count][i].y = normalY + 0.03f;
							Rect2[Rect_count][i].z = 0.0f;
						}
						else if (i == 1) {
							Rect1[Rect_count][i].x = normalX + 0.03f;
							Rect1[Rect_count][i].y = normalY - 0.03f;
							Rect1[Rect_count][i].z = 0.0f;

							Rect2[Rect_count][i].x = normalX + 0.03f;
							Rect2[Rect_count][i].y = normalY - 0.03f;
							Rect2[Rect_count][i].z = 0.0f;

						}
						else if (i == 2) {
							Rect1[Rect_count][i].x = normalX - 0.03f;
							Rect1[Rect_count][i].y = normalY + 0.03f;
							Rect1[Rect_count][i].z = 0.0f;

							Rect2[Rect_count][i].x = normalX - 0.03f;
							Rect2[Rect_count][i].y = normalY + 0.03f;
							Rect2[Rect_count][i].z = 0.0f;
						}
					}
					Rect_count++;
				}
				else {
					printf("overflow\n");
				}
			}
		}
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