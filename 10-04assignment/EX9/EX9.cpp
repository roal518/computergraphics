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
coord Trian[4][3] = { 0 };
GLuint vao; // 삼각형 개수 저장
GLuint vbo[4]; // 색상,정점 위치 저장
GLuint ebo;

GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;

unsigned int indices[] = {0,1,2}; // 삼각형 인덱스 어쩌고~버퍼

GLvoid drawScene();
GLvoid Reshape(int w, int h);
void makeTriangles();
void InitBuffer(int i);
void draw_triangles();
void keyboard(unsigned char key, int x, int y);

void angle(int val);
void sidetoside(int val);
void lickwall(int val);
void sprialmoving(int val);

void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

bool timer[4] = { false,false,false,false };
float theta[4] = { 0.0f };
float movingxy[4][2];


void main(int argc, char** argv) {
	srand(unsigned(time(NULL)));
	makeTriangles();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 9");
	glewExperimental = GL_TRUE;
	glewInit();
	make_shaderProgram();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
void keyboard(unsigned char key, int x, int y) {
	if (key == '1') {
		if (timer[0] != true) {
			timer[0] = true;
			timer[1] = false;
			timer[2] = false;
			timer[3] = false;
			glutTimerFunc(30, angle, 0);
		}
		else {
			timer[0] = false;
			glutTimerFunc(0, angle, 0);
		}
	}
	else if (key == '2') {
		if (timer[1] != true) {
			timer[0] = false;
			timer[1] = true;
			timer[2] = false;
			timer[3] = false;
			glutTimerFunc(30, sidetoside, 0);
		}
		else {
			timer[1] = false;
			glutTimerFunc(0, sidetoside, 0);
		}
	}
	else if (key == '3') {
		if (timer[2] != true) {
			timer[0] = false;
			timer[1] = false;
			timer[2] = true;
			timer[3] = false;
			glutTimerFunc(30, lickwall, 0);
		}
		else {
			timer[2] = false;
		}
	}
	else if (key == '4') {
		if (timer[3] != true) {
			timer[0] = false;
			timer[1] = false;
			timer[2] = false;
			timer[3] = true;
			glutTimerFunc(30, sprialmoving, 0);
		}
		else {
			timer[3] = false;
		}
	}
	else if (key == 27) {
		exit(0);
	}
	glutPostRedisplay();
}
int movemode[4] = { 0,0,0,0 };
int sidemode[4] = { 0,0,0,0 };
int lickmode[4] = { 0,0,0,0 };
void angle(int val) {
	if (timer[0]) {
		for (int i = 0; i < 4; i++) {
			glm::vec3 triangleVertex[3];
			glm::vec3 center(0.0f, 0.0f, 0.0f);
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			for (int j = 0; j < 3; j++) {
				triangleVertex[j] = glm::vec3(Trian[i][j].x, Trian[i][j].y, Trian[i][j].z);
				center += triangleVertex[j];
			}
			center /= 3.0f;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(movingxy[i][0], movingxy[i][1], 0));
			if (glm::dot(center, center) >= 0.0f) {
				modelMatrix = glm::translate(modelMatrix, center);//삼각형을 원점으로 이동
				modelMatrix = glm::rotate(modelMatrix, glm::radians(theta[i]), glm::vec3(0.0f, 0.0f, 1.0f));//회전
				modelMatrix = glm::translate(modelMatrix, -center);//삼각형을 다시 제자리로 돌려보냄
			}
			else {
				modelMatrix = glm::translate(modelMatrix, -center);//삼각형을 원점으로 이동
				modelMatrix = glm::rotate(modelMatrix, glm::radians(theta[i]), glm::vec3(0.0f, 0.0f, 1.0f));//회전
				modelMatrix = glm::translate(modelMatrix, center);//삼각형을 다시 제자리로 돌려보냄
			}
			for (int j = 0; j < 3; j++) {
				triangleVertex[j] = glm::vec3(modelMatrix * glm::vec4(triangleVertex[j], 1.0f));
			}
			float xspeed = (static_cast<float>(rand()) / RAND_MAX) / 25; 
			float yspeed = (static_cast<float>(rand()) / RAND_MAX) / 25;
			if (movemode[i] == 0) {
				if ((triangleVertex[0].x < 1.0f && triangleVertex[1].x < 1.0f && triangleVertex[2].x < 1.0f) &&
					(triangleVertex[0].y < 1.0f && triangleVertex[1].y < 1.0f && triangleVertex[2].y < 1.0f)) {
					movingxy[i][0] += xspeed;
					movingxy[i][1] += yspeed;
					theta[i] -= 1;
				}
				else {
					theta[i] += 10;
					movemode[i] = rand() % 4;
				}
			}
			else if (movemode[i] == 1) {
				if ((triangleVertex[0].x > -1.0f && triangleVertex[1].x > -1.0f && triangleVertex[2].x > -1.0f) &&
					(triangleVertex[0].y < 1.0f && triangleVertex[1].y < 1.0f && triangleVertex[2].y < 1.0f)) {
					movingxy[i][0] -= xspeed;
					movingxy[i][1] += yspeed;
					theta[i] += 1;
				}
				else {
					theta[i] += 10;
					movemode[i] = rand() % 4;
				}
			}
			else if (movemode[i] == 2) {
				if ((triangleVertex[0].x > -1.0f && triangleVertex[1].x > -1.0f && triangleVertex[2].x > -1.0f) &&
					(triangleVertex[0].y > -1.0f && triangleVertex[1].y > -1.0f && triangleVertex[2].y > -1.0f)) {
					movingxy[i][0] -= xspeed;
					movingxy[i][1] -= yspeed;
					theta[i] -= 1;
				}
				else {
					theta[i] += 10;
					movemode[i] = rand() % 4;
				}
			}
			else if (movemode[i] == 3) {
				if ((triangleVertex[0].x < 1.0f && triangleVertex[1].x < 1.0f && triangleVertex[2].x < 1.0f) &&
					(triangleVertex[0].y > -1.0f && triangleVertex[1].y > -1.0f && triangleVertex[2].y > -1.0f)) {
					movingxy[i][0] += xspeed;
					movingxy[i][1] -= yspeed;
					theta[i] += 1;
				}
				else {
					theta[i] += 10;
					movemode[i] = rand() % 4;
				}
			}
		}
		glutPostRedisplay();
		glutTimerFunc(30, angle, 0);
	}
}
float movegauge[2] = { 0.05f ,-0.05f };
float flag[4] = { 0 };
void sidetoside(int val) {
	if (timer[1]) {
		for (int i = 0; i < 4; i++) {
			glm::vec3 triangleVertex[3];
			glm::vec3 center(0.0f, 0.0f, 0.0f);
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			for (int j = 0; j < 3; j++) {
				triangleVertex[j] = glm::vec3(Trian[i][j].x, Trian[i][j].y, Trian[i][j].z);
				center += triangleVertex[j];
			}
			center /= 3.0f;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(movingxy[i][0], movingxy[i][1], 0));
			if (glm::dot(center, center) >= 0.0f) {
				modelMatrix = glm::translate(modelMatrix, center);//삼각형을 원점으로 이동
				modelMatrix = glm::rotate(modelMatrix, glm::radians(theta[i]), glm::vec3(0.0f, 0.0f, 1.0f));//회전
				modelMatrix = glm::translate(modelMatrix, -center);//삼각형을 다시 제자리로 돌려보냄
			}
			else {
				modelMatrix = glm::translate(modelMatrix, -center);//삼각형을 원점으로 이동
				modelMatrix = glm::rotate(modelMatrix, glm::radians(theta[i]), glm::vec3(0.0f, 0.0f, 1.0f));//회전
				modelMatrix = glm::translate(modelMatrix, center);//삼각형을 다시 제자리로 돌려보냄
			}
			for (int j = 0; j < 3; j++) {
				triangleVertex[j] = glm::vec3(modelMatrix * glm::vec4(triangleVertex[j], 1.0f));
			}
			int random = rand() % 2;
			float speed = (static_cast<float>(rand()) / RAND_MAX)/25;
			if (sidemode[i] == 0) {
				if ((triangleVertex[0].x < 1.0f && triangleVertex[1].x < 1.0f && triangleVertex[2].x < 1.0f)) {
					movingxy[i][0] += speed;
				}
				else {
					theta[i] = 90;
					if (random == 0) {
						sidemode[i] = 2;
					}
					else {
						sidemode[i] = 3;
					}
				}
			}
			else if (sidemode[i] == 1) {
				if ((triangleVertex[0].x > -1.0f && triangleVertex[1].x > -1.0f && triangleVertex[2].x > -1.0f)) {
					movingxy[i][0] -= speed;
				}
				else {
					theta[i] = -90;
					if (random == 0) {
						sidemode[i] = 2;
					}
					else {
						sidemode[i] = 3;
					}
				}
			}
			else if (sidemode[i] == 2) {
				if ((triangleVertex[0].y < 1.0f && triangleVertex[1].y < 1.0f && triangleVertex[2].y < 1.0f)) {
					if (flag[i] < movegauge[0]) {
						movingxy[i][1] += 0.01f;
						flag[i] += 0.01f;
					}
					else if (flag[i] > movegauge[0]) {
						if (triangleVertex[0].x > 0) {
							sidemode[i] = 1;
							flag[i] = 0;
						}
						else {
							sidemode[i] = 0;
							flag[i] = 0;
						}
					}
				}
				else {
					sidemode[i] = 3;
				}
			}
			else if (sidemode[i] == 3) {
				if ((triangleVertex[0].y > -1.0f && triangleVertex[1].y > -1.0f && triangleVertex[2].y > -1.0f)) {
					if (flag[i] > movegauge[1]) {
						movingxy[i][1] -= 0.01f;
						flag[i] -= 0.01f;
					}
					else if (flag[i] < movegauge[1]) {
						if (triangleVertex[0].x > 0) {
							sidemode[i] = 1;
							flag[i] = 0;
						}
						else {
							sidemode[i] = 0;
							flag[i] = 0;
						}
					}
				}
				else {
					sidemode[i] = 2;
				}
			}
		}
		glutPostRedisplay();
		glutTimerFunc(30, sidetoside, 0);
	}
}
float xflag[2] = { 1.0f,-1.0f };
float yflag[2] = { 1.0f,-1.0f };
float decrease[4] = { 0.0f };
void lickwall(int val) {
	if (timer[2]) {
		for (int i = 0; i < 4; i++) {
			glm::vec3 triangleVertex[3];
			glm::vec3 center(0.0f, 0.0f, 0.0f);
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			for (int j = 0; j < 3; j++) {
				triangleVertex[j] = glm::vec3(Trian[i][j].x, Trian[i][j].y, Trian[i][j].z);
				center += triangleVertex[j];
			}
			center /= 3.0f;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(movingxy[i][0], movingxy[i][1], 0));
			if (glm::dot(center, center) >= 0.0f) {
				modelMatrix = glm::translate(modelMatrix, center);//삼각형을 원점으로 이동
				modelMatrix = glm::rotate(modelMatrix, glm::radians(theta[i]), glm::vec3(0.0f, 0.0f, 1.0f));//회전
				modelMatrix = glm::translate(modelMatrix, -center);//삼각형을 다시 제자리로 돌려보냄
			}
			else {
				modelMatrix = glm::translate(modelMatrix, -center);//삼각형을 원점으로 이동
				modelMatrix = glm::rotate(modelMatrix, glm::radians(theta[i]), glm::vec3(0.0f, 0.0f, 1.0f));//회전
				modelMatrix = glm::translate(modelMatrix, center);//삼각형을 다시 제자리로 돌려보냄
			}
			for (int j = 0; j < 3; j++) {
				triangleVertex[j] = glm::vec3(modelMatrix * glm::vec4(triangleVertex[j], 1.0f));
			}
			float speed = (static_cast<float>(rand()) / RAND_MAX) / 25;
			if (lickmode[i] == 0) {
				if ((triangleVertex[0].x < xflag[0]-decrease[i] &&
					triangleVertex[1].x < xflag[0] - decrease[i] &&
					triangleVertex[2].x < xflag[0] - decrease[i])) {
					movingxy[i][0] += speed;
				}
				else {
					theta[i] = 90;
					lickmode[i] = 1;
					decrease[i] += 0.1f;
				}
			}
			else if (lickmode[i] == 1) {
				if ((triangleVertex[0].y < yflag[0] - decrease[i] &&
					triangleVertex[1].y < yflag[0] - decrease[i] &&
					triangleVertex[2].y < yflag[0] - decrease[i])) {
					movingxy[i][1] += speed;
				}
				else {
					theta[i] = 180;
					lickmode[i] = 2;
					decrease[i] += 0.1f;
				}
			}
			else if (lickmode[i] == 2) {
				if ((triangleVertex[0].x > xflag[1] + decrease[i] &&
					triangleVertex[1].x > xflag[1] + decrease[i] &&
					triangleVertex[2].x > xflag[1] + decrease[i])) {
					movingxy[i][0] -= speed;
				}
				else {
					theta[i] = 270;
					lickmode[i] = 3;
					decrease[i] += 0.1f;
				}
			}
			else if (lickmode[i] == 3) {
				if ((triangleVertex[0].y > yflag[1] + decrease[i] &&
					triangleVertex[1].y > yflag[1] + decrease[i] &&
					triangleVertex[2].y > yflag[1] + decrease[i])) {
					movingxy[i][1] -= speed;
				}
				else {
					theta[i] = 0;
					lickmode[i] = 0;
					decrease[i] += 0.1f;
				}
			}
			if(decrease[i]>1.0f){
				decrease[i] = 0.0f;
			}
		}
		glutPostRedisplay();
		glutTimerFunc(30, lickwall, 0);
	}
}
int LorR[4] = { 0,0,0,0 };
float degree = 0.0f;
int scale[4][2] = { {1,3},{1,3},{1,3},{1,3} };
int counting = 0;
void sprialmoving(int val) {
	if (timer[3]) {
		for (int i = 0; i < 4; i++) {
			glm::vec3 triangleVertex[3];//정점 변환을 받을 놈~ 잘 쓰거라~
			glm::vec3 center(0.0f, 0.0f, 0.0f);
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			glm::vec3 rotating(0.0f, 0.0f, 0.0f);
			for (int j = 0; j < 3; j++) {
				triangleVertex[j] = glm::vec3(Trian[i][j].x, Trian[i][j].y, Trian[i][j].z);
				center += triangleVertex[j];
			}
			center /= 3.0f;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(movingxy[i][0], movingxy[i][1], 0));
			if (glm::dot(center, center) >= 0.0f) {
				modelMatrix = glm::translate(modelMatrix, center);//삼각형을 원점으로 이동
				modelMatrix = glm::rotate(modelMatrix, glm::radians(theta[i]), glm::vec3(0.0f, 0.0f, 1.0f));//회전
				modelMatrix = glm::translate(modelMatrix, -center);//삼각형을 다시 제자리로 돌려보냄
			}
			else {
				modelMatrix = glm::translate(modelMatrix, -center);//삼각형을 원점으로 이동
				modelMatrix = glm::rotate(modelMatrix, glm::radians(theta[i]), glm::vec3(0.0f, 0.0f, 1.0f));//회전
				modelMatrix = glm::translate(modelMatrix, center);//삼각형을 다시 제자리로 돌려보냄
			}
			for (int j = 0; j < 3; j++) {
				triangleVertex[j] = glm::vec3(modelMatrix * glm::vec4(triangleVertex[j], 1.0f));
			}
			for (int j = 0; j < 3; j++) {
				rotating += triangleVertex[j];
			}
			rotating /= 3.0f;
			float xspeed = (static_cast<float>(rand()) / RAND_MAX) / 25;
			float yspeed = (static_cast<float>(rand()) / RAND_MAX) / 25;
			if (LorR[i] == 0) {
				if ((triangleVertex[0].x < 1.0f && triangleVertex[1].x < 1.0f && triangleVertex[2].x < 1.0f) &&
					(triangleVertex[0].y < 1.0f && triangleVertex[1].y < 1.0f && triangleVertex[2].y < 1.0f) &&
					(triangleVertex[0].x > -1.0f && triangleVertex[1].x > -1.0f && triangleVertex[2].x > -1.0f) &&
					(triangleVertex[0].y > -1.0f && triangleVertex[1].y > -1.0f && triangleVertex[2].y > -1.0f)) {
					movingxy[i][0] += 0.01 * (rotating.x + scale[i][1]++ * (glm::cos(glm::radians(degree))));
					movingxy[i][1] += 0.01 * (rotating.y + scale[i][0]++ * (glm::sin(glm::radians(degree))));
					theta[i] += 6;

					degree += 5;
				}
				else {
					theta[i] += 30;
					scale[i][0] = 1;
					scale[i][1] = 3;
					LorR[i] = 1;
				}
			}
			else if (LorR[i] == 1) {
				counting++;
				if (movemode[i] == 0) {
					if ((triangleVertex[0].x < 1.0f && triangleVertex[1].x < 1.0f && triangleVertex[2].x < 1.0f) &&
						(triangleVertex[0].y < 1.0f && triangleVertex[1].y < 1.0f && triangleVertex[2].y < 1.0f)) {
						movingxy[i][0] += xspeed;
						movingxy[i][1] += yspeed;
						theta[i] -= 1;
					}
					else {
						theta[i] += 10;
						movemode[i] = rand() % 4;
					}
				}
				else if (movemode[i] == 1) {
					if ((triangleVertex[0].x > -1.0f && triangleVertex[1].x > -1.0f && triangleVertex[2].x > -1.0f) &&
						(triangleVertex[0].y < 1.0f && triangleVertex[1].y < 1.0f && triangleVertex[2].y < 1.0f)) {
						movingxy[i][0] -= xspeed;
						movingxy[i][1] += yspeed;
						theta[i] += 1;
					}
					else {
						theta[i] += 10;
						movemode[i] = rand() % 4;
					}
				}
				else if (movemode[i] == 2) {
					if ((triangleVertex[0].x > -1.0f && triangleVertex[1].x > -1.0f && triangleVertex[2].x > -1.0f) &&
						(triangleVertex[0].y > -1.0f && triangleVertex[1].y > -1.0f && triangleVertex[2].y > -1.0f)) {
						movingxy[i][0] -= xspeed;
						movingxy[i][1] -= yspeed;
						theta[i] -= 1;
					}
					else {
						theta[i] += 10;
						movemode[i] = rand() % 4;
					}
				}
				else if (movemode[i] == 3) {
					if ((triangleVertex[0].x < 1.0f && triangleVertex[1].x < 1.0f && triangleVertex[2].x < 1.0f) &&
						(triangleVertex[0].y > -1.0f && triangleVertex[1].y > -1.0f && triangleVertex[2].y > -1.0f)) {
						movingxy[i][0] += xspeed;
						movingxy[i][1] -= yspeed;
						theta[i] += 1;
					}
					else {
						theta[i] += 10;
						movemode[i] = rand() % 4;
					}
				}
				if (counting % 30 == 0) {
					LorR[i] = 0;
				}
			}
		}
		glutTimerFunc(30, sprialmoving, 0);
		glutPostRedisplay();
	}
}

void makeTriangles() {
	for (int i = 0; i < 4; i++) { 
		float x = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
		float y = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
		sidemode[i] = rand() % 2;
		movemode[i] = rand() % 4;
		Trian[i][0].x = x;
		Trian[i][0].y = y + 0.1;

		Trian[i][1].x = x - 0.075;
		Trian[i][1].y = y - 0.1;

		Trian[i][2].x = x + 0.075;
		Trian[i][2].y = y - 0.1;
		for (int j = 0; j < 3; j++) {
			Trian[i][j].r = static_cast<float>(rand()) / RAND_MAX;
			Trian[i][j].g = static_cast<float>(rand()) / RAND_MAX;
			Trian[i][j].b = static_cast<float>(rand()) / RAND_MAX;
		}
	}
}
GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_triangles();
	glutSwapBuffers();
}
void draw_triangles() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);

	glGenBuffers(4, vbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	for (int i = 0; i < 4; i++) {
		glm::vec3 center(0.0f, 0.0f, 0.0f);
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		for (int j = 0; j < 3; j++) {
			center += glm::vec3(Trian[i][j].x, Trian[i][j].y, Trian[i][j].z);
		}
		modelMatrix = glm::translate(modelMatrix, glm::vec3(movingxy[i][0], movingxy[i][1], 0));
		center = glm::vec3(center.x / 3.0f, center.y / 3.0f, center.z / 3.0f);
		if (glm::dot(center, center) >= 0.0f) {
			modelMatrix = glm::translate(modelMatrix, center);//삼각형을 원점으로 이동
			modelMatrix = glm::rotate(modelMatrix, glm::radians(theta[i]), glm::vec3(0.0f, 0.0f, 1.0f));//회전
			modelMatrix = glm::translate(modelMatrix, -center);//삼각형을 다시 제자리로 돌려보냄
		}
		else {
			modelMatrix = glm::translate(modelMatrix, -center);//삼각형을 원점으로 이동
			modelMatrix = glm::rotate(modelMatrix, glm::radians(theta[i]), glm::vec3(0.0f, 0.0f, 1.0f));//회전
			modelMatrix = glm::translate(modelMatrix, center);//삼각형을 다시 제자리로 돌려보냄
		}
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		//중요한점:: 행렬 곱 순서를 다르게하면 결과 또한 다르다 잊지말자!

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), Trian[i], GL_STATIC_DRAW);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}
}
GLvoid Reshape(int w, int h){
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