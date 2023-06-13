#ifndef INPUT_H
#define INPUT_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm/glm.hpp>

extern float yaw;
extern float pitch;

extern bool keyState[256];  // �洢���̰���״̬������

extern float sensitivity;  // ����ƶ�������

extern int window_width;   // ���ڿ���
extern int window_height;  // ���ڸ߶�

extern int lastMouseX;  // ��һ������Xλ��
extern int lastMouseY;  // ��һ������Yλ��

extern glm::vec3 cameraPos;   // �����λ��
extern glm::vec3 cameraFront; // �����ǰ����
extern glm::vec3 Right;       // �ҷ�������
extern glm::vec3 cameraUp;    // �Ϸ�������

// �ص�����
void mouseMotionHandler(int x, int y);            // ��������ƶ��¼�
void keyPressRelease(unsigned char key, int x, int y, bool isPressed);  // �������̰����ͷ��¼�

// �����������λ��
void updateCamera();

// ע�������صĺ���
void registerKeyBoardFunc();

#endif // INPUT_H