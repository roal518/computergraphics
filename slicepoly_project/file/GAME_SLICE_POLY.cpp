﻿#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
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
typedef struct Node {
	coord* vertex;//정점 정보 저장 근데 이거맞냐?
	int vertex_counter = 0;
	float theta = 0;//회전 각
	struct Node* next;//// 단일 연결 리스트로 구성
}Node;
Node* createnode(coord data[], Node* next, int i) {
	Node* newnode;
	newnode = (Node*)malloc(sizeof(Node));
	newnode->vertex = data;
	newnode->vertex_counter = i;
	newnode->theta = 0;
	newnode->next = NULL;
	return newnode;
	//정점 정보를 받는다 정점은
	//1. 랜덤정점-> 뭐라도 떨어져야 하니까
	//2. 슬라이스 결과 정점-> 이후 추가 슬라이스 가능
}
void insert(Node** head, Node* newnode) {
	if ((*head) == NULL) {
		(*head) = newnode;
	}
	else {
		Node* cur = (*head);
		while (cur->next != NULL) {
			cur = cur->next;
		}
		cur->next = newnode;
	}
}
void listmaker(Node** list) {
	for (int i = 0; i < 20; i++) {
		int get_rand_vertex = rand() % 4 + 3;// 3~6 각형만 생성하자
		float mid_point_X = (rand() % 2 == 0) ? -1.2 : 1.2;
		float mid_point_Y = 0.6 + ((float)rand() / RAND_MAX) * 0.4;
		
		coord* vertex = NULL;
		vertex = (coord*)malloc(get_rand_vertex * sizeof(coord));
		for (int j = 0; j < get_rand_vertex; j++) {
			vertex[j].r = static_cast<float>(rand()) / RAND_MAX;
			vertex[j].g = static_cast<float>(rand()) / RAND_MAX;
			vertex[j].b = static_cast<float>(rand()) / RAND_MAX;
			float angle = 2 * glm::pi<float>() * j / get_rand_vertex;
			vertex[j].x = mid_point_X + 0.125f * glm::cos(angle);
			vertex[j].y = mid_point_Y + 0.125f * glm::sin(angle);
			vertex[j].z = 0;
		}
		insert(list, createnode(vertex, NULL, get_rand_vertex));
	}
}
GLuint vbo;
GLuint vao;
Node* main_list = NULL;

GLchar* vertexSource, * fragmentSource;
GLuint vertexshader, fragmentshader;
GLuint shaderProgramID;
GLvoid drawScene();
void initBuffer();
void starter();// make list
void auto_polygon(int val);
void move_polygon(int val);
void Keyboard(unsigned char key, int x, int y);
void Mouseclick(int button, int state, int x, int y);
void Mousemove(int x, int y);
void Delete(int val);
GLvoid Reshape(int w, int h);
char* filetobuf(const char* file);
void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
int create_speed = 1000;
float move_speed = 1;
void printmenu() {

}
void main(int argc, char** argv) {
	printmenu();
	srand(unsigned(time(NULL)));
	starter();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("game");
	glewExperimental = GL_TRUE;
	glewInit();
	initBuffer();
	make_shaderProgram();
	glutTimerFunc(create_speed, auto_polygon, 0);//폴리곤 생성 일단 disable?
	glutTimerFunc(30, move_polygon, 0);
	glutTimerFunc(30, Delete, 0);
	glutMouseFunc(Mouseclick);
	glutMotionFunc(Mousemove);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
void Keyboard(unsigned char key, int x, int y) {
	if (key == '[') {
		if (create_speed > 10) {
			create_speed -= 10;
		}
		printf("now_create_speed: %d\n", create_speed);
	}
	if (key == ']') {
		create_speed += 10;
		printf("now_create_speed: %d\n", create_speed);
	}
	if (key == '-') {
		move_speed -= 0.5;
		printf("now_move_speed: %f\n", move_speed);
	}
	if (key == '=' || key == '+') {
		move_speed += 0.5;
		printf("now_move_speed: %f\n", move_speed);
	}
}
void Mouseclick(int button, int state, int x, int y) {

}
void Mousemove(int x, int y) {

}
void move_polygon(int val) {//생성된 폴리곤을 움직이게 만들것
	//control point 0 생성된 위치
	//control point 1 x=0.3 y는 0.4
	//control point 2 x=-0.3 y는 -0.4 
	Node* horse = main_list;
	while (horse != NULL) {
		horse->theta += move_speed;
		horse = horse->next;
	}
	glutPostRedisplay();
	glutTimerFunc(30, move_polygon, 0);
}

void auto_polygon(int val) {
	int get_rand_vertex = rand() % 4 + 3;// 3~6 각형만 생성하자
	float mid_point_X = (rand() % 2 == 0) ? -1.2 : 1.2;
	float mid_point_Y = 0.6 + ((float)rand() / RAND_MAX) * 0.5;


	coord* vertex = NULL;
	vertex = (coord*)malloc(get_rand_vertex * sizeof(coord));

	for (int j = 0; j < get_rand_vertex; j++) {
		vertex[j].r = static_cast<float>(rand()) / RAND_MAX;
		vertex[j].g = static_cast<float>(rand()) / RAND_MAX;
		vertex[j].b = static_cast<float>(rand()) / RAND_MAX;
		float angle = 2 * glm::pi<float>() * j / get_rand_vertex;
		vertex[j].x = mid_point_X + 0.175f * glm::cos(angle);
		vertex[j].y = mid_point_Y + 0.175f * glm::sin(angle);
		vertex[j].z = 0;
	}
	insert(&main_list, createnode(vertex, NULL, get_rand_vertex));
	glutPostRedisplay();
	glutTimerFunc(create_speed, auto_polygon, 0); // good~성능 미쳤따~ 일단은 쓰지말고 폴리곤 자르기부터 시작

}
void starter() {
	listmaker(&main_list);
}
void initBuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
}
void Delete(int val) {
	Node* current = main_list;
	Node* previous = NULL;
	int idx = 0;
	while (current != NULL) {
		if (current->theta > 180.f) {
			if (previous == NULL) {
				// If the first node meets the condition, update the head
				main_list = current->next;
			}
			else {
				// For other nodes, update the 'next' pointer of the previous node
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
		idx++;
	}
	glutTimerFunc(30, Delete, 0);
} //삭제 조건 완성
void draw_poly(Node*list) {
	Node* horse = list;
	while (horse != NULL) {//리스트를 순회하면서 polygon draw
		//슬슬 불안한데?? ㅠㅠ
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		if (horse->vertex[0].x > 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.65, -1.25, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(horse->theta), glm::vec3(0.0f, 0.0f, 1.0f));//회전
		}
		else if (horse->vertex[0].x < 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.65, -1.25, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(-horse->theta), glm::vec3(0.0f, 0.0f, 1.0f));//회전
		}
		//theta가 일정 값 이상이면 즉시 삭제하자!
		GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, horse->vertex_counter * 6 * sizeof(float), horse->vertex, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_POLYGON, 0,horse->vertex_counter);
		//glDrawArrays(GL_LINE_STRIP, 0, horse->vertex_counter);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		horse = horse->next;
	}
}
GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_poly(main_list);
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
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






// 노드 운영 중점
// 개 복잡한데??
// 하나로는 못쓰나
// 
// 노드에 뭘 담을것인가-> 1 삼각형 정점 정보 
// 삭제가 매우 중요해졌다. -> 이미 밑으로 떨어진 폴리곤은 데이터 낭비가 된다.
// 그럼 어쩌라고
// 삼각형만 먼저 만들어본다.
//2D 필요한거
// 정점 회전/이동 끝?
// 물체 이동 x축 시야 밖 생성
// 이동은? -1.0~1.0 사이에 도착
// 슬라이스
// 정점 레이저 검사 하면 교점 위치 나옴->그걸로 슬라이스 하면 되지않나
// 
//10-25 일지
//동적할당을 썼다.
//삭제,생성을 고쳤음
//리스트 하나로 여러개의 정점 얻을수 있다.
// 
//중요 1.
// 이제 그리기
// 자르기 
// 마지막 이동 
//  
// //