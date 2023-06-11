#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <windows.h> //libraries for time etc
#include <mmsystem.h> //libraries for time etc

#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Model.h"

#define MAX_PARTICLES 1000

float slowdown = 1.0;
float velocity = 0.0;
float zoom = -40.0;

typedef struct {
	// Life
	bool alive;	// is the particle alive?
	float life;	// particle lifespan
	float fade; // decay
	// color
	float red;
	float green;
	float blue;
	// Position/direction
	float xpos;
	float ypos;
	float zpos;
	// Velocity/Direction, only goes down in y dir
	float vel;
	// Gravity
	float gravity;
}particles;

particles par_sys[MAX_PARTICLES];
Shader* ptr1;
Shader* b_shader;
Shader* sky_shader;
Model* ptr2, *ptr3, *ptr4, *ptr5, *ptr6, *ptr7, *ptr8, *ptr9, *ptr10, *ptr11, *ptr12, *ptr13, *ptr14;
unsigned int texture1;
unsigned int EBO;
unsigned int skyboxVAO, skyboxVBO;
unsigned int skymapTexture;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 Mov = glm::vec3(0.9f, 0.0f, 0.4f);
glm::vec3 Mov_1 = glm::vec3(0.8f, 0.0f, 0.3f);
glm::vec3 Mov_2 = glm::vec3(0.6f, -0.02f, 0.8); 
float r1 = 0.0f;
float r2 = 180.0f;
float s1 = -25.0f;

glm::vec3 lightpos_2 = glm::vec3(0.7f, 0.63f, -0.2f);
glm::vec3 lightpos_3 = glm::vec3(-1.1f, 0.63f, 0.27f);


bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

glm::vec3 Position;
glm::vec3 Front;
glm::vec3 Up;
glm::vec3 Right;
glm::vec3 WorldUp;

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLuint VAO, VBO;

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
	// positions   
	//values based on max of projection which is 100 in my case
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

vector<std::string> faces
{

	"..\\skybox\\posx.jpg",
	"..\\skybox\\negx.jpg",
	"..\\skybox\\posy.jpg",
	"..\\skybox\\negy.jpg",
	"..\\skybox\\posz.jpg",
	"..\\skybox\\negz.jpg"
};

void mouseMotionHandler(int x, int y)
{
	float xoffset = x - lastX;
	float yoffset = lastY - y;
	lastX = x;
	lastY = y;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0;
	if (pitch < -89.0f)
		pitch = -89.0;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = direction;
	Right = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
	cameraUp = glm::normalize(glm::cross(Right, cameraFront));
}

void keyPress(unsigned char key, int x, int y) {

	const float cameraSpeed = 0.05f; // adjust accordingly

	switch (key)
	{
	case 'w':
		cameraPos += cameraSpeed * cameraFront;
		break;

	case 's':
		cameraPos -= cameraSpeed * cameraFront;
		break;

	case 'd':
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case 'a':

		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		break;
	case 't':
		Mov.b += 0.01;
		glutPostRedisplay();
		break;
	case 'f':
		Mov.r -= 0.01;
		glutPostRedisplay();
		break;
	case 'g':
		Mov.b -= 0.01;
		glutPostRedisplay();
		break;
	case 'h':
		Mov.r += 0.01;
		glutPostRedisplay();
		break;
	case 'i':
		Mov_1.b -= 0.01;
		glutPostRedisplay();
		break;
	case 'j':
		Mov_1.r -= 0.01;
		glutPostRedisplay();
		break;
	case 'k':
		Mov_1.b += 0.01;
		glutPostRedisplay();
		break;
	case 'l':
		Mov_1.r += 0.01;
		glutPostRedisplay();
		break;
	case 'r':
		r1 += 0.7;
		glutPostRedisplay();
		break;
	case 'y':
		r1 -= 0.7;
		glutPostRedisplay();
		break;
	case 'u':
		r2 += 0.7;
		glutPostRedisplay();
		break;
	case 'o':
		r2 -= 0.7;
		glutPostRedisplay();
		break;
	case 'b':
		s1 -= 0.7;
		glutPostRedisplay();
		break;
	case 'n':
		s1 += 0.7;
		glutPostRedisplay();
		break;
	case 'x':
		Mov_2.b += 0.01;
		glutPostRedisplay();
		break;
	case 'z':
		Mov_2.r -= 0.01;
		glutPostRedisplay();
		break;
	case 'c':
		Mov_2.b -= 0.01;
		glutPostRedisplay();
		break;
	case 'v':
		Mov_2.r += 0.01;
		glutPostRedisplay();
		break;

	default:
		break;

	}
}


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

//Initialise the snowflake particles
void createParticles(int i) {
	par_sys[i].alive = true;
	par_sys[i].life = 2.0;
	par_sys[i].fade = float(rand() % 200) / 1000.0f + 0.003f;

	par_sys[i].xpos = (float)(rand() % 20) - 10;
	par_sys[i].ypos = 8.0;
	par_sys[i].zpos = (float)(rand() % 10) + 35;

	par_sys[i].red = 0.5;
	par_sys[i].green = 0.5;
	par_sys[i].blue = 1.0;

	par_sys[i].vel = velocity;
	par_sys[i].gravity = -0.8;//-0.8;

}

// For Snowflake falling down and loading the model
void drawSnow() {
	float x, y, z;
	for (int loop = 0; loop < MAX_PARTICLES; loop = loop + 2) {
		if (par_sys[loop].alive == true) {
			x = par_sys[loop].xpos;
			y = par_sys[loop].ypos;
			z = par_sys[loop].zpos + zoom;
			
			ptr1->setVec3("viewPos", cameraPos);
			glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			ptr1->setMat4("projection", projection);
			glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			ptr1->setMat4("view", view);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(x,y,z)); 
			model = glm::scale(model, glm::vec3(1.0f/1000));
			ptr1->setMat4("model", model);
			ptr6->Draw(ptr1);
			

			// Update values
			//Move
			par_sys[loop].ypos += par_sys[loop].vel / (slowdown * 1000);
			par_sys[loop].vel += par_sys[loop].gravity;
			// Decay
			par_sys[loop].life -= par_sys[loop].fade;

			if (par_sys[loop].ypos <= -10) {
				par_sys[loop].life = -1.0;
			}

			//Revive 
			if (par_sys[loop].life < 0.0) {
				createParticles(loop);
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
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
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

int main(int argc, char** argv) {

	// 窗口初始化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Final Project Submission - 21355130");
	
	// 告诉glut显示函数在哪里
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutMotionFunc(mouseMotionHandler);


	// 对glewInit()的调用必须在glut初始化之后完成
	GLenum res = glewInit();
	// 检查错误
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// 设置所有对象和着色器
	ptr1 = new Shader("..\\VertexS.vert", "..\\FragmentS1.frag");
	b_shader = new Shader("..\\b_vert.vert", "..\\b_frag.frag");
	sky_shader = new Shader("..\\box_shader.vert", "..\\box_fragment.frag");
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

	// 光源设置
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), 0);

	// 天空盒设置
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	skymapTexture = loadskymap(faces);

	// 初始化例子
	for (int loop = 0; loop < MAX_PARTICLES; loop++) {
		createParticles(loop);
	}

	glutMainLoop();
	return 0;
}











