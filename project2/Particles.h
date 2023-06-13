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
	// 生命
	bool alive;   // 粒子存活状态
	float life;   // 粒子的寿命
	float fade;   // 粒子衰减速度
	// 颜色
	float red;
	float green;
	float blue;
	// 位置/方向
	float xpos;
	float ypos;
	float zpos;
	// 速度/方向，只在y方向上运动
	float vel;
	// 重力
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

// 初始化雪花粒子
void createParticles();

// 为单个粒子重新初始化
void respawnByIndex(int index);

// 为所有粒子设置存活状态
void setAlive(bool alive);

// 为所有粒子设置寿命
void setLife(float life);

// 为所有粒子设置衰减速度
void setFade(int fadeMAX);

// 为所有粒子设置重力
void setGravity(float gravity);

// 更新粒子
void updateParticles();

void drawSnow();

#endif // PARTICLES_H