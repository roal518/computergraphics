#pragma once
/**************************헤더파일**************************/
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;

/**************************변수선언**************************/
random_device seeder;
const auto seed = seeder.entropy() ? seeder() : time(nullptr);
mt19937 eng(static_cast<mt19937::result_type>(seed));
uniform_real_distribution<double> snowPosition(-1.0f, 1.0f);
uniform_real_distribution<double> snowSpeed(0.5f, 1.5f);
uniform_real_distribution<double> randomRev(0.0f, 360.0f);
/*셰이더 프로그램 변수*/
GLuint shaderID;
GLuint vertexShader;
GLuint fragmentShader;
/*obj파일 불러오는 변수들*/
std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals;
/*텍스쳐 관련 변수들*/
unsigned int texture[7];
unsigned int whiteTexture;
/*체크 변수*/
bool lightRoate;
bool lightSwitch = true;
bool cameraRotate = false;
bool snowSwitch = false;
bool objectRotateY = false;
bool objectRotateX = false;
/*상태 변화 변수*/
float lightX = 0.0f, lightY = 1.0f, lightZ = 1.0f;
float lightRadians = 90.0f / 360.0f * 2.0f * 3.141592f;
float lightRadiusZ = 0.0f;
float cameraX = 0.0f;
float cameraY = 5.0f;
float cameraZ = 0.0f;
float cameraRaidans = 0.0f;
float lightPower = 1.0f;
float lightVal = 90.0f;
float rotateY = 0.0f;
float rotateX = 0.0f;
/*버텍스 배열*/
float cubeVerticles[] = {
	// 뒷면
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
	// 앞면
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
	//왼쪽
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	//오른쪽
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
	 //아래
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	//위
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
};
float rectVerticles[] = {
	//아래
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
};
float pyramidVerticles[] = {
	//뒷면
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
	 0.0f,  0.5f, -0.0f,  0.0f,  0.0f, -1.0f, 0.5f, 1.0f,
	 //앞면
	 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
	  0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f, 0.5f, 1.0f,
	  //왼쪽
	  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
	  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
	   0.0f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.5f, 1.0f,
	   //오른쪽
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.5f, 1.0f,
		//아래
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f
};
/**************************오브젝트**************************/
/********땅********/
class Plain
{
private:
	/*배열 관련*/
	GLuint _vao, _vbo;
	/*행렬 관련*/
	glm::mat4 _mixMat = glm::mat4{ 1.0f };
	/*상태 변화 관련*/
	bool _Alive = false;
	float _positionX, _positionY, _positionZ;
	float _moveX;
	float _colorR, _colorG, _colorB;
	float _scaleX, _scaleY, _scaleZ;
	int _moveArrow;
public:
	Plain()
	{


	};
	/*객체 초기화*/
	void SetAlive(bool alive);
	void SetColor(float r, float g, float b);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	/*버퍼 초기화*/
	void InitBuffer();
	/*그리기*/
	void Draw();
	/*상태 변화*/
	void Transform();
	/*애니메이션*/
	void Move();
	~Plain()
	{


	};
};
Plain plain[15];
/*******큐브********/
class Cube
{
private:
	/*배열 관련*/
	GLuint _vao[6], _vbo[1];
	/*행렬 관련*/
	glm::mat4 _mixMat = glm::mat4{ 1.0f };
	/*상태 변화 관련*/
	bool _Alive = false;
	float _positionX, _positionY, _positionZ;
	float _moveX;
	float _colorR, _colorG, _colorB;
	float _scaleX, _scaleY, _scaleZ;
	int _moveArrow;
public:
	Cube()
	{


	};
	/*객체 초기화*/
	void SetAlive(bool alive);
	void SetColor(float r, float g, float b);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	/*버퍼 초기화*/
	void InitBuffer();
	/*그리기*/
	void Draw();
	void Draw2();
	/*상태 변화*/
	void Transform();
	/*애니메이션*/
	void Move();
	~Cube()
	{


	};
};
Cube cube[3];
Cube obstacle[3];
Cube robot[2];
/*******조명********/
class LightCube
{
private:
	/*배열 관련*/
	GLuint _vao, _vbo[2];
	/*행렬 관련*/
	glm::mat4 _mixMat = glm::mat4{ 1.0f };
	/*상태 변화 관련*/
	bool _Alive = false;
	float _rotateY = 0.0f;
public:
	LightCube()
	{


	};
	/*객체 초기화*/
	void SetAlive(bool alive);

	/*버퍼 초기화*/
	void InitBuffer();
	/*그리기*/
	void Draw();
	/*상태 변화*/
	void Transform();
	void Rotating();
	/*애니메이션*/
	~LightCube()
	{


	};
};
LightCube lightCube;
/*******구********/
class Sphere
{
private:
	/*배열 관련*/
	GLuint _vao, _vbo[2], _ebo;
	/*행렬 관련*/
	glm::mat4 _mixMat = glm::mat4{ 1.0f };
	/*상태 변화 관련*/
	bool _Alive = false;
	float _positionX, _positionY, _positionZ;
	float _colorR, _colorG, _colorB;
	float _revolution;
	int _revType = 0;
	float _speed = 0.0f;
public:
	Sphere()
	{
		_revolution = (float)randomRev(eng);
		_speed = (float)snowSpeed(eng);
	};
	/*객체 초기화*/
	void SetAlive(bool alive);
	void SetColor(float r, float g, float b);
	void SetPosition(float x, float y, float z);
	void SetRevType(int num);
	/*버퍼 초기화*/
	void InitBuffer();
	/*그리기*/
	void Draw();
	/*상태 변화*/
	void Transform();
	/*애니메이션*/
	~Sphere()
	{


	};
};
Sphere sphere[3];
Sphere snow[50];
/*****피라미드******/
class Pyramid
{
private:
	/*배열 관련*/
	GLuint _vao[5], _vbo[5];
	/*행렬 관련*/
	glm::mat4 _mixMat = glm::mat4{ 1.0f };
	/*상태 변화 관련*/
	bool _Alive = false;
	float _positionX, _positionY, _positionZ;
	float _moveX;
	float _colorR, _colorG, _colorB;
	float _scaleX, _scaleY, _scaleZ;
	int _moveArrow;
public:
	Pyramid()
	{


	};
	/*객체 초기화*/
	void SetAlive(bool alive);
	void SetColor(float r, float g, float b);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);
	/*버퍼 초기화*/
	void InitBuffer();
	/*그리기*/
	void Draw();
	/*상태 변화*/
	void Transform();
	/*애니메이션*/
	void Move();
	~Pyramid()
	{


	};
};
Pyramid pyramid;