#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<gl/glew.h>
#include <gl/glut.h>
#include<gl/freeglut.h>
#include<gl/freeglut_ext.h>
#define MAX_RECTANGLES 8
typedef struct rectangle {
    float x1 = 0, y1 = 0, x2 = 0, y2 = 0; // 위치 
    float r=0, g=0, b=0; // 색상
    int index = 0;
} rect;//사각형 데이터 저장

GLfloat red, green, blue; // RGB 색상 변수
float winWidth = 800;
float winHeight = 600;
bool left_botton = true; 
bool right_botton = true;

rect rectangles[MAX_RECTANGLES];
bool atStart = true;
GLvoid drawfirstScene(GLvoid);
void Reshape(int w, int h);
void Clickmouse(int button, int state, int x, int y);

void drawRectangle(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}
void Clickmouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        left_botton = true;
        double nomalX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
        double nomalY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
        for (int i = 0; i < MAX_RECTANGLES/2; i++) {
            if ((rectangles[i].x1 <= nomalX && nomalX <= rectangles[i + 4].x1 ||
                rectangles[i + 4].x2 <= nomalX && nomalX <= rectangles[i].x2) &&
                (rectangles[i].y1 <= nomalY && nomalY <= rectangles[i + 4].y1 ||
                rectangles[i + 4].y2 <= nomalY && nomalY <= rectangles[i].y2)) {
                rectangles[i].r = static_cast<float>(rand()) / RAND_MAX;
                rectangles[i].g = static_cast<float>(rand()) / RAND_MAX;
                rectangles[i].b = static_cast<float>(rand()) / RAND_MAX;
            }
        }
        for (int i = MAX_RECTANGLES / 2; i < MAX_RECTANGLES; i++) {
            if ((rectangles[i].x1 <= nomalX && nomalX <= rectangles[i].x2)&&
                rectangles[i].y1 < nomalY && nomalY <= rectangles[i].y2) {
                rectangles[i].r = static_cast<float>(rand()) / RAND_MAX;
                rectangles[i].g = static_cast<float>(rand()) / RAND_MAX;
                rectangles[i].b = static_cast<float>(rand()) / RAND_MAX;
            }
        }
        glutPostRedisplay();
    }
    else if (state == GLUT_UP) {
        left_botton = false;

    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        right_botton = true;
        double nomalX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
        double nomalY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
        for (int i = 0; i < MAX_RECTANGLES / 2; i++) {
            if ((rectangles[i].x1 <= nomalX && nomalX <= rectangles[i + 4].x1 ||
                rectangles[i + 4].x2 <= nomalX && nomalX <= rectangles[i].x2) &&
                (rectangles[i].y1 <= nomalY && nomalY <= rectangles[i + 4].y1 ||
                    rectangles[i + 4].y2 <= nomalY && nomalY <= rectangles[i].y2)) {
                float newWidth = rectangles[i+4].x2 - rectangles[i+4].x1 + 0.1f;
                float newHeight = rectangles[i+4].y2 - rectangles[i+4].y1 + 0.1f;
                if (newWidth >= 1.0f) {
                    newWidth = 1.0f;
                }
                if (newHeight >= 1.0f) {
                    newHeight = 1.0f;
                }
                rectangles[i+4].x1 += (rectangles[i+4].x2 - rectangles[i+4].x1 - newWidth) / 2;
                rectangles[i+4].y1 += (rectangles[i+4].y2 - rectangles[i+4].y1 - newHeight) / 2;
                rectangles[i+4].x2 = rectangles[i+4].x1 + newWidth;
                rectangles[i+4].y2 = rectangles[i+4].y1 + newHeight;
            }
        }
        for (int i = MAX_RECTANGLES / 2; i < MAX_RECTANGLES; i++) {
            if ((rectangles[i].x1 <= nomalX && nomalX <= rectangles[i].x2) &&
                rectangles[i].y1 < nomalY && nomalY <= rectangles[i].y2) {
                float newWidth = rectangles[i].x2 - rectangles[i].x1 - 0.1f;
                float newHeight = rectangles[i].y2 - rectangles[i].y1 - 0.1f;
                if (newWidth < 0.1f) {
                    newWidth = 0.1f;
                }
                if (newHeight < 0.1f) {
                    newHeight = 0.1f;
                }
                rectangles[i].x1 += (rectangles[i].x2 - rectangles[i].x1 - newWidth) / 2;
                rectangles[i].y1 += (rectangles[i].y2 - rectangles[i].y1 - newHeight) / 2;
                rectangles[i].x2 = rectangles[i].x1 + newWidth;
                rectangles[i].y2 = rectangles[i].y1 + newHeight;
            }
        }
        glutPostRedisplay();
    }
    else if (state == GLUT_UP) {
        right_botton = false;
    }
}
void handleKeyDown(unsigned char key, int x, int y) {
    if (key == 27 || key == 'q' || key == 'Q') {
        exit(0);
    }
}
void drawfirstScene(GLvoid) {
    glClear(GL_COLOR_BUFFER_BIT);
    // 그리기 코드 작성
    if (atStart) {
        for (int i = 0; i < MAX_RECTANGLES; i++) {
            red = static_cast<float>(rand()) / RAND_MAX;
            green = static_cast<float>(rand()) / RAND_MAX;
            blue = static_cast<float>(rand()) / RAND_MAX;
            rectangles[i].r = red;
            rectangles[i].g = green;
            rectangles[i].b = blue;
            glColor3f(rectangles[i].r, rectangles[i].g, rectangles[i].b);
            drawRectangle(rectangles[i].x1, rectangles[i].y1, rectangles[i].x2, rectangles[i].y2);
        }
        glutSwapBuffers();
        atStart = false;
    }
    else if (!atStart) {
        for (int i = 0; i < MAX_RECTANGLES; i++) {
            glColor3f(rectangles[i].r, rectangles[i].g, rectangles[i].b);
            drawRectangle(rectangles[i].x1, rectangles[i].y1, rectangles[i].x2, rectangles[i].y2);
            rectangles[i].index = 0;
        }
        glutSwapBuffers();
    }

}
void Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}
//뷰포트 설정 당장은 만질일이 없다.
int main(int argc, char** argv) {
    srand(unsigned(time(NULL)));
    // GLUT 초기화
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 100);
    glutInitWindowSize((int)winWidth, (int)winHeight);
    glutCreateWindow("EX 2");
    {
        rectangles[0] = { 0.0f,0.0f,1.0f,1.0f };
        rectangles[1] = { -1.0f,0.0f,0.0f,1.0f };
        rectangles[2] = { -1.0f,-1.0f,0.0f,0.0f };
        rectangles[3] = { 0.0f,-1.0f,1.0f,0.0f };
        rectangles[4] = { 0.0f,0.0f,1.0f,1.0f };
        rectangles[5] = { -1.0f,0.0f,0.0f,1.0f };
        rectangles[6] = { -1.0f,-1.0f,0.0f,0.0f };
        rectangles[7] = { 0.0f,-1.0f,1.0f,0.0f };
    }
    glutDisplayFunc(drawfirstScene);
    glutReshapeFunc(Reshape);

    glutMouseFunc(Clickmouse);
    glutKeyboardFunc(handleKeyDown);
    //glutMotionFunc(mousemotion);
    // 다른 콜백 함수 등록
    // 메인 루프 시작
    glutMainLoop();

    return 0;
}
