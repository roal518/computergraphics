#include "function.h"
#include <iostream>

int main(int argc, char** argv)
{
	/***********************************
	[����]
	1.�����츦 �����Ѵ�.
	2.2.0�̻� Ȯ�� ����� ����ϱ� ���� GLEW�� ���� �������� �ʱ�ȭ�Ѵ�
	3.�پ��� �ݹ� �Լ����� ȣ���Ѵ�.
	***********************************/

	/*********������ �����ϱ�*********/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(550, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Drill33");

	/*********GLEW �ʱ�ȭ�ϱ�*********/
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	/***********���� �ʱ�ȭ***********/
	/*���̴� ���α׷�*/
	make_shaderProgram();
	/*��ü �ʱ�ȭ*/
	InitObject();
	InitTexture();
	/*vao�� vbo�����ϱ�*/
	InitBuffer();
	/*�� ��*/
	glEnable(GL_DEPTH_TEST); //���� ����
	printf("[q]:���α׷� ����\n");
	/*********�ݹ� �Լ�(�ü�� ȣ�� �Լ�) ȣ���ϱ�*********/

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(50, TimerFunction, 1);
	glutMainLoop();

	return 0;
}
