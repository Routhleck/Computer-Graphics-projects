#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <windows.h> //libraries for time etc
#include <vector>
#include <mmsystem.h> //libraries for time etc


#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Model.h"
#include "GUI.h"
#include "Input.h"
#include "Particles.h"



float particles_slowdown = 1.0;  // 粒子减速因子
float velocity = 0.0;  // 粒子速度

int MAX_PARTICLES = 1000; // 定义最大粒子数
bool particles_alive = true; // 粒子存活状态
float particles_life = 2.0f; // 粒子寿命
int particles_fadeMax = 200; // 粒子衰减速度
float particles_gravity = -0.8f; // 粒子重力
float zoom = -40.0;   // 粒子缩放


std::vector<particles> par_sys;  // 粒子vector
Shader* ptr1;                      // 着色器指针
Shader* b_shader;                  // 边框着色器指针
Shader* sky_shader;                // 天空着色器指针
Model* ptr2, * ptr3, * ptr4, * ptr5, * ptr6, * ptr7, * ptr8, * ptr9, * ptr10, * ptr11, * ptr12, * ptr13, * ptr14;  // 模型指针
unsigned int texture1;             // 纹理ID
unsigned int EBO;                  // 索引缓冲对象ID
unsigned int skyboxVAO, skyboxVBO;  // 天空盒顶点数组对象和顶点缓冲对象
unsigned int skymapTexture;        // 天空盒纹理ID

glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 3.0f);     // 摄像机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);  // 摄像机方向
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);      // 摄像机的上方向

glm::vec3 Mov = glm::vec3(0.9f, 0.0f, 0.4f);    // 移动向量
glm::vec3 Mov_1 = glm::vec3(0.8f, 0.0f, 0.3f);  // 移动向量1
glm::vec3 Mov_2 = glm::vec3(0.6f, -0.02f, 0.8); // 移动向量2
float r1 = 0.0f;   // 旋转角度1
float r2 = 180.0f; // 旋转角度2
float s1 = -25.0f; // 缩放因子1

glm::vec3 lightpos_2 = glm::vec3(0.7f, 0.63f, -0.2f); // 光源2位置
glm::vec3 lightpos_3 = glm::vec3(-1.1f, 0.63f, 0.27f); // 光源3位置


bool firstMouse = true;
float yaw = -90.0f; // 偏航角初始值为-90.0度，因为偏航角为0时，方向向量指向右侧，所以初始时稍微向左旋转一些。
float pitch = 0.0f; // 俯仰角初始值为0度
float lastX = 800.0f / 2.0;  // 上次鼠标位置的X坐标
float lastY = 600.0 / 2.0;   // 上次鼠标位置的Y坐标
float fov = 45.0f;           // 透视投影的视野

glm::vec3 Position;  // 位置向量
glm::vec3 Front;     // 前方向量
glm::vec3 Up;        // 上方向量
glm::vec3 Right;     // 右方向量
glm::vec3 WorldUp;   // 场景上方向量


int window_width = 800;
int window_height = 600;

bool isFullScreen = false; // 是否全屏（初始值为否）
bool isCursor = false;      // 是否显示光标（初始值为是）

// 定义宏以索引顶点缓冲
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

const unsigned int SCR_WIDTH = 800;  // 屏幕宽度
const unsigned int SCR_HEIGHT = 600; // 屏幕高度

bool keyState[256];
float sensitivity = 0.1f; //灵敏度
float cameraSpeed = 0.05f;      //相机速度

int lastMouseX = window_width / 2;
int lastMouseY = window_height / 2;
int currentMouseX = window_width / 2;
int currentMouseY = window_height / 2;

GLuint VAO, VBO;  // 顶点数组对象和顶点缓冲对象

// 立方体顶点坐标
float vertices[] = {
	   -0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
	   -0.5f,  0.5f, -0.5f,
	   -0.5f, -0.5f, -0.5f,

	   -0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f,  0.5f,
	   -0.5f, -0.5f,  0.5f,

	   -0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f, -0.5f,
	   -0.5f, -0.5f, -0.5f,
	   -0.5f, -0.5f, -0.5f,
	   -0.5f, -0.5f,  0.5f,
	   -0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

	   -0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
	   -0.5f, -0.5f,  0.5f,
	   -0.5f, -0.5f, -0.5f,

	   -0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f,  0.5f,
	   -0.5f,  0.5f, -0.5f,
};

float skyboxVertices[] = {
	// 天空盒顶点坐标
	// 坐标值基于投影的最大值，此处为100
	-12.0f,  12.0f, -12.0f,
	-12.0f, -12.0f, -12.0f,
	 12.0f, -12.0f, -12.0f,
	 12.0f, -12.0f, -12.0f,
	 12.0f,  12.0f, -12.0f,
	-12.0f,  12.0f, -12.0f,

	-12.0f, -12.0f,  12.0f,
	-12.0f, -12.0f, -12.0f,
	-12.0f,  12.0f, -12.0f,
	-12.0f,  12.0f, -12.0f,
	-12.0f,  12.0f,  12.0f,
	-12.0f, -12.0f,  12.0f,

	 12.0f, -12.0f, -12.0f,
	 12.0f, -12.0f,  12.0f,
	 12.0f,  12.0f,  12.0f,
	 12.0f,  12.0f,  12.0f,
	 12.0f,  12.0f, -12.0f,
	 12.0f, -12.0f, -12.0f,

	-12.0f, -12.0f,  12.0f,
	-12.0f,  12.0f,  12.0f,
	 12.0f,  12.0f,  12.0f,
	 12.0f,  12.0f,  12.0f,
	 12.0f, -12.0f,  12.0f,
	-12.0f, -12.0f,  12.0f,

	-12.0f,  12.0f, -12.0f,
	 12.0f,  12.0f, -12.0f,
	 12.0f,  12.0f,  12.0f,
	 12.0f,  12.0f,  12.0f,
	-12.0f,  12.0f,  12.0f,
	-12.0f,  12.0f, -12.0f,

	-12.0f, -12.0f, -12.0f,
	-12.0f, -12.0f,  12.0f,
	 12.0f, -12.0f, -12.0f,
	 12.0f, -12.0f, -12.0f,
	-12.0f, -12.0f,  12.0f,
	 12.0f, -12.0f,  12.0f
};

// 天空盒纹理图像路径
vector<std::string> faces
{

	"..\\skybox\\posx.jpg",
	"..\\skybox\\negx.jpg",
	"..\\skybox\\posy.jpg",
	"..\\skybox\\negy.jpg",
	"..\\skybox\\posz.jpg",
	"..\\skybox\\negz.jpg"
};




// 窗口大小改变回调函数
void windowResizeHandler(int width, int height) {
	window_width = width;
    window_height = height;

    // 设置视口
    glViewport(0, 0, window_width, window_height);
}


// 加载天空盒纹理
unsigned int loadskymap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

// 绘制下落的雪花并更新其位置
void drawSnow() {
	float x, y, z;
	for (int loop = 0; loop < par_sys.size(); loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos;
			y = par_sys[loop].ypos;
			z = par_sys[loop].zpos + zoom;

			// 将摄像机位置、投影矩阵、视图矩阵和模型矩阵传递给着色器，并使用指定的着色器绘制模型
			ptr1->setVec3("viewPos", cameraPos);
			glm::mat4 projection = glm::perspective(glm::radians(fov), (float)window_width / (float)window_height, 0.1f, 100.0f);
			ptr1->setMat4("projection", projection);
			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			ptr1->setMat4("view", view);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(x, y, z));
			model = glm::scale(model, glm::vec3(1.0f / 1000));
			ptr1->setMat4("model", model);
			ptr6->Draw(ptr1);


			// 更新粒子的位置
			// 移动
			par_sys[loop].ypos += par_sys[loop].vel / (particles_slowdown * 1000);
			par_sys[loop].vel += par_sys[loop].gravity;
			// 衰变
			par_sys[loop].life -= par_sys[loop].fade;

			// 如果粒子掉出了视野范围，重新生成一个新的雪花粒子
			if (par_sys[loop].ypos <= -10) {
				par_sys[loop].life = -1.0;
			}

			// 如果粒子寿命结束，重新生成一个新的雪花粒子 
			if (par_sys[loop].life < 0.0) {
				respawnByIndex(loop);
			}
		}
	}
}

//显示函数
void display() {

	// 清除颜色缓冲区和深度缓冲区
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//启用深度测试
	glEnable(GL_DEPTH_TEST);

	// 设置摄像机位置和观察方向
	ptr1->use();
	// 将投影矩阵传递给着色器(注意，在这种情况下，它可以改变每一帧)
	ptr1->setVec3("viewPos", cameraPos);
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)window_width / (float)window_height, 0.1f, 100.0f);
	ptr1->setMat4("projection", projection);

	// 相机/视口变换
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	ptr1->setMat4("view", view);

	// 使用粒子效果开始下雪
	drawSnow();
	
	// 房屋模型读取
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(1.0f/100, 1.0f/100, 1.0f/100));	
	model = glm::rotate(model, glm::radians(182.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr2->Draw(ptr1);

	// 地面
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
	model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr3->Draw(ptr1);

	// 山脉地形
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f)); 
	model = glm::scale(model, glm::vec3(3.5f, 2.7f, 3.5f));	
	ptr1->setMat4("model", model);
	ptr4->Draw(ptr1);

	// 驯鹿 1
	model = glm::mat4(1.0f);
	model = glm::translate(model, Mov); 
	model = glm::scale(model, glm::vec3(0.1f/100, 0.1f/100, 0.1f/100));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(r1), glm::vec3(0.0f, 0.0f, 1.0f));
	ptr1->setMat4("model", model);
	ptr5->Draw(ptr1);

	// 驯鹿 2 right
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.48f)); 
	model = glm::scale(model, glm::vec3(0.1f / 100, 0.1f / 100, 0.1f / 100));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-50.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ptr1->setMat4("model", model);
	ptr5->Draw(ptr1);

	// 驯鹿 3 left
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.63f)); 
	model = glm::scale(model, glm::vec3(0.1f / 100, 0.1f / 100, 0.1f / 100));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ptr1->setMat4("model", model);
	ptr5->Draw(ptr1);

	// 驯鹿 4 back facing
	model = glm::mat4(1.0f);
	model = glm::translate(model, Mov_1); 
	model = glm::scale(model, glm::vec3(0.1f / 100, 0.1f / 100, 0.1f / 100));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(r2), glm::vec3(0.0f, 0.0f, 1.0f));
	ptr1->setMat4("model", model);
	ptr5->Draw(ptr1);

	// 驯鹿 back on mountain
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.8f, 0.46f, -1.3f)); 
	model = glm::scale(model, glm::vec3(0.1f / 100, 0.1f / 100, 0.1f / 100));	
	model = glm::rotate(model, glm::radians(110.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ptr1->setMat4("model", model);
	ptr5->Draw(ptr1);

	// 驯鹿 front on mountain
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.68f, -1.6f)); 
	model = glm::scale(model, glm::vec3(0.1f / 100, 0.1f / 100, 0.1f / 100));	
	model = glm::rotate(model, glm::radians(112.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(200.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ptr1->setMat4("model", model);
	ptr5->Draw(ptr1);

	// 驯鹿 more front on mountain
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.7f, 0.85f, -1.95f)); 
	model = glm::scale(model, glm::vec3(0.1f / 100, 0.1f / 100, 0.1f / 100));	
	model = glm::rotate(model, glm::radians(120.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(203.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ptr1->setMat4("model", model);
	ptr5->Draw(ptr1);

	// 驯鹿 1 left side scene
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.9f, 0.3f, -2.1f)); 
	model = glm::scale(model, glm::vec3(0.1f / 100, 0.1f / 100, 0.1f / 100));	
	model = glm::rotate(model, glm::radians(98.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr5->Draw(ptr1);

	// 驯鹿 left side scene front
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.7f, 0.17f, -1.7f)); 
	model = glm::scale(model, glm::vec3(0.1f / 100, 0.1f / 100, 0.1f / 100));	
	model = glm::rotate(model, glm::radians(114.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr5->Draw(ptr1);
	   
	// 驯鹿 behind house
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.3f, 0.0f, -0.7f)); 
	model = glm::scale(model, glm::vec3(0.1f / 100, 0.1f / 100, 0.1f / 100));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(245.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ptr1->setMat4("model", model);
	ptr5->Draw(ptr1);

	// 长椅 on right towards mountain
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.4f, 0.0f, 0.3f));
	model = glm::scale(model, glm::vec3(0.1f/2.6));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr7->Draw(ptr1);

	// 长椅 on right towards camera
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.4f, 0.0f, 0.7f));
	model = glm::scale(model, glm::vec3(0.1f / 2.6));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr7->Draw(ptr1);

	// 长椅 on left
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.3f));
	model = glm::scale(model, glm::vec3(0.1f / 2.6));	
	model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr7->Draw(ptr1);

	//长椅 on left
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.1f, 0.0f, 0.27f)); 
	model = glm::scale(model, glm::vec3(0.1f / 2.6));	
	ptr1->setMat4("model", model);
	ptr8->Draw(ptr1);

	// 灯 左侧
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.7, 0.0, -0.2));
	model = glm::scale(model, glm::vec3(0.1f / 2.6));	
	ptr1->setMat4("model", model);
	ptr8->Draw(ptr1);

	// 汽车
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0, 0.0, -0.1));
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr9->Draw(ptr1);

	// 房屋右侧雪人
	model = glm::mat4(1.0f);
	model = glm::translate(model, Mov_2);
	model = glm::scale(model, glm::vec3(0.05f));	
	model = glm::rotate(model, glm::radians(s1), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr10->Draw(ptr1);

	// 长椅右侧雪人
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.2, -0.02, 0.35));
	model = glm::scale(model, glm::vec3(0.05f));	
	model = glm::rotate(model, glm::radians(-70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr10->Draw(ptr1);

	// 左侧雪人
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.3, -0.02, -0.9));
	model = glm::scale(model, glm::vec3(0.05f));	
	model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr10->Draw(ptr1);

	// 道路
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.4, 0.0, -0.7)); 
	model = glm::scale(model, glm::vec3(0.1f/8, 0.1f/10, 0.1f/5));	
	ptr1->setMat4("model", model);
	ptr11->Draw(ptr1);

	// 灯柱旁的护栏
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.2, 0.0, -0.15));
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 左侧极限出的护栏
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-5.0, 0.0, -0.5)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 1 (from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-4.5, 0.0, -0.5)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 2 (from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-4.0, 0.0, -0.5)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 3 (from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.5, 0.0, -0.5)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 4 (from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0, 0.0, -0.5)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 5 (from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.5, 0.0, -0.5)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 6 (from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.0, 0.0, -0.5)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 1 across road(from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-4.75, 0.0, -1.2)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 2 across road(from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-4.25, 0.0, -1.2)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 3 across road(from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.75, 0.0, -1.2)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 4 across road(from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.25, 0.0, -1.2)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 5 across road(from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.75, 0.0, -1.2)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 6 across road(from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.25, 0.0, -1.2)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	// 护栏 7 across road(from left)
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.75, 0.0, -1.2)); 
	model = glm::scale(model, glm::vec3(0.15f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr12->Draw(ptr1);

	//树木
	// 在左边的灯旁
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0, 0.0, -0.1)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 房屋前
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.2, 0.0, 0.25)); 
	model = glm::scale(model, glm::vec3(0.07f));	
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr14->Draw(ptr1);

	// 就在长凳前
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.5, 0.0, 0.9)); 
	model = glm::scale(model, glm::vec3(0.07f));	
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr14->Draw(ptr1);

	// 就在长椅之间
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.6, 0.0, -0.6)); 
	model = glm::scale(model, glm::vec3(0.07f));	
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr14->Draw(ptr1);

	// 房屋后
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.2, 0.0, -1.9)); 
	model = glm::scale(model, glm::vec3(0.07f));	
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr14->Draw(ptr1);

	// 在右边的灯旁
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.7, 0.0, -0.4)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 在山的右前方
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.3, 0.6, -1.2)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 在山顶右方
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.8, 0.9, -1.9)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 在山顶左方
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.7, 0.3, -1.7)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 小山左方
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.5, 0.2, -1.7)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 左后方
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5, 0.2, -2.4)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 左前方
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.5, -0.1, -4.0)); 
	model = glm::scale(model, glm::vec3(0.07f));	
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr14->Draw(ptr1);

	// 左侧
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.2, 0.0, -0.25)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 路左方
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.2, 0.0, -0.2)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 路左过去 1
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.8, -0.0, -1.0)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 路左过去 2
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-2.7, -0.0, -1.0)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 路左过去 3
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.7, -0.0, -1.0)); 
	model = glm::scale(model, glm::vec3(0.1f));	
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ptr1->setMat4("model", model);
	ptr13->Draw(ptr1);

	// 右侧光源
	b_shader->use();
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightpos_2);
	model = glm::scale(model, glm::vec3(1.0f/30));
	b_shader->setMat4("projection", projection);
	b_shader->setMat4("model", model);
	b_shader->setMat4("view", view);
	b_shader->setVec4("color", glm::vec4(0.96,0.83,0.51,1.0));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 左侧光源
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightpos_3);
	model = glm::scale(model, glm::vec3(1.0f/30));
	b_shader->setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LEQUAL);

	// 天空盒mapping
	sky_shader->use();
	sky_shader->setInt("skybox", 0);
	sky_shader->setMat4("projection", projection);
	sky_shader->setMat4("view", view);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skymapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 更新摄像机信息
	updateCamera();

	// 渲染GUI
	if (isCursor) renderGUI();

	/*
	glDepthFunc() 函数用于指定深度测试函数的类型。
	在这里，参数 GL_LESS 指示深度测试函数应该是 "小于" 的关系。
	这意味着，如果当前像素的深度值小于存储在深度缓冲区中的深度值，
	那么该像素将被绘制。否则，它将被丢弃
	*/
	glDepthFunc(GL_LESS);


	glutPostRedisplay();
	glutSwapBuffers();

}

// 初始化着色器
void initShader() {
	ptr1 = new Shader("..\\VertexS.vert", "..\\FragmentS1.frag");
	b_shader = new Shader("..\\b_vert.vert", "..\\b_frag.frag");
	sky_shader = new Shader("..\\box_shader.vert", "..\\box_fragment.frag");
}

// 初始化对象
void initObject() {
	ptr2 = new Model("..\\models\\home\\finalghar.obj");
	ptr3 = new Model("..\\models\\plane_dhanju\\ok1.dae");
	ptr4 = new Model("..\\models\\moun\\everest.obj");
	ptr5 = new Model("..\\models\\reindeer\\12164_reindeer_v1_L3.obj");
	ptr6 = new Model("..\\models\\snowflake\\snowf.dae");
	ptr7 = new Model("..\\models\\bench\\bench.obj");
	ptr8 = new Model("..\\models\\lamppost\\lamppost.obj");
	ptr9 = new Model("..\\models\\car\\car.obj");
	ptr10 = new Model("..\\models\\snowman\\snowman.obj");
	ptr11 = new Model("..\\models\\road\\road.obj");
	ptr12 = new Model("..\\models\\barrier\\uploads_files_3661633_Barrier.obj");
	ptr13 = new Model("..\\models\\trees\\treemain.obj");
	ptr14 = new Model("..\\models\\trees\\tree2.obj");
}

// 光源初始化设置
void initLight() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), 0);
}

// 天空盒初始化设置
void initSkybox() {
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	skymapTexture = loadskymap(faces);
}

int main(int argc, char** argv) {

	// 窗口初始化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("Computer Graphics Homework 2 -- Group 4");

	// 隐藏鼠标并放在中心
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(window_width / 2, window_height / 2);
	
	// 注册各种回调函数
	glutDisplayFunc(display);
	registerKeyBoardFunc();
	glutMotionFunc(mouseMotionHandler);
	glutPassiveMotionFunc(mouseMotionHandler);
	glutMouseFunc(mouseButtonHandler);
	glutReshapeFunc(windowResizeHandler);

	// 对glewInit()的调用必须在glut初始化之后完成
	GLenum res = glewInit();

	// 检查错误
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	// 初始化ImGui
	initGUI(nullptr);

	// 初始化着色器
	initShader();

	// 初始化对象
	initObject();
	
	// 光源设置
	initLight();

	// 天空盒设置
	initSkybox();

	// 初始化粒子
	createParticles();

	glutMainLoop();

	cleanupGUI();
	return 0;
}











