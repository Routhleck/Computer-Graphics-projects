#include "Gui.h"

void renderGUI() {
	// ���� OpenGL ��Ⱦ״̬
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// ��ʼ�µ� ImGui ֡
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();
	ImGui::NewFrame();

	// ���ý���ߴ�
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(window_width), static_cast<float>(window_height));

	// ��������Ⱦ GUI ����Ĵ���...
	if (ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)) {
		// ��� "Continue" ��ť
		if (ImGui::Button("Continue")) {
			// ������ "Continue" ��ť���߼�
			isCursor = false;
		}

		// ������Ͳ���
		if (ImGui::CollapsingHeader("Camera parameter")) {
			// ��� Type 1 ��Ӧ�Ĳ����ؼ�
			ImGui::SliderFloat("Sensitivity", &sensitivity, 0.1f, 1.0f);
			ImGui::SliderFloat("Camera Speed", &cameraSpeed, 0.01f, 0.50f);
			ImGui::SliderFloat("FOV", &fov, 15.0f, 120.0f);
			// ��Ӹ���Ĳ����ؼ�...
		}

		if (ImGui::CollapsingHeader("Snow Particles")) {
			// ��� Type 2 ��Ӧ�Ĳ����ؼ�
			ImGui::Checkbox("Alive", &particles_alive);
			ImGui::SliderInt("Max particles", &MAX_PARTICLES, 0, 5000);
			ImGui::SliderFloat("Life", &particles_life, 0.0f, 10.0f);
			ImGui::SliderInt("Max Fade Factor", &particles_fadeMax, 0, 2000);
			ImGui::SliderFloat("Gravity", &particles_gravity, -2.0f, 1.0f);
			ImGui::SliderFloat("Zoom", &zoom, -60.0f, 60.0f);
			// ��Ӹ���Ĳ����ؼ�...
		}

		// ��� "Exit" ��ť
		if (ImGui::Button("Exit")) {
			// ������ "Exit" ��ť���߼�
			exit(0);
		}
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}



// ��ʼ��ImGui����ص�������
void initGUI(void*) {
	// ��ʼ�� ImGui ������
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// ���� ImGui ��Ⱦ���Ļص�����
	ImGui_ImplOpenGL2_Init();
	ImGui_ImplGLUT_Init();

	// ���� ImGui ����ʾ�ߴ�
	io.DisplaySize.x = static_cast<float>(window_width);
	io.DisplaySize.y = static_cast<float>(window_height);
}


// ����ImGui����ص�������
void cleanupGUI() {
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
}
