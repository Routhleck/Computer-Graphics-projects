#pragma once
#ifndef GUI_H
#define GUI_H
#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Particles.h"

extern int window_width;
extern int window_height;

extern bool isCursor;

extern float sensitivity;
extern float fov;
extern float cameraSpeed;

extern int MAX_PARTICLES;
extern bool particles_alive;
extern float particles_life;
extern int particles_fadeMax;
extern float particles_gravity;
extern float zoom;

extern bool fog_display;
extern float fog_maxdist;
extern float fog_mindist;
extern float fog_colour_red;
extern float fog_colour_green;
extern float fog_colour_blue ;

extern bool ambientLight_enable;
extern float ambientLight_strength;
extern bool diffuseLight_enable;
extern bool specularLight_enable;
extern float kd1_red;
extern float kd1_green;
extern float kd1_blue;
extern float kd2_red;
extern float kd2_green;
extern float kd2_blue;
extern float kd3_red;
extern float kd3_green;
extern float kd3_blue;
extern float ld1_red;
extern float ld1_green;
extern float ld1_blue;
extern float ld2_red;
extern float ld2_green;
extern float ld2_blue;
extern float ld3_red;
extern float ld3_green;
extern float ld3_blue;
extern float ld4_red;
extern float ld4_green;
extern float ld4_blue;

extern bool shadows;
extern bool isSingleLight;
extern float lightPos_x;
extern float lightPos_y;
extern float lightPos_z;

void renderGUI();
void initGUI(void* window);
void cleanupGUI();








#endif
