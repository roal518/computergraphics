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
coord ground[4];
coord chest[8];
coord head[8];
coord arm[8];
coord horn[8];
int indices[] = {
	0,2,1,0,3,2,//밑면
	4,5,6,4,6,7,//윗면
	0,1,5,0,5,4,//좌측면
	1,2,5,2,6,5,//정면
	2,3,7,2,7,6,//우측면
	0,7,3,0,4,7 //후면
};
GLuint vao; // 삼각형 개수 저장
// 색상,정점 위치 저장
GLuint chest_vbo;
GLuint head_vbo;
GLuint arm_vbo;
GLuint horn_vbo;

GLuint midvbo;
GLuint groundvbo;

GLuint ebo;


void Move_crane(int val);//crane moving animate
void Top_spin_crane(int val);
void Son_spin(int val);







void keyboard(unsigned char key, int x, int y);
void initbuffer();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
void makeshape();
void menu() {
	printf("b/B: 크레인의 아래 몸체가 x축 방향으로 양/음 방향으로 이동한다.\n");
	printf("m/M: 크레인의 중앙 몸체가 y축에 대하여 양/음 방향으로 회전한다.\n");
	printf("f/F: 포신이 y축에 대하여 양/음 방향으로 회전하는데, 두 포신이 서로 반대방향으로 회전한다\n");
	printf("e/E: 2개 포신이 조금씩 이동해서 한 개가 된다/다시 제자리로 이동해서 2개가 된다\n");
	printf("t/T: 크레인의 맨 위 2개의 팔이 z축에 대하여 양/음 방향으로 서로 반대방향으로 회전한다. \n");
	printf("w/s: 카메라가 z축 양/음 방향으로 이동\n");
	printf("a/d: 카메라가 x축 양/음 방향으로 이동\n");
	printf("y/Y: 카메라 기준 y축에 대하여 회전\n");
	printf("r/R: 화면의 중심의 y축에 대하여 카메라가 회전 (중점에 대하여 공전)\n");
	printf("a/A: r 명령어와 같이 화면의 중심의 축에 대하여 카메라가 회전하는 애니메이션을 진행한다/멈춘다.\n");
	printf("s/S: 모든 움직임 멈추기\n");
	printf("c/C: 모든 움직임이 초기화된다\n");
	printf("Q: 프로그램 종료하기\n");
}
void main(int argc, char** argv) {
	menu();
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
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
bool viewmode = false;
bool is_move = false;//move_crane running check timerfunc
bool is_move_foward = false;
bool is_top = false;//top_spin running check timerfunc oh shit
bool is_top_spin = false;
bool is_son = false;



float x_cam_move = 0.0f;
float y_cam_move = 0.0f;
float z_cam_move = 0.0f;
float x_crane_move = 0.0f;
float y_crane_theta_mom = 0.0f;
float y_crane_theta_son = 0.0f;//only horn,arm spinng
void keyboard(unsigned char key, int x, int y) {
	if (key == 'p') {

	}
	if (key == 'l') {
		if (!viewmode) {
			viewmode = true;
		}
		else {
			viewmode = false;
		}
	}
	if (key == 'w') {
		z_cam_move ++;
	}
	if (key == 's') {
		z_cam_move --;
	}
	if (key == 'y') {
		//rotate
		y_cam_move++;
	}
	if (key == 'Y') {
		//rotate
		y_cam_move--;
	}
	if (key == 'a') {
		x_cam_move++;
	}
	if (key == 'd') {
		x_cam_move--;
	}
	if (key == 'b') {
		if (!is_move) {
			is_move = true;
			is_move_foward = true;
			glutTimerFunc(30, Move_crane, 0);
		}
		else {
			if (is_move_foward) {
				is_move = false;

			}
			else {
				is_move_foward = true;
			}
		}
	}
	if (key == 'B') {
		if (!is_move) {
			is_move = true;
			is_move_foward = false;
			glutTimerFunc(30, Move_crane, 0);
		}
		else {
			if (!is_move_foward) {
				is_move = false;

			}
			else {
				is_move_foward = false;
			}
		}
	}
	if (key == 'm') {
		if (!is_top) {
			is_top = true;
			is_top_spin = true;
			glutTimerFunc(30, Top_spin_crane, 0);
		}
		else {
			if (is_top_spin) {
				is_top = false;

			}
			else {
				is_top_spin = true;
			}
		}
	}
	if (key == 'M') {
		if (!is_top) {
			is_top = true;
			is_top_spin = false;
			glutTimerFunc(30, Top_spin_crane, 0);
		}
		else {
			if (is_top_spin) {
				is_top = false;

			}
			else {
				is_top_spin = false;
			}
		}
	}
	if (key == 'f') {
		if (!is_son) {
			is_son = true;
			glutTimerFunc(30, Son_spin, 0);
		}
		else {
			is_son = false;
		}
	}
	if (key == 'e') {

	}
	if (key == 'E') {

	}
	if (key == 't') {

	}
	if (key == 'T') {

	}
	glutPostRedisplay();
}
void Move_crane(int val) {
	if (is_move) {
		if (is_move_foward) {
			if (x_crane_move < 80.f) {
				x_crane_move++;
			}
			else {
				is_move_foward = false;
				is_move = false;
			}
		}
		else {
			if (x_crane_move > -80.f) {
				x_crane_move--;
			}
			else {
				is_move_foward = true;
				is_move = false;
			}
		}
		glutTimerFunc(30, Move_crane, 0);
	}
	glutPostRedisplay();
}
void Top_spin_crane(int val) {
	if (is_top) {
		if (is_top_spin) {
			y_crane_theta_mom++;
		}
		else {
			y_crane_theta_mom--;
		}
		glutTimerFunc(30, Top_spin_crane, 0);
	}
	glutPostRedisplay();
}
void Son_spin(int val) {
	if (is_son) {
		y_crane_theta_son++;
		glutTimerFunc(30, Son_spin, 0);
	}
	glutPostRedisplay();
}
void draw_mid_line() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, midvbo);
	for (int i = 0; i < 3; i++) {
		glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), midline[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_LINES, 0, 2);
	}
}
void draw_ground() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));


	glBindBuffer(GL_ARRAY_BUFFER, groundvbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float), ground, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_POLYGON, 0, 4);
}
void draw_chest() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, chest_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), chest, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_head() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, head_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), head, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_R_arm() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(7.5f, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-y_crane_theta_son), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, arm_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), arm, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_L_arm() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-7.5f, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(y_crane_theta_son), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, arm_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), arm, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_R_horn() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(7.5f, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-y_crane_theta_son), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, horn_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), horn, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_L_horn() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-7.5f, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(y_crane_theta_son), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, horn_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), horn, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void robot() {
	draw_head();//dae-ga-ri
	
	draw_R_horn();
	draw_L_horn();

	draw_chest();

	draw_R_arm();
	draw_L_arm();
}
void viewproj() {
	GLuint projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	GLuint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(x_cam_move, 0, z_cam_move));
	view = glm::rotate(view, glm::radians(10.f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(10.f), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::rotate(view, glm::radians(y_cam_move), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0, 0.0, -100.0));

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(35.0f), 1.0f, 0.1f, 400.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -100.0));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}
GLvoid drawScene() {
	viewproj();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	draw_mid_line();
	draw_ground();
	robot();
	glutSwapBuffers();
}
GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w , h );

}

void make_mid() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			midline[i][j].r = 0;
			midline[i][j].g = 0;
			midline[i][j].b = 0;
			midline[i][j].x = 0;
			midline[i][j].y = 0;
			midline[i][j].z = 0;
		}
	}
	midline[0][0].x = -100.f;
	midline[0][1].x = 100.f;
	midline[0][0].r = 1.f;
	midline[0][1].r = 1.f;

	midline[1][0].y = -100.f;
	midline[1][1].y = 100.f;
	midline[1][0].g = 1.f;
	midline[1][1].g = 1.f;

	midline[2][0].z = -100.f;
	midline[2][1].z = 100.f;
	midline[2][0].b = 1.f;
	midline[2][1].b = 1.f;

}
void make_ground() {
	for (int i = 0; i < 4; i++) {
		ground[i].r = static_cast<float>(rand()) / RAND_MAX;
		ground[i].g = static_cast<float>(rand()) / RAND_MAX;
		ground[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
	ground[0].x = 100.f;
	ground[0].z = 100.f;
	ground[1].x = -100.f;
	ground[1].z = 100.f;
	ground[2].x = -100.f;
	ground[2].z = -100.f;
	ground[3].x = 100.f;
	ground[3].z = -100.f;
	
}
void make_head() {
	head[0].x = -10.f;
	head[0].z = 5.f;
	head[0].y = 10.f;

	head[1].x = -10.f;
	head[1].z = -5.f;
	head[1].y = 10.f; 

	head[2].x = 10.f;
	head[2].z = -5.f;
	head[2].y = 10.f;

	head[3].x = 10.f;
	head[3].z = 5.f;
	head[3].y = 10.f;

	head[4].x = -10.f;
	head[4].z = 5.f;
	head[4].y = 20.f;

	head[5].x = -10.f;
	head[5].z = -5.f;
	head[5].y = 20.f;

	head[6].x = 10.f;
	head[6].z = -5.f;
	head[6].y = 20.f;

	head[7].x = 10.f;
	head[7].z = 5.f;
	head[7].y = 20.f;
	for (int i = 0; i < 8; i++) {
		head[i].r = static_cast<float>(rand()) / RAND_MAX;
		head[i].g = static_cast<float>(rand()) / RAND_MAX;
		head[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
}
void make_chest() {
	chest[0].x = -20.f;
	chest[0].z = 15.f;
	chest[0].y = 0;

	chest[1].x = -20.f;
	chest[1].y = 0;
	chest[1].z = -15.f;

	chest[2].x = 20.f;
	chest[2].y = 0;
	chest[2].z = -15.f;

	chest[3].x = 20.f;
	chest[3].y = 0;
	chest[3].z = 15.f;

	chest[4].x = -20.f;
	chest[4].z = 15.f;
	chest[4].y = 10;

	chest[5].x = -20.f;
	chest[5].y = 10;
	chest[5].z = -15.f;

	chest[6].x = 20.f;
	chest[6].y = 10;
	chest[6].z = -15.f;

	chest[7].x = 20.f;
	chest[7].y = 10;
	chest[7].z = 15.f;
	for (int i = 0; i < 8; i++) {
		chest[i].r = static_cast<float>(rand()) / RAND_MAX;
		chest[i].g = static_cast<float>(rand()) / RAND_MAX;
		chest[i].b = static_cast<float>(rand()) / RAND_MAX;
	}

}
void make_horn() {
	horn[0].x = -1.5f;
	horn[0].z = 1.25f;
	horn[0].y = 0.f;

	horn[1].x = -1.5f;
	horn[1].z = -1.25f;
	horn[1].y = 0.f;

	horn[2].x = 1.5f;
	horn[2].z = -1.25f;
	horn[2].y = 0.f;

	horn[3].x = 1.5f;
	horn[3].z = 1.25f;
	horn[3].y = 0.f;

	horn[4].x = -1.5f;
	horn[4].z = 1.25f;
	horn[4].y = 30.f;

	horn[5].x = -1.5f;
	horn[5].z = -1.25f;
	horn[5].y = 30.f;

	horn[6].x = 1.5f;
	horn[6].z = -1.25f;
	horn[6].y = 30.f;

	horn[7].x = 1.5f;
	horn[7].z = 1.25f;
	horn[7].y = 30.f;
	for (int i = 0; i < 8; i++) {
		horn[i].r = static_cast<float>(rand()) / RAND_MAX;
		horn[i].g = static_cast<float>(rand()) / RAND_MAX;
		horn[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
}
void make_arm() {
	arm[0].x = -1.25f;
	arm[0].y = 0;
	arm[0].z = -30.f;

	arm[1].x = -1.25f;
	arm[1].y = 0;
	arm[1].z = 30.f;

	arm[2].x = 1.25f;
	arm[2].y = 0;
	arm[2].z = 30.f;

	arm[3].x = 1.25f;
	arm[3].y = 0;
	arm[3].z = -30.f;

	arm[4].x = -1.25f;
	arm[4].y = 2.5;
	arm[4].z = -30.f;

	arm[5].x = -1.25f;
	arm[5].y = 2.5;
	arm[5].z = 30.f;

	arm[6].x = 1.25f;
	arm[6].y = 2.5;
	arm[6].z = 30.f;

	arm[7].x = 1.25f;
	arm[7].y = 2.5;
	arm[7].z = -30.f;
	for (int i = 0; i < 8; i++) {
		arm[i].r = static_cast<float>(rand()) / RAND_MAX;
		arm[i].g = static_cast<float>(rand()) / RAND_MAX;
		arm[i].b = static_cast<float>(rand()) / RAND_MAX;
	}

}
void makeshape() {
	make_mid();
	make_ground();
	make_chest();
	make_head();
	make_arm();
	make_horn();
}
void initbuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &arm_vbo);
	glGenBuffers(1, &head_vbo);
	glGenBuffers(1, &chest_vbo);
	glGenBuffers(1, &arm_vbo);
	glGenBuffers(1, &horn_vbo);
	glGenBuffers(1, &midvbo);
	glGenBuffers(1, &groundvbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
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