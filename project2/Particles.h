#pragma once
#ifndef PARTICLES_H
#define PARTICLES_H

#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


typedef struct {
	// ����
	bool alive;   // ���Ӵ��״̬
	float life;   // ���ӵ�����
	float fade;   // ����˥���ٶ�
	// ��ɫ
	float red;
	float green;
	float blue;
	// λ��/����
	float xpos;
	float ypos;
	float zpos;
	// �ٶ�/����ֻ��y�������˶�
	float vel;
	// ����
	float gravity;
}particles;

extern int window_width;
extern int window_height;

extern std::vector<particles> par_sys;

extern float zoom;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern float fov;


extern int MAX_PARTICLES;
extern bool particles_alive;
extern float particles_life;
extern int particles_fadeMax;
extern float particles_gravity;
extern float particles_slowdown;

// ��ʼ��ѩ������
void createParticles();

// Ϊ�����������³�ʼ��
void respawnByIndex(int index);

// Ϊ�����������ô��״̬
void setAlive(bool alive);

// Ϊ����������������
void setLife(float life);

// Ϊ������������˥���ٶ�
void setFade(int fadeMAX);

// Ϊ����������������
void setGravity(float gravity);

// ��������
void updateParticles();

void drawSnow();

#endif // PARTICLES_H