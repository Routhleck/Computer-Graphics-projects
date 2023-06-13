#include "input.h"


void mouseMotionHandler(int x, int y) {
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

void keyPressRelease(unsigned char key, int x, int y, bool isPressed) {
	if (isPressed) {
		keyState[key] = true;
		if (key == 27) {
			exit(0);
		}
	}
	else {
		keyState[key] = false;
	}
}

void updateCamera() {
	const float cameraSpeed = 0.05f;

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
