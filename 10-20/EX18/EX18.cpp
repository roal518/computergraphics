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
unsigned int cubedices[] = {
	0, 1, 2, 0, 2, 3,   // 정면 사각형 1
	4, 5, 6, 6, 7, 4,   // 후면 사각형 2
	1, 6, 5, 5, 2, 1,   // 하면 사각형 2
	0, 3, 4, 0, 4, 7,   // 상면 사각형 3
	0, 7, 6, 0, 6, 1,   // 좌측 사각형 4
	2, 5, 4, 4, 3, 2    // 우측 사각형 4
};
float cube_info[6][3];//이동,회전,스케일
coord tetra[5];
unsigned int tetradices[] = {
	0,2,1,0,3,2,
	0,1,4,
	1,2,4,
	2,3,4,
	3,0,4
};
GLuint vao; // 삼각형 개수 저장
GLuint cubevbo; // 색상,정점 위치 저장
GLuint tetravbo;
GLuint midvbo;
GLuint ebo;

void keyboard(unsigned char key, int x, int y);
void get_cube();
void spin(int val);
void scale(int val);
void open_door(int val);
void open_front(int val);
void Y_axis_spin(int val);
void makeshape();
void t1_spin(int val);
void rand_spin(int val);
void make_tetra();
void make_cube();
void initbuffer();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
void draw2page();
float theta = 0;
float Ytheta = 0;
float tetrathetaY = 0.0f;
float tetratheta[4] = { 0.0f ,0.0f ,0.0f ,0.0f };
float opentheta = 0;
float scaling = 5.0f;
float Xmove = 0;
float Ymove = 0;
float Zmove = 0;
bool is_culling = true;
bool movement = false;
void menu() {
	printf("h: 은면제거/해제\n");
	printf("y: y축 자전\n");
	printf("t: 육면체 윗면 회전\n");
	printf("f: 육면체 앞면 열기\n");
	printf("s: 육면체 옆면 열기\n");
	printf("b: 육면체 뒷면 신축\n");
	printf("o: 사각뿔 전체 회전\n");
	printf("r: 사각뿔 한면 회전\n");
	printf("p: 직각 투영/원근 투영\n");

}
void main(int argc, char** argv) {
	menu();
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 18");
	glewExperimental = GL_TRUE;
	glewInit();
	initbuffer();
	makeshape();
	make_shaderProgram();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
bool top_spin = false;
bool get_scale = false;
bool open_side = false;
bool open_f = false;
bool y_spin = false;
bool all_spin = false;
bool p2_spin = false;
bool p3_spin = false;
bool p4_spin = false;
bool change_poly = false;
bool is_back = false;
int rand1, rand2;
bool proj_mode = false;
void keyboard(unsigned char key, int x, int y) {
	if (key == 'h') {
		if (!is_culling) {
			is_culling = true;
		}
		else {
			is_culling = false;
		}
	}
	if (key == 't') {//top spin
		if (!movement) {
			top_spin = true;
			glutTimerFunc(30, spin, 0);
		}
		else {
			top_spin = false;
		}
	}
	if (key == 's') {//open fw face
		if (!movement) {
			open_side = true;
			glutTimerFunc(30, open_door, 0);

		}
		else {
			open_side = false;
		}
	}
	if (key == 'f') {//open fw face
		if (!open_f) {
			open_f = true;
			glutTimerFunc(30, open_front, 0);

		}
		else {
			open_f = false;
		}
	}
	if (key == 'b') {
		if (!movement) {
			get_scale = true;
			glutTimerFunc(30, scale, 0);

		}
		else {
			get_scale = false;
		}
	}
	if (key == 'y') {
		if (!y_spin) {
			y_spin = true;
			glutTimerFunc(30, Y_axis_spin, 0);
		}
		else {
			y_spin = false;
			glutTimerFunc(30, Y_axis_spin, 0);
		}
	}
	if (key == 'o') {
		if (!all_spin) {
			all_spin = true;
			if (p2_spin) {
				p2_spin = false;
			}
			glutTimerFunc(30, t1_spin, 0);
		}
		else {
			all_spin = false;
			glutTimerFunc(30, t1_spin, 0);
		}
	}
	if (key == 'r') {
		if (!is_back) {
			rand1 = rand() % 4;
			rand2 = rand() % 4;
			is_back = true;
		}
		while (rand1 == rand2) {
			rand2 = rand() % 4;
		}
		printf("%d, %d\n", rand1, rand2);
		if (!p2_spin) {

			p2_spin = true;
			glutTimerFunc(30, rand_spin, 0);
		}
		else {
			p2_spin = false;
			glutTimerFunc(30, rand_spin, 0);
		}
	}
	if (key == 'p') {
		if (!proj_mode) {
			proj_mode = true;
		}
		else {
			proj_mode = false;
		}
	}
	if (key == 'c') {
		if (!change_poly) {
			change_poly = true;
		}
		else {
			change_poly = false;
		}
	}
	glutPostRedisplay();
}
bool random_spin = false;
bool turn_time = false;
bool limitheight = false;
bool limitangle = false;
bool checkt90 = false;
void rand_spin(int val) {
	if (p2_spin) {
		if (tetrathetaY >= 0) {
			tetrathetaY--;
			all_spin = false;
		}
		if (!checkt90) {
			tetratheta[rand1]++;
			tetratheta[rand2]++;
			if (tetratheta[rand1] >= 90) {
				checkt90 = true;
				p2_spin = false;
			}
		}
		else {
			tetratheta[rand1]--;
			tetratheta[rand2]--;
			if (tetratheta[rand1] <= 0) {
				checkt90 = false;
				p2_spin = false;
				is_back = false;
			}
		}

		glutTimerFunc(30, rand_spin, 0);
	}
	glutPostRedisplay();
}
void t1_spin(int val) {
	if (all_spin) {
		if (tetrathetaY >= 233) {
			all_spin = false;
		}
		tetrathetaY++;		
		glutTimerFunc(30, t1_spin, 0);
	}
	else {
		return;
	}
	glutPostRedisplay();
}
void spin(int val) {
	if (top_spin) {
		theta++;
	}
	glutTimerFunc(30, spin, 0);
	glutPostRedisplay();
}
void scale(int val) {
	if (get_scale) {
		if (!turn_time) {
			scaling -= 0.075;
			if (scaling <= 0) {
				turn_time = true;
			}
		}
		else if (turn_time) {
			scaling += 0.075;
			if (scaling >= 5) {
				turn_time = false;
			}
		}
	}

	glutTimerFunc(30, scale, 0);
	glutPostRedisplay();
}
void open_door(int val) {
	if (open_side) {
		if (!limitheight) {
			Ymove += 0.01;
			if (Ymove >= 1.0f) {
				limitheight = true;
				open_side = false;
			}
		}
		else if (limitheight) {
			Ymove -= 0.01;
			if (Ymove <= 0.0) {
				limitheight = false;
				open_side = false;
			}
		}
	}
	glutTimerFunc(30, open_door, 0);
	glutPostRedisplay();

}
void open_front(int val) {
	if (open_f) {
		if (!limitangle) {
			opentheta--;
			if (opentheta <= -90.0f) {
				limitangle = true;
				open_f = false;
			}
		}
		else if (limitangle) {
			opentheta++;
			if (opentheta >= 0.0f) {
				limitangle = false;
				open_f = false;
			}
		}
	}
	glutTimerFunc(30, open_front, 0);
	glutPostRedisplay();
}
void Y_axis_spin(int val) {
	if (y_spin) {
		Ytheta++;
		glutTimerFunc(30, Y_axis_spin, 0);
	}
	else {
		return;
	}
	glutPostRedisplay();
}
void drawmidline() {
	glLineWidth(2.0f);
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &midvbo);
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix *= rotationMatrix;
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
void draw1page() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f + Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(theta), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));



	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(float)));//뚜껑
}
void draw2page() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaling, scaling, scaling));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f + Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(6 * sizeof(float)));//뒤,바닥
}
void draw3page() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f + Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(Xmove, Ymove, Zmove));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));


	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(24 * sizeof(float)));//측면
}
void draw4page() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f + Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.5f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(opentheta), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, -0.5f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(float)));
}
void get_cube() {
	glBindBuffer(GL_ARRAY_BUFFER, cubevbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), cube, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubedices), cubedices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	draw1page();
	draw2page();
	draw3page();
	draw4page();
}
void tetra_floor() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f + Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(float)));//뒤,바닥

}
void tetra1p() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f + Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(tetrathetaY +tetratheta[0]), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 0.0f, 0.0f));

	//	modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 6.0f, 0.0f));

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(6 * sizeof(float)));//뒤,바닥
}
void tetra2p() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f + Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-tetrathetaY - tetratheta[1]), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 3.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(9 * sizeof(float)));//뒤,바닥

}
void tetra3p() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f + Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-tetrathetaY -tetratheta[2]), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(12 * sizeof(float)));//뒤,바닥

}
void tetra4p() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(30.0f + Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));

	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 3.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(tetrathetaY +tetratheta[3]), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(15 * sizeof(float)));//뒤,바닥

}
void get_tetra() {

	glBindBuffer(GL_ARRAY_BUFFER, tetravbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), tetra, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetradices), tetradices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	tetra_floor();
	tetra1p();
	tetra2p();
	tetra3p();
	tetra4p();
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
	//**사각뿔
	//정면h
	make_tetra();
}
void make_tetra() {
	tetra[0].x = -3;
	tetra[0].y = 0;
	tetra[0].z = 3;

	tetra[1].x = -3;
	tetra[1].y = 0;
	tetra[1].z = -3;

	tetra[2].x = 3;
	tetra[2].y = 0;
	tetra[2].z = -3;

	tetra[3].x = 3;
	tetra[3].y = 0;
	tetra[3].z = 3;

	tetra[4].x = 0;
	tetra[4].y = 6;
	tetra[4].z = 0;
	for (int i = 0; i < 4; i++) {
		tetra[i].r = static_cast<float>(rand()) / RAND_MAX;
		tetra[i].g = static_cast<float>(rand()) / RAND_MAX;
		tetra[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
}
void make_cube() {
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
void initbuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &cubevbo);
	glGenBuffers(1, &tetravbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}
void viewproj() {
	GLuint projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	GLuint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	if (!proj_mode) {
		projection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, -25.0f, 25.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	}
	else {
		projection = glm::perspective(glm::radians(100.0f), 0.8f, 0.1f, 50.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -25.0));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	}
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

}
GLvoid drawScene() {
	viewproj();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (is_culling) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
	if (change_poly) {
		get_cube();
	}
	else {
		get_tetra();
	}
	drawmidline();
	glutSwapBuffers();
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