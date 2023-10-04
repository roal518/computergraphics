#define _CRT_SECURE_NO_WARNINGS 
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
//기본 정보
coord dot[3];
coord line[3][2];
coord trian[3][3];
coord rect[3][2][4];

//합쳐서 나온 새 도형
coord newdot[3];