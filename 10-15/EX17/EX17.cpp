
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
coord cube[8];
coord tetra[5];
coord spiral[75];
GLuint vao; // 삼각형 개수 저장
GLuint cubevbo; // 색상,정점 위치 저장
GLuint tetravbo;
GLuint midvbo;
GLuint spiralvbo;
GLuint ebo;
float cube_Xtheta = 30.0f;
float cube_Ytheta = 30.0f;
float cube_Ztheta = 0.0f;
unsigned int spiraldice[75];
unsigned int cubelinedice[] = {
	0,1,1,2,2,0,
	0,2,2,3,3,0,
	4,5,5,6,6,4,
	6,7,7,4,4,6,
	0,3,3,4,4,0,
	0,4,4,7,7,0,
	1,6,6,5,5,1,
	5,2,2,1,1,5,
	0,7,7,6,6,0,
	0,6,6,1,1,0,
	2,5,5,4,4,2,
	4,3,3,2,2,4
};
void change(int val);
void specialkey(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void rotation(int val);
void drawmidline();
void initbuffer();
void drawspiral();
void drawcube();
void drawsphere();
void spin(int val);
GLvoid drawScene();
void makeshape();
void make_cube();
void make_spiral();
GLvoid Reshape(int w, int h);
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
GLUquadricObj* qobj;
int rotating_mode = 0;
int spin_mode = 0;
bool already_rotating = false;
bool already_spin = false;
int drawing_case = 0;
float Xmove = 0;
float Ymove = 0;
float Zmove = 0;
float Xscale = 0.25f;
float Yscale = 0.25f;
float Zscale = 0.25f;
bool scalemode = false;

float useoneY;
float theta = 0;
void main(int argc, char** argv) {
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("EX 17");
	glewExperimental = GL_TRUE;
	glewInit();
	initbuffer();
	makeshape();
	make_shaderProgram();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkey);
	glutMainLoop();
}
bool draw_spiral = false;
float svX, svY, svZ;
float radious;
void specialkey(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		Xmove -= 0.1;
		svX = Xmove;
	}
	if (key == GLUT_KEY_RIGHT) {
		Xmove += 0.1;
		svX = Xmove;
	}
	if (key == GLUT_KEY_UP) {
		Ymove += 0.1;
		svY = Ymove;
	}
	if (key == GLUT_KEY_DOWN) {
		Ymove -= 0.1;
		svY = Ymove;
	}
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 'X') {
		if (!already_rotating) {
			rotating_mode = 1;
			glutTimerFunc(30, rotation, 0);
			already_rotating = true;
		}
		else if (already_rotating) {
			rotating_mode = 1;
		}
	}
	if (key == 'x') {
		if (!already_rotating) {
			rotating_mode = 0;
			glutTimerFunc(30, rotation, 0);
			already_rotating = true;
		}
		else if (already_rotating) {
			rotating_mode = 0;
		}
	}
	if (key == 'Y') {
		if (!already_rotating) {
			rotating_mode = 2;
			glutTimerFunc(30, rotation, 0);
			already_rotating = true;
		}
		else if (already_rotating) {
			rotating_mode = 2;
		}
	}
	if (key == 'y') {
		if (!already_rotating) {
			rotating_mode = 3;
			glutTimerFunc(30, rotation, 0);
			already_rotating = true;
		}
		else if (already_rotating) {
			rotating_mode = 3;
		}
	}
	if (key == 's') {
		already_spin = false;
		already_rotating = false;
		rotating_mode = -1;
		spin_mode = -1;
	}
	if (key == 'h') {
		if (!scalemode) {
			scalemode = true;
			Xmove = 0;
			Ymove = 0;
			Zmove = 0;
		}
		else {
			scalemode = false;
			Xmove = 0;
			Ymove = 0;
			Zmove = 0;
		}
	}
	if (key == '.') {
		Zmove += 0.1;
		svZ = Zmove;
	}
	if (key == '/') {
		Zmove -= 0.1;
		svZ = Zmove;
	}
	if (key == 't') {
		if (!already_spin) {
			spin_mode = 0 ;
			svX = Xmove;
			svY = Ymove;
			svZ = Zmove;
			glutTimerFunc(30, spin, 0);
			already_spin = true;
		}
		else if (already_spin) {
			spin_mode = 0;
		}
	}
	if (key == '1') {
		if (!already_spin) {
			spin_mode = 1;
			svX = Xmove;
			svY = Ymove;
			svZ = Zmove;
			glutTimerFunc(30, spin, 0);
			already_spin = true;
		}
		else if (already_spin) {
			spin_mode = 1;
			svX = Xmove;
			svY = Ymove;
			svZ = Zmove;
		}
	}
	if (key == '2') {
		if (!already_spin) {
			spin_mode = 2;
			radious = glm::sqrt(glm::pow(Xmove, 2) + glm::pow(Zmove, 2));
			glutTimerFunc(30, spin, 0);
			already_spin = true;
		}
		else if (already_spin) {
			spin_mode = 2;
		}
	}
	if (key == '3') {
		if (!already_spin) {
			svX = Xmove;
			svY = Ymove;
			svZ = Zmove;
			spin_mode = 3;
			glutTimerFunc(30, spin, 0);
			already_spin = true;

		}
		else if (already_spin) {
			svX = Xmove;
			svY = Ymove;
			svZ = Zmove;
			spin_mode = 3;
		}
	}
	if (key == 'r') {
		if (!draw_spiral) {
			make_spiral();
			draw_spiral = true;
			spin_mode = 4;
			svX = Xmove;
			svY = Ymove;
			svZ = Zmove;
			glutTimerFunc(30, spin, 0);
		}
		else {
			draw_spiral = false;
		}
	}
	if (key == '[') {
		Xscale += 0.001f;
		Yscale += 0.001f;
		Zscale += 0.001f;
	}
	if (key == ']') {
		Xscale -= 0.001f;
		Yscale -= 0.001f;
		Zscale -= 0.001f;
	}
	glutPostRedisplay();
}
int check = 0;
bool reflect = false;
float spintheta = 0.0f;
int count = 0;
 void spin(int val) {
	if (spin_mode == 0) {
		if (!reflect) {
			float t = check / 30.0f;
			glm::vec3 start(svX, svY, svZ);
			glm::vec3 end(0.0f, 0.0f, 0.0f);
			glm::vec3 interpolatedValue = glm::mix(start, end, t);//선형보간함수 사용
			Xmove = interpolatedValue.x;
			Ymove = interpolatedValue.y;
			Zmove = interpolatedValue.z;
			check++;
			if (check == 30) {
				reflect = true;
				check = 0;
			}
		}
		else {
			float t = check / 30.0f;
			glm::vec3 start(0.0f, 0.0f, 0.0f);
			glm::vec3 end(svX, svY, svZ);
			glm::vec3 interpolatedValue = glm::mix(start, end, t);//선형보간함ㅅ수 사용해봄
			Xmove = interpolatedValue.x;
			Ymove = interpolatedValue.y;
			Zmove = interpolatedValue.z;
			check++;
			if (check == 30) {
				reflect = false;
				spin_mode = -1;
				already_spin = false;
				check = 0;
			}
		}
		glutPostRedisplay();
		glutTimerFunc(30, spin, 0);
	}
	if (spin_mode == 1) { // 선형보간 식 사용
		if (!reflect) {
			float t = check / 30.0f;
			Xmove = (1 - t) * svX;
			Ymove = (1 - t) * svY;
			Zmove = (1 - t) * svZ;
			check++;
			if (check == 30) {
				reflect = true;
				check = 0;
			}
		}
		else {
			float t = check / 30.0f;
			Xmove = -(t)*svX;
			Ymove = (t)*svY;
			Zmove = -(t)*svZ;

			check++;
			if (check == 30) {
				reflect = false;
				spin_mode = -1;
				already_spin = false;
				check = 0;

			}
		}
		glutPostRedisplay();
		glutTimerFunc(30, spin, 0);
	}
	if (spin_mode == 2) {
		glm::vec3 center(Xmove, Ymove, Zmove);
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 transformedCenter = rotationMatrix * glm::vec4(center, 1.0f);
		glm::vec3 finalCenter = glm::vec3(transformedCenter);
		Xmove = finalCenter.x;
		Ymove = finalCenter.y;
		Zmove = finalCenter.z;

		theta++;
		if (theta >= 180) {
			spin_mode = -1;
			already_spin = false;
			theta = 0;
		}

		glutPostRedisplay();
		glutTimerFunc(30, spin, 0);
	}
	if (spin_mode == 3) {
		if (!reflect) {
			float t = check / 30.0f;
			Xmove = (1 - t) * svX;
			Ymove = (1 - t) * svY + t * (2.0f + svY);
			useoneY = -Ymove;
			Zmove = (1 - t) * svZ;
			check++;
			if (check == 30) {
				reflect = true;
				check = 0;
			}
		}
		else {
			float t = check / 30.0f;
			Xmove = -(t)*svX;
			Ymove =  (t) * svY +(1 -t) * (2.0f + svY);
			useoneY = -Ymove;
			Zmove = -(t)*svZ;
			check++;
			if (check == 30) {
				reflect = false;
				spin_mode = -1;
				already_spin = false;
				check = 0;
			}
		}
		glutPostRedisplay();
		glutTimerFunc(30, spin, 0);
	} 
	if (spin_mode == 4) {
		if (!reflect) {
			float t = check / 30.0f;
			Xmove = (1 - t) * svX;
			Ymove = Ymove;
			Zmove = (1 - t) * svZ;
			check++;
			if (check == 30) {
				reflect = true;
				check = 0;
			}
		}
		else {
			float a = 0.01;
			float b = 0.02;
			float r = a + b * spintheta;
			if (!scalemode) {
				Xmove = 0.04 * r * (glm::cos(glm::radians(spintheta))) / Xscale;
				Zmove = -0.04 * r * (glm::sin(glm::radians(spintheta))) / Zscale;
			}
			else {
				Xmove = 0.04 * r * (glm::cos(glm::radians(spintheta))) ;
				Zmove = -0.04 * r * (glm::sin(glm::radians(spintheta))) ;
				Ymove *= Yscale;
			}
			spintheta += 18.4;
			count++;
			if (count == 75) {
				count = 0;
				spin_mode = -1;
				reflect = false;
				already_spin = false;
				spintheta = 0;
			}
		}
		glutPostRedisplay();
		glutTimerFunc(30, spin, 0);
	}
	glutPostRedisplay();
}
void drawsphere() {
	qobj = gluNewQuadric();

	glLineWidth(2.0f);
	gluQuadricDrawStyle(qobj, GLU_LINE);
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(Xscale, Yscale, Zscale));

	if (!scalemode) {
		modelMatrix *= scaleMatrix;
	}
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix *= rotationMatrix;

	translationMatrix = glm::translate(translationMatrix, glm::vec3(Xmove, Ymove, Zmove));
	modelMatrix *= translationMatrix;

	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Xtheta), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix *= rotationMatrix;
	if (scalemode) {
		modelMatrix *= scaleMatrix;
	}
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	gluSphere(qobj, 0.5, 10, 10);

}
void drawcube() {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 translationMatrix = glm::mat4(1.0f);
	if (spin_mode == 3) {
		translationMatrix = glm::translate(translationMatrix, glm::vec3(-Xmove, -Ymove+2*svY, -Zmove));
	}
	else {
		translationMatrix = glm::translate(translationMatrix, glm::vec3(-Xmove,Ymove, -Zmove));
	}
	scaleMatrix = glm::scale(scaleMatrix, glm::vec3(Xscale, Yscale, Zscale));
	if (!scalemode) {
		modelMatrix *= scaleMatrix;
	}
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix *= rotationMatrix;

	modelMatrix *= translationMatrix;

	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Xtheta), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Ytheta), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(cube_Ztheta), glm::vec3(0.0f, 0.0f, 1.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix *= rotationMatrix;

	if (scalemode) {
		modelMatrix *= scaleMatrix;
	}
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));


	glLineWidth(2.0f);
	glBindBuffer(GL_ARRAY_BUFFER, cubevbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), cube, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubelinedice), cubelinedice, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_LINES, 72, GL_UNSIGNED_INT, 0);

}
void rotation(int val) {
	if (rotating_mode == 0) {
		cube_Xtheta -= 1;
		glutTimerFunc(30, rotation, 0);
	}
	else if (rotating_mode == 1) {
		cube_Xtheta += 1;
		glutTimerFunc(30, rotation, 0);
	}
	else if (rotating_mode == 2) {
		cube_Ytheta += 1;
		glutTimerFunc(30, rotation, 0);
	}
	else if (rotating_mode == 3) {
		cube_Ytheta -= 1;
		glutTimerFunc(30, rotation, 0);
	}
	else {
		return;
	}
	glutPostRedisplay();
}
GLvoid drawScene() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	drawmidline();
	drawcube();
	drawsphere();
	if (draw_spiral) {
		drawspiral();
	}
	glutSwapBuffers();
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
void drawspiral() {
	glLineWidth(2.0f);
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix *= rotationMatrix;
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glLineWidth(2.0f);
	glBindBuffer(GL_ARRAY_BUFFER, spiralvbo);
	glBufferData(GL_ARRAY_BUFFER, 75 * 6 * sizeof(float), spiral, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(spiraldice), spiraldice, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_LINE_STRIP, 75, GL_UNSIGNED_INT, 0);
}
void initbuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &cubevbo);
	glGenBuffers(1, &spiralvbo);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
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

	make_cube();
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
void make_spiral() {
	float newtheta = 0.0f;
	float a = 0.01;
	float b = 0.02;
	for (int i = 0; i < 75; i++) {
		float r = a + b * newtheta;
		spiral[i].x = 0.04* r * (glm::cos(glm::radians(newtheta)));
		spiral[i].y = Ymove * Yscale;
		spiral[i].z = 0.04* r * (glm::sin(glm::radians(newtheta)));
		spiral[i].r = 0.8f;
		spiral[i].g = 0.9f;
		spiral[i].b = 0.3f;
		spiraldice[i] = i;
		newtheta += 18.4;
	}
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