#include "input.h"
#include "imgui.h"
#include "Particles.h"


void mouseMotionHandler(int x, int y) {
	if (isCursor) {
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(static_cast<float>(x), static_cast<float>(y));
		return;
	}
	float xOffset = (x - lastMouseX) * sensitivity;
	float yOffset = (lastMouseY - y) * sensitivity;

	lastMouseX = x;
	lastMouseY = y;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	Right = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
	cameraUp = glm::normalize(glm::cross(Right, cameraFront));
}

void mouseButtonHandler(int button, int state, int x, int y) {
	if (isCursor) {
		ImGuiIO& io = ImGui::GetIO();
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) {
				io.MouseDown[0] = true;
			}
			else if (state == GLUT_UP) {
				io.MouseDown[0] = false;
			}
		}
	}
}

void keyPressRelease(unsigned char key, int x, int y, bool isPressed) {
	if (isPressed) {
		keyState[key] = true;
		if (key == 27) {
			if (isCursor) {
				glutSetCursor(GLUT_CURSOR_NONE);
				isCursor = false;
				// 更新粒子信息
				updateParticles();
	
			}
			else {
				glutSetCursor(GLUT_CURSOR_INHERIT);
				isCursor = true;
			}
		}
		if (key == 'f' || key == 'F') {
			if (isFullScreen) {
				glutReshapeWindow(window_width, window_height);
				isFullScreen = false;
			}
			else {
				glutFullScreen();
				isFullScreen = true;
			}
		}
	}
	else {
		keyState[key] = false;
	}
}

void updateCamera() {
	if (isCursor) return;

	if (keyState['w'] && keyState['d']) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		cameraPos += cameraSpeed * cameraFront;
	}
	else if (keyState['w'] && keyState['a']) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		cameraPos += cameraSpeed * cameraFront;
	}
	else if (keyState['s'] && keyState['d']) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		cameraPos -= cameraSpeed * cameraFront;
	}
	else if (keyState['s'] && keyState['a']) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		cameraPos -= cameraSpeed * cameraFront;
	}
	else if (keyState['w']) {
		cameraPos += cameraSpeed * cameraFront;
	}
	else if (keyState['s']) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	else if (keyState['d']) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	else if (keyState['a']) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}

void registerKeyBoardFunc() {
	glutKeyboardFunc([](unsigned char key, int x, int y) {
		keyPressRelease(key, x, y, true);
	});

	glutKeyboardUpFunc([](unsigned char key, int x, int y) {
		keyPressRelease(key, x, y, false);
	});
}
