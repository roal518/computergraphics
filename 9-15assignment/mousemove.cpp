#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<gl/glew.h>
#include <gl/glut.h>
#include<gl/freeglut.h>
#include<gl/freeglut_ext.h>
#define MAX_RECTANGLES 5
typedef struct rectangle {
	float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	float r = 0, g = 0, b = 0;
}rect;
float mouseX, mouseY;
int numRect = 0;
bool left_button = false;
rect rectangles[MAX_RECTANGLES];
int nowrectangle = 0;
void Reshape(int w, int h);
void drawScene(GLvoid);
void Movemouse(int x, int y);
void Clickmouse(int button, int state, int x, int y);
void handleKeyDown(unsigned char key, int x, int y);
void drawRectangle(float x1, float y1, float x2, float y2);
float randomXY();
int main(int argc, char** argv) {
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("EX 3");
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMotionFunc(Movemouse);
	glutKeyboardFunc(handleKeyDown);
	glutMouseFunc(Clickmouse);

	glutMainLoop();
	return 0;
}
void Reshape(int w, int h) {
	glViewport(0, 0, w, h);
}
void drawScene(GLvoid) {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < numRect; i++) {
		glColor3f(rectangles[i].r, rectangles[i].g, rectangles[i].b);
		drawRectangle(rectangles[i].x1, rectangles[i].y1, rectangles[i].x2, rectangles[i].y2);
	}
	glutSwapBuffers();
}
void Movemouse(int x, int y) {
	if (left_button) {
		float deltaX = (x - mouseX) /400.0f;
		float deltaY = (y - mouseY) / 400.0f;
		rectangles[nowrectangle].x1 += deltaX;
		rectangles[nowrectangle].y1 -= deltaY;
		rectangles[nowrectangle].x2 += deltaX;
		rectangles[nowrectangle].y2 -= deltaY;
		mouseX = x;
		mouseY = y;
		glutPostRedisplay();
	}
}
void Clickmouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float normalX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
		float normalY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
		for (int i = 0; i < numRect; i++) {
			if ((rectangles[i].x1 <= normalX && normalX <= rectangles[i].x2) &&
				rectangles[i].y1 < normalY && normalY <= rectangles[i].y2) {
				left_button = true;
				nowrectangle = i;
			}
		}
		mouseX = x;
		mouseY = y;
	}
	else if (state == GLUT_UP) {
		left_button = false;
	}
}
void handleKeyDown(unsigned char key, int x, int y) {
	if ((key == 'a' || key == 'A') && numRect < 5) {
		bool doitonce = true;
		float width = 0.2f; // 사각형 너비
		float height = 0.2f; // 사각형 높
		rectangle rect;
		rect.r = static_cast<float>(rand()) / RAND_MAX;
		rect.g = static_cast<float>(rand()) / RAND_MAX;
		rect.b = static_cast<float>(rand()) / RAND_MAX;
		rect.x1 = randomXY() * 2 - 1.0; // x 좌표 (-1에서 1까지)
		rect.y1 = randomXY() * 2 - 1.0; // y 좌표 (-1에서 1까지)
		rect.x2 = rect.x1 + width;
		rect.y2 = rect.y1 + height;
		rectangles[numRect] = rect;
		numRect++;
		glutPostRedisplay();
	}
	if (key == 27 || key == 'q' || key == 'Q') {
		exit(0);
	}
}
void drawRectangle(float x1, float y1, float x2, float y2) {
	glBegin(GL_QUADS);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
}
float randomXY() {
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
