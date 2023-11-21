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
	float xn = 0;
	float yn = 0;
	float zn = 0;
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
coord bulb[8];
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
GLuint bulb_vbo;
GLuint midvbo;
GLuint groundvbo;

GLuint ebo;


void Move_crane(int val);//crane moving animate
void Top_spin_crane(int val);
void Son_spin(int val);
void Horn_spin(int val);
void go_to_mid_arm(int val);
void go_to_mid_horn(int val);
void cam_spin(int val);
void cam_spinner(int val);
void Light_rotation(int val);

int light_on = 0;

float Light_X = 10.f;
float Light_Y = 5.f;
float Light_Z = 10.f;

void keyboard(unsigned char key, int x, int y);
void initbuffer();
GLvoid drawScene();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
void makeshape();
void lightOption();
void drawbulb();
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
	//glutReshapeFunc(Reshape);
	glutMainLoop();
}
bool viewmode = false;
bool is_move = false;//move_crane running check timerfunc
bool is_move_foward = false;
bool is_top = false;//top_spin running check timerfunc oh shit
bool is_top_spin = false;
bool is_son = false;
bool is_horn = false;
bool is_equal_arm = false;
bool is_equal_horn = false;
bool is_x_come = false;
bool is_y_come = false;
bool cam_spin_ = false;
bool viewing_mode = false;
bool is_cam_spining = false;
bool is_light_spin = false;

float y_cam_spin = 0.0f;
float x_horn_equal = 7.5f;
float x_arm_equal = 15.0f;
float x_cam_move = 0.0f;
float y_cam_move = 0.0f;
float z_cam_move = 0.0f;
float x_crane_move = 0.0f;
float y_crane_theta_mom = 0.0f;
float y_crane_theta_son = 0.0f;//only horn,arm spinng
float y_crane_horn = 0.0f;

float light_red = 1.f;
float light_green = 1.f;
float light_blue = 1.f;
float light_theta = 0.0f;

void keyboard(unsigned char key, int x, int y) {
	if (key == 'p') {
		if (!is_cam_spining) {
			is_cam_spining = true;
			glutTimerFunc(30, cam_spinner, 0);
		}
		else {
			is_cam_spining = false;
		}
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
		z_cam_move++;
	}
	if (key == 's') {
		z_cam_move--;
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
	if (key == 't') {
		if (!is_horn) {
			is_horn = true;
			glutTimerFunc(30, Horn_spin, 0);
		}
		else {
			is_horn = false;
		}
	}
	if (key == 'r') {
		y_cam_spin++;
	}
	if (key == 'R') {
		y_cam_spin--;
	}
	if (key == 'q') {
		if (!cam_spin_) {
			cam_spin_ = true;
			glutTimerFunc(30, cam_spin, 0);
		}
		else {
			cam_spin_ = false;
		}
	}
	if (key == 'e') {
		is_son = false;
		is_horn = false;
		is_y_come = true;
		is_x_come = true;
		is_equal_arm = true;
		is_equal_horn = true;
		glutTimerFunc(30, go_to_mid_arm, 0);
		glutTimerFunc(30, go_to_mid_horn, 0);
	}
	if (key == 'E') {
		//왜 없어짐???????
	}
	if (key == 'T') {
		//????????????
	}
	if (key == 'u') {
		is_move = false;//move_crane running check timerfunc
		is_move_foward = false;
		is_top = false;//top_spin running check timerfunc oh shit
		is_top_spin = false;
		is_son = false;
		is_horn = false;
		is_equal_arm = false;
		is_equal_horn = false;
		is_x_come = false;
		is_y_come = false;
	}
	if (key == '1') {
		if (!viewing_mode) {
			viewing_mode = true;
		}
		else {
			viewing_mode = false;			
		}
	}
	if (key == '2') {
		light_red = static_cast<float>(rand()) / RAND_MAX;
		light_green = static_cast<float>(rand()) / RAND_MAX;
		light_blue = static_cast<float>(rand()) / RAND_MAX;
	}
	if (key == '3') {
		if (!is_light_spin) {
			is_light_spin = true;
			glutTimerFunc(30, Light_rotation, 0);
		}
		else {
			is_light_spin = false;
		}
	}
	if (key == '4') {
		if (light_on == 1) {
			light_on = 0;
		}
		else {
			light_on = 1;
		}
	}
	glutPostRedisplay();
}
void Light_rotation(int val) {
	if (is_light_spin) {
		light_theta += 1.0f;
		glutTimerFunc(30, Light_rotation, 0);
	}
	glutPostRedisplay();
}
void Horn_spin(int val) {
	if (is_horn) {
		y_crane_horn++;
		glutTimerFunc(30, Horn_spin, 0);
	}
	if (is_x_come) {//actually it gets y rotating for horn
		if (y_crane_horn / 360 == 0) {
			is_y_come = false;
		}
		y_crane_horn--;
		glutTimerFunc(30, Horn_spin, 0);
	}
	glutPostRedisplay();
}
void cam_spin(int val) {
	if (cam_spin_) {
		y_cam_spin++;
		glutTimerFunc(30, cam_spin, 0);
	}
	glutPostRedisplay();
}
void cam_spinner(int val) {
	if (is_cam_spining) {
		y_cam_move++;
		glutTimerFunc(30, cam_spinner, 0);
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
void Son_spin(int val) {//arm rotate
	if (is_son) {
		y_crane_theta_son++;
		glutTimerFunc(30, Son_spin, 0);
	}
	if (is_y_come) {
		if (y_crane_theta_son / 360 == 0) {
			is_y_come = false;
		}
		y_crane_theta_son--;
		glutTimerFunc(30, Son_spin, 0);
	}
	glutPostRedisplay();
}
bool arm_swap = false;
void go_to_mid_arm(int val) {
	if (is_equal_arm) {
		if (x_arm_equal <= 0) {
			is_equal_arm = false;
			arm_swap = true;
		}
		x_arm_equal -= 0.1f;
		glutTimerFunc(30, go_to_mid_arm, 0);
	}
	else {
		if (arm_swap) {
			if (x_arm_equal >= 15) {
				is_equal_arm = false;
				arm_swap = false;
			}
			x_arm_equal += 0.1f;
			glutTimerFunc(30, go_to_mid_arm, 0);
		}
	}
	glutPostRedisplay();
}
bool horn_swap = false;
void go_to_mid_horn(int val) {
	if (is_equal_horn) {
		if (x_horn_equal <= 0) {
			is_equal_horn = false;
			horn_swap = true;
		}
		x_horn_equal -= 0.1f;
		glutTimerFunc(30, go_to_mid_horn, 0);
	}
	else {
		if (horn_swap) {
			if (x_horn_equal >= 7.5) {
				is_equal_horn = false;
				horn_swap = false;
			}
			x_horn_equal += 0.1f;
			glutTimerFunc(30, go_to_mid_horn, 0);
		}
	}
	glutPostRedisplay();
}
void draw_mid_line() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, midvbo);
	for (int i = 0; i < 3; i++) {
		glBufferData(GL_ARRAY_BUFFER, 2 * 9 * sizeof(float), midline[i], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glDrawArrays(GL_LINES, 0, 2);
	}
}
void draw_ground() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));


	glBindBuffer(GL_ARRAY_BUFFER, groundvbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 9 * sizeof(float), ground, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glDrawArrays(GL_POLYGON, 0, 4);
}
void draw_chest() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, chest_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(float), chest, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_head() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, head_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(float), head, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_R_arm() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_arm_equal, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-y_crane_theta_son), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, arm_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(float), arm, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_L_arm() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-x_arm_equal, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(y_crane_theta_son), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, arm_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(float), arm, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_R_horn() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_horn_equal, 0, 0));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 15));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-y_crane_horn), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -15));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, horn_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(float), horn, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void draw_L_horn() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x_crane_move, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(y_crane_theta_mom), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-x_horn_equal, 0, 0));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 15));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(y_crane_horn), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -15));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, horn_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(float), horn, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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
void drawbulb() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(light_theta), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(Light_X, Light_Y, Light_Z));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, bulb_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 9 * sizeof(float), bulb, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void viewproj() {
	GLuint projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	GLuint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");

	glm::mat4 view = glm::mat4(1.0f);

	view = glm::translate(view, glm::vec3(x_cam_move, -50.0, z_cam_move - sqrt(101)));
	view = glm::rotate(view, glm::radians(y_cam_spin), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0, 0, 100));
	view = glm::rotate(view, glm::radians(y_cam_move), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0, 0, -100));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 400.0f);
	projection = glm::translate(projection, glm::vec3(0.0, -1.0, -100.0));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

}
void second_viewproj() {
	GLuint projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	GLuint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");

	glm::mat4 view = glm::mat4(1.0f);

	view = glm::translate(view, glm::vec3(x_cam_move, z_cam_move, 0));
	view = glm::rotate(view, glm::radians(y_cam_spin), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0, 0, -sqrt(101)));
	view = glm::rotate(view, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0, 0, 100));
	view = glm::rotate(view, glm::radians(y_cam_move), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0, 0, -100));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 400.0f);
	projection = glm::translate(projection, glm::vec3(0.0, -1.0, -100.0));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));;
}
void third_viewproj() {
	GLuint projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	GLuint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");

	glm::mat4 view = glm::mat4(1.0f);

	view = glm::translate(view, glm::vec3(x_cam_move, 0, z_cam_move));
	view = glm::rotate(view, glm::radians(y_cam_spin), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0, 0, -sqrt(101)));
	view = glm::rotate(view, glm::radians(y_cam_spin), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0, 0, 100));
	view = glm::rotate(view, glm::radians(y_cam_move), glm::vec3(0.0f, 1.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0, 0, -100));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));


	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 400.0f);
	projection = glm::translate(projection, glm::vec3(0.0, -1.0, -100.0));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));;
}
void first_View() {
	glViewport(0, 0, 400, 600);
	viewproj();
	lightOption();
	draw_mid_line();
	draw_ground();
	drawbulb();
	robot();
}
void second_View() {
	glViewport(400, 300, 800, 600);
	second_viewproj();
	lightOption();
	draw_mid_line();
	draw_ground();
	drawbulb();
	robot();
}
void third_View() {
	glViewport(400, 0, 800, 300); 
	third_viewproj();
	lightOption();
	draw_mid_line();
	draw_ground();
	drawbulb();
	robot();
}
void lightOption() {
	GLuint BoolUniformLocation = glGetUniformLocation(shaderProgramID, "light_Mode");
	glUniform1i(BoolUniformLocation, light_on);

	glm::vec3 lightPosition(Light_X, Light_Y, Light_Z);
	GLuint lightPositionUniformLocation = glGetUniformLocation(shaderProgramID, "lightPosition");
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	GLuint lightMatrixLocation = glGetUniformLocation(shaderProgramID, "lightMatrix");
	glm::mat4 lightMatrix = glm::mat4(1.0f);
	lightMatrix = glm::rotate(lightMatrix, glm::radians(light_theta), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(lightMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightMatrix));
	
	GLuint camPosLocation = glGetUniformLocation(shaderProgramID, "viewDir");
	glm::vec3 camDir(0.0f, 0.0f, 0.0f);
	glUniform3fv(camPosLocation, 1, &camDir[0]);

	GLuint lightSetLocation = glGetUniformLocation(shaderProgramID, "light_Color");
	glm::vec3 light_Color(light_red, light_green, light_blue);
	glUniform3fv(lightSetLocation, 1, &light_Color[0]);

}
GLvoid drawScene() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	if (viewing_mode) {
		first_View();
		second_View();
		third_View();
	}
	else{
		glViewport(0, 0, 800, 600);
		viewproj();
		lightOption();
		draw_mid_line();
		draw_ground();
		drawbulb();
		robot();
	}
	glutSwapBuffers();
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
	ground[0].yn = 1.f;
	ground[1].x = -100.f;
	ground[1].z = 100.f;
	ground[1].yn = 1.f;
	ground[2].x = -100.f;
	ground[2].z = -100.f;
	ground[2].yn = 1.f;
	ground[3].x = 100.f;
	ground[3].z = -100.f;
	ground[3].yn = 1.f;

}
void make_head() {
	head[0].x = -10.f;
	head[0].z = 5.f;
	head[0].y = 10.f;
	head[0].yn = -1;

	head[1].x = -10.f;
	head[1].z = -5.f;
	head[1].y = 10.f;
	head[1].yn = -1;

	head[2].x = 10.f;
	head[2].z = -5.f;
	head[2].y = 10.f;
	head[2].yn = -1;

	head[3].x = 10.f;
	head[3].z = 5.f;
	head[3].y = 10.f;
	head[3].yn = -1;

	head[4].x = -10.f;
	head[4].z = 5.f;
	head[4].y = 20.f;
	head[4].yn = 1;

	head[5].x = -10.f;
	head[5].z = -5.f;
	head[5].y = 20.f;
	head[5].yn = 1;

	head[6].x = 10.f;
	head[6].z = -5.f;
	head[6].y = 20.f;
	head[6].yn = 1;

	head[7].x = 10.f;
	head[7].z = 5.f;
	head[7].y = 20.f;
	head[7].yn = 1;
	for (int i = 0; i < 8; i++) {
		head[i].r = static_cast<float>(rand()) / RAND_MAX;
		head[i].g = static_cast<float>(rand()) / RAND_MAX;
		head[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
}
void make_chest() {
	chest[0].x = -20.f;
	chest[0].y = 0;
	chest[0].z = 15.f;
	chest[0].yn = -1;

	chest[1].x = -20.f;
	chest[1].y = 0;
	chest[1].z = -15.f;
	chest[1].yn = -1;
	
	chest[2].x = 20.f;
	chest[2].y = 0;
	chest[2].z = -15.f;
	chest[2].yn = -1;

	chest[3].x = 20.f;
	chest[3].y = 0;
	chest[3].z = 15.f;
	chest[3].yn = -1;

	chest[4].x = -20.f;
	chest[4].z = 15.f;
	chest[4].y = 10;
	chest[4].yn = 1;

	chest[5].x = -20.f;
	chest[5].y = 10;
	chest[5].z = -15.f;
	chest[5].yn = 1;

	chest[6].x = 20.f;
	chest[6].y = 10;
	chest[6].z = -15.f;
	chest[6].yn = 1;

	chest[7].x = 20.f;
	chest[7].y = 10;
	chest[7].z = 15.f;
	chest[7].yn = 1;
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
	horn[0].yn = -1;

	horn[1].x = -1.5f;
	horn[1].z = -1.25f;
	horn[1].y = 0.f;
	horn[1].yn = -1;

	horn[2].x = 1.5f;
	horn[2].z = -1.25f;
	horn[2].y = 0.f;
	horn[2].yn = -1;

	horn[3].x = 1.5f;
	horn[3].z = 1.25f;
	horn[3].y = 0.f;
	horn[3].yn = -1;

	horn[4].x = -1.5f;
	horn[4].z = 1.25f;
	horn[4].y = 30.f;
	horn[4].yn = 1;

	horn[5].x = -1.5f;
	horn[5].z = -1.25f;
	horn[5].y = 30.f;
	horn[5].yn = 1;

	horn[6].x = 1.5f;
	horn[6].z = -1.25f;
	horn[6].y = 30.f;
	horn[6].yn = 1.f;

	horn[7].x = 1.5f;
	horn[7].z = 1.25f;
	horn[7].y = 30.f;
	horn[7].yn = 1;
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
	arm[0].yn = -1;

	arm[1].x = -1.25f;
	arm[1].y = 0;
	arm[1].z = 30.f;
	arm[1].yn = -1;

	arm[2].x = 1.25f;
	arm[2].y = 0;
	arm[2].z = 30.f;
	arm[2].yn = -1;

	arm[3].x = 1.25f;
	arm[3].y = 0;
	arm[3].z = -30.f;
	arm[3].yn = -1;

	arm[4].x = -1.25f;
	arm[4].y = 2.5;
	arm[4].z = -30.f;
	arm[4].yn =1;

	arm[5].x = -1.25f;
	arm[5].y = 2.5;
	arm[5].z = 30.f;
	arm[5].yn = 1;

	arm[6].x = 1.25f;
	arm[6].y = 2.5;
	arm[6].z = 30.f;
	arm[6].yn = 1;

	arm[7].x = 1.25f;
	arm[7].y = 2.5;
	arm[7].z = -30.f;
	arm[7].yn = 1;
	for (int i = 0; i < 8; i++) {
		arm[i].r = static_cast<float>(rand()) / RAND_MAX;
		arm[i].g = static_cast<float>(rand()) / RAND_MAX;
		arm[i].b = static_cast<float>(rand()) / RAND_MAX;
	}

}
void make_bulb() {
	bulb[0].x = -5;
	bulb[0].y = -5;
	bulb[0].z = -5;
	bulb[0].yn = -1;

	bulb[1].x = -5;
	bulb[1].y = -5;
	bulb[1].z = 5;
	bulb[1].yn = 1;
				
	bulb[2].x = 5;
	bulb[2].y = -5;
	bulb[2].z = -5;
	bulb[2].yn = 1;
				
	bulb[3].x = 5;
	bulb[3].y = -5;
	bulb[3].z = 5;
	bulb[3].yn = 1;
				
	bulb[4].x = -5;
	bulb[4].y = 5;
	bulb[4].z = 5;
	bulb[4].yn = 1;
				
	bulb[5].x = -5;
	bulb[5].y = 5;
	bulb[5].z = -5;
	bulb[5].yn = 1;
				
	bulb[6].x = 5;
	bulb[6].y = 5;
	bulb[6].z = -5;
	bulb[6].yn = 1;
				
	bulb[7].x = 5;
	bulb[7].y = 5;
	bulb[7].z = 5;
	bulb[7].yn = 1;

	for (int i = 0; i < 8; i++) {
		bulb[i].r = 0.5f;
		bulb[i].g = 1.0f;
		bulb[i].b = 1.0f;
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
	glGenBuffers(1, &bulb_vbo);

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