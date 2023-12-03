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
struct info {
	float height = 0.0f;
	float increaser = 0.0f;
	bool touch_ceil = false;
};
GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;

coord** box;
info** inform;//각각의 Y좌표를 받을 예정
coord ground[4];
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
GLuint vao;
GLuint box_vbo;
GLuint ground_vbo;
GLuint ebo;
int draw_S_level = 0;
int light_on = 0;
float Light_X = 0.f;
float Light_Y = 100.f;
float Light_Z = 0.f;
int speed = 30;
void init_box();
void remake_box(int dx, int dy);
void keyboard(unsigned char key, int x, int y);
void mouse_wheel(int wheel, int direction, int x, int y);
void initbuffer();
GLvoid drawScene();
void mouse_move(int x, int y);
void make_shaderProgram();
void make_vertexShaders();
void make_fragmentShaders();
char* filetobuf(const char* file);
void makeshape();
void timers(int val);
void menu() {
	printf("===Amazing Movement===\n");
	printf("\t=== INFO === \n");
	printf("미니맵은 우상단에 생성됩니다\n");
	printf("마우스로 시점을 회전 할 수 있습니다.\n");
	printf("예외 처리가 될때 그 값은 크기가 2로 설정됩니다\n");
	printf("마우스 휠을 이용해 y축에 대해 시점을 이동할수 있습니다\n");
	printf("이때 (**초기 시점에서만 모든 축이 자유롭게 회전됩니다, zx평면 회전 시 휠을 움직여 초기 시점으로 바꿔주십시오**)\n");
	printf("\t=== KEYS === \n");
	printf("1:\t랜덤 움직임\n");
	printf("2:\t계단식 움직임\n");
	printf("3:\t다이아몬드 움직임\n");
	printf("r:\t값 초기화\n");
	printf("c:\t조명 색 변경\n");
	printf("t:\t조명 효과 사용/미사용\n");
	printf("'-'또는'_':\t육면체 애니메이션 속도 증가\n");
	printf("'+'또는'=':\t육면체 애니메이션 속도 감소\n");
	printf("ESC:\t프로그램 종료\n");
}
void main(int argc, char** argv) {
	menu();
	makeshape();
	init_box();
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("A_M_EX");
	glewExperimental = GL_TRUE;
	glewInit();
	initbuffer();
	make_shaderProgram();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(speed, timers, 0);
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
float x_cam_move = 0.0f;
float y_cam_move = 100.f;
float z_cam_move = sqrt(101);
float cam_height = 0.0f;
float light_theta = 0.0f;
float light_red = 1.0f;
float light_green = 1.0f;
float light_blue = 1.0f;
float light_str = 0.01f;
int x_line = 3;
int z_line = 3;
int info_x = 2;
int info_z = 2;
bool first_case = false;
bool second_case = false;
bool third_case = false;
bool forth_case = false;
void timers(int val) {
	if (first_case) {
		for (int i = 0; i < info_x; i++) {
			for (int j = 0; j < info_z; j++) {
				inform[i][j].increaser = static_cast<float>(rand()) / RAND_MAX;
				if (!inform[i][j].touch_ceil) {
					inform[i][j].height += 0.1f + inform[i][j].increaser;
					if (inform[i][j].height > 30.f) {
						inform[i][j].touch_ceil = true;
					}
				}
				else {
					inform[i][j].height -= 0.1f + inform[i][j].increaser;
					if (inform[i][j].height < 0.1f) {
						inform[i][j].touch_ceil = false;
					}
				}
			}
		}
	}
	if (second_case) {
		for (int i = 0; i < info_x; i++) {
			for (int j = 0; j < info_z; j++) {
				if (!inform[i][j].touch_ceil) {
					inform[i][j].height += 0.1f + inform[i][j].increaser;
					if (inform[i][j].height > 30.f) {
						inform[i][j].touch_ceil = true;
					}
				}
				else {
					inform[i][j].height -= 0.1f + inform[i][j].increaser;
					if (inform[i][j].height < 0.1f) {
						inform[i][j].touch_ceil = false;
					}
				}
			}
		}
	}
	if (third_case) {
		for (int i = 0; i < info_x; i++) {
			for (int j = 0; j < info_z; j++) {
				if (!inform[i][j].touch_ceil) {
					inform[i][j].height += 0.1f + inform[i][j].increaser;
					if (inform[i][j].height > 30.f) {
						inform[i][j].touch_ceil = true;
					}
				}
				else {
					inform[i][j].height -= 0.1f + inform[i][j].increaser;
					if (inform[i][j].height < 0.1f) {
						inform[i][j].touch_ceil = false;
					}
				}
			}
		}
	}
	if (forth_case) {
		for (int i = 0; i < info_x; i++) {
			for (int j = 0; j < info_z; j++) {
				if (!inform[i][j].touch_ceil) {
					inform[i][j].height += 0.1f + inform[i][j].increaser;
					if (inform[i][j].height > 30.f) {
						inform[i][j].touch_ceil = true;
					}
				}
				else {
					inform[i][j].height -= 0.1f + inform[i][j].increaser;
					if (inform[i][j].height < 0.1f) {
						inform[i][j].touch_ceil = false;
					}
				}
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(speed, timers, 0);
}
void init_box() {
	box = (coord**)malloc(x_line * sizeof(coord*));
	for (int i = 0; i < z_line; i++) {
		box[i] = (coord*)malloc(z_line * sizeof(coord));
	}
	inform = (info**)malloc(info_x * sizeof(info*));
	for (int i = 0; i < info_z; i++) {
		inform[i] = (info*)malloc(info_z * sizeof(info));
	}
	float tx = (ground[2].x - ground[0].x) / (x_line - 1);
	float tz = (ground[2].z - ground[0].z) / (z_line - 1);
	for (int i = 0; i < x_line; i++) {
		for (int j = 0; j < z_line; j++) {
			box[i][j].x = ground[0].x + tx * (i);
			box[i][j].y = ground[0].y;
			box[i][j].z = ground[0].z + tz * (j);
			box[i][j].r = static_cast<float>(rand()) / RAND_MAX;
			box[i][j].g = static_cast<float>(rand()) / RAND_MAX;
			box[i][j].b = static_cast<float>(rand()) / RAND_MAX;
		}
	}
	for (int i = 0; i < info_x; i++) {
		for (int j = 0; j < info_x; j++) {
			inform[i][j].height = 0.0f;
			inform[i][j].increaser = 0.0f;
			inform[i][j].touch_ceil = false;
		}
	}
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
	x_cam_move = glm::cos(glm::radians(camYaw)) * glm::cos(glm::radians(camPit));
	y_cam_move = glm::sin(glm::radians(camPit));
	z_cam_move = glm::sin(glm::radians(camYaw)) * glm::cos(glm::radians(camPit));
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}
	if (key == '1') {
		first_case = true;
		second_case = false;
		third_case = false;
		forth_case = false;
		if (info_x > info_z) {
			for (int i = 0; i < info_x; i++) {
				for (int j = 0; j < info_z; j++) {
					inform[i][j].height = 0.0f;
					inform[i][j].increaser = 0.0f;
					inform[i][j].touch_ceil = false;
				}
			}
		}
	}
	if (key == '2') {
		first_case = false;
		second_case = true;
		third_case = false;
		forth_case = false;
		float creaser = 0.001f;
		if (info_x > info_z) {
			for (int i = 0; i < info_x; i++) {
				creaser += 0.01f;
				for (int j = 0; j < info_z; j++) {
					inform[i][j].height = 0.0f;
					inform[i][j].increaser = creaser;
					inform[i][j].touch_ceil = false;
				}
			}
		}
		else {
			for (int i = 0; i < info_z; i++) {
				creaser += 0.01f;
				for (int j = 0; j < info_x; j++) {
					inform[j][i].height = 0.0f;
					inform[j][i].increaser = creaser;
					inform[j][i].touch_ceil = false;
				}
			}
		}

	}
	if (key == '3') {
		first_case = false;
		second_case = false;
		third_case = true;
		forth_case = false;
		for (int i = 0; i < info_x; i++) {
			for (int j = 0; j < info_z; j++) {
				inform[i][j].height = 0.0f;
				inform[i][j].increaser = 0.0f;
				inform[i][j].touch_ceil = false;
			}
		}
		if (info_x > info_z) {
			int midRow = info_x;
			int midCol = info_z;
			midRow /= 2;
			midCol /= 2;
			float creaser = 0.0001f;
			float dec = 0.0001f;
			for (int i = 0; i < info_x; i++) {
				creaser -= 0.00001f;
				dec += 0.00001f;
				for (int j = 0; j < info_z; j++) {
					if (i <= midRow) {
						creaser += 0.00001f;
						if (j >= midCol - i && j <= midCol + i) {
							inform[i][j].increaser = 0.001f + creaser;
						}
						else {
							dec += 0.00001f;
							inform[i][j].increaser = 0.001f - dec;
						}
					}
					else {
						creaser += 0.00001f;
						if (j >= i - midCol && j <= 3 * midCol - i) {
							inform[i][j].increaser = 0.001f + creaser;
						}
						else {
							dec += 0.00001f;
							inform[i][j].increaser = 0.001f - dec;
						}
					}
				}
			}
		}
		else {
			int midRow = info_z;
			int midCol = info_x;
			midRow /= 2;
			midCol /= 2;
			float creaser = 0.0001f;
			float dec = 0.0001f;
			for (int i = 0; i < info_z; i++) {
				creaser -= 0.00001f;
				dec += 0.00001f;
				for (int j = 0; j < info_x; j++) {
					if (i <= midRow) {
						creaser += 0.00001f;
						if (j >= midCol - i && j <= midCol + i) {
							inform[j][i].increaser = 0.001f + creaser;
						}
						else {
							dec += 0.00001f;
							inform[j][i].increaser = 0.001f - dec;
						}
					}
					else {
						creaser += 0.00001f;
						if (j >= i - midCol && j <= 3 * midCol - i) {
							inform[j][i].increaser = 0.001f + creaser;
						}
						else {
							dec += 0.00001f;
							inform[j][i].increaser = 0.001f - dec;
						}
					}
				}
			}
		}
	}
	if (key == '4') {
		first_case = false;
		second_case = false;
		third_case = false;
		forth_case = true;
		float creaser = 0.0f;
		for (int i = 0; i < info_z/3; i++) {
			for (int j = 0; j < info_x/4; j++) {
				creaser += 0.001f;
				inform[j][i].height = 0.0f;
				inform[j][i].increaser = creaser;
				inform[j][i].touch_ceil = false;
			}
		}
		for (int i = info_z / 3; i < 2 * info_z / 3; i++) {
			for (int j = 0; j < info_x / 4; j++) {
				creaser += 0.001f;
				inform[j][i].height = 0.0f;
				inform[j][i].increaser = creaser;
				inform[j][i].touch_ceil = false;
			}
		}
	}
	if (key == 'r') {
		first_case = false;
		second_case = false;
		third_case = false;
		int new_x, new_z;
		char buffer[100];
		printf("Enter new X (1-100): ");
		if (scanf("%d", &new_x) != 1 || new_x < 1 || new_x > 100) {
			printf("Invalid input for X. Setting X to 2.\n");
			new_x = 2;
		}
		fgets(buffer, sizeof(buffer), stdin);
		printf("Enter new Y (1-100): ");
		if (scanf("%d", &new_z) != 1 || new_z < 1 || new_z > 100) {
			printf("Invalid input for Y. Setting Y to 2.\n");
			new_z = 2;
		}
		fgets(buffer, sizeof(buffer), stdin);
		printf("new\tX: %d, Y: %d\n", new_x, new_z);
		remake_box(new_x + 1, new_z + 1);
		printf("\n\n");
		menu();

	}
	if (key == 'c') {
		light_red = static_cast<float>(rand()) / RAND_MAX;
		light_green = static_cast<float>(rand()) / RAND_MAX;
		light_blue = static_cast<float>(rand()) / RAND_MAX;
	}
	if (key == 't') {
		if (light_on == 0) {
			light_on = 1;
		}
		else {
			light_on = 0;
		}
	}
	if (key == '-' || key == '_') {
		if (speed > 1) {
			speed -= 1;
			printf("now speed: %d\n", speed);
		}
		else {
			printf("max speed\n");
		}

	}
	if (key == '+' || key == '=') {
		speed += 1;
		printf("now speed: %d\n", speed);
	}
}
void remake_box(int dx, int dz) {
	for (int i = 0; i < x_line; i++) {
		if (box[i] != NULL) {
			free(box[i]);
			box[i] = NULL;
		}
	}
	if (box != NULL) {
		free(box);
		box = NULL;
	}
	for (int i = 0; i < info_x; i++) {
		if (inform[i] != NULL) {
			free(inform[i]);
			inform[i] = NULL;
		}
	}
	if (inform != NULL) {
		free(inform);
		inform = NULL;
	}
	x_line = dx;
	z_line = dz;
	info_x = x_line - 1;
	info_z = z_line - 1;
	inform = (info**)malloc(info_x * sizeof(info*));
	for (int i = 0; i < info_x; i++) {
		inform[i] = (info*)malloc(info_z * sizeof(info));
	}

	box = (coord**)malloc(x_line * sizeof(coord*));
	for (int i = 0; i < x_line; i++) {
		box[i] = (coord*)malloc(z_line * sizeof(coord));
	}
	float tx = (ground[2].x - ground[0].x) / (x_line - 1);
	float tz = (ground[2].z - ground[0].z) / (z_line - 1);
	for (int i = 0; i < x_line; i++) {
		for (int j = 0; j < z_line; j++) {
			box[i][j].x = ground[0].x + tx * (i);
			box[i][j].y = ground[0].y;
			box[i][j].z = ground[0].z + tz * (j);
		}
	}
	for (int i = 0; i < info_x; i++) {
		for (int j = 0; j < info_z; j++) {
			inform[i][j].height = 0.0f;
			inform[i][j].increaser = 0.0f;
			inform[i][j].touch_ceil = false;
		}
	}
}
void viewproj() {
	GLuint projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	GLuint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");

	glm::vec3 camPos = glm::vec3(x_cam_move, -cam_height - y_cam_move, z_cam_move);
	glm::vec3 camTag = glm::vec3(0, 0, 0);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view = glm::lookAt(camPos, camTag, camUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 400.0f);
	projection = glm::translate(projection, glm::vec3(0.0, -1.0, -100.0));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

}
void second_viewproj() {
	GLuint projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
	GLuint viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");

	glm::vec3 camPos = glm::vec3(0, 30, 1);
	glm::vec3 camTag = glm::vec3(0, 0, 0);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view = glm::lookAt(camPos, camTag, camUp);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 400.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -100.0));
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
void draw_b(coord* box) {
	GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glBindBuffer(GL_ARRAY_BUFFER, box_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * sizeof(float), box, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glDrawElements(GL_POLYGON, 36, GL_UNSIGNED_INT, 0);
}
void draw_box() {
	coord new_box[8];
	for (int i = 0; i < x_line - 1; i++) {
		for (int j = 0; j < z_line - 1; j++) {
			new_box[0].x = box[i][j].x;
			new_box[0].y = box[i][j].y;
			new_box[0].z = box[i][j].z;

			new_box[1].x = box[i + 1][j].x;
			new_box[1].y = box[i + 1][j].y;
			new_box[1].z = box[i + 1][j].z;

			new_box[2].x = box[i + 1][j + 1].x;
			new_box[2].y = box[i + 1][j + 1].y;
			new_box[2].z = box[i + 1][j + 1].z;

			new_box[3].x = box[i][j + 1].x;
			new_box[3].y = box[i][j + 1].y;
			new_box[3].z = box[i][j + 1].z;

			new_box[4].x = box[i][j].x;
			new_box[4].y = box[i][j].y + inform[i][j].height;
			new_box[4].z = box[i][j].z;

			new_box[5].x = box[i + 1][j].x;
			new_box[5].y = box[i + 1][j].y + inform[i][j].height;
			new_box[5].z = box[i + 1][j].z;

			new_box[6].x = box[i + 1][j + 1].x;
			new_box[6].y = box[i + 1][j + 1].y + inform[i][j].height;
			new_box[6].z = box[i + 1][j + 1].z;

			new_box[7].x = box[i][j + 1].x;
			new_box[7].y = box[i][j + 1].y + inform[i][j].height;
			new_box[7].z = box[i][j + 1].z;

			for (int n = 0; n < 4; n++) {
				new_box[n].r = 0.7f;
				new_box[n].g = 0.9f;
				new_box[n].b = 0.3f;
			}
			for (int n = 4; n < 8; n++) {
				new_box[n].r = 0.7f;
				new_box[n].g = 0.3f;
				new_box[n].b = 0.3f;
			}
			draw_b(new_box);
		}
	}
}
void first_view() {
	lightOption();
	glViewport(0, 0, WINDOWS_HEIGHT, WINDOWS_WIDTH);
	viewproj();
	draw_box();
}
void second_view() {
	glViewport(0, WINDOWS_WIDTH - 100, 100, 100);
	second_viewproj();
	draw_box();
}
GLvoid drawScene() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	lightOption();
	first_view();
	second_view();
	glutSwapBuffers();
}
void make_ground() {
	ground[0].x = -30.f;
	ground[0].y = 0.f;
	ground[0].z = -30.f;

	ground[1].x = -30.f;
	ground[1].y = 0.f;
	ground[1].z = 30.f;

	ground[2].x = 30.f;
	ground[2].y = 0.f;
	ground[2].z = 30.f;

	ground[3].x = 30.f;
	ground[3].y = 0.f;
	ground[3].z = -30.f;

	for (int i = 0; i < 4; i++) {
		ground[i].r = static_cast<float>(rand()) / RAND_MAX;
		ground[i].g = static_cast<float>(rand()) / RAND_MAX;
		ground[i].b = static_cast<float>(rand()) / RAND_MAX;
	}
}
void makeshape() {
	make_ground();
}
void initbuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &box_vbo);
	glGenBuffers(1, &ground_vbo);
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