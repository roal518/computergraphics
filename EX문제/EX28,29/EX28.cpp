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
coord* sphere = NULL;
coord* earth = NULL;
coord* mars = NULL;
int * sphereindice = NULL;
int vertexCount = 0;
int faceCount = 0;
GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;
coord midline[3][2];
coord ground[4];
coord head[8];
coord bulb[8];
coord tetra[5];
coord tetra_rear[3];
coord tetra_far[3];
coord tetra_right[3];
coord tetra_left[3];

coord snow[8];
coord line_planet[3][50];
typedef struct Sphere {
	float x = 0;
	float y = 0;
	float z = 0;
	float theta = 0;
};
Sphere planet[3];

typedef struct Node {
	coord snowvertex[8];
	float x = 0;
	float y = 0;
	float z = 0;
	struct Node* next;
};
Node* createnode(Node* next, float x, float y, float z) {
	Node* newnode;
	newnode = (Node*)malloc(sizeof(Node));
	for (int i = 0; i < 8; i++) {
		newnode->snowvertex[i] = snow[i];
	}
	newnode->x = x;
	newnode->y = y;
	newnode->z = z;
	newnode->next = NULL;
	return newnode;
}
void insert(Node** head, Node* newnode) {
	if ((*head) == NULL) {
		(*head) = newnode;
		newnode->next = NULL;
	}
	else {
		newnode->next = (*head);
		(*head) = newnode;
	}
}

Node* snow_list = NULL;
int indices[] = {
	0,2,1,0,3,2,//밑면
	4,5,6,4,6,7,//윗면
	0,1,5,0,5,4,//좌측면
	1,2,5,2,6,5,//정면
	2,3,7,2,7,6,//우측면
	0,7,3,0,4,7 //후면
};
unsigned int tetradices[] = {
	0,2,1,0,3,2,
	0,1,4,
	1,2,4,
	2,3,4,
	3,0,4
};
int line_dices[51];
GLuint vao;
GLuint bulb_vbo;
GLuint midvbo;
GLuint groundvbo;
GLuint sphere_vbo;
GLuint tetra_vbo;
GLuint snow_vbo;
GLuint planet_road_vbo;
GLuint ebo;
int draw_S_level = 0;
void Light_rotation(int val);
void mouse_move(int x, int y);

int light_on = 0;

float Light_X = 100.f;
float Light_Y = 50.f;
float Light_Z = 100.f;

void keyboard(unsigned char key, int x, int y);
void mouse_wheel(int wheel, int direction, int x, int y);
void initbuffer();
GLvoid drawScene();
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
void makeshape();
void lightOption();
void drawbulb();
void loopTimers();
void main(int argc, char** argv) {
	makeshape();
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 28//29");
	glewExperimental = GL_TRUE;
	glewInit();
	initbuffer();
	loopTimers();
	make_shaderProgram();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse_move);
	glutMouseWheelFunc(mouse_wheel);
	glutMainLoop();
}
GLfloat lastX = 0.0f;
GLfloat lastY = 0.0f;
GLfloat lastZ = 0.0f;
GLfloat camYaw = 0.0f;
GLfloat camPit = 0.0f;
float y_cam_spin = 0.0f;
float x_horn_equal = 7.5f;
float x_arm_equal = 15.0f;
float x_cam_move = 0.0f;
float y_cam_move = 300.f;
float z_cam_move = sqrt(101);
float x_crane_move = -10.0f;
float y_crane_move = 10.0f;
float z_crane_move = -10.0f;
float cam_height = 0.0f;
void snow_create(int val) {
	float x = static_cast<float>(rand()) / RAND_MAX * 200 - 100;
	float z = static_cast<float>(rand()) / RAND_MAX * 200 - 100;
	int y = 50;
	insert(&snow_list, createnode(NULL, x, y, z));
	glutTimerFunc(10, snow_create, 0);
}
void snow_move(int val) {
	Node* snowy = snow_list;
	while (snowy != NULL) {
		snowy->y -= 0.1f;
		snowy = snowy->next;
	}
	glutPostRedisplay();
	glutTimerFunc(30, snow_move, 0);
}
void snow_delete(int val) {
	Node* current = snow_list;
	Node* previous = NULL;
	while (current != NULL) {
		if ((current->y <= -0.5)) {
			if (previous == NULL) {
				snow_list = current->next;
			}
			else {
				previous->next = current->next;
			}
			Node* del = current;
			current = current->next;
			free(del);
		}
		else {
			previous = current;
			current = current->next;
		}
	}
	glutTimerFunc(10, snow_delete, 0);
}
void snow_timers() {
	glutTimerFunc(10, snow_delete, 0);
	glutTimerFunc(30, snow_move, 0);
	glutTimerFunc(10, snow_create, 0);
}
void planet_move(int val) {
	for (int i = 0; i < 3; i++) {
		planet[i].theta += 1.0f;
	}
	glutTimerFunc(30, planet_move, 0);
}
void planet_timers() {
	glutTimerFunc(30, planet_move, 0);
}
void loopTimers() {
	snow_timers();
	planet_timers();
}
void mouse_wheel(int wheel, int direction, int x, int y) {
	if (direction > 0) {
		cam_height++;
	}
	else {
		cam_height--;
	}
	glutPostRedisplay();
}
void mouse_move(int x, int y) {
	GLfloat xoffset = x - lastX;//Dx
	GLfloat yoffset = lastY - y;//Dy
	lastX = x;
	lastY = y;
	GLfloat sensitivity = 0.1f;
	xoffset *= 1.0f;
	yoffset *= sensitivity;
	camYaw += xoffset;
	camPit += yoffset;
	/*
	if (camYaw > 90.0f) camYaw = 89.0f;
	if (camYaw < -90.0f) camYaw = -89.0f;
	if (camPit > 90.0f) camPit = 90.0f;
	if (camPit < -90.0f) camPit = -90.0f;
	
	*/
	x_cam_move = glm::cos(glm::radians(camYaw)) * glm::cos(glm::radians(camPit));
	y_cam_move = glm::sin(glm::radians(camPit));
	z_cam_move = glm::sin(glm::radians(camYaw)) * glm::cos(glm::radians(camPit));
	glutPostRedisplay();
}
bool cam_spin_ = false;
bool is_light_spin = false;
float light_red = 1.f;
float light_blue = 1.f;
float light_green = 1.f;
float light_theta = 0.0f;
float light_str = 0.01f;
int change_light_loc = 0;
void keyboard(unsigned char key, int x, int y) {
	if (key == 'c') {
		light_red = static_cast<float>(rand()) / RAND_MAX;
		light_green = static_cast<float>(rand()) / RAND_MAX;
		light_blue = static_cast<float>(rand()) / RAND_MAX;
	}
	if (key == 'r') {
		if (!is_light_spin) {
			is_light_spin = true;
			glutTimerFunc(30, Light_rotation, 0);
		}
		else {
			is_light_spin = false;
		}
	}
	if (key == 'm') {
		if (light_on == 1) {
			light_on = 0;
		}
		else {
			light_on = 1;
		}
	}
	if (key == '0') {
		draw_S_level = 0;
	}
	if (key == '1') {
		draw_S_level = 1;
	}
	if (key == '2') {
		draw_S_level = 2;
	}
	if (key == '3') {
		draw_S_level = 3;
	}
	if (key == '4') {
		draw_S_level = 4;
	}
	if (key == '5') {
		draw_S_level = 5;
	}
	if (key == 'p') {
		if (change_light_loc == 0) {
			Light_X = 0.f;
			Light_Z = 100.0f;
			change_light_loc = 1;
		}
		else if (change_light_loc == 1) {
			Light_X = 0.f;
			Light_Z = -100.0f;
			change_light_loc = 2;
		}
		else if (change_light_loc == 2) {
			Light_X = 100.f;
			Light_Z = 0.0f;
			change_light_loc = 3;
		}
		else if (change_light_loc == 3) {
			Light_X = -100.f;
			Light_Z = 0.0f;
			change_light_loc = 0;
		}
	}
	if (key == 'n') {
		if (glm::pow(Light_X, 2) > 0) {
			if (Light_X > 0) {
				Light_X -= 1;
			}
			else if (Light_X < 0) {
				Light_X += 1;
			}
		}
		if (glm::pow(Light_Y, 2) > 0) {
			if (Light_Y > 0) {
				Light_Y -= 1;
			}
			else if (Light_Y < 0) {
				Light_Y += 1;
			}
		}
		if (glm::pow(Light_Z, 2) > 0) {
			if (Light_Z > 0) {
				Light_Z -= 1;
			}
			else if (Light_Z < 0) {
				Light_Z += 1;
			}
		}
	}
	if (key == 'f') {
			if (Light_X >= 0) {
				Light_X += 1;
			}
			else if (Light_X <= 0) {
				Light_X -= 1;
			}
			if (Light_Y >= 0) {
				Light_Y += 1;
			}
			if (Light_Z >= 0) {
				Light_Z += 1;
			}
			else if (Light_Z <= 0) {
				Light_Z -= 1;
			}
		
	}
	if (key == '-') {
		if (light_str > 0) {
			light_str -= 0.001f;
		}
	}
	if (key == '+') {
		light_str += 0.001f;
	}
	if (key == 27) {
		exit(0);
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
void draw_mid_line() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);

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
void draw_tetra_rear() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// 바인딩된 버퍼 없이 정점 데이터 전송
	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), tetra_rear, GL_STATIC_DRAW);

	// 정점 속성 포인터 설정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
void draw_tetra_far() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// 바인딩된 버퍼 없이 정점 데이터 전송
	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), tetra_far, GL_STATIC_DRAW);

	// 정점 속성 포인터 설정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
void draw_tetra_left() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// 바인딩된 버퍼 없이 정점 데이터 전송
	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), tetra_left, GL_STATIC_DRAW);

	// 정점 속성 포인터 설정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
void draw_tetra_right() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), tetra_right, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
void draw_tetra() {
	draw_tetra_rear();
	draw_tetra_far();
	draw_tetra_left();
	draw_tetra_right();
}
void draw_ground() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));



	glBindBuffer(GL_ARRAY_BUFFER, groundvbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float), ground, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glDrawArrays(GL_POLYGON, 0, 4);
}
void draw_snow() {
	Node* snowy = snow_list;
	while (snowy != NULL) {
		GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(snowy->x, snowy->y, snowy->z));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glBindBuffer(GL_ARRAY_BUFFER, snow_vbo);
		glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), snowy->snowvertex, GL_STATIC_DRAW);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		snowy = snowy->next;
	}
}
void draw_sphere() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 20.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(planet[0].theta), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(50.f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 6 * sizeof(float), sphere, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (faceCount - 9) * sizeof(int), sphereindice, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawElements(GL_TRIANGLES, faceCount - 9, GL_UNSIGNED_INT, 0);
}
void draw_sphere_road() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 20.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, planet_road_vbo);
	glBufferData(GL_ARRAY_BUFFER, 50 * 6 * sizeof(float), line_planet[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  sizeof(line_dices), line_dices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawElements(GL_LINE_STRIP, 51, GL_UNSIGNED_INT, 0);
}
void draw_mars() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 20.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(planet[2].theta), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(50.f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 6 * sizeof(float), mars, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (faceCount - 100) * sizeof(int), sphereindice, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawElements(GL_TRIANGLES, faceCount - 100, GL_UNSIGNED_INT, 0);

}
void draw_mars_road() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 20.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, planet_road_vbo);
	glBufferData(GL_ARRAY_BUFFER, 50 * 6 * sizeof(float), line_planet[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line_dices), line_dices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawElements(GL_LINE_STRIP, 51, GL_UNSIGNED_INT, 0);
}
void draw_earth() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 20.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-45.f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(planet[1].theta), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-50.f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 6 * sizeof(float), earth, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (faceCount - 100) * sizeof(int), sphereindice, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawElements(GL_TRIANGLES, faceCount - 100, GL_UNSIGNED_INT, 0);

}
void draw_earth_road() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 20.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-45.f), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, planet_road_vbo);
	glBufferData(GL_ARRAY_BUFFER, 50 * 6 * sizeof(float), line_planet[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line_dices), line_dices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawElements(GL_LINE_STRIP, 51, GL_UNSIGNED_INT, 0);
}
void draw_planet() {
	draw_sphere();
	draw_sphere_road();
	draw_mars();
	draw_mars_road();
	draw_earth();
	draw_earth_road();
}
void drawbulb() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(light_theta), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(Light_X, Light_Y, Light_Z));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, bulb_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), bulb, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void viewproj() {
	GLuint projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	GLuint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");

	glm::vec3 camPos = glm::vec3(x_cam_move, -cam_height - y_cam_move,z_cam_move);
	glm::vec3 camTag = glm::vec3(0, 0, 0);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view = glm::lookAt(camPos, camTag, camUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 400.0f);
	projection = glm::translate(projection, glm::vec3(0.0, -1.0, -100.0));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

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

	GLuint lightStrSetLocation = glGetUniformLocation(shaderProgramID, "light_Str");
	glUniform1f(lightStrSetLocation, light_str);
}
void draw_plane_1(coord* plane) {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), plane, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

}
void draw_plane_2(coord* plane) {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), plane, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

}
void draw_plane_3(coord* plane) {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), plane, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

}
void draw_plane_4(coord* plane) {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(270.f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), plane, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

}
void draw_S_Trian(int depth, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
	if (depth == 0) {
		coord plane_1[3];
		plane_1[0].x = x1;
		plane_1[0].y = y1;
		plane_1[0].z = z1;
		plane_1[0].r = tetra_rear[0].r;
		plane_1[0].g = tetra_rear[0].g;
		plane_1[0].b = tetra_rear[0].b;
		plane_1[1].x = x2;
		plane_1[1].y = y2;
		plane_1[1].z = z2;
		plane_1[1].r = tetra_rear[1].r;
		plane_1[1].g = tetra_rear[1].g;
		plane_1[1].b = tetra_rear[1].b;

		plane_1[2].x = x3;
		plane_1[2].y = y3;
		plane_1[2].z = z3;
		plane_1[2].r = tetra_rear[2].r;
		plane_1[2].g = tetra_rear[2].g;
		plane_1[2].b = tetra_rear[2].b;
		draw_plane_1(plane_1);
		draw_plane_2(plane_1);
		draw_plane_3(plane_1);
		draw_plane_4(plane_1);
	}
	else {
		// 삼각형의 중점 계산
		float mx1 = (x1 + x2) / 2;
		float my1 = (y1 + y2) / 2;
		float mz1 = (z1 + z2) / 2;
		float mx2 = (x2 + x3) / 2;
		float my2 = (y2 + y3) / 2;
		float mz2 = (z2 + z3) / 2;
		float mx3 = (x3 + x1) / 2;
		float my3 = (y3 + y1) / 2;
		float mz3 = (z3 + z1) / 2;

		// 새로운 세 개의 삼각형 그리기
		draw_S_Trian(depth - 1, x1, y1, z1, mx1, my1, mz1, mx3, my3, mz3);
		draw_S_Trian(depth - 1, mx1, my1, mz1, x2, y2, z2, mx2, my2, mz2);
		draw_S_Trian(depth - 1, mx3, my3, mz3, mx2, my2, mz2, x3, y3, z3);
	}
}
GLvoid drawScene() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WINDOWS_HEIGHT, WINDOWS_WIDTH);
	viewproj();
	lightOption();
	draw_mid_line();
	draw_ground();
	drawbulb();
	draw_S_Trian(
		draw_S_level,
		tetra_rear[0].x, tetra_rear[0].y, tetra_rear[0].z,
		tetra_rear[1].x, tetra_rear[1].y, tetra_rear[1].z,
		tetra_rear[2].x, tetra_rear[2].y, tetra_rear[2].z
	);
	draw_snow();
	draw_planet();
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
void make_rear() {


	tetra_rear[0].x = 0;
	tetra_rear[0].y = 18;
	tetra_rear[0].z = 0;

	tetra_rear[1].x = -9;
	tetra_rear[1].y = 0;
	tetra_rear[1].z = 9;

	tetra_rear[2].x = -9;
	tetra_rear[2].y = 0;
	tetra_rear[2].z = -9;
	
	for (int i = 0; i < 3; i++) {
		tetra_rear[i].r = static_cast<float>(rand()) / RAND_MAX;
		tetra_rear[i].g = static_cast<float>(rand()) / RAND_MAX;
		tetra_rear[i].b = static_cast<float>(rand()) / RAND_MAX;
	}	
}
void make_far() {

	tetra_far[0].x = -9;
	tetra_far[0].y = 0;
	tetra_far[0].z = -9;

	tetra_far[1].x = 9;
	tetra_far[1].y = 0;
	tetra_far[1].z = -9;

	tetra_far[2].x = 0;
	tetra_far[2].y = 18;
	tetra_far[2].z = 0;
	for (int i = 0; i < 3; i++) {
		tetra_far[i].r = static_cast<float>(rand()) / RAND_MAX;
		tetra_far[i].g = static_cast<float>(rand()) / RAND_MAX;
		tetra_far[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
}
void make_left() {
	tetra_left[0].x = 9;
	tetra_left[0].y = 0;
	tetra_left[0].z = -9;

	tetra_left[1].x = 9;
	tetra_left[1].y = 0;
	tetra_left[1].z = 9;

	tetra_left[2].x = 0;
	tetra_left[2].y = 18;
	tetra_left[2].z = 0;
	for (int i = 0; i < 3; i++) {
		tetra_left[i].r = static_cast<float>(rand()) / RAND_MAX;
		tetra_left[i].g = static_cast<float>(rand()) / RAND_MAX;
		tetra_left[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
}
void make_right() {
	tetra_right[0].x = 9;
	tetra_right[0].y = 0;
	tetra_right[0].z = 9;

	tetra_right[1].x = -9;
	tetra_right[1].y = 0;
	tetra_right[1].z = 9;

	tetra_right[2].x = 0;
	tetra_right[2].y = 18;
	tetra_right[2].z = 0;
	for (int i = 0; i < 3; i++) {
		tetra_right[i].r = static_cast<float>(rand()) / RAND_MAX;
		tetra_right[i].g = static_cast<float>(rand()) / RAND_MAX;
		tetra_right[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
}
void make_tetra() {
	make_rear();
	make_far();
	make_left();
	make_right();
}
void make_bulb() {
	bulb[0].x = -5;
	bulb[0].y = -5;
	bulb[0].z = 5;

	bulb[1].x = -5;
	bulb[1].y = -5;
	bulb[1].z = -5;

	bulb[2].x = 5;
	bulb[2].y = -5;
	bulb[2].z = -5;

	bulb[3].x = 5;
	bulb[3].y = -5;
	bulb[3].z = 5;

	bulb[4].x = -5;
	bulb[4].y = 5;
	bulb[4].z = 5;

	bulb[5].x = -5;
	bulb[5].y = 5;
	bulb[5].z = -5;

	bulb[6].x = 5;
	bulb[6].y = 5;
	bulb[6].z = -5;

	bulb[7].x = 5;
	bulb[7].y = 5;
	bulb[7].z = 5;

	for (int i = 0; i < 8; i++) {
		bulb[i].r = 1.0f;
		bulb[i].g = 1.0f;
		bulb[i].b = 1.0f;
	}
}
void make_sphere() {
	FILE* obj_file;
	char line[256];
	obj_file = fopen("sphere.obj", "r");
	if (obj_file == NULL) {
		perror("Error opening file");
		return;
	}
	while (fgets(line, sizeof(line), obj_file)) {
		if (line[0] == 'v' && line[1] == ' ') {
			coord vertex;
			sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			vertex.r = 1.0f;
			vertex.g = 0.0f;
			vertex.b = 0.0f;
			sphere = (coord*)realloc(sphere, (vertexCount + 1) * sizeof(coord));			
			sphere[vertexCount] = vertex;
			vertex.r = 0.0f;
			vertex.g = 1.0f;
			vertex.b = 0.0f;
			mars = (coord*)realloc(mars, (vertexCount + 1) * sizeof(coord));
			mars[vertexCount] = vertex;

			vertex.r = 0.0f;
			vertex.g = 0.0f;
			vertex.b = 1.0f;
			earth = (coord*)realloc(earth, (vertexCount + 1) * sizeof(coord));
			earth[vertexCount] = vertex; 
			vertexCount++;
		}
		if (line[0] == 'f' && line[1] == ' ') {
			int face[3];
			sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &face[0], &face[1], &face[2]);

			sphereindice = (int*)realloc(sphereindice, (faceCount + 3) * sizeof(int));
			sphereindice[faceCount++] = face[0];
			sphereindice[faceCount++] = face[1];
			sphereindice[faceCount++] = face[2];
		}
	}
	for (int i = 0; i < 3; i++) {
		planet[i].x = static_cast<float>(rand()) / RAND_MAX * 100 - 50;
		planet[i].y = 10;
		planet[i].z = static_cast<float>(rand()) / RAND_MAX * 100 - 50;
	}
	fclose(obj_file);
}
void make_snow() {
	snow[0].x = -0.1;
	snow[0].y = -0.1;
	snow[0].z = 0.1;

	snow[1].x = -0.1;
	snow[1].y = -0.1;
	snow[1].z = -0.1;

	snow[2].x = 0.1;
	snow[2].y = -0.1;
	snow[2].z = -0.1;

	snow[3].x = 0.1;
	snow[3].y = -0.1;
	snow[3].z = 0.1;

	snow[4].x = -0.1;
	snow[4].y = 0.1;
	snow[4].z = 0.1;

	snow[5].x = -0.1;
	snow[5].y = 0.1;
	snow[5].z = -0.1;

	snow[6].x = 0.1;
	snow[6].y = 0.1;
	snow[6].z = -0.1;

	snow[7].x = 0.1;
	snow[7].y = 0.1;
	snow[7].z = 0.1;

	for (int i = 0; i < 8; i++) {
		snow[i].r = 1.0f;
		snow[i].g = 1.0f;
		snow[i].b = 1.0f;
	}
}
void make_planet_road() {
	for (int i = 0; i < 3; i++) {
		float info_theta = 0.0f;
		float rad = glm::distance(glm::vec3(10, 10, 0), glm::vec3(0, 0, 0));
		for (int j = 0; j < 50; j++) {
			line_planet[i][j].x = rad * glm::cos(glm::radians(info_theta));
			line_planet[i][j].z = rad * glm::sin(glm::radians(info_theta));

			line_planet[i][j].r = 1.0f;
			line_planet[i][j].g = 1.0f;
			line_planet[i][j].b = 0.0f;
			info_theta += 7.2f;
		}
		for (int j = 0; j < 50; j++) {
			line_planet[i][j].x = 5 * glm::cos(glm::radians(info_theta));
			line_planet[i][j].z = 5 * glm::sin(glm::radians(info_theta));

			line_planet[i][j].r = 0.0f;
			line_planet[i][j].g = 1.0f;
			line_planet[i][j].b = 0.0f;
			info_theta += 7.2f;
		}
	}
	for (int i = 0; i < 50; i++) {
		line_dices[i] = i;
	}
	line_dices[50] = 0;

}
void makeshape() {
	make_mid();
	make_ground();
	make_head();
	make_bulb();
	make_sphere();
	make_snow();
	make_tetra();
	make_planet_road();
}
void initbuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &snow_vbo);
	glGenBuffers(1, &sphere_vbo);
	glGenBuffers(1, &midvbo);
	glGenBuffers(1, &groundvbo);
	glGenBuffers(1, &bulb_vbo);
	glGenBuffers(1, &tetra_vbo);
	glGenBuffers(1, &planet_road_vbo);

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