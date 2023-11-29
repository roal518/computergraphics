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

#define WINDOWS_HEIGHT	800
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
	coord* cp;
	coord* ep;
	int vertex_counter = 0;//vertex
	float index = 0;
	int mouse_node = 0;//0이면 폴리곤 노드,1이면 마우스 노드,3이면 삭제대상인 노드 
	struct Node* next;//// 단일 연결 리스트로 구성
}Node;
Node* createnode(coord* data, coord* cp, coord* ep, Node* next, int i, int m) {
	Node* newnode;
	newnode = (Node*)malloc(sizeof(Node));
	newnode->vertex = data;
	newnode->cp = cp;
	newnode->ep = ep;//솔직히 이거 하나만 줄여도 메모리 반으로 절약한다.. 방법 찾기
	newnode->vertex_counter = i;
	newnode->mouse_node = m;
	newnode->index = 0;
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
GLuint vao;
GLuint vbo;
GLuint mouse_vbo;
Node* main_list = NULL;
GLchar* vertexSource, * fragmentSource, * vertexSource_for_mouse;
GLuint vertexshader, fragmentshader, vertex_mouse_shader;
GLuint shaderProgramID, shaderProgramID_m;
void timers();
GLvoid drawScene();
void initBuffer();
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
void mouse_node_create();
int create_speed = 1000;
float move_speed = 1;
int max_polygon_shape = 3;
bool is_clicked = false;
float get_delta_X;
float get_delta_Y;
bool draw_mode = false;
void main(int argc, char** argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF || _CRTDBG_LEAK_CHECK_DF);
	mouse_node_create();
	srand(unsigned(time(NULL)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOWS_HEIGHT, WINDOWS_WIDTH);
	glutCreateWindow("game");
	glewExperimental = GL_TRUE;
	glewInit();
	initBuffer();
	make_shaderProgram();
	timers();
	glutMouseFunc(Mouseclick);
	glutMotionFunc(Mousemove);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}
void timers() {
	glutTimerFunc(create_speed, auto_polygon, 0);//폴리곤 생성 일단 disable?
	glutTimerFunc(30, move_polygon, 0);
	glutTimerFunc(30, Delete, 0);
	glutPostRedisplay();
}
void mouse_node_create() {//마우스 노드에서 정점을 구하고 
	int get_vertex = 2;
	coord* vertex = NULL;
	coord* cp = NULL;
	coord* ep = NULL;
	vertex = (coord*)malloc(get_vertex * sizeof(coord));
	for (int j = 0; j < get_vertex; j++) {
		vertex[j].r = 0;
		vertex[j].g = 1;
		vertex[j].b = 0;
		vertex[j].x = 0;
		vertex[j].y = 0;
		vertex[j].z = 0;
	}
	insert(&main_list, createnode(vertex, cp, ep, NULL, get_vertex, 1));
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
		if (move_speed > 1) {
			move_speed -= 1;
		}
		printf("now_move_speed: %f\n", move_speed);
	}
	if (key == '=' || key == '+') {
		move_speed += 1;
		printf("now_move_speed: %f\n", move_speed);
	}
	if (key == ',' || key == '<') {
		if (max_polygon_shape > 4) {
			max_polygon_shape--;
		}
		else {
			max_polygon_shape = 3;
		}
		printf("shape : 3~%d\n", max_polygon_shape + 2);
	}
	if (key == '.' || key == '>') {
		max_polygon_shape++;
		printf("shape : 3~%d\n", max_polygon_shape + 2);
	}
	if (key == '1') {
		if (!draw_mode) {
			draw_mode = true;
		}
		else {
			draw_mode = false;
		}
	}
}/////
void L_node_sep(int v_counter, coord* ic, Node* target, int* line_n) {
	int temp_n[2];
	temp_n[0] = line_n[0];
	temp_n[1] = line_n[1];
	// line_n의 데이터를 그대로 가져가서 쓰면 R_node_sep에서 훼손이 일어난다. 변수를 새로 받아서 오류를 방지한다.
	coord* vertex = NULL;
	coord* cp = NULL;
	coord* ep = NULL;
	float end_point_X = ((float)rand() / RAND_MAX) * 0.8 - 0.4;
	vertex = (coord*)malloc(v_counter * sizeof(coord));	
	cp = (coord*)malloc(v_counter * sizeof(coord));
	ep = (coord*)malloc(v_counter * sizeof(coord));
	//LINE_N1+1 이 LINE_N2-1와 같아질 때까지 진행한다.
	int set = 1;
	vertex[0] = ic[0];//시작점
	cp[0] = ic[0];
	while (temp_n[0] != temp_n[1]) {
		temp_n[0]++;
		vertex[set] = target->vertex[temp_n[0]];
		cp[set]= target->vertex[temp_n[0]];
		set++;
	}
	vertex[v_counter - 1]= ic[1];	//끝점
	cp[v_counter - 1] = ic[1];
	for (int j = 0; j < v_counter; j++) {
		vertex[j].r = static_cast<float>(rand()) / RAND_MAX;
		vertex[j].g = static_cast<float>(rand()) / RAND_MAX;
		vertex[j].b = static_cast<float>(rand()) / RAND_MAX;
		vertex[j].z = 0;
		ep[j].x = end_point_X + cp[j].x;
		ep[j].y = -1.1 + cp[j].y;
		ep[j].z = 0;
	}
	insert(&main_list, createnode(vertex, cp, ep, NULL,v_counter, 0));
}
void R_node_sep(int v_counter, coord* ic, Node* target, int* line_n) {
	int temp_n[2];
	temp_n[0] = line_n[0];
	temp_n[1] = line_n[1];
	coord* vertex = NULL;
	coord* cp = NULL;
	coord* ep = NULL;
	float end_point_X = ((float)rand() / RAND_MAX) * 0.8 - 0.4;
	vertex = (coord*)malloc(v_counter * sizeof(coord));
	cp = (coord*)malloc(v_counter * sizeof(coord));
	ep = (coord*)malloc(v_counter * sizeof(coord));
	//LINE_N1+1 이 LINE_N2-1와 같아질 때까지 진행한다.
	vertex[0] = ic[1];
	cp[0] = ic[1];
	int set = 1;
	while (temp_n[1] != target->vertex_counter-1) {
		temp_n[1]++;
		vertex[set] = target->vertex[temp_n[1]];
		cp[set] = target->vertex[temp_n[1]];
		set++;
	}
	temp_n[1] = 0;
	while (temp_n[1] != temp_n[0] + 1) {
		vertex[set] = target->vertex[temp_n[1]];
		cp[set] = target->vertex[temp_n[1]];
		temp_n[1]++;
		set++;
	}
	vertex[v_counter - 1] = ic[0];
	cp[v_counter - 1] = ic[0];
	for (int j = 0; j < v_counter; j++) {
		vertex[j].r = static_cast<float>(rand()) / RAND_MAX;
		vertex[j].g = static_cast<float>(rand()) / RAND_MAX;
		vertex[j].b = static_cast<float>(rand()) / RAND_MAX;
		vertex[j].z = 0;
		ep[j].x = end_point_X + cp[j].x;
		ep[j].y = -1.1 + cp[j].y;
		ep[j].z = 0;
	}
	insert(&main_list, createnode(vertex, cp, ep, NULL, v_counter, 0));
}
void real_slice(coord* ic, Node* target, int* line_n) {
	if (line_n[0] - line_n[1] > 0) {//swap을 해준다. 대충 유사정렬이다.좀 더 깔끔ㄴ하게 슬라이스 각을 보자
		int temp = line_n[1];
		line_n[1] = line_n[0];
		line_n[0] = temp;
		coord ic_temp = ic[0];
		ic[0] = ic[1];
		ic[1] = ic_temp;
	}
	int L_vertex = line_n[1] - line_n[0] + 2;
	int R_vertex = target->vertex_counter + 4 - L_vertex;
	//L/R 두개 모두 몇각형을 만드는지 알려줄수 있다.
	L_node_sep(L_vertex, ic, target, line_n);
	R_node_sep(R_vertex, ic, target, line_n);
}
void slice() {
	coord mouse[2];
	Node* horse = main_list;
	while (horse != NULL) {
		if (horse->mouse_node == 1) {
			mouse[0].x = horse->vertex[0].x;
			mouse[0].y = horse->vertex[0].y;
			mouse[1].x = horse->vertex[1].x;
			mouse[1].y = horse->vertex[1].y;
			horse->vertex[0].x = 0;
			horse->vertex[0].y = 0;
			horse->vertex[1].x = 0;
			horse->vertex[1].y = 0;
			break;
		}
		else {
			horse = horse->next;
		}
	}
	horse = main_list;
	while (horse != NULL) {
		int line_n[2];
		coord intersection[2];
		int count = 0;
		for (int i = 0; i < horse->vertex_counter; i++) {
			float t = ((mouse[0].x - horse->vertex[i].x) * (horse->vertex[i].y - horse->vertex[(i + 1) % horse->vertex_counter].y)
				- (mouse[0].y - horse->vertex[i].y) * (horse->vertex[i].x - horse->vertex[(i + 1) % horse->vertex_counter].x))
				/ ((mouse[0].x - mouse[1].x) * (horse->vertex[i].y - horse->vertex[(i + 1) % horse->vertex_counter].y)
					- (mouse[0].y - mouse[1].y) * (horse->vertex[i].x - horse->vertex[(i + 1) % horse->vertex_counter].x));

			float u = ((mouse[0].x - horse->vertex[i].x) * (mouse[0].y - mouse[1].y)
				- (mouse[0].y - horse->vertex[i].y) * (mouse[0].x - mouse[1].x))
				/ ((mouse[0].x - mouse[1].x) * (horse->vertex[i].y - horse->vertex[(i + 1) % horse->vertex_counter].y)
					- (mouse[0].y - mouse[1].y) * (horse->vertex[i].x - horse->vertex[(i + 1) % horse->vertex_counter].x));
			if ((0 <= t && t <= 1) && (0 <= u && u <= 1)) {
				if (count == 0) {
					line_n[0] = i;
					intersection[0].x = mouse[0].x + t * (mouse[1].x - mouse[0].x);
					intersection[0].y = mouse[0].y + t * (mouse[1].y - mouse[0].y);
				}
				else {
					line_n[1] = i;
					intersection[1].x = mouse[0].x + t * (mouse[1].x - mouse[0].x);
					intersection[1].y = mouse[0].y + t * (mouse[1].y - mouse[0].y);
				}
				count++;
			}
		}
		if (count == 2) {
			real_slice(intersection, horse, line_n);
			horse->mouse_node = 3;
		}
		horse = horse->next;
	}
}
void Mouseclick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Node* horse = main_list;
		while (horse != NULL) {
			if (horse->mouse_node == 1) {
				horse->vertex[0].x = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
				horse->vertex[0].y = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
				horse->vertex[1].x = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
				horse->vertex[1].y = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
				get_delta_X = x;
				get_delta_Y = y;
				is_clicked = true;
			}
			horse = horse->next;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		is_clicked = false;
		slice();
	}
	glutPostRedisplay();
}
void Mousemove(int x, int y) {
	if (is_clicked) {
		Node* horse = main_list;
		while (horse != NULL) {
			if (horse->mouse_node == 1) {
				horse->vertex[1].x += (x - get_delta_X) / 400.f;;
				horse->vertex[1].y -= (y - get_delta_Y) / 300.f;
			}
			horse = horse->next;
		}
		get_delta_X = x;
		get_delta_Y = y;
		glutPostRedisplay();
	}
}
void move_polygon(int val) {
	Node* horse = main_list;
	while (horse != NULL) {
		if (horse->mouse_node == 0) {
			float t = horse->index / 1000.f;
			for (int i = 0; i < horse->vertex_counter; i++) {
				horse->vertex[i].x = glm::pow((1 - t), 3) * horse->cp[i].x
					+ 3 * glm::pow(t, 1) * glm::pow((1 - t), 2) * (0.75 * horse->cp[i].x + 0.25 * horse->ep[i].x)
					+ 3 * glm::pow(t, 2) * glm::pow((1 - t), 1) * (-0.5 * horse->cp[i].x + 1.5 * horse->ep[i].x)
					+ glm::pow(t, 3) * horse->ep[i].x;
				horse->vertex[i].y = glm::pow((1 - t), 3) * horse->cp[i].y
					+ 3 * glm::pow(t, 1) * glm::pow((1 - t), 2) * (1.65 * horse->cp[i].y - 0.65 * horse->ep[i].y)
					+ 3 * glm::pow(t, 2) * glm::pow((1 - t), 1) * (0.25 * horse->cp[i].y + 0.75 * horse->ep[i].y)
					+ glm::pow(t, 3) * horse->ep[i].y;
			}

			horse->index += move_speed;
		}
		horse = horse->next;
	}
	glutPostRedisplay();
	glutTimerFunc(10, move_polygon, 0);
}
void auto_polygon(int val) {//폴리곤 자동생성
	int get_rand_vertex = rand() %  max_polygon_shape+ 3;// 3~6 각형만 생성하자
	float mid_point_X = (rand() % 2 == 0) ? -1.1 : 1.1;
	float mid_point_Y = static_cast<float>(rand()) / RAND_MAX;
	coord* vertex = NULL;
	coord* cp = NULL;
	coord* ep = NULL;
	float end_point_X = ((float)rand() / RAND_MAX) * 1.6 - 0.8;
	vertex = (coord*)malloc(get_rand_vertex * sizeof(coord));
	cp = (coord*)malloc(get_rand_vertex * sizeof(coord));
	ep = (coord*)malloc(get_rand_vertex * sizeof(coord));
	for (int j = 0; j < get_rand_vertex; j++) {
		vertex[j].r = static_cast<float>(rand()) / RAND_MAX;
		vertex[j].g = static_cast<float>(rand()) / RAND_MAX;
		vertex[j].b = static_cast<float>(rand()) / RAND_MAX;
		float angle = 2 * glm::pi<float>() * j / get_rand_vertex;
		vertex[j].x = mid_point_X + 0.175f * glm::cos(angle);
		vertex[j].y = mid_point_Y + 0.175f * glm::sin(angle);
		vertex[j].z = 0;
		cp[j].x = mid_point_X + 0.175f * glm::cos(angle);
		cp[j].y = mid_point_Y + 0.175f * glm::sin(angle);
		cp[j].z = 0;
	}
	for (int j = 0; j < get_rand_vertex; j++) {
		float angle = 2 * glm::pi<float>() * j / get_rand_vertex;
		ep[j].x = end_point_X + 0.175f * glm::cos(angle);
		ep[j].y = -1 + 0.175f * glm::sin(angle);
		ep[j].z = 0;
	}
	insert(&main_list, createnode(vertex, cp, ep, NULL, get_rand_vertex, 0));
	glutPostRedisplay();
	glutTimerFunc(create_speed, auto_polygon, 0);
}
void initBuffer() {
	glUseProgram(shaderProgramID);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &mouse_vbo);
}
void Delete(int val) {
	Node* current = main_list;
	Node* previous = NULL;
	int idx = 0;
	while (current != NULL) {
		if ((current->index > 1000.f && current->mouse_node == 0) || current->mouse_node == 3) {
			if (previous == NULL) {
				main_list = current->next;
			}
			else {
				previous->next = current->next;
			}
			Node* del = current;
			current = current->next;
			free(del->cp);//동적할당 배열 해제 해주는것 잊지않았다...
			free(del->ep);
			free(del->vertex);
			free(del);
		}
		else {
			previous = current;
			current = current->next;
		}
		idx++;
	}
	glutTimerFunc(30, Delete, 0);
}
void draw_poly(Node* list) {
	Node* horse = list;
	while (horse != NULL) {
		if (horse->mouse_node == 1) {
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			glBindBuffer(GL_ARRAY_BUFFER, mouse_vbo);
			glBufferData(GL_ARRAY_BUFFER, horse->vertex_counter * 6 * sizeof(float), horse->vertex, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glDrawArrays(GL_LINES, 0, 2);
		}
		else if (horse->mouse_node == 0) {
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			GLuint modelMatrixLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, horse->vertex_counter * 6 * sizeof(float), horse->vertex, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glDrawArrays(GL_POLYGON, 0, horse->vertex_counter);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		horse = horse->next;
	}
}
GLvoid drawScene() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!draw_mode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
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
	fseek(fptr, 0, SEEK_SET);
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