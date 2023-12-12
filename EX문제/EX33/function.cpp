#define _CRT_SECURE_NO_WARNINGS
#include "function.h"
#include "header.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

/***********************콜백 함수**********************/
GLvoid drawScene()
{
	/*바탕 화면*/
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/******************************************************************/
	/*셰이더 프로그램 사용*/
	glUseProgram(shaderID);
	/*그리기*/
	Draw();
	/******************************************************************/
	glutSwapBuffers(); // 화면에 출력하기
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}
GLvoid Keyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'q':
		exit(1);
		break;
	}
}
GLvoid TimerFunction(int value)
{
	/*변화해야 할 부분*/
	/*조명 회전*/
	if (lightRoate == true) {
		lightRadians = lightVal / 360.0f * 2.0f * 3.141592f;
		lightVal += 2.0f;
		if (lightVal >= 360.0f) {
			lightVal = 0.0f;
		}
	}
	unsigned int lightPowerLocation = glGetUniformLocation(shaderID, "lightPower");
	glUniform1f(lightPowerLocation, lightPower);

	if (objectRotateX == true) {
		rotateX += 5.0f;
	}

	if (objectRotateY == true) {
		rotateY += 5.0f;
	}
	/*Update와 Draw호출*/
	glutPostRedisplay();
	/*타이머 무한 반복*/
	glutTimerFunc(50, TimerFunction, 1);
}
/**********************초기화 함수*********************/
void InitObject()
{
	for (int i = 0; i < 2; i++) {
		robot[i].SetAlive(true);
	}
	for (int i = 0; i < 15; i++) {
		plain[i].SetAlive(true);
	}
	for (int i = 0; i < 3; i++) {
		cube[i].SetAlive(true);
		obstacle[i].SetAlive(true);
	}
	/*타일*/
	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 5; j++) {
			plain[(i * 5) + j].SetColor(1.0f, 1.0f, 1.0f);
			plain[(i * 5) + j].SetScale(2.0f, 2.0f, 2.0f);
			plain[(i * 5) + j].SetPosition(-4.0f + 2.0f * j, 0.5f, -2.0f + 2.0f * i);
		}
	}
	/*정육면체*/
	cube[0].SetColor(1.0f, 1.0f, 1.0f);
	cube[0].SetScale(1.0f, 0.5f, 1.0f);
	cube[0].SetPosition(-3.0f, 0.25f, 2.0f);

	cube[1].SetColor(1.0f, 1.0f, 1.0f);
	cube[1].SetScale(1.0f, 0.5f, 1.0f);
	cube[1].SetPosition(-2.0f, 0.25f, 2.0f);

	cube[2].SetColor(1.0f, 1.0f, 1.0f);
	cube[2].SetScale(1.0f, 0.5f, 1.0f);
	cube[2].SetPosition(-1.0f, 0.25f, 2.0f);
	/*장애물*/
	obstacle[0].SetColor(1.0f, 0.0f, 0.0f);
	obstacle[0].SetScale(1.0f, 3.0f, 1.0f);
	obstacle[0].SetPosition(-2.0f, 1.5f, -1.0f);
	obstacle[1].SetColor(1.0f, 0.0f, 0.0f);
	obstacle[1].SetScale(1.0f, 3.0f, 1.0f);
	obstacle[1].SetPosition(0.0f, 1.5f, -1.0f);
	obstacle[2].SetColor(1.0f, 0.0f, 0.0f);
	obstacle[2].SetScale(3.0f, 1.0f, 1.0f);
	obstacle[2].SetPosition(-1.0f, 3.0f, -1.0f);
	/*로봇*/
	robot[0].SetColor(0.0f, 1.0f, 0.0f);
	robot[0].SetScale(0.5f, 0.5f, 0.5f);
	robot[0].SetPosition(0.0f, 1.5f, 0.0f);
	robot[1].SetColor(0.0f, 0.0f, 1.0f);
	robot[1].SetScale(1.0f, 3.0f, 1.0f);
	robot[1].SetPosition(0.0f, 0.0f, 0.0f);
}
void InitBuffer() {
	for (int i = 0; i < 2; i++) {
		robot[i].InitBuffer();
	}
	/*큐브*/
	for (int i = 0; i < 3; i++) {
		cube[i].InitBuffer();
		obstacle[i].InitBuffer();
	}
	/*타일1*/
	for (int i = 0; i < 15; i++) {
		plain[i].InitBuffer();
	}
}
/*********************화면 출력 함수*******************/
void Draw()
{
	/*카메라 변환*/
	CameraTransform();
	/*조명 변환*/
	LightTransform();
	/*타일1*/
	for (int i = 0; i < 15; i++) {
		plain[i].Draw();
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < 3; i++) {
		cube[i].Draw();
		obstacle[i].Draw();
	}
	for (int i = 0; i < 2; i++) {
		robot[i].Draw2();
	}
	glDisable(GL_BLEND);
}
/*******************카메라 변환 함수*******************/
void CameraTransform()
{
	/*카메라 설정*/
	glm::vec3 cameraPos = glm::vec3(0.0f + cameraX, cameraY, 4.5f + cameraZ); //--- 카메라 위치를 이 좌표에 고정
	glm::vec3 cameraDirection = glm::vec3(0.0f + cameraX, 0.0f, 0.0f + cameraZ); //--- 카메라 바라보는 방향. 계속 이 방향만 바라본다.
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- 카메라 위쪽 방향

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	view = glm::rotate(view, glm::radians(cameraRaidans), glm::vec3(0.0f, 1.0f, 0.0f));
	if (cameraRotate == true) {
		view = glm::translate(view, glm::vec3(-cameraX, 0.0f, -cameraZ));
	}
	unsigned int viewLocation = glGetUniformLocation(shaderID, "view"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	//원근투영
	unsigned int projLoc = glGetUniformLocation(shaderID, "projection");
	glm::mat4 pTransform = glm::mat4(1.0f);

	//종횡비 1:1로 설정, 수직 시야각 60', z축으로 5.0만큼 늘렸다.
	pTransform = glm::perspective(glm::radians(60.0f), (float)800 / (float)800, 0.1f, 500.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &pTransform[0][0]);
}
/********************조명 변환 함수********************/
void LightTransform()
{
	/*조명 좌표*/
	lightX = (2.0f + lightRadiusZ) * cos(lightRadians);
	lightZ = (2.0f + lightRadiusZ) * sin(lightRadians);
	/*셰이더: 조명 좌표 넣기*/
	unsigned int lightPosLocation = glGetUniformLocation(shaderID, "lightPos");
	glUniform3f(lightPosLocation, lightX, lightY, lightZ);
	/*앰비언트 색*/
	unsigned int lightColorLocation = glGetUniformLocation(shaderID, "lightColor");
	glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);

	/*카메라 좌표 전달*/
	unsigned int viewPosLocation = glGetUniformLocation(shaderID, "viewPos");
	glUniform3f(viewPosLocation, 0.0f, 3.0f, 5.0f);
}
/*****************Class::LightCube 함수****************/
void LightCube::InitBuffer()
{
	GLint pAttribute = glGetAttribLocation(shaderID, "vPos");
	GLint nAttribute = glGetAttribLocation(shaderID, "vNormal");

	/*create buffer*/
	glGenVertexArrays(1, &_vao);
	glGenBuffers(2, _vbo);

	/*vao binding*/
	glBindVertexArray(_vao);

	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticles), cubeVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(1);

	glEnableVertexAttribArray(pAttribute);
	glEnableVertexAttribArray(nAttribute);
}
void LightCube::Draw()
{
	if (_Alive == true)
	{
		glBindVertexArray(_vao);
		/*도형의 색*/
		int objColorLocation = glGetUniformLocation(shaderID, "objectColor");
		glUniform3f(objColorLocation, 0.5f, 0.5f, 0.5f);

		GLint pAttribute = glGetAttribLocation(shaderID, "vPos");
		/*초기화 후 변환 행렬 채우기*/
		Transform();
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}
void LightCube::SetAlive(bool alive)
{
	if (alive == true)
		_Alive = true;
	else
		_Alive = false;

}
void LightCube::Transform()
{
	unsigned int transformLocate = glGetUniformLocation(shaderID, "model");
	/*리셋 한번 해주고*/
	_mixMat = glm::mat4{ 1.0f };
	_mixMat = glm::translate(_mixMat, glm::vec3(0.0f + lightX, 0.0f + lightY, 0.0f + lightZ));
	_mixMat = glm::rotate(_mixMat, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	_mixMat = glm::rotate(_mixMat, glm::radians(30.0f + _rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
	_mixMat = glm::scale(_mixMat, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(transformLocate, 1, GL_FALSE, glm::value_ptr(_mixMat));
}
void LightCube::Rotating()
{
	_rotateY += 10.0f;
	if (_rotateY >= 360.0f) {
		_rotateY = 0.0f;
	}
}
/*******************Class::Cube 함수*******************/
void Cube::InitBuffer()
{
	GLint pAttribute = glGetAttribLocation(shaderID, "vPos");
	GLint nAttribute = glGetAttribLocation(shaderID, "vNormal");
	GLint tAttribute = glGetAttribLocation(shaderID, "vTexCoord");
	/*****1*****/
	/*create buffer*/
	glGenVertexArrays(6, _vao);
	glGenBuffers(6, _vbo);
	/*vao binding*/
	glBindVertexArray(_vao[0]);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticles), cubeVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);

	/*****2*****/
	/*vao binding*/
	glBindVertexArray(_vao[1]);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticles), cubeVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);

	/*****3*****/
	/*vao binding*/
	glBindVertexArray(_vao[2]);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticles), cubeVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);

	/*****4*****/
	/*vao binding*/
	glBindVertexArray(_vao[3]);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticles), cubeVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);

	/*****5*****/
	/*vao binding*/
	glBindVertexArray(_vao[4]);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticles), cubeVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);

	/*****6*****/
	/*vao binding*/
	glBindVertexArray(_vao[5]);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticles), cubeVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);
}
void Cube::Draw()
{
	if (_Alive == true)
	{
		/*도형의 색*/
		int objColorLocation = glGetUniformLocation(shaderID, "objectColor");
		glUniform4f(objColorLocation, _colorR, _colorG, _colorB, 0.5f);

		/*텍스쳐1*/
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBindVertexArray(_vao[0]);
		/*초기화 후 변환 행렬 채우기*/
		Transform();
		glDrawArrays(GL_TRIANGLES, 0, 6); //뒤

		/*텍스쳐2*/
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBindVertexArray(_vao[1]);
		/*초기화 후 변환 행렬 채우기*/
		Transform();
		glDrawArrays(GL_TRIANGLES, 6, 6); //앞

		/*텍스쳐3*/
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glBindVertexArray(_vao[2]);
		/*초기화 후 변환 행렬 채우기*/
		Transform();
		glDrawArrays(GL_TRIANGLES, 12, 6); //왼

		/*텍스쳐4*/
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBindVertexArray(_vao[3]);
		/*초기화 후 변환 행렬 채우기*/
		Transform();
		glDrawArrays(GL_TRIANGLES, 18, 6); //오

		/*텍스쳐5*/
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glBindVertexArray(_vao[4]);
		/*초기화 후 변환 행렬 채우기*/
		Transform();
		glDrawArrays(GL_TRIANGLES, 24, 6); //아래

		/*텍스쳐6*/
		glBindTexture(GL_TEXTURE_2D, texture[4]); //위
		glBindVertexArray(_vao[5]);


		/*초기화 후 변환 행렬 채우기*/
		Transform();
		glDrawArrays(GL_TRIANGLES, 30, 6);
	}
}
void Cube::Draw2()
{
	/*도형의 색*/
	int objColorLocation = glGetUniformLocation(shaderID, "objectColor");
	glUniform4f(objColorLocation, _colorR, _colorG, _colorB, 1.0f);

	/*텍스쳐1*/
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBindVertexArray(_vao[0]);
	/*초기화 후 변환 행렬 채우기*/
	Transform();
	glDrawArrays(GL_TRIANGLES, 0, 6); //뒤

	/*텍스쳐2*/
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindVertexArray(_vao[1]);
	/*초기화 후 변환 행렬 채우기*/
	Transform();
	glDrawArrays(GL_TRIANGLES, 6, 6); //앞

	/*텍스쳐3*/
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBindVertexArray(_vao[2]);
	/*초기화 후 변환 행렬 채우기*/
	Transform();
	glDrawArrays(GL_TRIANGLES, 12, 6); //왼

	/*텍스쳐4*/
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBindVertexArray(_vao[3]);
	/*초기화 후 변환 행렬 채우기*/
	Transform();
	glDrawArrays(GL_TRIANGLES, 18, 6); //오

	/*텍스쳐5*/
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBindVertexArray(_vao[4]);
	/*초기화 후 변환 행렬 채우기*/
	Transform();
	glDrawArrays(GL_TRIANGLES, 24, 6); //아래

	/*텍스쳐6*/
	glBindTexture(GL_TEXTURE_2D, texture[4]); //위
	glBindVertexArray(_vao[5]);


	/*초기화 후 변환 행렬 채우기*/
	Transform();
	glDrawArrays(GL_TRIANGLES, 30, 6);

}
void Cube::SetAlive(bool alive)
{
	if (alive == true)
		_Alive = true;
	else
		_Alive = false;

}
void Cube::SetColor(float r, float g, float b)
{
	_colorR = r;
	_colorG = g;
	_colorB = b;
}
void Cube::SetPosition(float x, float y, float z)
{
	_positionX = x;
	_positionY = y;
	_positionZ = z;
}
void Cube::SetScale(float x, float y, float z)
{
	_scaleX = x;
	_scaleY = y;
	_scaleZ = z;
}
void Cube::Transform()
{
	unsigned int transformLocate = glGetUniformLocation(shaderID, "model");
	/*리셋 한번 해주고*/
	_mixMat = glm::mat4{ 1.0f };
	_mixMat = glm::translate(_mixMat, glm::vec3(_positionX + _moveX, _positionY, _positionZ));
	_mixMat = glm::rotate(_mixMat, glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
	_mixMat = glm::rotate(_mixMat, glm::radians(rotateX), glm::vec3(1.0f, 0.0f, 0.0f));
	_mixMat = glm::scale(_mixMat, glm::vec3(_scaleX, _scaleY, _scaleZ));
	glUniformMatrix4fv(transformLocate, 1, GL_FALSE, glm::value_ptr(_mixMat));
}
void Cube::Move()
{
	if (_moveArrow == 0) {
		_moveX += 0.01f;
	}
	else if (_moveArrow == 1) {
		_moveX -= 0.01f;
	}

	if (_moveX >= 1.0f) {
		_moveArrow = 1;
	}
	else if (_moveX <= -1.0f) {
		_moveArrow = 0;
	}
}
/*****************Class::Pyramid 함수******************/
void Pyramid::InitBuffer()
{
	GLint pAttribute = glGetAttribLocation(shaderID, "vPos");
	GLint nAttribute = glGetAttribLocation(shaderID, "vNormal");
	GLint tAttribute = glGetAttribLocation(shaderID, "vTexCoord");

	/*****1*****/
	/*create buffer*/
	glGenVertexArrays(5, _vao);
	glGenBuffers(5, _vbo);
	/*vao binding*/
	glBindVertexArray(_vao[0]);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVerticles), pyramidVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);

	/*****2*****/
	/*vao binding*/
	glBindVertexArray(_vao[1]);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVerticles), pyramidVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);

	/*****3*****/
	/*vao binding*/
	glBindVertexArray(_vao[2]);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVerticles), pyramidVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);

	/*****4*****/
	/*vao binding*/
	glBindVertexArray(_vao[3]);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVerticles), pyramidVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);

	/*****5*****/
	/*vao binding*/
	glBindVertexArray(_vao[4]);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVerticles), pyramidVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);
}
void Pyramid::Draw()
{
	if (_Alive == true)
	{
		/*텍스쳐1*/
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBindVertexArray(_vao[0]);
		/*초기화 후 변환 행렬 채우기*/
		Transform();
		glDrawArrays(GL_TRIANGLES, 0, 3); //뒤

		/*텍스쳐2*/
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBindVertexArray(_vao[1]);
		/*초기화 후 변환 행렬 채우기*/
		Transform();
		glDrawArrays(GL_TRIANGLES, 3, 3); //앞

		/*텍스쳐3*/
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glBindVertexArray(_vao[2]);
		/*초기화 후 변환 행렬 채우기*/
		Transform();
		glDrawArrays(GL_TRIANGLES, 6, 3); //왼

		/*텍스쳐4*/
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glBindVertexArray(_vao[3]);
		/*초기화 후 변환 행렬 채우기*/
		Transform();
		glDrawArrays(GL_TRIANGLES, 9, 3); //오

		/*텍스쳐5*/
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glBindVertexArray(_vao[4]);
		/*초기화 후 변환 행렬 채우기*/
		Transform();

		/*도형의 색*/
		int objColorLocation = glGetUniformLocation(shaderID, "objectColor");
		glUniform4f(objColorLocation, _colorR, _colorG, _colorB, 1.0f);

		glDrawArrays(GL_TRIANGLES, 12, 6); //아래
	}
}
void Pyramid::SetAlive(bool alive)
{
	if (alive == true)
		_Alive = true;
	else
		_Alive = false;

}
void Pyramid::SetColor(float r, float g, float b)
{
	_colorR = r;
	_colorG = g;
	_colorB = b;
}
void Pyramid::SetPosition(float x, float y, float z)
{
	_positionX = x;
	_positionY = y;
	_positionZ = z;
}
void Pyramid::SetScale(float x, float y, float z)
{
	_scaleX = x;
	_scaleY = y;
	_scaleZ = z;
}
void Pyramid::Transform()
{
	unsigned int transformLocate = glGetUniformLocation(shaderID, "model");
	/*리셋 한번 해주고*/
	_mixMat = glm::mat4{ 1.0f };
	_mixMat = glm::translate(_mixMat, glm::vec3(_positionX + _moveX, _positionY, _positionZ));
	_mixMat = glm::rotate(_mixMat, glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
	_mixMat = glm::rotate(_mixMat, glm::radians(rotateX), glm::vec3(1.0f, 0.0f, 0.0f));
	_mixMat = glm::scale(_mixMat, glm::vec3(_scaleX, _scaleY, _scaleZ));
	glUniformMatrix4fv(transformLocate, 1, GL_FALSE, glm::value_ptr(_mixMat));
}
void Pyramid::Move()
{
	if (_moveArrow == 0) {
		_moveX += 0.01f;
	}
	else if (_moveArrow == 1) {
		_moveX -= 0.01f;
	}

	if (_moveX >= 1.0f) {
		_moveArrow = 1;
	}
	else if (_moveX <= -1.0f) {
		_moveArrow = 0;
	}
}
/*******************Class::Plain 함수*******************/
void Plain::InitBuffer()
{
	GLint pAttribute = glGetAttribLocation(shaderID, "vPos");
	GLint nAttribute = glGetAttribLocation(shaderID, "vNormal");
	GLint tAttribute = glGetAttribLocation(shaderID, "vTexCoord");
	/*create buffer*/
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	/*vao binding*/
	glBindVertexArray(_vao);
	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectVerticles), rectVerticles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //--- 위치 속성
	glEnableVertexAttribArray(pAttribute);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //--- 노말 속성
	glEnableVertexAttribArray(nAttribute);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //--- 텍스처 좌표 속성
	glEnableVertexAttribArray(tAttribute);
}
void Plain::Draw()
{
	/*도형의 색*/
	int objColorLocation = glGetUniformLocation(shaderID, "objectColor");
	glUniform4f(objColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);

	/*텍스쳐6*/

	glBindVertexArray(_vao);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	/*초기화 후 변환 행렬 채우기*/
	Transform();

	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Plain::SetAlive(bool alive)
{
	if (alive == true)
		_Alive = true;
	else
		_Alive = false;

}
void Plain::SetColor(float r, float g, float b)
{
	_colorR = r;
	_colorG = g;
	_colorB = b;
}
void Plain::SetPosition(float x, float y, float z)
{
	_positionX = x;
	_positionY = y;
	_positionZ = z;
}
void Plain::SetScale(float x, float y, float z)
{
	_scaleX = x;
	_scaleY = y;
	_scaleZ = z;
}
void Plain::Transform()
{
	unsigned int transformLocate = glGetUniformLocation(shaderID, "model");
	/*리셋 한번 해주고*/
	_mixMat = glm::mat4{ 1.0f };
	_mixMat = glm::translate(_mixMat, glm::vec3(_positionX + _moveX, _positionY, _positionZ));
	_mixMat = glm::scale(_mixMat, glm::vec3(_scaleX, _scaleY, _scaleZ));
	glUniformMatrix4fv(transformLocate, 1, GL_FALSE, glm::value_ptr(_mixMat));
}
void Plain::Move()
{
	if (_moveArrow == 0) {
		_moveX += 0.01f;
	}
	else if (_moveArrow == 1) {
		_moveX -= 0.01f;
	}

	if (_moveX >= 1.0f) {
		_moveArrow = 1;
	}
	else if (_moveX <= -1.0f) {
		_moveArrow = 0;
	}
}
/******************Class::Sphere 함수******************/
void Sphere::InitBuffer()
{
	Load_Object("sphere.obj");

	GLint pAttribute = glGetAttribLocation(shaderID, "vPos");
	GLint vAttribute = glGetAttribLocation(shaderID, "vNormal");
	/*create buffer*/
	glGenVertexArrays(1, &_vao);
	glGenBuffers(2, _vbo);
	glGenBuffers(1, &_ebo);

	/*vao binding*/
	glBindVertexArray(_vao);

	/*vbo binding*/
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(vAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int), &vertexIndices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(vAttribute);
	glEnableVertexAttribArray(pAttribute);
}
void Sphere::Draw()
{
	if (_Alive == true)
	{
		glBindVertexArray(_vao);
		/*초기화 후 변환 행렬 채우기*/
		Transform();

		/*도형의 색*/
		int objColorLocation = glGetUniformLocation(shaderID, "objectColor");
		glUniform4f(objColorLocation, _colorR, _colorG, _colorB, 1.0f);

		glDrawElements(GL_TRIANGLES, 3000, GL_UNSIGNED_INT, 0);
	}
}
void Sphere::SetAlive(bool alive)
{
	if (alive == true)
		_Alive = true;
	else
		_Alive = false;
}
void Sphere::SetColor(float r, float g, float b)
{
	_colorR = r;
	_colorG = g;
	_colorB = b;
}
void Sphere::SetPosition(float x, float y, float z)
{
	_positionX = x;
	_positionY = y;
	_positionZ = z;
}
void Sphere::Transform()
{
	unsigned int transformLocate = glGetUniformLocation(shaderID, "model");
	_revolution += 4.0f;

	/*리셋 한번 해주고*/
	_mixMat = glm::mat4{ 1.0f };
	if (_revType == 0) {
		_mixMat = glm::translate(_mixMat, glm::vec3(_positionX, 1.0f, _positionZ));
		_mixMat = glm::rotate(_mixMat, glm::radians(_revolution), glm::vec3(0.0f, 0.0f, 1.0f));
		_mixMat = glm::translate(_mixMat, glm::vec3(0.0f, 1.0f, 0.0f));
		_mixMat = glm::scale(_mixMat, glm::vec3(1.0f, 1.0f, 1.0f));
	}
	else if (_revType == 1) {
		_mixMat = glm::translate(_mixMat, glm::vec3(_positionX, 1.0f, _positionZ));
		_mixMat = glm::rotate(_mixMat, glm::radians(_revolution), glm::vec3(1.0f, 0.0f, 0.0f));
		_mixMat = glm::translate(_mixMat, glm::vec3(0.0f, 1.0f, 0.0f));
		_mixMat = glm::scale(_mixMat, glm::vec3(0.5f, 0.5f, 0.5f));
	}
	else if (_revType == 2) {
		_mixMat = glm::translate(_mixMat, glm::vec3(_positionX, 0.7f, _positionZ));
		_mixMat = glm::rotate(_mixMat, glm::radians(_revolution), glm::vec3(0.0f, 1.0f, 0.0f));
		_mixMat = glm::translate(_mixMat, glm::vec3(1.0f, 0.0f, 0.0f));
		_mixMat = glm::scale(_mixMat, glm::vec3(0.7f, 0.7f, 0.7f));
	}
	else if (_revType == 3) {
		if (snowSwitch == true) {
			_positionY -= 0.01f * _speed;
		}
		if (_positionY <= 0.0f)
		{
			_positionY = 4.0f, _positionX = (float)snowPosition(eng), _positionZ = (float)snowPosition(eng);
		}
		_mixMat = glm::translate(_mixMat, glm::vec3(_positionX, _positionY, _positionZ));
		_mixMat = glm::scale(_mixMat, glm::vec3(0.1f, 0.1f, 0.1f));
	}
	glUniformMatrix4fv(transformLocate, 1, GL_FALSE, glm::value_ptr(_mixMat));
}
void Sphere::SetRevType(int revType) {
	_revType = revType;
}
/********************셰이더 프로그램*******************/
void make_vertexShader()
{
	GLchar* vertexSource;
	GLint result;
	GLchar errorLog[512];
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_fragmentShader()
{
	GLchar* fragmentSource;
	GLint result;
	GLchar errorLog[512];
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf("fragment.glsl"); // 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_shaderProgram()
{
	GLchar errorLog[512];
	GLint result;
	make_vertexShader();
	make_fragmentShader();

	//-- shader Program
	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);
	glLinkProgram(shaderID);

	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ---세이더가 잘 연결되었는지 체크하기
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return;
	}

	//--- Shader Program 사용하기
	glUseProgram(shaderID);
}
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0; // Null terminator 
	return buf; // Return the buffer
}
bool Load_Object(const char* path)
{
	vertexIndices.clear();
	uvIndices.clear();
	normalIndices.clear();
	vertices.clear();
	uvs.clear();
	normals.clear();

	ifstream in(path);
	if (!in) {
		cerr << path << "파일 못찾음";
		exit(1);
	}

	//vector<char> lineHeader(istream_iterator<char>{in}, {});

	while (in) {
		string lineHeader;
		in >> lineHeader;
		if (lineHeader == "v") {
			glm::vec3 vertex = { 0.0f,0.0f,0.0f };
			in >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		}
		else if (lineHeader == "vt") {
			glm::vec2 uv = { 0.0f,0.0f };
			in >> uv.x >> uv.y;
			uvs.push_back(uv);
		}
		else if (lineHeader == "vn") {
			glm::vec3 normal = { 0.0f,0.0f,0.0f };
			in >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (lineHeader == "f") {
			char a;
			unsigned int vertexIndex[3] = { 0 }, uvIndex[3] = { 0 }, normalIndex[3] = { 0 };

			for (int i = 0; i < 3; i++)
			{
				in >> vertexIndex[i] >> a >> uvIndex[i] >> a >> normalIndex[i];
				vertexIndices.push_back(vertexIndex[i] - 1);
				uvIndices.push_back(uvIndex[i] - 1);
				normalIndices.push_back(normalIndex[i] - 1);
			}
		}
	}
	return true;
} //obj파일 로드
/*********************텍스쳐 함수**********************/
void InitTexture() {
	/*****1:앞*****/
	glGenTextures(6, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data1 = stbi_load("container1.jpg", &width, &height, &nrChannels, 0);
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data1);

	/*****2:오*****/
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	unsigned char* data2 = stbi_load("container2.jpg", &width, &height, &nrChannels, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data2);

	/*****3:뒤*****/
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	unsigned char* data3 = stbi_load("container3.jpg", &width, &height, &nrChannels, 0);
	if (data3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data3);

	/*****4:왼*****/
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	unsigned char* data4 = stbi_load("container4.jpg", &width, &height, &nrChannels, 0);
	if (data4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data4);

	/*****5:위*****/
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	unsigned char* data5 = stbi_load("container5.jpg", &width, &height, &nrChannels, 0);
	if (data5)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data5);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data5);

	/*****6:아래*****/
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	unsigned char* data6 = stbi_load("container6.jpg", &width, &height, &nrChannels, 0);
	if (data6)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data6);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data6);



	/*****7:타일1*****/
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	unsigned char* data7 = stbi_load("check.jpg", &width, &height, &nrChannels, 0);
	if (data7)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data7);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data7);

}
GLubyte* LoadDIBitmap(const char* filename, BITMAPINFO** info)
{
	FILE* fp;
	GLubyte* bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	//--- 바이너리 읽기 모드로 파일을 연다
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	//--- 비트맵 파일 헤더를 읽는다.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp); return NULL;
	}
	//--- 파일이 BMP 파일인지 확인한다.
	if (header.bfType != 'MB') {
		fclose(fp); return NULL;
	}
	//--- BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	//--- 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ((*info = (BITMAPINFO*)malloc(infosize)) == NULL) {
		fclose(fp); return NULL;
	}
	//--- 비트맵 인포 헤더를 읽는다.
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp); return NULL;
	}
	//--- 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth * (*info)->bmiHeader.biBitCount + 7) / 8.0 * abs((*info)->bmiHeader.biHeight);
	//--- 비트맵의 크기만큼 메모리를 할당한다.
	if ((bits = (unsigned char*)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp); return NULL;
	}
	//--- 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp); return NULL;
	}
	fclose(fp);
	return bits;
}

