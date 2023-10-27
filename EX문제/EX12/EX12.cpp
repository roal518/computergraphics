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


struct coord {
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
};
//기본 정보 일단 기초적인 틀만 구성하기 위해 이렇게만 한다.

coord dot[8];
bool dotmove[8];
bool dotsure[8];
bool ismake1[8];

coord line[8][2];
bool linemove[8];
bool linesure[8];
bool ismake2[8];
int iscase2[8];

coord trian[8][3];
bool trianmove[8];
bool triansure[8];
bool ismake3[8];
int iscase3[8];

coord rect[8][4];
bool rectmove[8];
bool rectsure[8];
bool ismake4[8];
int iscase4[8];

coord penta[8][5];
bool pentamove[8];
bool pentasure[8];
bool ismake5[8];
int iscase5[8];

coord hexa[8][6];
bool hexamove[8];
bool hexasure[8];
bool ismake6[8];
int iscase6[8];



int iscase1[8];
int linedice[2] = { 0,1 };
int trindice[3] = { 0,1,2 };
int rectdice[4] = { 0,1,2,3 };
int pentdice[5] = { 0,1,2,3,4};
int hexadice[6] = { 0,1,2,3,4,5 };
GLuint vao, vbo, ebo;

void makeshape();
void drawDot();
void drawLine();
void drawTrian();
void drawRect();
void drawPenta();
void drawhexa();
void initBuffer(int i);
void getinfo(int val);

void mouse(int button, int state, int x, int y);
void mousemove(int x, int y);
void getmove(float dx, float dy);

bool checkdot(float normalX, float normalY, int i);
bool checkline(float normalX, float normalY, int i);
bool checktriangle(float x, float y, int i);
bool checkrectangles(float normalX, float normalY, int i);
bool checkpentagon(float x, float y, int i);
bool checkhexagon(float x, float y, int i);

GLvoid Reshape(int w, int h);
GLvoid drawScene();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);

bool isclicked = false;

void main(int argc, char** argv) {
	srand(unsigned(time(NULL)));
	makeshape();
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
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);

	glutMainLoop();
}
float mouseX;
float mouseY;
int shapetype = -1;
int index = -1;
bool ischange = false;
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float normalX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
		float normalY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
		index = -1;
		shapetype = -1;
		for (int i = 0; i < 8; i++) {
			if (checkdot(normalX, normalY, i)) {
				dotmove[i] = true;
				linemove[i] = false;
				trianmove[i] = false;
				rectmove[i] = false;
				pentamove[i] = false;
				hexamove[i] = false;
				isclicked = true;
				index = i;
				shapetype = 1;
				printf("shapetype: %d index:  %d \n", shapetype, index);
			}
			else if (checkline(normalX, normalY, i)) {
				dotmove[i] = false;
				linemove[i] = true;
				trianmove[i] = false;
				rectmove[i] = false;
				pentamove[i] = false;
				hexamove[i] = false;
				isclicked = true;
				index = i;
				shapetype = 2;
				printf("shapetype: %d index:  %d \n", shapetype, index);

			}
			else if (checktriangle(normalX, normalY, i)) {
				dotmove[i] = false;
				linemove[i] = false;
				trianmove[i] = true;
				rectmove[i] = false;
				pentamove[i] = false;
				hexamove[i] = false;
				isclicked = true;

				index = i;
				shapetype = 3;
				printf("shapetype: %d index:  %d \n", shapetype, index);

			}
			else if (checkrectangles(normalX, normalY, i)) {
				dotmove[i] = false;
				linemove[i] = false;
				trianmove[i] = false;
				rectmove[i] = true;
				pentamove[i] = false;
				hexamove[i] = false;
				isclicked = true;

				index = i;
				shapetype = 4;
				printf("shapetype: %d index:  %d \n", shapetype, index);

			}
			else if (checkpentagon(normalX, normalY, i)) {
				dotmove[i] = false;
				linemove[i] = false;
				trianmove[i] = false;
				rectmove[i] = false;
				pentamove[i] = true;
				hexamove[i] = false;
				isclicked = true;

				index = i;
				shapetype = 5;
				printf("shapetype: %d index:  %d \n", shapetype, index);

			}
			else if (checkhexagon(normalX, normalY, i)) {
				dotmove[i] = false;
				linemove[i] = false;
				trianmove[i] = false;
				rectmove[i] = false;
				pentamove[i] = false;

				hexamove[i] = true;
				isclicked = true;

				index = i;
				shapetype = 6;
				printf("shapetype: %d index:  %d \n", shapetype, index);
			}
			else {
				dotmove[i] = false;
				linemove[i] = false;
				trianmove[i] = false;
				rectmove[i] = false;
				pentamove[i] = false;
				hexamove[i] = false;
			}
		}
		mouseX = x;
		mouseY = y;
	//	ischange = false;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		isclicked = false;
		if (index != -1 && shapetype != -1) {
			if (shapetype == 1) {//dot
				float midX = dot[index].x;
				float midY = dot[index].y;
				//index=>마우스도형
				//i => 겹치는도형
				for (int i = 0; i < 8; i++) {
					if (checkdot(midX, midY, i)) {
						if (i != index) {
							dotsure[index] = false;
							dotsure[i] = false;
							for (int n = 0; n < 8; n++) {
								if (linesure[n] == false) {
									linesure[n] = true;
									ismake2[n] = true;
									dot[i].x = 2;
									dot[i].y = 2;
									dot[index].x = 2;
									dot[index].y = 2;
									line[n][0].x = midX - 0.2;
									line[n][0].y = midY;
									line[n][1].x = midX + 0.2;
									line[n][1].y = midY;
									ismake2[n] = true;
									break;
								}
							}
							break;
						}
					}
					if (checkline(midX, midY, i)) {
						dotsure[index] = false;
						linesure[i] = false;
						for (int n = 0; n < 8; n++) {
							if (triansure[n] == false) {
								triansure[n] = true;
								ismake3[n] = true;

								dot[index].x = 2;
								dot[index].y = 2;
								line[i][0].x = 2;
								line[i][0].y = 2;
								line[i][1].x = 2;
								line[i][1].y = 2;
								trian[n][0].x = midX;
								trian[n][0].y = midY + 0.2;
								trian[n][1].x = midX - 0.15;
								trian[n][1].y = midY - 0.1;
								trian[n][2].x = midX + 0.15;
								trian[n][2].y = midY - 0.1;
								break;
							}
						}
						break;
					}
					if (checktriangle(midX, midY, i)) {
						dotsure[index] = false;
						triansure[i] = false;
						for (int n = 0; n < 8; n++) {
							if (rectsure[n] == false) {
								rectsure[n] = true;
								ismake4[n] = true;

								dot[index].x = 2;
								dot[index].y = 2;
								trian[i][0].x = 2;
								trian[i][0].y = 2;
								trian[i][1].x = 2;
								trian[i][1].y = 2;
								trian[i][2].x = 2;
								trian[i][2].y = 2;

								rect[n][0].x = midX - 0.1;
								rect[n][0].y = midY + 0.1;
								rect[n][1].x = midX - 0.1;
								rect[n][1].y = midY - 0.1;
								rect[n][2].x = midX + 0.1;
								rect[n][2].y = midY - 0.1;
								rect[n][3].x = midX + 0.1;
								rect[n][3].y = midY + 0.1;
								break;
							}
						}
						break;
					}
					if (checkrectangles(midX, midY, i)) {
						dotsure[index] = false;
						rectsure[i] = false;
						for (int n = 0; n < 8; n++) {
							if (pentasure[n] == false) {
								pentasure[n] = true;
								ismake5[n] = true;

								dot[index].x = 2;
								dot[index].y = 2;
								rect[i][0].x = 2;
								rect[i][0].y = 2;
								rect[i][1].x = 2;
								rect[i][1].y = 2;
								rect[i][2].x = 2;
								rect[i][2].y = 2;
								rect[i][3].x = 2;
								rect[i][3].y = 2;

								penta[n][0].x = midX;
								penta[n][0].y = midY + 0.2;

								penta[n][1].x = midX - 0.15;
								penta[n][1].y = midY + 0.1;

								penta[n][2].x = midX - 0.1;
								penta[n][2].y = midY - 0.1;

								penta[n][3].x = midX + 0.1;
								penta[n][3].y = midY - 0.1;

								penta[n][4].x = midX + 0.15;
								penta[n][4].y = midY + 0.1;
								break;
							}
						}
						break;
					}
					if (checkpentagon(midX, midY, i)) {
						dotsure[index] = false;
						pentasure[i] = false;
						for (int n = 0; n < 8; n++) {
							if (hexasure[n] == false) {
								hexasure[n] = true;
								ismake6[n] = true;

								dot[index].x = 2;
								dot[index].y = 2;
								for (int j = 0; j < 5; j++) {
									penta[i][j].x = 2;
									penta[i][j].y = 2;
								}
								hexa[n][0].x = midX - 0.1;
								hexa[n][0].y = midY + 0.2;

								hexa[n][1].x = midX - 0.2;
								hexa[n][1].y = midY;

								hexa[n][2].x = midX - 0.1;
								hexa[n][2].y = midY - 0.2;

								hexa[n][3].x = midX + 0.1;
								hexa[n][3].y = midY - 0.2;

								hexa[n][4].x = midX + 0.2;
								hexa[n][4].y = midY;

								hexa[n][5].x = midX + 0.1;
								hexa[n][5].y = midY + 0.2;
								break;
							}
						}
						break;
					}
					if (checkhexagon(midX, midY, i)) {
						hexasure[i] = false;
						ismake1[index] = true;
						for (int n = 0; n < 6; n++) {
							hexa[i][n].x = 2;
							hexa[i][n].y = 2;
						}
						break;
					}
				}
			}
			else if (shapetype == 2) {
				float midX = line[index][0].x + 0.2f;
				float midY = line[index][0].y;
				for (int i = 0; i < 8; i++) {
					if (checkline(midX, midY, i)) {
						if (i != index) {
							linesure[index] = false;
							linesure[i] = false;
							for (int n = 0; n < 8; n++) {
								if (rectsure[n] == false) {
									rectsure[n] = true;
									ismake4[n] = true;

									for (int p = 0; p < 2; p++) {
										line[index][p].x = 2;
										line[index][p].y = 2;
										line[i][p].x = 2;
										line[i][p].y = 2;
									}
									rect[n][0].x = midX - 0.1;
									rect[n][0].y = midY + 0.1;

									rect[n][1].x = midX - 0.1;
									rect[n][1].y = midY - 0.1;

									rect[n][2].x = midX + 0.1;
									rect[n][2].y = midY - 0.1;

									rect[n][3].x = midX + 0.1;
									rect[n][3].y = midY + 0.1;
									break;
								}
							}
							break;
						}
					}
					if (checkdot(midX, midY, i)) {
						linesure[index] = false;
						dotsure[i] = false;
						for (int n = 0; n < 8; n++) {
							if (triansure[n] == false) {
								triansure[n] = true;
								ismake3[n] = true;

								for (int p = 0; p < 2; p++) {
									line[index][p].x = 2;
									line[index][p].y = 2;
									dot[i].x = 2;
									dot[i].y = 2;
								}
								trian[n][0].x = midX;
								trian[n][0].y = midY + 0.2;

								trian[n][1].x = midX - 0.15;
								trian[n][1].y = midY - 0.1;

								trian[n][2].x = midX + 0.15;
								trian[n][2].y = midY - 0.1;
								break;
							}
						}
						break;
					}
					if (checktriangle(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (pentasure[n] == false) {
								pentasure[n] = true;
								ismake5[n] = true;

								linesure[index] = false;
								triansure[i] = false;
								for (int p = 0; p < 2; p++) {
									line[index][p].x = 2;
									line[index][p].y = 2;
								}
								for (int p = 0; p < 3; p++) {
									trian[i][p].x = 2;
									trian[i][p].y = 2;
								}
								penta[n][0].x = midX;
								penta[n][0].y = midY + 0.2;

								penta[n][1].x = midX - 0.15;
								penta[n][1].y = midY + 0.1;

								penta[n][2].x = midX - 0.1;
								penta[n][2].y = midY - 0.1;

								penta[n][3].x = midX + 0.1;
								penta[n][3].y = midY - 0.1;

								penta[n][4].x = midX + 0.15;
								penta[n][4].y = midY + 0.1;
								break;
							}
						}
						break;
					}
					if (checkrectangles(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (hexasure[n] == false) {
								hexasure[n] = true;
								ismake6[n] = true;

								linesure[index] = false;
								rectsure[i] = false;
								for (int p = 0; p < 2; p++) {
									line[index][p].x = 2;
									line[index][p].y = 2;
								}
								for (int p = 0; p < 3; p++) {
									rect[i][p].x = 2;
									rect[i][p].y = 2;
								}
								hexa[n][0].x = midX - 0.1;
								hexa[n][0].y = midY + 0.2;

								hexa[n][1].x = midX - 0.2;
								hexa[n][1].y = midY;

								hexa[n][2].x = midX - 0.1;
								hexa[n][2].y = midY - 0.2;

								hexa[n][3].x = midX + 0.1;
								hexa[n][3].y = midY - 0.2;

								hexa[n][4].x = midX + 0.2;
								hexa[n][4].y = midY;

								hexa[n][5].x = midX + 0.1;
								hexa[n][5].y = midY + 0.2;

								break;
							}
						}
						break;
					}
					if (checkpentagon(midX, midY, i)) {
						pentasure[i] = false;
						linesure[index] = false;
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 2; p++) {
									line[index][p].x = 2;
									line[index][p].y = 2;
								}
								for (int p = 0; p < 5; p++) {
									penta[i][p].x = 2;
									penta[i][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
					if (checkhexagon(midX, midY, i)) {
						hexasure[i] = false;
						linesure[index] = false;
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 2; p++) {
									line[index][p].x = 2;
									line[index][p].y = 2;
								}
								for (int p = 0; p < 6; p++) {
									hexa[i][p].x = 2;
									hexa[i][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
				}
			}
			else if (shapetype == 3) {
				float midX = trian[index][0].x;
				float midY = trian[index][0].y - 0.2f;
				for (int i = 0; i < 8; i++) {
					if (checktriangle(midX, midY, i)) {
						if (i != index) {
							for (int n = 0; n < 8; n++) {
								if (hexasure[n] == false) {
									hexasure[n] = true;
									ismake6[n] = true;
									triansure[index] = false;
									triansure[i] = false;
									for (int p = 0; p < 2; p++) {
										trian[index][p].x = 2;
										trian[index][p].y = 2;
										trian[i][p].x = 2;
										trian[i][p].y = 2;
									}
									hexa[n][0].x = midX - 0.1;
									hexa[n][0].y = midY + 0.2;

									hexa[n][1].x = midX - 0.2;
									hexa[n][1].y = midY;

									hexa[n][2].x = midX - 0.1;
									hexa[n][2].y = midY - 0.2;

									hexa[n][3].x = midX + 0.1;
									hexa[n][3].y = midY - 0.2;

									hexa[n][4].x = midX + 0.2;
									hexa[n][4].y = midY;

									hexa[n][5].x = midX + 0.1;
									hexa[n][5].y = midY + 0.2;
									break;
								}
							}
							break;
						}
					}
					if (checkdot(midX, midY, i)) {
						dotsure[i] = false;
						triansure[index] = false;
						for (int n = 0; n < 8; n++) {
							if (rectsure[n] == false) {
								rectsure[n] = true;
								ismake4[n] = true;

								dot[i].x = 2;
								dot[i].y = 2;
								trian[index][0].x = 2;
								trian[index][0].y = 2;
								trian[index][1].x = 2;
								trian[index][1].y = 2;
								trian[index][2].x = 2;
								trian[index][2].y = 2;

								rect[n][0].x = midX - 0.1;
								rect[n][0].y = midY + 0.1;
								rect[n][1].x = midX - 0.1;
								rect[n][1].y = midY - 0.1;
								rect[n][2].x = midX + 0.1;
								rect[n][2].y = midY - 0.1;
								rect[n][3].x = midX + 0.1;
								rect[n][3].y = midY + 0.1;
								break;
							}
						}
						break;
					}
					if (checkline(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (pentasure[n] == false) {
								pentasure[n] = true;
								ismake5[n] = true;

								linesure[i] = false;
								triansure[index] = false;
								for (int p = 0; p < 2; p++) {
									line[i][p].x = 2;
									line[i][p].y = 2;
								}
								for (int p = 0; p < 3; p++) {
									trian[index][p].x = 2;
									trian[index][p].y = 2;
								}
								penta[n][0].x = midX;
								penta[n][0].y = midY + 0.2;

								penta[n][1].x = midX - 0.15;
								penta[n][1].y = midY + 0.1;

								penta[n][2].x = midX - 0.1;
								penta[n][2].y = midY - 0.1;

								penta[n][3].x = midX + 0.1;
								penta[n][3].y = midY - 0.1;

								penta[n][4].x = midX + 0.15;
								penta[n][4].y = midY + 0.1;
								break;
							}
						}
						break;
					}
					if (checkrectangles(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								dotsure[n] = true;
								ismake1[n] = true;

								triansure[index] = false;
								rectsure[i] = false;
								for (int p = 0; p < 3; p++) {
									trian[index][p].x = 2;
									trian[index][p].y = 2;
								}
								for (int p = 0; p < 4; p++) {
									rect[i][p].x = 2;
									rect[i][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
					if (checkpentagon(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								dotsure[n] = true;
								ismake1[n] = true;

								triansure[index] = false;
								pentasure[i] = false;
								for (int p = 0; p < 3; p++) {
									trian[index][p].x = 2;
									trian[index][p].y = 2;
								}
								for (int p = 0; p < 5; p++) {
									penta[i][p].x = 2;
									penta[i][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
					if (checkhexagon(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								dotsure[n] = true;
								ismake1[n] = true;

								triansure[index] = false;
								hexasure[i] = false;
								for (int p = 0; p < 3; p++) {
									trian[index][p].x = 2;
									trian[index][p].y = 2;
								}
								for (int p = 0; p < 6; p++) {
									hexa[i][p].x = 2;
									hexa[i][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
				}
			}
			else if (shapetype == 4) {
				float midX = rect[index][0].x + 0.1f;
				float midY = rect[index][0].y - 0.1f;
				for (int i = 0; i < 8; i++) {
					if (checkrectangles(midX, midY, i)) {
						if (i != index) {
							for (int n = 0; n < 8; n++) {
								if (dotsure[n] == false) {
									dotsure[n] = true;
									ismake1[n] = true;

									rectsure[index] = false;
									rectsure[i] = false;
									for (int p = 0; p < 4; p++) {
										rect[index][p].x = 2;
										rect[index][p].y = 2;
										rect[i][p].x = 2;
										rect[i][p].y = 2;
									}
									dot[n].x = midX;
									dot[n].y = midY;
									break;
								}
							}
							break;
						}
					}
					if (checkdot(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (pentasure[n] == false) {
								dotsure[i] = false;
								rectsure[index] = false;
								pentasure[n] = true;
								ismake5[n] = true;

								dot[i].x = 2;
								dot[i].y = 2;
								rect[index][0].x = 2;
								rect[index][0].y = 2;
								rect[index][1].x = 2;
								rect[index][1].y = 2;
								rect[index][2].x = 2;
								rect[index][2].y = 2;
								rect[index][3].x = 2;
								rect[index][3].y = 2;

								penta[n][0].x = midX;
								penta[n][0].y = midY + 0.2;

								penta[n][1].x = midX - 0.15;
								penta[n][1].y = midY + 0.1;

								penta[n][2].x = midX - 0.1;
								penta[n][2].y = midY - 0.1;

								penta[n][3].x = midX + 0.1;
								penta[n][3].y = midY - 0.1;

								penta[n][4].x = midX + 0.15;
								penta[n][4].y = midY + 0.1;
								break;
							}
						}
						break;
					}
					if (checkline(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (hexasure[n] == false) {
								hexasure[n] = true;
								ismake6[n] = true;

								linesure[i] = false;
								rectsure[index] = false;
								for (int p = 0; p < 2; p++) {
									line[i][p].x = 2;
									line[i][p].y = 2;
								}
								for (int p = 0; p < 3; p++) {
									rect[index][p].x = 2;
									rect[index][p].y = 2;
								}
								hexa[n][0].x = midX - 0.1;
								hexa[n][0].y = midY + 0.2;

								hexa[n][1].x = midX - 0.2;
								hexa[n][1].y = midY;

								hexa[n][2].x = midX - 0.1;
								hexa[n][2].y = midY - 0.2;

								hexa[n][3].x = midX + 0.1;
								hexa[n][3].y = midY - 0.2;

								hexa[n][4].x = midX + 0.2;
								hexa[n][4].y = midY;

								hexa[n][5].x = midX + 0.1;
								hexa[n][5].y = midY + 0.2;

								break;
							}
						}
						break;
					}
					if (checktriangle(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								rectsure[index] = false;
								triansure[i] = false;
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 4; p++) {
									rect[index][p].x = 2;
									rect[index][p].y = 2;
								}
								for (int p = 0; p < 3; p++) {
									trian[i][p].x = 2;
									trian[i][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
					if (checkpentagon(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								rectsure[index] = false;
								pentasure[i] = false;
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 4; p++) {
									rect[index][p].x = 2;
									rect[index][p].y = 2;
								}
								for (int p = 0; p < 5; p++) {
									penta[i][p].x = 2;
									penta[i][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
					if (checkhexagon(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								rectsure[index] = false;
								hexasure[i] = false;
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 4; p++) {
									rect[index][p].x = 2;
									rect[index][p].y = 2;
								}
								for (int p = 0; p < 6; p++) {
									hexa[i][p].x = 2;
									hexa[i][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
				}
			}
			else if (shapetype == 5) {
				float midX = penta[index][0].x;
				float midY = penta[index][0].y - 0.2f;
				for (int i = 0; i < 8; i++) {
					if (checkpentagon(midX, midY, i)) {
						if (i != index) {
							for (int n = 0; n < 8; n++) {
								if (dotsure[n] == false) {
									dotsure[n] = true;
									pentasure[index] = false;
									pentasure[i] = false;
									ismake1[n] = true;

									for (int p = 0; p < 4; p++) {
										penta[index][p].x = 2;
										penta[index][p].y = 2;
										penta[i][p].x = 2;
										penta[i][p].y = 2;
									}
									dot[n].x = midX;
									dot[n].y = midY;
									break;
								}
							}
							break;
						}
					}
					if (checkdot(midX, midY, i)) {
						pentasure[index] = false;
						ismake1[i] = true;

						for (int p = 0; p < 5; p++) {
							penta[index][p].x = 2;
							penta[index][p].y = 2;
						}
						break;
					}
					if (checkline(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								pentasure[index] = false;
								linesure[i] = false;
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 2; p++) {
									line[i][p].x = 2;
									line[i][p].y = 2;
								}
								for (int p = 0; p < 5; p++) {
									penta[index][p].x = 2;
									penta[index][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
					if (checktriangle(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								pentasure[index] = false;
								triansure[i] = false;
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 3; p++) {
									trian[i][p].x = 2;
									trian[i][p].y = 2;
								}
								for (int p = 0; p < 5; p++) {
									penta[index][p].x = 2;
									penta[index][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
					if (checkrectangles(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								pentasure[index] = false;
								rectsure[i] = false;
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 4; p++) {
									rect[i][p].x = 2;
									rect[i][p].y = 2;
								}
								for (int p = 0; p < 5; p++) {
									penta[index][p].x = 2;
									penta[index][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
					if (checkhexagon(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								pentasure[index] = false;
								hexasure[i] = false;
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 6; p++) {
									hexa[i][p].x = 2;
									hexa[i][p].y = 2;
								}
								for (int p = 0; p < 5; p++) {
									penta[index][p].x = 2;
									penta[index][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
						}
						break;
					}
				}
			}
			else if (shapetype == 6) {
				float midX = hexa[index][0].x + 0.1;
				float midY = hexa[index][0].y - 0.2f;
				for (int i = 0; i < 8; i++) {
					if (checkdot(midX, midY, i)) {
						hexasure[index] = false;
						ismake1[i] = true;

						for (int p = 0; p < 5; p++) {
							hexa[index][p].x = 2;
							hexa[index][p].y = 2;
						}
						break;
					}
					if (checkline(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								hexasure[index] = false;
								linesure[i] = false;
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 2; p++) {
									line[i][p].x = 2;
									line[i][p].y = 2;
								}
								for (int p = 0; p < 6; p++) {
									hexa[index][p].x = 2;
									hexa[index][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
							break;
						}
					}
					if (checktriangle(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								hexasure[index] = false;
								triansure[i] = false;
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 3; p++) {
									trian[i][p].x = 2;
									trian[i][p].y = 2;
								}
								for (int p = 0; p < 6; p++) {
									hexa[index][p].x = 2;
									hexa[index][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
							break;
						}
					}
					if (checkrectangles(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								hexasure[index] = false;
								rectsure[i] = false;
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 4; p++) {
									rect[i][p].x = 2;
									rect[i][p].y = 2;
								}
								for (int p = 0; p < 6; p++) {
									hexa[index][p].x = 2;
									hexa[index][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
							break;
						}
					}
					if (checkpentagon(midX, midY, i)) {
						for (int n = 0; n < 8; n++) {
							if (dotsure[n] == false) {
								hexasure[index] = false;
								pentasure[i] = false;
								dotsure[n] = true;
								ismake1[n] = true;

								for (int p = 0; p < 5; p++) {
									penta[i][p].x = 2;
									penta[i][p].y = 2;
								}
								for (int p = 0; p < 6; p++) {
									hexa[index][p].x = 2;
									hexa[index][p].y = 2;
								}
								dot[n].x = midX;
								dot[n].y = midY;
								break;
							}
							break;
						}
					}
					if (checkhexagon(midX, midY, i)) {
						if (i != index) {
							for (int n = 0; n < 8; n++) {
								if (dotsure[n] == false) {
									hexasure[index] = false;
									hexasure[i] = false;
									dotsure[n] = true;
									ismake1[n] = true;

									for (int p = 0; p < 6; p++) {

										printf("true\n");
										hexa[i][p].x = 2;
										hexa[i][p].y = 2;
										hexa[index][p].x = 2;
										hexa[index][p].y = 2;
									}
									dot[n].x = midX;
									dot[n].y = midY;
									break;
								}
							}
							break;
						}
					}
				}
			}
			ischange = true;
			glutTimerFunc(30, getinfo, 0);
		}
		else {
			printf("now\t%d\t%d\n", index, shapetype);
		}
	}
}
void getinfo(int val) {
	if (ischange == true) {
		for (int i = 0; i < 8; i++) {
			if (ismake1[i]) {
				if (iscase1[i] == 0) {
					if (dot[i].x < 1.0f && dot[i].y < 1.0f) {
						dot[i].x += 0.001f;
						dot[i].y += 0.001f;
					}
					else {
						iscase1[i] = rand() % 3 + 1;
					}
				}
				if (iscase1[i] == 1) {
					if (dot[i].x < 1.0f && -1.0f < dot[i].y) {
						dot[i].x += 0.001f;
						dot[i].y -= 0.001f;
					}
					else {
						iscase1[i] = rand() % 2 + 2;
					}
				}
				if (iscase1[i] == 2) {
					if (-1.0f < dot[i].x && -1.0f < dot[i].y) {
						dot[i].x -= 0.001f;
						dot[i].y -= 0.001f;
					}
					else {
						iscase1[i] = rand() % 4;
					}
				}
				if (iscase1[i] == 3) {
					if (-1.0f < dot[i].x && dot[i].y < 1.0f) {
						dot[i].x -= 0.001f;
						dot[i].y += 0.001f;
					}
					else {
						iscase1[i] = rand() % 3;
					}
				}
			}
			if (ismake2[i]) {
				float midX = line[i][0].x + 0.2f;
				float midY = line[i][0].y;
				if (iscase2[i] == 0) {
					if (midX < 1.0f && midY < 1.0f) {
						for (int j = 0; j < 2; j++) {
							line[i][j].x += 0.001f;
							line[i][j].y += 0.001f;
						}
					}
					else {
						iscase2[i] = rand() % 3 + 1;
					}
				}
				if (iscase2[i] == 1) {
					if (midX< 1.0f && -1.0f < midY) {
						for (int j = 0; j < 2; j++) {
							line[i][j].x += 0.001f;
							line[i][j].y -= 0.001f;
						}
					}
					else {
						iscase2[i] = rand() % 2 + 2;
					}
				}
				if (iscase2[i] == 2) {
					if (-1.0f < midX && -1.0f < midY) {
						for (int j = 0; j < 2; j++) {
							line[i][j].x -= 0.001f;
							line[i][j].y -= 0.001f;
						}
					}
					else {
						iscase2[i] = rand() % 4;
					}
				}
				if (iscase2[i] == 3) {
					if (-1.0f < midX && midY < 1.0f) {
						for (int j = 0; j < 2; j++) {
							line[i][j].x -= 0.001f;
							line[i][j].y += 0.001f;
						}
					}
					else {
						iscase2[i] = rand() % 3;
					}
				}
			}
			if (ismake3[i]) {
				float midX = trian[i][0].x;
				float midY = trian[i][0].y - 0.2f;				
				if (iscase3[i] == 0) {
					if (midX < 1.0f && midY < 1.0f) {
						for (int j = 0; j < 3; j++) {
							trian[i][j].x += 0.001f;
							trian[i][j].y += 0.001f;
						}
					}
					else {
						iscase3[i] = rand() % 3 + 1;
					}
				}
				if (iscase3[i] == 1) {
					if (midX < 1.0f && -1.0f < midY) {
						for (int j = 0; j < 3; j++) {
							trian[i][j].x += 0.001f;
							trian[i][j].y -= 0.001f;
						}
					}
					else {
						iscase3[i] = rand() % 2 + 2;
					}
				}
				if (iscase3[i] == 2) {
					if (-1.0f < midX && -1.0f < midY) {
						for (int j = 0; j < 3; j++) {
							trian[i][j].x -= 0.001f;
							trian[i][j].y -= 0.001f;
						}
					}
					else {
						iscase3[i] = rand() % 4;
					}
				}
				if (iscase3[i] == 3) {
					if (-1.0f < midX && midY < 1.0f) {
						for (int j = 0; j < 3; j++) {
							trian[i][j].x -= 0.001f;
							trian[i][j].y += 0.001f;
						}
					}
					else {
						iscase3[i] = rand() % 3;
					}
				}
			}
			if (ismake4[i]) {
				float midX = rect[i][0].x + 0.1f;
				float midY = rect[i][0].y - 0.1f;
				if (iscase4[i] == 0) {
					if (midX < 1.0f && midY < 1.0f) {
						for (int j = 0; j < 4; j++) {
							rect[i][j].x += 0.001f;
							rect[i][j].y += 0.001f;
						}
					}
					else {
						iscase4[i] = rand() % 3 + 1;
					}
				}
				if (iscase4[i] == 1) {
					if (midX < 1.0f && -1.0f < midY) {
						for (int j = 0; j < 4; j++) {
							rect[i][j].x += 0.001f;
							rect[i][j].y -= 0.001f;
						}
					}
					else {
						iscase4[i] = rand() % 2 + 2;
					}
				}
				if (iscase4[i] == 2) {
					if (-1.0f < midX && -1.0f < midY) {
						for (int j = 0; j < 4; j++) {
							rect[i][j].x -= 0.001f;
							rect[i][j].y -= 0.001f;
						}
					}
					else {
						iscase4[i] = rand() % 4;
					}
				}
				if (iscase4[i] == 3) {
					if (-1.0f < midX && midY < 1.0f) {
						for (int j = 0; j < 4; j++) {
							rect[i][j].x -= 0.001f;
							rect[i][j].y += 0.001f;
						}
					}
					else {
						iscase4[i] = rand() % 3;
					}
				}
			}
			if (ismake5[i]) {
				float midX = penta[i][0].x;
				float midY = penta[i][0].y - 0.2f;
				if (iscase5[i] == 0) {
					if (midX < 1.0f && midY < 1.0f) {
						for (int j = 0; j < 5; j++) {
							penta[i][j].x += 0.001f;
							penta[i][j].y += 0.001f;
						}
					}
					else {
						iscase5[i] = rand() % 3 + 1;
					}
				}
				if (iscase4[i] == 1) {
					if (midX < 1.0f && -1.0f < midY) {
						for (int j = 0; j < 5; j++) {
							penta[i][j].x += 0.001f;
							penta[i][j].y -= 0.001f;
						}
					}
					else {
						iscase5[i] = rand() % 2 + 2;
					}
				}
				if (iscase5[i] == 2) {
					if (-1.0f < midX && -1.0f < midY) {
						for (int j = 0; j < 5; j++) {
							penta[i][j].x -= 0.001f;
							penta[i][j].y -= 0.001f;
						}
					}
					else {
						iscase5[i] = rand() % 4;
					}
				}
				if (iscase5[i] == 3) {
					if (-1.0f < midX && midY < 1.0f) {
						for (int j = 0; j < 5; j++) {
							penta[i][j].x -= 0.001f;
							penta[i][j].y += 0.001f;
						}
					}
					else {
						iscase5[i] = rand() % 3;
					}
				}
			}
			if (ismake6[i]) {
				float midX = hexa[i][0].x + 0.1;
				float midY = hexa[i][0].y - 0.2f;
				if (iscase6[i] == 0) {
					if (midX < 1.0f && midY < 1.0f) {
						for (int j = 0; j < 6; j++) {
							hexa[i][j].x += 0.001f;
							hexa[i][j].y += 0.001f;
						}
					}
					else {
						iscase6[i] = rand() % 3 + 1;
					}
				}
				if (iscase6[i] == 1) {
					if (midX < 1.0f && -1.0f < midY) {
						for (int j = 0; j < 6; j++) {
							hexa[i][j].x += 0.001f;
							hexa[i][j].y -= 0.001f;
						}
					}
					else {
						iscase6[i] = rand() % 2 + 2;
					}
				}
				if (iscase6[i] == 2) {
					if (-1.0f < midX && -1.0f < midY) {
						for (int j = 0; j < 6; j++) {
							hexa[i][j].x -= 0.001f;
							hexa[i][j].y -= 0.001f;
						}
					}
					else {
						iscase6[i] = rand() % 4;
					}
				}
				if (iscase6[i] == 3) {
					if (-1.0f < midX && midY < 1.0f) {
						for (int j = 0; j < 6; j++) {
							hexa[i][j].x -= 0.001f;
							hexa[i][j].y += 0.001f;
						}
					}
					else {
						iscase6[i] = rand() % 3;
					}
				}
			}
		}
		glutPostRedisplay();
		glutTimerFunc(30, getinfo, 0);
	}
	else {

	}

}
void mousemove(int x, int y) {
	if (isclicked) {
		float deltaX = (x - mouseX) / 400.0f;
		float deltaY = -(y - mouseY) / 300.0f;
		getmove(deltaX, deltaY);
		mouseX = x;
		mouseY = y;
		glutPostRedisplay();
	}
}
void getmove(float dx, float dy) {
	int target = 0;//target get type of  shape ex)dot,line,...etc
	for (int i = 0; i < 8; i++) {
		if (dotmove[i]) {
			dot[i].x += dx;
			dot[i].y += dy;
		}
		else if (linemove[i]) {
			for (int j = 0; j < 2; j++) {
				line[i][j].x += dx;
				line[i][j].y += dy;		
			}
		}
		else if (trianmove[i]) {
			for (int j = 0; j < 3; j++) {
				trian[i][j].x += dx;
				trian[i][j].y += dy;
			}
		}
		else if (rectmove[i]) {
			for (int j = 0; j < 4; j++) {
				rect[i][j].x += dx;
				rect[i][j].y += dy;
			}
		}
		else if (pentamove[i]) {
			for (int j = 0; j < 5; j++) {
				penta[i][j].x += dx;
				penta[i][j].y += dy;
			}
		}
		else if (hexamove[i]) {
			for (int j = 0; j < 6; j++) {
				hexa[i][j].x += dx;
				hexa[i][j].y += dy;
			}
		}
	}
}

//점 폴리곤 내부 검사 
bool checkdot(float normalX, float normalY, int i) {
	if ((normalX <dot[i].x + 0.01 && normalX>dot[i].x - 0.01) &&
		(normalY <dot[i].y + 0.01 && normalY>dot[i].y - 0.01)) {
		return true;
	}
	else {
		return false;
	}
}
bool checkline(float normalX, float normalY, int i) {
	if ((normalX > line[i][0].x && normalX < line[i][1].x) &&
		(normalY > line[i][0].y - 0.005 && normalY < line[i][1].y + 0.005)) {
		return true;
	}
	else {
		return false;
	}

}
bool checktriangle(float x, float y, int i) {
		int j;
		int n = 2;
		float delX1 = trian[i][0].x - trian[i][1].x; //leftcase
		float delY1 = trian[i][0].y - trian[i][1].y; //leftcase

		float delX2 = trian[i][0].x - trian[i][2].x; //rightcase
		float delY2 = trian[i][0].y - trian[i][2].y; //rightcase
		if (trian[i][1].y < y && y < trian[i][0].y) {
			if (x < trian[i][0].x) {
				float compareY = (delY1 / delX1) * (x - trian[i][0].x) + trian[i][0].y;
				if (y < compareY) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (trian[i][0].x <= x) {
				float compareY = (delY2 / delX2) * (x - trian[i][0].x) + trian[i][0].y;
				if (y < compareY) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
}
bool checkrectangles(float normalX, float normalY, int i) {
	if ((rect[i][1].x <= normalX) && (normalX <= rect[i][3].x) &&
		(rect[i][1].y <= normalY) && (normalY <= rect[i][3].y)) {
		return true;
	}
	else {
		return false;
	}
}
bool checkpentagon(float x, float y, int i) {
	if ((penta[i][2].y < y && y < penta[i][0].y) && (penta[i][1].x < x && x < penta[i][4].x)) {
		if (penta[i][0].x < x) {
			if (penta[i][1].y < y) {
				float delX1 = penta[i][0].x - penta[i][4].x;
				float delY1 = penta[i][0].y - penta[i][4].y;
				float compareY = (delY1 / delX1) * (x - penta[i][0].x) + penta[i][0].y;
				if (y < compareY) {
					pentamove[i] = true;
					return true;
				}
				else {
					pentamove[i] = false;
					return false;
				}
			}
			if (penta[i][1].y >= y) {
				if (penta[i][3].x < x) {
					float delX1 = penta[i][3].x - penta[i][4].x;
					float delY1 = penta[i][3].y - penta[i][4].y;
					float compareY = (delY1 / delX1) * (x - penta[i][3].x) + penta[i][3].y;
					if (y > compareY) {
						pentamove[i] = true;
						return true;
					}
					else {
						pentamove[i] = false;
						return false;
					}
				}
				if (x < penta[i][3].x) {
					pentamove[i] = true;
					return true;
				}
			}
		}
		else if (penta[i][0].x >= x) {//반갈죽
			if (penta[i][1].y < y) {//정점 y축 기준 가르기 
				float delX1 = penta[i][0].x - penta[i][1].x;
				float delY1 = penta[i][0].y - penta[i][1].y;
				float compareY = (delY1 / delX1) * (x - penta[i][0].x) + penta[i][0].y;
				if (y < compareY) {
					pentamove[i] = true;
					return true;
				}
				else {
					pentamove[i] = false;
					return false;
				}
			}
			if (penta[i][1].y >= y) {
				if (penta[i][2].x > x) {
					float delX1 = penta[i][1].x - penta[i][2].x;
					float delY1 = penta[i][1].y - penta[i][2].y;
					float compareY = (delY1 / delX1) * (x - penta[i][2].x) + penta[i][2].y;
					if (y > compareY) {
						pentamove[i] = true;
						return true;
					}
					else {
						pentamove[i] = false;
						return false;

					}
				}
				if (x > penta[i][2].x) {
					pentamove[i] = true;
					return true;
				}
			}
		}
		else {
			pentamove[i] = false;
			return false;
		}
	}
	else {
		pentamove[i] = false;
		return false;
	}
}
bool checkhexagon(float x, float y, int i) {
	if ((hexa[i][2].y < y && y < hexa[i][0].y) &&
		(hexa[i][1].x < x && x < hexa[i][4].x)) {
		if (hexa[i][1].y < y) {
			if (hexa[i][0].x < x && x < hexa[i][5].x) {
				return true;
			}
			else if (x < hexa[i][0].x) {
				float delX1 = hexa[i][0].x - hexa[i][1].x;
				float delY1 = hexa[i][0].y - hexa[i][1].y;
				float compareY = (delY1 / delX1) * (x - hexa[i][0].x) + hexa[i][0].y;
				if (y < compareY) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (hexa[i][5].x < x) {
				float delX1 = hexa[i][4].x - hexa[i][5].x;
				float delY1 = hexa[i][4].y - hexa[i][5].y;
				float compareY = (delY1 / delX1) * (x - hexa[i][4].x) + hexa[i][4].y;
				if (y < compareY) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		else if (y < hexa[i][1].y) {
			if (hexa[i][0].x < x && x < hexa[i][5].x) {
				return true;
			}
			else if (x < hexa[i][0].x) {
				float delX1 = hexa[i][2].x - hexa[i][1].x;
				float delY1 = hexa[i][2].y - hexa[i][1].y;
				float compareY = (delY1 / delX1) * (x - hexa[i][1].x) + hexa[i][1].y;
				if (y > compareY) {
					return true;
				}
				else {
					return false;
				}
			}
			else if (hexa[i][5].x < x) {
				float delX1 = hexa[i][3].x - hexa[i][4].x;
				float delY1 = hexa[i][3].y - hexa[i][4].y;
				float compareY = (delY1 / delX1) * (x - hexa[i][3].x) + hexa[i][3].y;
				if (y > compareY) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

//초기 형태 만들기
void makeshape() {//at first display item
	for (int i = 0; i < 8; i++) {
		ismake1[i] = false;
		ismake2[i] = false;
		ismake3[i] = false;
		ismake4[i] = false;
		ismake5[i] = false;
		ismake6[i] = false;

		iscase1[i] = rand() % 4;
		iscase2[i] = rand() % 4;
		iscase3[i] = rand() % 4;
		iscase4[i] = rand() % 4;
		iscase5[i] = rand() % 4;
		iscase6[i] = rand() % 4;
		//점
		{
			float midX = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float midY = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float red = static_cast<float>(rand()) / RAND_MAX;
			float green = static_cast<float>(rand()) / RAND_MAX;
			float blue = static_cast<float>(rand()) / RAND_MAX;
			dot[i].x = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			dot[i].y = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			dot[i].z = 0.0f;
			dot[i].r = static_cast<float>(rand()) / RAND_MAX;
			dot[i].g = static_cast<float>(rand()) / RAND_MAX;
			dot[i].b = 0;
			dotsure[i] = true;
			if (i >= 3) {
				dot[i].x = 2;
				dot[i].y = 2;
				dot[i].z = 0.0f;
				dot[i].r = static_cast<float>(rand()) / RAND_MAX;
				dot[i].g = static_cast<float>(rand()) / RAND_MAX;
				dot[i].b = static_cast<float>(rand()) / RAND_MAX;
				dotsure[i] = false;
			}
		}
		//선
		{
			float midX = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float midY = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float red = static_cast<float>(rand()) / RAND_MAX;
			float green = static_cast<float>(rand()) / RAND_MAX;
			float blue = 0;
			if (i < 3) {
				linesure[i] = true;
				for (int j = 0; j < 2; j++) {
					if (j == 0) {
						line[i][j].x = midX - 0.2;
						line[i][j].y = midY;
					}
					if (j == 1) {
						line[i][j].x = midX + 0.2;
						line[i][j].y = midY;
					}
					line[i][j].r = red;
					line[i][j].g = green;
					line[i][j].b = blue;
				}
			}
			else {
				for (int j = 0; j < 2; j++) {
					line[i][j].x = 2;
					line[i][j].y = 2;
					line[i][j].r = red;
					line[i][j].g = green;
					line[i][j].b = blue;
				}
				linesure[i] = false;
			}
		}
		//삼
		{
			float midX = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float midY = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float red = static_cast<float>(rand()) / RAND_MAX;
			float green = static_cast<float>(rand()) / RAND_MAX;
			float blue = 0;
			if (i < 3) {
				triansure[i] = true;
				for (int j = 0; j < 3; j++) {
					if (j == 0) { //top position ->to left
						trian[i][j].x = midX;
						trian[i][j].y = midY + 0.2;
					}
					if (j == 1) {
						trian[i][j].x = midX - 0.15;
						trian[i][j].y = midY - 0.1;
					}
					if (j == 2) {
						trian[i][j].x = midX + 0.15;
						trian[i][j].y = midY - 0.1;
					}
					trian[i][j].r = red;
					trian[i][j].g = green;
					trian[i][j].b = blue;
				}
			}
			else {
				for (int j = 0; j < 3; j++) {
					trian[i][j].x = 2;
					trian[i][j].y = 2;
					trian[i][j].r = red;
					trian[i][j].g = green;
					trian[i][j].b = blue;
				}
				triansure[i] = false;
			}
		}
		//사
		{
			float midX = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float midY = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float red = static_cast<float>(rand()) / RAND_MAX;
			float green = static_cast<float>(rand()) / RAND_MAX;
			float blue = 0;
			if (i < 3) {
				rectsure[i] = true;
				for (int j = 0; j < 4; j++) {
					if (j == 0) {
						rect[i][j].x = midX - 0.1;
						rect[i][j].y = midY + 0.1;
					}
					if (j == 1) {
						rect[i][j].x = midX - 0.1;
						rect[i][j].y = midY - 0.1;
					}
					if (j == 2) {
						rect[i][j].x = midX + 0.1;
						rect[i][j].y = midY - 0.1;
					}
					if (j == 3) {
						rect[i][j].x = midX + 0.1;
						rect[i][j].y = midY + 0.1;
					}
					rect[i][j].r = red;
					rect[i][j].g = blue;
					rect[i][j].b = green;
				}
			}
			else {
				for (int j = 0; j < 4; j++) {
					rect[i][j].x = 2;
					rect[i][j].y = 2;
					rect[i][j].r = red;
					rect[i][j].g = green;
					rect[i][j].b = blue;
				}
				rectsure[i] = false;
			}
		}
		//오
		{
			float midX = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float midY = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float red = static_cast<float>(rand()) / RAND_MAX;
			float green = static_cast<float>(rand()) / RAND_MAX;
			float blue = 0;
			if (i < 3) {
				pentasure[i] = true;
				for (int j = 0; j < 5; j++) {
					penta[i][j].r = red;
					penta[i][j].g = green;
					penta[i][j].b = blue;
					if (j == 0) {//top position->to left
						penta[i][j].x = midX;
						penta[i][j].y = midY + 0.2;
					}
					if (j == 1) {
						penta[i][j].x = midX - 0.15;
						penta[i][j].y = midY + 0.1;
					}
					if (j == 2) {
						penta[i][j].x = midX - 0.1;
						penta[i][j].y = midY - 0.1;
					}
					if (j == 3) {
						penta[i][j].x = midX + 0.1;
						penta[i][j].y = midY - 0.1;
					}
					if (j == 4) {
						penta[i][j].x = midX + 0.15;
						penta[i][j].y = midY + 0.1;
					}
				}
			}
			else {
				for (int j = 0; j < 5; j++) {
					penta[i][j].x = 2;
					penta[i][j].y = 2;
					penta[i][j].r = red;
					penta[i][j].g = green;
					penta[i][j].b = blue;
				}
				pentasure[i] = false;
			}
		}
		//육
		{
			float midX = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float midY = static_cast<float>(rand()) / (RAND_MAX / 1.4) - 0.7f;
			float red = static_cast<float>(rand()) / RAND_MAX;
			float green = static_cast<float>(rand()) / RAND_MAX;
			float blue = 0;
			for (int j = 0; j < 6; j++) {
				hexa[i][j].x = 2;
				hexa[i][j].y = 2;
				hexa[i][j].z = 0;
				hexa[i][j].r = red;
				hexa[i][j].g = green;
				hexa[i][j].b = blue;
			}
			hexasure[i] = false;
		}
	}
}
//그림 함수
GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawDot();
	drawLine();
	drawTrian();
	drawRect();
	drawPenta();
	drawhexa();
	glutSwapBuffers();
}

void drawDot() {
	initBuffer(0);
	glPointSize(5.0f);
	int PosLocation = glGetAttribLocation(shaderProgramID, "in_Position");//0
	int ColorLocation = glGetAttribLocation(shaderProgramID, "in_Color");
	glEnableVertexAttribArray(PosLocation);
	glEnableVertexAttribArray(ColorLocation);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(PosLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), 0);
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));

	glDrawArrays(GL_POINTS, 0, 8);//정수부분은 점의 개수에 따라 변하게 변경하자 
}
void drawLine() {
	initBuffer(1);
}
void drawTrian() {
	initBuffer(2);
}
void drawRect() {
	initBuffer(3);
}
void drawPenta() {
	initBuffer(4);
}
void drawhexa() {
	initBuffer(5);
}

//그림설정
void initBuffer(int i) {
	if (i == 0) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), dot, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	else if (i == 1) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * 2 * 6 * sizeof(float), line, GL_STATIC_DRAW);
		for (int i = 0; i < 8; i++) {
			glLineWidth(2.0f);
			glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), line[i], GL_STATIC_DRAW);
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(linedice), linedice, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			if (linesure[i] == true) {
				glDrawElements(GL_LINE_STRIP, 2, GL_UNSIGNED_INT, 0);
			}
			glDeleteBuffers(1, &ebo);
		}
	}
	else if (i == 2) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * 3 * 6 * sizeof(float), trian, GL_STATIC_DRAW);
		for (int i = 0; i < 8; i++) {
			glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), trian[i], GL_STATIC_DRAW);
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(trindice), trindice, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_INT, 0);

			glDeleteBuffers(1, &ebo);
		}
	}	
	else if (i == 3) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * 4 * 6 * sizeof(float),rect, GL_STATIC_DRAW);
		for (int i = 0; i < 8; i++) {
			glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float), rect[i], GL_STATIC_DRAW);
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectdice), rectdice, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, 0);

			glDeleteBuffers(1, &ebo);
		}
	}
	else if (i == 4) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * 5 * 6 * sizeof(float), penta, GL_STATIC_DRAW);
		for (int i = 0; i < 8; i++) {
			glBufferData(GL_ARRAY_BUFFER, 5 * 6 * sizeof(float), penta[i], GL_STATIC_DRAW);
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pentdice), pentdice, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_INT, 0);

			glDeleteBuffers(1, &ebo);
		}
	}
	else if (i == 5) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * 6 * 6 * sizeof(float), hexa, GL_STATIC_DRAW);
		for (int i = 0; i < 8; i++) {
			glBufferData(GL_ARRAY_BUFFER, 6 * 6 * sizeof(float), hexa[i], GL_STATIC_DRAW);
			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hexadice), hexadice, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glDrawElements(GL_POLYGON, 6, GL_UNSIGNED_INT, 0);

			glDeleteBuffers(1, &ebo);
		}

	}

}

//초기 설정
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