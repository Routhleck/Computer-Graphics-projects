#ifndef INPUT_H
#define INPUT_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm/glm.hpp>

extern float yaw;
extern float pitch;

extern bool keyState[256];  // 存储键盘按键状态的数组

extern float sensitivity;  // 鼠标移动灵敏度

extern int window_width;   // 窗口宽度
extern int window_height;  // 窗口高度

extern int lastMouseX;  // 上一次鼠标的X位置
extern int lastMouseY;  // 上一次鼠标的Y位置

extern glm::vec3 cameraPos;   // 摄像机位置
extern glm::vec3 cameraFront; // 摄像机前方向
extern glm::vec3 Right;       // 右方向向量
extern glm::vec3 cameraUp;    // 上方向向量

// 回调函数
void mouseMotionHandler(int x, int y);            // 处理鼠标移动事件
void keyPressRelease(unsigned char key, int x, int y, bool isPressed);  // 处理键盘按下释放事件

// 更新摄像机的位置
void updateCamera();

// 注册键盘相关的函数
void registerKeyBoardFunc();

#endif // INPUT_H