#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>

GLfloat red, green, blue; // RGB 색상 변수

GLvoid drawScene(GLvoid);
void Reshape(int w, int h);
void timermake(int val);

int timer = 0;

void handleKeyDown(unsigned char key, int x, int y) {
    if (key == 'c' || key == 'C') {
        red = 0.0f;
        green = 1.0f;
        blue = 1.0f;
    }
    else if (key == 'm' || key == 'M') {
        red = 1.0f;
        green = 0.0f;
        blue = 1.0f;
    }
    else if (key == 'y' || key == 'Y') {
        red = 1.0f;
        green = 1.0f;
        blue = 0.0f;
    }
    else if (key == 'w' || key == 'W') {
        // "b" 키를 누를 때 검은색 설정
        red = 1.0f;
        green = 1.0f;
        blue = 1.0f;
    }
    else if (key == 'k' || key == 'K') {
        // "b" 키를 누를 때 검은색 설정
        red = 0.0f;
        green = 0.0f;
        blue = 0.0f;
    }
    else if (key == 'a' || key == 'A') {
       red = static_cast<GLfloat>(rand()) / RAND_MAX;
       green = static_cast<GLfloat>(rand()) / RAND_MAX;
       blue = static_cast<GLfloat>(rand()) / RAND_MAX;
    }
    else if (key == 't' || key == 'T') {
        if (timer == 0) {
            glutTimerFunc(1000, timermake, 0);
            timer = 1;
        }
    }
    else if (key == 's' || key == 'S') {
        if (timer == 1) {
            glutTimerFunc(0, timermake, 0);
            timer = 0;
        }
    }
    else if (key == 27 || key == 'q'||key=='Q') {
        exit(0);
    }
    glClearColor(red, green, blue, 1.0f);
    glutPostRedisplay(); // 화면 다시 그리기 요청
}
void timermake(int val) {
    if (timer == 1) {
        red = static_cast<GLfloat>(rand()) / RAND_MAX;
        green = static_cast<GLfloat>(rand()) / RAND_MAX;
        blue = static_cast<GLfloat>(rand()) / RAND_MAX;
        glClearColor(red, green, blue, 1.0f);
        glutPostRedisplay();
        glutTimerFunc(1000, timermake, 0);
    }
    else if (timer == 0) {
        glutTimerFunc(0, timermake, 0);

    }
}
void drawScene(GLvoid) {
    glClear(GL_COLOR_BUFFER_BIT);
    // 그리기 코드 작성
    glutSwapBuffers();
}
void Reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

int main(int argc, char** argv) {
    // GLUT 초기화
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("EX 1");

    // 초기 배경색 설정 (흰색)
    red = 1.0f;
    green = 1.0f;
    blue = 1.0f;
    glClearColor(red, green, blue, 1.0f);

    //이벤트 콜백 함수 등록
    glutKeyboardFunc(handleKeyDown);
    // 다른 콜백 함수 등록
    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);

    // 메인 루프 시작
    glutMainLoop();

    return 0;
}
