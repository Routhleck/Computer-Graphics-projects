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

void renderGUI();
void initGUI(void* window);
void cleanupGUI();








#endif
